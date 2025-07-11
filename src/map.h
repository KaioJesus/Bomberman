#ifndef MAPA_H
#define MAPA_H

#include "inimigo.h"
#include "jogador.h"
#include "estruturas.h"

// Funções do mapa
void CarregaMapa(Game *game, int level);
bool EhValidaPosicao(Game *game, Posicao pos);

#endif
