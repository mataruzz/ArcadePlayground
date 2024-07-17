#include "tictactoeboard.h"

TicTacToeBoard::TicTacToeBoard(QWidget *parent)
    : QWidget{parent}
    , ui(new Ui::Form)
{
    ui->setupUi(this);
}
