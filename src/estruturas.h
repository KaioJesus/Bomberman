#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#include <raylib.h>  // Biblioteca para gráficos 2D/3D e jogos
#include <stdbool.h> // Biblioteca para tipo booleano (true/false)

// ================================
// CONSTANTES DO JOGO
// ================================

// Dimensões da tela do jogo
#define TELA_LARGURA 1200 // Largura da janela em pixels
#define TELA_ALTURA 600   // Altura da janela em pixels

// Dimensões do mapa do jogo
#define MAPA_LARGURA 60  // Largura do mapa em células/blocos
#define MAPA_ALTURA 25   // Altura do mapa em células/blocos
#define BLOCO_TAMANHO 20 // Tamanho de cada bloco/célula em pixels

// Áreas da interface
#define GAME_AREA_HEIGHT 500 // Altura da área de jogo (onde acontece a ação)
#define INTERFACE_ALTURA 100 // Altura da interface (pontos, vidas, etc.)

// Limites do jogo
#define MAX_BOMBAS 10   // Máximo de bombas que podem existir simultaneamente
#define MAX_INIMIGOS 10 // Máximo de inimigos no jogo

// Configurações de gameplay
#define BOMBA_TEMPORIZADOR 3.0f // Tempo em segundos para bomba explodir
#define EXPLOSAO_TAMANHO 100    // Tamanho da explosão em pixels

// ================================
// ENUMERAÇÕES (TIPOS DE DADOS)
// ================================

// Estados possíveis do jogo
typedef enum
{
    GAME_PLAYING, // Jogando normalmente
    GAME_MENU,    // No menu principal
    GAME_OVER,    // Game over (perdeu)
    GAME_WIN      // Ganhou o jogo
} GameState;

// Opções do menu
typedef enum
{
    MENU_NOVO_JOGO,     // Começar novo jogo
    MENU_CARREGAR_JOGO, // Carregar jogo salvo
    MENU_SALVAR_JOGO,   // Salvar jogo atual
    MENU_SAIR,          // Sair do jogo
    MENU_VOLTAR         // Voltar ao jogo
} MenuOpcoes;

// Direções de movimento
typedef enum
{
    DIR_CIMA,     // Movimento para cima
    DIR_BAIXO,    // Movimento para baixo
    DIR_ESQUERDA, // Movimento para esquerda
    DIR_DIREITA   // Movimento para direita
} Direcao;

// ================================
// ESTRUTURAS DE DADOS
// ================================

// Posição simples (coordenadas x, y)
typedef struct
{
    int x, y; // Coordenadas x e y no mapa
} Posicao;

// Estrutura do jogador
typedef struct
{
    Posicao pos;         // Posição atual do jogador no mapa
    int vidas;           // Número de vidas restantes
    int pontos;          // Pontuação atual
    int bombaContador;   // Quantas bombas o jogador pode colocar
    int chavesColetadas; // Número de chaves coletadas
} Jogador;

// Estrutura dos inimigos
typedef struct
{
    Posicao pos;     // Posição atual do inimigo
    Direcao dir;     // Direção atual do movimento
    float moveTimer; // Timer para controlar velocidade do movimento
} Inimigo;

// Estrutura das bombas
typedef struct
{
    Posicao pos; // Posição onde a bomba foi colocada
    float timer; // Tempo restante até explodir
    bool ativo;  // Se a bomba está ativa (true) ou não (false)
} Bomba;

// Estrutura do mapa do jogo
typedef struct
{
    char **grid;     // Matriz 2D representando o mapa
                     // Cada char representa um tipo de bloco:
                     // 'W' = parede indestrutível
                     // 'D' = bloco destrutível
                     // ' ' = espaço vazio
                     // 'K' = Bloco com Chave
                     // 'B' = Bloco sem Chave
    int levelAtual;  // Nível atual do jogo
    int totalChaves; // Total de chaves no nível atual
} GameMapa;

// Estrutura principal do jogo (contém tudo)
typedef struct
{
    Jogador jogador;               // Dados do jogador
    Inimigo enemies[MAX_INIMIGOS]; // Array de inimigos
    int InimigoCount;              // Número atual de inimigos ativos
    Bomba bombas[MAX_BOMBAS];      // Array de bombas
    int bombaContador;             // Número atual de bombas ativas
    GameMapa map;                  // Mapa do jogo
    GameState state;               // Estado atual do jogo
    MenuOpcoes menuSelecionado;    // Opção selecionada no menu
    float gameTime;                // Tempo total de jogo
} Game;

#endif