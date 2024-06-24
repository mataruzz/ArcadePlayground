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
#include <QLabel>
#include <QTimer>
#include <QComboBox>

#include <unordered_set>
#include <iostream>
#include <random>
#include <algorithm>

// Game states
enum gameState{won, draw, playing, error};

enum gameLevel{easy, medium, hard};

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
    QIcon player_icon_, computer_icon_;

    QSize original_widget_size_;

    QDialog change_icon_dialog_;

    QLabel *player_icon_label_;

    QComboBox *level_combo_box_;

    QVector<QVector<char>> board_;
    QVector<QVector<QPushButton*>> board_buttons_;
    QVector<QString> front_button_names_, back_button_names_;

    char player_icon_char_, computer_icon_char_;
    char starter_player_;
    qint8 score_player_, score_computer_;
    std::unordered_set<qint8> free_spots_;
    QString o_icon_path_, x_icon_path_;
    QString x_black_icon_path_, o_black_icon_path_, x_red_icon_path_, o_red_icon_path_, x_green_icon_path_, o_green_icon_path_;
    bool easy_randomness_;

    gameLevel game_level_;


    void initializeBoard();
    void initializeIcons();
    void initializeChangeIconDialog();
    void initializeLevelComboBox();

    void handleBoardButtonClick();
    void handlePlayerAction(const gameState &action_result, const char player);
    void handleChangeIconClicked();
    gameState checkGameStateForPlayer(char player);
    void disableButton(QPushButton *button);
    void enableButton(QPushButton *button);
    void clearIconFromButton(QPushButton *button);
    qint8 idxsToSpot(const qint8 &i, const qint8 &j);
    std::pair<qint8, qint8> spotToIdxs(const qint8 &s);
    void randomBotAction();
    void mediumBotAction();
    void botActionBasedOnLevel();
    void updateLcdScores();
    void resetLcdScores();
    void markWinningButtons(const QVector<std::pair<qint8, qint8>> &idxs_vect);
    void markButtonWithIcon(QPushButton *button, QIcon icon);
    void markComputerButton(qint8 row, qint8 col);
    void updatePlayerIconLabel();
    bool canWinLine(const char a, const char b, const char c, const char player, QVector<qint8> &win_poses);
    bool canWinSingleMove(const char a, const char b, const char c, const char player, qint8 &pose);

private slots:
    void onGoBackButtonClicked();
    void handleResetBoardClicked();
    void handleResetScoreClicked();
    void changeIconCircle();
    void changeIconCross();
    void handleChangeLevelBox();
    void performBotAction();

signals:
    void goBackToMainMenu();
    void botTurn();
};

#endif // TICTACTOE_H

/* TO DO LIST:
 * - IMPROVE COMPUTER LOGIC GAME, adding 3 levels:
 * --- Dummy (dummy bot -> using simple random)
 * --- Easy (using personal bot trying to find if missing single shot to win)
 * --- Medium/Hard (minmax algorithm)
 * - Gray out when draw game
 */
