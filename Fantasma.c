#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "Fantasma.h"

void inicializarListaFantasmas(ListaFantasmas *L) {
    L->inicio = NULL;
}

void liberarListaFantasmas(ListaFantasmas *L) {
    NoFantasma *atual = L->inicio;
    while (atual) {
        NoFantasma *prox = atual->prox;
        free(atual);
        atual = prox;
    }
    L->inicio = NULL;
}

void inserirFantasma(ListaFantasmas *L, Fantasma f) {
    NoFantasma *novo = malloc(sizeof(NoFantasma));
    if (!novo) return;

    novo->f = f;
    novo->prox = L->inicio;
    L->inicio = novo;
}

int podeMover(char **mapa, int l, int c, int maxL, int maxC) {
    if (l < 0 || l >= maxL) return 0;
    if (c < 0 || c >= maxC) return 0;
    return (mapa[l][c] != '#');
}

void moverFantasmas(ListaFantasmas *L, char **mapa, int linhas, int colunas, int tamTile) {

    int movL[4] = {-1, 1, 0, 0};
    int movC[4] = {0, 0, -1, 1};

    for (NoFantasma *atual = L->inicio; atual; atual = atual->prox) {

        if (!atual->f.vivo) continue;

        int movido = 0;
        int tentativas = 0;
        // Fantasma tenta até achar uma direção válida
        while (!movido && tentativas < 10) {

            int dir = rand() % 4;
            int nl = atual->f.linha + movL[dir];
            int nc = atual->f.coluna + movC[dir];

            if (podeMover(mapa, nl, nc, linhas, colunas)) {

                atual->f.linha = nl;
                atual->f.coluna = nc;

                
                atual->f.posicao.x = nc * tamTile + tamTile / 2.0f;
                atual->f.posicao.y = nl * tamTile + tamTile / 2.0f;

                movido = 1;
            }
            tentativas++;
        }
    }
}

void ativarModoVulneravel(ListaFantasmas *L) {
    for (NoFantasma *atual = L->inicio; atual; atual = atual->prox) {
        if (atual->f.vivo) {
            atual->f.vulneravel = 1;
            atual->f.tempoVulneravel = 8.0f;
            atual->f.condicao = VULNERAVELf;
        }
    }
}

void atualizarTemporizadorVulneravel(ListaFantasmas *L, float deltaTime) {
    for (NoFantasma *atual = L->inicio; atual; atual = atual->prox) {

        if (atual->f.vulneravel) {
            atual->f.tempoVulneravel -= deltaTime;

            if (atual->f.tempoVulneravel <= 0) {
                atual->f.vulneravel = 0;
                atual->f.condicao = VIVOf;
            }
        }
    }
}

int verificarColisoes(ListaFantasmas *L, int pacL, int pacC) {

    for (NoFantasma *atual = L->inicio; atual; atual = atual->prox) {

        if (!atual->f.vivo) continue;

        if (atual->f.linha == pacL && atual->f.coluna == pacC) {

            if (atual->f.vulneravel) {
                atual->f.vivo = 0;
                atual->f.condicao = MORTOf;
                return 2;
            }
            return 1;
        }
    }
    return 0;
}

int verificarColisoesPorDistancia(ListaFantasmas *L, Vector2 posPacman, float raio) {

    for (NoFantasma *atual = L->inicio; atual; atual = atual->prox) {

        if (!atual->f.vivo) continue;

        float dx = posPacman.x - atual->f.posicao.x;
        float dy = posPacman.y - atual->f.posicao.y;
        float distancia = sqrt(dx * dx + dy * dy);

        if (distancia < raio) {

            if (atual->f.vulneravel) {
                // Fantasma vulnerável foi comido - mata ele
                atual->f.vivo = 0;
                atual->f.condicao = MORTOf;
                atual->f.vulneravel = 0;
                return 2; // Pacman comeu o fantasma
            } else {
                // Pacman colidiu com fantasma normal
                // RESETAR APENAS ESTE FANTASMA
                switch(atual->f.tipo) {
                    case BLINKY:
                        resetarPosicaoFantasma(&atual->f, 14, 11);
                        break;
                    case PINKY:
                        resetarPosicaoFantasma(&atual->f, 14, 12);
                        break;
                    case INKY:
                        resetarPosicaoFantasma(&atual->f, 14, 13);
                        break;
                    case CLYDE:
                        resetarPosicaoFantasma(&atual->f, 14, 14);
                        break;
                }
                return 1; // Pacman foi atingido
            }
        }
    }
    return 0; // Sem colisão
}

Fantasma criarFantasma(TipoFantasma tipo, int linha, int coluna, int tamTile,
                       char *spriteNormal, char *spriteVulneravel, char *spriteOlhos) {

    Fantasma f;

    f.tipo = tipo;
    f.linha = linha;
    f.coluna = coluna;
    f.posicao.x = coluna * tamTile + tamTile / 2.0f;
    f.posicao.y = linha * tamTile + tamTile / 2.0f;

    f.vivo = 1;
    f.vulneravel = 0;
    f.tempoVulneravel = 0.0f;
    f.velocidade = 60.0f;
    f.condicao = VIVOf;

    f.spritePadrao = LoadTexture(spriteNormal);
    f.spriteVulneravel = LoadTexture(spriteVulneravel);
    f.spriteOlhos = LoadTexture(spriteOlhos);

    return f;
}

void desenharFantasma(ListaFantasmas *lf, int tamTile) {

    for (NoFantasma *atual = lf->inicio; atual; atual = atual->prox) {

        Texture2D sprite;

        if (atual->f.condicao == MORTOf)
            sprite = atual->f.spriteOlhos;
        else if (atual->f.vulneravel)
            sprite = atual->f.spriteVulneravel;
        else
            sprite = atual->f.spritePadrao;

        if (sprite.id != 0) {
            Rectangle sourceRec = {0, 0, (float)sprite.width, (float)sprite.height};
            Rectangle destRec = {
                atual->f.posicao.x,
                atual->f.posicao.y,
                (float)tamTile,
                (float)tamTile
            };
            Vector2 origin = {tamTile / 2.0f, tamTile / 2.0f};
            DrawTexturePro(sprite, sourceRec, destRec, origin, 0.0f, WHITE);
        }
    }
}

void resetarPosicaoFantasma(Fantasma* f, int linha, int coluna) {
    f->linha = linha;
    f->coluna = coluna;
    f->posicao.x = coluna * 20 + 20 / 2.0f;
    f->posicao.y = linha * 20 + 20 / 2.0f;
    f->vivo = 1;
    f->vulneravel = 0;
    f->condicao = VIVOf;
}

void limparListaFantasmas(ListaFantasmas *lista) {
    NoFantasma *atual = lista->inicio;
    while (atual) {
        NoFantasma *prox = atual->prox;
        free(atual);
        atual = prox;
    }
    lista->inicio = NULL;
}