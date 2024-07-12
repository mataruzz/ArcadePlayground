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

void TetrisBoard::resetTimeout(){
    timeout_time_ = 700;
}

void TetrisBoard::paintEvent(QPaintEvent *event)
{
    // qDebug() << "Called paintEvent" ;
    QFrame::paintEvent(event);

    QPainter painter(this);
    QRect rect = contentsRect();
    int alpha_color = active_alpha_color_;

    // QFont original_font = painter.font();
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

void TetrisBoard::clearBoard()
{
    // qDebug() << "clearBoard started" ;
    for (int i = 0; i < board_height_steps_ * board_width_steps_; ++i)
        board_[i] = NoShape;

    // qDebug() << "clearBoard completed" ;
}

void TetrisBoard::keyReleaseEvent(QKeyEvent *event){
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


void TetrisBoard::speedUp(){
    timer_.start(50, this);
}

void TetrisBoard::backToNormalSpeed(){
    timer_.start(timeout_time_, this);
}

void TetrisBoard::levelUp(){
    timeout_time_ -= qRound( static_cast<float>(20 * timeout_time_) / 100 );
    timer_.stop();
    timer_.start(timeout_time_, this);
    std::cout << "Leveled up, timeout = " <<  timeout_time_ << std::endl;
}

void TetrisBoard::timerEvent(QTimerEvent *event){
    if(event->timerId() == timer_.timerId()){
        // std::cout << "Timout event passed" << std::endl;
        oneLineDown();
    } else {
        QFrame::timerEvent(event);
    }
}

void TetrisBoard::drawSquare(QPainter &painter, int x, int y, TetrisShape shape, int alpha_color = 255){
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

void TetrisBoard::drawBackgroundGrid(QPainter &painter, int alpha_color = 255){

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

void TetrisBoard::drawPlacedPieces(QPainter &painter, int alpha_color)
{
    QRect rect = contentsRect();

    // qDebug() << "Drawing OLD pieces" ;
    // // Drawing back all past squares (stored in board_)
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

QSize TetrisBoard::getBoardSize(){
    // qDebug() << "getBoardSize: [Width, Height]: " << square_side_*board_width_steps_ + 2*frameWidth() << "," <<square_side_*board_height_steps_ + 2*frameWidth();

    return QSize(square_side_*board_width_steps_ + 2*frameWidth(),
                 square_side_*board_height_steps_ + 2*frameWidth());
}


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

void TetrisBoard::setNextPieceLabel(QLabel *label){
    next_piece_label_ = label;
}

void TetrisBoard::showNextPiece(){

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

void TetrisBoard::oneLineDown(){

    if(!tryMove(curr_piece_, curr_x_, curr_y_ + 1))
        pieceDropped();

}

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


void TetrisBoard::removeFullLines(){

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

// void TetrisBoard::dropDown(){
//     // need to move the piece till the bottom
//     while(tryMove(curr_piece_, curr_x_, curr_y_ + 1)){
//         // tryMove(curr_piece_, curr_x_, curr_y_ + 1);
//         num_piece_dropped_++;
//     }
// }

void TetrisBoard::updateScore(const int lines_removed){
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

void TetrisBoard::setBoardSize(QSize board_size){

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

void TetrisBoard::start(){
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

void TetrisBoard::reset(){
    is_started_ = false;
    is_paused_ = false;
    score_ = 0;
    timer_.stop();
    // std::cout << "Game logic has stopped." << std::endl;
}

void TetrisBoard::pause(){
    if(!is_started_)
        return;

    is_paused_ = true;
    timer_.stop();
    // std::cout << "Game has paused" << std::endl;
    update();
}

void TetrisBoard::resume(){
    if(!is_paused_)
        return;

    is_paused_ = false;
    timer_.start(timeout_time_, this);
    // std::cout << "Game has resumed after paused." << std::endl;
    update();
}
