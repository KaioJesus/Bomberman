#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "inimigo.h"
<<<<<<< HEAD
#include"bomb.h"
=======
#include "map.h"
#include "bomb.h"
#include "state_game.h"
#include "menu.h"

>>>>>>> f4b3632 (feat: modularizacao)
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

// Protótipos das funções
Game *IniciaJogo(void);
void AtualizaJogo(Game *game);
void DesenhaJogo(Game *game);
void HandleInput(Game *game);
bool verificaColisao(Posicao pos1, Posicao pos2);
void MoveJogador(Game *game, Direcao dir);

// Função principal
int main(void)
{
    InitWindow(TELA_LARGURA, TELA_ALTURA, "Bomberman - Programação II");
    SetTargetFPS(60);

    srand(time(NULL));

    Game *game = IniciaJogo();
    CarregaMapa(game, 1);

    while (!WindowShouldClose())
    {
        HandleInput(game);

        if (game->state == GAME_PLAYING)
        {
            AtualizaJogo(game);
        }

        BeginDrawing();
        ClearBackground(SKYBLUE);

        if (game->state == GAME_PLAYING)
        {
            DesenhaJogo(game);
        }
        else if (game->state == GAME_MENU)
        {
            DesenhaMenu(game);
        }
        else if (game->state == GAME_OVER)
        {
            DesenhaTelaJogoPerdido(game);
        }
        else if (game->state == GAME_WIN)
        {
            DesenhaTelaVitoria(game);
        }

        EndDrawing();
    }

    FreeGame(game);
    CloseWindow();

    return 0;
}

// Inicialização do jogo
Game *IniciaJogo(void)
{
    Game *game = (Game *)malloc(sizeof(Game));

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
    game->map.grid = (char **)malloc(MAPA_ALTURA * sizeof(char *));
    for (int i = 0; i < MAPA_ALTURA; i++)
    {
        game->map.grid[i] = (char *)malloc(MAPA_LARGURA * sizeof(char));
    }
    game->map.levelAtual = 1;
    game->map.totalChaves = 5;

    // Inicializar bombas
    for (int i = 0; i < MAX_BOMBAS; i++)
    {
        game->bombas[i].ativo = false;
    }

    return game;
}

// Atualização do jogo
void AtualizaJogo(Game *game)
{
    game->gameTime += GetFrameTime();

    AtualizaBomba(game);
    AtualizarInimigos(game);

    // Verificar colisão com inimigos
    for (int i = 0; i < game->InimigoCount; i++)
    {
        if (verificaColisao(game->jogador.pos, game->enemies[i].pos))
        {
            game->jogador.vidas--;
            game->jogador.pontos = (game->jogador.pontos > 100) ? game->jogador.pontos - 100 : 0;

            if (game->jogador.vidas <= 0)
            {
                game->state = GAME_OVER;
            }

            // Reposicionar jogador
            game->jogador.pos.x = 1;
            game->jogador.pos.y = 1;
        }
    }

    // Verificar vitória
    if (game->jogador.chavesColetadas >= game->map.totalChaves)
    {
        game->map.levelAtual++;
        game->jogador.chavesColetadas = 0;
        CarregaMapa(game, game->map.levelAtual);
    }
}

// Desenho do jogo
void DesenhaJogo(Game *game)
{
    // Desenhar mapa
    for (int y = 0; y < MAPA_ALTURA; y++)
    {
        for (int x = 0; x < MAPA_LARGURA; x++)
        {
            Rectangle rect = {x * BLOCO_TAMANHO, y * BLOCO_TAMANHO, BLOCO_TAMANHO, BLOCO_TAMANHO};

            switch (game->map.grid[y][x])
            {
            case 'W':
                // DrawRectangle(posicao eixo horizontal, posicao eixo vertical, largura, altura, cor)
                // DrawRectangleRec(Rectangle rect, cor)
                // Desenha um retangulo na posicao informada
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
        BLOCO_TAMANHO};
    DrawRectangleRec(jogadorRect, BLUE);

    // Desenhar inimigos
    for (int i = 0; i < game->InimigoCount; i++)
    {
        Rectangle InimigoRect = {
            game->enemies[i].pos.x * BLOCO_TAMANHO,
            game->enemies[i].pos.y * BLOCO_TAMANHO,
            BLOCO_TAMANHO,
            BLOCO_TAMANHO};
        DrawRectangleRec(InimigoRect, RED);
    }

    // Desenhar bombas
    for (int i = 0; i < MAX_BOMBAS; i++)
    {
        if (game->bombas[i].ativo)
        {
            Rectangle bombRect = {
                game->bombas[i].pos.x * BLOCO_TAMANHO,
                game->bombas[i].pos.y * BLOCO_TAMANHO,
                BLOCO_TAMANHO,
                BLOCO_TAMANHO};
            DrawRectangleRec(bombRect, BLACK);
            DrawRectangleLinesEx(bombRect, 2, WHITE);
        }
    }

    // Desenha o retangulo onde as informacoes sobre quantidade de bomba, vidas e pontuacao ficarao
    DrawRectangle(0, GAME_AREA_HEIGHT, TELA_LARGURA, INTERFACE_ALTURA, SKYBLUE);

    char informacoesJogo[200];
    sprintf(informacoesJogo, "Bombas: %d | Vidas: %d  | Pontos: %d",
            game->jogador.bombaContador, game->jogador.vidas, game->jogador.pontos);

    // DrawText(texto, posicao eixo horizontal, posicao eixo vertical, tamanho da fonte, cor)

    // Escreve no retangulo reservado as informacoes de quantidade de bomba, vidas e pontuacao ficarao
    DrawText(informacoesJogo, 10, GAME_AREA_HEIGHT + 20, 24, BLACK);

    // Escreve as funcionalidades de jogo
    DrawText("TAB: Menu | WASD/Setas: Mover | B: Bomba", 10, GAME_AREA_HEIGHT + 64, 24, BLACK);
}

// Tratamento de entrada
void HandleInput(Game *game)
{
    if (game->state == GAME_MENU)
    {
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        {
            game->menuSelecionado = (game->menuSelecionado - 1 + 5) % 5;
        }
        else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        {
            game->menuSelecionado = (game->menuSelecionado + 1) % 5;
        }
        else if (IsKeyPressed(KEY_ENTER))
        {
            switch (game->menuSelecionado)
            {
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
        }
        else if (IsKeyPressed(KEY_N))
        {
            game->menuSelecionado = MENU_NOVO_JOGO;
        }
        else if (IsKeyPressed(KEY_C))
        {
            game->menuSelecionado = MENU_CARREGAR_JOGO;
        }
        else if (IsKeyPressed(KEY_S))
        {
            game->menuSelecionado = MENU_SALVAR_JOGO;
        }
        else if (IsKeyPressed(KEY_Q))
        {
            game->menuSelecionado = MENU_SAIR;
        }
        else if (IsKeyPressed(KEY_V))
        {
            game->menuSelecionado = MENU_VOLTAR;
        }
    }
    else if (game->state == GAME_PLAYING)
    {
        if (IsKeyPressed(KEY_TAB))
        {
            game->state = GAME_MENU;
        }
        else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        {
            MoveJogador(game, DIR_CIMA);
        }
        else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        {
            MoveJogador(game, DIR_BAIXO);
        }
        else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
        {
            MoveJogador(game, DIR_ESQUERDA);
        }
        else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
        {
            MoveJogador(game, DIR_DIREITA);
        }
        else if (IsKeyPressed(KEY_B))
        {
            PlantaBomba(game);
        }
    }
    else if (game->state == GAME_OVER || game->state == GAME_WIN)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            game->state = GAME_MENU;
            game->menuSelecionado = MENU_NOVO_JOGO; // Seleciona "Novo Jogo" por padrão no menu
        }
    }
}

// Implementações das funções auxiliares
bool verificaColisao(Posicao pos1, Posicao pos2)
{
    return (pos1.x == pos2.x && pos1.y == pos2.y);
}

bool EhValidaPosicao(Game *game, Posicao pos)
{
    if (pos.x < 0 || pos.x >= MAPA_LARGURA || pos.y < 0 || pos.y >= MAPA_ALTURA)
    {
        return false;
    }

    char tile = game->map.grid[pos.y][pos.x];
    return (tile == ' ' || tile == 'K');
}

void MoveJogador(Game *game, Direcao dir)
{
    Posicao newPos = game->jogador.pos;

    switch (dir)
    {
    case DIR_CIMA:
        newPos.y--;
        break;
    case DIR_BAIXO:
        newPos.y++;
        break;
    case DIR_ESQUERDA:
        newPos.x--;
        break;
    case DIR_DIREITA:
        newPos.x++;
        break;
    }

    if (EhValidaPosicao(game, newPos))
    {
        // Verificar se é uma chave
        if (game->map.grid[newPos.y][newPos.x] == 'K')
        {
            game->jogador.chavesColetadas++;
            game->map.grid[newPos.y][newPos.x] = ' ';
        }

        game->jogador.pos = newPos;
    }
}
