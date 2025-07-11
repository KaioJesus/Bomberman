#ifndef BOMB_H
#define BOMB_H

#include "estruturas.h"
#include <raylib.h>

// Funções relacionadas à bomba
void PlantaBomba(Game *game);
void AtualizaBomba(Game *game);
void ExplodeBomba(Game *game, int bombIndex);
void DesenhaBombas(Game *game);

#endif
