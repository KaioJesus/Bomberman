// Baseado no código fornecido no segundo arquivo (paste-2.txt)
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "jogador.h"

// Função responsável por inicializar uma nova instância do jogo
Game *IniciaJogo(void)
{
    // Aloca memória dinamicamente para a estrutura Game
    Game *game = (Game *)malloc(sizeof(Game));

    // Inicializa o jogador na posição (0, 0)
    inicializa_jogador(&game->jogador, 0, 0);

    // Configuração inicial do estado do jogo
    game->state = GAME_PLAYING;             // Começa no estado de jogo
    game->menuSelecionado = MENU_NOVO_JOGO; // Opção padrão do menu
    game->gameTime = 0.0f;                  // Tempo de jogo zerado
    game->InimigoCount = 0;                 // Número de inimigos zerado
    game->bombaContador = 0;                // Contador de bombas zerado

    // Inicialização do mapa
    // Aloca memória para o array de ponteiros (linhas do mapa)
    game->map.grid = (char **)malloc(MAPA_ALTURA * sizeof(char *));
    for (int i = 0; i < MAPA_ALTURA; i++)
    {
        // Aloca memória para cada linha do mapa
        game->map.grid[i] = (char *)malloc(MAPA_LARGURA * sizeof(char));
    }
    game->map.levelAtual = 1;  // Inicia no nível 1
    game->map.totalChaves = 5; // Define 5 chaves por nível

    // Inicializa todas as bombas como inativas
    for (int i = 0; i < MAX_BOMBAS; i++)
    {
        game->bombas[i].ativo = false;
    }

    // Retorna o ponteiro para a estrutura Game criada
    return game;
}

// Função principal de atualização da lógica do jogo
void AtualizaJogo(Game *game)
{
    // Atualiza o tempo total de jogo
    // GetFrameTime() retorna o tempo desde o último frame
    game->gameTime += GetFrameTime();

    // Atualiza o estado de todas as bombas
    AtualizaBomba(game);

    // Atualiza o movimento e comportamento dos inimigos
    AtualizaInimigos(game);

    // Verificação de colisão entre jogador e inimigos
    for (int i = 0; i < game->InimigoCount; i++)
    {
        // Verifica se o jogador colidiu com algum inimigo
        if (verificaColisao(game->jogador.pos, game->enemies[i].pos))
        {
            // Reduz uma vida do jogador
            game->jogador.vidas--;

            // Reduz pontos (mínimo 0)
            game->jogador.pontos = (game->jogador.pontos > 100) ? game->jogador.pontos - 100 : 0;

            // Verifica se o jogador perdeu todas as vidas
            if (game->jogador.vidas <= 0)
            {
                game->state = GAME_OVER; // Muda para estado de Game Over
            }

            // Reposiciona o jogador para a posição inicial após colisão
            game->jogador.pos.x = 1;
            game->jogador.pos.y = 1;
        }
    }

    // Verificação de condição de vitória
    if (game->jogador.chavesColetadas >= game->map.totalChaves)
    {
        // Avança para o próximo nível
        game->map.levelAtual++;
        game->state = GAME_WIN;

        // Reseta o contador de chaves coletadas
        game->jogador.chavesColetadas = 0;

        game->jogador.pontos = 0;

        // Carrega o próximo mapa
        // CarregaMapa(game, game->map.levelAtual);
    }
}

// Função responsável por desenhar todos os elementos do jogo na tela
void DesenhaJogo(Game *game)
{
    // Desenha o mapa (fundo, paredes, blocos destrutíveis, chaves)
    for (int y = 0; y < MAPA_ALTURA; y++)
    {
        for (int x = 0; x < MAPA_LARGURA; x++)
        {
            // Calcula a posição do retângulo na tela
            Rectangle rect = {x * BLOCO_TAMANHO, y * BLOCO_TAMANHO, BLOCO_TAMANHO, BLOCO_TAMANHO};

            // Desenha diferentes tipos de blocos baseado no caractere do mapa
            switch (game->map.grid[y][x])
            {
            case 'W': // Parede indestrutível
                DrawRectangleRec(rect, GRAY);
                break;
            case 'D': // Parede destrutível
                DrawRectangleRec(rect, BROWN);
                break;
            case 'B': // Caixas sem chave
                DrawRectangleRec(rect, GOLD);
                break;
            case 'K': // Caixas com Chave
                DrawRectangleRec(rect, GOLD);
                break;
                // Espaços vazios (' ') não são desenhados (ficam transparentes)
            }
        }
    }

    // Desenha o jogador na sua posição atual
    desenhar_jogador(game->jogador);

    // Desenha todos os inimigos ativos
    for (int i = 0; i < game->InimigoCount; i++)
    {
        // Calcula a posição do retângulo do inimigo
        Rectangle InimigoRect = {
            game->enemies[i].pos.x * BLOCO_TAMANHO,
            game->enemies[i].pos.y * BLOCO_TAMANHO,
            BLOCO_TAMANHO,
            BLOCO_TAMANHO};

        // Desenha o inimigo como um retângulo vermelho
        DrawRectangleRec(InimigoRect, RED);
    }

    // Desenha todas as bombas ativas
    for (int i = 0; i < MAX_BOMBAS; i++)
    {
        if (game->bombas[i].ativo) // Só desenha se a bomba estiver ativa
        {
            // Calcula a posição do retângulo da bomba
            Rectangle bombRect = {
                game->bombas[i].pos.x * BLOCO_TAMANHO,
                game->bombas[i].pos.y * BLOCO_TAMANHO,
                BLOCO_TAMANHO,
                BLOCO_TAMANHO};

            // Desenha a bomba como um retângulo preto com borda branca
            DrawRectangleRec(bombRect, BLACK);
            DrawRectangleLinesEx(bombRect, 2, WHITE);
        }
    }

    // Desenha a interface do usuário (HUD)
    // Retângulo azul na parte inferior da tela para as informações
    DrawRectangle(0, GAME_AREA_HEIGHT, TELA_LARGURA, INTERFACE_ALTURA, SKYBLUE);

    // Prepara o texto com as informações do jogador
    char informacoesJogo[200];
    sprintf(informacoesJogo, "Bombas: %d | Vidas: %d  | Pontos: %d | Chaves: %d",
            game->jogador.bombaContador, game->jogador.vidas, game->jogador.pontos, game->jogador.chavesColetadas);

    // Desenha as informações do jogador na interface
    DrawText(informacoesJogo, 10, GAME_AREA_HEIGHT + 20, 24, BLACK);

    // Desenha as instruções de controle
    DrawText("TAB: Menu | WASD/Setas: Mover | B: Bomba", 10, GAME_AREA_HEIGHT + 64, 24, BLACK);
}

// Função para liberar toda a memória alocada dinamicamente
void FreeGame(Game *game)
{
    // Libera a memória de cada linha do grid do mapa
    for (int i = 0; i < MAPA_ALTURA; i++)
    {
        free(game->map.grid[i]);
    }

    // Libera a memória do array de ponteiros do grid
    free(game->map.grid);

    // Libera a memória da estrutura principal do jogo
    free(game);
}

/*
OBSERVAÇÕES IMPORTANTES:

1. GERENCIAMENTO DE MEMÓRIA:
   - A função IniciaJogo() aloca memória que deve ser liberada com FreeGame()
   - O mapa é uma matriz dinâmica (array de ponteiros para arrays)
   - É crucial chamar FreeGame() para evitar vazamentos de memória

2. ESTRUTURA DO LOOP DE JOGO:
   - AtualizaJogo() atualiza a lógica (física, IA, colisões)
   - DesenhaJogo() renderiza tudo na tela
   - Esta separação é uma boa prática em game development

3. SISTEMA DE COORDENADAS:
   - O mapa usa coordenadas de grid (x, y em células)
   - O desenho usa coordenadas de pixel (multiplicadas por BLOCO_TAMANHO)
   - Conversão: pixel = grid * BLOCO_TAMANHO

4. ESTADOS DO JOGO:
   - GAME_PLAYING: jogando normalmente
   - GAME_OVER: jogador perdeu
   - GAME_MENU: no menu
   - GAME_WIN: jogador venceu (não implementado neste arquivo)
*/