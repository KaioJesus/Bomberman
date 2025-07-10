#include "jogador.h"

// Implementações das funções auxiliares
bool verificaColisao(Posicao pos1, Posicao pos2)
{
    return (pos1.x == pos2.x && pos1.y == pos2.y);
}

void MoveJogador(Game *game, Direcao dir)
{
    Posicao newPos = game->jogador.pos;

    switch (dir)
    {
    case DIR_CIMA:
        newPos.y--;
        break;
    case DIR_BAIXO:
        newPos.y++;
        break;
    case DIR_ESQUERDA:
        newPos.x--;
        break;
    case DIR_DIREITA:
        newPos.x++;
        break;
    }

    if (EhValidaPosicao(game, newPos))
    {
        // Verificar se é uma chave
        if (game->map.grid[newPos.y][newPos.x] == 'K')
        {
            game->jogador.chavesColetadas++;
            game->map.grid[newPos.y][newPos.x] = ' ';
        }

        game->jogador.pos = newPos;
    }
}