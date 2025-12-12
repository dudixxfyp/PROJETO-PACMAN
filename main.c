#include "raylib.h"
#include <stdio.h>
#include "pacman.h"
#include "Fantasma.h"
#include <math.h>
#include "menu2.h"

#define TILE 20
#define MAZE_WIDTH 40
#define MAZE_HEIGHT 20

float temporest = 120.0f;

typedef enum GameScreen{
    INICIAL,
    MENU,
    DESCRICAO,
    CHICO,
    DIFICULDADE,
    SELECAO,
    SALVAR,
    CARREGAR,
    GAMEPLAY,
    FIM_DE_JOGO,
    REGRAS,
    PAUSA,
    VITORIA
} GameScreen;

typedef struct {
    Vector2 posicaoPacman;
    Vector2 posicoesFantasmas[10];
    int numFantasmas;
    Vector2 portais[4];
    int numPortais;
    bool encontrouPacman;
} InfoMapa;

GameScreen telaAtual = INICIAL;

const char initialmaze[MAZE_HEIGHT][MAZE_WIDTH] = {
   "########################################",
   "#..................##..................#",
   "#.####.#####.##.#####.####.#####.##.##.#",
   "#O####.#####.##.#####.####.#####.##.##O#",
   "#......................................#",
   "#.####.##.########.##.########.##.####.#",
   "#......##....##....##....##....##......#",
   "######.#####.##.#####.#####.##.#####.###",
   "T     ..##   F      F    ##.#      ....T",
   "######.##.##############.##.############",
   "#..........##      P     ##............#",
   "######.##.##############.##.############",
   "..   ..#.   F      F    ...............#",
   "######.##.##############.##.############",
   "#............##....##..................#",
   "#.####.#####.##.##.#####.####.#####.##.#",
   "#...##................##...............#",
   "###.##.##.########.##.##.###.##.########",
   "#O....................................O#",
   "########################################"
};

char maze[MAZE_HEIGHT][MAZE_WIDTH];

void drawMaze(char maze[MAZE_HEIGHT][MAZE_WIDTH]);
void checarColisao(Vector2 pos, char maze[MAZE_HEIGHT][MAZE_WIDTH], int *pontos, int *comeu, Sound sound);
void updatePosition(Vector2 *pos, DIRECAO *direction, DIRECAO *buffer_direction, char maze[MAZE_HEIGHT][MAZE_WIDTH], char Char);
int avoidRotation(Vector2 *pos, DIRECAO *direction, DIRECAO *buffer_direction, char maze[MAZE_HEIGHT][MAZE_WIDTH], char Char);
void desenharPlacar(int pontos);
void desenharVidas(int vidas);
InfoMapa processarMapa(char maze[MAZE_HEIGHT][MAZE_WIDTH]);
void desenharTempo(int minutos, int segundos);
void desenharNivel(int mapaAtual);

float SPEED = 2.0f;

int contarPelletsRestantes(char maze[MAZE_HEIGHT][MAZE_WIDTH]) {
    int count = 0;
    for (int i = 0; i < MAZE_HEIGHT; i++) {
        for (int j = 0; j < MAZE_WIDTH; j++) {
            if (maze[i][j] == '.' || maze[i][j] == 'O') {
                count++;
            }
        }
    }
    return count;
}

int main(void){
    const int screenWidth = MAZE_WIDTH * 20;
    const int screenHeight = MAZE_HEIGHT * 20;

    Mapa *mapaCarregado = carregarMapa("mapa.txt");
    Mapa *mapaCarregado2 = carregarMapa("mapa2.txt");

    if (!mapaCarregado) {
        for (int i = 0; i < MAZE_HEIGHT; i++) {
            for (int j = 0; j < MAZE_WIDTH; j++) {
                maze[i][j] = initialmaze[i][j];
            }
        }
    } else {
        for (int i = 0; i < MAZE_HEIGHT; i++) {
            for (int j = 0; j < MAZE_WIDTH; j++) {
                maze[i][j] = mapaCarregado->celulas[i][j];
            }
        }
        liberarMapa(mapaCarregado);
    }
    
    InfoMapa infoMapa = processarMapa(maze);
    
    InitWindow(MAZE_WIDTH * 20, MAZE_HEIGHT * 23, "PAC - MAN");
    SetTargetFPS(60);
    InitAudioDevice();

    ListaFantasmas fantasmas;
    inicializarListaFantasmas(&fantasmas);

    Vector2 pos;
    if (infoMapa.encontrouPacman) {
        pos = infoMapa.posicaoPacman;
    } else {
        pos = (Vector2){(float)screenWidth / 2 + 10, (float)(screenHeight / 2 + 40)};
    }
    
    Vector2 posInici = pos;

    Texture2D blinkyload = LoadTexture("resources/sprites/ghosts/blinky.png");
    Texture2D inkyload = LoadTexture("resources/sprites/ghosts/inky.png");
    Texture2D pinkyload = LoadTexture("resources/sprites/ghosts/pinky.png");
    Texture2D clydeload = LoadTexture("resources/sprites/ghosts/clyde.png");
    Texture2D pacload = LoadTexture("resources/sprites/pac/baixo1.png");
    Texture2D paclogo = LoadTexture("resources/pacmantext.png");
    Texture2D chicopombo = LoadTexture("resources/sprites/pac/chicopombo.png");
    Texture2D raposadif = LoadTexture("resources/sprites/ghosts/raposadif.png");
    Texture2D chicoman = LoadTexture("resources/sprites/pac/MENU.png");
    Texture2D ovo = LoadTexture("resources/ovo.png");
    Texture2D vitoria = LoadTexture("resources/gamewin.png");

    Sound intro = LoadSound("resources/Audio/take_on_me_small.mp3");
    Sound sound1 = LoadSound("resources/Audio/munch_1.wav");
    Sound siren = LoadSound("resources/Audio/siren_2.wav"); 
    Sound Startmusic = LoadSound("resources/Audio/start.mp3");
    Sound sup_siren = LoadSound("resources/Audio/sup_pellet.mp3");
    Sound death = LoadSound("resources/Audio/death.mp3");


    Pacman pac = criaPacman("resources/sprites/pac/cima1.png", "resources/sprites/pac/cima2.png",
        "resources/sprites/pac/esquerda1.png", "resources/sprites/pac/esquerda2.png",
        "resources/sprites/pac/baixo1.png", "resources/sprites/pac/baixo2.png",
        "resources/sprites/pac/direita1.png", "resources/sprites/pac/direita2.png",
        "resources/sprites/pac/parado.png", pos);
    
    TipoFantasma tipos[] = {BLINKY, PINKY, INKY, CLYDE};
    char* spritesNormais[] = {
        "resources/sprites/ghosts/blinky.png",
        "resources/sprites/ghosts/pinky.png",
        "resources/sprites/ghosts/inky.png",
        "resources/sprites/ghosts/clyde.png"
    };

    if (infoMapa.numFantasmas > 0) {
        for (int i = 0; i < infoMapa.numFantasmas && i < 4; i++) {
            int linha = (int)infoMapa.posicoesFantasmas[i].y;
            int coluna = (int)infoMapa.posicoesFantasmas[i].x;
            
            Fantasma f = criarFantasma(tipos[i], linha, coluna, TILE, spritesNormais[i],
                "resources/sprites/ghosts/frightened.png", "resources/sprites/ghosts/eyes.png");
            inserirFantasma(&fantasmas, f);
        }
    } else {
        Fantasma blinky = criarFantasma(BLINKY, 8, 17, TILE, "resources/sprites/ghosts/blinky.png", 
            "resources/sprites/ghosts/frightened.png", "resources/sprites/ghosts/eyes.png");
        inserirFantasma(&fantasmas, blinky);

        Fantasma pinky = criarFantasma(PINKY, 8, 23, TILE, "resources/sprites/ghosts/pinky.png", 
            "resources/sprites/ghosts/frightened.png", "resources/sprites/ghosts/eyes.png");
        inserirFantasma(&fantasmas, pinky);

        Fantasma inky = criarFantasma(INKY, 12, 17, TILE, "resources/sprites/ghosts/inky.png", 
            "resources/sprites/ghosts/frightened.png", "resources/sprites/ghosts/eyes.png");
        inserirFantasma(&fantasmas, inky);

        Fantasma clyde = criarFantasma(CLYDE, 12, 23, TILE, "resources/sprites/ghosts/clyde.png", 
            "resources/sprites/ghosts/frightened.png", "resources/sprites/ghosts/eyes.png");
        inserirFantasma(&fantasmas, clyde);
    }
    
    float tempoEspera = 0.0f;
    int displaycounter = 0;
    int x = 0;
    int nivel = 1;
    int mapaAtual = 1;
    pac.pontos = 0;
    DIRECAO direcao = PARADO;
    DIRECAO bufferDirecao = PARADO;
    int comeuSuper = 0;
    float velocidadeFantasma;
    bool reiniciar = false;
    bool gameOver = false;
    int tempoVulneravel = 9;
    int contagemPellets;
    int contagemSuperPellets;
    bool jogoPausado = false;
    float tempoJogo = 0.0f;
    bool carregJogo = false;
    bool salvJogo = false;
    float tempoMovimentoFantasmas = 0.0f;
    float intervaloMovimentoFantasmas = 0.05f;
    bool musicaMenuTocando = false;
    bool musicaGameplayTocando = false;
    bool musicaVulneravelTocando = false;
    bool somMorteTocado = false;
    bool somStartTocado = false;    

    while (!WindowShouldClose())
    {   
        float dt = GetFrameTime();
        float raioColisao = TILE * 1.0f; 
        char Char1 = '#';

        switch (telaAtual)
        {
            case INICIAL:{
                displaycounter++;
                if (displaycounter%30==0) {
                    x++;
                }
                if(IsKeyPressed(KEY_ENTER)){
                    telaAtual = MENU;
                }
            } break;
            
            case MENU:{
                if(IsKeyPressed(KEY_ENTER)){
                    StopSound(intro);
                    musicaMenuTocando = false;
                    telaAtual = DESCRICAO;
                }
                if(IsKeyPressed(KEY_C)){
                    StopSound(intro);
                    musicaMenuTocando = false;
                    if(carregarJogo(&pac, &fantasmas, maze, &nivel, &tempoJogo)){
                        telaAtual = GAMEPLAY;
                        direcao = pac.direcao;
                        bufferDirecao = PARADO;
                    }
                }
                if(IsKeyPressed(KEY_ESCAPE)){
                   gameOver = true;
                }
            } break;
            
            case DESCRICAO:{
                displaycounter++;
                if (displaycounter%30==0) {
                    x++;
                }
                if(IsKeyPressed(KEY_ENTER)){
                    StopSound(intro);
                    musicaMenuTocando = false;
                    telaAtual = CHICO;
                }
            }break;
            case CHICO:{
                displaycounter++;
                if (displaycounter%30==0) {
                    x++;
                };
                if(IsKeyPressed(KEY_ENTER)){
                    telaAtual = REGRAS;
                };
            }break;
            case REGRAS:{
                displaycounter++;
                if (displaycounter%30==0) {
                    x++;
                }
                if(IsKeyPressed(KEY_ENTER)){
                    telaAtual = DIFICULDADE;
                }
            }break;
            
            case DIFICULDADE:{
                if (IsKeyPressed(KEY_ONE))
                {
                    nivel = 1;
                    intervaloMovimentoFantasmas = 1.0f;
                    temporest = 160.0f;
                    PlaySound(Startmusic);
                    telaAtual = GAMEPLAY;
                }
                if (IsKeyPressed(KEY_TWO))
                {
                    nivel = 2;
                    intervaloMovimentoFantasmas = 0.2f;
                    temporest = 120.0f;
                    PlaySound(Startmusic);
                    telaAtual = GAMEPLAY;
                }
                if (IsKeyPressed(KEY_THREE))
                {
                    nivel = 3;
                    intervaloMovimentoFantasmas = 0.09f;
                    temporest = 90.0f;
                    PlaySound(Startmusic);
                    telaAtual = GAMEPLAY;
                }
            } break;
            
            case GAMEPLAY:{
                if(IsKeyPressed(KEY_M)){
                    StopSound(siren);
                    musicaGameplayTocando = false;
                    musicaVulneravelTocando = false;
                    telaAtual = PAUSA;
                    jogoPausado = true;
                }
                
                // ========== CHEATS SECRETOS ==========
                // CTRL + SHIFT + 2 = Ir para mapa 2
                if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_TWO)){
                    Mapa *mapa2 = carregarMapa("mapa2.txt");
                    if(mapa2){
                        for (int i = 0; i < MAZE_HEIGHT; i++){
                            for (int j = 0; j < MAZE_WIDTH; j++){
                                maze[i][j] = mapa2->celulas[i][j];
                            }
                        }
                        liberarMapa(mapa2);
                        
                        InfoMapa novoMapa = processarMapa(maze);
                        
                        if(novoMapa.encontrouPacman){
                            pac.pos = novoMapa.posicaoPacman;
                            posInici = novoMapa.posicaoPacman;
                        }
                        
                        limparListaFantasmas(&fantasmas);
                        inicializarListaFantasmas(&fantasmas);
                        
                        if(novoMapa.numFantasmas > 0){
                            for(int i = 0; i < novoMapa.numFantasmas && i < 4; i++){
                                Fantasma f = criarFantasma(
                                    tipos[i],
                                    (int)novoMapa.posicoesFantasmas[i].y,
                                    (int)novoMapa.posicoesFantasmas[i].x,
                                    TILE,
                                    spritesNormais[i],
                                    "resources/sprites/ghosts/frightened.png",
                                    "resources/sprites/ghosts/eyes.png"
                                );
                                inserirFantasma(&fantasmas, f);
                            }
                        }
                        
                        mapaAtual = 2;
                        nivel = 2;
                        temporest = 300.0f;
                        tempoJogo = 0.0f;
                        direcao = PARADO;
                        bufferDirecao = PARADO;
                    }
                }
                
                // CTRL + SHIFT + 1 = Voltar para mapa 1
                if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_ONE)){
                    for (int i = 0; i < MAZE_HEIGHT; i++){
                        for (int j = 0; j < MAZE_WIDTH; j++){
                            maze[i][j] = initialmaze[i][j];
                        }
                    }
                    
                    InfoMapa novoMapa = processarMapa(maze);
                    
                    if(novoMapa.encontrouPacman){
                        pac.pos = novoMapa.posicaoPacman;
                        posInici = novoMapa.posicaoPacman;
                    }
                    
                    limparListaFantasmas(&fantasmas);
                    inicializarListaFantasmas(&fantasmas);
                    
                    if(novoMapa.numFantasmas > 0){
                        for(int i = 0; i < novoMapa.numFantasmas && i < 4; i++){
                            Fantasma f = criarFantasma(
                                tipos[i],
                                (int)novoMapa.posicoesFantasmas[i].y,
                                (int)novoMapa.posicoesFantasmas[i].x,
                                TILE,
                                spritesNormais[i],
                                "resources/sprites/ghosts/frightened.png",
                                "resources/sprites/ghosts/eyes.png"
                            );
                            inserirFantasma(&fantasmas, f);
                        }
                    }
                    
                    mapaAtual = 1;
                    nivel = 1;
                    temporest = 300.0f;
                    tempoJogo = 0.0f;
                    direcao = PARADO;
                    bufferDirecao = PARADO;
                }
                
                // CTRL + SHIFT + V = 99 vidas
                if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_V)){
                    pac.vidas = 99;
                }
                
                // CTRL + SHIFT + T = +1 minuto
                if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyDown(KEY_LEFT_SHIFT) && IsKeyPressed(KEY_T)){
                    temporest += 60.0f;
                }
                // =====================================
            }break;
            
            case FIM_DE_JOGO:{
                if(IsKeyPressed(KEY_R)){
                    reiniciar = true;
                    telaAtual = GAMEPLAY;
                }
                if(IsKeyPressed(KEY_N)){
                    gameOver = true;
                }
            }break;
            
            case PAUSA:{
                if(IsKeyPressed(KEY_C)){
                    telaAtual = GAMEPLAY;
                    jogoPausado = false;
                }
                if(IsKeyPressed(KEY_R)){
                    reiniciar = true;
                    telaAtual = GAMEPLAY;
                    jogoPausado = false;
                }
                if(IsKeyPressed(KEY_ESCAPE)){
                    CloseWindow();
                }
                if(IsKeyPressed(KEY_F)){
                    if(carregarJogo(&pac, &fantasmas, maze, &nivel, &tempoJogo)){
                        telaAtual = GAMEPLAY;
                        direcao = pac.direcao;
                        bufferDirecao = PARADO;
                        jogoPausado = false;
                    }
                }
                if(IsKeyPressed(KEY_S)){
                    if(salvarJogo(&pac, &fantasmas, maze, nivel, tempoJogo)){
                        DrawText("JOGO SALVO!", 200, 300, 30, GREEN);
                    }
                }
            }break;
            default:
                break;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        switch (telaAtual)
        {
            case INICIAL: {
                char* texto = "PRESSIONE ENTER";
                int larguraTexto = MeasureText(texto, 25);
                if (displaycounter > 60 && (x % 2 == 0)){
                    DrawText(texto,(GetScreenWidth() - larguraTexto) / 2, GetScreenHeight() / 2, 25, GRAY);
                }
            } break;
            
            case MENU:{
                DrawTextureEx(chicoman, (Vector2){140, 70}, 0.0f, 0.25f, WHITE);
                if(!musicaMenuTocando){
                    PlaySound(intro);
                    musicaMenuTocando = true;
                }
            }break;
            
            case DESCRICAO:{
                DrawText("PERSONAGEM / NOME", 130, 40, 25, GRAY);
                
                DrawText("   BLINKY", 210, 140, 25, RED);
                DrawTextureEx(blinkyload, (Vector2){130, 100}, 0.0f, 0.75f, WHITE);

                DrawText("   PINKY", 510, 140, 25, PINK);
                DrawTextureEx(pinkyload, (Vector2){430, 100}, 0.0f, 0.75f, WHITE);

                DrawText("   INKY", 210, 270, 25, BLUE);
                DrawTextureEx(inkyload, (Vector2){130, 240}, 0.0f, 0.75f, WHITE);

                DrawText("   CLYDE", 510, 270, 25, ORANGE);
                DrawTextureEx(clydeload, (Vector2){430, 240}, 0.0f, 0.75f, WHITE);

                if (displaycounter > 60 && (x % 2 == 0)){
                    DrawText("ENTER - CONTINUAR", 130, 390, 30, GRAY);
                };

                if(!musicaMenuTocando){
                    PlaySound(intro);
                    musicaMenuTocando = true;
                }
            } break;
            case CHICO:{
                DrawText("CHICO-MAN", 50, 50, 25, YELLOW);
                DrawTextureEx(pacload, (Vector2){170, 15}, 0.0f, 0.75f, WHITE);
                
                DrawText("Era uma vez, um pequeno pintinho que havia acabado de chocar.", 60, 110, 20, WHITE);
                DrawText("Chico não conhecia o mundo, ", 60, 140, 20, WHITE);
                DrawText("mas ao ver o céu pela primeira vez se encantou.", 60, 170, 20, WHITE);

                DrawText("Não com o azul, não com o branco,", 60, 200, 20, WHITE);
                DrawText("mas o amarelo que caía em sua direção.", 60, 230, 20, WHITE);
                DrawText("Milhos. Mais do que ele conseguia contar. Então comeu.", 60, 260, 20, WHITE);
                DrawText("Comeu até escurecer, até sua mãe ir embora, até as raposas chegarem.", 60, 290, 20, WHITE);

                DrawText("Apesar de não entender, seu corpo sabe que ele deve fugir.", 60, 320, 20, WHITE);
                DrawText("Mas como fugir sem antes comer todos os milhos?", 60, 350, 20, WHITE);
                DrawText("Há apenas uma solução possível:", 60, 380, 20, WHITE);
                DrawText("COMER E CORRER.", 60, 410, 20, YELLOW);

                if (displaycounter > 60 && (x % 2 == 0)){
                    DrawText("PRESSIONE ENTER PARA CONTINUAR", 180, 600, 22, LIGHTGRAY);
                }

            }break;
            case REGRAS:{
                DrawText("REGRAS", 50, 40, 40, RED);
                DrawText("Coma todos os milhos EM ATÉ 5 MINUTOS", 50, 140, 25, YELLOW);
                DrawTextureEx(ovo, (Vector2){340, 20}, 0.0f, 6.0f, WHITE);

                DrawText("BOA SORTE!", 50, 240, 40, RED);
                if (displaycounter > 60 && (x % 2 == 0)){
                    DrawText("ENTER - INICIAR JOGO", 50, 340, 30, GRAY);
                }
            }break;
            
            case DIFICULDADE:{
                DrawText("SELECIONE A DIFICULDADE:", 100, 80, 28, YELLOW);                    
                DrawText("1. FÁCIL", 120, 130, 28, YELLOW);
                DrawText("2. MÉDIO", 120, 170, 28, YELLOW);
                DrawText("3. DIFÍCIL", 120, 210, 28, YELLOW);
                DrawTextureEx(raposadif, (Vector2){340, 20}, 0.0f, 6.0f, WHITE);
            }break;
            
            case GAMEPLAY:{
                
                if(!musicaGameplayTocando && !jogoPausado){
                    PlaySound(siren);
                    musicaGameplayTocando = true;
                }

                if(reiniciar){
                    pac.vidas = 3;
                    pac.direcao = PARADO;
                    pac.pos = posInici;
                    pac.condicao = VIVO;
                    pac.movimento = false;
                    nivel = 1;
                    mapaAtual = 1;
                    pac.pontos = 0;
                    bufferDirecao = PARADO;
                    contagemPellets = 0;
                    contagemSuperPellets = 0;
                    pac.passoAberto = false;
                    direcao = PARADO;
                    tempoEspera = 0.0f;
                    pac.temporizadoBoca = 0.0f;
                    pac.frameAtualMorte = 0;
                    temporest = 120.0f;
                    tempoJogo = 0.0f;


                    for (int i = 0; i < MAZE_HEIGHT; i++){
                        for (int j = 0; j < MAZE_WIDTH; j++){
                            maze[i][j] = initialmaze[i][j];
                        }
                    }

                    InfoMapa infoMapaReinicio = processarMapa(maze);
                    
                    if(infoMapaReinicio.encontrouPacman){
                        pac.pos = infoMapaReinicio.posicaoPacman;
                        posInici = infoMapaReinicio.posicaoPacman;
                    }
                    
                    limparListaFantasmas(&fantasmas);
                    inicializarListaFantasmas(&fantasmas);
                    
                    if(infoMapaReinicio.numFantasmas > 0){
                        for(int i = 0; i < infoMapaReinicio.numFantasmas && i < 4; i++){
                            Fantasma f = criarFantasma(
                                tipos[i],
                                (int)infoMapaReinicio.posicoesFantasmas[i].y,
                                (int)infoMapaReinicio.posicoesFantasmas[i].x,
                                TILE,
                                spritesNormais[i],
                                "resources/sprites/ghosts/frightened.png",
                                "resources/sprites/ghosts/eyes.png"
                            );
                            inserirFantasma(&fantasmas, f);
                        }
                    }

                    musicaGameplayTocando = false;
                    musicaVulneravelTocando = false;
                    somMorteTocado = false;

                    reiniciar = false;
                }

                if(!jogoPausado){
                    tempoJogo += dt;
                    temporest -= dt;

                    if(temporest <= 0){
                        temporest = 0;
                        telaAtual = FIM_DE_JOGO;
                    }
                    
                    attPacman(&pac, dt);
                    
                    if(pac.condicao == VIVO)
                    {
                        checarColisao(pac.pos, maze, &pac.pontos, &comeuSuper, sound1);
                        updatePosition(&pac.pos, &direcao, &bufferDirecao, maze, Char1);
                        
                        int pelletsRestantes = contarPelletsRestantes(maze);
                        if(pelletsRestantes == 0){
                            if(mapaAtual == 1){
                                Mapa *mapa2 = carregarMapa("mapa2.txt");
                                if(mapa2){
                                    for (int i = 0; i < MAZE_HEIGHT; i++){
                                        for (int j = 0; j < MAZE_WIDTH; j++){
                                            maze[i][j] = mapa2->celulas[i][j];
                                        }
                                    }
                                    liberarMapa(mapa2);
                                    
                                    InfoMapa novoMapa = processarMapa(maze);
                                    if(novoMapa.encontrouPacman){
                                        pac.pos = novoMapa.posicaoPacman;
                                        posInici = novoMapa.posicaoPacman;
                                    }
                                    
                                    limparListaFantasmas(&fantasmas);
                                    inicializarListaFantasmas(&fantasmas);
                                    
                                    if(novoMapa.numFantasmas > 0){
                                        for(int i = 0; i < novoMapa.numFantasmas && i < 4; i++){
                                            Fantasma f = criarFantasma(
                                                tipos[i],
                                                (int)novoMapa.posicoesFantasmas[i].y,
                                                (int)novoMapa.posicoesFantasmas[i].x,
                                                TILE,
                                                spritesNormais[i],
                                                "resources/sprites/ghosts/frightened.png",
                                                "resources/sprites/ghosts/eyes.png"
                                            );
                                            inserirFantasma(&fantasmas, f);
                                        }
                                    }
                                    
                                    mapaAtual = 2;
                                    nivel = 2;
                                    temporest = 120.0f;
                                    tempoJogo = 0.0f;

                                    StopSound(siren);
                                    StopSound(sup_siren);
                                    musicaGameplayTocando = false;
                                    musicaVulneravelTocando = false;
                                    
                                }
                            } else if(mapaAtual == 2){
                                StopSound(siren);
                                StopSound(sup_siren);
                                telaAtual = VITORIA;
                            }
                        }

                        if(comeuSuper){
                            ativarModoVulneravel(&fantasmas);
                            comeuSuper = 0;

                            StopSound(siren);
                            PlaySound(sup_siren);
                            musicaVulneravelTocando = true;
                            musicaGameplayTocando = false;
                        }

                        atualizarTemporizadorVulneravel(&fantasmas, dt);



                        tempoMovimentoFantasmas += dt;
                        if(tempoMovimentoFantasmas >= intervaloMovimentoFantasmas){ 
                            char *mapaPtr[MAZE_HEIGHT];
                            for(int i = 0; i < MAZE_HEIGHT; i++){
                                mapaPtr[i] = maze[i];
                            }
                        
                            moverFantasmas(&fantasmas, mapaPtr, MAZE_HEIGHT, MAZE_WIDTH, TILE);
                            tempoMovimentoFantasmas = 0.0f;
                        }

                        int colisao = verificarColisoesPorDistancia(&fantasmas, pac.pos, raioColisao);

                        if(colisao == 1){
                            pac.condicao = MORTO;
                            pac.vidas--;
                            pac.frameAtualMorte = 0;
                            pac.temporizadorMorte = 0.0f;
                            pac.direcao = PARADO;
                            pac.movimento = false;

                            StopSound(siren);
                            StopSound(sup_siren);
                            PlaySound(death);
                            musicaGameplayTocando = false;
                            musicaVulneravelTocando = false;
                        }

                        pac.movimento = true;
                        pac.direcao = direcao;
                    }

                    if (pac.condicao == MORTO){
                        pac.movimento = false;
                        tempoEspera += dt;
                        if (tempoEspera >= 1.5f){
                            if (pac.vidas > 0){
                                pac.condicao = VIVO;
                                pac.pos = posInici;
                                direcao = PARADO;
                                bufferDirecao = PARADO;
                                tempoEspera = 0.0f;
                                PlaySound(siren);
                                musicaGameplayTocando = true;
                            } else {
                                telaAtual = FIM_DE_JOGO;
                            }
                        }
                    }
                }

                desenhaPacman(&pac);
                desenharFantasma(&fantasmas, TILE);
                drawMaze(maze);
                desenharPlacar(pac.pontos);
                desenharVidas(pac.vidas);
                
                int minutos = (int)temporest / 60;
                int segundos = (int)temporest % 60;
                desenharTempo(minutos, segundos);
                desenharNivel(mapaAtual);
            } break;

            case FIM_DE_JOGO:{
                DrawText("O QUE VOCE QUER FAZER? ", 100, 80, 28, YELLOW);
                DrawText("R - REINICIAR", 120, 230, 28, YELLOW);
                DrawText("N - SAIR DO JOGO", 120, 330, 28, YELLOW);
                
                if(reiniciar){
                    pac.vidas = 3;
                    pac.direcao = PARADO;
                    pac.pos = posInici;
                    pac.condicao = VIVO;
                    pac.movimento = false;
                    nivel = 1;
                    mapaAtual = 1;
                    pac.pontos = 0;
                    bufferDirecao = PARADO;
                    contagemPellets = 0;
                    contagemSuperPellets = 0;
                    pac.passoAberto = false;
                    direcao = PARADO;
                    tempoEspera = 0.0f;
                    pac.temporizadoBoca = 0.0f;
                    pac.frameAtualMorte = 0;
                    temporest = 300.0f;
                    tempoJogo = 0.0f;

                    for (int i = 0; i < MAZE_HEIGHT; i++){
                        for (int j = 0; j < MAZE_WIDTH; j++){
                            maze[i][j] = initialmaze[i][j];
                        }
                    }

                    InfoMapa infoMapaReinicio = processarMapa(maze);
    
                    if (infoMapaReinicio.encontrouPacman) {
                        pac.pos = infoMapaReinicio.posicaoPacman;
                        posInici = infoMapaReinicio.posicaoPacman;
                    }

                    if (infoMapaReinicio.numFantasmas > 0) {
                        limparListaFantasmas(&fantasmas);
                        inicializarListaFantasmas(&fantasmas);
                        
                        for (int i = 0; i < infoMapaReinicio.numFantasmas && i < 4; i++) {
                            int linha = (int)infoMapaReinicio.posicoesFantasmas[i].y;
                            int coluna = (int)infoMapaReinicio.posicoesFantasmas[i].x;
            
                            Fantasma f = criarFantasma(
                                tipos[i], linha, coluna, TILE, 
                                spritesNormais[i],
                                "resources/sprites/ghosts/frightened.png",
                                "resources/sprites/ghosts/eyes.png"
                            );
                            inserirFantasma(&fantasmas, f);
                        }
                    }

                    reiniciar = false;
                }
            }break;
            
            case PAUSA:{
                DrawText("PAUSA ", 100, 80, 28, YELLOW);
                DrawText("C - CONTINUAR", 120, 130, 28, YELLOW);
                DrawText("R - REINICIAR", 120, 170, 28, YELLOW);
                DrawText("S - SALVAR JOGO", 120, 210, 28, YELLOW);
                DrawText("F - CARREGAR JOGO", 120, 250, 28, YELLOW);
                DrawText("ESC - SAIR DO JOGO", 120, 290, 28, YELLOW);
                DrawTextureEx(chicopombo, (Vector2){340, 20}, 0.0f, 6.0f, WHITE);

            }break;
            case VITORIA:{
                char* texto = "VITÓRIA!";
                int larguraTexto = MeasureText(texto, 25);
                if (displaycounter > 60 && (x % 2 == 0)){
                    DrawText(texto,(GetScreenWidth() - larguraTexto) / 2, GetScreenHeight() / 2, 25, RED);
                }
            }
            default:    
                break;
        }

        if(gameOver == true){
            CloseWindow();
        }

        EndDrawing();
    }

    UnloadSound(intro);
    UnloadSound(sound1);
    UnloadSound(siren);
    UnloadSound(Startmusic);
    UnloadSound(sup_siren);
    UnloadSound(death);
    CloseAudioDevice();

    CloseWindow();
    return 0;
}

void desenharTempo(int minutos, int segundos){
    DrawText(TextFormat("Tempo: %02d:%02d", minutos, segundos), 660, (MAZE_HEIGHT*23) - 54, 20, YELLOW);
}

void desenharNivel(int mapaAtual){
    DrawText(TextFormat("Fase: %d", mapaAtual), 660, (MAZE_HEIGHT*23) - 34, 20, BLUE);
}

void desenharPlacar(int pontos){
    DrawText(TextFormat("Milhos: %i", pontos), 10, (MAZE_HEIGHT*23) - 54, 20, LIGHTGRAY);
}

void desenharVidas(int vidasPac){
    DrawText(TextFormat("Vidas: %i", vidasPac), 10, (MAZE_HEIGHT*23) - 30, 20, LIGHTGRAY);
}

void drawMaze(char maze[MAZE_HEIGHT][MAZE_WIDTH]) {
    for (int y = 0; y < MAZE_HEIGHT; y++)
    {
        for (int x = 0; x < MAZE_WIDTH; x++)
        {
            if (maze[y][x] == '#')
            {   
                Rectangle rec = {x*20, y*20, 20, 20};
                DrawRectangleRec(rec, BLACK);
                DrawRectangleLinesEx(rec, 5, DARKBLUE);
            }
            else if (maze[y][x] == '.')
            {
                DrawCircle(x*20+10, y*20+10, 2, YELLOW);
            }
            else if (maze[y][x] == 'O')
            {
                DrawCircle(x*20+10, y*20+10, 6, RED);
            }
            else if (maze[y][x] == '_')
            {   
                Rectangle rec = {x*20, y*20, 20, 20};
                DrawRectangleRec(rec, BLACK);
                DrawRectangleLinesEx(rec, 5, WHITE);
            }
        }
    }
}

void checarColisao(Vector2 pos, char maze[MAZE_HEIGHT][MAZE_WIDTH], int *pontos, int *comeu, Sound sound1){
    if(maze[(int)pos.y/20][(int)pos.x/20] == '.'){
        *pontos += 10;
        maze[(int)pos.y/20][(int)pos.x/20] = ' ';
        PlaySound(sound1);
    }
    else if(maze[(int)pos.y/20][(int)pos.x/20] == 'O'){
        *pontos += 50;
        maze[(int)pos.y/20][(int)pos.x/20] = ' ';
        *comeu = 1;
    }
}

int avoidRotation(Vector2 *pos, DIRECAO *direction, DIRECAO *buffer_direction, char maze[MAZE_HEIGHT][MAZE_WIDTH], char Char) {
    if ((*direction == DIREITO && maze[(int)(pos->y)/20][(int)(pos->x + 20)/20] == Char)
    || (*direction == DIREITO && maze[(int)(pos->y)/20][(int)(pos->x + 20)/20] == '_')) {
        *buffer_direction = PARADO;
    }
    else if ((*direction == ESQUERDA && maze[(int)(pos->y)/20][(int)(pos->x - 20)/20] == Char)
    || (*direction == ESQUERDA && maze[(int)(pos->y)/20][(int)(pos->x - 20)/20] == '_')) {
        *buffer_direction = PARADO;
    }
    else if ((*direction == CIMA && maze[(int)(pos->y - 20)/20][(int)(pos->x)/20] == Char)
    || (*direction == CIMA && maze[(int)(pos->y - 20)/20][(int)(pos->x)/20] == '_')) {
        *buffer_direction = PARADO;
    }
    else if ((*direction == BAIXO && maze[(int)(pos->y + 20)/20][(int)(pos->x)/20] == Char)
    || (*direction == BAIXO && maze[(int)(pos->y + 20)/20][(int)(pos->x)/20] == '_')) {
        *buffer_direction = PARADO;
    }
    else
        return 0;
    return 1;
}

void updatePosition(Vector2 *pos, DIRECAO *direction, DIRECAO *buffer_direction, char maze[MAZE_HEIGHT][MAZE_WIDTH], char Char) {
    Vector2 newPos = *pos;
    float error_term = 7;

    if (pos->x > (int)((MAZE_WIDTH - 1) * 20)) {
        pos->x = 1;
    } else if (pos->x < 1) {
        pos->x = (MAZE_WIDTH - 1) * 20;
    }

    if (IsKeyPressed(KEY_RIGHT)) {
        *buffer_direction = DIREITO;
        if (((maze[(int)(pos->y - error_term)/ 20][(int)(pos->x + 20)/ 20] != Char) && (maze[(int)(pos->y + error_term)/ 20][(int)(pos->x + 20)/ 20] != Char))
        && ((maze[(int)(pos->y - error_term)/ 20][(int)(pos->x + 20)/ 20] != '_') && (maze[(int)(pos->y + error_term)/ 20][(int)(pos->x + 20)/ 20] != '_'))) {
            *direction = *buffer_direction;
            *buffer_direction = PARADO;
        }
        else
            avoidRotation(pos, direction, buffer_direction, maze, Char);
    } 
    if (IsKeyPressed(KEY_LEFT)) {
        *buffer_direction = ESQUERDA;
        if (((maze[(int)(pos->y - error_term)/ 20][(int)(pos->x - 20)/ 20] != Char) && (maze[(int)(pos->y + error_term)/ 20][(int)(pos->x - 20)/ 20] != Char))
        && ((maze[(int)(pos->y - error_term)/ 20][(int)(pos->x - 20)/ 20] != '_') && (maze[(int)(pos->y + error_term)/ 20][(int)(pos->x - 20)/ 20] != '_'))) {
            *direction = *buffer_direction;
            *buffer_direction = PARADO;
        }
        else
            avoidRotation(pos, direction, buffer_direction, maze, Char);
    } 
    if (IsKeyPressed(KEY_UP)) {
        *buffer_direction = CIMA;
        if (((maze[(int)(pos->y - 20)/ 20][(int)(pos->x + error_term)/ 20] != Char) && (maze[(int)(pos->y - 20)/ 20][(int)(pos->x - error_term)/ 20] != Char))
        && ((maze[(int)(pos->y - 20)/ 20][(int)(pos->x + error_term)/ 20] != '_') && (maze[(int)(pos->y - 20)/ 20][(int)(pos->x - error_term)/ 20] != '_'))) {
            *direction = *buffer_direction;
            *buffer_direction = PARADO;
        }
        else
            avoidRotation(pos, direction, buffer_direction, maze, Char);
    } 
    if (IsKeyPressed(KEY_DOWN)) {
        *buffer_direction = BAIXO;
        if (((maze[(int)(pos->y + 20)/ 20][(int)(pos->x + error_term)/ 20] != Char) && (maze[(int)(pos->y + 20)/ 20][(int)(pos->x - error_term)/ 20] != Char))
        && ((maze[(int)(pos->y + 20)/ 20][(int)(pos->x + error_term)/ 20] != '_') && (maze[(int)(pos->y + 20)/ 20][(int)(pos->x - error_term)/ 20] != '_'))) {
            *direction = *buffer_direction;
            *buffer_direction = PARADO;
        }
        else
            avoidRotation(pos, direction, buffer_direction, maze, Char);
    }
    
    if (*buffer_direction == DIREITO) {
        if (((maze[(int)(pos->y - error_term)/ 20][(int)(pos->x + 20)/ 20] != Char) && (maze[(int)(pos->y + error_term)/ 20][(int)(pos->x + 20)/ 20] != Char))
        && ((maze[(int)(pos->y - error_term)/ 20][(int)(pos->x + 20)/ 20] != '_') && (maze[(int)(pos->y + error_term)/ 20][(int)(pos->x + 20)/ 20] != '_'))) {
            *direction = *buffer_direction;
            *buffer_direction = PARADO;
        }
    } 
    if (*buffer_direction == ESQUERDA) {
        if (((maze[(int)(pos->y - error_term)/ 20][(int)(pos->x - 20)/ 20] != Char) && (maze[(int)(pos->y + error_term)/ 20][(int)(pos->x - 20)/ 20] != Char))
        && ((maze[(int)(pos->y - error_term)/ 20][(int)(pos->x - 20)/ 20] != '_') && (maze[(int)(pos->y + error_term)/ 20][(int)(pos->x - 20)/ 20] != '_'))) {
            *direction = *buffer_direction;
            *buffer_direction = PARADO;
        }
    } 
    if (*buffer_direction == CIMA) {
        if (((maze[(int)(pos->y - 20)/ 20][(int)(pos->x + error_term)/ 20] != Char) && (maze[(int)(pos->y - 20)/ 20][(int)(pos->x - error_term)/ 20] != Char))
        && ((maze[(int)(pos->y - 20)/ 20][(int)(pos->x + error_term)/ 20] != '_') && (maze[(int)(pos->y - 20)/ 20][(int)(pos->x - error_term)/ 20] != '_'))) {
            *direction = *buffer_direction;
            *buffer_direction = PARADO;
        }
    } 
    if (*buffer_direction == BAIXO) {
        if (((maze[(int)(pos->y + 20)/ 20][(int)(pos->x + error_term)/ 20] != Char) && (maze[(int)(pos->y + 20)/ 20][(int)(pos->x - error_term)/ 20] != Char))
        && ((maze[(int)(pos->y + 20)/ 20][(int)(pos->x + error_term)/ 20] != '_') && (maze[(int)(pos->y + 20)/ 20][(int)(pos->x - error_term)/ 20] != '_'))) {
            *direction = *buffer_direction;
            *buffer_direction = PARADO;
        }
    }    
 
    if (*direction == DIREITO) {
        newPos.x += SPEED; 
        if ((maze[(int)newPos.y / 20][(int)(newPos.x + error_term) / 20] != Char) && (maze[(int)newPos.y / 20][(int)(newPos.x + error_term) / 20] != '_')) {
            *pos = newPos;
        }
        else {
            newPos.x -= SPEED;
        }
    }
    else if (*direction == ESQUERDA) {
        newPos.x -= SPEED;
        if ((maze[(int)newPos.y / 20][(int)(newPos.x - error_term)/ 20] != Char) && (maze[(int)newPos.y / 20][(int)(newPos.x - error_term)/ 20] != '_')) {
            *pos = newPos;
        }
        else {
            newPos.x += SPEED;
        }
    }        
    else if (*direction == CIMA) {
        newPos.y -= SPEED;
        if ((maze[(int)(newPos.y - error_term)/ 20][(int)newPos.x / 20] != Char) && (maze[(int)(newPos.y - error_term)/ 20][(int)newPos.x / 20] != '_')) {
            *pos = newPos;
        }
        else {
            newPos.y += SPEED;
        }
    }
    else if (*direction == BAIXO) {
        newPos.y += SPEED; 
        if ((maze[(int)(newPos.y + error_term)/ 20][(int)newPos.x / 20] != Char) && (maze[(int)(newPos.y + error_term)/ 20][(int)newPos.x / 20] != '_')) {
            *pos = newPos;
        }
        else {
            newPos.y -= SPEED;
        }    
    }
}

InfoMapa processarMapa(char maze[MAZE_HEIGHT][MAZE_WIDTH]) {
    InfoMapa info;
    info.numFantasmas = 0;
    info.numPortais = 0;
    info.encontrouPacman = false;
    info.posicaoPacman = (Vector2){-1, -1};
    
    for (int i = 0; i < MAZE_HEIGHT; i++) {
        for (int j = 0; j < MAZE_WIDTH; j++) {
            if (maze[i][j] == 'P') {
                info.posicaoPacman.x = j * TILE + TILE / 2.0f;
                info.posicaoPacman.y = i * TILE + TILE / 2.0f;
                info.encontrouPacman = true;
                maze[i][j] = ' ';
            }
            else if (maze[i][j] == 'F') {
                if (info.numFantasmas < 10) {
                    info.posicoesFantasmas[info.numFantasmas].x = j;
                    info.posicoesFantasmas[info.numFantasmas].y = i;
                    info.numFantasmas++;
                }
                maze[i][j] = ' ';
            }
            else if (maze[i][j] == 'T') {
                if (info.numPortais < 4) {
                    info.portais[info.numPortais].x = j;
                    info.portais[info.numPortais].y = i;
                    info.numPortais++;
                }
            }
        }
    }
    
    return info;
}