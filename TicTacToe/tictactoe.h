#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <QWidget>
#include <QPushButton>
#include <QIcon>
#include <QString>
#include <QVector>

#include <iostream>

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
    QIcon x_icon_, o_icon_;
    QVector<QVector<char>> board_; // parallel board filled with char
    QVector<QVector<QPushButton*>> board_buttons_;
    QVector<QString> front_button_names_, back_button_names_;
    char current_player_;

    void initializeBoard();
    void handleBoardButtonClick();
    void handlePlayerAction(const gameState &action_result);
    gameState checkGameStateForPlayer(char palyer);

private slots:
    void onGoBackButtonClicked();

signals:
    void goBackToMainMenu();
};

#endif // TICTACTOE_H
