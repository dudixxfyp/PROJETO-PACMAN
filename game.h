#ifndef GAME_H
#define GAME_H

// Este header é uma "ponte" entre o código da Pessoa 1 (pacman.c / mapa.c) e o menu (Pessoa 4).
// Ele deve ser incluído tanto em pacman.c quanto em menu.c (ou basta em pacman.c definir
// as variáveis e em menu.c declará-las como extern).

#ifndef HERO_GHOST_TYPES
#define HERO_GHOST_TYPES

#ifndef MAX_GHOSTS
#define MAX_GHOSTS 4
#endif




typedef struct _TAG_hero {
    int pos[2];      // pos[0] = x (coluna), pos[1] = y (linha)
    int direction;
    char face;
} hero;

typedef struct _TAG_ghost {
    int pos[2];
} ghost;
#endif

// constantes do mapa (iguais às do pacman.c)
#ifndef H
#define H 25
#endif
#ifndef W
#define W 42
#endif


extern hero pacman;        // definido em pacman.c
extern ghost blinky;       // definido em pacman.c (ou outros ghosts)
extern int game_score;     // definido em pacman.c
extern char map[H][W];     // definido em pacman.c
extern ghost ghosts[MAX_GHOSTS];        // novo: vetor de fantasmas


extern int numFantasmas;
extern int nivelAtual;
extern float tempoDesdeInicio;
extern int pelletsRemaining;
extern int jogoPausado;
extern int resetar;

#endif // GAME_H