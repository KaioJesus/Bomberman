#include "jogador.h"
#include "raylib.h"
#include "mapa.h" 

bool verificaColisao(Posicao pos1, Posicao pos2) {
    return (pos1.x == pos2.x && pos1.y == pos2.y);
}

void inicializa_jogador(Jogador *j, int x, int y) {
    j->pos.x = x;
    j->pos.y = y;
    j->vidas = 3;
    j->pontos = 0;
    j->bombaContador = 3;
    j->chavesColetadas = 0;
}

void MoveJogador(Game *game, Direcao dir) {
    Posicao newPos = game->jogador.pos;

    switch (dir) {
        case DIR_CIMA: newPos.y--; break;
        case DIR_BAIXO: newPos.y++; break;
        case DIR_ESQUERDA: newPos.x--; break;
        case DIR_DIREITA: newPos.x++; break;
    }

    if (EhValidaPosicao(game, newPos)) {
        if (game->map.grid[newPos.y][newPos.x] == 'K') {
            game->jogador.chavesColetadas++;
            game->map.grid[newPos.y][newPos.x] = ' ';
        }

        game->jogador.pos = newPos;
    }
}

void desenhar_jogador(Jogador j) {
    DrawRectangle(j.pos.x * BLOCO_TAMANHO, j.pos.y * BLOCO_TAMANHO, BLOCO_TAMANHO, BLOCO_TAMANHO, BLUE);
}

bool EhValidaPosicao(Game *game, Posicao pos){
    if (pos.x < 0 || pos.x >= MAPA_LARGURA || pos.y < 0 || pos.y >= MAPA_ALTURA)
    {
        return false;
    }

    char tile = game->map.grid[pos.y][pos.x];
    return (tile == ' ' || tile == 'K');
}

