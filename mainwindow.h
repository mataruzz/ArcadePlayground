#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>

#include <QMainWindow>
#include <QPushButton>
#include <QStackedWidget>
// #include <QWidget>
#include <QHBoxLayout>

#include "TicTacToe/tictactoe.h"
#include "Tetris/tetriswindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ArcadePlayground;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showTicTacToe();
    void showTetris();
    void showMainFromTicTacToe();

private:
    Ui::ArcadePlayground *ui;

    QSize original_main_window_size_;

    QPushButton *tic_tac_toe_button_;
    QPushButton *tetris_button_;

    // Widgets
    TicTacToe *tic_tac_toe_widget_;
    TetrisWindow *tetris_widget_;

    QStackedWidget *stacked_widget_;

};
#endif // MAINWINDOW_H
