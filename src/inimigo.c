#include "inimigo.h"
#include <stdlib.h>
#include "raylib.h"

// Função para inicializar inimigos no mapa (achar 'E')
void InicializarInimigos(Game *game)
{
    int count = 0;

    // Percorre o grid do mapa
    for (int i = 0; i < MAPA_ALTURA; i++)
    {
        for (int j = 0; j < MAPA_LARGURA; j++)
        {
            if (game->map.grid[i][j] == 'E' && count < MAX_INIMIGOS)
            {
                game->enemies[count].pos.x = j;
                game->enemies[count].pos.y = i;
                game->enemies[count].dir = (Direcao)(rand() % 4); // Direção aleatória inicial
                game->enemies[count].moveTimer = GetTime();
                count++;
            }
        }
    }

    game->InimigoCount = count;
}

// Função para atualizar movimento dos inimigos
<<<<<<< HEAD
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
