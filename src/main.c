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
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Bomberman - Programação II");
    SetTargetFPS(60);
    
    srand(time(NULL));
    
    Game *game = InitGame();
    LoadMap(game, 1);
    
    while (!WindowShouldClose()) {
        HandleInput(game);
        
        if (game->state == GAME_PLAYING) {
            UpdateGame(game);
        }
        
        BeginDrawing();
        ClearBackground(BLACK);
        
        if (game->state == GAME_PLAYING) {
            DrawGame(game);
        } else if (game->state == GAME_MENU) {
            DrawMenu(game);
        }
        
        EndDrawing();
    }
    
    FreeGame(game);
    CloseWindow();
    
    return 0;
}

// Inicialização do jogo
Game* InitGame(void) {
    Game *game = (Game*)malloc(sizeof(Game));
    
    // Inicializar jogador
    game->player.pos.x = 0;
    game->player.pos.y = 0;
    game->player.lives = 3;
    game->player.score = 0;
    game->player.bombCount = 3;
    game->player.keysCollected = 0;
    
    // Inicializar estado
    game->state = GAME_PLAYING;
    game->selectedMenu = MENU_NEW_GAME;
    game->gameTime = 0.0f;
    game->enemyCount = 0;
    game->bombCount = 0;
    
    // Inicializar mapa
    game->map.grid = (char**)malloc(MAP_HEIGHT * sizeof(char*));
    for (int i = 0; i < MAP_HEIGHT; i++) {
        game->map.grid[i] = (char*)malloc(MAP_WIDTH * sizeof(char));
    }
    game->map.currentLevel = 1;
    game->map.totalKeys = 5;
    
    // Inicializar bombas
    for (int i = 0; i < MAX_BOMBS; i++) {
        game->bombs[i].active = false;
    }
    
    return game;
}

// Carregamento do mapa
void LoadMap(Game *game, int level) {
    char filename[50];
    sprintf(filename, "mapa%d.txt", level);
    
    FILE *file = fopen(filename, "r");
    if (!file) {
        // Criar mapa padrão se arquivo não existir
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                if (x == 0 || x == MAP_WIDTH-1 || y == 0 || y == MAP_HEIGHT-1) {
                    game->map.grid[y][x] = 'W';
                } else if (x == 1 && y == 1) {
                    game->map.grid[y][x] = 'J';
                    game->player.pos.x = x;
                    game->player.pos.y = y;
                } else if ((x + y) % 4 == 0 && x > 2 && y > 2) {
                    game->map.grid[y][x] = (rand() % 100 < 20) ? 'E' : 'B';
                } else if ((x + y) % 6 == 0 && x > 5 && y > 5) {
                    game->map.grid[y][x] = 'K';
                } else {
                    game->map.grid[y][x] = ' ';
                }
            }
        }
    } else {
        // Carregar do arquivo
        game->enemyCount = 0;
        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                char c = fgetc(file);
                if (c == '\n') c = fgetc(file);
                
                game->map.grid[y][x] = c;
                
                if (c == 'J') {
                    game->player.pos.x = x;
                    game->player.pos.y = y;
                    game->map.grid[y][x] = ' ';
                } else if (c == 'E' && game->enemyCount < MAX_ENEMIES) {
                    game->enemies[game->enemyCount].pos.x = x;
                    game->enemies[game->enemyCount].pos.y = y;
                    game->enemies[game->enemyCount].dir = rand() % 4;
                    game->enemies[game->enemyCount].moveTimer = 0.0f;
                    game->enemyCount++;
                    game->map.grid[y][x] = ' ';
                }
            }
        }
        fclose(file);
    }
    
    game->map.currentLevel = level;
}

// Atualização do jogo
void UpdateGame(Game *game) {
    game->gameTime += GetFrameTime();
    
    UpdateBombs(game);
    UpdateEnemies(game);
    
    // Verificar colisão com inimigos
    for (int i = 0; i < game->enemyCount; i++) {
        if (CheckCollision(game->player.pos, game->enemies[i].pos)) {
            game->player.lives--;
            game->player.score = (game->player.score > 100) ? game->player.score - 100 : 0;
            
            if (game->player.lives <= 0) {
                game->state = GAME_OVER;
            }
            
            // Reposicionar jogador
            game->player.pos.x = 1;
            game->player.pos.y = 1;
        }
    }
    
    // Verificar vitória
    if (game->player.keysCollected >= game->map.totalKeys) {
        game->map.currentLevel++;
        game->player.keysCollected = 0;
        LoadMap(game, game->map.currentLevel);
    }
}

// Desenho do jogo
void DrawGame(Game *game) {
    // Desenhar mapa
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            Rectangle rect = {x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE};
            
            switch (game->map.grid[y][x]) {
                case 'W':
                    DrawRectangleRec(rect, DARKGRAY);
                    break;
                case 'D':
                    DrawRectangleRec(rect, BROWN);
                    break;
                case 'B':
                case 'K':
                    DrawRectangleRec(rect, ORANGE);
                    break;
                case 'K':
                    if (game->map.grid[y][x] == 'K') {
                        DrawRectangleRec(rect, GOLD);
                    }
                    break;
            }
        }
    }
    
    // Desenhar jogador
    Rectangle playerRect = {
        game->player.pos.x * TILE_SIZE, 
        game->player.pos.y * TILE_SIZE, 
        TILE_SIZE, 
        TILE_SIZE
    };
    DrawRectangleRec(playerRect, BLUE);
    
    // Desenhar inimigos
    for (int i = 0; i < game->enemyCount; i++) {
        Rectangle enemyRect = {
            game->enemies[i].pos.x * TILE_SIZE,
            game->enemies[i].pos.y * TILE_SIZE,
            TILE_SIZE,
            TILE_SIZE
        };
        DrawRectangleRec(enemyRect, RED);
    }
    
    // Desenhar bombas
    for (int i = 0; i < MAX_BOMBS; i++) {
        if (game->bombs[i].active) {
            Rectangle bombRect = {
                game->bombs[i].pos.x * TILE_SIZE,
                game->bombs[i].pos.y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE
            };
            DrawRectangleRec(bombRect, BLACK);
            DrawRectangleLinesEx(bombRect, 2, WHITE);
        }
    }
    
    // Desenhar UI
    DrawRectangle(0, GAME_AREA_HEIGHT, SCREEN_WIDTH, UI_HEIGHT, DARKBLUE);
    
    char uiText[200];
    sprintf(uiText, "Vidas: %d | Pontos: %d | Bombas: %d | Chaves: %d/%d | Nível: %d", 
            game->player.lives, game->player.score, game->player.bombCount, 
            game->player.keysCollected, game->map.totalKeys, game->map.currentLevel);
    
    DrawText(uiText, 10, GAME_AREA_HEIGHT + 10, 20, WHITE);
    DrawText("TAB: Menu | WASD/Setas: Mover | B: Bomba", 10, GAME_AREA_HEIGHT + 40, 16, LIGHTGRAY);
}

// Desenho do menu
void DrawMenu(Game *game) {
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 180});
    
    char *menuItems[] = {"Novo Jogo (N)", "Carregar Jogo (C)", "Salvar Jogo (S)", "Sair (Q)", "Voltar (V)"};
    int menuCount = 5;
    
    int startY = SCREEN_HEIGHT / 2 - (menuCount * 30) / 2;
    
    DrawText("MENU", SCREEN_WIDTH / 2 - 50, startY - 60, 40, WHITE);
    
    for (int i = 0; i < menuCount; i++) {
        Color color = (i == game->selectedMenu) ? YELLOW : WHITE;
        DrawText(menuItems[i], SCREEN_WIDTH / 2 - 100, startY + i * 40, 20, color);
    }
}

// Tratamento de entrada
void HandleInput(Game *game) {
    if (game->state == GAME_MENU) {
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
            game->selectedMenu = (game->selectedMenu - 1 + 5) % 5;
        } else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
            game->selectedMenu = (game->selectedMenu + 1) % 5;
        } else if (IsKeyPressed(KEY_ENTER)) {
            switch (game->selectedMenu) {
                case MENU_NEW_GAME:
                    // Resetar jogo
                    game->player.lives = 3;
                    game->player.score = 0;
                    game->player.bombCount = 3;
                    game->player.keysCollected = 0;
                    LoadMap(game, 1);
                    game->state = GAME_PLAYING;
                    break;
                case MENU_LOAD_GAME:
                    LoadGame(game);
                    game->state = GAME_PLAYING;
                    break;
                case MENU_SAVE_GAME:
                    SaveGame(game);
                    game->state = GAME_PLAYING;
                    break;
                case MENU_QUIT:
                    exit(0);
                    break;
                case MENU_BACK:
                    game->state = GAME_PLAYING;
                    break;
            }
        } else if (IsKeyPressed(KEY_N)) {
            game->selectedMenu = MENU_NEW_GAME;
        } else if (IsKeyPressed(KEY_C)) {
            game->selectedMenu = MENU_LOAD_GAME;
        } else if (IsKeyPressed(KEY_S)) {
            game->selectedMenu = MENU_SAVE_GAME;
        } else if (IsKeyPressed(KEY_Q)) {
            game->selectedMenu = MENU_QUIT;
        } else if (IsKeyPressed(KEY_V)) {
            game->selectedMenu = MENU_BACK;
        }
    } else if (game->state == GAME_PLAYING) {
        if (IsKeyPressed(KEY_TAB)) {
            game->state = GAME_MENU;
        } else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
            MovePlayer(game, DIR_UP);
        } else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
            MovePlayer(game, DIR_DOWN);
        } else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
            MovePlayer(game, DIR_LEFT);
        } else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
            MovePlayer(game, DIR_RIGHT);
        } else if (IsKeyPressed(KEY_B)) {
            PlantBomb(game);
        }
    }
}

// Implementações das funções auxiliares
bool CheckCollision(Position pos1, Position pos2) {
    return (pos1.x == pos2.x && pos1.y == pos2.y);
}

bool IsValidPosition(Game *game, Position pos) {
    if (pos.x < 0 || pos.x >= MAP_WIDTH || pos.y < 0 || pos.y >= MAP_HEIGHT) {
        return false;
    }
    
    char tile = game->map.grid[pos.y][pos.x];
    return (tile == ' ' || tile == 'K');
}

void MovePlayer(Game *game, Direction dir) {
    Position newPos = game->player.pos;
    
    switch (dir) {
        case DIR_UP: newPos.y--; break;
        case DIR_DOWN: newPos.y++; break;
        case DIR_LEFT: newPos.x--; break;
        case DIR_RIGHT: newPos.x++; break;
    }
    
    if (IsValidPosition(game, newPos)) {
        // Verificar se é uma chave
        if (game->map.grid[newPos.y][newPos.x] == 'K') {
            game->player.keysCollected++;
            game->map.grid[newPos.y][newPos.x] = ' ';
        }
        
        game->player.pos = newPos;
    }
}

void PlantBomb(Game *game) {
    if (game->player.bombCount <= 0) return;
    
    // Encontrar slot vazio para bomba
    for (int i = 0; i < MAX_BOMBS; i++) {
        if (!game->bombs[i].active) {
            game->bombs[i].pos = game->player.pos;
            game->bombs[i].timer = BOMB_TIMER;
            game->bombs[i].active = true;
            game->player.bombCount--;
            break;
        }
    }
}

void UpdateBombs(Game *game) {
    for (int i = 0; i < MAX_BOMBS; i++) {
        if (game->bombs[i].active) {
            game->bombs[i].timer -= GetFrameTime();
            if (game->bombs[i].timer <= 0) {
                ExplodeBomb(game, i);
            }
        }
    }
}

void ExplodeBomb(Game *game, int bombIndex) {
    Position bombPos = game->bombs[bombIndex].pos;
    game->bombs[bombIndex].active = false;
    game->player.bombCount++;
    
    // Verificar dano ao jogador
    float distToPlayer = sqrt(pow(game->player.pos.x - bombPos.x, 2) + pow(game->player.pos.y - bombPos.y, 2));
    if (distToPlayer * TILE_SIZE <= EXPLOSION_SIZE / 2) {
        game->player.lives--;
        game->player.score = (game->player.score > 100) ? game->player.score - 100 : 0;
    }
    
    // Destruir elementos em cruz
    int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    
    for (int d = 0; d < 4; d++) {
        for (int range = 1; range <= 2; range++) {
            int x = bombPos.x + directions[d][0] * range;
            int y = bombPos.y + directions[d][1] * range;
            
            if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) break;
            
            char tile = game->map.grid[y][x];
            
            if (tile == 'W') break; // Parede indestrutível
            
            if (tile == 'D' || tile == 'B') {
                game->map.grid[y][x] = ' ';
                game->player.score += 10;
                break;
            }
            
            if (tile == 'K') {
                // Chave revelada
                game->player.score += 10;
                break;
            }
        }
    }
    
    // Verificar inimigos na explosão
    for (int i = 0; i < game->enemyCount; i++) {
        float distToEnemy = sqrt(pow(game->enemies[i].pos.x - bombPos.x, 2) + pow(game->enemies[i].pos.y - bombPos.y, 2));
        if (distToEnemy * TILE_SIZE <= EXPLOSION_SIZE / 2) {
            // Remover inimigo
            for (int j = i; j < game->enemyCount - 1; j++) {
                game->enemies[j] = game->enemies[j + 1];
            }
            game->enemyCount--;
            game->player.score += 20;
            i--;
        }
    }
}

void UpdateEnemies(Game *game) {
    for (int i = 0; i < game->enemyCount; i++) {
        game->enemies[i].moveTimer += GetFrameTime();
        
        if (game->enemies[i].moveTimer >= 0.5f) {
            game->enemies[i].moveTimer = 0.0f;
            
            // Tentar mover na direção atual
            Position newPos = game->enemies[i].pos;
            
            switch (game->enemies[i].dir) {
                case DIR_UP: newPos.y--; break;
                case DIR_DOWN: newPos.y++; break;
                case DIR_LEFT: newPos.x--; break;
                case DIR_RIGHT: newPos.x++; break;
            }
            
            if (IsValidPosition(game, newPos)) {
                // Verificar se não há outro inimigo na posição
                bool occupied = false;
                for (int j = 0; j < game->enemyCount; j++) {
                    if (j != i && CheckCollision(newPos, game->enemies[j].pos)) {
                        occupied = true;
                        break;
                    }
                }
                
                if (!occupied) {
                    game->enemies[i].pos = newPos;
                } else {
                    game->enemies[i].dir = rand() % 4;
                }
            } else {
                game->enemies[i].dir = rand() % 4;
            }
        }
    }
}

void SaveGame(Game *game) {
    FILE *file = fopen("savegame.dat", "wb");
    if (file) {
        fwrite(game, sizeof(Game), 1, file);
        
        // Salvar mapa
        for (int i = 0; i < MAP_HEIGHT; i++) {
            fwrite(game->map.grid[i], sizeof(char), MAP_WIDTH, file);
        }
        
        fclose(file);
    }
}

void LoadGame(Game *game) {
    FILE *file = fopen("savegame.dat", "rb");
    if (file) {
        fread(game, sizeof(Game), 1, file);
        
        // Carregar mapa
        for (int i = 0; i < MAP_HEIGHT; i++) {
            fread(game->map.grid[i], sizeof(char), MAP_WIDTH, file);
        }
        
        fclose(file);
    }
}

void FreeGame(Game *game) {
    for (int i = 0; i < MAP_HEIGHT; i++) {
        free(game->map.grid[i]);
    }
    free(game->map.grid);
    free(game);
}