#include "Games.h"

/*Returns true if the string represents a number*/
bool isNum(string num) {
    for(int i=0; i<num.length(); i++) {
        if (!isdigit(num[i]))
            return false;
    }
    return true;
}

/*============= Bound between Game UI and the Game logic ================*/

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
    int gameOption;
    int difficulty;
    vector<vector<char>> board; 

    _ui->start();
    gameOption = _ui->chooseGame();
    difficulty = _ui->chooseDifficulty();

    //assign the game
    switch(gameOption) {
        case 1:
            board.resize ( 3 , vector<char> (3,0) ); //define board 3x3 filled with zero
            
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

    _game->start();

    //play the game
    while(_game->getGameState() == PLAY) {
        _game->getTurn() ? _game->getPlayerMove() : _game->getComputerMove();
        //check the game state (getGameState())
    }

    //print the result
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


//Get player sign ( 0=O, 1=X )
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
        cin>>c;
        cin>>r;

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


//check if the place on the board is free
bool TicTacToeGame::isFree(Move move) const {
    return _board[move.row][move.column] == 0;
}


void TicTacToeGame::start() {
    _ui->print("_____ Tic Tac Toe_____ ");                    //print the game name
    _turn = _ui->ticTacToeSign();                             //get a sign from the user to determine the turn (X starts first)
    _ui->print("Format: x y [x-column index, y-row index]");  //print the game format    
    _ui->printBoard(_board);                                  //print the game board
}


void TicTacToeGame::getPlayerMove() {
    Move move;

    while(true) {
        move = _ui->getMove(2, 2);
        if(isFree(move)) {
             _board[move.row][move.column] = 'X';
             break;
        }
        else {
            _ui->print("Invalid Move");
        }
    }

    _ui->printBoard(_board);
    _turn = !_turn;
}

void TicTacToeRand::getComputerMove() {
    cout<<"Computer move"<<endl;
    exit(1);
}