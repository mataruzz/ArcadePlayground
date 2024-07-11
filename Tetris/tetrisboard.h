#ifndef TETRISBOARD_H
#define TETRISBOARD_H

#include <QFrame>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>
#include <QLabel>
#include <QKeyEvent>
#include <QBasicTimer>
#include <QtMath>
#include <QSettings>

#include <QDebug>

#include "iostream"
#include "Tetris/tetrispiece.h"

class TetrisBoard : public QFrame
{
    Q_OBJECT

public:

    explicit TetrisBoard(QWidget *parent = nullptr);

    QSize getBoardSize();
    void setNextPieceLabel(QLabel *label);
    void setBoardSize(QSize board_size);
    void setSquareSide(int side_size){ square_side_ = side_size; }
    int loadBestScore();

public slots:
    void start();
    void reset();
    void pause();
    void resume();

signals:
    void gameLost();
    void updateBestScoreLcd(const int score);
    void updateScoreLcd(const int score);


protected:
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void timerEvent(QTimerEvent *event) override;


private:
    void backToNormalSpeed();
    void clearBoard();
    void drawBackgroundGrid(QPainter &painter, int alpha_color);
    void drawCurrentPiece(QPainter &painter, int alpha_color);
    void drawPlacedPieces(QPainter &painter, int alpha_color);
    void drawSquare(QPainter &painter, int x, int y, TetrisShape shape, int alpha_color);
    void dropDown();
    void levelUp();
    void newPiece();
    void oneLineDown();
    void pieceDropped();
    void removeFullLines();
    void resetTimeout();
    void saveBestScore();
    void showNextPiece();
    void speedUp();
    TetrisShape &shapeAt(int x, int y) { return board_[(y * board_width_steps_) + x]; }
    bool tryMove(const TetrisPiece &new_piece, int new_x, int new_y);
    void updateScore(const int lines_removed);

    int board_width_steps_, board_height_steps_;
    int square_side_;
    bool is_started_, is_paused_;
    int timeout_time_;
    int score_, best_score_;
    int curr_x_, curr_y_;
    int num_piece_dropped_, num_lines_removed_;
    int level_up_range_;
    int not_active_alpha_color_, active_alpha_color_;
    int is_lost_;

    QSettings settings_;
    QBasicTimer timer_;
    TetrisPiece curr_piece_, next_piece_;
    QLabel *next_piece_label_;

    QVector<TetrisShape> board_;
};

#endif // TETRISBOARD_H