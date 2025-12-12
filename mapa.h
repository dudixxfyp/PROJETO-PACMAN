#ifndef mapa_h
#define mapa_h

typedef struct {
    int linhas;
    int colunas;
    char **celulas; // matriz que representa as celulas do mapa, celulas[linhas][colunas]
} Mapa;

typedef struct {
    int linha; // representa o indice do y(linha) no mapa
    int coluna; // representa o indice do x(coluna) no mapa
} Posicao;

// carregar o mapa a partir de um arquivo de texto. Com tamanhos de até 20x40 LxC
// Retorna um ponteiro para o mapa carregado ou NULL em caso de erro.
Mapa* carregarMapa(const char* arquivo);

//liberar a memoria alocada para o mapa
void liberarMapa(Mapa* mapa);

// Desenhar o mapa na tela usando ncurses.
// pacman e os fantasmas(blinky, pinky, inky, clyde) são posições no estilo(linha, coluna).
// vidas, pontuacao e nivel são informações adicionais a serem exibidas na tela.



#endif // mapa_h
