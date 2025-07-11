#include <stdbool.h>
#include "estruturas.h"
#include <stdlib.h>
#include "menu.h"

// Desenho do menu
void DesenhaMenu(Game *game)
{
    DrawRectangle(0, 0, TELA_LARGURA, TELA_ALTURA, BLACK);

    char *menuOpcoes[] = {"Novo Jogo (N)", "Carregar Jogo (C)", "Salvar Jogo (S)", "Sair (Q)", "Voltar (V)"};
    char *menuOpcoesPerderOuGanhar[] = {"Novo Jogo (N)", "Sair (Q)"}; // Apenas Novo Jogo e Sair
    int contadorMenu = 5;

    char **menuOpcoesAtual;

    bool jogoTerminou = (game->jogador.vidas <= 0);

    if (jogoTerminou)
    {
        menuOpcoesAtual = menuOpcoesPerderOuGanhar;
        contadorMenu = 2; // Apenas Novo Jogo e Sair
        // Garante que o item selecionado seja uma opção válida no menu reduzido
        if (game->menuSelecionado != MENU_NOVO_JOGO && game->menuSelecionado != MENU_SAIR)
        {
            game->menuSelecionado = MENU_NOVO_JOGO; // Volta para Novo Jogo se for uma opção inválida
        }
    }
    else
    { // Caso contrário (se entrou no menu a partir de GAME_PLAYING)
        menuOpcoesAtual = menuOpcoes;
        contadorMenu = 5;
    }

    int posicaoVerticalMenu = TELA_ALTURA / 2 - (contadorMenu * 30) / 2;
    int posicaoHorizontalMenu = TELA_LARGURA / 2;

    // Escreve na tela a palava meno na posição informada
    DrawText("MENU", posicaoHorizontalMenu - 50, posicaoVerticalMenu - 50, 40, WHITE);

    for (int i = 0; i < contadorMenu; i++)
    {
        Color cor = (i == game->menuSelecionado) ? YELLOW : WHITE;
        DrawText(menuOpcoes[i], posicaoHorizontalMenu - 100, posicaoVerticalMenu + i * 40, 20, cor);
    }
}

void DesenhaTelaJogoPerdido(Game *game)
{
    DrawRectangle(0, 0, TELA_LARGURA, TELA_ALTURA, (Color){0, 0, 0, 100});
    DrawText("GAME OVER", TELA_LARGURA / 2 - MeasureText("GAME OVER", 60) / 2, TELA_ALTURA / 2 - 50, 60, RED);
    DrawText("Pressione ENTER para voltar ao Menu", TELA_LARGURA / 2 - MeasureText("Pressione ENTER para voltar ao Menu", 20) / 2, TELA_ALTURA / 2 + 20, 20, WHITE);
}

void DesenhaTelaVitoria(Game *game)
{
    DrawRectangle(0, 0, TELA_LARGURA, TELA_ALTURA, (Color){0, 0, 0, 200});
    DrawText("VOCÊ VENCEU!", TELA_LARGURA / 2 - MeasureText("VOCÊ VENCEU!", 60) / 2, TELA_ALTURA / 2 - 50, 60, GREEN);
    DrawText("Pressione ENTER para voltar ao Menu", TELA_LARGURA / 2 - MeasureText("Pressione ENTER para voltar ao Menu", 20) / 2, TELA_ALTURA / 2 + 20, 20, WHITE);
}

// Tratamento de entrada
void HandleInput(Game *game)
{
    if (game->state == GAME_MENU)
    {
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        {
            game->menuSelecionado = (game->menuSelecionado - 1 + 5) % 5;
        }
        else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        {
            game->menuSelecionado = (game->menuSelecionado + 1) % 5;
        }
        else if (IsKeyPressed(KEY_ENTER))
        {
            switch (game->menuSelecionado)
            {
            case MENU_NOVO_JOGO:
                // Resetar jogo
                inicializa_jogador(&game->jogador, 0, 0);
                CarregaMapa(game, 1);
                game->state = GAME_PLAYING;
                break;
            case MENU_CARREGAR_JOGO:
                CarregarJogo(game);
                game->state = GAME_PLAYING;
                break;
            case MENU_SALVAR_JOGO:
                SalvarJogo(game);
                game->state = GAME_PLAYING;
                break;
            case MENU_SAIR:
                exit(0);
                break;
            case MENU_VOLTAR:
                game->state = GAME_PLAYING;
                break;
            }
        }
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
    else if (game->state == GAME_PLAYING)
    {
        if (IsKeyPressed(KEY_TAB))
        {
            game->state = GAME_MENU;
        }
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
        else if (IsKeyPressed(KEY_B))
        {
            PlantaBomba(game);
        }
    }
    else if (game->state == GAME_OVER || game->state == GAME_WIN)
    {
        if (IsKeyPressed(KEY_ENTER))
        {
            game->state = GAME_MENU;
            game->menuSelecionado = MENU_NOVO_JOGO; // Seleciona "Novo Jogo" por padrão no menu
        }
    }
}