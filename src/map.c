#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void IniciaMapa(GameMapa *mapa) {
    mapa->grid = (char**)malloc(MAPA_ALTURA * sizeof(char*));
    for (int i = 0; i < MAPA_ALTURA; i++) {
        mapa->grid[i] = (char*)malloc(MAPA_LARGURA * sizeof(char));
    }
    mapa->levelAtual = 1;
    mapa->totalChaves = 5;
}

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

bool EhValidaPosicao(Game *game, Posicao pos) {
    if (pos.x < 0 || pos.x >= MAPA_LARGURA || pos.y < 0 || pos.y >= MAPA_ALTURA) {
        return false;
    }
    
    char tile = game->map.grid[pos.y][pos.x];
    return (tile == ' ' || tile == 'K');
}

void DesenhaMapa(Game *game) {
    for (int y = 0; y < MAPA_ALTURA; y++) {
        for (int x = 0; x < MAPA_LARGURA; x++) {
            Rectangle rect = {x * BLOCO_TAMANHO, y * BLOCO_TAMANHO, BLOCO_TAMANHO, BLOCO_TAMANHO};
            
            switch (game->map.grid[y][x]) {
                case 'W':
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
}

void FreeMapa(GameMapa *map) {
    for (int i = 0; i < MAPA_ALTURA; i++) {
        free(map->grid[i]);
    }
    free(map->grid);
    free(map);
}
