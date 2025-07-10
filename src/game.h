#ifndef GAME_H
#define GAME_H

#include "estruturas.h"
#include <stdio.h>

// Protótipos das funções
Game *IniciaJogo(void);
void AtualizaJogo(Game *game);
void DesenhaJogo(Game *game);

#endif