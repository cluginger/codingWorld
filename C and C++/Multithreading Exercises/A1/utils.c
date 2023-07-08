#include <locale.h>
#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include "littlearrow.h"

struct timeval lastdraw;
struct timeval start;
struct timeval shotrefresh;

WINDOW* window;
WINDOW* statuswindow;
int field[FIELD_HEIGHT][FIELD_WIDTH];

int dynamic_refresh = 0;

Player player = {{
  FIELD_WIDTH / 2,
  FIELD_HEIGHT / 2,
  'U'}, LIVES, SHOTS
};


void initField()
{
  srand(time(0));
  gettimeofday(&start, 0);
  gettimeofday(&shotrefresh, 0);
  setlocale(LC_ALL, "");
  initscr();
  checkScreenSize();
  for(int y = 0; y < FIELD_HEIGHT; y++)
  {
    for(int x = 0; x < FIELD_WIDTH; x++)
    {
      field[y][x] = F_EMPTY;
    }
  }

  for(int y = 1; y < FIELD_HEIGHT-1; y++)
  {
    field[y][1] = F_WALL;
    field[y][FIELD_WIDTH-2] = F_WALL;
  }
  for(int x = 2; x < FIELD_WIDTH-2; x++)
  {
    field[1][x] = F_UWALL;
    field[FIELD_HEIGHT-2][x] = F_LWALL;
  }
  
  field[player.pos_.y_][player.pos_.x_] = F_PLAYER_U;
  
  window = newwin(FIELD_HEIGHT,FIELD_WIDTH, 0, 0);
  statuswindow = newwin(STATUS_HEIGHT,FIELD_WIDTH, FIELD_HEIGHT,0);
  drawField();

}

void drawField()
{
  checkScreenSize();
  werase(window);
  werase(statuswindow);
  for(int y = 0; y < FIELD_HEIGHT; y++)
  {
    for(int x = 0; x < FIELD_WIDTH; x++)
    {
      if(field[y][x] != F_EMPTY)
      {
        if(field[y][x] == F_WALL)
          mvwprintw(window,y,x,"\u2588");
        if(field[y][x] == F_LWALL)
          mvwprintw(window,y,x,"\u2584");
        if(field[y][x] == F_UWALL)
          mvwprintw(window,y,x,"\u2580");
        if(field[y][x] == F_GROUND)
          mvwprintw(window,y,x,"\u2591");
        if(field[y][x] == F_PLAYER_U)
          mvwprintw(window,y,x,"\u25B5");
        if(field[y][x] == F_PLAYER_D)
          mvwprintw(window,y,x,"\u25BF");
        if(field[y][x] == F_PLAYER_L)
          mvwprintw(window,y,x,"\u25C3");
        if(field[y][x] == F_PLAYER_R)
          mvwprintw(window,y,x,"\u25B9");
        if(field[y][x] == F_PLAYER_RIP)
          mvwprintw(window,y,x,"\u2620");
        if(field[y][x] == F_PROJECTILE)
          mvwprintw(window,y,x,"\u263c");
        if(field[y][x] == F_ENEMY)
          mvwprintw(window,y,x,"\u2603");
        if(field[y][x] == F_ENEMY_DEAD)
          mvwprintw(window,y,x,"\u2668");
      }
    }
  }
  struct timeval now;
  gettimeofday(&now, 0);
  uint64_t dur_usec = (now.tv_sec - lastdraw.tv_sec) * 1000000 +
    (now.tv_usec - lastdraw.tv_usec);
  uint64_t gamesec = (now.tv_sec - start.tv_sec);
  uint64_t gameusec =  (now.tv_usec - start.tv_usec);
  uint64_t gamedur = (gamesec * 1000000) + gameusec;
  int rps = 1000000 / dur_usec;
  if(rps > 200)
    dynamic_refresh = dynamic_refresh + 25;
  if(rps < 200 && dynamic_refresh > 0)
    dynamic_refresh = dynamic_refresh - 25;

  mvwprintw(statuswindow, 1,FIELD_WIDTH - 10, "%d RPS",rps);
  mvwprintw(statuswindow, 3,1, "TIME:  %.2f seconds", ((float)gamedur / 1000000));
  mvwprintw(statuswindow, 1, 1, "LIVES: ");
  for(int l = 0; l < LIVES; l++)
  {
    mvwprintw(statuswindow, 1, 8+l, "\u2591");
  }
  for(int l = 0; l < player.lives_; l++)
  {
    mvwprintw(statuswindow, 1, 8+l, "\u2592");
  }
  mvwprintw(statuswindow, 1,9 + LIVES, "%d", player.lives_);
  mvwprintw(statuswindow, 2, 1, "SHOTS: ");
  for(int s = 0; s < SHOTS; s++)
  {
    mvwprintw(statuswindow, 2, 8+s, "\u2591");
  }
  for(int s = 0; s < player.shots_; s++)
  {
    mvwprintw(statuswindow, 2, 8+s, "\u2592");
  }
  mvwprintw(statuswindow, 2,9 + SHOTS, "%d", player.shots_);
  curs_set(0);
  box(window,0,0);
  box(statuswindow,0,0);
  wrefresh(window);
  wrefresh(statuswindow);
  gettimeofday(&lastdraw, 0);
  usleep(DEFAULT_SLEEP + dynamic_refresh);
}

void checkScreenSize()
{
  int h,w;
  getmaxyx(stdscr, h ,w);
  if((h < FIELD_HEIGHT + STATUS_HEIGHT + 1) || (w < FIELD_WIDTH + 1))
  {
    endwin();
    printf("Your terminal needs at least a size of %dx%d but has only %dx%d.\n", FIELD_HEIGHT + STATUS_HEIGHT + 1, FIELD_WIDTH + 1, h ,w);
    exit(128);
  }
}

void movePlayerUp()
{
  field[player.pos_.y_][player.pos_.x_] = F_EMPTY;
  if(player.pos_.d_=='U')
  {
    player.pos_.y_--;
    if(field[player.pos_.y_][player.pos_.x_] != F_EMPTY)
    {
      player.pos_.y_++;
      player.lives_ = player.lives_ + PENALTY_HIT_WALL;
    }
  }
  else
  {
    player.pos_.d_='U';
  }
  field[player.pos_.y_][player.pos_.x_] = F_PLAYER_U;
}

void movePlayerDown()
{
  field[player.pos_.y_][player.pos_.x_] = F_EMPTY;
  if(player.pos_.d_=='D')
  {
    player.pos_.y_++;
    if(field[player.pos_.y_][player.pos_.x_] != F_EMPTY)
    {
      player.pos_.y_--;
      player.lives_ = player.lives_ + PENALTY_HIT_WALL;
    }
  }
  else
  {
    player.pos_.d_='D';
  }
  field[player.pos_.y_][player.pos_.x_] = F_PLAYER_D;
}

void movePlayerLeft()
{
  field[player.pos_.y_][player.pos_.x_] = F_EMPTY;
  if(player.pos_.d_=='L')
  {
    player.pos_.x_--;
    if(field[player.pos_.y_][player.pos_.x_] != F_EMPTY)
    {
      player.pos_.x_++;
      player.lives_ = player.lives_ + PENALTY_HIT_WALL;
    }
  }
  else
  {
    player.pos_.d_='L';
  }
  field[player.pos_.y_][player.pos_.x_] = F_PLAYER_L;
}

void movePlayerRight()
{
  field[player.pos_.y_][player.pos_.x_] = F_EMPTY;
  if(player.pos_.d_=='R')
  {
    player.pos_.x_++;
    if(field[player.pos_.y_][player.pos_.x_] != F_EMPTY)
    {
      player.pos_.x_--;
      player.lives_ = player.lives_ + PENALTY_HIT_WALL;
    }
  }
  else
  {
    player.pos_.d_='R';
  }
  field[player.pos_.y_][player.pos_.x_] = F_PLAYER_R;
}

void moveEnemy(Position* pos, int* dead)
{
  if((field[pos->y_][pos->x_]==F_ENEMY_DEAD) || (player.lives_ < 1))
  {
    *dead = 1;
    return;
  }

  int lahX = 0;
  int lahY = 0;

  if(pos->d_ == 'D')
    lahY = 1;
  
  if(pos->d_ == 'U')
    lahY = -1;

  if(pos->d_ == 'R')
    lahX = 1;
  
  if(pos->d_ == 'L')
    lahX = -1;

  if(field[pos->y_+ lahY][pos->x_+lahX] == F_EMPTY)
  {
    if(field[pos->y_][pos->x_] == F_ENEMY)
      field[pos->y_][pos->x_] = F_EMPTY;
    pos->y_ = pos->y_ + lahY;
    pos->x_ = pos->x_ + lahX;
    field[pos->y_][pos->x_] = F_ENEMY;
  }
  else if((field[pos->y_ + lahY][pos->x_ + lahX] == F_PLAYER_L) ||
          (field[pos->y_ + lahY][pos->x_ + lahX] == F_PLAYER_R) ||
          (field[pos->y_ + lahY][pos->x_ + lahX] == F_PLAYER_U) ||
          (field[pos->y_ + lahY][pos->x_ + lahX] == F_PLAYER_D))
  {
    field[pos->y_][pos->x_] = F_EMPTY;
    pos->y_ = pos->y_ + lahY;
    pos->x_ = pos->x_ + lahX;
    field[pos->y_][pos->x_] = F_PLAYER_RIP;
    player.lives_--;
  }
  else
  {
    pos->d_ = randomDir();
  }
  
  int p = rand() % 100;

  if(p > 95)
    pos->d_ = randomDir();
 
  int lookaheadcount = 0; 
  for(int y = pos->y_-1; y > 1; y--)
  {

    lookaheadcount++;

    if((field[y][pos->x_] == F_PLAYER_L) ||
       (field[y][pos->x_] == F_PLAYER_R) ||
       (field[y][pos->x_] == F_PLAYER_U) ||
       (field[y][pos->x_] == F_PLAYER_D))
    {
      if(player.pos_.d_ == 'D')
      {
        if(lookaheadcount < ENEMY_LOOKAHEAD)
          pos->d_ = randomDir();
      }
      else
        pos->d_ = 'U';
    }
  }

  lookaheadcount = 0; 
  for(int y = pos->y_+1; y < FIELD_HEIGHT - 2; y++)
  {

    lookaheadcount++;

    if((field[y][pos->x_] == F_PLAYER_L) ||
       (field[y][pos->x_] == F_PLAYER_R) ||
       (field[y][pos->x_] == F_PLAYER_U) ||
       (field[y][pos->x_] == F_PLAYER_D))
    {
      if(player.pos_.d_ == 'U')
      {
        if(lookaheadcount < ENEMY_LOOKAHEAD)
          pos->d_ = randomDir();
      }
      else
        pos->d_ = 'D';
    }
  }

  lookaheadcount = 0; 
  for(int x = pos->x_-1; x > 1; x--)
  {

    lookaheadcount++;

    if((field[pos->y_][x] == F_PLAYER_L) ||
       (field[pos->y_][x] == F_PLAYER_R) ||
       (field[pos->y_][x] == F_PLAYER_U) ||
       (field[pos->y_][x] == F_PLAYER_D))
    {
      if(player.pos_.d_ == 'R')
      {
        if(lookaheadcount < ENEMY_LOOKAHEAD)
          pos->d_ = randomDir();
      }
      else
        pos->d_ = 'L';
    }
  }

  lookaheadcount = 0; 
  for(int x = pos->x_+1; x < FIELD_WIDTH - 2; x++)
  {

    lookaheadcount++;

    if((field[pos->y_][x] == F_PLAYER_L) ||
       (field[pos->y_][x] == F_PLAYER_R) ||
       (field[pos->y_][x] == F_PLAYER_U) ||
       (field[pos->y_][x] == F_PLAYER_D))
    {
      if(player.pos_.d_ == 'L')
      {
        if(lookaheadcount < ENEMY_LOOKAHEAD)
          pos->d_ = randomDir();
      }
      else
        pos->d_ = 'R';
    }
  }

  if((pos->d_ == 'U') || (pos->d_ == 'D'))
    usleep(DEFAULT_ENEMY_SLEEP_TIME * 2);
  
  if((pos->d_ == 'L') || (pos->d_ == 'R'))
    usleep(DEFAULT_ENEMY_SLEEP_TIME);
}

void reloadShots()
{
  if(field[player.pos_.y_][player.pos_.x_] == F_PLAYER_RIP)
    return;

  struct timeval now;
  gettimeofday(&now, 0);
  uint64_t dur_usec = (now.tv_sec - shotrefresh.tv_sec) * 1000000 +
    (now.tv_usec - shotrefresh.tv_usec);
  if(dur_usec > RELOAD_TIME)
  {
    if(player.shots_ < SHOTS)
      player.shots_++;
    gettimeofday(&shotrefresh, 0);
  }
}

void shoot(Position* pos)
{

  if(field[player.pos_.y_][player.pos_.x_] == F_PLAYER_RIP)
    return;
  
  if(player.shots_ == 0)
    return;

  player.shots_--;

  int x = 0;
  int y = 0;

  if(pos->d_ == 'L')
    x = -1;

  if(pos->d_ == 'R')
    x = 1;

  if(pos->d_ == 'U')
    y = -1;

  if(pos->d_ == 'D')
    y = 1;

  for(int dist = 0; dist < max(FIELD_WIDTH, FIELD_HEIGHT); dist++)
  {
    if((dist > 0) && field[pos->y_][pos->x_] == F_PROJECTILE)
      field[pos->y_][pos->x_] = F_EMPTY;

    if((pos->y_ > 0) && (pos->y_ < FIELD_HEIGHT))
      pos->y_ = pos->y_ + y;
    if((pos->x_ > 0) && (pos->x_ < FIELD_WIDTH))
      pos->x_ = pos->x_ + x;

    if(field[pos->y_][pos->x_] == F_EMPTY)
      field[pos->y_][pos->x_] = F_PROJECTILE;

    if(field[pos->y_][pos->x_] == F_ENEMY)
    {
      field[pos->y_][pos->x_] = F_ENEMY_DEAD;
      break;
    }

    if(field[pos->y_][pos->x_] == F_WALL)
    {
      break;
    }

    usleep(DEFAULT_SHOOT_SLEEP_TIME);
  }    

}

void endGame()
{
  struct timeval now;
  gettimeofday(&now, 0);
  uint64_t gamesec = (now.tv_sec - start.tv_sec);
  uint64_t gameusec =  (now.tv_usec - start.tv_usec);
  float gamedur = ((float)((gamesec * 1000000) + gameusec)/1000000);
  werase(window);
  mvwprintw(window, (FIELD_HEIGHT/2)-1,(FIELD_WIDTH/2) - 5, "              ");
  mvwprintw(window, FIELD_HEIGHT/2,(FIELD_WIDTH/2) - 5, "GAME  OVER");
  if(player.lives_ > 0)
    mvwprintw(window, (FIELD_HEIGHT/2) + 1,(FIELD_WIDTH/2) - 4, "YOU WON!");
  else
    mvwprintw(window, (FIELD_HEIGHT/2) + 1,(FIELD_WIDTH/2) - 4, "YOU LOST");
  mvwprintw(window, (FIELD_HEIGHT/2)+2,(FIELD_WIDTH/2) - 4, " %3.1f s", gamedur);
  mvwprintw(window, (FIELD_HEIGHT/2)+3,(FIELD_WIDTH/2) - 5, "              ");
  curs_set(0);
  box(window,0,0);
  box(statuswindow,0,0);
  wrefresh(window);
  wrefresh(statuswindow);
  sleep(5);
  endwin();
}

int randomY()
{
  int y = (rand() % FIELD_HEIGHT);
  if(y<3)
    y = 3;
  if(y > FIELD_HEIGHT - 3)
    y = FIELD_HEIGHT - 3;
  return y;
}

int randomX()
{
  int x = (rand() % FIELD_WIDTH);
  if(x < 3)
    x = 3;
  if(x > FIELD_WIDTH - 3)
    x = FIELD_WIDTH - 3;
  return x;
}

char randomDir()
{
  int x = rand() % 4;
  if(x == 0)
    return 'U';
  else if(x == 1)
    return 'D';
  else if(x == 2)
    return 'L';
  else
    return 'R';
}

int max(int a, int b)
{
  if(a > b)
    return a;
  else
    return b;
}
