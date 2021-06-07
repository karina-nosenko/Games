#include "Games.h"

int main(int argc, char const *argv[])
{
    //pass to myGame argv[1] = "console" for console ui, "gui" for graphical ui. Console is default
    GameController myGame;
    myGame.play();  

    return 0;
}
