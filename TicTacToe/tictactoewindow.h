#ifndef TICTACTOEWINDOW_H
#define TICTACTOEWINDOW_H

#include <QWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QLCDNumber>
#include <QFrame>
#include <QDialog>

#include "TicTacToe/tictactoeboard.h"

class TicTacToeWindow : public QWidget
{
    Q_OBJECT
public:
    explicit TicTacToeWindow(QWidget *parent = nullptr);

    QSize getWidgetSize();

signals:
    void goBackToMainMenu();
    void iconCrossSelected();
    void iconCircleSelected();

private slots:
    void onGoBackButtonClicked();
    void handleChangeIconClicked();
    void handleResetBoardClicked();
    void handleResetScoreClicked();
    void updateDisplayWithText(const QString &text);
    void handleChangeLevelBox();
    void handleFinishedGame(const gameState &game_state, const char &player);

private:
    void initializeWindow();
    void initializeButtons();
    void initializeChangeIconDialog();
    QVBoxLayout* setIconLayout();
    QVBoxLayout* setLCDScoreLayout();
    QFrame* initializeDialogArea();
    void changeIconCircle();
    void changeIconCross();
    void updatePlayerIconLabel();
    void resetLcdScores();


    QLabel *title_label_;
    QLabel *comment_label_;
    QLabel *icon_player_label_;

    QPushButton *reset_score_button_, *reset_board_button_;
    QPushButton *go_back_button_, *change_icon_button_;

    QLCDNumber *lcd_player_, *lcd_computer_;

    QComboBox *level_combo_box_;

    QDialog change_icon_dialog_;

    TicTacToeBoard *game_board_;

    QSize original_widget_size_;
    gameLevel game_level_;
    int player_score_, computer_score_;
};

#endif // TICTACTOEWINDOW_H
