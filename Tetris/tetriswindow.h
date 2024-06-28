#ifndef TETRISWINDOW_H
#define TETRISWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLCDNumber>
#include <QLabel>
#include <QGridLayout>
#include <QSize>
#include <QSpacerItem>

#include "Tetris/tetrisboard.h"

class TetrisWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TetrisWindow(QWidget *parent = nullptr);
    ~TetrisWindow();

    QSize getWidgetSize();


private:
    TetrisBoard *board_;
    QPushButton *start_game_button_;
    QPushButton *pause_restart_button_;
    QPushButton *go_back_button_;

    QLCDNumber *score_lcd_; //1: 40 - 2: 100 - 3: 300 - 4: 1200

    QLabel *next_piece_label_;

    QSize original_widget_size_;

    bool is_started_, is_paused_;

    QLabel *createLabel(const QString &text);
    void initializeWindow();

private slots:
    void onGoBackButtonClicked();
    void handleStartResetButtonClicked();
    void handlePauseRestartButtonClicked();

signals:
    void goBackToMainMenu();

};

#endif // TETRISWINDOW_H

/*
 * TO DO:
 *  - Fix button display: make that pause is disactivated is start has not clicked
 *  - When resize the Painter properly? Not good inside the paintEvent().
 *  - Improve timer function
 *  - Add boards limits
 *
 * - include level difficulty
 *
 */
