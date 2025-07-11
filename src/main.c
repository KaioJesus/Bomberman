#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "inimigo.h"
#include "map.h"
#include "bomb.h"
#include "state_game.h"
#include "menu.h"
#include "game.h"
#include "jogador.h"

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
