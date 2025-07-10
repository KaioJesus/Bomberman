#include "bomb.h"
#include <math.h>

void PlantaBomba(Game *game) {
    if (game->jogador.bombaContador <= 0) return;

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
    Posicao pos = game->bombas[bombIndex].pos;
    game->bombas[bombIndex].ativo = false;
    game->jogador.bombaContador++;

    if (sqrt(pow(game->jogador.pos.x - pos.x, 2) + pow(game->jogador.pos.y - pos.y, 2)) * BLOCO_TAMANHO <= EXPLOSAO_TAMANHO / 2) {
        game->jogador.vidas--;
        if (game->jogador.pontos > 100) game->jogador.pontos -= 100;
        if (game->jogador.vidas <= 0) game->state = GAME_OVER;
    }

    int dir[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    for (int d = 0; d < 4; d++) {
        for (int a = 1; a <= 2; a++) {
            int x = pos.x + dir[d][0] * a;
            int y = pos.y + dir[d][1] * a;

            if (x < 0 || x >= MAPA_LARGURA || y < 0 || y >= MAPA_ALTURA) break;
            char bloco = game->map.grid[y][x];

            if (bloco == 'W') break;
            if (bloco == 'D' || bloco == 'B') {
                game->map.grid[y][x] = ' ';
                game->jogador.pontos += 10;
                break;
            }
            if (bloco == 'K') {
                game->jogador.pontos += 10;
                break;
            }
        }
    }

    for (int i = 0; i < game->InimigoCount; i++) {
        float dist = sqrt(pow(game->enemies[i].pos.x - pos.x, 2) + pow(game->enemies[i].pos.y - pos.y, 2));
        if (dist * BLOCO_TAMANHO <= EXPLOSAO_TAMANHO / 2) {
            for (int j = i; j < game->InimigoCount - 1; j++) {
                game->enemies[j] = game->enemies[j + 1];
            }
            game->jogador.pontos += 20;
            i--;
            game->InimigoCount--;
        }
    }
}

void DesenhaBombas(Game *game) {
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
}
