#include "tictactoewindow.h"

TicTacToeWindow::TicTacToeWindow(QWidget *parent)
    : QWidget{parent}
    , player_score_(0)
    , computer_score_(0)
    , widget_size_(560, 440)
    , change_icon_dialog_(this)
{
    setTitle("Tic Tac Toe");

    comment_label_ = new QLabel();
    QFont commentFont = QFont("Noto sans", 14);
    comment_label_->setFont(commentFont);
    comment_label_->setAlignment(Qt::AlignCenter);
    comment_label_->setAlignment(Qt::AlignCenter);

    icon_player_label_ = new QLabel();
    lcd_player_ = new QLCDNumber(3);
    lcd_computer_ = new QLCDNumber(3);

    // Game board initialized, with possibility to choose any image
    game_board_ = new TicTacToeBoard(nullptr,
                                     ":/TicTacToe/Images/TicTacToe/x_icon_black.png",
                                     ":/TicTacToe/Images/TicTacToe/o_icon_black.png"
                                     );
    game_board_->setCurrentIconLabel(icon_player_label_);
    game_board_->setFixedSize(240, 240);

    initializeButtons();
    initializeLevelBox();
    updatePlayerIconLabel();
    initializeChangeIconDialog();
    initializeWindow();

    connect(go_back_button_, &QPushButton::clicked, this, &TicTacToeWindow::onGoBackButtonClicked);
    connect(change_icon_button_, &QPushButton::clicked, this, &TicTacToeWindow::handleChangeIconClicked);
    connect(game_board_, &TicTacToeBoard::updateCommentLabel, this, &TicTacToeWindow::handleUpdateCommentLabel);
    connect(reset_board_button_, &QPushButton::clicked, game_board_, &TicTacToeBoard::handleResetBoardClicked);
    connect(reset_score_button_, &QPushButton::clicked, this, &TicTacToeWindow::handleResetScoreClicked);
    connect(this, &TicTacToeWindow::iconCrossSelected, game_board_, &TicTacToeBoard::changeIconToCross);
    connect(this, &TicTacToeWindow::iconCircleSelected, game_board_, &TicTacToeBoard::changeIconToCircle);
    connect(level_combo_box_, &QComboBox::activated, this, &TicTacToeWindow::handleChangeLevelBox);
    connect(game_board_, &TicTacToeBoard::gameIsOver, this, &TicTacToeWindow::handleFinishedGame);
    connect(game_board_, &TicTacToeBoard::gameStarted, this, &TicTacToeWindow::handleGameStarted);
}


/* INITIALISAZION */
/**
 * @brief Initializes the buttons used in the TicTacToe window.
 *
 * This function creates and configures various QPushButton instances.
 */
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

/**
 * @brief Initializes the dialog for changing the player's icon.
 *
 * This function sets up a dialog with options to choose between a circle or a cross icon.
 * It configures the dialog's title, size, and layout, and styles the buttons with the
 * appropriate icons based on the current player's character. It also connects the buttons
 * to their respective signal handlers for changing the icon.
 */
void TicTacToeWindow::initializeChangeIconDialog()
{
    change_icon_dialog_.setWindowTitle("Choose your Icon");
    change_icon_dialog_.resize(200, 100);

    // Create the buttons
    QPushButton *circle_button = new QPushButton("", &change_icon_dialog_);
    QPushButton *cross_button = new QPushButton("", &change_icon_dialog_);

    char playerChar = game_board_->getPlayerChar();
    QIcon o_icon;
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
 * @brief Initializes the dialog area for the TicTacToe window.
 *
 * This function creates and configures an outer frame with a vertical layout,
 * adds a label with a welcome message, and a comment label to the layout.
 *
 * @return A pointer to the initialized QFrame object representing the dialog area.
 */
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
    label1->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    QFont label1Font = QFont("Noto sans", 11, -1, true);
    label1Font.setBold(true);
    label1->setFont(label1Font);

    outerLayout->addWidget(label1);
    outerLayout->addWidget(comment_label_);
    outerLayout->setStretch(1,5);

    return outerFrame;
}

/**
 * @brief Initializes the level selection combo box.
 *
 * This function creates and configures a QComboBox for selecting the difficulty level,
 * sets its font, default selection, and maximum width.
 */
void TicTacToeWindow::initializeLevelBox()
{
    QFont fontCombo;
    fontCombo.setPointSize(8);

    level_combo_box_ = new QComboBox();
    level_combo_box_->addItem("Easy");
    level_combo_box_->addItem("Medium");
    level_combo_box_->addItem("Hard");
    level_combo_box_->setFont(fontCombo);
    level_combo_box_->setCurrentIndex(1);
    level_combo_box_->setMaximumWidth(70);
}

/**
 * @brief Initializes the main layout and widgets for the TicTacToe window.
 *
 * This function sets up the main grid layout for the TicTacToe window, adding and
 * positioning various widgets such as buttons, labels, and the game board. It also
 * configures the window title and size.
 */
void TicTacToeWindow::initializeWindow()
{
    // As default, player start
    comment_label_->setText("YOU START");

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

    // LCD SCORE LAYOUT
    [[maybe_unused]] qint8 row_comment_label_start = row_board_end+1;
    [[maybe_unused]] qint8 row_comment_label_size = 4;
    [[maybe_unused]] qint8 row_comment_label_end = row_comment_label_size+row_comment_label_size-1;
    [[maybe_unused]] qint8 col_comment_label_start = col_board_start;
    [[maybe_unused]] qint8 col_comment_label_size = col_board_size;
    [[maybe_unused]] qint8 col_comment_label_end = col_comment_label_size+col_comment_label_start-1;

    // WINDOW
    [[maybe_unused]] qint8 row_layout_end = 8;
    [[maybe_unused]] qint8 col_layout_end =col_reset_score_end+1;


    QGridLayout *layout = new QGridLayout;

    layout->addWidget(title_label_, 0, 0, 1, col_layout_end);
    layout->addLayout(changeIconLayout, row_change_icon_layout_start, col_change_icon_layout_start, row_change_icon_layout_size, col_change_icon_layout_size);
    layout->addWidget(game_board_, row_board_start, col_board_start, row_board_size, col_board_size, Qt::AlignCenter);

    layout->addWidget(go_back_button_, row_go_back_button_start, col_go_back_button_start, row_go_back_button_size, col_go_back_button_size);
    layout->addWidget(reset_board_button_, row_reset_button_start, col_reset_button_start, row_reset_button_size, col_reset_button_size);

    layout->addLayout(LCDScoreLayout, row_LCD_score_layout_start, col_LCD_score_layout_start, row_LCD_score_layout_size, col_LCD_score_layout_size, Qt::AlignCenter | Qt::AlignVCenter);

    layout->addWidget(level_combo_box_, row_level_box_start, col_level_box_start, row_level_box_size, col_level_box_size, Qt::AlignHCenter | Qt::AlignVCenter);

    layout->addWidget(reset_score_button_, row_reset_score_start, col_reset_score_start, row_reset_score_size, col_reset_score_size);
    layout->addWidget(commentAreaFrame, row_comment_label_start, col_comment_label_start, row_comment_label_size, col_comment_label_size);

    setLayout(layout);

    setWindowTitle("Tic Tac Toe");
    resize(widget_size_);
}

/* SLOTS */
/**
 * @brief Handles the event when the "Change Icon" button is clicked.
 *
 * This function opens the dialog box for selecting a new icon for the player.
 */
void TicTacToeWindow::handleChangeIconClicked(){
    // Run dialog box
    change_icon_dialog_.exec();
}

/**
 * @brief Handles the change in game difficulty level from the combo box.
 *
 * This function updates the game board's difficulty level based on the selected index
 * from the combo box.
 */
void TicTacToeWindow::handleChangeLevelBox()
{
    game_board_->setGameLevel(static_cast<gameLevel>(level_combo_box_->currentIndex()));
}

/**
 * @brief Handles the end of a game and updates the UI accordingly.
 *
 * This function updates the "Reset Board" button to indicate a new game, and
 * adjusts the scores based on the game state.
 *
 * @param game_state The current state of the game (won, draw, etc.).
 * @param player The character representing the player whose result is being processed.
 */
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
    // case gameState::draw:
    //     comment_label_->setText("DRAW GAME");
    //     break;
    default:
        break;
    }
}

/**
 * @brief Handles the "Game Started" event.
 *
 * This function sets the "Reset Board" button to its default state and updates its text.
 */
void TicTacToeWindow::handleGameStarted()
{
    reset_board_button_->setDefault(false);
    reset_board_button_->setText("Reset Board");
}

/**
 * @brief Handles the "Reset Board" button click event.
 *
 * This function updates the "Reset Board" button to its default state and resets its text.
 */
void TicTacToeWindow::handleResetBoardClicked()
{
    reset_board_button_->setDefault(false);
    reset_board_button_->setText("Reset Board");
}

/**
 * @brief Handles the "Reset Score" button click event.
 *
 * This function resets the scores displayed on the LCD screens to their initial values.
 */
void TicTacToeWindow::handleResetScoreClicked(){
    resetLcdScores();
    reset_board_button_->click();
}

/**
 * @brief Updates the comment label with the specified text.
 *
 * This function sets the text of the comment label to the provided string.
 *
 * @param text The text to be displayed on the comment label.
 */
void TicTacToeWindow::handleUpdateCommentLabel(const QString &text)
{
    comment_label_->setText(text);
}

/**
 * @brief Handles the "Go Back" button click event.
 *
 * This function emits a signal to navigate back to the main menu when the "Go Back" button is clicked.
 */
void TicTacToeWindow::onGoBackButtonClicked(){
    emit goBackToMainMenu();
}

/* OTHER */
/**
 * @brief Handles the event of changing the player icon to a circle.
 *
 * This function emits a signal indicating that the circle icon has been selected,
 * closes the icon change dialog, and updates the player icon label to reflect the new icon.
 */
void TicTacToeWindow::changeIconCircle(){
    emit iconCircleSelected();
    change_icon_dialog_.close();
    updatePlayerIconLabel();
}

/**
 * @brief Handles the event of changing the player icon to a cross.
 *
 * This function emits a signal indicating that the cross icon has been selected,
 * closes the icon change dialog, and updates the player icon label to reflect the new icon.
 */
void TicTacToeWindow::changeIconCross(){
    emit iconCrossSelected();
    change_icon_dialog_.close();
    updatePlayerIconLabel();
}

/**
 * @brief Retrieves the current size of the main window widget.
 *
 * This function returns the size of the widget, to get the dimensions of the TicTacToe window.
 *
 * @return QSize The current size of the window widget.
 */
QSize TicTacToeWindow::getWidgetSize()
{
    return widget_size_;
}

/**
 * @brief Resets the scores displayed on the LCDs screens to zero.
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

/**
 * @brief Creates and sets up the layout for changing icons in the TicTacToe window.
 *
 * This function initializes a vertical layout containing a label indicating "You",
 * a horizontal layout for the player icon, and a button to change the icon.
 * The layouts are configured with appropriate alignment and sizes for proper UI placement.
 *
 * @return A pointer to the QVBoxLayout configured for changing icons.
 */
QVBoxLayout* TicTacToeWindow::setIconLayout()
{
    QVBoxLayout *changeIconLayout = new QVBoxLayout();
    QHBoxLayout *iconPLayerLayout = new QHBoxLayout();

    QLabel *youLabel = new QLabel("You");
    QFont labelFont = youLabel->font();
    labelFont.setBold(true);
    labelFont.setPointSize(15);
    youLabel->setFont(labelFont);
    youLabel->setAlignment(Qt::AlignBottom | Qt::AlignCenter);

    icon_player_label_->setFixedSize(32,32);

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

/**
 * @brief Creates and sets up the layout for displaying scores using LCD displays.
 *
 * This function initializes a vertical layout containing labels and LCD displays
 * for showing the scores of the player and computer. It also includes a label for
 * displaying "VS" between the scores.
 *
 * @return A pointer to the QVBoxLayout configured for LCD score display.
 */
QVBoxLayout* TicTacToeWindow::setLCDScoreLayout()
{
    QVBoxLayout *LCDScoreLayout = new QVBoxLayout();

    lcd_player_->setFrameStyle(QFrame::Raised | QFrame::Box);
    lcd_player_->setFixedSize(70,50);
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

/**
 * @brief Sets the title of the TicTacToe window.
 *
 * This function sets the font and alignment for the title label with the specified title.
 *
 * @param title The title to be displayed in the TicTacToe window.
 */
void TicTacToeWindow::setTitle(const QString &title) {
    // Title
    title_label_ = new QLabel(title);
    QFont title_font("FreeMono", 26, QFont::Bold, 1);
    title_label_->setFont(title_font);
    title_label_->setAlignment(Qt::AlignCenter);
}

/**
 * @brief Updates the player icon label with the current player icon.
 *
 * This function converts the player's icon to a pixmap and updates the label to display the icon.
 * The pixmap is scaled to fit the label's dimensions.
 */
void TicTacToeWindow::updatePlayerIconLabel(){
    // Converting Icon to pixmap
    QIcon iconPlayer = game_board_->getPlayerIcon();
    QPixmap pixmap = iconPlayer.pixmap(icon_player_label_->size());

    icon_player_label_->setPixmap(pixmap);
    icon_player_label_->setScaledContents(true);
}
