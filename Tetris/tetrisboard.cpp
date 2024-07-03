#include "tetrisboard.h"

TetrisBoard::TetrisBoard(QWidget *parent)
    : QFrame(parent)
    , board_width_steps_(1)
    , board_height_steps_(1)
    , timeout_time_(100)
    , is_started_(false)
    , is_paused_(false)
    , square_side_(1)  // px
    , board_(QVector<TetrisShape>(board_width_steps_*board_height_steps_))
{

    // Set some default properties for the frame
    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(2);
    setMidLineWidth(0);

    // Needed to recognize event in frame from used (i.e., key pressed)
    setFocusPolicy(Qt::StrongFocus);

    clearBoard();
    next_piece_.setRandomShape();
    // next_piece_.setShape(TetrisShape::LineShape);
    qDebug() << "Initialization completed: piece: " << int(next_piece_.shape()) ;
}

void TetrisBoard::paintEvent(QPaintEvent *event)
{

    // qDebug() << "Called paintEvent" ;
    QFrame::paintEvent(event);

    QPainter painter(this);
    QRect rect = contentsRect();

    if (!is_started_) {
        painter.drawText(rect, Qt::AlignCenter, "Start New Game");
        return;
    } else if(is_paused_){
        painter.drawText(rect, Qt::AlignCenter, "Pause");
        return;
    }

    drawBackgroundGrid(painter);

    // qDebug() << "Drawing OLD piece" ;
    // // Drawing back all past squares (stored in board_)
    for(int i = 0; i<board_height_steps_; ++i){
        for (int j = 0; j<board_width_steps_; ++j){
            TetrisShape shape = shapeAt(j, i);
            if (shape != NoShape){
                 drawSquare(painter, rect.left() + j * square_side_,
                            rect.top() + i * square_side_,
                            shape);
            }
        }
    }

    // qDebug() << "Drawing NEW piece" ;
    // Drawing new piece
    if (curr_piece_.shape() != NoShape) {
        for (int i = 0; i < 4; ++i) {
            int x = curr_x_ + curr_piece_.x(i);
            int y = curr_y_ + curr_piece_.y(i);
            drawSquare(painter, rect.left() + x * square_side_,
                       rect.top() + y*square_side_,
                       curr_piece_.shape());
        }
    }

    // qDebug() << "paintEvent completed" ;
}

void TetrisBoard::clearBoard()
{
    qDebug() << "clearBoard started" ;
    for (int i = 0; i < board_height_steps_ * board_width_steps_; ++i)
        board_[i] = NoShape;


    qDebug() << "clearBoard completed" ;
}

void TetrisBoard::keyPressEvent(QKeyEvent *event)
{
    if (!is_started_ || is_paused_ || curr_piece_.shape() == NoShape) {
        QFrame::keyPressEvent(event);
        return;
    }

    switch (event->key()) {
    case Qt::Key_Left:
        std::cout << "LEFT" << std::endl;
        tryMove(curr_piece_, curr_x_ - 1, curr_y_);
        // update();
        break;
    case Qt::Key_Right:
        std::cout << "RIGHT" << std::endl;
        tryMove(curr_piece_, curr_x_ + 1, curr_y_);
        // update();
        break;
    case Qt::Key_Up:
        std::cout << "UP" << std::endl;
        // y--;
        // update();

        break;
    case Qt::Key_Down:
        std::cout << "DOWN" << std::endl;
        // y++;
        // update();

        break;
    default:
        QFrame::keyPressEvent(event);
    }
}

void TetrisBoard::timerEvent(QTimerEvent *event){
    if(event->timerId() == timer_.timerId()){
        // std::cout << "Timout event passed" << std::endl;
        oneLineDown();
    } else {
        QFrame::timerEvent(event);
    }
}

void TetrisBoard::drawSquare(QPainter &painter, int x, int y, TetrisShape shape){
    static constexpr QRgb colorTable[8] = {
        0x000000, 0xCC6666, 0x66CC66, 0x6666CC,
        0xCCCC66, 0xCC66CC, 0x66CCCC, 0xDAAA00
    };


    QColor color = colorTable[int(shape)];
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

void TetrisBoard::drawBackgroundGrid(QPainter &painter){

    // qDebug() << "grid background started" ;
    QRect rect = contentsRect();
    painter.setPen(QPen(Qt::lightGray, 1, Qt::SolidLine));

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

QSize TetrisBoard::getBoardSize(){
    qDebug() << "getBoardSize: [Width, Height]: " << square_side_*board_width_steps_ + 2*frameWidth() << "," <<square_side_*board_height_steps_ + 2*frameWidth();

    return QSize(square_side_*board_width_steps_ + 2*frameWidth(),
                 square_side_*board_height_steps_ + 2*frameWidth());
}


void TetrisBoard::newPiece()
{
    curr_piece_ = next_piece_;
    next_piece_.setRandomShape();
    qDebug() << "[NEWPIECE] Current piece: " << curr_piece_.shape();
    qDebug() << "[NEWPIECE] Next selected piece: " << next_piece_.shape();
    // next_piece_.setShape(TetrisShape::LineShape);
    // showNextPiece();
    curr_x_ = board_width_steps_ / 2;
    curr_y_ = qAbs(curr_piece_.minY());


    qDebug() << "[NEWPIECE] Trying positioning new piece top. Curr coords: " << curr_x_ << ", " << curr_y_;
    if (!tryMove(curr_piece_, curr_x_, curr_y_)) {
        qDebug() << "New piece no have space, setting noShape.";
        curr_piece_.setShape(NoShape);
        timer_.stop();
    }

    qDebug() << "NewPiece completed.";
}

void TetrisBoard::oneLineDown(){

    if(!tryMove(curr_piece_, curr_x_, curr_y_ + 1)){
        pieceDropped();
        std::cout << "Piece cannot go more down" << std::endl;
    }

}

bool TetrisBoard::tryMove(const
                          TetrisPiece &new_piece, int new_x, int new_y)
{
    // qDebug() << "TryMove starting." ;
    for(int i  = 0; i<4; ++i){
        int x = new_piece.x(i) + new_x;
        int y = new_piece.y(i) + new_y;

        qDebug() << "[tryMove] X and Y: " << new_piece.x(i) << ", " << new_piece.y(i) << " -- " << new_x << ", " << new_y;
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
    newPiece();
    // Increase level each 25 piece dropped
    // if (num_piece_dropped_ % 25 == 0) {
    //     // ++level;
    //     timer_.start(timeout_time_, this);
    //     // emit levelChanged(level);
    // }

    // score += dropHeight + 7;
    // emit scoreChanged(score);
    // removeFullLines();

    // if (!isWaitingAfterLine)
    //     newPiece();
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

    num_piece_dropped_ = 0;
    score_ = 0;

    clearBoard();
    newPiece();
    timer_.start(timeout_time_, this);
    std::cout << "Game logic has started. Timer started" << std::endl;
}

void TetrisBoard::reset(){
    is_started_ = false;
    is_paused_ = false;
    timer_.stop();
    std::cout << "Game logic has stopped." << std::endl;
}

void TetrisBoard::pause(){
    is_paused_ = true;
    timer_.stop();
    std::cout << "Game has paused" << std::endl;
    update();
}

void TetrisBoard::resume(){
    is_paused_ = false;
    timer_.start(timeout_time_, this);
    std::cout << "Game has resumed, after paused." << std::endl;
    update();
}
