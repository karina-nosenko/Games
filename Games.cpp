#include "Games.h"

bool isNum(string num) {
    for(int i=0; i<num.length(); i++) {
        if (!isdigit(num[i]))
            return false;
    }
    return true;
}

/*============= Connect between Game UI and the Game logic ================*/

GameController:: GameController (char const* mode) {
    string console = "console";

    // determine the game ui mode
    if(mode == console)   _ui = new Console;
    else                  _ui = new GUI;
}

GameController:: ~GameController () {
    delete _ui;
    if(_game)   delete _game;
}

void GameController:: play() {
    int option;
    int gameOption;
    int difficulty;
    bool mainMenu = true;
    vector<vector<char>> board; 

    while(mainMenu) {
        _ui->start();
        gameOption = _ui->chooseGame();
        difficulty = _ui->chooseDifficulty();

        //assign the game
        switch(gameOption) {
            case 1:
                board.resize ( BOARD_SIZE , vector<char> (BOARD_SIZE,0) ); //define board 3x3 filled with zero
                
                switch(difficulty) {         
                    case 1:
                        _game = new TicTacToeRand(board);
                        break;
                    case 2:
                        _game = new TicTacToeEdu(board);
                        break;
                    default:
                        exit(1);
                }
                break;
            default:
                exit(1);
        }
        _game->setUI(*_ui);
        mainMenu = false;

        while(mainMenu == false) {
            _game->start();

            //play the game
            while(_game->getGameState() == PLAY) {
                _game->getTurn() ? _game->getPlayerMove() : _game->getComputerMove();
                _game->updateGameState();
            }

            //print the result of the game
            _ui->printBoard(_game->getBoard());
            switch(_game->getGameState()) {
                case 0:
                    _ui->print("O Won!");
                    break;
                case 1:
                    _ui->print("X Won!");
                    break;
                case 2:
                    _ui->print("Draw!");
                    break;
                default: 
                    exit(1);
            }
            cout<<endl;

            _game->clear(); 

            //ask what to do next
            option = _ui->end();
            switch(option) {
                case 1:     //Play again
                    break;
                case 2:     //Return to the main Menu
                    mainMenu = true;
                    break;
                case 3:     //Exit
                    exit(1);
                default:
                    exit(1);
            }
        }
    }
}

/*============================ UI part =================================*/

void Console:: start() {
    cout<< "\tWelcome!"<<endl;
    cout<< "======================\n"<<endl;
}


//Choose the game
int Console:: chooseGame() {
    string gameOption;
    int result=0;

    while(true){
        cout<< "Choose a game number:"<<endl;
        cout<< "1 - Tic Tac Toe"<<endl;
        cout<< "> ";
        cin>>gameOption;

        if( isNum(gameOption) ){
            result = stoi(gameOption);
            if( result==1 )
                break;
            else cout<< "Wrong Input"<<endl;
        }
        else cout<< "Wrong Input"<<endl;
    }

    return result;
}

//Ask the user what to do in the end of the game
int Console:: end() {
    string option;
    int result=0;

    while(true){
        cout<< "1 - Play again"<<endl;
        cout<< "2 - Return to the Main Menu"<<endl;
        cout<< "3 - Exit"<<endl;
        cout<< "> ";
        cin>>option;

        if( isNum(option) ){
            result = stoi(option);
            if( result>=1 && result<=3 )
                break;
            else cout<< "Wrong Input"<<endl;
        }
        else cout<< "Wrong Input"<<endl;
    }

    return result;
}


//Print the game board
void Console::printBoard(vector<vector<char>> board) const {
    cout<<endl;
    for(int i=0; i<board.size(); i++) {
        for( int j=0; j<board[0].size(); j++) {
            if (board[i][j] == 0)   cout<<"-   ";
            else    cout<<board[i][j]<<"   ";
        }
        cout<<endl;
    }
    cout<<endl;
}

//Get player's sign ( 0 = O, 1 = X )
int Console::ticTacToeSign() {
    string sign;
    int result;
    
    while(true){
        cout<< "Choose your sign:"<<endl;
        cout<< "1 - X"<<endl;
        cout<< "2 - O"<<endl;
        cout<< "> ";
        cin>>sign;

        if( isNum(sign) ){
            result = stoi(sign);
            if( result==1 ){
                return 1;
                break;
            }
            else if( result==2 ){
                return 0;
                break;
            }
            else cout<< "Wrong Input"<<endl;
        }
        else cout<< "Wrong Input"<<endl;
    }
}


//get the move from the user
Move Console::getMove(int maxRow, int maxColumn) {
    Move move;
    
    while(true) {
        string r,c;
        cout<<"> ";
        cin>>r;
        cin>>c;

        if( isNum(r) && isNum(r)){
            move.row = stoi(r);
            move.column = stoi(c);
            if( move.row>=0 && move.row<=maxRow && move.column>=0 && move.column<=maxColumn )
                return move;
            else cout<< "Invalid Move"<<endl;
        }
        else cout<< "Invalid Move"<<endl;
    } 
}


//Choose the game difficulty
int Console:: chooseDifficulty() {
    string difficulty;
    int result;

    while(true){
        cout<< "Choose a difficulty:"<<endl;
        cout<< "1 - Easy"<<endl;
        cout<< "2 - Medium"<<endl;
        cout<< "> ";
        cin>>difficulty;

        if( isNum(difficulty) ){
            result = stoi(difficulty);
            if( result>=1 && result<=2 )
                break;
            else cout<< "Wrong Input"<<endl;
        }
        else cout<< "Wrong Input"<<endl;
    }

    return result;
}

/*======================== Game logic part ============================*/

BoardGame::BoardGame()
    :Game(), _state(PLAY) 
    {}


BoardGame::BoardGame(vector<vector<char>> board)
    :Game(), _state(PLAY), _board(board)
    {}

//zero the board values and the game state
void BoardGame::clear() {
    for(int i=0; i<_board.size(); i++) {
        for(int j=0; j<_board[i].size(); j++) {
            _board[i][j] = 0;
        }
    }

    _state = PLAY;
}


bool TicTacToeGame::isFree(Move move) const {
    return _board[move.row][move.column] == 0;
}

//return a vector of available (free) moves
vector <Move> TicTacToeGame::findAllFree() const {
    Move move;
    vector <Move> freeMoves;
    for(int i=0; i<BOARD_SIZE; i++) {
        for(int j=0; j<BOARD_SIZE; j++) {
            move.column = i;
            move.row = j;
            if (isFree(move)) {
                freeMoves.push_back(move);
            }
        }
    }
    return freeMoves;
}

bool BoardGame::isFullBoard() const {
    for(int i=0; i<_board.size(); i++) {
        for(int j=0; j<_board[i].size(); j++) {
            if(_board[i][j]==0)
                return false;
        }
    }
    return true;
}

vector<Move> TicTacToeGame::findPlayerMoves() const{
    vector<Move> move;
    for(int i=0; i<_board.size(); i++) {
        for(int j=0; j<_board[i].size(); j++) {
            if(_board[i][j] == _playerSign) {
                Move m = {i,j};
                move.push_back(m);
            }
        }
    }
    return move;
}

vector<Move> TicTacToeGame::findComputerMoves() const{
    vector<Move> move;
    for(int i=0; i<_board.size(); i++) {
        for(int j=0; j<_board[i].size(); j++) {
            if(_board[i][j] == _computerSign) {
                Move m = {i,j};
                move.push_back(m);
            }
        }
    }
    return move;
}

void TicTacToeGame::start() {
    _ui->print("_____ Tic Tac Toe_____ ");

    _turn = _ui->ticTacToeSign();
    if(_turn) {
        _playerSign = 'X';
        _computerSign = 'O';
    }
    else {
        _playerSign = 'O';
        _computerSign = 'X';
    }

    _ui->print("Format: x y [x-row index, y-column index]");    
    _ui->printBoard(_board);
}

void TicTacToeGame::updateGameState() {
    bool rowFlag = true;
    bool columnFlag = true;
    bool diagonalFlag = true;
    int i,c,r;

    //check if there is a row with the same non-zero values
    for(r=0; r<_board.size(); r++) {
        for(c=1; c<_board[r].size(); c++) {
            if((_board[r][0] != _board[r][c]) || (_board[r][0]==0) || (_board[r][c] == 0)) {
                rowFlag = false;
                break;
            }
        }
        if (rowFlag == true) {
            if (_board[r][0] == 'O') {  
                _state = O;
            }
            else    _state = X;
            return;
        }
        rowFlag = true;
    }

    //check if there is a column with the same non-zero values
    for(c=0; c<_board.size(); c++) {
        for(r=1; r<_board[c].size(); r++) {
            if((_board[0][c] != _board[r][c]) || (_board[0][c]==0) || (_board[r][c] == 0)) {
                columnFlag = false;
                break;
            }
        }
        if (columnFlag == true) {
            if (_board[0][c] == 'O') {  
                _state = O;
            }
            else    _state = X;
            return;
        }
        columnFlag = true;
    }

    //check if there is a diagonal with the same non-zero values
    for(i=1; i<_board.size(); i++) {
        if ((_board[0][0] != _board[i][i]) || (_board[0][0]==0) || (_board[i][i] == 0) ) {
            diagonalFlag = false;
            break;
        }
    }
    if(diagonalFlag == true) {
        if (_board[0][0] == 'O') {
            _state = O;
        }
        else    _state = X;
        return;
    }
    if(_board[0][2]!=0 && _board[1][1]!=0 && _board[2][0]!=0) {
        if(_board[0][2]== _board[1][1] && _board[0][2]== _board[2][0]) {
            if (_board[0][2] == 'O') {
                _state = O;
            }
            else    _state = X;
            return;
        }
    }

    if(isFullBoard()) {
        _state = DRAW;
        return;
    }

    _state = PLAY;
}

void TicTacToeGame::getPlayerMove() {
    Move move;

    while(true) {
        move = _ui->getMove(2, 2);
        if(isFree(move)) {
             _board[move.row][move.column] = _playerSign;
             updateBoard(move, _playerSign);
             break;
        }
        else {
            _ui->print("Invalid Move");
        }
    }
    _turn = !_turn;
}

void TicTacToeRand::getComputerMove() {
    vector <Move>freeMoves = findAllFree(); //get a vector of all available moves
    int index = rand() % freeMoves.size();
    updateBoard(freeMoves[index], _computerSign);   //update a board with random available move

    _turn = !_turn;
    _ui->printBoard(_board);
}

void TicTacToeEdu::getComputerMove() {
    vector <Move> freeMoves = findAllFree();
    vector <Move> playerMoves = findPlayerMoves();
    vector <Move> computerMoves = findComputerMoves();

    //find if there is a row/column/diagonal filled with 2 computer's moves 
    for(int i=0; i<computerMoves.size(); i++) {
        for(int j=i+1; j<computerMoves.size(); j++) {
            //check row
            if(computerMoves[i].row == computerMoves[j].row) {
                for(int c=0; c<_board.size(); c++) {
                    if(c!=computerMoves[i].column && c!=computerMoves[j].column && _board[computerMoves[i].row][c]==0) {
                        Move m = { computerMoves[i].row, c };
                        updateBoard(m, _computerSign);
                        _turn = !_turn;
                        _ui->printBoard(_board);
                        return;
                    }
                }
            }
            //check column
            if(computerMoves[i].column == computerMoves[j].column) {
                for(int c=0; c<_board.size(); c++) {
                    if(c!=computerMoves[i].row && c!=computerMoves[j].row && _board[c][computerMoves[i].column]==0) {
                        Move m = { c, computerMoves[i].column };
                        updateBoard(m, _computerSign);
                        _turn = !_turn;
                        _ui->printBoard(_board);
                        return;
                    }
                }
            }
            //check diagonals
            if((computerMoves[i].column == computerMoves[i].row) && (computerMoves[j].column == computerMoves[j].row)) {
                for(int c=0; c<_board.size(); c++) {
                    if(c!=computerMoves[i].column && c!=computerMoves[j].column && _board[c][c]==0) {
                        Move m = { c, c };
                        updateBoard(m, _computerSign);
                        _turn = !_turn;
                        _ui->printBoard(_board);
                        return;
                    }
                }
            }
            if((isSameMove(computerMoves[i],{0,2}) || isSameMove(computerMoves[i],{1,1}) || isSameMove(computerMoves[i],{2,0}))&&
                (isSameMove(computerMoves[j],{0,2}) || isSameMove(computerMoves[j],{1,1}) || isSameMove(computerMoves[j],{2,0}))) {
                for(int c=0; c<_board.size(); c++) {
                    if(c!=computerMoves[i].column && c!=computerMoves[j].column) {
                        for(int r=0; r<_board.size(); r++) {
                            if(r!=computerMoves[i].row && r!=computerMoves[j].row && _board[r][c]==0) {
                                Move m = { r, c };
                                updateBoard(m, _computerSign);
                                _turn = !_turn;
                                _ui->printBoard(_board);
                                return;
                            }
                        }
                    }
                }
            }
        }
    }

    //find if there is a row/column/diagonal filled with 2 player's moves
    for(int i=0; i<playerMoves.size(); i++) {
        for(int j=i+1; j<playerMoves.size(); j++) {
            //check row
            if(playerMoves[i].row == playerMoves[j].row) {
                for(int c=0; c<_board.size(); c++) {
                    if(c!=playerMoves[i].column && c!=playerMoves[j].column && _board[playerMoves[i].row][c]==0) {
                        Move m = { playerMoves[i].row, c };
                        updateBoard(m, _computerSign);
                        _turn = !_turn;
                        _ui->printBoard(_board);
                        return;
                    }
                }
            }
            //check column
            if(playerMoves[i].column == playerMoves[j].column) {
                for(int c=0; c<_board.size(); c++) {
                    if(c!=playerMoves[i].row && c!=playerMoves[j].row && _board[c][playerMoves[i].column]==0) {
                        Move m = { c, playerMoves[i].column };
                        updateBoard(m, _computerSign);
                        _turn = !_turn;
                        _ui->printBoard(_board);
                        return;
                    }
                }
            }
            //check diagonals
            if((playerMoves[i].column == playerMoves[i].row) && (playerMoves[j].column == playerMoves[j].row)) {
                for(int c=0; c<_board.size(); c++) {
                    if(c!=playerMoves[i].column && c!=playerMoves[j].column && _board[c][c]==0) {
                        Move m = { c, c };
                        updateBoard(m, _computerSign);
                        _turn = !_turn;
                        _ui->printBoard(_board);
                        return;
                    }
                }
            }
            if((isSameMove(playerMoves[i],{0,2}) || isSameMove(playerMoves[i],{1,1}) || isSameMove(playerMoves[i],{2,0}))&&
                (isSameMove(playerMoves[j],{0,2}) || isSameMove(playerMoves[j],{1,1}) || isSameMove(playerMoves[j],{2,0}))) {
                for(int c=0; c<_board.size(); c++) {
                    if(c!=playerMoves[i].column && c!=playerMoves[j].column) {
                        for(int r=0; r<_board.size(); r++) {
                            if(r!=playerMoves[i].row && r!=playerMoves[j].row && _board[r][c]==0) {
                                Move m = { r, c };
                                updateBoard(m, _computerSign);
                                _turn = !_turn;
                                _ui->printBoard(_board);
                                return;
                            }
                        }
                    }
                }
            }
        }
    }

    //else - make a random move
    int index = rand() % freeMoves.size();
    updateBoard(freeMoves[index], _computerSign);
    _turn = !_turn;
    _ui->printBoard(_board);
}
