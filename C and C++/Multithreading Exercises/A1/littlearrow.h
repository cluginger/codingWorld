// FIELD TYPES
#define F_EMPTY 0
#define F_WALL 1
#define F_UWALL 2
#define F_LWALL 3
#define F_GROUND 4
#define F_PLAYER_L 10
#define F_PLAYER_R 11
#define F_PLAYER_U 12
#define F_PLAYER_D 13
#define F_PLAYER_RIP 14
#define F_PROJECTILE 100
#define F_ENEMY 200
#define F_ENEMY_DEAD 201

// FILED SIZE
#define FIELD_HEIGHT 40
#define FIELD_WIDTH 120
#define STATUS_HEIGHT 5

//Enemies
#define ENEMIES 10
#define ENEMY_LOOKAHEAD 14

//Speeds
#define DEFAULT_SLEEP 100
#define DEFAULT_ENEMY_SLEEP_TIME 50000
#define DEFAULT_SHOOT_SLEEP_TIME 20000

//Points
#define PENALTY_HIT_WALL 0
#define LIVES 10
#define SHOTS 15

#define RELOAD_TIME 1000000

typedef struct __attribute__((__packed__)) {
  short x_;
  short y_;
  char d_;
}Position;

typedef struct __attribute__((__packed__)) {
  Position pos_;
  int lives_;
  int shots_;
}Player;


extern WINDOW* window;
extern WINDOW* statuswindow;
extern int field[FIELD_HEIGHT][FIELD_WIDTH];
extern struct timeval lastdraw;
extern Player player;

void initField();
void drawField();
void checkScreenSize();

void movePlayerUp();
void movePlayerDown();
void movePlayerLeft();
void movePlayerRight();

void moveEnemy(Position*, int*);

void reloadShots();
void shoot(Position*);

void endGame();

int randomX();
int randomY();
char randomDir();

int max(int, int);

void *funcPlayer();
void *funcEnemy();
void *funcDraw();
void *funcShoot();
