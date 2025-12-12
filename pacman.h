#ifndef PACMAN_H
#define PACMAN_H

#include "raylib.h"
#include <stdbool.h>
#include "mapa.h" 

typedef enum DIRECAO{
    CIMA,
    BAIXO,
    DIREITO, 
    ESQUERDA,
    PARADO
} DIRECAO;

typedef enum CONDICAO{
    VIVO,
    MORTO
} CONDICAO;

typedef struct Pacman
{
    Vector2 pos;
    int vidas;
    int pontos;
    float velocidade;

    Texture2D andarCima;
    Texture2D andarCima2;
    Texture2D andarBaixo;
    Texture2D andarBaixo2;
    Texture2D andarEsquerda;
    Texture2D andarEsquerda2;
    Texture2D andarDireita;
    Texture2D andarDireita2;
    Texture2D spritesMorte[4];
    Texture2D spriteParado;
    
    int frames;
    bool passoAberto;
    float temporizadoBoca;
    float rotacao;
    DIRECAO direcao;
    bool movimento;
    CONDICAO condicao;
    int frameAtualMorte;
    float temporizadorMorte;


}Pacman;

Pacman criaPacman(char *andarCima, char *andarCima2, char *andarEsquerda, char *andarEsquerda2, char *andarBaixo, char *andarBaixo2, char *andarDireita, char *andarDireita2, char *parado,Vector2 posInicial);
void desenhaPacman(Pacman *p);
void attPacman(Pacman *p, float dt);
void resetarPac(Pacman *p, Vector2 posInicial);


#endif