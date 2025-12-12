#ifndef MENU2_H
#define MENU2_H

#include "pacman.h"
#include "Fantasma.h"
#include "raylib.h"

#define MAZE_HEIGHT 20
#define MAZE_WIDTH 40
#define TILE 20

typedef struct 
{
    TipoFantasma tipo;
    int linha;
    int coluna;
    int vivo;
    int vulneravel;
    float tempoVulneravel;
    CondicaoFantasma condicao_f;

}dadosFantasma;

typedef struct
{
    float pos_pacmanX;
    float pos_pacmanY;
    int vidas;
    int pontos;
    int direcao;
    int nivel;
    float tempoJogo;
    char maze[MAZE_HEIGHT][MAZE_WIDTH];
    int numeroFantasmas;
    dadosFantasma fantasmas[4];

}dadosJogo;


int salvarJogo(Pacman *pac, ListaFantasmas *fantasmas, char mapa[MAZE_HEIGHT][MAZE_WIDTH], int nivel, float tempoJogo);
int carregarJogo(Pacman *p, ListaFantasmas *fantasmas, char maze[MAZE_HEIGHT][MAZE_WIDTH], int *nivel, float *tempoJogo);

#endif