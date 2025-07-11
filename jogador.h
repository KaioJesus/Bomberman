#ifndef JOGADOR_H
#define JOGADOR_H

#include <stdbool.h>
#include "estruturas.h"

// Inicializa os dados do jogador
void inicializa_jogador(Jogador *j, int x, int y);

// Move o jogador de acordo com a direção e verifica colisões
void MoveJogador(Game *game, Direcao dir);

// Verifica colisão entre duas posições
bool verificaColisao(Posicao pos1, Posicao pos2);

// Desenha o jogador na tela
void desenhar_jogador(Jogador j);

bool EhValidaPosicao(Game *game, Posicao pos);

#endif
