#include "tictactoe.h"
#include "qthread.h"
#include "ui_tictactoe.h"

TicTacToe::~TicTacToe()
{
    delete ui;
}

TicTacToe::TicTacToe(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TicTacToe)
    , change_icon_dialog_(this)
    // , original_widget_size_(this->size())
    , score_player_(0)
    , score_computer_(0)

{
    ui->setupUi(this);

    original_widget_size_ = this->size();
    // change_icon_dialog_. = new QDialog(this);

    front_button_names_ = {{"left"}, {"center"}, {"right"}};
    back_button_names_ = {{"top"}, {"center"}, {"bottom"}};

    player_icon_ = 'O';

    // Buttons and ui objects
    go_back_button_ = ui->goBackButton;
    reset_board_button_ = ui->resetBoardButton;
    reset_score_button_ = ui->resetScoreButton;
    change_icon_button_ = ui->changeIconButton;

    lcd_player_ = ui->lcdPlayer;
    lcd_computer_ = ui->lcdComputer;

    comment_frame_ = ui->commentFrame;

    initializeBoard();
    initializeChangeIconDialog();

    // Button connections
    connect(go_back_button_, &QPushButton::released, this, &TicTacToe::onGoBackButtonClicked);
    connect(reset_board_button_, &QPushButton::released, this, &TicTacToe::handleResetBoardClicked);
    connect(reset_score_button_, &QPushButton::released, this, &TicTacToe::handleResetScoreClicked);
    connect(change_icon_button_, &QPushButton::released, this, &TicTacToe::handleChangeIconClicked);
}

/**
 *   @brief Initialize the board, resetting it. Useful to reset the game.
 */
void TicTacToe::initializeBoard(){

    player_icon_ == 'O' ? computer_icon_ = 'X' : computer_icon_ = 'O';

    // Update different icons
    initializeIcons();

    // Resetting variables
    board_ = QVector<QVector<char>>(3, QVector<char>(3, ' '));
    board_buttons_.clear();
    free_spots_.clear();

    for(qint8 i=0; i<9; i++){ free_spots_.insert(i); }

    // Linking ui button to variable
    for(const QString &back_name: back_button_names_){
        QVector<QPushButton*> row;
        for(const QString &front_name: front_button_names_){
            QString name = front_name + "_" +  back_name;
            QPushButton *button = findChild<QPushButton*>(name);
            clearIconFromButton(button);
            enableButton(button);
            connect(button, &QPushButton::released, this, &TicTacToe::handleBoardButtonClick);
            row.append(button);
        }
        board_buttons_.append(row);

    }

}

void TicTacToe::initializeIcons(){
    x_icon_path_ = ":/Images/TicTacToe/x_icon_";
    o_icon_path_ = ":/Images/TicTacToe/o_icon_";

    x_black_icon_path_ = x_icon_path_ + "black.png";
    o_black_icon_path_ = o_icon_path_ + "black.png";
    x_red_icon_path_ = x_icon_path_ + "red.png";
    o_red_icon_path_ = o_icon_path_ + "red.png";
    x_green_icon_path_ = x_icon_path_ + "green.png";
    o_green_icon_path_ = o_icon_path_ + "green.png";

    x_icon_ = QIcon(x_black_icon_path_);
    x_icon_.addPixmap(QPixmap(x_black_icon_path_), QIcon::Disabled);

    o_icon_ = QIcon(o_black_icon_path_);
    o_icon_.addPixmap(QPixmap(o_black_icon_path_), QIcon::Disabled);

    if(player_icon_ == 'O'){
        player_winning_icon_ = QIcon(o_green_icon_path_);
        player_winning_icon_.addPixmap(QPixmap(o_green_icon_path_), QIcon::Disabled);

        computer_winning_icon_ = QIcon(x_red_icon_path_);
        computer_winning_icon_.addPixmap(QPixmap(x_red_icon_path_), QIcon::Disabled);
    }else{
        player_winning_icon_ = QIcon(x_green_icon_path_);
        player_winning_icon_.addPixmap(QPixmap(x_green_icon_path_), QIcon::Disabled);

        computer_winning_icon_ = QIcon(o_red_icon_path_);
        computer_winning_icon_.addPixmap(QPixmap(o_red_icon_path_), QIcon::Disabled);
    }
}

void TicTacToe::initializeChangeIconDialog(){
    change_icon_dialog_.setWindowTitle("Choose your Icon");
    change_icon_dialog_.resize(200, 100);

    // Create the buttons
    QPushButton *circle_button = new QPushButton("", &change_icon_dialog_);
    QPushButton *cross_button = new QPushButton("", &change_icon_dialog_);
    QIcon o_icon(o_black_icon_path_);
    QIcon x_icon(x_black_icon_path_);

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
    connect(circle_button, &QPushButton::clicked, this, &TicTacToe::changeIconCircle);
    connect(cross_button, &QPushButton::clicked, this, &TicTacToe::changeIconCross);
}

void TicTacToe::onGoBackButtonClicked(){
    emit goBackToMainMenu();
}

void TicTacToe::handleResetBoardClicked(){
    std::cout << "Resetting board" << std::endl;
    initializeBoard();
    reset_board_button_->setDefault(false);
}

void TicTacToe::handleResetScoreClicked(){
    resetLcdScores();
}

void TicTacToe::handleChangeIconClicked(){
    // Run dialog box
    change_icon_dialog_.exec();
}

void TicTacToe::changeIconCircle(){
    player_icon_ = 'O';
    initializeBoard();
    change_icon_dialog_.close();
}

void TicTacToe::changeIconCross(){
    player_icon_ = 'X';
    initializeBoard();
    change_icon_dialog_.close();

}

/**
 * @brief Handle the click event for any click on the board game.
 * Implement the main logic.
 */
void TicTacToe::handleBoardButtonClick(){
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    int row = -1, col = -1;

    // Find which button was clicked
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board_buttons_[i][j] == button) {
                row = i;
                col = j;
                break;
            }
        }
    }

    // Check if the cell is empty and fill with proper icon

    /*
     * Below code reused below. Better to implement a fucntion
     */
    if (row != -1 && col != -1 && board_[row][col] == ' ') {
        board_[row][col] = player_icon_;
        button->setIcon(player_icon_ == 'O' ? o_icon_ : x_icon_);

        QSize icon_size(button->width(), button->height());
        button->setIconSize(icon_size);
        disableButton(button);
        free_spots_.erase(idxsToSpot(row,col));


        // Player action
        gameState player_icon_status, robot_status;
        player_icon_status = checkGameStateForPlayer(player_icon_);
        handlePlayerAction(player_icon_status, player_icon_);

        // Bot action
        if (player_icon_status != gameState::won && player_icon_status != gameState::draw){
            randomBotAction(computer_icon_);
            robot_status = checkGameStateForPlayer(computer_icon_);
            handlePlayerAction(robot_status, computer_icon_);
        }
    }
}

void TicTacToe::updateLcdScores(){
    lcd_computer_->display(score_computer_);
    lcd_player_->display(score_player_);
}

void TicTacToe::resetLcdScores(){
    qint8 zero = 0;
    score_computer_ = zero;
    score_player_ = zero;
    lcd_computer_->display(zero);
    lcd_player_->display(zero);
}

void TicTacToe::randomBotAction(const char player){
    if(free_spots_.empty())
        return;

    std::mt19937 gen( std::random_device{}() );
    qint8 el;
    std::sample(free_spots_.begin(), free_spots_.end(), &el, 1, gen);

    // Mark button with icon
    std::pair<qint8, qint8> idxs = spotToIdxs(el);
    qint8 row = idxs.first;
    qint8 col = idxs.second;
    board_[row][col] = player ;

    QPushButton *button = board_buttons_[row][col];

    // Disable button
    button->setIcon(player == 'O' ? o_icon_ : x_icon_);  // Update the button text

    QSize icon_size(button->width(), button->height());
    button->setIconSize(icon_size);
    disableButton(button);
    free_spots_.erase(idxsToSpot(row,col));

}

void TicTacToe::handlePlayerAction(const gameState &action_result, const char player){
    qint8 *score;
    player == player_icon_ ? score = &score_player_ : score = &score_computer_;

    switch (action_result) {
    case gameState::won:{
        std::cout << "WON" << std::endl;
        // Updating score
        *score+=1;
        QString p((QChar(player)));
        comment_frame_->setText(p + " WON");

        reset_board_button_->setDefault(true);
        updateLcdScores();

        // Disable all the available left spots
        for(const auto& el: free_spots_){
            std::pair<qint8, qint8> idxs = spotToIdxs(el);
            qint8 row = idxs.first;
            qint8 col = idxs.second;
            QPushButton *tmp_button = board_buttons_[row][col];
            disableButton(tmp_button);
        }
        break;
    }
    case gameState::lost:{
        std::cout << "LOST" << std::endl;
        break;
    }
    case gameState::draw:{
        std::cout << "DRAW" << std::endl;
        reset_board_button_->setDefault(true);
        break;
    }
    case gameState::playing:{
        std::cout << "PLAYING" << std::endl;
        break;
    }
    case gameState::error:{
        std::cout << "ERROR" << std::endl;
        break;
    }
    }
}

/**
 * @brief Checks if the desired player won the game.
 * @param player current player
 * @return status of the game for the selected player.
 */
gameState TicTacToe::checkGameStateForPlayer(char player){
    // Checking consistancy between possible players
    if (player != 'O' && player != 'X'){
        return gameState::error;
    }

    // Checking for WINNING
    //Manually handling result check
    for (int i=0; i<3; i++){
        // ROW
        if( board_[i][0] == player &&
            board_[i][1] == player &&
            board_[i][2] == player){

            QVector<std::pair<qint8, qint8>> cells = {
                std::pair<qint8,qint8>(i,0),
                std::pair<qint8,qint8>(i,1),
                std::pair<qint8,qint8>(i,2)};

            markWinningButtons(cells);

            return gameState::won;
        }

        // COLUMN
        if( board_[0][i] == player &&
            board_[1][i] == player &&
            board_[2][i] == player){

            QVector<std::pair<qint8, qint8>> cells = {
                std::pair<qint8,qint8>(0,i),
                std::pair<qint8,qint8>(1,i),
                std::pair<qint8,qint8>(2,i)};

            markWinningButtons(cells);

            return gameState::won;
        }
    }

    // MAIN DIAG
    if( board_[0][0] == player &&
        board_[1][1] == player &&
        board_[2][2] == player){

        QVector<std::pair<qint8, qint8>> cells = {
                std::pair<qint8,qint8>(0,0),
                std::pair<qint8,qint8>(1,1),
                std::pair<qint8,qint8>(2,2)};

        markWinningButtons(cells);
        return gameState::won;
    }

    // SECOND DIAG
    if( board_[0][2] == player &&
        board_[1][1] == player &&
        board_[2][0] == player){

        QVector<std::pair<qint8, qint8>> cells = {
              std::pair<qint8,qint8>(0,2),
              std::pair<qint8,qint8>(1,1),
              std::pair<qint8,qint8>(2,0)};

        markWinningButtons(cells);
        return gameState::won;
    }

    // Checking for DRAWING, if not won
    if(free_spots_.empty()){
        return gameState::draw;
    }

    return gameState::playing;
}

/**
 * @brief Disable the button
 * @param button desired button to disable
 */
void TicTacToe::disableButton(QPushButton *button){
    button->setEnabled(false);
}

void TicTacToe::enableButton(QPushButton *button){
    button->setEnabled(true);
}

void TicTacToe::clearIconFromButton(QPushButton *button){
    button->setIcon(QIcon());
}

void TicTacToe::markWinningButtons(const QVector<std::pair<qint8, qint8>> &idxs_vect){
    for(const std::pair<qint8, qint8> &idxs : idxs_vect){
        qint8 row = idxs.first;
        qint8 col = idxs.second;
        QIcon icon;
        QPushButton *winning_button = board_buttons_[row][col];
        board_[row][col] == player_icon_ ? icon = player_winning_icon_ : icon = computer_winning_icon_;
        disableButton(winning_button);
        winning_button->setIcon(icon);
    }
}

qint8 TicTacToe::idxsToSpot(const qint8 &r, const qint8 &c){
    return (3*r + c);
}

std::pair<qint8, qint8> TicTacToe::spotToIdxs(const qint8 &s){
    if (s < 0 || s > 8) {
        throw std::out_of_range("Number must be between 0 and 8");
    }
    qint8 row = s / 3;
    qint8 col = s % 3;
    return std::make_pair(row, col);
}

QSize TicTacToe::getWidgetSize(){
    return original_widget_size_;
}
