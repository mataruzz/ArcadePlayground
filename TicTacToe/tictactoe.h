#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <QWidget>
#include <QPushButton>
#include <QIcon>
#include <QString>
#include <QVector>
#include <QLCDNumber>

#include <unordered_set>
#include <iostream>
#include <random>
#include <algorithm>

// Game states
enum gameState{won, lost, draw, playing, error};

namespace Ui {
class TicTacToe;
}

class TicTacToe : public QWidget
{
    Q_OBJECT

public:
    explicit TicTacToe(QWidget *parent = nullptr);
    ~TicTacToe();

private:
    Ui::TicTacToe *ui;
    QPushButton *go_back_button_;
    QPushButton *reset_board_button_;
    QPushButton *reset_score_button_;

    QLCDNumber *lcd_player_, *lcd_computer_;

    QIcon x_icon_, o_icon_;

    QVector<QVector<char>> board_; // parallel board filled with char
    QVector<QVector<QPushButton*>> board_buttons_;
    QVector<QString> front_button_names_, back_button_names_;

    char player_icon_, computer_icon_;
    qint8 score_player_, score_computer_;
    std::unordered_set<qint8> free_spots_;

    void initializeBoard();
    void handleBoardButtonClick();
    void handlePlayerAction(const gameState &action_result, const char player);
    gameState checkGameStateForPlayer(char palyer);
    void disableButton(QPushButton *button);
    void enableButton(QPushButton *button);
    void clearIconFromButton(QPushButton *button);
    qint8 idxsToSpot(const qint8 &i, const qint8 &j);
    std::pair<qint8, qint8> spotToIdxs(const qint8 &s);
    void randomBotAction(const char player);
    void updateLcdScores();
    void resetLcdScores();

private slots:
    void onGoBackButtonClicked();
    void handleResetBoardClicked();
    void handleResetScoreClicked();

signals:
    void goBackToMainMenu();
};

#endif // TICTACTOE_H

/* MISSING FEATURES:
 * - IMPROVE COMPUTER LOGIC GAME
 * - IMPLEMENT FUNCTION WHICH TRACK WHICH LINE IS THE WINNING
 * - DRAW A RED LINE ON TOP (OR COLOR BY RED THE ICONS)
 * - MAKE THE PLAYER CHOOSE WHAT TO BE
 * - RANDOM START, NOT ALWAYS PLAYER
 * - FIX WINDOWS RESIZING
 * -
 *
 */
