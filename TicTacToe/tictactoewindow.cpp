#include "tictactoewindow.h"

TicTacToeWindow::TicTacToeWindow(QWidget *parent)
    : QWidget{parent}
    , original_widget_size_(560, 440)
{
    // Title
    title_label_ = new QLabel("Tic Tac Toe");
    QFont title_font("FreeMono", 26, QFont::Bold, 1);
    title_label_->setFont(title_font);
    title_label_->setAlignment(Qt::AlignCenter);
    // title_label_->setFrameStyle(2);

    // TO BE REMOVED ------
    // // Title
    // game_board_ = new QLabel("BOARD");
    // game_board_->setFrameStyle(2);
    // game_board_->setFixedSize(250, 250);
    game_board_ = new TicTacToeBoard();
    game_board_->setFixedSize(240, 240);

    // ------

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

    comment_frame_ = new QLabel();

    initializeButtons();

    initializeWindow();

    connect(go_back_button_, &QPushButton::clicked, this, &TicTacToeWindow::onGoBackButtonClicked);
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
    label1->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    QFont label1Font = QFont("Noto sans", 11, -1, true);
    label1Font.setBold(true);
    label1->setFont(label1Font);

    outerLayout->addWidget(label1);
    outerLayout->addWidget(comment_frame_);

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

    QLabel *iconPlayerLabel = new QLabel();
    iconPlayerLabel->setFixedSize(32,32);
    iconPlayerLabel->setFrameStyle(3);

    // HORIZONTAL LAYOUT
    iconPLayerLayout->addWidget(iconPlayerLabel);
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

    QLCDNumber *lcdPlayer = new QLCDNumber(3);
    lcdPlayer->setFrameStyle(QFrame::Raised | QFrame::Box);
    lcdPlayer->setFixedSize(70,50);
    QLCDNumber *lcdComputer = new QLCDNumber(3);
    lcdComputer->setFrameStyle(QFrame::Raised | QFrame::Box);
    lcdComputer->setFixedSize(70,50);

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
    LCDScoreLayout->addWidget(lcdPlayer, 20);
    LCDScoreLayout->addWidget(VSText);
    LCDScoreLayout->setStretch(2, 0);
    LCDScoreLayout->addWidget(lcdComputer, 20);
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
