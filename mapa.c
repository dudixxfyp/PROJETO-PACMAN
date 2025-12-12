#include "mapa.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"


Mapa *carregarMapa(const char *arquivo){
    if (!arquivo) return NULL; // 
    FILE *f = fopen(arquivo, "r");
    if (!f) { // Mensagem de erro caso o arquivo não possa ser aberto
        fprintf(stderr,"Erro ao abrir o arquivo do mapa. %s\n", arquivo); 
        return NULL;
    }


    Mapa *m = malloc (sizeof(Mapa)); // alocar estrutura do mapa dinamicamente
    if (!m) { // erro na alocação
        fclose(f);
        return NULL;
    }

    m->linhas = 20;
    m->colunas = 40;

    m->celulas = malloc(sizeof(char*) * m->linhas); // alocar dinacamente matriz de celulas
    if (!m -> celulas) {
        free(m);
        fclose(f);
        return NULL;
    }

    for (int i = 0; i < m->linhas; i++) {
        m->celulas[i] = malloc(sizeof(char) * (m->colunas + 1)); // copiar as linhas lidas para a matriz do mapa
        if(!m->celulas[i]){

            for(int k = 0; k < 1; k++){
                free(m->celulas[k]);
            }
            free(m->celulas);
            free(m);
            fclose(f);
            return NULL;
        }

        if (fgets(m->celulas[i], m->colunas + 2, f)) {
            // Remover caracteres de nova linha
            size_t len = strlen(m->celulas[i]);
            while (len > 0 && (m->celulas[i][len-1] == '\n' || m->celulas[i][len-1] == '\r')) {
                m->celulas[i][--len] = '\0';
            }
            // Preencher com espaços se necessário
            while (len < m->colunas) {
                m->celulas[i][len++] = ' ';
            }
            m->celulas[i][m->colunas] = '\0';
        }
    }
    return m;
}

// liberarMapa: liberar a memoria alocada para o mapa
void liberarMapa(Mapa *mapa){
    if (!mapa) return; // retorna se o mapa for NULL
    if (mapa->celulas) {
        for (int i = 0; i < mapa->linhas; i++) {
            free(mapa->celulas[i]);
        }
        free(mapa->celulas);
    } free(mapa);
}

// inicializarJogo: monta o nome mapa<N>.txt e chama carregarMapa
