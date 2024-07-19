#include "tictactoeboard.h"
/* ERROR TO FIX:
 * undefined reference mediumBotAction
 *
 */

TicTacToeBoard::TicTacToeBoard(QWidget *parent, const QString xIconPath, const QString oIconPath)
    : QWidget{parent}
    , ui(new Ui::Form)
    , x_icon_path_(xIconPath)
    , o_icon_path_(oIconPath)
    , easy_randomness_(true)
{
    ui->setupUi(this);

    // Front and back part of name buttons
    front_button_names_ = {{"left"}, {"center"}, {"right"}};
    back_button_names_ = {{"top"}, {"center"}, {"bottom"}};

    // Setting default player Icon as circle and starter player
    player_icon_char_ = 'O';
    starter_player_ = player_icon_char_;

    initializeBoard();

    connect(this, &TicTacToeBoard::botTurn, this, &TicTacToeBoard::performBotAction);
}


/**
 * @brief Performs the bot's action during the game.
 *
 * This function executes the bot's move based on the current difficulty level,
 * checks the game state after the bot's move, and handles the outcome accordingly.
 */
void TicTacToeBoard::performBotAction(){
    gameState robot_status;

    botActionBasedOnLevel();
    robot_status = checkGameStateForPlayer(computer_icon_char_);
    handlePlayerAction(robot_status, computer_icon_char_);
}

/**
 * @brief Executes a medium-level strategy for the bot's move on the game board.
 *
 * This function implements a medium-level strategy for the bot's move:
 * - It first checks if there's an opportunity to win in a single move.
 * - If no immediate winning move is found, it checks for defensive moves to prevent the player from winning.
 * - If neither winning nor defensive moves are available, it resorts to selecting a random move.
 */
void TicTacToeBoard::mediumBotAction(){
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
bool TicTacToeBoard::canWinLine(const char a, const char b, const char c, const char player, QVector<qint8> &win_poses){
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
 * @brief Marks a button on the game board with the computer's icon.
 *
 * This function updates the game board with the computer's move at the specified
 * row and column, marks the corresponding button with the computer's icon, and
 * removes the position from the available free spots on the board.
 *
 * @param row The row index of the button on the game board.
 * @param col The column index of the button on the game board.
 */
void TicTacToeBoard::markComputerButton(qint8 row, qint8 col){
    QPushButton *button = board_buttons_[row][col];
    board_[row][col] = computer_icon_char_;
    markButtonWithIcon(button, computer_icon_);
    free_spots_.remove(idxsToSpot(row,col));
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
bool TicTacToeBoard::canWinSingleMove(const char a, const char b, const char c, const char player, qint8 &pose){
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
 * @brief Executes the bot's action based on the current game level.
 *
 * This function determines the bot's action based on the selected game level:
 * - For Easy level, it alternates between random moves and medium-level strategy.
 * - For Medium level, it uses a medium-level strategy to determine the bot's move.
 * - For Hard level, no specific action is implemented in this function.
 *
 * If an unrecognized game level is provided, it defaults to the medium-level strategy.
 */
void TicTacToeBoard::botActionBasedOnLevel(){

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
 * @brief Executes a random move for the bot on the game board.
 *
 * This function selects a random position from the available free spots on the game board,
 * marks the corresponding button with the bot's icon, and updates the game state accordingly.
 * If no free spots are available, the function returns without taking any action.
 */
void TicTacToeBoard::randomBotAction(){
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
    free_spots_.remove(idxsToSpot(row,col));
}

void TicTacToeBoard::setGameLevel(gameLevel level)
{
    game_level_ = level;
}

void TicTacToeBoard::initializeBoard()
{
    player_icon_char_ == 'O' ? computer_icon_char_ = 'X' : computer_icon_char_ = 'O';

    initializeIcons();
    emit gameStarted();

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
            connect(button, &QPushButton::released, this, &TicTacToeBoard::handleBoardButtonClick);
            row.append(button);
        }
        board_buttons_.append(row);
    }

    // Managing different player start
    if(starter_player_ == computer_icon_char_){
        emit updateCommentLabel("BOT STARTED");
        emit botTurn();
        starter_player_ = player_icon_char_;
    }else{
        starter_player_ = computer_icon_char_;
        emit updateCommentLabel("YOU START");
    }
}

void TicTacToeBoard::handleResetBoardClicked(){
    initializeBoard();
}

void TicTacToeBoard::setCommentLabel(QLabel *label)
{
    comment_label_ = label;
}

void TicTacToeBoard::setCurrentIconLabel(QLabel *label)
{
    current_icon_label_ = label;
}

QIcon TicTacToeBoard::changeIconColor(const QIcon& icon, const QColor& color) {
    // Assume we want to change the color of the icon at a specific size
    QSize iconSize = icon.availableSizes().first(); // Adjust the size if necessary
    QPixmap originalPixmap = icon.pixmap(iconSize);

    QImage img = originalPixmap.toImage();
    for (int y = 0; y < img.height(); ++y) {
        for (int x = 0; x < img.width(); ++x) {
            QColor pixelColor = img.pixelColor(x, y);
            if (pixelColor.alpha() > 0) {  // Only change non-transparent pixels
                // Change the color of the pixel to the specified color
                img.setPixelColor(x, y, QColor(color.red(), color.green(), color.blue(), pixelColor.alpha()));
            }
        }
    }
    QPixmap coloredPixmap = QPixmap::fromImage(img);
    QIcon coloredIcon;
    coloredIcon.addPixmap(coloredPixmap, QIcon::Normal);
    coloredIcon.addPixmap(coloredPixmap, QIcon::Disabled);
    return coloredIcon;
}

/**
 * @brief Initializes icons for the Tic Tac Toe game.
 *
 * This function sets up icons for X and O players in various colors
 * (black, red, green) based on the selected player icon and status.
 */
void TicTacToeBoard::initializeIcons(){
    x_icon_ = QIcon(x_icon_path_);
    x_icon_.addPixmap(QPixmap(x_icon_path_), QIcon::Disabled);

    o_icon_ = QIcon(o_icon_path_);
    o_icon_.addPixmap(QPixmap(o_icon_path_), QIcon::Disabled);

    if(player_icon_char_ == 'O'){
        player_icon_ = o_icon_;
        computer_icon_ = x_icon_;
    }else{
        player_icon_ = x_icon_;
        computer_icon_ = o_icon_;
    }

    player_winning_icon_ = changeIconColor(player_icon_, Qt::green);
    computer_winning_icon_ = changeIconColor(computer_icon_, Qt::red);
}

/**
 * @brief Handle the click event for any click on the board game.
 *
 * This function identifies the clicked button, updates the game board with the player's move,
 * marks the button with the player's icon, and checks the game state. It implements the main logic.
 */
void TicTacToeBoard::handleBoardButtonClick(){
    if(free_spots_.size()==9)
        emit updateCommentLabel("");

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
        QColor color = Qt::green;
        color.setAlpha(255);
        markButtonWithIcon(button, player_icon_);

        free_spots_.remove(idxsToSpot(row,col));


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
void TicTacToeBoard::handlePlayerAction(const gameState &action_result, const char player){
    switch (action_result) {
    case gameState::won:{
        // qDebug() << "WON";

        // Displaying text below the game
        if(player == player_icon_char_){
            emit updateCommentLabel("YOU WON :)");
        }else{
            emit updateCommentLabel("YOU LOST :(");
        }

        // Disable all the available left spots
        for(const auto& el: free_spots_){
            std::pair<qint8, qint8> idxs = spotToIdxs(el);
            qint8 row = idxs.first;
            qint8 col = idxs.second;
            QPushButton *tmp_button = board_buttons_[row][col];
            disableButton(tmp_button);
        }

        emit gameIsOver(action_result, player);

        break;
    }
    case gameState::draw:{
        // qDebug() << "DRAW" ;
        emit updateCommentLabel("DRAW GAME");
        grayOutBoardButtons();
        emit gameIsOver(action_result, player);
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
 * @brief Converts all board buttons to grayscale.
 *
 * This function iterates through all buttons on the Tic Tac Toe board,
 * converts each button's icon to grayscale, and sets the gray icon back
 * to the button.
 */
void TicTacToeBoard::grayOutBoardButtons(){
    QIcon xGrayIcon, oGrayIcon;
    xGrayIcon = changeIconColor(x_icon_, Qt::gray);
    oGrayIcon = changeIconColor(o_icon_, Qt::gray);

    for (qint8 i = 0; i < 3; ++i) {
        for (qint8 j = 0; j < 3; ++j) {
            QPushButton *button = board_buttons_[i][j];
            board_[i][j] == 'X' ? button->setIcon(xGrayIcon) : button->setIcon(oGrayIcon);
        }
    }
}

void TicTacToeBoard::changeIconToCircle()
{
    player_icon_char_ = 'O';
    player_icon_ = o_icon_;
    computer_icon_ = x_icon_;
    initializeBoard();
}

void TicTacToeBoard::changeIconToCross()
{
    player_icon_char_ = 'X';
    player_icon_ = x_icon_;
    computer_icon_ = o_icon_;
    initializeBoard();
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
std::pair<qint8, qint8> TicTacToeBoard::spotToIdxs(const qint8 &s){
    if (s < 0 || s > 8) {
        throw std::out_of_range("Number must be between 0 and 8");
    }
    qint8 row = s / 3;
    qint8 col = s % 3;
    return std::make_pair(row, col);
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
gameState TicTacToeBoard::checkGameStateForPlayer(char player){
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
 * @brief Marks the winning buttons on the game board with a winning icon.
 *
 * This function updates the specified buttons on the Tic Tac Toe game board
 * with a winning icon based on the provided vector of button indices.
 *
 * @param idxs_vect Vector containing pairs of row and column indices of the winning buttons.
 */
void TicTacToeBoard::markWinningButtons(const QVector<std::pair<qint8, qint8>> &idxs_vect){
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
 * @brief Converts row and column indices to a spot index on the Tic Tac Toe board.
 *
 * This function calculates and returns the spot index corresponding to the given
 * row and column indices on the Tic Tac Toe board.
 *
 * @param r Row index.
 * @param c Column index.
 * @return Spot index calculated from row and column indices.
 */
qint8 TicTacToeBoard::idxsToSpot(const qint8 &r, const qint8 &c){
    return (3*r + c);
}

/**
 * @brief Disables a QPushButton, preventing user interaction.
 *
 * This function disables the specified QPushButton, making it inactive and
 * preventing any further user interaction with it.
 *
 * @param button Pointer to the QPushButton to be disabled.
 */
void TicTacToeBoard::disableButton(QPushButton *button){
    button->setEnabled(false);
}

/**
 * @brief Marks a button on the game board with a specified icon.
 *
 * This function sets the icon of the given QPushButton to the specified QIcon,
 * adjusts the icon size to fit the button, and disables the button to prevent
 * further interaction.
 */
void TicTacToeBoard::markButtonWithIcon(QPushButton *button, QIcon icon){
    button->setIcon(icon);
    button->setIconSize(button->size());
    disableButton(button);
}

/**
 * @brief Enables a QPushButton, allowing user interaction.
 *
 * This function enables the specified QPushButton, making it active and
 * allowing user interaction with it.
 *
 * @param button Pointer to the QPushButton to be enabled.
 */
void TicTacToeBoard::enableButton(QPushButton *button){
    button->setEnabled(true);
}

/**
 * @brief Clears the icon from a QPushButton.
 *
 * This function removes the icon set on the specified QPushButton,
 * effectively clearing any visual representation associated with it.
 *
 * @param button Pointer to the QPushButton from which the icon will be cleared.
 */
void TicTacToeBoard::clearIconFromButton(QPushButton *button){
    button->setIcon(QIcon());
}

QIcon TicTacToeBoard::getPlayerIcon(){
    return player_icon_;
}

QIcon TicTacToeBoard::getComputerIcon(){
    return computer_icon_;
}
