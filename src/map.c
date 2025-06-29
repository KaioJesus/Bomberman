#include "raylib.h"

// definindo o numero máximo de inimigos e chaves
#define MAX_ENEMIES 5
#define KEYS 5

typedef struct RECTANGLE {
    int x;
    int y;
    int width;
    int height;
    char color[10];
} RECTANGLE;

// estrutura definida com os dados do mapa
typedef struct {
    char **grid;
    int currentLevel;
    int totalKeys;
} GameMap;
 
// tipos de bloco
enum BLOCKS { CAIXA_COM_CHAVE, PAREDE_INDESTRUTIVEL, PAREDE_DESTRUTIVEL}

int main(){

    const int screenWidth = 1280;
    const int screenHeight = 600;
    const int mapHeight = 25;
    const int mapWidth = 60;
    const int blockSize = 20
    const int areaInfoSize = 100;


    //Inicializa a Janela gráfica
    InitWindow(screenWidth, screenHeight, "Bomberman");

    // Define os quadros por segundo
    SetTargetFPS(60)

    // Retorna verdadeiro casa a tecla senha pressionada
    IsKeyPressed()

    //Define o início do contexto do desenho gráfico
    BeginDrawing()
    
    //Define o fim do desenho
    EndDrawing()

    //Desenha um retangulo na posição informada
    DrawRectangle();

    //Fecha a Janela
    CloseWindow()

    //
    IsKeyDown();
}

dawMap(
    // Se a Janela for fechada ou o ESC pressionado, retorna true
    while(!WindowShouldClose){
        BeginDrawing();

        EndDrawing();
    }

    CloseWindow();
)

openTextFile(){
    FILE *parq;
    parq=fopen("mapa1.txt","r");

    if(parq==NULL){
        printf("Erro na abertura do arquivo");
        return -1;
    }

    i=0;
    while (!feof(parq)){
        i++;
    }
    fclose(parq);
}