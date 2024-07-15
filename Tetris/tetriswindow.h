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
#include <QMultiMap>
#include <QSettings>
#include <QInputDialog>

#include "Tetris/tetrisboard.h"

class TetrisWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TetrisWindow(QWidget *parent = nullptr);
    ~TetrisWindow();

    QSize getWidgetSize();
    QMultiMap<int, QString> getScores(){return scores_;};

private slots:
    void displayBestScores();
    void handleGameLost(const int score);
    void handlePauseGame();
    void handlePauseRestartButtonClicked();
    void handleResumeGame();
    void handleStartResetButtonClicked();
    void onGoBackButtonClicked();

public slots:
    void updateScores(int new_score, const QString& new_username);

signals:
    void goBackToMainMenu();
    void gameStarted();
    void gameResetted();
    void gamePaused();
    void gameResumed();

private:
    QLabel *createLabel(const QString &text);
    void initializeWindow();
    QSize getSizeFromCellToCell(QGridLayout* layout, int from_row, int from_column, int to_row, int to_column);
    void loadScores();
    void saveScores();

    TetrisBoard *board_;
    QPushButton *start_game_button_;
    QPushButton *pause_restart_button_;
    QPushButton *go_back_button_;
    QLCDNumber *score_lcd_; //1: 40 - 2: 100 - 3: 300 - 4: 1200
    QLabel *next_piece_label_, *best_score_label_;
    QLabel *title_label_;
    QSize original_widget_size_;
    bool is_started_, is_paused_;
    QMultiMap<int, QString> scores_; // {score, username}
    QSettings db_;

    static const QString SCORE_KEY_PREFIX;
    static const QString USERNAME_KEY_PREFIX;
    static const int NUM_SCORES;
};

#endif // TETRISWINDOW_H
