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

    // Front and back part of name buttons
    front_button_names_ = {{"left"}, {"center"}, {"right"}};
    back_button_names_ = {{"top"}, {"center"}, {"bottom"}};

    // Setting default player Icon as circle and starter player
    player_icon_char_ = 'O';
    starter_player_ = computer_icon_char_;

    // Buttons and ui objects
    go_back_button_ = ui->goBackButton;
    reset_board_button_ = ui->resetBoardButton;
    reset_score_button_ = ui->resetScoreButton;
    change_icon_button_ = ui->changeIconButton;

    player_icon_label_  = ui->playerIconLabel;

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

/****** MAIN LOGIC ******/
/**
 * @brief Handle the click event for any click on the board game.
 *
 * This function identifies the clicked button, updates the game board with the player's move,
 * marks the button with the player's icon, and checks the game state. It implements the main logic.
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


/****** INITIALIZATIONS FUNCTIONS ******/
/**
 * @brief Initializes the Tic Tac Toe board, resetting all game elements.
 *
 * This function resets the game board and initializes various UI elements
 * and game variables for a new game session. It clears the board, updates
 * player icons, resets button states, and determines which player starts
 * based on previous settings.
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

/**
 * @brief Initializes icons for the Tic Tac Toe game.
 *
 * This function sets up icons for X and O players in various colors
 * (black, red, green) based on the selected player icon and status.
 */
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

/**
 * @brief Initializes the "Change Icon" dialog for choosing player icons.
 *
 * This function sets up a dialog window that allows players to select
 * their preferred icon (either a circle or a cross) for the game. It
 * creates and styles buttons for both icon options, sets their icons,
 * and connects the buttons to their respective signal handlers.
 */
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

/**
 * @brief Initializes the level combo box with difficulty options.
 *
 * This function populates the combo box with difficulty levels ("Easy", "Medium", "Hard"),
 * and set "Medium" as the default level. It also updates the
 * game level based on the selected index.
 */
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

/****** CALLBACK FUNCTIONS ******/
/**
 * @brief Handles the "Go Back" button click event.
 *
 * This function emits a signal to navigate back to the main menu when the "Go Back" button is clicked.
 */
void TicTacToe::onGoBackButtonClicked(){
    emit goBackToMainMenu();
}

/**
 * @brief Handles the "Reset Board" button click event.
 *
 * This function resets the game board to its initial state and updates the
 * reset button's default status.
 */
void TicTacToe::handleResetBoardClicked(){
    initializeBoard();
    reset_board_button_->setDefault(false);
}

/**
 * @brief Handles the "Reset Score" button click event.
 *
 * This function resets the scores displayed on the LCD screens to their initial values.
 */
void TicTacToe::handleResetScoreClicked(){
    resetLcdScores();
}

/**
 * @brief Handles the "Change Icon" button click event.
 *
 * This function displays the "Change Icon" dialog box, allowing the player to choose a new icon.
 */
void TicTacToe::handleChangeIconClicked(){
    // Run dialog box
    change_icon_dialog_.exec();
}

/**
 * @brief Handles the level selection change event.
 *
 * This function updates the game level based on the current selection in the combo box
 * and reinitializes the game board to reflect the new difficulty setting.
 */
void  TicTacToe::handleChangeLevelBox(){
    game_level_ = static_cast<gameLevel>(level_combo_box_->currentIndex());
    initializeBoard();
}

/**
 * @brief Performs the bot's action during the game.
 *
 * This function executes the bot's move based on the current difficulty level,
 * checks the game state after the bot's move, and handles the outcome accordingly.
 */
void TicTacToe::performBotAction(){
    gameState robot_status;

    botActionBasedOnLevel();
    robot_status = checkGameStateForPlayer(computer_icon_char_);
    handlePlayerAction(robot_status, computer_icon_char_);
}

/**
 * @brief Changes the player's icon to a circle ('O') and reinitializes the board.
 *
 * This function sets the player's icon to a circle, updates the corresponding icons,
 * reinitializes the game board, closes the change icon dialog, and updates the player icon label.
 */
void TicTacToe::changeIconCircle(){
    player_icon_char_ = 'O';
    player_icon_ = o_icon_;
    computer_icon_ = x_icon_;
    initializeBoard();
    change_icon_dialog_.close();
    updatePlayerIconLabel();
}

/**
 * @brief Changes the player's icon to a cross ('X') and reinitializes the board.
 *
 * This function sets the player's icon to a cross, updates the corresponding icons,
 * reinitializes the game board, closes the change icon dialog, and updates the player icon label.
 */
void TicTacToe::changeIconCross(){
    player_icon_char_ = 'X';
    player_icon_ = x_icon_;
    computer_icon_ = o_icon_;
    initializeBoard();
    change_icon_dialog_.close();
    updatePlayerIconLabel();
}

/**
 * @brief Marks a button on the game board with a specified icon.
 *
 * This function sets the icon of the given QPushButton to the specified QIcon,
 * adjusts the icon size to fit the button, and disables the button to prevent
 * further interaction.
 */
void TicTacToe::markButtonWithIcon(QPushButton *button, QIcon icon){
    button->setIcon(icon);
    button->setIconSize(button->size());
    disableButton(button);
}

/**
 * @brief Marks a button on the game board with the computer's icon.
 *
 * This function updates the game board with the computer's move at the specified
 * row and column, marks the corresponding button with the computer's icon, and
 * removes the position from the available free spots on the board.
 *
 * @param row The row index of the button on the game board.
 * @param col The column index of the button on the game board.
 */
void TicTacToe::markComputerButton(qint8 row, qint8 col){
    QPushButton *button = board_buttons_[row][col];
    board_[row][col] = computer_icon_char_;
    markButtonWithIcon(button, computer_icon_);
    free_spots_.erase(idxsToSpot(row,col));
}

/**
 * @brief Clears the icon from a QPushButton.
 *
 * This function removes the icon set on the specified QPushButton,
 * effectively clearing any visual representation associated with it.
 *
 * @param button Pointer to the QPushButton from which the icon will be cleared.
 */
void TicTacToe::clearIconFromButton(QPushButton *button){
    button->setIcon(QIcon());
}

/**
 * @brief Marks the winning buttons on the game board with a winning icon.
 *
 * This function updates the specified buttons on the Tic Tac Toe game board
 * with a winning icon based on the provided vector of button indices.
 *
 * @param idxs_vect Vector containing pairs of row and column indices of the winning buttons.
 */
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

/**
 * @brief Updates the scores displayed on the LCD screens.
 *
 * This function updates the values displayed on the LCD screen for the computer's
 * and player's scores based on the current game state.
 */
void TicTacToe::updateLcdScores(){
    lcd_computer_->display(score_computer_);
    lcd_player_->display(score_player_);
}

/**
 * @brief Resets the scores displayed on the LCD screens to zero.
 *
 * This function sets both the computer's and player's scores to zero and updates
 * the corresponding LCD screens to display these reset scores.
 */
void TicTacToe::resetLcdScores(){
    qint8 zero = 0;
    score_computer_ = zero;
    score_player_ = zero;
    lcd_computer_->display(zero);
    lcd_player_->display(zero);
}

/**
 * @brief Checks if a line of three positions can potentially lead to a win for a player.
 *
 * This function examines three positions on a Tic Tac Toe board and determines if placing
 * the player's icon in one of these positions would result in a winning line. It also
 * populates a vector with the positions that would constitute a winning line if the condition is met.
 *
 * @param a The state of the first position ('X', 'O', or ' ').
 * @param b The state of the second position ('X', 'O', or ' ').
 * @param c The state of the third position ('X', 'O', or ' ').
 * @param player The player's icon ('X' or 'O').
 * @param win_poses A vector to store the positions that form a winning line if true.
 * @return true if placing the player's icon in one of the positions would create a winning line, false otherwise.
 */
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

/**
 * @brief Determines if placing the player's icon in one position can lead to an immediate win.
 *
 * This function evaluates three positions on a Tic Tac Toe board and checks if placing the player's
 * icon in one of these positions would result in an immediate win. It also returns the position where
 * placing the icon would lead to a win.
 *
 * @param a The state of the first position ('X', 'O', or ' ').
 * @param b The state of the second position ('X', 'O', or ' ').
 * @param c The state of the third position ('X', 'O', or ' ').
 * @param player The player's icon ('X' or 'O').
 * @param pose A reference to store the position that would result in a win if true.
 * @return true if placing the player's icon in one of the positions would create an immediate win, false otherwise.
 */
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

/**
 * @brief Executes a random move for the bot on the game board.
 *
 * This function selects a random position from the available free spots on the game board,
 * marks the corresponding button with the bot's icon, and updates the game state accordingly.
 * If no free spots are available, the function returns without taking any action.
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

/**
 * @brief Executes a medium-level strategy for the bot's move on the game board.
 *
 * This function implements a medium-level strategy for the bot's move:
 * - It first checks if there's an opportunity to win in a single move.
 * - If no immediate winning move is found, it checks for defensive moves to prevent the player from winning.
 * - If neither winning nor defensive moves are available, it resorts to selecting a random move.
 */
void TicTacToe::mediumBotAction(){
    // If multiple choses, choose random
    qint8 pose;
    QVector<qint8> poses;
    QVector<char> players{computer_icon_char_, player_icon_char_};

    // Single spot search
    for(auto &player: players){
        for (qint8 i = 0; i<3; i++){
            // ROW
            bool can_win_single_move_row = canWinSingleMove(board_[i][0], board_[i][1], board_[i][2], player, pose);
            if(can_win_single_move_row){
                markComputerButton(i, pose);
                return;
            }

            // COLUMN
            bool can_win_single_move_col = canWinSingleMove(board_[0][i], board_[1][i], board_[2][i], player, pose);
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
        }

        // MAIN DIAG
        bool can_win_main_dial = canWinLine(board_[0][0], board_[1][1], board_[2][2], player, poses);
        if(can_win_main_dial){
            std::mt19937 gen( std::random_device{}() );
            std::sample(poses.begin(), poses.end(), &pose, 1, gen);
            markComputerButton(pose, pose);
            return;
        }

        // SECOND DIAG
        bool can_win_sec_dial = canWinLine(board_[0][2], board_[1][1], board_[2][0], player, poses);
        if(can_win_sec_dial){
            std::mt19937 gen( std::random_device{}() );
            std::sample(poses.begin(), poses.end(), &pose, 1, gen);
            markComputerButton(pose, 2-pose);
            return;
        }
    }

    // Random spot selection as last chance
    randomBotAction();
}

/**
 * @brief Executes the bot's action based on the current game level.
 *
 * This function determines the bot's action based on the selected game level:
 * - For Easy level, it alternates between random moves and medium-level strategy.
 * - For Medium level, it uses a medium-level strategy to determine the bot's move.
 * - For Hard level, no specific action is implemented in this function.
 *
 * If an unrecognized game level is provided, it defaults to the medium-level strategy.
 */
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

/**
 * @brief Handles the outcome of a player's action during the game.
 *
 * This function processes the result of a player's action based on the game state:
 * - If the player wins, it updates the score, displays a corresponding message, and prepares for a new game.
 * - If the game ends in a draw, it displays a draw message and prepares for a new game.
 * - If the game is still ongoing, it continues without any additional action.
 * - If an error occurs, it handles it appropriately.
 *
 * @param action_result The result of the player's action (won, draw, playing, or error).
 * @param player The player's icon ('X' or 'O').
 */
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
    case gameState::draw:{
        // std::cout << "DRAW" << std::endl;
        comment_frame_->setText("DRAW GAME");
        comment_frame_->setAlignment(Qt::AlignCenter);

        reset_board_button_->setDefault(true);
        reset_board_button_->setText("New Game");
        grayOutBoardButtons();
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
 * @brief Checks the game state for a specified player ('X' or 'O').
 *
 * This function examines the current state of the Tic Tac Toe board to determine
 * whether the specified player has won, if the game is a draw, or if it is still
 * ongoing (playing).
 *
 * @param player The player's icon ('X' or 'O').
 * @return The game state for the specified player (won, draw, playing, or error if player is invalid).
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
 * @brief Disables a QPushButton, preventing user interaction.
 *
 * This function disables the specified QPushButton, making it inactive and
 * preventing any further user interaction with it.
 *
 * @param button Pointer to the QPushButton to be disabled.
 */
void TicTacToe::disableButton(QPushButton *button){
    button->setEnabled(false);
}

/**
 * @brief Enables a QPushButton, allowing user interaction.
 *
 * This function enables the specified QPushButton, making it active and
 * allowing user interaction with it.
 *
 * @param button Pointer to the QPushButton to be enabled.
 */
void TicTacToe::enableButton(QPushButton *button){
    button->setEnabled(true);
}

/**
 * @brief Converts row and column indices to a spot index on the Tic Tac Toe board.
 *
 * This function calculates and returns the spot index corresponding to the given
 * row and column indices on the Tic Tac Toe board.
 *
 * @param r Row index.
 * @param c Column index.
 * @return Spot index calculated from row and column indices.
 */
qint8 TicTacToe::idxsToSpot(const qint8 &r, const qint8 &c){
    return (3*r + c);
}

/**
 * @brief Converts a spot index on the Tic Tac Toe board to row and column indices.
 *
 * This function calculates and returns the row and column indices corresponding to
 * the given spot index on the Tic Tac Toe board.
 *
 * @param s Spot index, must be between 0 and 8.
 * @return Pair of row and column indices.
 * @throws std::out_of_range if the spot index is out of the valid range (0-8).
 */
std::pair<qint8, qint8> TicTacToe::spotToIdxs(const qint8 &s){
    if (s < 0 || s > 8) {
        throw std::out_of_range("Number must be between 0 and 8");
    }
    qint8 row = s / 3;
    qint8 col = s % 3;
    return std::make_pair(row, col);
}

/**
 * @brief Updates the player icon label with the current player icon.
 *
 * This function converts the player's current icon to a pixmap and updates
 * the player icon label with the scaled pixmap.
 */
void TicTacToe::updatePlayerIconLabel(){
    // Converting Icon to pixmap
    QPixmap pixmap = player_icon_.pixmap(player_icon_label_->size());

    player_icon_label_->setPixmap(pixmap);
    player_icon_label_->setScaledContents(true);
}

/**
 * @brief Converts all board buttons to grayscale.
 *
 * This function iterates through all buttons on the Tic Tac Toe board,
 * converts each button's icon to grayscale, and sets the gray icon back
 * to the button.
 */
void TicTacToe::grayOutBoardButtons(){
    for (qint8 i = 0; i < 3; ++i) {
        for (qint8 j = 0; j < 3; ++j) {
            QPushButton *button = board_buttons_[i][j];
            QIcon icon = button->icon();
            QSize iconSize = button->iconSize();

            // Retrieve the current icon pixmap
            QPixmap originalPixmap = icon.pixmap(iconSize);

            // Convert the pixmap to grayscale using a separate function
            QPixmap grayPixmap = convertPixmapToGray(originalPixmap);

            // Create a new QIcon with the grayscale pixmap
            QIcon grayIcon(grayPixmap);

            // Set the gray icon back to the button
            button->setIcon(grayIcon);
        }
    }
}

/**
 * @brief Converts a given pixmap to grayscale.
 *
 * This function takes an original pixmap as input and converts it to grayscale,
 * returning the grayscale version as a new QPixmap.
 *
 * @param original_pixmap Original pixmap to be converted to grayscale.
 * @return Grayscale version of the original pixmap.
 */
QPixmap TicTacToe::convertPixmapToGray(const QPixmap &original_pixmap) {
    QSize iconSize = original_pixmap.size();

    // Create a new pixmap for the grayscale version
    QPixmap gray_pixmap(iconSize);
    // Ensure transparency is preserved
    gray_pixmap.fill(Qt::transparent);

    // Create a QPainter to paint the grayscale version
    QPainter painter(&gray_pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.drawPixmap(0, 0, original_pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(gray_pixmap.rect(), QColor(10, 10, 10));
    painter.end();

    return gray_pixmap;
}


/****** PUBLIC FUNCTIONS ******/
/**
 * @brief Returns the original size of the widget.
 *
 * This function retrieves and returns the original size of the widget used in the Tic Tac Toe game.
 *
 * @return Original size of the widget.
 */
QSize TicTacToe::getWidgetSize(){
    return original_widget_size_;
}
