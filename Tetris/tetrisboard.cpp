#include "tetrisboard.h"

TetrisBoard::TetrisBoard(QWidget *parent)
    : QFrame(parent)
    , is_player_playing_(false)
    , timeout_time_(1000)
{
    // Set some default properties for the frame
    setFrameStyle(QFrame::Box | QFrame::Plain);
    setLineWidth(2);
    setMidLineWidth(0);

    // Needed to recognize event in frame from used (i.e., key pressed)
    setFocusPolicy(Qt::StrongFocus);

}

void TetrisBoard::paintEvent(QPaintEvent *event){
    QFrame::paintEvent(event);
    QPainter painter(this);
    QRect rect = contentsRect();
    int boardTop = rect.top();
    drawSquare(painter, rect.left() + x*squareWidth(),
               boardTop + y*squareHeight(), TetrisShape::SquareShape);
    drawBackgroundGrid(painter);

    // Resizing the board window to fit the squares of the grid game
    if(!is_started_)
        resize(getBoardSize());


}

void TetrisBoard::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Left:
        // std::cout << "LEFT" << std::endl;
        x--;
        update();
        break;
    case Qt::Key_Right:
        // std::cout << "RIGHT" << std::endl;
        x++;
        update();

        break;
    case Qt::Key_Up:
        // std::cout << "UP" << std::endl;
        y--;
        update();

        break;
    case Qt::Key_Down:
        // std::cout << "DOWN" << std::endl;
        y++;
        update();

        break;
    default:
        break;
    }
}

void TetrisBoard::timerEvent(QTimerEvent *event){
    if(event->timerId() == timer_.timerId()){
        std::cout << "Timout event" << std::endl;
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
    painter.fillRect(x + 1, y + 1, squareWidth() - 2, squareHeight() - 2,
                     color);

    painter.setPen(color.lighter());
    painter.drawLine(x, y + squareHeight() - 1, x, y);
    painter.drawLine(x, y, x + squareWidth() - 1, y);

    painter.setPen(color.darker());
    painter.drawLine(x + 1, y + squareHeight() - 1,
                     x + squareWidth() - 1, y + squareHeight() - 1);
    painter.drawLine(x + squareWidth() - 1, y + squareHeight() - 1,
                     x + squareWidth() - 1, y + 1);
}

void TetrisBoard::drawBackgroundGrid(QPainter &painter){
    QRect rect = contentsRect();
    painter.setPen(QPen(Qt::lightGray, 1, Qt::SolidLine));

    // Draw vertical lines
    for (int i = 1; i <= BoardWidth; ++i) {
        int x = rect.left() + i * squareWidth();
        painter.drawLine(x, rect.top(), x, rect.bottom());
    }

    // Draw horizontal lines
    for (int i = 1; i <= BoardHeight; ++i) {
        int y = rect.top() + i * squareHeight();
        painter.drawLine(rect.left(), y, rect.right(), y);
    }

}

QSize TetrisBoard::getBoardSize(){
    return QSize(squareWidth()*BoardWidth + 2*frameWidth(),
                 squareHeight()*BoardHeight + 2*frameWidth());
}

void TetrisBoard::start_pause_logic(){

    if(!is_started_){
        if(!is_player_playing_){
            timer_.start(timeout_time_, this);
            std::cout << "timer is on first time" << std::endl;
        }else{

            std::cout << "timer is stopped" << std::endl;

        }

        is_started_ = true;
        std::cout << "Game logic has started" << std::endl;

    }else{
        is_started_ = false;
        timer_.stop();
        std::cout << "Game logic has paused, with time:" << timer_.timerId() << std::endl;
    }

}

// QSize TetrisBoard::sizeHint() const{
//     return QSize(BoardWidth * 15 + frameWidth() * 2,
//                  BoardHeight * 15 + frameWidth() * 2);
// }

// QSize TetrisBoard::minimumSizeHint() const
// {
//     return QSize(BoardWidth * 5 + frameWidth() * 2,
//                  BoardHeight * 5 + frameWidth() * 2);
// }
