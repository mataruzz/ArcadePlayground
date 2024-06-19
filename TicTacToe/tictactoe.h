#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <QWidget>
#include <QPushButton>
#include <QIcon>
#include <QString>
#include <QVector>
#include <QLCDNumber>
#include <QPalette>
#include <QTableWidgetItem>
#include <QTextEdit>
#include <QDialog>
#include <QHBoxLayout>
#include <QSize>

#include <QDebug>

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

    QSize getWidgetSize();

private:
    Ui::TicTacToe *ui;
    QPushButton *go_back_button_;
    QPushButton *reset_board_button_;
    QPushButton *reset_score_button_;
    QPushButton *change_icon_button_;

    QLCDNumber *lcd_player_, *lcd_computer_;

    QTextEdit *comment_frame_;

    QIcon x_icon_, o_icon_, player_winning_icon_, computer_winning_icon_;

    QSize original_widget_size_;

    QDialog change_icon_dialog_;

    QVector<QVector<char>> board_; // parallel board filled with char
    QVector<QVector<QPushButton*>> board_buttons_;
    QVector<QString> front_button_names_, back_button_names_;

    char player_icon_, computer_icon_;
    qint8 score_player_, score_computer_;
    std::unordered_set<qint8> free_spots_;
    QString o_icon_path_, x_icon_path_;
    QString x_black_icon_path_, o_black_icon_path_, x_red_icon_path_, o_red_icon_path_, x_green_icon_path_, o_green_icon_path_;

    void initializeBoard();
    void initializeIcons();
    void initializeChangeIconDialog();
    void handleBoardButtonClick();
    void handlePlayerAction(const gameState &action_result, const char player);
    void handleChangeIconClicked();
    gameState checkGameStateForPlayer(char palyer);
    void disableButton(QPushButton *button);
    void enableButton(QPushButton *button);
    void clearIconFromButton(QPushButton *button);
    qint8 idxsToSpot(const qint8 &i, const qint8 &j);
    std::pair<qint8, qint8> spotToIdxs(const qint8 &s);
    void randomBotAction(const char player);
    void updateLcdScores();
    void resetLcdScores();
    void markWinningButtons(const QVector<std::pair<qint8, qint8>> &idxs_vect);

private slots:
    void onGoBackButtonClicked();
    void handleResetBoardClicked();
    void handleResetScoreClicked();
    void changeIconCircle();
    void changeIconCross();

signals:
    void goBackToMainMenu();
};

#endif // TICTACTOE_H

/* MISSING FEATURES:
 * - IMPROVE COMPUTER LOGIC GAME
 * - IMPLEMENT FUNCTION WHICH TRACK WHICH LINE IS THE WINNING
 * - RANDOM START, NOT ALWAYS PLAYER
 * - Change reset board button name to play again when winning found
 * - Improve output in the console log below game
 */
