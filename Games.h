#pragma once
#include <iostream>
#include <string.h>
#include <string>
#include <vector>

using namespace std;

class GameController;
class GamesUI;
class Console;
class GUI;
class Game;
class BoardGame;
class TicTacToeGame;

enum GameState { O , X , DRAW , PLAY };
#define  BOARD_SIZE 3

struct Move {
    int row;
    int column;
};

/*============= Bound between Game UI and the Game logic ================*/

class GameController {
public: 
    GameController(char const* = "console");
    ~GameController();

    void play();

private:
    GamesUI* _ui;
    Game* _game;
};

/*============================ UI part =================================*/

class GamesUI {
public: 
    GamesUI(){}
    virtual ~GamesUI(){}

    virtual void start()=0;
    virtual int chooseGame()=0;
    virtual int chooseDifficulty()=0;

    virtual void print(string msg)=0;
    virtual void printBoard(vector<vector<char>> board) const=0;
    virtual int ticTacToeSign()=0;
    virtual Move getMove(int, int)=0;

private:
};


class Console: public GamesUI{
public: 
    Console():GamesUI(){}
    ~Console(){}

    void start();
    int chooseGame();
    int chooseDifficulty();

    void print(string msg) { cout<<msg<<endl; }
    void printBoard(vector<vector<char>> board) const;
    int ticTacToeSign();
    Move getMove(int, int);

private:

};


class GUI: public GamesUI {
public: 
    GUI():GamesUI(){}
    ~GUI(){}

    void start() { cout<<"Graphical interface unavailable."<<endl; exit(1); }
    int chooseGame() { return 0; }
    int chooseDifficulty() { return 0; }

    void print(string msg) { cout<<msg<<endl; }
    void printBoard(vector<vector<char>> board) const {}
    int ticTacToeSign() { return 0; }
    Move getMove(int, int) { Move move{0,0}; return move; }

private:

};

/*======================== Game logic part ============================*/

class Game {
public: 
    Game(): _turn(0) {}
    virtual ~Game() {};

    virtual void start()=0;
    virtual void getPlayerMove()=0;
    virtual void getComputerMove()=0;
    virtual GameState getGameState() const=0;
    virtual void updateGameState()=0;
    virtual vector<vector<char>> getBoard() const=0;

    virtual bool getTurn() const        { return _turn; }
    virtual void setTurn( bool turn )   { _turn = turn; }
    void setUI( GamesUI& ui)      { _ui = &ui; }

protected:
    bool _turn; // 0-computer, 1-player
    GamesUI* _ui;
};


class BoardGame: public Game {
public:
    BoardGame();
    BoardGame(vector<vector<char>>);
    virtual ~BoardGame(){};

    virtual void start(){}
    virtual void getPlayerMove(){}
    virtual void getComputerMove(){}
    virtual void updateGameState(){}
    void updateBoard(Move move, char sign) { _board[move.row][move.column] = sign; }
    bool isFullBoard() const;

    bool getTurn() const                    { return _turn; }
    vector<vector<char>> getBoard() const   { return _board; }
    virtual GameState getGameState() const  { return _state; }

    void setTurn( bool turn )                   { _turn = turn; }   
    void setGameState( GameState state )        { _state = state; }

protected:
    vector<vector<char>> _board;
    GameState _state;
};

/*======================== TicTacToe Game ============================*/

class TicTacToeGame: public BoardGame {
public:
    TicTacToeGame(): BoardGame(), _playerSign('X'), _computerSign('O') {}
    TicTacToeGame(vector<vector<char>> board): BoardGame(board), _playerSign('X'), _computerSign('O') {}
    virtual ~TicTacToeGame(){};

    void start();
    void getPlayerMove();
    virtual void getComputerMove(){}
    bool isFree(Move) const;
    vector <Move> findAllFree() const;
    void updateGameState();

    GameState getGameState() const { return _state; }

protected:
    char _playerSign;
    char _computerSign;
};


class TicTacToeEdu: public TicTacToeGame {
public:
    TicTacToeEdu(): TicTacToeGame() {}
    TicTacToeEdu(vector<vector<char>> board): TicTacToeGame(board) {}
    ~TicTacToeEdu(){};
    
    void getComputerMove(){cout<<"Educated computer!"<<endl;}

private:
};


class TicTacToeRand: public TicTacToeGame {
public:
    TicTacToeRand(): TicTacToeGame() {}
    TicTacToeRand(vector<vector<char>> board): TicTacToeGame(board) {}
    ~TicTacToeRand(){};
    
    void getComputerMove();

private:
};