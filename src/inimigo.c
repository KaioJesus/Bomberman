
#include "inimigo.h"
#include <stdlib.h>
#include "raylib.h"

// Função para inicializar inimigos no mapa (achar 'E')
void InicializarInimigos(Game *game)
{
    int count = 0; // Contador de inimigos encontrados

    // Percorre o grid do mapa
    for (int i = 0; i < MAPA_ALTURA; i++)
    {
        for (int j = 0; j < MAPA_LARGURA; j++)
        {
            if (game->map.grid[i][j] == 'E' && count < MAX_INIMIGOS) // Verifica se a célula atual é um inimigo ('E') e se ainda cabe mais
            {
                game->enemies[count].pos.x = j; // Define a posição inicial do inimigo
                game->enemies[count].pos.y = i;
                game->enemies[count].dir = (Direcao)(rand() % 4); // Define uma direção inicial aleatória
                game->enemies[count].moveTimer = GetTime();       // Inicializa o timer de movimento com o tempo atual

                game->map.grid[i][j] = ' '; // Limpa a célula do mapa (retira o 'E' depois de inicializar)

                count++; // Incrementa o contador de inimigos
            }
        }
    }

    game->InimigoCount = count; // Atualiza a quantidade total de inimigos no jogo
}

// Função para atualizar movimento dos inimigos
void AtualizaInimigos(Game *game)
{
    for (int i = 0; i < game->InimigoCount; i++) // Percorre todos os inimigos existentes
    {                                            // Atualiza o timer de movimento acumulando o tempo do frame
        game->enemies[i].moveTimer += GetFrameTime();
        // Se já passou o tempo de espera para se mover (0.5 segundos)
        if (game->enemies[i].moveTimer >= 0.5f)
        { // Zera o timer para começar a contar de novo
            game->enemies[i].moveTimer = 0.0f;

            // Copia a posição atual para uma nova posição
            Posicao novaPosicao = game->enemies[i].pos;

            // Calcula a nova posição de acordo com a direção atual
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
            // Verifica se a nova posição é válida (não é parede, nem fora do mapa)
            if (EhValidaPosicao(game, novaPosicao))
            {
                // Verificar se não há outro inimigo na posição
                bool occupied = false;
                for (int j = 0; j < game->InimigoCount; j++)
                {
                    if (j != i && verificaColisao(novaPosicao, game->enemies[j].pos))
                    {
                        occupied = true; // Outra posição ocupada
                        break;
                    }
                }
                // Se não está ocupada, move o inimigo para a nova posição
                if (!occupied)
                {
                    game->enemies[i].pos = novaPosicao;
                }
                else
                {
                    // Se a posição estiver ocupada, escolhe uma nova direção aleatória
                    game->enemies[i].dir = rand() % 4;
                }
            }
            else
            { // Se a posição não for válida, escolhe uma nova direção aleatória
                game->enemies[i].dir = rand() % 4;
            }
        }
    }
}
