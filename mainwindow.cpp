#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ArcadePlayground)
    , tic_tac_toe_widget_(new TicTacToe)
    , tetris_widget_(new TetrisWindow)
    , stacked_widget_(new QStackedWidget)
    // ,Add other widgets here
{
    ui->setupUi(this);

    // Add widgets to QStackedWidget
    stacked_widget_->addWidget(ui->centralwidget);
    stacked_widget_->addWidget(tic_tac_toe_widget_);
    stacked_widget_->addWidget(tetris_widget_);

    // Set QStackedWidget as the central widget
    setCentralWidget(stacked_widget_);

    // Linking button with the designed one
    tic_tac_toe_button_ = ui->TicTacToeButton;
    tetris_button_ = ui->TetrisButton;

    original_main_window_size_ = this->size();
    setFixedSize(original_main_window_size_);

    // Connecting button signal to appropriate slot
    connect(tic_tac_toe_button_, &QPushButton::clicked, this, &MainWindow::showTicTacToe);
    connect(tetris_button_, &QPushButton::clicked, this, &MainWindow::showTetris);

    // Connecting signals
    connect(tic_tac_toe_widget_, &TicTacToe::goBackToMainMenu, this, &MainWindow::showMainFromTicTacToe);
    connect(tetris_widget_, &TetrisWindow::goBackToMainMenu, this, &MainWindow::showMainFromTicTacToe);

}


void MainWindow::showTicTacToe(){
    stacked_widget_->setCurrentWidget(tic_tac_toe_widget_);
    QSize tic_tac_toe_widget_size = tic_tac_toe_widget_->getWidgetSize();
    setFixedSize(tic_tac_toe_widget_size);
}

void MainWindow::showTetris(){
    stacked_widget_->setCurrentWidget(tetris_widget_);
    QSize tetris_widget_size = tetris_widget_->getWidgetSize();
    setFixedSize(tetris_widget_size);

}

void MainWindow::showMainFromTicTacToe(){
    stacked_widget_->setCurrentIndex(0);
    setFixedSize(original_main_window_size_);
}


