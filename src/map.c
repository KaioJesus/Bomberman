#include "map.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Função responsável por carregar um mapa específico do jogo
void CarregaMapa(Game *game, int level)
{
    // Cria o nome do arquivo baseado no nível (ex: "mapas/mapa1.txt")
    char filename[50];
    sprintf(filename, "mapas/mapa%d.txt", level);

    // Tenta abrir o arquivo do mapa em modo de leitura
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        // Se não conseguir abrir o arquivo, encerra a função
        return;
    }
    // Zera o contador de inimigos antes de carregar o novo mapa
    game->InimigoCount = 0;

    // Loop duplo para percorrer todas as posições do mapa
    for (int y = 0; y < MAPA_ALTURA; y++)
    {
        for (int x = 0; x < MAPA_LARGURA; x++)
        {
            // Lê um caractere do arquivo
            char c = fgetc(file);
            if (c == '\n')
                c = fgetc(file);

            // Armazena o caractere na posição correspondente do grid
            game->map.grid[y][x] = c;

            // Se o caractere for 'J' (posição inicial do jogador)
            if (c == 'J')
            {

                // Define a posição inicial do jogador
                game->jogador.pos.x = x;
                game->jogador.pos.y = y;

                // Substitui o 'J' por um espaço vazio no mapa
                game->map.grid[y][x] = ' ';
            }
        }
    }

    // Fecha o arquivo após a leitura
    fclose(file);

    // Atualiza o nível atual do mapa
    game->map.levelAtual = level;

    // Inicializa os inimigos do mapa carregado
    InicializarInimigos(game);
}

// Função que verifica se uma posição é válida para movimento
bool EhValidaPosicao(Game *game, Posicao pos)
{
    // Verifica se a posição está dentro dos limites do mapa
    if (pos.x < 0 || pos.x >= MAPA_LARGURA || pos.y < 0 || pos.y >= MAPA_ALTURA)
    {
        return false;
    }

    // Obtém o tipo de bloco na posição
    char tile = game->map.grid[pos.y][pos.x];

    // Retorna true apenas se for espaço vazio (' ')
    // Isso significa que o jogador pode se mover para essas posições
    return (tile == ' ');
}
