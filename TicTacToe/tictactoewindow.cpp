#include "tictactoewindow.h"

TicTacToeWindow::TicTacToeWindow(QWidget *parent)
    : QWidget{parent}
    , change_icon_dialog_(this)
    , original_widget_size_(560, 440)
    , player_score_(0)
    , computer_score_(0)
{
    // Title
    title_label_ = new QLabel("Tic Tac Toe");
    QFont title_font("FreeMono", 26, QFont::Bold, 1);
    title_label_->setFont(title_font);
    title_label_->setAlignment(Qt::AlignCenter);
    // title_label_->setFrameStyle(2);

    comment_label_ = new QLabel();
    // As default, player start every time platform is loaded for the first time
    updateDisplayWithText("YOU START");

    icon_player_label_ = new QLabel();
    lcd_player_ = new QLCDNumber(3);
    lcd_computer_ = new QLCDNumber(3);

    game_board_ = new TicTacToeBoard(nullptr,
                                     ":/TicTacToe/Images/TicTacToe/x_icon_black.png",
                                     ":/TicTacToe/Images/TicTacToe/o_icon_black.png"
                                     );
    game_board_->setCurrentIconLabel(icon_player_label_);
    game_board_->setFixedSize(240, 240);

    // Level window list
    QFont fontCombo;
    fontCombo.setPointSize(8);
    level_combo_box_ = new QComboBox();
    level_combo_box_->addItem("Easy");
    level_combo_box_->addItem("Medium");
    level_combo_box_->addItem("Hard");
    level_combo_box_->setFont(fontCombo);
    level_combo_box_->setCurrentIndex(1);
    level_combo_box_->setMaximumWidth(70);

    initializeButtons();
    updatePlayerIconLabel();
    initializeChangeIconDialog();
    initializeWindow();

    connect(go_back_button_, &QPushButton::clicked, this, &TicTacToeWindow::onGoBackButtonClicked);

    connect(change_icon_button_, &QPushButton::clicked, this, &TicTacToeWindow::handleChangeIconClicked);
    connect(game_board_, &TicTacToeBoard::updateCommentLabel, this, &TicTacToeWindow::updateDisplayWithText);
    connect(reset_board_button_, &QPushButton::clicked, this, &TicTacToeWindow::handleResetBoardClicked);
    connect(reset_board_button_, &QPushButton::clicked, game_board_, &TicTacToeBoard::handleResetBoardClicked);
    connect(reset_score_button_, &QPushButton::clicked, this, &TicTacToeWindow::handleResetScoreClicked);
    connect(this, &TicTacToeWindow::iconCrossSelected, game_board_, &TicTacToeBoard::changeIconToCross);
    connect(this, &TicTacToeWindow::iconCircleSelected, game_board_, &TicTacToeBoard::changeIconToCircle);
    connect(level_combo_box_, &QComboBox::activated, this, &TicTacToeWindow::handleChangeLevelBox);
    connect(game_board_, &TicTacToeBoard::gameIsOver, this, &TicTacToeWindow::handleFinishedGame);

}

/**
 * @brief Handles the "Reset Score" button click event.
 *
 * This function resets the scores displayed on the LCD screens to their initial values.
 */
void TicTacToeWindow::handleResetScoreClicked(){
    resetLcdScores();
    // Resetting also the board
    reset_board_button_->click();
}

/**
 * @brief Resets the scores displayed on the LCD screens to zero.
 *
 * This function sets both the computer's and player's scores to zero and updates
 * the corresponding LCD screens to display these reset scores.
 */
void TicTacToeWindow::resetLcdScores(){
    qint8 zero = 0;
    computer_score_ = zero;
    player_score_ = zero;
    lcd_computer_->display(zero);
    lcd_player_->display(zero);
}

void TicTacToeWindow::handleFinishedGame(const gameState &game_state, const char &player)
{
    reset_board_button_->setDefault(true);
    reset_board_button_->setText("New Game");

    switch (game_state) {
    case gameState::won:
        if(player == game_board_->getPlayerChar()){
            player_score_++;
            lcd_player_->display(player_score_);
        }else{
            computer_score_++;
            lcd_computer_->display(computer_score_);
        }
        break;
    default:
        break;
    }


}


void TicTacToeWindow::handleChangeLevelBox()
{
    game_board_->setGameLevel(static_cast<gameLevel>(level_combo_box_->currentIndex()));
    game_board_->handleResetBoardClicked();
}


void TicTacToeWindow::updateDisplayWithText(const QString &text)
{
    qDebug() << "Emit received";
    QFont commentFont = QFont("Noto sans", 14);
    comment_label_->setFont(commentFont);
    comment_label_->setAlignment(Qt::AlignCenter);
    comment_label_->setText(text);
    comment_label_->setAlignment(Qt::AlignCenter);
}

/**
 * @brief Updates the player icon label with the current player icon.
 *
 * This function converts the player's current icon to a pixmap and updates
 * the player icon label with the scaled pixmap.
 */
void TicTacToeWindow::updatePlayerIconLabel(){
    // Converting Icon to pixmap
    QIcon iconPlayer = game_board_->getPlayerIcon();
    QPixmap pixmap = iconPlayer.pixmap(icon_player_label_->size());

    icon_player_label_->setPixmap(pixmap);
    icon_player_label_->setScaledContents(true);
}



/**
 * @brief Changes the player's icon to a circle ('O') and reinitializes the board.
 *
 * This function sets the player's icon to a circle, updates the corresponding icons,
 * reinitializes the game board, closes the change icon dialog, and updates the player icon label.
 */
void TicTacToeWindow::changeIconCircle(){
    // player_icon_char_ = 'O';
    // player_icon_ = o_icon_;
    // computer_icon_ = x_icon_;
    // initializeBoard();
    emit iconCircleSelected();
    change_icon_dialog_.close();
    updatePlayerIconLabel();
}

/**
 * @brief Changes the player's icon to a cross ('X') and reinitializes the board.
 *
 * This function sets the player's icon to a cross, updates the corresponding icons,
 * reinitializes the game board, closes the change icon dialog, and updates the player icon label.
 */
void TicTacToeWindow::changeIconCross(){
    // player_icon_char_ = 'X';
    // player_icon_ = x_icon_;
    // computer_icon_ = o_icon_;
    // initializeBoard();
    emit iconCrossSelected();
    change_icon_dialog_.close();
    updatePlayerIconLabel();
}

void TicTacToeWindow::handleResetBoardClicked()
{
    reset_board_button_->setDefault(false);
    reset_board_button_->setText("Reset Board");
}

QFrame* TicTacToeWindow::initializeDialogArea()
{
    // Outer frame
    QFrame *outerFrame = new QFrame();
    outerFrame->setFrameStyle(QFrame::Box | QFrame::Sunken);
    outerFrame->setLineWidth(1);
    outerFrame->setMidLineWidth(0);
    QVBoxLayout *outerLayout = new QVBoxLayout(outerFrame);

    // Label for first text
    QLabel *label1 = new QLabel("Welcome to the game:");
    // label1->setFrameStyle(2);
    label1->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    QFont label1Font = QFont("Noto sans", 11, -1, true);
    label1Font.setBold(true);
    label1->setFont(label1Font);

    // comment_label_->setFrameStyle(2);

    outerLayout->addWidget(label1);
    outerLayout->addWidget(comment_label_);
    outerLayout->setStretch(1,5);

    return outerFrame;
}

void TicTacToeWindow::initializeButtons()
{
    go_back_button_ = new QPushButton("Go back");
    go_back_button_->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::GoPrevious));

    change_icon_button_ = new QPushButton("Change icon");
    change_icon_button_->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::ContactNew));
    change_icon_button_->setLayoutDirection(Qt::LayoutDirection::RightToLeft);

    reset_board_button_ = new QPushButton("Reset Board");
    reset_board_button_->setSizeIncrement(91, 31);
    reset_board_button_->setMinimumHeight(33);

    reset_score_button_ = new QPushButton("Reset score");
    reset_score_button_->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::DialogWarning));

}

QVBoxLayout* TicTacToeWindow::setIconLayout()
{
    QVBoxLayout *changeIconLayout = new QVBoxLayout();
    QHBoxLayout *iconPLayerLayout = new QHBoxLayout();

    QLabel *youLabel = new QLabel("You");
    QFont labelFont = youLabel->font();
    labelFont.setBold(true);
    labelFont.setPointSize(15);
    // youLabel->setFrameStyle(2);
    youLabel->setFont(labelFont);
    youLabel->setAlignment(Qt::AlignBottom | Qt::AlignCenter);

    icon_player_label_->setFixedSize(32,32);
    // icon_player_label_->setFrameStyle(3);
    // icon_player_label_->

    // HORIZONTAL LAYOUT
    iconPLayerLayout->addWidget(icon_player_label_);
    iconPLayerLayout->setAlignment(Qt::AlignHCenter);

    // VERTICAL LAYOUT
    changeIconLayout->addWidget(youLabel);
    changeIconLayout->addLayout(iconPLayerLayout);
    changeIconLayout->addWidget(change_icon_button_);
    changeIconLayout->setAlignment(Qt::AlignCenter);

    return changeIconLayout;
}

QVBoxLayout* TicTacToeWindow::setLCDScoreLayout()
{
    QVBoxLayout *LCDScoreLayout = new QVBoxLayout();

    // QLCDNumber *lcdPlayer = new QLCDNumber(3);
    lcd_player_->setFrameStyle(QFrame::Raised | QFrame::Box);
    lcd_player_->setFixedSize(70,50);
    // QLCDNumber *lcdComputer = new QLCDNumber(3);
    lcd_computer_->setFrameStyle(QFrame::Raised | QFrame::Box);
    lcd_computer_->setFixedSize(70,50);

    QFont playerFont = QFont("Noto Sans", 9);
    QLabel *playerText = new QLabel("player");
    playerText->setFont(playerFont);
    playerText->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);

    QLabel *computerText = new QLabel("computer");
    computerText->setFont(playerFont);
    computerText->setAlignment(Qt::AlignHCenter | Qt::AlignTop);

    QLabel *VSText = new QLabel("VS");
    VSText->setFont(QFont("Noto Sans", 20, QFont::Bold));
    // VSText->setFrameStyle(2);
    VSText->setAlignment(Qt::AlignCenter);
    VSText->setMaximumHeight(30);

    LCDScoreLayout->addWidget(playerText);
    LCDScoreLayout->addWidget(lcd_player_, 20);
    LCDScoreLayout->addWidget(VSText);
    LCDScoreLayout->setStretch(2, 0);
    LCDScoreLayout->addWidget(lcd_computer_, 20);
    LCDScoreLayout->addWidget(computerText);
    LCDScoreLayout->setAlignment(Qt::AlignCenter);

    return LCDScoreLayout;
}

void TicTacToeWindow::initializeWindow()
{
    QVBoxLayout *changeIconLayout = setIconLayout();
    QVBoxLayout *LCDScoreLayout = setLCDScoreLayout();
    QFrame *commentAreaFrame = initializeDialogArea();

    // CHANGE ICON LAYOUT
    [[maybe_unused]] qint8 row_change_icon_layout_start = 3;
    [[maybe_unused]] qint8 row_change_icon_layout_size = 1;
    [[maybe_unused]] qint8 row_change_icon_layout_end = row_change_icon_layout_size+row_change_icon_layout_start-1;
    [[maybe_unused]] qint8 col_change_icon_layout_start = 0;
    [[maybe_unused]] qint8 col_change_icon_layout_size = 3;
    [[maybe_unused]] qint8 col_change_icon_layout_end = col_change_icon_layout_size+col_change_icon_layout_start-1;

    // BOARD
    [[maybe_unused]] qint8 row_board_start = 1;
    [[maybe_unused]] qint8 row_board_size = 4;
    [[maybe_unused]] qint8 row_board_end = row_board_size+row_board_start-1;
    [[maybe_unused]] qint8 col_board_start = col_change_icon_layout_end + 1;
    [[maybe_unused]] qint8 col_board_size = 6;
    [[maybe_unused]] qint8 col_board_end = col_board_size+col_board_start-1;

    // GO BACK BUTTON
    [[maybe_unused]] qint8 row_go_back_button_start = 1;
    [[maybe_unused]] qint8 row_go_back_button_size = 1;
    [[maybe_unused]] qint8 row_go_back_button_end = row_go_back_button_size+row_go_back_button_start-1;
    [[maybe_unused]] qint8 col_go_back_button_start = col_board_end + 1;
    [[maybe_unused]] qint8 col_go_back_button_size = 2;
    [[maybe_unused]] qint8 col_go_back_button_end = col_go_back_button_size+col_go_back_button_start-1;

    // RESET SCORE BUTTON
    [[maybe_unused]] qint8 row_reset_score_start = row_go_back_button_start;
    [[maybe_unused]] qint8 row_reset_score_size = 1;
    [[maybe_unused]] qint8 row_reset_score_end = row_reset_score_size+row_reset_score_start-1;
    [[maybe_unused]] qint8 col_reset_score_start = col_go_back_button_end + 1;
    [[maybe_unused]] qint8 col_reset_score_size = 2;
    [[maybe_unused]] qint8 col_reset_score_end = col_reset_score_size+col_reset_score_start-1;


    // RESET BOARD BUTTON
    [[maybe_unused]] qint8 row_reset_button_start = row_board_end + 2;
    [[maybe_unused]] qint8 row_reset_button_size = 1;
    [[maybe_unused]] qint8 row_reset_button_end = row_reset_button_size+row_reset_button_start-1;
    [[maybe_unused]] qint8 col_reset_button_start = col_board_end + 2;
    [[maybe_unused]] qint8 col_reset_button_size = 2;
    [[maybe_unused]] qint8 col_reset_button_end = col_reset_button_size+col_reset_button_start-1;

    // LEVEL COMBO BOX
    [[maybe_unused]] qint8 row_level_box_start = row_reset_button_end + 1;
    [[maybe_unused]] qint8 row_level_box_size = 1;
    [[maybe_unused]] qint8 row_level_box_end = row_level_box_size+row_level_box_start-1;
    [[maybe_unused]] qint8 col_level_box_start = col_board_end + 2;
    [[maybe_unused]] qint8 col_level_box_size = 2;
    [[maybe_unused]] qint8 col_level_box_end = col_level_box_size+col_level_box_start-1;

    // LCD SCORE LAYOUT
    [[maybe_unused]] qint8 row_LCD_score_layout_start = row_board_start+1;
    [[maybe_unused]] qint8 row_LCD_score_layout_size = 3;
    [[maybe_unused]] qint8 row_LCD_score_layout_end = row_LCD_score_layout_size+row_LCD_score_layout_start-1;
    [[maybe_unused]] qint8 col_LCD_score_layout_start = col_board_end + 2;
    [[maybe_unused]] qint8 col_LCD_score_layout_size = 2;
    [[maybe_unused]] qint8 col_LCD_score_layout_end = col_LCD_score_layout_size+col_LCD_score_layout_start-1;

    // WINDOW
    [[maybe_unused]] qint8 row_layout_end = 8;
    [[maybe_unused]] qint8 col_layout_end =col_reset_score_end+1;


    QGridLayout *layout = new QGridLayout;

    layout->addWidget(title_label_, 0, 0, 1, col_layout_end);
    layout->addLayout(changeIconLayout, row_change_icon_layout_start, col_change_icon_layout_start, row_change_icon_layout_size, col_change_icon_layout_size);
    layout->addWidget(game_board_, row_board_start, col_board_start, row_board_size, col_board_size, Qt::AlignCenter);

    layout->addWidget(go_back_button_, row_go_back_button_start, col_go_back_button_start, row_go_back_button_size, col_go_back_button_size);
    layout->addWidget(reset_board_button_, row_reset_button_start, col_reset_button_start, row_reset_button_size, col_reset_button_size);
    // layout->setRowStretch(row_reset_button_start, 2);

    layout->addLayout(LCDScoreLayout, row_LCD_score_layout_start, col_LCD_score_layout_start, row_LCD_score_layout_size, col_LCD_score_layout_size, Qt::AlignCenter | Qt::AlignVCenter);

    layout->addWidget(level_combo_box_, row_level_box_start, col_level_box_start, row_level_box_size, col_level_box_size, Qt::AlignHCenter | Qt::AlignVCenter);
    // layout->setRowStretch(row_level_box_start, 1);

    layout->addWidget(reset_score_button_, row_reset_score_start, col_reset_score_start, row_reset_score_size, col_reset_score_size);
    layout->addWidget(commentAreaFrame, row_board_end+1, col_board_start, 4, col_board_size);

    // layout->addWidget(change_icon_button_, 6,3);

    /*
     * Add level window
     *
     * Add comment frame with relative comment
     *
     * Add LCDs with 'VS' comment
     *
     * Add board game
     */
    // layout->addWidget();

    setLayout(layout);

    setWindowTitle("Tic Tac Toe");
    resize(original_widget_size_);
}

void TicTacToeWindow::initializeChangeIconDialog()
{
    change_icon_dialog_.setWindowTitle("Choose your Icon");
    change_icon_dialog_.resize(200, 100);

    // Create the buttons
    QPushButton *circle_button = new QPushButton("", &change_icon_dialog_);
    QPushButton *cross_button = new QPushButton("", &change_icon_dialog_);

    char playerChar = game_board_->getPlayerChar();
    QIcon o_icon;// = game_board_->getComputerIcon();
    QIcon x_icon;

    if(playerChar == 'X')
    {
        x_icon = game_board_->getPlayerIcon();
        o_icon = game_board_->getComputerIcon();
    }else{
        x_icon = game_board_->getComputerIcon();
        o_icon = game_board_->getPlayerIcon();
    }

    // Desired button size
    qint8 button_size = 80;
    qint8 icon_reduction = 10;

    // Styling the buttons
    circle_button->setFixedSize(button_size, button_size);
    circle_button->setIcon(o_icon);
    circle_button->setIconSize(QSize(button_size - icon_reduction, button_size - icon_reduction));
    cross_button->setFixedSize(button_size, button_size);
    cross_button->setIcon(x_icon);
    cross_button->setIconSize(QSize(button_size - icon_reduction, button_size - icon_reduction));

    // Set up the layout for the dialog
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(circle_button);
    layout->addWidget(cross_button);
    change_icon_dialog_.setLayout(layout);

    // Connect both buttons to signals
    connect(circle_button, &QPushButton::clicked, this, &TicTacToeWindow::changeIconCircle);
    connect(cross_button, &QPushButton::clicked, this, &TicTacToeWindow::changeIconCross);
}

/**
 * @brief Handles the "Change Icon" button click event.
 *
 * This function displays the "Change Icon" dialog box, allowing the player to choose a new icon.
 */
void TicTacToeWindow::handleChangeIconClicked(){
    // Run dialog box
    change_icon_dialog_.exec();
}


/**
 * @brief Handles the "Go Back" button click event.
 *
 * This function emits a signal to navigate back to the main menu when the "Go Back" button is clicked.
 */
void TicTacToeWindow::onGoBackButtonClicked(){
    emit goBackToMainMenu();
}

QSize TicTacToeWindow::getWidgetSize()
{
    return original_widget_size_;
}
