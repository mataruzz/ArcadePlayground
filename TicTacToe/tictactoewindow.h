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

#include "TicTacToe/tictactoeboard.h"

class TicTacToeWindow : public QWidget
{
    Q_OBJECT
public:
    explicit TicTacToeWindow(QWidget *parent = nullptr);

    QSize getWidgetSize();

signals:
    void goBackToMainMenu();

private slots:
    void onGoBackButtonClicked();

private:
    void initializeWindow();
    void initializeButtons();
    QVBoxLayout* setIconLayout();
    QVBoxLayout* setLCDScoreLayout();
    QFrame* initializeDialogArea();

    QLabel *title_label_;
    QPushButton *reset_score_button_, *reset_board_button_;
    QPushButton *go_back_button_, *change_icon_button_;
    QComboBox *level_combo_box_;

    // QLabel *game_board_;
    QLabel *comment_frame_;

    TicTacToeBoard *game_board_;

    QSize original_widget_size_;
    gameLevel game_level_;
};

#endif // TICTACTOEWINDOW_H
