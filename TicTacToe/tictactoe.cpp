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
    , score_player_(0)
    , score_computer_(0)
{
    ui->setupUi(this);
    front_button_names_ = {{"left"}, {"center"}, {"right"}};
    back_button_names_ = {{"top"}, {"center"}, {"bottom"}};

    player_icon_ = 'O';
    player_icon_ == 'O' ? computer_icon_ = 'X' : computer_icon_ = 'O';

    // Storing x and o icons games
    x_icon_ = QIcon(":/Images/TicTacToe/x_icon.png");
    o_icon_ = QIcon(":/Images/TicTacToe/o_icon.png");

    go_back_button_ = ui->goBackButton;
    reset_board_button_ = ui->resetBoardButton;
    reset_score_button_ = ui->resetScoreButton;

    lcd_player_ = ui->lcdPlayer;
    lcd_computer_ = ui->lcdComputer;

    initializeBoard();

    connect(go_back_button_, &QPushButton::released, this, &TicTacToe::onGoBackButtonClicked);
    connect(reset_board_button_, &QPushButton::released, this, &TicTacToe::handleResetBoardClicked);
    connect(reset_score_button_, &QPushButton::released, this, &TicTacToe::handleResetScoreClicked);
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
/**
 *   @brief Initialize the board, resetting it. Useful to reset the game.
 */
void TicTacToe::initializeBoard(){
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
        board_[row][col] = player_icon_;       // Update the game board
        button->setIcon(player_icon_ == 'O' ? o_icon_ : x_icon_);  // Update the button text

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
            return gameState::won;
        }

        // COLUMN
        if( board_[0][i] == player &&
            board_[1][i] == player &&
            board_[2][i] == player){
            return gameState::won;
        }
    }

    // MAIN DIAG
    if( board_[0][0] == player &&
        board_[1][1] == player &&
        board_[2][2] == player){
        return gameState::won;
    }

    // SECOND DIAG
    if( board_[0][2] == player &&
        board_[1][1] == player &&
        board_[2][0] == player){
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
