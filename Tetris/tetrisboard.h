#ifndef TETRISBOARD_H
#define TETRISBOARD_H

#include <QFrame>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>
#include <QKeyEvent>
#include <QBasicTimer>
#include <QtMath>

#include <QDebug>

#include "iostream"
#include "Tetris/tetrispiece.h"

class TetrisBoard : public QFrame
{
    Q_OBJECT

public:

    explicit TetrisBoard(QWidget *parent = nullptr);

    QSize getBoardSize();
    void setBoardSize(QSize board_size);
    void setSquareSide(int side_size){ square_side_ = side_size; }

public slots:
    void start();
    void reset();
    void pause();
    void resume();


protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event) override;


private:
    // TO BE CANCELED
    // enum { BoardWidth = 14, BoardHeight = 16}; // real proportion frame height: 9cm, width: 8cm
    // int squareWidth() {return contentsRect().width() / BoardWidth; }
    // int squareHeight() { return contentsRect().height() / BoardHeight; }

    TetrisShape &shapeAt(int x, int y) { return board_[(y * board_width_steps_) + x]; }

    void drawSquare(QPainter &painter, int x, int y, TetrisShape shape);
    void drawBackgroundGrid(QPainter &painter);
    void oneLineDown();
    bool tryMove(const TetrisPiece &new_piece, int new_x, int new_y);
    void newPiece();
    void clearBoard();
    void pieceDropped();


    int board_width_steps_, board_height_steps_;
    int timeout_time_;
    bool is_started_, is_paused_;
    int curr_x_, curr_y_;
    int num_piece_dropped_;
    int score_;
    int square_side_;


    QBasicTimer timer_;
    TetrisPiece curr_piece_, next_piece_;

    QVector<TetrisShape> board_;
};

#endif // TETRISBOARD_H
