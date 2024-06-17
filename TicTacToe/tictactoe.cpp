#include "tictactoe.h"
#include "ui_tictactoe.h"

TicTacToe::~TicTacToe()
{
    delete ui;
}

TicTacToe::TicTacToe(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TicTacToe)
{
    ui->setupUi(this);
    front_button_names_ = {{"left"}, {"center"}, {"right"}};
    back_button_names_ = {{"top"}, {"center"}, {"bottom"}};
    current_player_ = 'O';
    // Storing x and o icons games
    x_icon_ = QIcon(":/Images/TicTacToe/x_icon.png");
    o_icon_ = QIcon(":/Images/TicTacToe/o_icon.png");

    initializeBoard();
    go_back_button_ = ui->goBackButton;

    connect(go_back_button_, &QPushButton::released, this, &TicTacToe::onGoBackButtonClicked);

}

void TicTacToe::onGoBackButtonClicked(){
    emit goBackToMainMenu();
}

void TicTacToe::initializeBoard(){
    board_ = QVector<QVector<char>>(3, QVector<char>(3, ' '));

    // Linking ui button to variable
    qint8 i{0}, j{0};

    for(QString back_name: back_button_names_){
        QVector<QPushButton*> row;
        for(QString front_name: front_button_names_){
            QString name = front_name + "_" +  back_name;
            QPushButton *button = findChild<QPushButton*>(name);
            connect(button, &QPushButton::released, this, &TicTacToe::handleBoardButtonClick);
            row.append(button);
            j+=1;
        }
        board_buttons_.append(row);

        i+=1;
    }

}

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
    if (row != -1 && col != -1 && board_[row][col] == ' ') {
        board_[row][col] = current_player_;       // Update the game board
        button->setIcon(current_player_ == 'O' ? o_icon_ : x_icon_);  // Update the button text

        QSize icon_size(button->width(), button->height());
        button->setIconSize(icon_size);


        // Check the game state
        /*
         * MAKE THE CHOSEN PLAYER MORE CONSISTANT INSTEAD OF PASSING 'O'
        */
        /*
         * ANALYZE PLAYER STATUS, WHICH WILL CHECK THE GAME STATE FOR THE SPECIFIC PLAYER
         */

        gameState current_player_status, robot_status;
        current_player_status = checkGameStateForPlayer(current_player_);
        robot_status = checkGameStateForPlayer(current_player_ == 'O' ? 'X' : 'O');


        /* switchPlayer();*/                        // Switch player
    }


}

void TicTacToe::handlePlayerAction(const gameState &action_result){

    switch (action_result) {
    case gameState::won:{
        std::cout << "WON" << std::endl;
        break;
    }
    case gameState::lost:{
        std::cout << "LOST" << std::endl;
        break;
    }
    case gameState::draw:{
        std::cout << "DRAW" << std::endl;
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

gameState TicTacToe::checkGameStateForPlayer(char player){
    // Checking consistancy between possible players
    if (player != 'O' && player != 'X'){
        return gameState::error;
    }

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

    return gameState::playing;
}
