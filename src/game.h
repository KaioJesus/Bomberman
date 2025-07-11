#ifndef GAME_H
#define GAME_H

#include "estruturas.h"
#include "bomb.h"
#include "inimigo.h"
#include "jogador.h"
#include "map.h"
#include <stdio.h>

// Protótipos das funções
Game *IniciaJogo(void);
void AtualizaJogo(Game *game);
void DesenhaJogo(Game *game);
void FreeGame(Game *game);

#endif