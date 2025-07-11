#ifndef JOGADOR_H
#define JOGADOR_H

#include <stdio.h>
#include "estruturas.h"
#include "map.h"

bool verificaColisao(Posicao pos1, Posicao pos2);
void MoveJogador(Game *game, Direcao dir);

#endif