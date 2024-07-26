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


class TicTacToeBoard : public QWidget
{
    Q_OBJECT
public:
    explicit TicTacToeBoard(QWidget *parent = nullptr, const QString xIconPath = QString(), const QString oIconPath = QString());
    ~TicTacToeBoard() { delete ui; }
    QIcon getComputerIcon();
    char getPlayerChar();
    QIcon getPlayerIcon();
    void setCurrentIconLabel(QLabel *label);
    void setGameLevel(gameLevel level);

signals:
    void botTurn();
    void gameIsOver(const gameState &state, const char &player);
    void gameStarted();
    void updateCommentLabel(const QString &text); // used to write text in a Label outside this class

public slots:
    void changeIconToCircle();
    void changeIconToCross();
    void handleResetBoardClicked();

private slots:
    void performBotAction();

private:
    void botActionBasedOnLevel();
    bool canWinLine(const char a, const char b, const char c, const char player, QVector<qint8> &win_poses);
    bool canWinSingleMove(const char a, const char b, const char c, const char player, qint8 &pose);
    QIcon changeIconColor(const QIcon& icon, const QColor& color);
    gameState checkGameStateForPlayer(char player);
    void clearIconFromButton(QPushButton *button);
    void disableButton(QPushButton *button);
    void enableButton(QPushButton *button);
    void grayOutBoardButtons();
    void handleBoardButtonClick();
    void handlePlayerAction(const gameState &action_result, const char player);
    qint8 idxsToSpot(const qint8 &r, const qint8 &c);
    void initializeBoard();
    void initializeIcons();
    void markButtonWithIcon(QPushButton *button, QIcon icon);
    void markComputerButton(qint8 row, qint8 col);
    void markWinningButtons(const QVector<std::pair<qint8, qint8>> &idxs_vect);
    void mediumBotAction();
    void randomBotAction();
    std::pair<qint8, qint8> spotToIdxs(const qint8 &s);


    Ui::Form *ui;
    QString x_icon_path_, o_icon_path_;
    char player_icon_char_, computer_icon_char_;
    char starter_player_;
    bool easy_randomness_;

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
};

#endif // TICTACTOEBOARD_H
