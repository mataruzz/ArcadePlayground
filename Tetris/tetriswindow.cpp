#include "tetriswindow.h"

TetrisWindow::~TetrisWindow(){};

TetrisWindow::TetrisWindow(QWidget *parent)
    : QWidget(parent)
    , is_started_(false)
    , is_paused_(false)

{
    original_widget_size_ = this->size();

    title_label_ = new QLabel("TETRIS");
    QFont title_font("FreeMono", 26, QFont::Bold, 1);
    title_label_->setFont(title_font);
    title_label_->setAlignment(Qt::AlignCenter);

    next_piece_label_ = new QLabel();
    next_piece_label_->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    board_ = new TetrisBoard();
    board_->setSquareSide(23);
    board_->setNextPieceLabel(next_piece_label_);

    score_lcd_ = new QLCDNumber(7);

    start_game_button_ = new QPushButton("&Start");
    pause_restart_button_ = new QPushButton("&Pause");
    pause_restart_button_->setEnabled(false);
    pause_restart_button_->setStyleSheet("QPushButton:disabled { background-color: rgb(190, 190, 190); }");

    go_back_button_ = new QPushButton("&Go Back");
    go_back_button_->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::GoPrevious));

    best_score_label_ = new QLabel();
    best_score_label_->setAlignment(Qt::AlignRight);
    int best_score = board_->loadBestScore();
    displayBestScore(best_score);

    initializeWindow();

    connect(go_back_button_, &QPushButton::clicked, this, &TetrisWindow::onGoBackButtonClicked);
    connect(start_game_button_, &QPushButton::clicked, this, &TetrisWindow::handleStartResetButtonClicked);
    connect(pause_restart_button_, &QPushButton::clicked, this, &TetrisWindow::handlePauseRestartButtonClicked);

    connect(this, &TetrisWindow::gameStarted, board_, &TetrisBoard::start);
    connect(this, &TetrisWindow::gameResetted, board_, &TetrisBoard::reset);
    connect(this, &TetrisWindow::gamePaused, board_, &TetrisBoard::pause);
    connect(this, &TetrisWindow::gameResumed, board_, &TetrisBoard::resume);
    connect(board_, &TetrisBoard::gameLost, this, &TetrisWindow::handleGameLost);

    connect(board_, &TetrisBoard::updateScoreLcd, score_lcd_, qOverload<int>(&QLCDNumber::display));
    connect(board_, &TetrisBoard::updateBestScoreLcd, this, &TetrisWindow::displayBestScore);

    connect(this, &TetrisWindow::goBackToMainMenu, board_, &TetrisBoard::pause);
    connect(this, &TetrisWindow::goBackToMainMenu, this, &TetrisWindow::handlePauseGame);

}

void TetrisWindow::displayBestScore(int score){
    QString best_score_text = "Best score:\n\n" + QString::number(score);
    QFont font = best_score_label_->font();
    font.setBold(true);
    font.setPointSize(9);
    best_score_label_->setFont(font);
    best_score_label_->setText(best_score_text);
}

void TetrisWindow::handleGameLost(){
    pause_restart_button_->setEnabled(false);
    // qDebug() << "[HandleGameLost] You lost :(";
}

void TetrisWindow::initializeWindow(){

    // BOARD
    [[maybe_unused]] qint8 row_board_start = 1;
    [[maybe_unused]] qint8 row_board_size = 10;
    [[maybe_unused]] qint8 row_board_end = row_board_size+row_board_start-1;
    [[maybe_unused]] qint8 col_board_start = 0;
    [[maybe_unused]] qint8 col_board_size = 4;
    [[maybe_unused]] qint8 col_board_end = col_board_size+col_board_start-1;

    // GO BACK BUTTON
    [[maybe_unused]] qint8 col_back_button_start = col_board_end + 2;
    [[maybe_unused]] qint8 col_back_button_size = 2;
    [[maybe_unused]] qint8 col_back_button_end = col_back_button_start + col_back_button_size -1;
    [[maybe_unused]] qint8 row_back_button_start = 1;
    [[maybe_unused]] qint8 row_back_button_size = 1;
    [[maybe_unused]] qint8 row_back_button_end = row_back_button_start + row_back_button_size -1;

    // NEXT LABEL
    [[maybe_unused]] qint8 col_next_label_start = col_board_end + 2;
    [[maybe_unused]] qint8 col_next_label_size = 2;
    [[maybe_unused]] qint8 col_next_label_end = col_next_label_start + col_next_label_size -1;
    [[maybe_unused]] qint8 row_next_label_start = row_back_button_end + 2;
    [[maybe_unused]] qint8 row_next_label_size = 1;
    [[maybe_unused]] qint8 row_next_label_end = row_next_label_start + row_next_label_size -1;

    // NEXT PIECE LABEL
    [[maybe_unused]] qint8 col_next_piece_start = col_board_end + 2;
    [[maybe_unused]] qint8 col_next_piece_size = 2;
    [[maybe_unused]] qint8 col_next_piece_end = col_next_piece_start + col_next_piece_size -1;
    [[maybe_unused]] qint8 row_next_piece_start = row_next_label_end + 1;
    [[maybe_unused]] qint8 row_next_piece_size = 1;
    [[maybe_unused]] qint8 row_next_piece_end = row_next_piece_start + row_next_piece_size -1;

    // SCORE LABEL
    [[maybe_unused]] qint8 col_score_label_start = col_board_end + 1;
    [[maybe_unused]] qint8 col_score_label_size = 2;
    [[maybe_unused]] qint8 col_score_label_end = col_score_label_start + col_score_label_size -1;
    [[maybe_unused]] qint8 row_score_label_start = row_next_piece_end + 2;
    [[maybe_unused]] qint8 row_score_label_size = 1;
    [[maybe_unused]] qint8 row_score_label_end = row_score_label_start+row_score_label_size-1;

    // BEST SCORE LABEL
    [[maybe_unused]] qint8 col_best_score_label_start = col_score_label_end + 2;
    [[maybe_unused]] qint8 col_best_score_label_size = 1;
    [[maybe_unused]] qint8 col_best_score_label_end = col_best_score_label_start + col_best_score_label_size -1;
    [[maybe_unused]] qint8 row_best_score_label_start = row_next_piece_end + 3;
    [[maybe_unused]] qint8 row_best_score_label_size = 1;
    [[maybe_unused]] qint8 row_best_score_label_end = row_best_score_label_start+row_best_score_label_size-1;

    // SCORE LCD
    [[maybe_unused]] qint8 col_score_lcd_start = col_board_end + 1;
    [[maybe_unused]] qint8 col_score_lcd_size = 2;
    [[maybe_unused]] qint8 col_score_lcd_end = col_score_lcd_start + col_score_lcd_size -1;
    [[maybe_unused]] qint8 row_score_lcd_start = row_score_label_end + 1;
    [[maybe_unused]] qint8 row_score_lcd_size = 1;
    [[maybe_unused]] qint8 row_score_lcd_end = row_score_lcd_start+row_score_lcd_size-1;

    // START BUTTON
    [[maybe_unused]] qint8 col_start_button_start = col_board_end + 1;
    [[maybe_unused]] qint8 col_start_button_size = 2;
    [[maybe_unused]] qint8 col_start_button_end = col_start_button_start + col_start_button_size -1;
    [[maybe_unused]] qint8 row_start_button_start = row_score_lcd_end + 2;
    [[maybe_unused]] qint8 row_start_button_size = 1;
    [[maybe_unused]] qint8 row_start_button_end = row_start_button_start+row_start_button_size-1;

    // PAUSE BUTTON
    [[maybe_unused]] qint8 col_pause_button_start = col_board_end + 3;
    [[maybe_unused]] qint8 col_pause_button_size = 2;
    [[maybe_unused]] qint8 col_pause_button_end = col_start_button_start + col_start_button_size -1;
    [[maybe_unused]] qint8 row_pause_button_start = row_score_lcd_end + 2;
    [[maybe_unused]] qint8 row_pause_button_size = 1;
    [[maybe_unused]] qint8 row_pause_button_end = row_start_button_start+row_start_button_size-1;

    // WINDOW
    [[maybe_unused]] qint8 row_layout_end = 8;
    [[maybe_unused]] qint8 col_layout_end = col_pause_button_end + 2;

    QGridLayout *layout = new QGridLayout;

    layout->addWidget(title_label_, 0, 0, 1, col_layout_end+1);
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
    layout->setRowStretch(row_score_lcd_start+1, 4);

    layout->addWidget(best_score_label_, row_best_score_label_start, col_best_score_label_start, row_best_score_label_size, col_best_score_label_size);
    layout->addWidget(start_game_button_, row_start_button_start, col_start_button_start, row_start_button_size, col_start_button_size);

    layout ->addWidget(pause_restart_button_, row_pause_button_start, col_pause_button_start, row_pause_button_size, col_pause_button_size);

    setLayout(layout);

    // Updating real space in layout and then shrink the size to make it multiple of squares
    QSize board_size_layout = getSizeFromCellToCell(layout, row_board_start, col_board_start, row_board_end, col_board_end);
    // Desired size: QSize(326, 372)
    board_->setBoardSize(board_size_layout);

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
    QFont font = label->font();
    font.setBold(true);
    font.setPointSize(9);
    label->setFont(font);
    return label;
}


QSize TetrisWindow::getWidgetSize(){
    return original_widget_size_;
}


void TetrisWindow::handleStartResetButtonClicked(){
    if(!is_started_){
        pause_restart_button_->setEnabled(true);

        start_game_button_->setText("Reset Game");
        is_started_ = true;
        emit gameStarted();
    }else{
        start_game_button_->setText("Start");
        pause_restart_button_->setEnabled(false);
        pause_restart_button_->setText("&Pause");
        is_started_ = false;
        is_paused_ = false;
        emit gameResetted();
    }

    // Setting back focus to main board
    board_->setFocus();

}

void TetrisWindow::handlePauseGame(){
    if(!is_started_)
        return;

    pause_restart_button_->setText("Play");
    is_paused_ = true;
    emit gamePaused();
}

void TetrisWindow::handleResumeGame(){
    if(!is_paused_)
        return;

    pause_restart_button_->setText("Pause");
    is_paused_ = false;
    emit gameResumed();
}

void TetrisWindow::handlePauseRestartButtonClicked(){
    if(!is_started_)
        return;

    if(!is_paused_){
        handlePauseGame();
    }else{
        handleResumeGame();
    }

    // Setting back focus to main board
    board_->setFocus();
}


QSize TetrisWindow::getSizeFromCellToCell(QGridLayout* layout, int from_row, int from_column, int to_row, int to_column){

    layout->activate();

    // Computing the distance from 2 cells
    QRect from_rect = layout->cellRect(from_row, from_column);
    QRect to_rect = layout->cellRect(to_row, to_column);

    int width = to_rect.right() - from_rect.left();
    int height = to_rect.bottom() - from_rect.top();

    // qDebug() << "getSizeFromCellToCell [width, height]: [" << width << ", " << height << "]";

    return QSize(width, height);
}
