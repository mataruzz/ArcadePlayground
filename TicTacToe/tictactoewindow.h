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
    void iconCircleSelected();
    void iconCrossSelected();

private slots:
    void handleChangeIconClicked();
    void handleChangeLevelBox();
    void handleFinishedGame(const gameState &game_state, const char &player);
    void handleGameStarted();
    void handleResetBoardClicked();
    void handleResetScoreClicked();
    void handleUpdateCommentLabel(const QString &text);
    void onGoBackButtonClicked();


private:
    void changeIconCircle();
    void changeIconCross();
    void initializeButtons();
    void initializeChangeIconDialog();
    QFrame* initializeDialogArea();
    void initializeLevelBox();
    void initializeWindow();
    void resetLcdScores();
    QVBoxLayout* setIconLayout();
    QVBoxLayout* setLCDScoreLayout();
    void setTitle(const QString &title);
    void updatePlayerIconLabel();


    int player_score_, computer_score_;
    QSize widget_size_;
    gameLevel game_level_;

    QLabel *title_label_;
    QLabel *comment_label_;
    QLabel *icon_player_label_;

    QPushButton *reset_score_button_, *reset_board_button_;
    QPushButton *go_back_button_, *change_icon_button_;

    QLCDNumber *lcd_player_, *lcd_computer_;

    QComboBox *level_combo_box_;

    QDialog change_icon_dialog_;

    TicTacToeBoard *game_board_;
};

#endif // TICTACTOEWINDOW_H
