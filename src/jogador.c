#include "jogador.h"
#include "raylib.h"
#include "mapa.h" 

// Função que verifica se duas posições são iguais (colisão)
bool verificaColisao(Posicao pos1, Posicao pos2) {
    return (pos1.x == pos2.x && pos1.y == pos2.y);
}

// Inicializa o jogador com posição, vidas, pontos, bombas e chaves
void inicializa_jogador(Jogador *j, int x, int y) {
    j->pos.x = x;
    j->pos.y = y;
    j->vidas = 3;
    j->pontos = 0;
    j->bombaContador = 3;
    j->chavesColetadas = 0;
}

// Move o jogador na direção indicada, se a posição for válida
void MoveJogador(Game *game, Direcao dir) {
    Posicao newPos = game->jogador.pos;  // posição atual do jogador

    // Atualiza a nova posição de acordo com a direção
    switch (dir) {
        case DIR_CIMA: newPos.y--; break;
        case DIR_BAIXO: newPos.y++; break;
        case DIR_ESQUERDA: newPos.x--; break;
        case DIR_DIREITA: newPos.x++; break;
    }

    // Verifica se a nova posição é válida dentro do mapa
    if (EhValidaPosicao(game, newPos)) {
        // Se for uma chave ('K'), o jogador coleta e remove do mapa
        if (game->map.grid[newPos.y][newPos.x] == 'K') {
            game->jogador.chavesColetadas++;
            game->map.grid[newPos.y][newPos.x] = ' ';
        }

        // Atualiza a posição do jogador
        game->jogador.pos = newPos;
    }
}

// Desenha o jogador na tela usando a biblioteca
void desenhar_jogador(Jogador j) {
    DrawRectangle(j.pos.x * BLOCO_TAMANHO, j.pos.y * BLOCO_TAMANHO, BLOCO_TAMANHO, BLOCO_TAMANHO, BLUE);
}

// Verifica se a posição está dentro dos limites do mapa e se não é uma parede
bool EhValidaPosicao(Game *game, Posicao pos){
    // Verifica limites do mapa
    if (pos.x < 0 || pos.x >= MAPA_LARGURA || pos.y < 0 || pos.y >= MAPA_ALTURA)
    {
        return false;
    }

    // Verifica se o tile é espaço vazio ou chave (permitido andar)
    char tile = game->map.grid[pos.y][pos.x];
    return (tile == ' ' || tile == 'K');
}
