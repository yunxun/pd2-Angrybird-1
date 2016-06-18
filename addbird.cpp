/*#include "addbird.h"
#include <cstdlib>
#include "mainwindow.h"
#define NUM_Bird 5

#define Red_Bird 0
#define Blue_Bird 1
#define Green_Bird 2
#define Yellow_Bird 3
#define Big_Bird 4

extern MainWindow * mainwindow;

Bird * Addbird()
{
   int bird_type;
    bird_type = rand()%NUM_Bird;
    Bird * ptr;

    switch (bird_type) {

     case Red_Bird:
         ptr = new RedBird(game->world);
         break;

     case Green_Bird:
         ptr = new GreenBird(game->world);
         break;

     case Blue_Bird:
         ptr = new BlueBird(game->world);
         break;
     case Yellow_Bird:
         ptr = new YellowBird(game->world);
         break;
    case Big_Bird:
        ptr = new BigBird(game->world);
        break;

    }

    return ptr;

}
*/
