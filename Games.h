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

protected:

private:
};


class Console: public GamesUI{
public: 
    Console():GamesUI(){}
    ~Console(){}

    void start();
    int chooseGame();
    int chooseDifficulty();

private:

};


class GUI: public GamesUI {
public: 
    GUI():GamesUI(){}
    ~GUI(){}

    void start() { cout<<"Graphical interface unavailable."<<endl; exit(1); }
    int chooseGame() { return 0; }
    int chooseDifficulty() { return 0; }

private:

};

/*======================== Game logic part ============================*/


enum GameState { O , X , DRAW , PLAY };


struct Move {
    int row;
    int column;
};


class Game {
public: 
    Game(): _turn(0) {}
    virtual ~Game() {};

    virtual void start()=0;
    virtual void getPlayerMove()=0;
    virtual void getComputerMove()=0;
    virtual GameState getGameState() const=0;

    virtual bool getTurn() const        { return _turn; }
    virtual void setTurn( bool turn )   { _turn = turn; }

protected:
    bool _turn; // 0-computer, 1-player
};


class BoardGame: public Game {
public:
    BoardGame();
    BoardGame(vector<vector<char>>);
    virtual ~BoardGame(){};

    void print() const;

    virtual void start(){}
    virtual void getPlayerMove(){}
    virtual void getComputerMove(){}

    bool getTurn() const                    { return _turn; }
    vector<vector<char>> getBoard() const   { return _board; }
    virtual GameState getGameState() const  { return _state; }

    void setTurn( bool turn )                   { _turn = turn; }   
    void setBoard( vector<vector<char>> board)  { _board = board; }
    void setGameState( GameState state )        { _state = state; }

protected:
    vector<vector<char>> _board;
    GameState _state;
};

/*======================== TicTacToe Game ============================*/

class TicTacToeGame: public BoardGame {
public:
    TicTacToeGame(): BoardGame() {}
    TicTacToeGame(vector<vector<char>> board): BoardGame(board) {}
    virtual ~TicTacToeGame(){};

    void start();
    void getPlayerMove(){cout<<"Player!"<<endl;}
    virtual void getComputerMove(){}

    GameState getGameState() const { return _state; }

private:
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
    
    void getComputerMove(){cout<<"Random computer!"<<endl;}

private:
};