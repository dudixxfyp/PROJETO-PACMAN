#ifndef FANTASMA_H
#define FANTASMA_H

#include <stdio.h>
#include "raylib.h"

typedef enum TipoFantasma{
    BLINKY,
    PINKY,
    INKY,
    CLYDE
} TipoFantasma;

typedef enum CondicaoFantasma{
    VIVOf,
    VULNERAVELf,
    MORTOf
} CondicaoFantasma;

typedef struct {
    Vector2 posicao;
    int linha;
    int coluna;
    int vivo;
    int vulneravel;
    float tempoVulneravel;
    float velocidade;
    TipoFantasma tipo;
    CondicaoFantasma condicao;

    Texture2D spritePadrao;
    Texture2D spriteVulneravel;
    Texture2D spriteOlhos;

} Fantasma;

typedef struct NoFantasma {
    Fantasma f;
    struct NoFantasma *prox;
} NoFantasma;

typedef struct {
    NoFantasma *inicio;
} ListaFantasmas;

// Funções da lista
void inicializarListaFantasmas(ListaFantasmas *L);
void inserirFantasma(ListaFantasmas *L, Fantasma f);
void liberarListaFantasmas(ListaFantasmas *L);

// Comportamento dos fantasmas
void moverFantasmas(ListaFantasmas *L, char **mapa, int linhas, int colunas, int tamTile);
void ativarModoVulneravel(ListaFantasmas *L);
void atualizarTemporizadorVulneravel(ListaFantasmas *L, float deltaTime);
void resetarPosicaoFantasma(Fantasma* f, int linha, int coluna);

// Checagem de colisões 
int verificarColisoes(ListaFantasmas *L, int pacLinha, int pacColuna);
int verificarColisoesPorDistancia(ListaFantasmas *L, Vector2 posPacman, float raio);

// Gráficos 
void desenharFantasma(ListaFantasmas *lf, int tamTile);
Fantasma criarFantasma(TipoFantasma tipo, int linha, int coluna, int tamTile, char *spriteNormal, char *spriteVulneravel, char *spriteOlhos);

void limparListaFantasmas(ListaFantasmas *lista);

#endif

