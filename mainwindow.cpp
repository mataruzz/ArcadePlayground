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
    , stacked_widget_(new QStackedWidget)
    // ,Add other widgets here
{
    ui->setupUi(this);

    // Add widgets to QStackedWidget
    stacked_widget_->addWidget(ui->centralwidget);
    stacked_widget_->addWidget(tic_tac_toe_widget_);

    // Set QStackedWidget as the central widget
    setCentralWidget(stacked_widget_);

    // Linking button with the designed one
    tic_tac_toe_button_ = ui->TicTacToeButton;

    // Connecting button signal to appropriate slot
    connect(tic_tac_toe_button_, &QPushButton::clicked, this, &MainWindow::showTicTacToe);

    // Connecting signals
    connect(tic_tac_toe_widget_, &TicTacToe::goBackToMainMenu, this, &MainWindow::showMainFromTicTacToe);

    // Connect the signal to the slot to resize the main window
    // connect(stacked_widget_, &QStackedWidget::currentChanged, this, &MainWindow::resizeMainWindow);

}
// void MainWindow::resizeMainWindow() {
//     // Get the current widget
//     QWidget *currentWidget = stacked_widget_->currentWidget();
//     if (currentWidget) {
//         // Resize the main window to fit the current widget
//         resize(currentWidget->size());
//     }
// }

void MainWindow::showTicTacToe(){
    stacked_widget_->setCurrentWidget(tic_tac_toe_widget_);
}

void MainWindow::showMainFromTicTacToe(){
    stacked_widget_->setCurrentIndex(0);
}

