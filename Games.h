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

    void start() { cout<<"Graphical interface unavailable."<<endl; }
    int chooseGame() { return 0; }
    int chooseDifficulty() { return 0; }

private:

};

/*======================== Game logic part ============================*/

struct GameState {
    enum state { O , X , DRAW , CONTINUE };
};


class Game {
public: 
    Game(){};
    virtual ~Game(){};

    //void getPlayerMove(Move);
    virtual void getComputerMove() {}
    //GameState getGameState();

private:
};


class BoardGame: public Game {
public:
    BoardGame():Game() {}
    BoardGame(vector<vector<char>>):Game() {}
    virtual ~BoardGame(){};

    virtual void getComputerMove() {}
    //virtual GameState getGameState();

private:
    vector<vector<char>> board;
    GameState _state;
};

/*======================== TicTacToe Game ============================*/

class TicTacToeGame: public BoardGame {
public:
    TicTacToeGame(): BoardGame() {}
    TicTacToeGame(vector<vector<char>> board): BoardGame(board) {}
    virtual ~TicTacToeGame(){};

    virtual void getComputerMove() {}
    //virtual GameState getGameState(){}
    //void getPlayerMove(Move);
    //GameState getGameState();

private:
};


class TicTacToeEdu: public TicTacToeGame {
public:
    TicTacToeEdu(): TicTacToeGame() {}
    TicTacToeEdu(vector<vector<char>> board): TicTacToeGame(board) {}
    ~TicTacToeEdu(){};
    
    void getComputerMove(){ cout<<"It's Edu"<<endl; }
    //GameState getGameState(){}
    //void getPlayerMove(Move);

private:
};


class TicTacToeRand: public TicTacToeGame {
public:
    TicTacToeRand(): TicTacToeGame() {}
    TicTacToeRand(vector<vector<char>> board): TicTacToeGame(board) {}
    ~TicTacToeRand(){};
    
    void getComputerMove(){ cout<<"It's Rand"<<endl; }
    //GameState getGameState(){}
    //void getPlayerMove(Move);

private:
};