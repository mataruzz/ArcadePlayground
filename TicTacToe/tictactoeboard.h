#ifndef TICTACTOEBOARD_H
#define TICTACTOEBOARD_H

#include <QWidget>
#include <QLabel>
#include <QDialog>

#include <random>
#include <algorithm>

#include "ui_board_form.h"

// Game states
enum gameState{won, draw, playing, error};

enum gameLevel{easy, medium, hard};

// namespace Ui {
// class BoardForm;
// }

class TicTacToeBoard : public QWidget
{
    Q_OBJECT
public:
    explicit TicTacToeBoard(QWidget *parent = nullptr, const QString xIconPath = QString(), const QString oIconPath = QString());
    ~TicTacToeBoard() { delete ui; }
    void setCurrentIconLabel(QLabel *label);
    void setCommentLabel(QLabel *label);
    void setGameLevel(gameLevel level);
    QIcon getPlayerIcon();
    QIcon getComputerIcon();
    char getPlayerChar(){return player_icon_char_;};

signals:
    void botTurn();
    void updateCommentLabel(const QString &text);
    void gameIsOver(const gameState &state, const char &player);
    void gameStarted();

public slots:
    void handleResetBoardClicked();
    void changeIconToCircle();
    void changeIconToCross();

private slots:
    void performBotAction();

private:
    void botActionBasedOnLevel();
    void randomBotAction();
    void mediumBotAction();
    void disableButton(QPushButton *button);
    void initializeBoard();
    void initializeIcons();
    void handleBoardButtonClick();
    void markButtonWithIcon(QPushButton *button, QIcon icon);
    void markComputerButton(qint8 row, qint8 col);
    // void updatePlayerIconLabel();
    void clearIconFromButton(QPushButton *button);
    void enableButton(QPushButton *button);
    qint8 idxsToSpot(const qint8 &r, const qint8 &c);
    std::pair<qint8, qint8> spotToIdxs(const qint8 &s);
    gameState checkGameStateForPlayer(char player);
    void markWinningButtons(const QVector<std::pair<qint8, qint8>> &idxs_vect);
    void handlePlayerAction(const gameState &action_result, const char player);
    QIcon changeIconColor(const QIcon& icon, const QColor& color);
    bool canWinLine(const char a, const char b, const char c, const char player, QVector<qint8> &win_poses);
    bool canWinSingleMove(const char a, const char b, const char c, const char player, qint8 &pose);
    void grayOutBoardButtons();



    Ui::Form *ui;
    QString x_icon_path_, o_icon_path_;
    char player_icon_char_, computer_icon_char_;
    char starter_player_;

    QVector<QVector<char>> board_;
    QVector<QVector<QPushButton*>> board_buttons_;
    QVector<QString> front_button_names_, back_button_names_;


    QSet<qint8> free_spots_;

    QIcon x_icon_, o_icon_;
    QIcon player_winning_icon_, computer_winning_icon_;
    QIcon player_icon_, computer_icon_;

    QLabel *current_icon_label_;
    QLabel *comment_label_;

    gameLevel game_level_;
    bool easy_randomness_;

};

#endif // TICTACTOEBOARD_H
