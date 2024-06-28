#include "tetriswindow.h"

TetrisWindow::~TetrisWindow(){};

TetrisWindow::TetrisWindow(QWidget *parent)
    : QWidget(parent)
    , is_started_(false)
    , is_paused_(false)

{
    original_widget_size_ = this->size();
    score_lcd_ = new QLCDNumber(5);

    start_game_button_ = new QPushButton("&Start");
    pause_restart_button_ = new QPushButton("&Pause");
    go_back_button_ = new QPushButton("&Go Back");

    next_piece_label_ = new QLabel();
    next_piece_label_->setFrameStyle(1);
    board_ = new TetrisBoard();

    // Resizing the board window to fit the squares of the grid game
    // board_->resize(board_->getBoardSize());

    initializeWindow();

    connect(go_back_button_, &QPushButton::clicked, this, &TetrisWindow::onGoBackButtonClicked);
    connect(start_game_button_, &QPushButton::clicked, this, &TetrisWindow::handleStartResetButtonClicked);
    connect(pause_restart_button_, &QPushButton::clicked, this, &TetrisWindow::handlePauseRestartButtonClicked);

    // connect(start_game_button_, &QPushButton::clicked, board_, &TetrisBoard::start);

}

void TetrisWindow::initializeWindow(){

    // WINDOW
    qint8 row_layout_end = 8;
    qint8 col_layout_end = 8;

    // BOARD
    qint8 row_board_start = 1;
    qint8 row_board_size = 10;
    qint8 row_board_end = row_board_size+row_board_start-1;
    qint8 col_board_start = 0;
    qint8 col_board_size = 5;
    qint8 col_board_end = col_board_size+col_board_start-1;

    // GO BACK BUTTON
    qint8 col_back_button_start = col_board_end + 2;
    qint8 col_back_button_size = 2;
    qint8 col_back_button_end = col_back_button_start + col_back_button_size -1;
    qint8 row_back_button_start = 1;
    qint8 row_back_button_size = 1;
    qint8 row_back_button_end = row_back_button_start + row_back_button_size -1;

    // NEXT LABEL
    qint8 col_next_label_start = col_board_end + 2;
    qint8 col_next_label_size = 2;
    qint8 col_next_label_end = col_next_label_start + col_next_label_size -1;
    qint8 row_next_label_start = row_back_button_end + 2;
    qint8 row_next_label_size = 1;
    qint8 row_next_label_end = row_next_label_start + row_next_label_size -1;

    // NEXT PIECE LABEL
    qint8 col_next_piece_start = col_board_end + 1;
    qint8 col_next_piece_size = 4;
    qint8 col_next_piece_end = col_next_piece_start + col_next_piece_size -1;
    qint8 row_next_piece_start = row_next_label_end + 1;
    qint8 row_next_piece_size = 1;
    qint8 row_next_piece_end = row_next_piece_start + row_next_piece_size -1;

    // SCORE LABEL
    qint8 col_score_label_start = col_board_end + 2;
    qint8 col_score_label_size = 2;
    qint8 col_score_label_end = col_score_label_start + col_score_label_size -1;
    qint8 row_score_label_start = row_next_piece_end + 2;
    qint8 row_score_label_size = 1;
    qint8 row_score_label_end = row_score_label_start+row_score_label_size-1;

    // SCORE LCD
    qint8 col_score_lcd_start = col_board_end + 2;
    qint8 col_score_lcd_size = 2;
    qint8 col_score_lcd_end = col_score_lcd_start + col_score_lcd_size -1;
    qint8 row_score_lcd_start = row_score_label_end + 1;
    qint8 row_score_lcd_size = 1;
    qint8 row_score_lcd_end = row_score_lcd_start+row_score_lcd_size-1;

    // START BUTTON
    qint8 col_start_button_start = col_board_end + 1;
    qint8 col_start_button_size = 2;
    qint8 col_start_button_end = col_start_button_start + col_start_button_size -1;
    qint8 row_start_button_start = row_score_lcd_end + 2;
    qint8 row_start_button_size = 1;
    qint8 row_start_button_end = row_start_button_start+row_start_button_size-1;

    // PAUSE BUTTON
    qint8 col_pause_button_start = col_board_end + 3;
    qint8 col_pause_button_size = 2;
    qint8 col_pause_button_end = col_start_button_start + col_start_button_size -1;
    qint8 row_pause_button_start = row_score_lcd_end + 2;
    qint8 row_pause_button_size = 1;
    qint8 row_pause_button_end = row_start_button_start+row_start_button_size-1;


    QGridLayout *layout = new QGridLayout;
    layout->addWidget(createLabel("TETRIS"), 0, 0, 1, col_layout_end+1);
    layout->setRowStretch(0, 10);
    layout->addWidget(board_, row_board_start, col_board_start, row_board_size, col_board_size);

    layout->addWidget(go_back_button_, row_back_button_start, col_back_button_start, row_back_button_size, col_back_button_size);
    // Empty rows
    layout->setRowStretch(row_back_button_start+1, 5);

    layout->addWidget(createLabel("NEXT"), row_next_label_end, col_next_label_start, row_next_label_size, col_next_label_size);
    layout->addWidget(next_piece_label_, row_next_piece_start, col_next_piece_start, row_next_piece_size, col_next_piece_size);
    layout->setRowStretch(row_next_piece_start, 20);
    // Empty rows
    layout->setRowStretch(row_next_piece_start+1, 5);

    layout->addWidget(createLabel("SCORE"), row_score_label_start, col_score_label_start, row_score_label_size, col_score_label_size);
    layout->addWidget(score_lcd_, row_score_lcd_start, col_score_lcd_start, row_score_lcd_size, col_score_lcd_size);
    layout->setRowStretch(row_score_lcd_start, 10);
    // Empty rows
    layout->setRowStretch(row_score_lcd_start+1, 5);

    layout->addWidget(start_game_button_, row_start_button_start, col_start_button_start, row_start_button_size, col_start_button_size);

    layout ->addWidget(pause_restart_button_, row_pause_button_start, col_pause_button_start, row_pause_button_size, col_pause_button_size);

    setLayout(layout);

    setWindowTitle("Tetris");
    resize(540, 400);

}

void TetrisWindow::onGoBackButtonClicked(){
    emit goBackToMainMenu();
}

QLabel *TetrisWindow::createLabel(const QString &text)
{
    QLabel *label = new QLabel(text);
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label->setFrameStyle(1);
    return label;
}


QSize TetrisWindow::getWidgetSize(){
    return original_widget_size_;
}


void TetrisWindow::handleStartResetButtonClicked(){
    if(!is_started_){
        start_game_button_->setText("Reset Game");
        is_started_ = true;
    }else{
        start_game_button_->setText("Start");
        is_started_ = false;
    }

}

void TetrisWindow::handlePauseRestartButtonClicked(){
    if(!is_paused_){
        pause_restart_button_->setText("Pause");
        is_paused_ = true;
    }else{
        pause_restart_button_->setText("Play");
        is_paused_ = false;
    }

}
