#ifndef TICTACTOEBOARD_H
#define TICTACTOEBOARD_H

#include <QWidget>

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
    explicit TicTacToeBoard(QWidget *parent = nullptr);
    ~TicTacToeBoard() {
        delete ui;
    }

private:
    Ui::Form *ui;

signals:
};

#endif // TICTACTOEBOARD_H
