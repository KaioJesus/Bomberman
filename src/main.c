#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// Constantes do jogo
#define TELA_LARGURA 1200
#define TELA_ALTURA 600
#define MAPA_LARGURA 60
#define MAPA_ALTURA 25
#define BLOCO_TAMANHO 20
#define GAME_AREA_HEIGHT 500
#define INTERFACE_ALTURA 100
#define MAX_BOMBAS 10
#define MAX_INIMIGOS 10
#define BOMBA_TEMPORIZADOR 3.0f
#define EXPLOSAO_TAMANHO 100

// Enums
typedef enum {
    GAME_PLAYING,
    GAME_MENU,
    GAME_OVER,
    GAME_WIN
} GameState;

typedef enum {
    MENU_NOVO_JOGO,
    MENU_CARREGAR_JOGO,
    MENU_SALVAR_JOGO,
    MENU_SAIR,
    MENU_VOLTAR
} MenuOpcoes;

typedef enum {
    DIR_CIMA,
    DIR_BAIXO,
    DIR_ESQUERDA,
    DIR_DIREITA
} Direcao;

// Estruturas
typedef struct {
    int x, y;
} Posicao;

typedef struct {
    Posicao pos;
    int vidas;
    int pontos;
    int bombaContador;
    int chavesColetadas;
} Jogador;

typedef struct {
    Posicao pos;
    Direcao dir;
    float moveTimer;
} Inimigo;

typedef struct {
    Posicao pos;
    float timer;
    bool ativo;
} Bomba;

typedef struct {
    char **grid;
    int levelAtual;
    int totalChaves;
} GameMapa;

typedef struct {
    Jogador jogador;
    Inimigo enemies[MAX_INIMIGOS];
    int InimigoCount;
    Bomba bombas[MAX_BOMBAS];
    int bombaContador;
    GameMapa map;
    GameState state;
    MenuOpcoes menuSelecionado;
    float gameTime;
} Game;

// Protótipos das funções
Game* IniciaJogo(void);
void CarregaMapa(Game *game, int level);
void AtualizaJogo(Game *game);
void DesenhaJogo(Game *game);
void DesenhaMenu(Game *game);
void DesenhaTelaJogoPerdido(Game *game);
void DesenhaTelaVitoria(Game *game);
void HandleInput(Game *game);
void PlantaBomba(Game *game);
void AtualizaBomba(Game *game);
void AtualizaInimigos(Game *game);
void ExplodeBomba(Game *game, int bombIndex);
bool verificaColisao(Posicao pos1, Posicao pos2);
void MoveJogador(Game *game, Direcao dir);
void MoveInimigo(Game *game, int InimigoIndex, Direcao dir);
bool EhValidaPosicao(Game *game, Posicao pos);
void SalvarJogo(Game *game);
void CarregarJogo(Game *game);
void FreeGame(Game *game);

// Função principal
int main(void) {
    InitWindow(TELA_LARGURA, TELA_ALTURA, "Bomberman - Programação II");
    SetTargetFPS(60);
    
    srand(time(NULL));
    
    Game *game = IniciaJogo();
    CarregaMapa(game, 1);
    
    while (!WindowShouldClose()) {
        HandleInput(game);
        
        if (game->state == GAME_PLAYING) {
            AtualizaJogo(game);
        }
        
        BeginDrawing();
        ClearBackground(SKYBLUE);
        
        if (game->state == GAME_PLAYING) {
            DesenhaJogo(game);
        } else if (game->state == GAME_MENU) {
            DesenhaMenu(game);
        } else if (game->state == GAME_OVER) { 
            DesenhaTelaJogoPerdido(game); 
        } else if (game->state == GAME_WIN) { 
            DesenhaTelaVitoria(game); 
        }
        
        EndDrawing();
    }
    
    FreeGame(game);
    CloseWindow();
    
    return 0;
}

// Inicialização do jogo
Game* IniciaJogo(void) {
    Game *game = (Game*)malloc(sizeof(Game));
    
    // Inicializar jogador
    game->jogador.pos.x = 0;
    game->jogador.pos.y = 0;
    game->jogador.vidas = 3;
    game->jogador.pontos = 0;
    game->jogador.bombaContador = 3;
    game->jogador.chavesColetadas = 0;
    
    // Inicializar estado
    game->state = GAME_PLAYING;
    game->menuSelecionado = MENU_NOVO_JOGO;
    game->gameTime = 0.0f;
    game->InimigoCount = 0;
    game->bombaContador = 0;
    
    // Inicializar mapa
    game->map.grid = (char**)malloc(MAPA_ALTURA * sizeof(char*));
    for (int i = 0; i < MAPA_ALTURA; i++) {
        game->map.grid[i] = (char*)malloc(MAPA_LARGURA * sizeof(char));
    }
    game->map.levelAtual = 1;
    game->map.totalChaves = 5;
    
    // Inicializar bombas
    for (int i = 0; i < MAX_BOMBAS; i++) {
        game->bombas[i].ativo = false;
    }
    
    return game;
}

// Carregamento do mapa
void CarregaMapa(Game *game, int level) {
    char filename[50];
    sprintf(filename, "mapa%d.txt", level);
    
    FILE *file = fopen(filename, "r");
    if (!file) {
        return;
    } 
    game->InimigoCount = 0;
    for (int y = 0; y < MAPA_ALTURA; y++) {
        for (int x = 0; x < MAPA_LARGURA; x++) {
            char c = fgetc(file);
            if (c == '\n') c = fgetc(file);
            
            game->map.grid[y][x] = c;
            
            if (c == 'J') {
                game->jogador.pos.x = x;
                game->jogador.pos.y = y;
                game->map.grid[y][x] = ' ';
            } else if (c == 'E' && game->InimigoCount < MAX_INIMIGOS) {
                game->enemies[game->InimigoCount].pos.x = x;
                game->enemies[game->InimigoCount].pos.y = y;
                game->enemies[game->InimigoCount].dir = rand() % 4;
                game->enemies[game->InimigoCount].moveTimer = 0.0f;
                game->InimigoCount++;
                game->map.grid[y][x] = ' ';
            }
        }
    }
    fclose(file);
    
    game->map.levelAtual = level;
}

// Atualização do jogo
void AtualizaJogo(Game *game) {
    game->gameTime += GetFrameTime();
    
    AtualizaBomba(game);
    AtualizaInimigos(game);
    
    // Verificar colisão com inimigos
    for (int i = 0; i < game->InimigoCount; i++) {
        if (verificaColisao(game->jogador.pos, game->enemies[i].pos)) {
            game->jogador.vidas--;
            game->jogador.pontos = (game->jogador.pontos > 100) ? game->jogador.pontos - 100 : 0;
            
            if (game->jogador.vidas <= 0) {
                game->state = GAME_OVER;
            }
            
            // Reposicionar jogador
            game->jogador.pos.x = 1;
            game->jogador.pos.y = 1;
        }
    }
    
    // Verificar vitória
    if (game->jogador.chavesColetadas >= game->map.totalChaves) {
        game->map.levelAtual++;
        game->jogador.chavesColetadas = 0;
        CarregaMapa(game, game->map.levelAtual);
    }
}

// Desenho do jogo
void DesenhaJogo(Game *game) {
    // Desenhar mapa
    for (int y = 0; y < MAPA_ALTURA; y++) {
        for (int x = 0; x < MAPA_LARGURA; x++) {
            Rectangle rect = {x * BLOCO_TAMANHO, y * BLOCO_TAMANHO, BLOCO_TAMANHO, BLOCO_TAMANHO};
            
            switch (game->map.grid[y][x]) {
                case 'W':
                    // DrawRectangle(posicao eixo horizontal, posicao eixo vertical, largura, altura, cor) 
                    // DrawRectangleRec(Rectangle rect, cor) 
                    //Desenha um retangulo na posicao informada
                    DrawRectangleRec(rect, GRAY);
                    break;
                case 'D':
                    DrawRectangleRec(rect, BROWN);
                    break;
                case 'B':
                    DrawRectangleRec(rect, ORANGE);
                    break;
                case 'K':
                    DrawRectangleRec(rect, GOLD);
                    break;
            }
        }
    }
    
    // Desenhar jogador
    Rectangle jogadorRect = {
        game->jogador.pos.x * BLOCO_TAMANHO, 
        game->jogador.pos.y * BLOCO_TAMANHO, 
        BLOCO_TAMANHO, 
        BLOCO_TAMANHO
    };
    DrawRectangleRec(jogadorRect, BLUE);
    
    // Desenhar inimigos
    for (int i = 0; i < game->InimigoCount; i++) {
        Rectangle InimigoRect = {
            game->enemies[i].pos.x * BLOCO_TAMANHO,
            game->enemies[i].pos.y * BLOCO_TAMANHO,
            BLOCO_TAMANHO,
            BLOCO_TAMANHO
        };
        DrawRectangleRec(InimigoRect, RED);
    }
    
    // Desenhar bombas
    for (int i = 0; i < MAX_BOMBAS; i++) {
        if (game->bombas[i].ativo) {
            Rectangle bombRect = {
                game->bombas[i].pos.x * BLOCO_TAMANHO,
                game->bombas[i].pos.y * BLOCO_TAMANHO,
                BLOCO_TAMANHO,
                BLOCO_TAMANHO
            };
            DrawRectangleRec(bombRect, BLACK);
            DrawRectangleLinesEx(bombRect, 2, WHITE);
        }
    }
    

    // Desenha o retangulo onde as informacoes sobre quantidade de bomba, vidas e pontuacao ficarao
    DrawRectangle(0, GAME_AREA_HEIGHT, TELA_LARGURA, INTERFACE_ALTURA, SKYBLUE);
    
    char informacoesJogo[200];
    sprintf(informacoesJogo, "Bombas: %d | Vidas: %d  | Pontos: %d" , 
            game->jogador.bombaContador, game->jogador.vidas, game->jogador.pontos);
    
    // DrawText(texto, posicao eixo horizontal, posicao eixo vertical, tamanho da fonte, cor)

    // Escreve no retangulo reservado as informacoes de quantidade de bomba, vidas e pontuacao ficarao
    DrawText(informacoesJogo, 10, GAME_AREA_HEIGHT + 20, 24, BLACK);

    // Escreve as funcionalidades de jogo
    DrawText("TAB: Menu | WASD/Setas: Mover | B: Bomba", 10, GAME_AREA_HEIGHT + 64, 24, BLACK);
}

// Desenho do menu
void DesenhaMenu(Game *game) {
    DrawRectangle(0, 0, TELA_LARGURA, TELA_ALTURA, BLACK);
    
    char *menuOpcoes[] = {"Novo Jogo (N)", "Carregar Jogo (C)", "Salvar Jogo (S)", "Sair (Q)", "Voltar (V)"};
    char *menuOpcoesPerderOuGanhar[] = {"Novo Jogo (N)", "Sair (Q)"}; // Apenas Novo Jogo e Sair
    int contadorMenu = 5;

    char **menuOpcoesAtual;

    bool jogoTerminou = (game->jogador.vidas <= 0);

   if (jogoTerminou) {
        menuOpcoesAtual = menuOpcoesPerderOuGanhar;
        contadorMenu = 2; // Apenas Novo Jogo e Sair
        // Garante que o item selecionado seja uma opção válida no menu reduzido
        if (game->menuSelecionado != MENU_NOVO_JOGO && game->menuSelecionado != MENU_SAIR) {
            game->menuSelecionado = MENU_NOVO_JOGO; // Volta para Novo Jogo se for uma opção inválida
        }
    } else { // Caso contrário (se entrou no menu a partir de GAME_PLAYING)
        menuOpcoesAtual = menuOpcoes;
        contadorMenu = 5;
    }
    
    int posicaoVerticalMenu = TELA_ALTURA / 2 - (contadorMenu * 30) / 2;
    int posicaoHorizontalMenu = TELA_LARGURA / 2;
    
    // Escreve na tela a palava meno na posição informada
    DrawText("MENU", posicaoHorizontalMenu - 50, posicaoVerticalMenu - 50 , 40, WHITE);
    
    for (int i = 0; i < contadorMenu; i++) {
        Color cor = (i == game->menuSelecionado) ? YELLOW : WHITE;
        DrawText(menuOpcoes[i], posicaoHorizontalMenu - 100, posicaoVerticalMenu + i * 40, 20, cor);
    }
}

void DesenhaTelaJogoPerdido(Game *game) {
    DrawRectangle(0, 0, TELA_LARGURA, TELA_ALTURA, (Color){0, 0, 0, 100});
    DrawText("GAME OVER", TELA_LARGURA / 2 - MeasureText("GAME OVER", 60) / 2, TELA_ALTURA / 2 - 50, 60, RED);
    DrawText("Pressione ENTER para voltar ao Menu", TELA_LARGURA / 2 - MeasureText("Pressione ENTER para voltar ao Menu", 20) / 2, TELA_ALTURA / 2 + 20, 20, WHITE);
}

void DesenhaTelaVitoria(Game *game) {
    DrawRectangle(0, 0, TELA_LARGURA, TELA_ALTURA, (Color){0, 0, 0, 200});
    DrawText("VOCÊ VENCEU!", TELA_LARGURA / 2 - MeasureText("VOCÊ VENCEU!", 60) / 2, TELA_ALTURA / 2 - 50, 60, GREEN);
    DrawText("Pressione ENTER para voltar ao Menu", TELA_LARGURA / 2 - MeasureText("Pressione ENTER para voltar ao Menu", 20) / 2, TELA_ALTURA / 2 + 20, 20, WHITE);
}

// Tratamento de entrada
void HandleInput(Game *game) {
    if (game->state == GAME_MENU) {
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
            game->menuSelecionado = (game->menuSelecionado - 1 + 5) % 5;
        } else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
            game->menuSelecionado = (game->menuSelecionado + 1) % 5;
        } else if (IsKeyPressed(KEY_ENTER)) {
            switch (game->menuSelecionado) {
                case MENU_NOVO_JOGO:
                    // Resetar jogo
                    game->jogador.vidas = 3;
                    game->jogador.pontos = 0;
                    game->jogador.bombaContador = 3;
                    game->jogador.chavesColetadas = 0;
                    CarregaMapa(game, 1);
                    game->state = GAME_PLAYING;
                    break;
                case MENU_CARREGAR_JOGO:
                    CarregarJogo(game);
                    game->state = GAME_PLAYING;
                    break;
                case MENU_SALVAR_JOGO:
                    SalvarJogo(game);
                    game->state = GAME_PLAYING;
                    break;
                case MENU_SAIR:
                    exit(0);
                    break;
                case MENU_VOLTAR:
                    game->state = GAME_PLAYING;
                    break;
            }
        } else if (IsKeyPressed(KEY_N)) {
            game->menuSelecionado = MENU_NOVO_JOGO;
        } else if (IsKeyPressed(KEY_C)) {
            game->menuSelecionado = MENU_CARREGAR_JOGO;
        } else if (IsKeyPressed(KEY_S)) {
            game->menuSelecionado = MENU_SALVAR_JOGO;
        } else if (IsKeyPressed(KEY_Q)) {
            game->menuSelecionado = MENU_SAIR;
        } else if (IsKeyPressed(KEY_V)) {
            game->menuSelecionado = MENU_VOLTAR;
        }
    } else if (game->state == GAME_PLAYING) {
        if (IsKeyPressed(KEY_TAB)) {
            game->state = GAME_MENU;
        } else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
            MoveJogador(game, DIR_CIMA);
        } else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
            MoveJogador(game, DIR_BAIXO);
        } else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
            MoveJogador(game, DIR_ESQUERDA);
        } else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
            MoveJogador(game, DIR_DIREITA);
        } else if (IsKeyPressed(KEY_B)) {
            PlantaBomba(game);
        }
    }  else if (game->state == GAME_OVER || game->state == GAME_WIN) {
        if (IsKeyPressed(KEY_ENTER)) {
            game->state = GAME_MENU;
            game->menuSelecionado = MENU_NOVO_JOGO; // Seleciona "Novo Jogo" por padrão no menu
        }
    }
}

// Implementações das funções auxiliares
bool verificaColisao(Posicao pos1, Posicao pos2) {
    return (pos1.x == pos2.x && pos1.y == pos2.y);
}

bool EhValidaPosicao(Game *game, Posicao pos) {
    if (pos.x < 0 || pos.x >= MAPA_LARGURA || pos.y < 0 || pos.y >= MAPA_ALTURA) {
        return false;
    }
    
    char tile = game->map.grid[pos.y][pos.x];
    return (tile == ' ' || tile == 'K');
}

void MoveJogador(Game *game, Direcao dir) {
    Posicao newPos = game->jogador.pos;
    
    switch (dir) {
        case DIR_CIMA: newPos.y--; break;
        case DIR_BAIXO: newPos.y++; break;
        case DIR_ESQUERDA: newPos.x--; break;
        case DIR_DIREITA: newPos.x++; break;
    }
    
    if (EhValidaPosicao(game, newPos)) {
        // Verificar se é uma chave
        if (game->map.grid[newPos.y][newPos.x] == 'K') {
            game->jogador.chavesColetadas++;
            game->map.grid[newPos.y][newPos.x] = ' ';
        }
        
        game->jogador.pos = newPos;
    }
}

void PlantaBomba(Game *game) {
    if (game->jogador.bombaContador <= 0) return;
    
    // Encontrar slot vazio para bomba
    for (int i = 0; i < MAX_BOMBAS; i++) {
        if (!game->bombas[i].ativo) {
            game->bombas[i].pos = game->jogador.pos;
            game->bombas[i].timer = BOMBA_TEMPORIZADOR;
            game->bombas[i].ativo = true;
            game->jogador.bombaContador--;
            break;
        }
    }
}

void AtualizaBomba(Game *game) {
    for (int i = 0; i < MAX_BOMBAS; i++) {
        if (game->bombas[i].ativo) {
            game->bombas[i].timer -= GetFrameTime();
            if (game->bombas[i].timer <= 0) {
                ExplodeBomba(game, i);
            }
        }
    }
}

void ExplodeBomba(Game *game, int bombIndex) {
    Posicao bombPos = game->bombas[bombIndex].pos;
    game->bombas[bombIndex].ativo = false;
    game->jogador.bombaContador++;
    
    // Verificar dano ao jogador
    float distTojogador = sqrt(pow(game->jogador.pos.x - bombPos.x, 2) + pow(game->jogador.pos.y - bombPos.y, 2));
    if (distTojogador * BLOCO_TAMANHO <= EXPLOSAO_TAMANHO / 2) {
        game->jogador.vidas--;
        game->jogador.pontos = (game->jogador.pontos > 100) ? game->jogador.pontos - 100 : 0;
    }

    if (game->jogador.vidas <= 0) {
        game->state = GAME_OVER;
    }
    
    // Destruir elementos em cruz
    int Direcaos[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    
    for (int d = 0; d < 4; d++) {
        for (int alcance = 1; alcance <= 2; alcance++) {
            int x = bombPos.x + Direcaos[d][0] * alcance;
            int y = bombPos.y + Direcaos[d][1] * alcance;
            
            if (x < 0 || x >= MAPA_LARGURA || y < 0 || y >= MAPA_ALTURA) break;
            
            char bloco = game->map.grid[y][x];
            
            if (bloco == 'W') break; // Parede indestrutível
            
            if (bloco == 'D' || bloco == 'B') {
                game->map.grid[y][x] = ' ';
                game->jogador.pontos += 10;
                break;
            }
            
            if (bloco == 'K') {
                // Chave revelada
                game->jogador.pontos += 10;
                break;
            }
        }
    }
    
    // Verificar inimigos na explosão
    for (int i = 0; i < game->InimigoCount; i++) {
        float distToInimigo = sqrt(pow(game->enemies[i].pos.x - bombPos.x, 2) + pow(game->enemies[i].pos.y - bombPos.y, 2));
        if (distToInimigo * BLOCO_TAMANHO <= EXPLOSAO_TAMANHO / 2) {
            // Remover inimigo
            for (int j = i; j < game->InimigoCount - 1; j++) {
                game->enemies[j] = game->enemies[j + 1];
            }
            game->jogador.vidas--;
            game->jogador.pontos += 20;
            i--;
        }
    }
}

void AtualizaInimigos(Game *game) {
    for (int i = 0; i < game->InimigoCount; i++) {
        game->enemies[i].moveTimer += GetFrameTime();
        
        if (game->enemies[i].moveTimer >= 0.5f) {
            game->enemies[i].moveTimer = 0.0f;
            
            // Tentar mover na direcao atual
            Posicao novaPosicao = game->enemies[i].pos;
            
            switch (game->enemies[i].dir) {
                case DIR_CIMA: novaPosicao.y--; break;
                case DIR_BAIXO: novaPosicao.y++; break;
                case DIR_ESQUERDA: novaPosicao.x--; break;
                case DIR_DIREITA: novaPosicao.x++; break;
            }
            
            if (EhValidaPosicao(game, novaPosicao)) {
                // Verificar se não há outro inimigo na posição
                bool occupied = false;
                for (int j = 0; j < game->InimigoCount; j++) {
                    if (j != i && verificaColisao(novaPosicao, game->enemies[j].pos)) {
                        occupied = true;
                        break;
                    }
                }
                
                if (!occupied) {
                    game->enemies[i].pos = novaPosicao;
                } else {
                    game->enemies[i].dir = rand() % 4;
                }
            } else {
                game->enemies[i].dir = rand() % 4;
            }
        }
    }
}

void SalvarJogo(Game *game) {
    FILE *file = fopen("SalvarJogo.dat", "wb");
    if (file) {
        fwrite(game, sizeof(Game), 1, file);
        
        // Salvar mapa
        for (int i = 0; i < MAPA_ALTURA; i++) {
            fwrite(game->map.grid[i], sizeof(char), MAPA_LARGURA, file);
        }
        
        fclose(file);
    }
}

void CarregarJogo(Game *game) {
    FILE *file = fopen("SalvarJogo.dat", "rb");
    if (file) {
        fread(game, sizeof(Game), 1, file);
        
        // Carregar mapa
        for (int i = 0; i < MAPA_ALTURA; i++) {
            fread(game->map.grid[i], sizeof(char), MAPA_LARGURA, file);
        }
        
        fclose(file);
    }
}

void FreeGame(Game *game) {
    for (int i = 0; i < MAPA_ALTURA; i++) {
        free(game->map.grid[i]);
    }
    free(game->map.grid);
    free(game);
}