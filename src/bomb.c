#include "bomb.h"
#include <math.h>

// Função que planta uma bomba na posição atual do jogador, se ele ainda tiver bombas disponíveis
void PlantaBomba(Game *game) {
    // Verifica se o jogador tem bombas disponíveis
    if (game->jogador.bombaContador <= 0) return;

    // Procura um espaço no array de bombas para ativar uma nova
    for (int i = 0; i < MAX_BOMBAS; i++) {
        if (!game->bombas[i].ativo) {  // Se a bomba não estiver ativa
            game->bombas[i].pos = game->jogador.pos;          // Coloca a bomba na posição do jogador
            game->bombas[i].timer = BOMBA_TEMPORIZADOR;       // Define o tempo até a explosão
            game->bombas[i].ativo = true;                     // Ativa a bomba
            game->jogador.bombaContador--;                    // Decrementa o número de bombas disponíveis do jogador
            break; // Sai do loop após plantar a bomba
        }
    }
}

// Atualiza o estado de todas as bombas do jogo a cada frame
void AtualizaBomba(Game *game) {
    for (int i = 0; i < MAX_BOMBAS; i++) {
        if (game->bombas[i].ativo) {
            // Reduz o tempo restante da bomba baseado no tempo do frame
            game->bombas[i].timer -= GetFrameTime();
            // Se o tempo acabar, explode a bomba
            if (game->bombas[i].timer <= 0) {
                ExplodeBomba(game, i);
            }
        }
    }
}

// Função que lida com a explosão da bomba
void ExplodeBomba(Game *game, int bombIndex) {
    Posicao bombPos = game->bombas[bombIndex].pos;   // Posição onde a bomba está
    game->bombas[bombIndex].ativo = false;           // Desativa a bomba
    game->jogador.bombaContador++;                   // Retorna uma bomba ao jogador

    // Verifica se o jogador está dentro da área de explosão
    float distTojogador = sqrt(pow(game->jogador.pos.x - bombPos.x, 2) + 
                               pow(game->jogador.pos.y - bombPos.y, 2));
    if (distTojogador * BLOCO_TAMANHO <= EXPLOSAO_TAMANHO / 2) {
        game->jogador.vidas--; // Jogador perde uma vida
        // Reduz os pontos do jogador, se ele tiver mais que 100
        game->jogador.pontos = (game->jogador.pontos > 100) ? game->jogador.pontos - 100 : 0;
    }

    // Verifica se o jogador perdeu todas as vidas
    if (game->jogador.vidas <= 0) {
        game->state = GAME_OVER;
    }

    // Explosão em cruz (4 direções: cima, baixo, esquerda, direita)
    int Direcaos[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    
    for (int d = 0; d < 4; d++) {
        for (int alcance = 1; alcance <= 2; alcance++) {  // Raio da explosão de 2 blocos
            int x = bombPos.x + Direcaos[d][0] * alcance;
            int y = bombPos.y + Direcaos[d][1] * alcance;
            
            // Verifica se está dentro dos limites do mapa
            if (x < 0 || x >= MAPA_LARGURA || y < 0 || y >= MAPA_ALTURA) break;

            char bloco = game->map.grid[y][x];

            if (bloco == 'W') break; // Parede indestrutível bloqueia a explosão

            if (bloco == 'D' || bloco == 'B') { // Bloco destrutível ou bomba
                game->map.grid[y][x] = ' ';     // Remove o bloco
                game->jogador.pontos += 10;     // Adiciona pontos
                break;                          // A explosão para nesse ponto
            }

            if (bloco == 'K') { // Se encontrar a chave
                game->jogador.chavesColetadas++;
                game->jogador.pontos += 10;
                game->map.grid[y][x] = ' '; // Remove a chave do mapa
                break;
            }
        }
    }

    // Verifica se algum inimigo foi atingido pela explosão
    for (int i = 0; i < game->InimigoCount; i++) {
        float distToInimigo = sqrt(pow(game->enemies[i].pos.x - bombPos.x, 2) + 
                                   pow(game->enemies[i].pos.y - bombPos.y, 2));
        if (distToInimigo * BLOCO_TAMANHO <= EXPLOSAO_TAMANHO / 2) {
            // Remove o inimigo atingido (reorganiza o vetor de inimigos)
            for (int j = i; j < game->InimigoCount - 1; j++) {
                game->enemies[j] = game->enemies[j + 1];
            }
            game->InimigoCount--;          // Diminui a quantidade de inimigos
            game->jogador.pontos += 20;    // Ganha pontos pela eliminação
            i--; // Reavalia a posição atual, já que os inimigos foram reorganizados
        }
    }
}
