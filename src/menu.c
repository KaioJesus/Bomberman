// Baseado no código fornecido no primeiro arquivo (paste.txt)
#include <stdbool.h>
#include "estruturas.h"
#include <stdlib.h>
#include "menu.h"

// Função responsável por desenhar o menu principal do jogo
void DesenhaMenu(Game *game)
{
    // Desenha um retângulo preto cobrindo toda a tela (fundo do menu)
    DrawRectangle(0, 0, TELA_LARGURA, TELA_ALTURA, BLACK);

    // Array com as opções do menu completo
    char *menuOpcoes[] = {"Novo Jogo (N)", "Carregar Jogo (C)", "Salvar Jogo (S)", "Sair (Q)", "Voltar (V)"};

    // Array com opções reduzidas para quando o jogo termina (Game Over)
    char *menuOpcoesPerderOuGanhar[] = {"Novo Jogo (N)", "Sair (Q)"}; // Apenas Novo Jogo e Sair

    // Número de opções no menu (inicialmente 5)
    int contadorMenu = 5;

    // Ponteiro para o array de opções que será usado
    char **menuOpcoesAtual;

    // Verifica se o jogo terminou (jogador perdeu todas as vidas)
    bool jogoTerminou = (game->jogador.vidas <= 0);

    if (jogoTerminou)
    {
        // Se o jogo terminou, usa o menu reduzido
        menuOpcoesAtual = menuOpcoesPerderOuGanhar;
        contadorMenu = 2; // Apenas Novo Jogo e Sair

        // Garante que o item selecionado seja uma opção válida no menu reduzido
        if (game->menuSelecionado != MENU_NOVO_JOGO && game->menuSelecionado != MENU_SAIR)
        {
            game->menuSelecionado = MENU_NOVO_JOGO; // Volta para Novo Jogo se for uma opção inválida
        }
    }
    else
    {
        // Caso contrário (se entrou no menu durante o jogo com TAB)
        menuOpcoesAtual = menuOpcoes;
        contadorMenu = 5;
    }

    // Calcula a posição vertical para centralizar o menu na tela
    int posicaoVerticalMenu = TELA_ALTURA / 2 - (contadorMenu * 30) / 2;

    // Posição horizontal centralizada
    int posicaoHorizontalMenu = TELA_LARGURA / 2;

    // Desenha o título "MENU" centralizado acima das opções
    DrawText("MENU", posicaoHorizontalMenu - 50, posicaoVerticalMenu - 50, 40, WHITE);

    // Loop para desenhar cada opção do menu
    for (int i = 0; i < contadorMenu; i++)
    {
        // Define a cor: amarelo para opção selecionada, branco para as outras
        Color cor = (i == game->menuSelecionado) ? YELLOW : WHITE;

        // Desenha a opção do menu na posição calculada
        DrawText(menuOpcoesAtual[i], posicaoHorizontalMenu - 100, posicaoVerticalMenu + i * 40, 20, cor);
    }
}

// Função para desenhar a tela de Game Over
void DesenhaTelaJogoPerdido(Game *game)
{
    // Desenha um retângulo semi-transparente sobre a tela
    DrawRectangle(0, 0, TELA_LARGURA, TELA_ALTURA, (Color){0, 0, 0, 100});

    // Desenha "GAME OVER" centralizado em vermelho
    DrawText("GAME OVER", TELA_LARGURA / 2 - MeasureText("GAME OVER", 60) / 2, TELA_ALTURA / 2 - 50, 60, RED);

    // Desenha instruções para voltar ao menu
    DrawText("Pressione ENTER para voltar ao Menu", TELA_LARGURA / 2 - MeasureText("Pressione ENTER para voltar ao Menu", 20) / 2, TELA_ALTURA / 2 + 20, 20, WHITE);
}

// Função para desenhar a tela de vitória
void DesenhaTelaVitoria(Game *game)
{
    // Desenha um retângulo semi-transparente mais escuro
    DrawRectangle(0, 0, TELA_LARGURA, TELA_ALTURA, (Color){0, 0, 0, 200});

    // Desenha "VOCÊ VENCEU!" centralizado em verde
    DrawText("VOCÊ VENCEU!", TELA_LARGURA / 2 - MeasureText("VOCÊ VENCEU!", 60) / 2, TELA_ALTURA / 2 - 50, 60, GREEN);

    // Desenha instruções para voltar ao menu
    DrawText("Pressione ENTER para jogar próximo nível", TELA_LARGURA / 2 - MeasureText("Pressione ENTER para jogar próximo nível", 20) / 2, TELA_ALTURA / 2 + 20, 20, WHITE);
}

// Função principal para tratamento de entrada do usuário
void HandleInput(Game *game)
{
    // Se estivermos no estado de menu
    if (game->state == GAME_MENU)
    {
        // Navegação para cima (setas ou WASD)
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        {
            // Decrementa com wrap-around (volta ao final se chegar no início)
            game->menuSelecionado = (game->menuSelecionado - 1 + 5) % 5;
        }
        // Navegação para baixo
        else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        {
            // Incrementa com wrap-around (volta ao início se chegar no final)
            game->menuSelecionado = (game->menuSelecionado + 1) % 5;
        }
        // Confirmação da seleção
        else if (IsKeyPressed(KEY_ENTER))
        {
            // Switch para executar a ação baseada na opção selecionada
            switch (game->menuSelecionado)
            {
            case MENU_NOVO_JOGO:
                // Reinicia o jogador na posição inicial
                inicializa_jogador(&game->jogador, 0, 0);
                // Carrega o primeiro mapa
                CarregaMapa(game, game->map.levelAtual);
                // Muda para o estado de jogo
                game->state = GAME_PLAYING;
                break;
            case MENU_CARREGAR_JOGO:
                // Carrega jogo salvo
                CarregarJogo(game);
                game->state = GAME_PLAYING;
                break;
            case MENU_SALVAR_JOGO:
                // Salva o jogo atual
                SalvarJogo(game);
                game->state = GAME_PLAYING;
                break;
            case MENU_SAIR:
                // Encerra o programa
                exit(0);
                break;
            case MENU_VOLTAR:
                // Volta ao jogo (se veio do jogo)
                game->state = GAME_PLAYING;
                break;
            }
        }
        // Atalhos de teclado para seleção direta
        else if (IsKeyPressed(KEY_N))
        {
            game->menuSelecionado = MENU_NOVO_JOGO;
        }
        else if (IsKeyPressed(KEY_C))
        {
            game->menuSelecionado = MENU_CARREGAR_JOGO;
        }
        else if (IsKeyPressed(KEY_S))
        {
            game->menuSelecionado = MENU_SALVAR_JOGO;
        }
        else if (IsKeyPressed(KEY_Q))
        {
            game->menuSelecionado = MENU_SAIR;
        }
        else if (IsKeyPressed(KEY_V))
        {
            game->menuSelecionado = MENU_VOLTAR;
        }
    }
    // Se estivermos jogando
    else if (game->state == GAME_PLAYING)
    {
        // TAB para abrir o menu
        if (IsKeyPressed(KEY_TAB))
        {
            game->state = GAME_MENU;
        }
        // Controles de movimento do jogador
        else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        {
            MoveJogador(game, DIR_CIMA);
        }
        else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        {
            MoveJogador(game, DIR_BAIXO);
        }
        else if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
        {
            MoveJogador(game, DIR_ESQUERDA);
        }
        else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
        {
            MoveJogador(game, DIR_DIREITA);
        }
        // B para plantar bomba
        else if (IsKeyPressed(KEY_B))
        {
            PlantaBomba(game);
        }
    }
    // Se estivermos nas telas de Game Over
    else if (game->state == GAME_OVER)
    {
        // ENTER para voltar ao menu
        if (IsKeyPressed(KEY_ENTER))
        {
            game->state = GAME_MENU;
            // game->menuSelecionado = MENU_NOVO_JOGO; // Seleciona "Novo Jogo" por padrão
        }
    }
    // se ganhar vai para o proximo nivel
    else if (game->state == GAME_WIN)
    {
        // ENTER para voltar ao menu
        if (IsKeyPressed(KEY_ENTER))
        {
            CarregaMapa(game, game->map.levelAtual);
            game->state = GAME_PLAYING;
        }
    }
}