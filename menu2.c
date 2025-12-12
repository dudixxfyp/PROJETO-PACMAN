#include "menu2.h"
#include "raylib.h"
#include "pacman.h"

int salvarJogo(Pacman *p, ListaFantasmas *fantasmas, char maze[MAZE_HEIGHT][MAZE_WIDTH], int nivel, float tempoJogo){
    dadosJogo jogo;

    jogo.pos_pacmanX = p->pos.x;
    jogo.pos_pacmanY = p->pos.y;
    jogo.vidas = p->vidas;
    jogo.pontos = p->pontos;
    jogo.direcao = (int)p->direcao; //converte pra inteiro para colocar no arquivo

    jogo.nivel = nivel;
    jogo.tempoJogo = tempoJogo;

    for (int i = 0; i < MAZE_HEIGHT; i++){
        for (int j = 0; j < MAZE_WIDTH; j++){
            jogo.maze[i][j] = maze[i][j];
        }
    }

    jogo.numeroFantasmas = 0;
    NoFantasma *atual = fantasmas->inicio;
    int index = 0;

    while (atual && index < 4)
    {
        jogo.fantasmas[index].tipo = atual->f.tipo;
        jogo.fantasmas[index].linha = atual->f.linha;
        jogo.fantasmas[index].coluna = atual->f.coluna;
        jogo.fantasmas[index].vivo = atual->f.vivo;
        jogo.fantasmas[index].vulneravel = atual->f.vulneravel;
        jogo.fantasmas[index].tempoVulneravel = atual->f.tempoVulneravel;
        jogo.fantasmas[index].condicao_f = atual->f.condicao;

        jogo.numeroFantasmas++;
        index++;
        atual = atual->prox;
    }

    FILE *f; 
    if(!(f = fopen("save.txt", "w"))){
        printf("erro na criacao do arquivo");
        return 0;
    }

    size_t escrito = fwrite(&jogo, sizeof(dadosJogo), 1, f);
    fclose(f);

    return 1;
    
}

int carregarJogo(Pacman *p, ListaFantasmas *fantasmas, char maze[MAZE_HEIGHT][MAZE_WIDTH], int *nivel, float *tempoJogo){

    FILE *f;
    if(!(f = fopen("save.txt", "r"))){
        printf("erro");
    }

    dadosJogo jogo;
    size_t lido = fread(&jogo, sizeof(jogo), 1, f);
    fclose(f);

    p->pos.x = jogo.pos_pacmanX;
    p->pos.y = jogo.pos_pacmanY;
    p->vidas = jogo.vidas;
    p->pontos = jogo.pontos;
    p->direcao = (DIRECAO)jogo.direcao;
    p->condicao = VIVO;
    p->movimento = false;

    *nivel = jogo.nivel;
    *tempoJogo = jogo.tempoJogo;

    for (int i = 0; i < MAZE_HEIGHT; i++){
        for (int j = 0; j < MAZE_WIDTH; j++){
            maze[i][j] = jogo.maze[i][j];
        }
    }
    
    liberarListaFantasmas(fantasmas);
    inicializarListaFantasmas(fantasmas);

    char *sprites[4][3] = {

        {"resources/sprites/ghosts/blinky.png", 
         "resources/sprites/ghosts/frightened.png", 
         "resources/sprites/ghosts/eyes.png"},
        {"resources/sprites/ghosts/pinky.png", 
         "resources/sprites/ghosts/frightened.png", 
         "resources/sprites/ghosts/eyes.png"},
        {"resources/sprites/ghosts/inky.png", 
         "resources/sprites/ghosts/frightened.png", 
         "resources/sprites/ghosts/eyes.png"},
        {"resources/sprites/ghosts/clyde.png", 
         "resources/sprites/ghosts/frightened.png", 
         "resources/sprites/ghosts/eyes.png"}

    };

    for (int i = 0; i < jogo.numeroFantasmas; i++){
        Fantasma f = criarFantasma(jogo.fantasmas[i].tipo, jogo.fantasmas[i].linha, jogo.fantasmas[i].coluna, 
            TILE, (char*)sprites[jogo.fantasmas[i].tipo][0], (char*)sprites[jogo.fantasmas[i].tipo][1], (char*)sprites[jogo.fantasmas[i].tipo][2]);

        f.vivo = jogo.fantasmas[i].vivo;
        f.vulneravel = jogo.fantasmas[i].vulneravel;
        f.tempoVulneravel = jogo.fantasmas[i].tempoVulneravel;
        f.condicao = jogo.fantasmas[i].condicao_f;


        inserirFantasma(fantasmas, f);
    }


    return 1;
}