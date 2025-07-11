# 💣 Bomberman em C com Raylib 🎮

Este é um projeto de um clone do clássico jogo Bomberman, desenvolvido em C utilizando a biblioteca gráfica [Raylib](https://www.raylib.com/). O objetivo é proporcionar uma experiência divertida e desafiadora, com foco na jogabilidade e na exploração de mapas.

## ✨ Funcionalidades

* **Movimentação do Jogador**: Controle o Bomberman pelo mapa usando as teclas W, A, S, D ou as setas direcionais.
* **Plantio de Bombas**: Posicione bombas estratégicas com a tecla `B` para destruir blocos e inimigos.
* **Explosões Dinâmicas**: As bombas explodem após um tempo determinado, afetando áreas em cruz e destruindo blocos e inimigos dentro do raio de alcance.
* **Inimigos Inteligentes**: Inimigos se movem aleatoriamente pelo mapa e podem tirar vidas do jogador em caso de colisão.
* **Coleta de Chaves**: Encontre e colete chaves espalhadas pelo mapa para avançar de nível.
* **Múltiplos Níveis**: Enfrente desafios crescentes em diferentes mapas.
* **Sistema de Vidas e Pontuação**: Monitore sua pontuação, vidas restantes e quantidade de bombas disponíveis através da interface do jogo.
* **Telas de Menu**: Navegue por um menu principal com opções de "Novo Jogo", "Carregar Jogo", "Salvar Jogo" e "Sair".
* **Telas de Game Over e Vitória**: Feedback visual claro ao perder ou vencer o jogo.
* **Salvar/Carregar Jogo**: Persista o estado do jogo para continuar sua aventura mais tarde.

## 🛠️ Tecnologias Utilizadas

* **Linguagem de Programação**: C
* **Biblioteca Gráfica**: Raylib (v4.5)

## 🚀 Como Rodar o Jogo

Para compilar e executar este projeto, você precisará ter o compilador GCC (ou similar, como MinGW no Windows) e a biblioteca Raylib instalada em seu sistema.

### Pré-requisitos

* **GCC**: Verifique se está instalado (`gcc --version`).
* **Raylib**: Siga as instruções de instalação para o seu sistema operacional em [raylib.com/cheatsheet/build_options.html](https://www.raylib.com/cheatsheet/build_options.html).

### Compilação e Execução

1.  **Clone o repositório**:
    ```bash
    git clone https://github.com/KaioJesus/Bomberman.git
    ```
2.  **Crie a pasta de mapas**:
    Dentro do diretório raiz do projeto, crie uma pasta chamada `mapas` e coloque os arquivos `mapa1.txt`, `mapa2.txt`, etc., dentro dela. (Exemplo: `mapas/mapa1.txt`).
3.  **Compile o jogo**:
    Certifique-se de que todos os arquivos `.c` e `.h` estejam na mesma pasta ou que seu `Makefile` (se houver) esteja configurado corretamente. Um comando de compilação básico seria:

    ```bash
    gcc -o bomberman main.c game.c map.c menu.c bomb.c inimigo.c jogador.c state_game.c -I. -L. -lraylib -lopengl32 -lgdi32 -lwinmm -std=c99
    ```
    * **`-o bomberman`**: Nome do executável de saída.
    * **`main.c game.c ...`**: Lista de todos os arquivos `.c` do projeto.
    * **`-I.`**: Inclui o diretório atual para encontrar os arquivos `.h`.
    * **`-L.`**: Inclui o diretório atual para encontrar bibliotecas (se houver alguma customizada aqui).
    * **`-lraylib -lopengl32 -lgdi32 -lwinmm`**: Flags para linkar com a Raylib e suas dependências (pode variar ligeiramente dependendo do OS).
    * **`-std=c99`**: Garante compatibilidade com o padrão C99.

4.  **Execute o jogo**:
    ```bash
    ./bomberman
    ```

## 🎮 Controles

* **Mover**: `W`, `A`, `S`, `D` ou `Setas Direcionais`.
* **Plantar Bomba**: `B`.
* **Abrir Menu**: `TAB` (durante o jogo).
* **Navegar no Menu**: `Setas Cima/Baixo` ou `W`/`S`.
* **Selecionar/Confirmar no Menu**: `ENTER`.
* **Atalhos do Menu**: `N` (Novo Jogo), `C` (Carregar Jogo), `S` (Salvar Jogo), `Q` (Sair), `V` (Voltar ao Jogo).

## 📄 Estrutura do Código

O projeto está modularizado para facilitar a organização e manutenção:

* `main.c`: Ponto de entrada do jogo, contém o loop principal.
* `estruturas.h`: Define todas as estruturas de dados e constantes globais do jogo (Jogador, Inimigo, Bomba, Game, etc.).
* `game.h`/`game.c`: Implementa a lógica principal do jogo, atualização de estado, colisões e desenho dos elementos.
* `map.h`/`map.c`: Lida com o carregamento dos mapas a partir de arquivos de texto e a validação de posições.
* `jogador.h`/`jogador.c`: Contém a lógica e desenho do jogador, incluindo movimento e coleta de chaves.
* `inimigo.h`/`inimigo.c`: Gerencia a inicialização e a atualização do comportamento dos inimigos.
* `bomb.h`/`bomb.c`: Controla a mecânica das bombas, incluindo plantio, temporizador e efeitos da explosão.
* `menu.h`/`menu.c`: Responsável por desenhar e gerenciar as interações no menu principal, Game Over e tela de vitória.
* `state_game.h`/`state_game.c`: Funções para salvar e carregar o estado atual do jogo.

## 🤝 Contribuição

Contribuições são bem-vindas! Se você tiver sugestões, melhorias ou encontrar bugs, sinta-se à vontade para abrir uma *issue* ou enviar um *pull request*.

## 👨‍💻 Desenvolvedores

* [Kaio Jesus]
* [Daniel]
* [Emily]
* Izabel

