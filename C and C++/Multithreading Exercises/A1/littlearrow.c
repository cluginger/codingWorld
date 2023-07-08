#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "littlearrow.h"

int main(int argc, char *argv[])
{
    /*
     * As you might already have noticed, this version of our little game
     * doesn't work as expected. It's your task to fix the game and create
     * a playable version. The Player should be moved with wasd and fire his
     * shots using space. The game should end after the player shot all enemies
     * or all his lives are used. Make sure to create the correct number of
     * enemies and to redraw the screen from time to time. It will also be
     * necessary to reload your weapon.
     *
     * Hints and rules:
     *
     * - It is neither allowed nor necessary to change anything except this file!
     * - As you might have guessed from the assignment title, you will have to
     *   use threads to get the correct behaviour.
     * - Think about why you need threads to achieve the desired behaviour.
     * - Dont change the pthread_setcanceltype statements below.
     * - You will need ncurses: apt install libncurses-dev libncurses6 on
     *   current ubuntu versions.
     * - It is not necessary to change or implement any game logic, all bits and
     *   pieces are already there.
     * - Try to understand how the game works, you might get asked in your AGs.
     * - Don't forget to clean up before you exit.
     * - You don't need to write a lot of code!
     *   (less than 100 lines should do the job)
     *
     * Our simple and lazy approach below did not work out very well. But
     * we are sure you can do better and create a working version which is more
     * fun to play. Feel free to change the code below as you need. It should
     * not be necessary to add additional functions or global variables.
     *
     */
    player.pos_.d_ = randomDir();
    initField();
    pthread_t id_drawing;
    pthread_t id_player;
    pthread_t id_enemies[ENEMIES];
    pthread_create(&id_drawing, NULL, funcDraw, NULL);
    pthread_create(&id_player, NULL, funcPlayer, NULL);
    for(int i =0; i < ENEMIES; i++)
    {
        pthread_create(&id_enemies[i], NULL, funcEnemy, NULL);
    }
    for(int i =0; i < ENEMIES; i++)
    {
        pthread_join(id_enemies[i], NULL);
    }
    pthread_cancel(id_drawing);
    pthread_cancel(id_player);
    endGame();
}

void *funcDraw()
{
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    while(1)
    {
        drawField();
    }
    return 0;
}

void *funcPlayer(){
    pthread_t id_shooting;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    while(1) {
        int ch = getchar();
        if(ch == ' ')
        {
            pthread_create(&id_shooting, NULL, funcShoot, NULL);
        }
        if (ch == 'a') {
            player.pos_.d_ = 'L';
            movePlayerLeft();
        }
        if (ch == 's') {
            player.pos_.d_ = 'D';
            movePlayerDown();
        }
        if (ch == 'd') {
            player.pos_.d_ = 'R';
            movePlayerRight();
        }
        if (ch == 'w') {
            player.pos_.d_ = 'U';
            movePlayerUp();
        }
    }
    pthread_cancel(id_shooting);
    return 0;
}

void *funcEnemy()
{
    int d = 0;
    Position enemy = {randomX(), randomY(), randomDir()};
    while(d != 1) {
        moveEnemy(&enemy, &d);
    }

    return 0;
}

void *funcShoot()
{
    Position spos = { player.pos_.x_, player.pos_.y_, player.pos_.d_ };
    shoot(&spos);
    reloadShots();

    return 0;
}
