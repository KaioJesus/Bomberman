#include <stdio.h>
#include <stdlib.h>
#include "game.h"

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