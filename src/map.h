#ifndef MAPA_H
#define MAPA_H

#include "game_types.h"

// Funções do mapa
void IniciaMapa(GameMapa *mapa);
void CarregaMapa(Game *game, int level);
bool EhValidaPosicao(Game *game, Posicao pos);
void DesenhaMapa(Game *game);
void FreeMapa(GameMapa *map);

#endif