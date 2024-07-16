#include "tetrisboard.h"

TetrisBoard::TetrisBoard(QWidget *parent)
    : QFrame(parent)
    , board_width_steps_(1)
    , board_height_steps_(1)
    , square_side_(1) // px
    , is_started_(false)
    , is_paused_(false)
    , score_(0)
    , level_up_range_(2400)
    , not_active_alpha_color_(100)
    , active_alpha_color_(255)
    , board_(QVector<TetrisShape>(board_width_steps_*board_height_steps_))
{
    // Set some default properties for the frame
    setFrameStyle(QFrame::StyledPanel | QFrame::Plain);

    // Needed to recognize event in frame from used (i.e., key pressed)
    setFocusPolicy(Qt::StrongFocus);

    clearBoard();
    next_piece_.setRandomShape();
    // qDebug() << "Initialization completed: piece: " << int(next_piece_.shape()) ;
}

/**
 * @brief Handles the painting of the Tetris board and game state visuals.
 *
 * Overrides the default paintEvent function to draw the Tetris board, Tetris pieces,
 * game messages, and background grid. Depending on the game state (started, paused, lost),
 * different elements are drawn or updated accordingly. The method uses QPainter to render
 * graphics on the widget surface.
 *
 * @param event Pointer to the QPaintEvent object representing the paint event.
 */
void TetrisBoard::paintEvent(QPaintEvent *event)
{
    // qDebug() << "Called paintEvent" ;
    QFrame::paintEvent(event);

    QPainter painter(this);
    QRect rect = contentsRect();
    int alpha_color = active_alpha_color_;

    QFont text_font("Arial", 15, QFont::Bold);
    QTextOption text_option;
    text_option.setWrapMode(QTextOption::WordWrap);
    text_option.setAlignment(Qt::AlignCenter);

    painter.setFont(text_font);
    if (!is_started_) {
        painter.drawText(rect, "Welcome", text_option);
        return;
    } else if(is_paused_){
        alpha_color = not_active_alpha_color_;
    } else if(is_lost_){
        alpha_color = not_active_alpha_color_;
        QString test = "test";
        emit updateScores(score_, test);
    }

    drawBackgroundGrid(painter, alpha_color);

    drawPlacedPieces(painter, alpha_color);

    drawCurrentPiece(painter, alpha_color);

    // To draw on top of everything
    painter.setPen(Qt::black);
    if(is_paused_ && !is_lost_)
        painter.drawText(rect, Qt::AlignCenter, "Pause");
    else if(is_lost_)
        painter.drawText(rect, Qt::AlignCenter, "Ouch, you lost ...");

    // qDebug() << "paintEvent completed" ;
}

/**
 * @brief Handles timer events for the Tetris game.
 *
 * Overrides the default timerEvent function. If the timer event is triggered
 * it calls the `oneLineDown()` method to move the current Tetris piece
 * down one line. If the timer event is not from the game timer, it delegates the event
 * handling to the base class (QFrame) implementation.
 *
 * @param event Pointer to the QTimerEvent object representing the timer event.
 */
void TetrisBoard::timerEvent(QTimerEvent *event){
    if(event->timerId() == timer_.timerId()){
        // std::cout << "Timout event passed" << std::endl;
        oneLineDown();
    } else {
        QFrame::timerEvent(event);
    }
}

/**
 * @brief Handles key release events for controlling the Tetris game.
 *
 * Overrides the default keyReleaseEvent function. Controls include releasing the space
 * key to return to normal speed after speeding up the descent of the current Tetris piece.
 * If the game is not started, paused, or there is no current piece, the event is delegated
 * to the base class (QFrame) implementation.
 *
 * @param event Pointer to the QKeyEvent object representing the key release event.
 */
void TetrisBoard::keyReleaseEvent(QKeyEvent *event)
{
    if (!is_started_ || is_paused_ || curr_piece_.shape() == NoShape) {
        QFrame::keyReleaseEvent(event);
        return;
    }

    // Managing hold button
    if(event->isAutoRepeat())
        return;


    switch (event->key()) {
    case Qt::Key_Space:
        // std::cout << "SPACE RELEASED." << std::endl;
        backToNormalSpeed();
        break;
    default:
        QFrame::keyReleaseEvent(event);
    }
}

/**
 * @brief Handles key press events for controlling the Tetris game.
 *
 * Overrides the default keyPressEvent. Controls include moving the current Tetris piece
 * left, right, rotating it left or right, and speeding up its descent. If the game is not started,
 * paused, or there is no current piece, the event is delegated to the base class
 * (QFrame) implementation.
 *
 * @param event Pointer to the QKeyEvent object representing the key press event.
 */
void TetrisBoard::keyPressEvent(QKeyEvent *event)
{
    if (!is_started_ || is_paused_ || curr_piece_.shape() == NoShape) {
        QFrame::keyPressEvent(event);
        return;
    }

    // Managing hold button
    if(event->isAutoRepeat() && event->key() == Qt::Key_Space)
        return;

    switch (event->key()) {
    case Qt::Key_Left:
        // std::cout << "LEFT" << std::endl;
        tryMove(curr_piece_, curr_x_ - 1, curr_y_);
        break;
    case Qt::Key_Right:
        // std::cout << "RIGHT" << std::endl;
        tryMove(curr_piece_, curr_x_ + 1, curr_y_);
        break;
    case Qt::Key_Up:
        // std::cout << "ROTATING LEFT" << std::endl;
        tryMove(curr_piece_.rotatedLeft(), curr_x_, curr_y_);
        break;
    case Qt::Key_Down:
        // std::cout << "ROTATING RIGHT" << std::endl;
        tryMove(curr_piece_.rotatedRight(), curr_x_, curr_y_);
        break;
    case Qt::Key_Space:
        // std::cout << "SPACE PRESSED. " << std::endl;
        speedUp();
        break;
    default:
        QFrame::keyPressEvent(event);
    }
}

/**
 * @brief Draws a single Tetris square at the specified position.
 *
 * Draws a square representing a Tetris piece at the specified coordinates (x, y) on the board
 * using the provided QPainter object. The color and appearance of the square are determined by
 * the shape parameter, with an optional alpha color value for transparency.
 *
 * @param painter Reference to the QPainter object used for drawing.
 * @param x X-coordinate of the top-left corner of the square.
 * @param y Y-coordinate of the top-left corner of the square.
 * @param shape Enum value representing the shape and color of the Tetris square.
 * @param alpha_color Optional alpha color value for transparency effect (default: 255, fully opaque).
 */
void TetrisBoard::drawSquare(QPainter &painter, int x, int y, TetrisShape shape, int alpha_color = 255)
{
    static constexpr QRgb colorTable[8] = {
        0x000000, 0xCC6666, 0x66CC66, 0x6666CC,
        0xCCCC66, 0xCC66CC, 0x66CCCC, 0xDAAA00
    };

    QColor color = QColor::fromRgb(colorTable[int(shape)]);
    color.setAlpha(alpha_color);

    painter.fillRect(x + 1, y + 1, square_side_ - 2, square_side_ - 2,
                     color);

    painter.setPen(color.lighter());
    painter.drawLine(x, y + square_side_ - 1, x, y);
    painter.drawLine(x, y, x + square_side_ - 1, y);

    painter.setPen(color.darker());

    painter.drawLine(x + 1, y + square_side_ - 1,
                     x + square_side_ - 1, y + square_side_ - 1);

    painter.drawLine(x + square_side_ - 1, y + square_side_ - 1,
                     x + square_side_ - 1, y + 1);
}

/**
 * @brief Draws the background grid of the Tetris board.
 *
 * Draws a grid pattern as the background of the Tetris board using the provided QPainter object.
 * The grid consists of vertical and horizontal lines spaced according to the square side length,
 * with an optional alpha color value for transparency.
 *
 * @param painter Reference to the QPainter object used for drawing.
 * @param alpha_color Optional alpha color value for transparency effect (default: 255, fully opaque).
 */
void TetrisBoard::drawBackgroundGrid(QPainter &painter, int alpha_color = 255)
{
    // qDebug() << "grid background started" ;
    QRect rect = contentsRect();
    QColor color = Qt::lightGray;
    color.setAlpha(alpha_color);

    painter.setPen(QPen(color, 1, Qt::SolidLine));

    // Draw vertical lines
    for (int i = 1; i < board_width_steps_; ++i) {
        int x = rect.left() + i * square_side_;
        painter.drawLine(x, rect.top(), x, rect.bottom());
    }

    // // Draw horizontal lines
    for (int i = 1; i < board_height_steps_; ++i) {
        int y = rect.top() + i * square_side_;
        painter.drawLine(rect.left(), y, rect.right(), y);
    }

    // qDebug() << "grid background completed" ;
}

/**
 * @brief Draws all placed pieces on the board.
 *
 * Draws squares representing all placed Tetris pieces on the board using the provided QPainter object.
 * The pieces are drawn based on their stored positions in the board matrix, applying an optional alpha
 * color effect for transparency.
 *
 * @param painter Reference to the QPainter object used for drawing.
 * @param alpha_color Optional alpha color value for transparency effect.
 */
void TetrisBoard::drawPlacedPieces(QPainter &painter, int alpha_color)
{
    QRect rect = contentsRect();

    // qDebug() << "Drawing OLD pieces" ;
    // Drawing back all past squares (stored in board_)
    for(int i = 0; i<board_height_steps_; ++i){
        for (int j = 0; j<board_width_steps_; ++j){
            TetrisShape shape = shapeAt(j, i);
            if (shape != NoShape){
                drawSquare(painter, rect.left() + j * square_side_,
                           rect.top() + i * square_side_,
                           shape, alpha_color);
            }
        }
    }
}

/**
 * @brief Draws the current piece on the board.
 *
 * Draws the squares of the current piece onto the board using the provided QPainter object.
 * The piece is drawn at its current position on the board, applying an optional alpha color effect.
 *
 * @param painter Reference to the QPainter object used for drawing.
 * @param alpha_color Optional alpha color value for transparency effect.
 */
void TetrisBoard::drawCurrentPiece(QPainter &painter, int alpha_color)
{
    QRect rect = contentsRect();

    // qDebug() << "Drawing NEW piece" ;
    // Drawing new piece
    if (curr_piece_.shape() != NoShape) {
        for (int i = 0; i < 4; ++i) {
            int x = curr_x_ + curr_piece_.x(i);
            int y = curr_y_ + curr_piece_.y(i);
            drawSquare(painter, rect.left() + x * square_side_,
                       rect.top() + y*square_side_,
                       curr_piece_.shape(), alpha_color);
        }
    }
}

/**
 * @brief Display a new piece in the game.
 *
 * Sets the current piece to be the previously shown next piece, generates a new random piece
 * for the next piece preview, updates and displays the next piece preview on the QLabel widget,
 * positions the current piece in the middle of the board's width, and attempts to place it at
 * the top of the board. If the new piece cannot be placed due to lack of space, the game is
 * considered lost, the current piece is set to NoShape, and the game stops with a signal emitted.
 */
void TetrisBoard::newPiece()
{
    curr_piece_ = next_piece_;
    next_piece_.setRandomShape();
    // qDebug() << "[NEWPIECE] Current piece: " << curr_piece_.shape();
    // qDebug() << "[NEWPIECE] Next selected piece: " << next_piece_.shape();
    showNextPiece();
    curr_x_ = board_width_steps_ / 2;
    curr_y_ = qAbs(curr_piece_.minY());

    // qDebug() << "[NEWPIECE] Trying positioning new piece top. Curr coords: " << curr_x_ << ", " << curr_y_;
    if (!tryMove(curr_piece_, curr_x_, curr_y_)) {
        // qDebug() << "New piece no have space, setting noShape.";
        curr_piece_.setShape(NoShape);
        is_lost_ = true;
        timer_.stop();
        emit gameLost(score_);
        update();
    }
    // qDebug() << "NewPiece completed.";
}

/**
 * @brief Sets the QLabel widget to display the next piece preview.
 *
 * Assigns the provided QLabel pointer to `next_piece_label_`, which is used
 * to show the preview of the next piece to be played in the Tetris game.
 *
 * @param label Pointer to the QLabel widget used for displaying the next piece preview.
 */
void TetrisBoard::setNextPieceLabel(QLabel *label)
{
    next_piece_label_ = label;
}

/**
 * @brief Displays the next piece preview on a QLabel widget.
 *
 * Retrieves the next piece to be played and calculates its dimensions.
 * Creates a pixmap to draw the piece preview, fills it with the window's palette color,
 * and draws each square of the piece onto the pixmap. Sets the pixmap to the QLabel
 * for display, centered within the label.
 */
void TetrisBoard::showNextPiece()
{
    if(!next_piece_label_)
        return;

    int dx  = next_piece_.maxX() - next_piece_.minX() + 1;
    int dy  = next_piece_.maxY() - next_piece_.minY() + 1;

    QPixmap pixmap(dx*square_side_, dy*square_side_);
    QPainter painter(&pixmap);
    painter.fillRect(pixmap.rect(), next_piece_label_->palette().window());

    for(int i = 0; i < 4; i++){
        int x = next_piece_.x(i) - next_piece_.minX();
        int y = next_piece_.y(i) - next_piece_.minY();

        drawSquare(painter, x*square_side_, y*square_side_, next_piece_.shape());
    }

    next_piece_label_->setPixmap(pixmap);
    next_piece_label_->setAlignment(Qt::AlignCenter);
}

/**
 * @brief Moves the current piece one line down if possible.
 *
 * Attempts to move the current piece one line down. If the move is successful,
 * the piece's position is updated. If the move fails (piece cannot move down further),
 * the pieceDropped() method is called to finalize the piece's position on the board.
 */
void TetrisBoard::oneLineDown()
{
    if(!tryMove(curr_piece_, curr_x_, curr_y_ + 1))
        pieceDropped();
}

/**
 * @brief Attempts to move a piece to a new position on the board.
 *
 * Checks if the new position for the piece is within the board boundaries and does not
 * overlap with existing pieces on the board. If the move is valid, updates the current
 * piece and its position, and triggers a board update.
 *
 * @param new_piece The piece to move.
 * @param new_x The new x-coordinate for the piece.
 * @param new_y The new y-coordinate for the piece.
 * @return true if the piece can be moved to the new position, false otherwise.
 */
bool TetrisBoard::tryMove(const TetrisPiece &new_piece, int new_x, int new_y)
{
    // qDebug() << "TryMove starting." ;
    for(int i  = 0; i<4; ++i){
        int x = new_piece.x(i) + new_x;
        int y = new_piece.y(i) + new_y;

        // qDebug() << "[tryMove] X and Y: " << new_piece.x(i) << ", " << new_piece.y(i) << " -- " << new_x << ", " << new_y;
        if(x < 0 || x >= board_width_steps_ || y < 0 || y >= board_height_steps_)
            return false;

        if (shapeAt(x, y) != NoShape)
            return false;
    }

    // qDebug() << "TryMove all pieces have space." ;
    curr_piece_ = new_piece;
    curr_x_ = new_x;
    curr_y_ = new_y;
    update();

    // qDebug() << "TryMove completed." ;
    return true;
}

/**
 * @brief Handles actions after a piece has been dropped.
 *
 * Places the current piece on the board, increments the count of dropped pieces,
 * increases the score, updates the score display, checks for level up conditions,
 * removes full lines from the board, and spawns a new piece.
 */
void TetrisBoard::pieceDropped()
{
    for (int i = 0; i < 4; ++i) {
        int x = curr_x_ + curr_piece_.x(i);
        int y = curr_y_ + curr_piece_.y(i);
        // qDebug() << "X AND Y: " << x << ", " << y;
        shapeAt(x, y) = curr_piece_.shape();
    }

    ++num_piece_dropped_;
    score_+=10;

    emit updateScoreLcd(score_);
    if(score_>best_score_)
        emit updateBestScoreLcd(score_);


    if(num_piece_dropped_ % 25 == 0)
        levelUp();

    removeFullLines();
    newPiece();
}

/**
 * @brief Removes full lines from the board and updates the score.
 *
 * Checks each line on the board to see if it is full. If a line is full, it is removed
 * and all lines above it are moved down. The score is updated based on the number of
 * lines removed, and the level is increased if necessary.
 */
void TetrisBoard::removeFullLines()
{
    int num_full_lines = 0;

    for(int i = 0; i < board_height_steps_; i++){
        bool is_line_full = true;

        for(int j = 0; j < board_width_steps_; j++){
            if(shapeAt(j, i) == TetrisShape::NoShape){
                is_line_full = false;
                break;
            }
        }

        // Counting
        if(is_line_full){
            num_full_lines++;
            // qDebug() << "Line is full, REMOVE LINE: " << i;
            // Move all line from 0 to actual found one
            for(int y = i; y>=1; y--){
                for(int x = 0; x < board_width_steps_; x++){
                    shapeAt(x, y) = shapeAt(x, y-1);
                }
            }
        }

        if(num_full_lines > 0){
            // Increase level each level_up_range_ points
            int prev_score_range = score_/level_up_range_;
            updateScore(num_full_lines);
            int curr_score_range = score_/level_up_range_;
            if(curr_score_range>prev_score_range)
                levelUp();

            emit updateScoreLcd(score_);
            if(score_ > best_score_)
                emit updateBestScoreLcd(score_);

        }
    }
}

/**
 * @brief Clears the Tetris board by resetting all board squares to NoShape.
 *
 * Resets the board by setting each square in the board array to NoShape,
 * indicating that no Tetris piece occupies that square. This method prepares the board
 * for a new game or after the game has ended.
 */
void TetrisBoard::clearBoard()
{
    // qDebug() << "clearBoard started" ;
    for (int i = 0; i < board_height_steps_ * board_width_steps_; ++i)
        board_[i] = NoShape;

    // qDebug() << "clearBoard completed" ;
}

/**
 * @brief Increases the game speed by reducing the timeout interval.
 *
 * Decreases the timeout interval (`timeout_time_`) by 20% of its current value,
 * which effectively speeds up the falling of Tetris pieces. Restarts the timer
 * with the updated timeout interval.
 */
void TetrisBoard::levelUp()
{
    timeout_time_ -= qRound( static_cast<float>(20 * timeout_time_) / 100 );
    timer_.stop();
    timer_.start(timeout_time_, this);
    // std::cout << "Leveled up, timeout = " <<  timeout_time_ << std::endl;
}

/**
 * @brief Updates the score based on the number of lines removed.
 *
 * Adds points to the score based on the number of lines removed in a single move.
 *
 * @param lines_removed The number of lines removed.
 */
void TetrisBoard::updateScore(const int lines_removed)
{
    switch (lines_removed) {
    case 1:
        score_+=40;
        break;
    case 2:
        score_+=100;
        break;
    case 3:
        score_+=300;
        break;
    case 4:
        score_+=1200;
        break;
    }
}

/**
 * @brief Calculates and returns the size of the game board.
 *
 * Computes the total width and height of the game board including squares and frame borders,
 * based on the square side length, number of steps in width and height, and the frame width.
 *
 * @return QSize object representing the size of the game board.
 */
QSize TetrisBoard::getBoardSize(){
    // qDebug() << "getBoardSize: [Width, Height]: " << square_side_*board_width_steps_ + 2*frameWidth() << "," <<square_side_*board_height_steps_ + 2*frameWidth();
    return QSize(square_side_*board_width_steps_ + 2*frameWidth(),
                 square_side_*board_height_steps_ + 2*frameWidth());
}

/**
 * @brief Sets the size of the game board.
 *
 * Resizes the board to the given dimensions, computes the number of squares
 * in width and height and adjusts the widget size to match an integer multiple
 * of squares.
 *
 * @param board_size The desired size of the board.
 */
void TetrisBoard::setBoardSize(QSize board_size)
{
    // qDebug() << "setBoardSize started" ;
    // Resizing with available dimension in the layout
    resize(board_size);

    // Computing, given square side, number of squares per width and height
    board_width_steps_ = contentsRect().width() / square_side_;
    board_height_steps_ = contentsRect().height() / square_side_;

    // Resizing to have a int multiple of squares as actual widget size
    setFixedSize(getBoardSize());

    // Updating all variable linked to size
    board_.resize(board_width_steps_ * board_height_steps_);
    clearBoard();

    // qDebug() << "setBoardSize completed" ;
}

/**
 * @brief Starts a new game.
 *
 * Initializes game state, resets the timeout, updates the score display,
 * clears the board, spawns a new piece, and starts the game timer.
 */
void TetrisBoard::start()
{
    is_started_ = true;
    is_lost_ = false;

    resetTimeout();

    num_piece_dropped_ = 0;

    emit updateScoreLcd(score_);

    clearBoard();
    newPiece();
    timer_.start(timeout_time_, this);
    // std::cout << "Game logic has started. Timer started" << std::endl;
}

/**
 * @brief Resets the game state.
 *
 * This method resets the game to its initial state. It stops the game logic,
 * sets the game as not started and not paused, and resets the score.
 */
void TetrisBoard::reset()
{
    is_started_ = false;
    is_paused_ = false;
    score_ = 0;
    timer_.stop();
    // std::cout << "Game logic has stopped." << std::endl;
}

/**
 * @brief Pauses the ongoing game.
 *
 * This method pauses the game if it is currently started. It stops the game timer
 * and updates the game state.
 */
void TetrisBoard::pause()
{
    if(!is_started_)
        return;

    is_paused_ = true;
    timer_.stop();
    // std::cout << "Game has paused" << std::endl;
    update();
}

/**
 * @brief Resumes the paused game.
 *
 * This method resumes the game if it is currently paused. It restarts the game timer
 * and updates the game state.
 */
void TetrisBoard::resume()
{
    if(!is_paused_)
        return;

    is_paused_ = false;
    timer_.start(timeout_time_, this);
    // std::cout << "Game has resumed after paused." << std::endl;
    update();
}
