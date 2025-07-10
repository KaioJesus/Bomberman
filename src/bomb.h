#ifndef BOMB_H
#define BOMB_H

#include "estruturas.h"
#include <raylib.h>

// Funções relacionadas à bomba
void PlantaBomba(Game game);
void AtualizaBomba(Gamegame);
void ExplodeBomba(Game game, int bombIndex);
void DesenhaBombas(Gamegame);

#endif
