#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "inimigo.h"
#include "map.h"
#include "bomb.h"
#include "state_game.h"
#include "menu.h"
#include "game.h"
#include "jogador.h"

// Função principal
int main(void)
{
    // Inicialização da janela do jogo
    // InitWindow(largura, altura, título)
    InitWindow(TELA_LARGURA, TELA_ALTURA, "Bomberman - Programação II");

    // Define o FPS (Frames Per Second) alvo do jogo
    // 60 FPS significa que o jogo tentará rodar a 60 quadros por segundo
    SetTargetFPS(60);

    // Inicializa o gerador de números aleatórios com o tempo atual
    // Isso garante que a sequência de números "aleatórios" seja diferente a cada execução
    srand(time(NULL));

    // Cria e inicializa uma nova instância do jog
    Game *game = IniciaJogo();

    // Carrega o primeiro mapa do jogo
    CarregaMapa(game, game->map.levelAtual);

    // LOOP PRINCIPAL DO JOGO
    // Continua executando enquanto a janela não for fechada
    while (!WindowShouldClose())
    {

        // === FASE DE ENTRADA (INPUT) ===
        // Processa todas as entradas do usuário (teclado, mouse, etc.)
        HandleInput(game);

        // === FASE DE ATUALIZAÇÃO (UPDATE) ===
        // Atualiza a lógica do jogo apenas se estivermos no estado de jogo
        if (game->state == GAME_PLAYING)
        {

            // Atualiza física, colisões, etc.
            AtualizaJogo(game);
        }

        // === FASE DE RENDERIZAÇÃO (RENDER) ===
        // Inicia o processo de desenho
        BeginDrawing();

        // Limpa a tela com a cor de fundo
        ClearBackground(SKYBLUE);

        // Desenha elementos diferentes baseado no estado atual do jogo
        if (game->state == GAME_PLAYING)
        {
            // Desenha o jogo em si (mapa, jogador, inimigos, etc.)
            DesenhaJogo(game);
        }
        else if (game->state == GAME_MENU)
        {
            // Desenha o menu principal
            DesenhaMenu(game);
        }
        else if (game->state == GAME_OVER)
        {
            // Desenha a tela de Game Over
            DesenhaTelaJogoPerdido(game);
        }
        else if (game->state == GAME_WIN)
        {
            // Desenha a tela de vitória
            DesenhaTelaVitoria(game);
        }

        // Finaliza o processo de desenho e apresenta o frame na tela
        EndDrawing();
    }

    // === LIMPEZA E FINALIZAÇÃO ===
    // Libera toda a memória alocada dinamicamente
    FreeGame(game);

    // Fecha a janela e finaliza o contexto gráfico
    CloseWindow();

    return 0;
}

/*
E

1. INICIALIZAÇÃO:
   - Configura a janela gráfica com Raylib
   - Define FPS alvo para controle de velocidade
   - Inicializa sistema de números aleatórios
   - Cria estrutura do jogo e carrega primeiro mapa

2. LOOP PRINCIPAL (Game Loop):
   Este é o coração de qualquer jogo. Consiste em três fases que se repetem:

   a) INPUT (Entrada):
      - Captura teclas pressionadas
      - Processa comandos do usuário
      - Atualiza estado baseado na entrada

   b) UPDATE (Atualização):
      - Atualiza lógica do jogo
      - Move personagens
      - Verifica colisões
      - Atualiza temporizadores
      - Aplica regras do jogo

   c) RENDER (Renderização):
      - Limpa tela anterior
      - Desenha todos os elementos
      - Apresenta frame final

3. ESTADOS DO JOGO:
   O jogo usa uma máquina de estados para controlar diferentes telas:
   - GAME_PLAYING: Jogando normalmente
   - GAME_MENU: No menu principal
   - GAME_OVER: Tela de derrota
   - GAME_WIN: Tela de vitória

4. GERENCIAMENTO DE RECURSOS:
   - Memória é alocada dinamicamente
   - Importante liberar recursos no final
   - Evita vazamentos de memória

5. CONTROLE DE FPS:
   - SetTargetFPS(60) tenta manter 60 quadros por segundo
   - Garante velocidade consistente em diferentes computadores
   - GetFrameTime() retorna o tempo entre frames

6. RAYLIB:
   - Biblioteca C para desenvolvimento de jogos
   - Simplifica criação de janelas, entrada e gráficos
   - Multiplataforma (Windows, Linux, Mac, Web)
*/
