#ifndef MENU_H
#define MENU_H

#include "estruturas.h"

void DesenhaMenu(Game *game);
void DesenhaTelaJogoPerdido(Game *game);
void DesenhaTelaVitoria(Game *game);
void HandleInput(Game *game);

#endif