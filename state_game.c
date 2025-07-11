#include <stdio.h>
#include <stdlib.h>
#include "state_game.h"

void SalvarJogo(Game *game)
{
    FILE *file = fopen("SalvarJogo.dat", "wb");
    if (file)
    {
        fwrite(game, sizeof(Game), 1, file);

        // Salvar mapa
        for (int i = 0; i < MAPA_ALTURA; i++)
        {
            fwrite(game->map.grid[i], sizeof(char), MAPA_LARGURA, file);
        }

        fclose(file);
    }
}

void CarregarJogo(Game *game)
{
    FILE *file = fopen("SalvarJogo.dat", "rb");
    if (file)
    {
        fread(game, sizeof(Game), 1, file);

        // Carregar mapa
        for (int i = 0; i < MAPA_ALTURA; i++)
        {
            fread(game->map.grid[i], sizeof(char), MAPA_LARGURA, file);
        }

        fclose(file);
    }
}