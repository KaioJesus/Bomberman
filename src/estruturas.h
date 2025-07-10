#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <raylib.h>
#include <stdbool.h>

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
typedef enum
{
    GAME_PLAYING,
    GAME_MENU,
    GAME_OVER,
    GAME_WIN
} GameState;

typedef enum
{
    MENU_NOVO_JOGO,
    MENU_CARREGAR_JOGO,
    MENU_SALVAR_JOGO,
    MENU_SAIR,
    MENU_VOLTAR
} MenuOpcoes;

typedef enum
{
    DIR_CIMA,
    DIR_BAIXO,
    DIR_ESQUERDA,
    DIR_DIREITA
} Direcao;

// Estruturas básicas
typedef struct
{
    int x, y;
} Posicao;

typedef struct
{
    Posicao pos;
    int vidas;
    int pontos;
    int bombaContador;
    int chavesColetadas;
} Jogador;

typedef struct
{
    Posicao pos;
    Direcao dir;
    float moveTimer;
} Inimigo;

typedef struct
{
    Posicao pos;
    float timer;
    bool ativo;
} Bomba;

typedef struct
{
    char **grid;
    int levelAtual;
    int totalChaves;
} GameMapa;

typedef struct
{
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

#endif