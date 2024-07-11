#ifndef TETRISWINDOW_H
#define TETRISWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLCDNumber>
#include <QLabel>
#include <QGridLayout>
#include <QSize>
#include <QSpacerItem>
#include <QFontDatabase>

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

    QLabel *next_piece_label_, *best_score_label_;
    QLabel *title_label_;

    QSize original_widget_size_;

    bool is_started_, is_paused_;

    QLabel *createLabel(const QString &text);
    void initializeWindow();
    QSize getSizeFromCellToCell(QGridLayout* layout, int from_row, int from_column, int to_row, int to_column);


private slots:
    void onGoBackButtonClicked();
    // void handleStartButtonClicked();
    // void handleResetButtonClicked();
    // void handlePauseButtonClicked();
    // void handleResumeButtonClicked();
    void handlePauseGame();
    void handleResumeGame();

    void handleStartResetButtonClicked();
    void handlePauseRestartButtonClicked();
    void handleGameLost();
    void displayBestScore(int score);

signals:
    void goBackToMainMenu();

    void gameStarted();
    void gameResetted();
    void gamePaused();
    void gameResumed();

};

#endif // TETRISWINDOW_H

/*
 * TO DO:
 * - ADD player name on best score in pop-up mode
 *
 * TO FIX:
 * - Check multiple timeout 560 print.
 */
