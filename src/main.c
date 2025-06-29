#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// Constantes do jogo
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 600
#define MAP_WIDTH 60
#define MAP_HEIGHT 25
#define TILE_SIZE 20
#define GAME_AREA_HEIGHT 500
#define UI_HEIGHT 100
#define MAX_BOMBS 10
#define MAX_ENEMIES 10
#define BOMB_TIMER 3.0f
#define EXPLOSION_SIZE 100

// Enums
typedef enum {
    GAME_PLAYING,
    GAME_MENU,
    GAME_OVER,
    GAME_WIN
} GameState;

typedef enum {
    MENU_NEW_GAME,
    MENU_LOAD_GAME,
    MENU_SAVE_GAME,
    MENU_QUIT,
    MENU_BACK
} MenuOption;

typedef enum {
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT
} Direction;

// Estruturas
typedef struct {
    int x, y;
} Position;

typedef struct {
    Position pos;
    int lives;
    int score;
    int bombCount;
    int keysCollected;
} Player;

typedef struct {
    Position pos;
    Direction dir;
    float moveTimer;
} Enemy;

typedef struct {
    Position pos;
    float timer;
    bool active;
} Bomb;

typedef struct {
    char **grid;
    int currentLevel;
    int totalKeys;
} GameMap;

typedef struct {
    Player player;
    Enemy enemies[MAX_ENEMIES];
    int enemyCount;
    Bomb bombs[MAX_BOMBS];
    int bombCount;
    GameMap map;
    GameState state;
    MenuOption selectedMenu;
    float gameTime;
} Game;

// Protótipos das funções
Game* InitGame(void);
void LoadMap(Game *game, int level);
void UpdateGame(Game *game);
void DrawGame(Game *game);
void DrawMenu(Game *game);
void HandleInput(Game *game);
void PlantBomb(Game *game);
void UpdateBombs(Game *game);
void UpdateEnemies(Game *game);
void ExplodeBomb(Game *game, int bombIndex);
bool CheckCollision(Position pos1, Position pos2);
void MovePlayer(Game *game, Direction dir);
void MoveEnemy(Game *game, int enemyIndex, Direction dir);
bool IsValidPosition(Game *game, Position pos);
void SaveGame(Game *game);
void LoadGame(Game *game);
void FreeGame(Game *game);

// Função principal
int main(void) {
 
}