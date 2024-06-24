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
    , score_player_(0)
    , score_computer_(0)
    , easy_randomness_(true)

{
    ui->setupUi(this);

    original_widget_size_ = this->size();

    front_button_names_ = {{"left"}, {"center"}, {"right"}};
    back_button_names_ = {{"top"}, {"center"}, {"bottom"}};

    player_icon_char_ = 'O';
    player_icon_label_  = ui->playerIconLabel;

    starter_player_ = computer_icon_char_;

    // Buttons and ui objects
    go_back_button_ = ui->goBackButton;
    reset_board_button_ = ui->resetBoardButton;
    reset_score_button_ = ui->resetScoreButton;
    change_icon_button_ = ui->changeIconButton;

    lcd_player_ = ui->lcdPlayer;
    lcd_computer_ = ui->lcdComputer;

    comment_frame_ = ui->commentFrame;

    level_combo_box_ = ui->levelComboBox;

    initializeBoard();
    initializeChangeIconDialog();
    initializeLevelComboBox();

    // Button connections
    connect(go_back_button_, &QPushButton::released, this, &TicTacToe::onGoBackButtonClicked);
    connect(reset_board_button_, &QPushButton::released, this, &TicTacToe::handleResetBoardClicked);
    connect(reset_score_button_, &QPushButton::released, this, &TicTacToe::handleResetScoreClicked);
    connect(change_icon_button_, &QPushButton::released, this, &TicTacToe::handleChangeIconClicked);

    connect(level_combo_box_, &QComboBox::activated, this, &TicTacToe::handleChangeLevelBox);
    connect(this, &TicTacToe::botTurn, this, &TicTacToe::performBotAction);
}

/**
 *   @brief Initialize the board, resetting it. Useful to reset the game.
 */
void TicTacToe::initializeBoard(){

    player_icon_char_ == 'O' ? computer_icon_char_ = 'X' : computer_icon_char_ = 'O';

    // Update different icons
    comment_frame_->setText("");
    comment_frame_->setAlignment(Qt::AlignCenter);

    initializeIcons();
    updatePlayerIconLabel();

    reset_board_button_->setText("Reset Board");

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

    // Managing different player start
    if(starter_player_ == computer_icon_char_){
        comment_frame_->setText("BOT started");
        comment_frame_->setAlignment(Qt::AlignCenter);
        emit botTurn();
        starter_player_ = player_icon_char_;
    }else{
        comment_frame_->setText("YOU start");
        comment_frame_->setAlignment(Qt::AlignCenter);
        starter_player_ = computer_icon_char_;
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

    if(player_icon_char_ == 'O'){
        player_icon_ = o_icon_;
        computer_icon_ = x_icon_;
        player_winning_icon_ = QIcon(o_green_icon_path_);
        player_winning_icon_.addPixmap(QPixmap(o_green_icon_path_), QIcon::Disabled);

        computer_winning_icon_ = QIcon(x_red_icon_path_);
        computer_winning_icon_.addPixmap(QPixmap(x_red_icon_path_), QIcon::Disabled);
    }else{
        player_icon_ = x_icon_;
        computer_icon_ = o_icon_;
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

void TicTacToe::initializeLevelComboBox(){
    QFont font;
    font.setPointSize(8);

    level_combo_box_->addItem("Easy");
    level_combo_box_->addItem("Medium");
    level_combo_box_->addItem("Hard");
    level_combo_box_->setFont(font);
    level_combo_box_->setCurrentIndex(1);
    game_level_ = static_cast<gameLevel>(level_combo_box_->currentIndex());
}

void TicTacToe::onGoBackButtonClicked(){
    emit goBackToMainMenu();
}

void TicTacToe::handleResetBoardClicked(){
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
    player_icon_char_ = 'O';
    player_icon_ = o_icon_;
    computer_icon_ = x_icon_;
    initializeBoard();
    change_icon_dialog_.close();
    updatePlayerIconLabel();
}

void TicTacToe::changeIconCross(){
    player_icon_char_ = 'X';
    player_icon_ = x_icon_;
    computer_icon_ = o_icon_;
    initializeBoard();
    change_icon_dialog_.close();
    updatePlayerIconLabel();
}


void  TicTacToe::handleChangeLevelBox(){
    game_level_ = static_cast<gameLevel>(level_combo_box_->currentIndex());
    initializeBoard();
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
    if (row != -1 && col != -1 && board_[row][col] == ' ') {
        board_[row][col] = player_icon_char_;
        markButtonWithIcon(button, player_icon_);
        free_spots_.erase(idxsToSpot(row,col));


        // Player action
        gameState player_icon_status;
        player_icon_status = checkGameStateForPlayer(player_icon_char_);
        handlePlayerAction(player_icon_status, player_icon_char_);

        // Bot action
        if (player_icon_status != gameState::won && player_icon_status != gameState::draw){
            emit botTurn();
        }
    }
}

void TicTacToe::markButtonWithIcon(QPushButton *button, QIcon icon){
    button->setIcon(icon);
    button->setIconSize(button->size());
    disableButton(button);
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

/*
 * passing char player not used. Store directly computer icon as for the player icon
 */
void TicTacToe::randomBotAction(){
    if(free_spots_.empty())
        return;

    // Random number extraction
    std::mt19937 gen( std::random_device{}() );
    qint8 el;
    std::sample(free_spots_.begin(), free_spots_.end(), &el, 1, gen);

    // Mark button with icon
    std::pair<qint8, qint8> idxs = spotToIdxs(el);

    qint8 row = idxs.first;
    qint8 col = idxs.second;

    board_[row][col] = computer_icon_char_ ;
    QPushButton *button = board_buttons_[row][col];

    QIcon icon;
    computer_icon_char_ == 'O' ? icon = o_icon_ : icon = x_icon_;
    markButtonWithIcon(button, icon);
    free_spots_.erase(idxsToSpot(row,col));
}


bool TicTacToe::canWinLine(const char a, const char b, const char c, const char player, QVector<qint8> &win_poses){
    win_poses.clear();

    if(a == player && b == ' ' && c == ' '){
        win_poses.append(1);
        win_poses.append(2);
        return true;
    }

    if(a == ' ' && b == player && c == ' '){
        win_poses.append(0);
        win_poses.append(2);
        return true;
    }

    if(a == ' ' && b == ' ' && c == player ){
        win_poses.append(0);
        win_poses.append(1);
        return true;
    }

    if(a == player && b == player && c == ' '){
        win_poses.append(2);
        return true;
    }

    if(a == player && b == ' ' && c == player){
        win_poses.append(1);
        return true;
    }

    if(a == ' ' && b == player && c == player){
        win_poses.append(0);
        return true;
    }

    if(a == b == c && a == ' '){
        win_poses = {0,1,2};
        return true;
    }

    return false;
}

bool TicTacToe::canWinSingleMove(const char a, const char b, const char c, const char player, qint8 &pose){
    QVector<qint8> poses;
    bool can_win_line = canWinLine(a, b, c, player, poses);
    qint8 free_spot = poses.size();

    if(can_win_line && free_spot==1){
        pose = poses.at(0);
        return true;
    }else{
        pose = -1;
        return false;
    }
};

void TicTacToe::markComputerButton(qint8 row, qint8 col){
    QPushButton *button = board_buttons_[row][col];
    board_[row][col] = computer_icon_char_;
    markButtonWithIcon(button, computer_icon_);
    free_spots_.erase(idxsToSpot(row,col));
}

void TicTacToe::mediumBotAction(){
    // If multiple choses, choose random
    qint8 pose;
    QVector<qint8> poses;
    QVector<char> players{computer_icon_char_, player_icon_char_};

    // Single spot search
    for(auto &player: players){
        for (qint8 i = 0; i<3; i++){
            // ROW
            bool can_win_single_move_row = canWinSingleMove(board_[i][0], board_[i][1], board_[i][2], player,  pose);
            if(can_win_single_move_row){
                markComputerButton(i, pose);
                return;
            }

            // COLUMN
            bool can_win_single_move_col = canWinSingleMove(board_[0][i], board_[1][i], board_[2][i], player,  pose);
            if(can_win_single_move_col){
                markComputerButton(pose, i);
                return;
            }
        }
        // MAIN DIAG
        bool can_win_main_dial = canWinSingleMove(board_[0][0], board_[1][1], board_[2][2], player, pose);
        if(can_win_main_dial){
            markComputerButton(pose, pose);
            return;
        }

        // SECOND DIAL
        bool can_win_sec_dial = canWinSingleMove(board_[0][2], board_[1][1], board_[2][0], player, pose);
        if(can_win_sec_dial){
            markComputerButton(pose, -pose+2);
            return;
        }
    }


    // Possible winning line
    for(auto &player : players){
        for(qint8 i = 0; i<3; i++){
            // ROW
            bool can_win_on_row = canWinLine(board_[i][0], board_[i][1], board_[i][2], player, poses);
            if(can_win_on_row){
                std::mt19937 gen( std::random_device{}() );
                std::sample(poses.begin(), poses.end(), &pose, 1, gen);
                markComputerButton(i, pose);
                return;
            }

            // COLUMN
            bool can_win_on_col = canWinLine(board_[0][i], board_[1][i], board_[2][i], player, poses);
            if(can_win_on_col){
                std::mt19937 gen( std::random_device{}() );
                std::sample(poses.begin(), poses.end(), &pose, 1, gen);
                markComputerButton(pose, i);
                return;
            }

            // MAIN DIAG
            bool can_win_main_dial = canWinLine(board_[0][0], board_[1][1], board_[2][2], player, poses);
            if(can_win_main_dial){
                std::mt19937 gen( std::random_device{}() );
                std::sample(poses.begin(), poses.end(), &pose, 1, gen);
                markComputerButton(pose, pose);
                return;
            }

            // SECOND DIAL
            bool can_win_sec_dial = canWinLine(board_[0][2], board_[1][1], board_[2][0], player, poses);
            if(can_win_sec_dial){
                std::mt19937 gen( std::random_device{}() );
                std::sample(poses.begin(), poses.end(), &pose, 1, gen);
                markComputerButton(pose, -pose+2);
                return;
            }
        }
    }

    std::cout << "BEFORE RANDOMBOTACTION" << std::endl;
    // Random spot selection as last chance
    randomBotAction();
}

void TicTacToe::botActionBasedOnLevel(){

    switch (game_level_) {
    case gameLevel::easy:{
        easy_randomness_ ? randomBotAction() : mediumBotAction();
        easy_randomness_ = !easy_randomness_;
        break;
    }
    case gameLevel::medium:
        mediumBotAction();
        break;
    case gameLevel::hard:
        break;

    default:
        mediumBotAction();
        break;
    }
}


void TicTacToe::handlePlayerAction(const gameState &action_result, const char player){
    qint8 *score;
    player == player_icon_char_ ? score = &score_player_ : score = &score_computer_;

    switch (action_result) {
    case gameState::won:{
        // std::cout << "WON" << std::endl;
        // Updating score
        *score+=1;

        // Displaying text below the game
        if(player == player_icon_char_){
            comment_frame_->setText("YOU WON :)");
        }else{
            comment_frame_->setText("YOU LOST :(");
        }
        comment_frame_->setAlignment(Qt::AlignCenter);

        reset_board_button_->setDefault(true);
        reset_board_button_->setText("New Game");
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
        // std::cout << "LOST" << std::endl;
        break;
    }
    case gameState::draw:{
        // std::cout << "DRAW" << std::endl;
        comment_frame_->setText("DRAW GAME");
        comment_frame_->setAlignment(Qt::AlignCenter);

        reset_board_button_->setDefault(true);
        reset_board_button_->setText("New Game");
        break;
    }
    case gameState::playing:{
        // std::cout << "PLAYING" << std::endl;
        break;
    }
    case gameState::error:{
        // std::cout << "ERROR" << std::endl;
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
        board_[row][col] == player_icon_char_ ? icon = player_winning_icon_ : icon = computer_winning_icon_;
        markButtonWithIcon(winning_button, icon);
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

void TicTacToe::updatePlayerIconLabel(){
    // Converting Icon to pixmap
    QPixmap pixmap = player_icon_.pixmap(player_icon_label_->size());

    player_icon_label_->setPixmap(pixmap);
    player_icon_label_->setScaledContents(true);
}

void TicTacToe::performBotAction(){
    gameState robot_status;

    botActionBasedOnLevel();
    robot_status = checkGameStateForPlayer(computer_icon_char_);
    handlePlayerAction(robot_status, computer_icon_char_);
}
