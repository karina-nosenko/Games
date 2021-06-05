#include "Games.h"

int main(int argc, char const *argv[])
{
    //argv[1] = "console" for console ui, "gui" for graphical ui
    GameController myGame(argv[1]);
    myGame.play();  

    return 0;
}
