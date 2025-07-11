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
void AtualizaInimigos(Game *game)
{
    for (int i = 0; i < game->InimigoCount; i++)
    {
        game->enemies[i].moveTimer += GetFrameTime();

        if (game->enemies[i].moveTimer >= 0.5f)
        {
            game->enemies[i].moveTimer = 0.0f;

            // Tentar mover na direcao atual
            Posicao novaPosicao = game->enemies[i].pos;

            switch (game->enemies[i].dir)
            {
            case DIR_CIMA:
                novaPosicao.y--;
                break;
            case DIR_BAIXO:
                novaPosicao.y++;
                break;
            case DIR_ESQUERDA:
                novaPosicao.x--;
                break;
            case DIR_DIREITA:
                novaPosicao.x++;
                break;
            }

            if (EhValidaPosicao(game, novaPosicao))
            {
                // Verificar se não há outro inimigo na posição
                bool occupied = false;
                for (int j = 0; j < game->InimigoCount; j++)
                {
                    if (j != i && verificaColisao(novaPosicao, game->enemies[j].pos))
                    {
                        occupied = true;
                        break;
                    }
                }

                if (!occupied)
                {
                    game->enemies[i].pos = novaPosicao;
                }
                else
                {
                    game->enemies[i].dir = rand() % 4;
                }
            }
            else
            {
                game->enemies[i].dir = rand() % 4;
            }
            == == == =
                         void AtualizarInimigos(Game * game)
            {
                float tempoAtual = GetTime();

                for (int i = 0; i < game->InimigoCount; i++)
                {
                    Inimigo *enemy = &game->enemies[i];

                    // Mudar direção a cada 2 segundos
                    if (tempoAtual - enemy->moveTimer > 2.0f)
                    {
                        enemy->dir = (Direcao)(rand() % 4);
                        enemy->moveTimer = tempoAtual;
                    }

                    Posicao novaPos = enemy->pos;
                    switch (enemy->dir)
                    {
                    case DIR_CIMA:
                        novaPos.y--;
                        break;
                    case DIR_BAIXO:
                        novaPos.y++;
                        break;
                    case DIR_ESQUERDA:
                        novaPos.x--;
                        break;
                    case DIR_DIREITA:
                        novaPos.x++;
                        break;
                    }

                    // Verifica se posição é livre
                    if (EhValidaPosicao(game, novaPos))
                    {
                        enemy->pos = novaPos;
                    }
                    else
                    {
                        // Se bater em parede, sorteia nova direção
                        enemy->dir = (Direcao)(rand() % 4);
                        enemy->moveTimer = tempoAtual;
                    }

                    // Verifica colisão com jogador
                    if (enemy->pos.x == game->jogador.pos.x && enemy->pos.y == game->jogador.pos.y)
                    {
                        game->jogador.vidas--;
                        game->jogador.pontos -= 100;
                        if (game->jogador.pontos < 0)
                            game->jogador.pontos = 0;
>>>>>>> f4b3632 (feat: modularizacao)
                    }
                }
            }
