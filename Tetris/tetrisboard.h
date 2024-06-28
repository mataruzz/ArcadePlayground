#ifndef TETRISBOARD_H
#define TETRISBOARD_H

#include <QFrame>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>
#include <QKeyEvent>
#include <QBasicTimer>

#include "iostream"
#include "Tetris/tetrispiece.h"

class TetrisBoard : public QFrame
{
    Q_OBJECT

public:
    explicit TetrisBoard(QWidget *parent = nullptr);

    QSize getBoardSize();

    // QSize sizeHint() const override;
    // QSize minimumSizeHint() const override;

public slots:
    void start_pause_logic();

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

private:
    enum { BoardWidth = 14, BoardHeight = 16}; // real proportion frame height: 9cm, width: 8cm
    int squareWidth() { return contentsRect().width() / BoardWidth; }
    int squareHeight() { return contentsRect().height() / BoardHeight; }
    void drawSquare(QPainter &painter, int x, int y, TetrisShape shape);
    void drawBackgroundGrid(QPainter &painter);


    int x{BoardWidth/2}, y{0};
    int timeout_time_;
    bool is_started_;
    bool is_player_playing_;
    QBasicTimer timer_;

};

#endif // TETRISBOARD_H
