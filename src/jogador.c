#include "jogador.h"

// Função para verificar colisão entre duas posições
bool verificaColisao(Posicao pos1, Posicao pos2)
{
    // Retorna true se as posições forem iguais (colisão)
    return (pos1.x == pos2.x && pos1.y == pos2.y);
}

// Função para mover o jogador em uma direção
void MoveJogador(Game *game, Direcao dir)
{
    // Cria uma nova posição inicializada com a posição atual do jogador
    Posicao newPos = game->jogador.pos;

    // Atualiza a nova posição de acordo com a direção escolhida
    switch (dir)
    {
    case DIR_CIMA:
        newPos.y--; // Move para cima (diminui y)
        break;
    case DIR_BAIXO:
        newPos.y++; // Move para baixo (aumenta y)
        break;
    case DIR_ESQUERDA:
        newPos.x--; // Move para esquerda (diminui x)
        break;
    case DIR_DIREITA:
        newPos.x++; // Move para direita (aumenta x)
        break;
    }

    // Verifica se a nova posição é válida (não é parede, nem fora do mapa)
    if (EhValidaPosicao(game, newPos))
    {
        // Verifica se na nova posição existe uma chave
        if (game->map.grid[newPos.y][newPos.x] == 'K')
        {
            // Incrementa o contador de chaves coletadas
            game->jogador.chavesColetadas++;
            // Remove a chave do mapa
            game->map.grid[newPos.y][newPos.x] = ' ';
        }

        // Atualiza a posição do jogador para a nova posição
        game->jogador.pos = newPos;
    }
}
