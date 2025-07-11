#ifndef MENU_H
#define MENU_H

#include "estruturas.h"
#include "map.h"
#include "state_game.h"
#include "jogador.h"
#include "bomb.h"

void DesenhaMenu(Game *game);
void DesenhaTelaJogoPerdido(Game *game);
void DesenhaTelaVitoria(Game *game);
void HandleInput(Game *game);

#endif