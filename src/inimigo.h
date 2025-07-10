#ifndef INIMIGO_H
#define INIMIGO_H

#include "estruturas.h"
#include "map.h"

// Inicializa inimigos a partir do mapa (achar 'E')
void InicializarInimigos(Game *game);

// Atualiza movimento e colisões
void AtualizarInimigos(Game *game);

#endif
