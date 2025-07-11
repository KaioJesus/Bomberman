<<<<<<< HEAD
#ifndef MAPA_H
#define MAPA_H

#include "inimigo.h"
#include "jogador.h"
#include "estruturas.h"

// Funções do mapa
void IniciaMapa(GameMapa *mapa);
void CarregaMapa(Game *game, int level);
bool EhValidaPosicao(Game *game, Posicao pos);
void DesenhaMapa(Game *game);
void FreeMapa(GameMapa *map);

#endif
=======
#ifndef MAPA_H
#define MAPA_H

#include "estruturas.h"

// Funções do mapa
void IniciaMapa(GameMapa *mapa);
void CarregaMapa(Game *game, int level);
bool EhValidaPosicao(Game *game, Posicao pos);
void DesenhaMapa(Game *game);
void FreeMapa(GameMapa *map);

#endif
>>>>>>> 7533bf8a2a625ca6c81d3e2d1303422c0ae902c7
