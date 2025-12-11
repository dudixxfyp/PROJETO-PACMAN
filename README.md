# PROJETO-PACMAN
# 🎮 PAC-MAN Game

pacman desenvolvimento com raylib, com adaptações do original
## 📋 Descrição

Este projeto implementa uma versão do icônico jogo Pac-Man, onde o jogador controla o personagem Chico-Man através de um labirinto, coletando pontos enquanto foge dos fantasmas. O jogo apresenta mecânicas clássicas como power-ups, sistema de vidas, salvamento de progresso e múltiplos níveis de dificuldade.

## ✨ Características

- **Gameplay Clássico**: Movimentação fluida pelo labirinto
- **4 Fantasmas Únicos**: Blinky, Pinky, Inky e Clyde, cada um com comportamento próprio
- **Sistema de Power-ups**: Colete pellets especiais para tornar os fantasmas vulneráveis
- **Múltiplos Níveis de Dificuldade**: Fácil, Médio e Difícil
- **Sistema de Salvamento**: Salve e carregue seu progresso a qualquer momento
- **Portais**: Teleporte-se entre os lados do mapa
- **Animações**: Sprites animados para Pac-Man e fantasmas
- **Interface Completa**: Menus, tela de pausa, game over e placar

## 🎯 Objetivos do Jogo

Existem duas formas de vencer:
1. Coletar todos os milhos (pellets) do mapa
2. Sobreviver por 5 minutos

## 🕹️ Controles

- **Setas Direcionais**: Movimentar o Pac-Man
  - ↑ : Cima
  - ↓ : Baixo
  - ← : Esquerda
  - → : Direita
- **M**: Abrir menu de pausa
- **ENTER**: Confirmar seleções nos menus
- **ESC**: Sair do jogo

## 📦 Estrutura do Projeto

```
├── main.c              # Loop principal do jogo e gerenciamento de telas
├── pacman.c/h          # Lógica e renderização do Pac-Man
├── Fantasma.c/h        # Sistema de fantasmas (IA, movimentação, colisões)
├── mapa.c/h            # Carregamento e manipulação de mapas
├── menu2.c/h           # Sistema de salvamento e carregamento
├── game.h              # Definições compartilhadas
└── resources/          # Sprites e recursos gráficos
    └── sprites/
        ├── pac/        # Sprites do Pac-Man
        └── ghosts/     # Sprites dos fantasmas
```

## 🛠️ Tecnologias Utilizadas

- **Linguagem**: C
- **Biblioteca Gráfica**: [Raylib](https://www.raylib.com/)
- **Estruturas de Dados**: Listas encadeadas para gerenciamento de fantasmas

## 📥 Instalação

### Pré-requisitos

- Compilador C (GCC ou Clang)
- Biblioteca Raylib instalada


# Compile o projeto
gcc -o pacman main.c pacman.c Fantasma.c mapa.c menu2.c -lraylib -lm

# Execute o jogo
./pacman
```

## 🎮 Como Jogar

1. **Menu Inicial**: Pressione ENTER para começar
2. **Apresentação**: Conheça os personagens
3. **Regras**: Entenda os objetivos
4. **Selecione a Dificuldade**: Escolha entre Fácil, Médio ou Difícil
5. **Jogue**: Use as setas para movimentar e coletar os milhos
6. **Power-ups**: Colete os pellets grandes (O) para comer os fantasmas
7. **Pause**: Pressione M para pausar e acessar opções de salvar/carregar

## 🎨 Personagens

| Personagem | Nome | Cor | Descrição |
|------------|------|-----|-----------|
| 👻 | **Blinky** | Vermelho | O fantasma perseguidor |
| 👻 | **Pinky** | Rosa | O fantasma emboscador |
| 👻 | **Inky** | Azul | O fantasma imprevisível |
| 👻 | **Clyde** | Laranja | O fantasma aleatório |
| 🟡 | **Chico-Man** | Amarelo | Nosso herói! |

## 💾 Sistema de Salvamento

O jogo possui um sistema completo de salvamento que armazena:
- Posição do Pac-Man
- Estado de todos os fantasmas
- Pontuação atual
- Vidas restantes
- Estado do mapa (pellets coletados)
- Nível de dificuldade
- Tempo de jogo

**Como Salvar**: No menu de pausa (tecla M), pressione S

**Como Carregar**: No menu inicial ou de pausa, selecione a opção de carregar

## 🗺️ Mapas

O jogo suporta mapas personalizados através de arquivos de texto. O formato do mapa utiliza os seguintes caracteres:

- `#` : Parede
- `.` : Pellet pequeno (10 pontos)
- `O` : Super pellet (50 pontos + modo vulnerável)
- `P` : Posição inicial do Pac-Man
- `F` : Posição inicial dos fantasmas
- `T` : Portal
- `_` : Parede invisível
- ` ` : Espaço vazio

## 🏗️ Arquitetura

### Estruturas de Dados Principais

**Pacman**
```c
typedef struct Pacman {
    Vector2 pos;
    int vidas;
    int pontos;
    DIRECAO direcao;
    CONDICAO condicao;
    Texture2D sprites[...];
    // ...
} Pacman;
```

**Fantasma**
```c
typedef struct Fantasma {
    Vector2 posicao;
    int linha, coluna;
    TipoFantasma tipo;
    CondicaoFantasma condicao;
    int vivo;
    int vulneravel;
    // ...
} Fantasma;
```

**Lista de Fantasmas**
```c
typedef struct ListaFantasmas {
    NoFantasma *inicio;
} ListaFantasmas;
```

### Fluxo do Jogo

1. **Inicialização**: Carrega recursos, cria entidades
2. **Loop Principal**: 
   - Processa entrada do usuário
   - Atualiza posições (Pac-Man e fantasmas)
   - Detecta colisões
   - Renderiza elementos
3. **Gerenciamento de Estado**: Alterna entre telas (menu, gameplay, pausa, etc.)

## 🐛 Características Técnicas

- **Taxa de Atualização**: 60 FPS
- **Tamanho do Tile**: 20x20 pixels
- **Dimensões do Mapa**: 40x20 tiles (800x400 pixels)
- **Sistema de Colisão**: Baseado em distância euclidiana
- **Movimentação**: Sistema de buffer para rotações suaves
- **IA dos Fantasmas**: Movimento aleatório com validação de colisão

## 📝 Sistema de Pontuação

- **Pellet Pequeno**: 10 pontos
- **Super Pellet**: 50 pontos
- **Fantasma Vulnerável**: Pontos adicionais

## 🎯 Modos de Fantasma

1. **Normal (VIVO)**: Fantasmas perseguem o Pac-Man
2. **Vulnerável (VULNERAVEL)**: Fantasmas ficam azuis e podem ser comidos (8 segundos)
3. **Morto (MORTO)**: Fantasma foi comido, mostra apenas os olhos


## 📄 Licença

Este projeto é um trabalho acadêmico desenvolvido para fins educacionais.

## 👥 Autores




## 📚 Documentação Detalhada dos Códigos

### 1. main.c - Coração do Jogo

O arquivo `main.c` é o ponto de entrada do programa e gerencia todo o fluxo do jogo.

#### Estruturas Principais

```c
typedef enum GameScreen{
    INICIAL,      // Tela inicial com "pressione enter"
    MENU,         // Menu principal
    DESCRICAO,    // Apresentação dos personagens
    DIFICULDADE,  // Seleção de dificuldade
    GAMEPLAY,     // Jogo em ação
    FIM_DE_JOGO,  // Tela de game over
    REGRAS,       // Explicação das regras
    PAUSA         // Menu de pausa
} GameScreen;
```

**Explicação**: Este enum define todos os estados possíveis do jogo, facilitando a navegação entre diferentes telas.

```c
typedef struct {
    Vector2 posicaoPacman;
    Vector2 posicoesFantasmas[10];
    int numFantasmas;
    Vector2 portais[4];
    int numPortais;
    bool encontrouPacman;
} InfoMapa;
```

**Explicação**: Estrutura que armazena informações extraídas do mapa durante o carregamento, incluindo posições iniciais de todos os elementos.

#### Mapa Inicial

```c
const char initialmaze[MAZE_HEIGHT][MAZE_WIDTH] = {
    "########################################",
    "#....................##................#",
    // ... resto do mapa
};
```

**Explicação**: Matriz de caracteres que define o layout do labirinto. Cada caractere representa um elemento diferente do jogo.

#### Função Principal - main()

```c
int main(void){
    // 1. Inicialização da janela
    const int screenWidth = MAZE_WIDTH * 20;
    const int screenHeight = MAZE_HEIGHT * 20;
    
    // 2. Carregamento de mapa personalizado
    Mapa *mapaCarregado = carregarMapa("mapa.txt");
    
    // 3. Processamento do mapa
    InfoMapa infoMapa = processarMapa(maze);
    
    // 4. Criação do Pac-Man
    Pacman pac = criaPacman(/* sprites */, pos);
    
    // 5. Criação dos fantasmas
    ListaFantasmas fantasmas;
    inicializarListaFantasmas(&fantasmas);
    
    // 6. Loop principal do jogo
    while (!WindowShouldClose()) {
        // Processa entrada, atualiza lógica, renderiza
    }
}
```

**Explicação Detalhada do Loop Principal**:

1. **Captura de Eventos**: `switch (telaAtual)` processa entradas diferentes para cada tela
2. **Atualização da Física**: Movimenta Pac-Man e fantasmas
3. **Detecção de Colisões**: Verifica interações entre entidades
4. **Renderização**: Desenha todos os elementos na tela

#### Funções Auxiliares

**drawMaze()** - Renderização do Labirinto
```c
void drawMaze(char maze[MAZE_HEIGHT][MAZE_WIDTH]) {
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            if (maze[y][x] == '#') {
                // Desenha paredes com bordas azuis
                Rectangle rec = {x*20, y*20, 20, 20};
                DrawRectangleRec(rec, BLACK);
                DrawRectangleLinesEx(rec, 5, DARKBLUE);
            }
            else if (maze[y][x] == '.') {
                // Pellet pequeno (círculo amarelo)
                DrawCircle(x*20+10, y*20+10, 2, YELLOW);
            }
            else if (maze[y][x] == 'O') {
                // Super pellet (círculo vermelho maior)
                DrawCircle(x*20+10, y*20+10, 6, RED);
            }
            else if (maze[y][x] == 'T') {
                // Portal (círculo roxo)
                DrawCircle(x*20+10, y*20+10, 8, PURPLE);
            }
        }
    }
}
```

**Explicação**: Percorre toda a matriz do mapa e desenha cada elemento baseado no caractere correspondente.

**checarColisao()** - Sistema de Coleta de Pontos
```c
void checarColisao(Vector2 pos, char maze[MAZE_HEIGHT][MAZE_WIDTH], 
                   int *pontos, int *comeu){
    int tileY = (int)pos.y / 20;
    int tileX = (int)pos.x / 20;
    
    if(maze[tileY][tileX] == '.'){
        *pontos += 10;              // Adiciona pontos
        maze[tileY][tileX] = ' ';   // Remove pellet do mapa
    }
    else if(maze[tileY][tileX] == 'O'){
        *pontos += 50;
        maze[tileY][tileX] = ' ';
        *comeu = 1;                 // Ativa modo vulnerável
    }
}
```

**Explicação**: Converte a posição em pixels para coordenadas de tile e verifica se há um item coletável naquela posição.

**updatePosition()** - Sistema de Movimentação
```c
void updatePosition(Vector2 *pos, DIRECAO *direction, 
                   DIRECAO *buffer_direction, 
                   char maze[MAZE_HEIGHT][MAZE_WIDTH], char Char) {
    Vector2 newPos = *pos;
    float error_term = 7;  // Margem de erro para colisões
    
    // Wraparound (teleporte nas bordas)
    if (pos->x > (int)((MAZE_WIDTH - 1) * 20)) {
        pos->x = 1;
    } else if (pos->x < 1) {
        pos->x = (MAZE_WIDTH - 1) * 20;
    }
    
    // Processa input do jogador
    if (IsKeyPressed(KEY_RIGHT)) {
        *buffer_direction = DIREITO;
        // Verifica se pode virar nessa direção
    }
    
    // Move na direção atual
    if (*direction == DIREITO) {
        newPos.x += SPEED;
        // Verifica colisão antes de aplicar movimento
        if ((maze[(int)newPos.y / 20][(int)(newPos.x + error_term) / 20] != Char)) {
            *pos = newPos;
        }
    }
}
```

**Explicação**: Sistema sofisticado que:
- Armazena uma direção desejada em buffer
- Aplica a mudança apenas quando possível
- Usa margem de erro para permitir curvas suaves
- Implementa teleporte nas bordas do mapa

**processarMapa()** - Extração de Informações
```c
InfoMapa processarMapa(char maze[MAZE_HEIGHT][MAZE_WIDTH]) {
    InfoMapa info;
    info.numFantasmas = 0;
    info.numPortais = 0;
    info.encontrouPacman = false;
    
    for (int i = 0; i < MAZE_HEIGHT; i++) {
        for (int j = 0; j < MAZE_WIDTH; j++) {
            if (maze[i][j] == 'P') {
                // Encontrou Pac-Man
                info.posicaoPacman.x = j * TILE + TILE / 2.0f;
                info.posicaoPacman.y = i * TILE + TILE / 2.0f;
                info.encontrouPacman = true;
                maze[i][j] = ' ';  // Limpa o marcador
            }
            else if (maze[i][j] == 'F') {
                // Encontrou fantasma
                info.posicoesFantasmas[info.numFantasmas].x = j;
                info.posicoesFantasmas[info.numFantasmas].y = i;
                info.numFantasmas++;
                maze[i][j] = ' ';
            }
            else if (maze[i][j] == 'T') {
                // Portal encontrado
                info.portais[info.numPortais].x = j;
                info.portais[info.numPortais].y = i;
                info.numPortais++;
            }
        }
    }
    return info;
}
```

**Explicação**: Varre o mapa procurando por marcadores especiais ('P', 'F', 'T') e armazena suas posições para uso posterior na inicialização das entidades.

---

### 2. pacman.c/h - O Protagonista

#### Estrutura do Pac-Man

```c
typedef struct Pacman {
    Vector2 pos;                    // Posição em pixels
    int vidas;                      // Vidas restantes
    int pontos;                     // Pontuação
    float velocidade;               // Velocidade de movimento
    
    // Sprites para cada direção
    Texture2D andarCima;
    Texture2D andarCima2;
    Texture2D andarBaixo;
    Texture2D andarBaixo2;
    Texture2D andarEsquerda;
    Texture2D andarEsquerda2;
    Texture2D andarDireita;
    Texture2D andarDireita2;
    Texture2D spritesMorte[4];      // Animação de morte
    Texture2D spriteParado;
    
    // Controle de animação
    bool passoAberto;               // Estado da boca (aberta/fechada)
    float temporizadoBoca;          // Timer para alternar animação
    float rotacao;                  // Rotação do sprite
    
    DIRECAO direcao;                // Direção atual
    bool movimento;                 // Está se movendo?
    CONDICAO condicao;              // VIVO ou MORTO
    
    int frameAtualMorte;            // Frame da animação de morte
    float temporizadorMorte;        // Timer da animação de morte
} Pacman;
```

**Explicação**: Estrutura completa que encapsula todos os dados necessários para o Pac-Man, incluindo sprites, estado e animação.

#### Criação do Pac-Man

```c
Pacman criaPacman(char *andarCima, char *andarCima2, 
                  char *andarEsquerda, char *andarEsquerda2,
                  char *andarBaixo, char *andarBaixo2, 
                  char *andarDireita, char *andarDireita2,
                  char *parado, Vector2 posInicial) {
    Pacman p;
    
    // Carrega todos os sprites
    p.andarCima = LoadTexture(andarCima);
    p.andarCima2 = LoadTexture(andarCima2);
    // ... carrega outros sprites
    
    // Carrega animação de morte
    for (int i = 0; i < 4; i++) {
        char nome[128];
        sprintf(nome, "resources/sprites/pac/deathAnim/death%d.png", i+1);
        p.spritesMorte[i] = LoadTexture(nome);
    }
    
    // Inicializa valores padrão
    p.velocidade = VEL_BASE;
    p.vidas = 3;
    p.pontos = 0;
    p.pos = posInicial;
    p.direcao = PARADO;
    p.condicao = VIVO;
    p.passoAberto = false;
    p.temporizadoBoca = 0.0f;
    
    return p;
}
```

**Explicação**: Função construtora que inicializa todos os campos da estrutura Pacman, carregando recursos e definindo valores iniciais.

#### Sistema de Animação

```c
void attPacman(Pacman *p, float dt) {
    // Se está morto, anima a morte
    if(p->condicao == MORTO){
        p->temporizadorMorte += dt;
        
        // Avança frame a cada 0.15 segundos
        if (p->temporizadorMorte >= 0.15f){
            p->frameAtualMorte++;
            p->temporizadorMorte = 0.0f;
            
            // Para no último frame
            if (p->frameAtualMorte >= 4){
                p->frameAtualMorte = 3;
            }
        }
        return;
    }
    
    // Anima a boca se estiver em movimento
    if(p->movimento){
        p->temporizadoBoca += dt;
        if(p->temporizadoBoca >= 0.3f){
            p->passoAberto = !p->passoAberto;  // Alterna estado
            p->temporizadoBoca = 0.0f;
        }
    } else {
        // Para a animação quando parado
        p->passoAberto = false;
        p->temporizadoBoca = 0.0f;
    }
}
```

**Explicação**: Sistema de animação baseado em tempo (delta time) que:
- Alterna entre sprites de boca aberta/fechada
- Executa animação de morte em sequência
- Para animações quando o Pac-Man está parado

#### Renderização do Pac-Man

```c
void desenhaPacman(Pacman *p) {
    // Se está morto, desenha animação de morte
    if (p->condicao == MORTO) {
        Texture2D spriteMorte = p->spritesMorte[p->frameAtualMorte];
        
        if (spriteMorte.id != 0) {
            Rectangle sourceRec = {0, 0, 
                (float)spriteMorte.width, 
                (float)spriteMorte.height};
            Rectangle destRec = {p->pos.x, p->pos.y, 
                (float)TILE_MORTO, (float)TILE_MORTO};
            Vector2 origin = {TILE_MORTO / 2.0f, TILE_MORTO / 2.0f};
            
            DrawTexturePro(spriteMorte, sourceRec, destRec, origin, 0.0f, WHITE);
        }
        return;
    }
    
    // Seleciona sprite baseado na direção e animação
    Texture2D spriteAtual;
    
    switch (p->direcao) {
        case CIMA:
            spriteAtual = p->passoAberto ? p->andarCima : p->andarCima2;
            break;
        case BAIXO:
            spriteAtual = p->passoAberto ? p->andarBaixo : p->andarBaixo2;
            break;
        case ESQUERDA:
            spriteAtual = p->passoAberto ? p->andarEsquerda : p->andarEsquerda2;
            break;
        case DIREITO:
            spriteAtual = p->passoAberto ? p->andarDireita : p->andarDireita2;
            break;
        case PARADO:
            spriteAtual = p->spriteParado;
            break;
    }
    
    // Desenha o sprite selecionado
    if (spriteAtual.id != 0) {
        Rectangle sourceRec = {0, 0, 
            (float)spriteAtual.width, 
            (float)spriteAtual.height};
        Rectangle destRec = {p->pos.x, p->pos.y, 
            (float)TILE, (float)TILE};
        Vector2 origin = {TILE / 2.0f, TILE / 2.0f};
        
        DrawTexturePro(spriteAtual, sourceRec, destRec, 
                      origin, p->rotacao, WHITE);
    }
}
```

**Explicação**: Sistema de renderização que:
- Escolhe o sprite correto baseado em direção e animação
- Usa `DrawTexturePro` para controle total (escala, rotação, origem)
- Trata morte como caso especial

---

### 3. Fantasma.c/h - Os Antagonistas

#### Estruturas dos Fantasmas

```c
typedef enum TipoFantasma {
    BLINKY,  // Vermelho - Agressivo
    PINKY,   // Rosa - Emboscador
    INKY,    // Azul - Imprevisível
    CLYDE    // Laranja - Aleatório
} TipoFantasma;

typedef enum CondicaoFantasma {
    VIVOf,        // Normal
    VULNERAVELf,  // Pode ser comido
    MORTOf        // Foi comido (apenas olhos)
} CondicaoFantasma;

typedef struct {
    Vector2 posicao;              // Posição em pixels
    int linha;                    // Posição em grid (linha)
    int coluna;                   // Posição em grid (coluna)
    int vivo;                     // Está ativo?
    int vulneravel;               // Está vulnerável?
    float tempoVulneravel;        // Tempo restante vulnerável
    float velocidade;             // Velocidade de movimento
    TipoFantasma tipo;            // Qual fantasma é
    CondicaoFantasma condicao;    // Estado at

Explicação: Estrutura que define um fantasma individual com todos seus estados possíveis.

---
````
### lista encadeada de fantasma

```c
ctypedef struct NoFantasma {
    Fantasma f;
    struct NoFantasma *prox;
} NoFantasma;

typedef struct {
    NoFantasma *inicio;
} ListaFantasmas;
// Explicação: Usa lista encadeada para permitir número variável de fantasmas e facilitar iteração.

---
````

### Estruturas dos Fantasmas

```c
 void inicializarListaFantasmas(ListaFantasmas *L) {
    L->inicio = NULL;
}

void inserirFantasma(ListaFantasmas *L, Fantasma f) {
    NoFantasma *novo = malloc(sizeof(NoFantasma));
    if (!novo) return;
    
    novo->f = f;
    novo->prox = L->inicio;  // Insere no início
    L->inicio = novo;
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
// Explicação: Funções básicas de gerenciamento de lista encadeada adaptadas para fantasmas

 ---
````

### sistema de movimento 

```c

 int podeMover(char **mapa, int l, int c, int maxL, int maxC) {
    if (l < 0 || l >= maxL) return 0;  // Fora dos limites
    if (c < 0 || c >= maxC) return 0;
    return (mapa[l][c] != '#');        // Não é parede
}

void moverFantasmas(ListaFantasmas *L, char **mapa, 
                   int linhas, int colunas, int tamTile) {
    // Vetores de direção: cima, baixo, esquerda, direita
    int movL[4] = {-1, 1, 0, 0};
    int movC[4] = {0, 0, -1, 1};
    
    for (NoFantasma *atual = L->inicio; atual; atual = atual->prox) {
        if (!atual->f.vivo) continue;  // Pula fantasmas mortos
        
        int movido = 0;
        int tentativas = 0;
        
        // Tenta até 10 vezes encontrar direção válida
        while (!movido && tentativas < 10) {
     …
// Explicação: Sistema simples de IA que: Move fantasmas aleatoriamente Valida cada movimento contra o mapa
Sincroniza posição em grid com posição em pixels Tenta múltiplas vezes para evitar ficar preso//

---

````
### sistema de vunerabilidade 

```c
 void ativarModoVulneravel(ListaFantasmas *L) {
    for (NoFantasma *atual = L->inicio; atual; atual = atual->prox) {
        if (atual->f.vivo) {
            atual->f.vulneravel = 1;
            atual->f.tempoVulneravel = 8.0f;  // 8 segundos
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
                atual->f.condicao = VIVOf;  // Volta ao normal
            }
        }
    }
}
// Explicação: Sistema de power-up que:
Ativa modo vulnerável em todos os fantasmas vivos
Decrementa timer baseado em tempo real (delta time)
Restaura estado normal quando timer expira//

---

````
### detecção de colisões

```c
 int verificarColisoesPorDistancia(ListaFantasmas *L, 
                                  Vector2 posPacman, float raio) {
    for (NoFantasma *atual = L->inicio; atual; atual = atual->prox) {
        if (!atual->f.vivo) continue;
        
        // Calcula distância euclidiana
        float dx = posPacman.x - atual->f.posicao.x;
        float dy = posPacman.y - atual->f.posicao.y;
        float distancia = sqrt(dx * dx + dy * dy);
        
        if (distancia < raio) {
            if (atual->f.vulneravel) {
                // Pac-Man come o fantasma
                atual->f.vivo = 0;
                atual->f.condicao = MORTOf;
                atual->f.vulneravel = 0;
                return 2;  // Fantasma comido
            } else {
                // Fant…
// Explicação: Sistema sofisticado de colisão que:
Usa distância euclidiana para detecção circular precisa
Diferencia entre colisão normal e vulnerável
Reseta fantasma para posição inicial quando come Pac-Man
Retorna códigos diferentes para cada tipo de colisão //

---


````
### rederização dos fantasmas

```c 
 void desenharFantasma(ListaFantasmas *lf, int tamTile) {
    for (NoFantasma *atual = lf->inicio; atual; atual = atual->prox) {
        Texture2D sprite;
        
        // Seleciona sprite baseado no estado
        if (atual->f.condicao == MORTOf)
            sprite = atual->f.spriteOlhos;
        else if (atual->f.vulneravel)
            sprite = atual->f.spriteVulneravel;
        else
            sprite = atual->f.spritePadrao;
        
        if (sprite.id != 0) {
            Rectangle sourceRec = {0, 0, 
                (float)sprite.width, (float)sprite.height};
            Rectangle destRec = {
                atual->f.posicao.x, atual->f.posicao.y,
                (float)tamTile, (float)tamTile
            };
            Vector2 origin = {tamTile /…
// Explicação: Renderiza todos os fantasmas com sprite apropriado baseado em seu estado atual.

```
### 4. menu2.C/menu2.h

```C
#include "menu.h"
#include "pacman.h"
#include <stdio.h>
#include <stdlib.h>


void initPilha(Pilha *p) { p->topo = NULL; }

void pushPilha(Pilha *p, char acao) {
    Node *n = malloc(sizeof(Node));
    if (!n) return;
    n->acao = acao;
    n->prox = p->topo;
    p->topo = n;
}

char popPilha(Pilha *p) {
    if (p->topo == NULL) return '\0';
    Node *n = p->topo;
    char a = n->acao;
    p->topo = n->prox;
    free(n);
    return a;
}

int pilhaVazia(Pilha *p) {
    return p->topo == NULL;
}

// ------------------ SALVAR JOGO ------------------
void salvarJogo(GameData *d) {
    FILE *f = fopen("save.dat","wb");
    if(!f) return;

    // Salva pontuação
    fwrite(&d->score, sizeof(int), 1, f);

    // Salva posição do pacman
    fwrite(&d->pacman.linha, sizeof(int), 1, f);
    fwrite(&d->pacman.coluna, sizeof(int), 1, f);

    // Salva mapa
    for (int i = 0; i < d->mapa->linhas; i++){  // CORRIGIDO: adicionado 'i' na condição
        fwrite(d->mapa->celulas[i], sizeof(char), d->mapa->colunas, f);
    }

    fclose(f);
}

// ------------------ CARREGAR JOGO ------------------
int carregarJogo(GameData *d) {
    FILE *f = fopen("save.dat","rb");
    if (!f) return 0;

    fread(&d->score, sizeof(int), 1, f);
    fread(&d->pacman.linha, sizeof(int), 1, f);
    fread(&d->pacman.coluna, sizeof(int), 1, f);

    d->pacman.posicao.x = d->pacman.coluna * TILE + TILE / 2;
    d->pacman.posicao.y = d->pacman.linha * TILE + TILE / 2;

    for(int i = 0; i < d->mapa->linhas; i++){
        fread(d->mapa->celulas[i], sizeof(char), d->mapa->colunas, f);  // CORRIGIDO: removida vírgula antes do ponto
    }

    fclose(f);
    return 1;
}

// ------------------ MENU PRINCIPAL ------------------
void mostrarMenu(GameData *data, int *jogoPausado, int *resetar) {
    static Pilha historico;
    static int inicializado = 0;

    if (!inicializado) {
        initPilha(&historico);
        inicializado = 1;
    }

    printf("\n=========== MENU DO JOGO ===========\n");
    printf("N - Novo Jogo\n");
    printf("S - Salvar Jogo\n");
    printf("C - Carregar Jogo\n");
    printf("V - Voltar ao Jogo\n");
    printf("Q - Sair\n");

    if (!pilhaVazia(&historico))
        printf("\nÚltima ação: %c\n", historico.topo->acao);

    printf("\nEscolha: ");
    int op = getchar();
    while (getchar() != '\n'); // limpa buffer

    switch(op) {
        case 'n': case 'N':
            pushPilha(&historico, 'N');
            *resetar = 1;
            break;
        case 's': case 'S':
            pushPilha(&historico, 'S');
            salvarJogo(data);  // CORRIGIDO: salvarJogo não retorna int
            printf("Jogo salvo!\n");
            break;
        case 'c': case 'C':
            pushPilha(&historico, 'C');
            if (carregarJogo(data)) {
                printf("Jogo carregado!\n");
            } else {
                printf("Sem save!\n");
            }
            break;
        case 'v': case 'V':
            pushPilha(&historico, 'V');
            *jogoPausado = 0;
            break;
        case 'q': case 'Q':
            pushPilha(&historico, 'Q');
            printf("Saindo...\n");
            exit(0);
        default:
            printf("Opcao invalida!\n");
            break;
    }
}
//EXPLICAÇÃO:Esse código cuida do menu do jogo e também de salvar e carregar o progresso. Ele usa uma pilha para guardar o histórico das ações que o jogador escolhe no menu. As funções de salvar gravam em um arquivo a pontuação, a posição do Pac-Man e o mapa do jogo, enquanto a função de carregar pega esses dados do arquivo e coloca tudo de volta no jogo. O menu mostra opções como começar um novo jogo, salvar, carregar, voltar ou sair, e cada vez que o jogador escolhe algo, essa ação é guardada na pilha. Assim, o jogo consegue manter o progresso e o jogador pode continuar de onde parou.

```
```c
#ifndef MENU_H
#define MENU_H

#include "pacman.h"
#include "Fantasma.h"
#include "mapa.h"

// Estrutura com tudo que o menu precisa acessar
typedef struct {
    Pacman pacman;
    Fantasma fantasma;
    int numFantasmas;
    int score;
    int nivel;
    float tempo;
    int pelletsRestantes;
    Mapa *mapa;
} GameData;

// ------------------ PILHA ------------------
typedef struct Node {
    char acao;
    struct Node *prox;
} Node;

typedef struct {
    Node *topo;
} Pilha;

void initPilha(Pilha *p);
void pushPilha(Pilha *p, char acao);
char popPilha(Pilha *p);
int pilhaVazia(Pilha *p);

// ------------------ MENU E SALVAMENTO ------------------
int salvarJogo(GameData *data);
int carregarJogo(GameData *data);

void mostrarMenu(GameData *data, int *jogoPausado, int *resetar);

#endif // MENU_H

//explicação:Esse código é um cabeçalho que organiza todas as estruturas e funções que o menu do jogo precisa. Ele define a estrutura **GameData**, que guarda tudo o que o jogo precisa acessar pelo menu, como o Pac-Man, os fantasmas, pontuação, nível, tempo, quantidade de pellets restantes e o mapa. Também define uma estrutura de **pilha**, usada para registrar o histórico das ações escolhidas pelo jogador no menu, especificando o tipo de nó e como a pilha funciona. Além disso, declara as funções que inicializam a pilha, adicionam e removem ações dela, e verificam se está vazia. Por fim, declara também as funções responsáveis por salvar e carregar o jogo, além da função que mostra o menu e controla as escolhas do jogador. Tudo isso serve para organizar o código e permitir que outras partes do jogo usem essas funcionalidades.

```
### 5. mapa.c/mapa.h
```c
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
// explicação :Esse trecho de código é responsável por carregar o mapa do jogo a partir de um arquivo de texto e também liberar toda a memória usada quando o mapa não é mais necessário. A função que carrega o mapa abre o arquivo indicado, confere se ele existe e, caso tudo esteja certo, cria dinamicamente uma estrutura que guarda o número de linhas, colunas e uma matriz com os caracteres do mapa. Cada linha do arquivo é lida, ajustada para remover quebras de linha e completada com espaços se for menor do que o tamanho esperado. O código também verifica possíveis erros de alocação de memória e libera tudo corretamente para evitar vazamentos. Já a função de liberar o mapa percorre todas as linhas, libera cada uma e depois libera a estrutura principal. Assim, esse código garante que o jogo carregue o mapa corretamente e gerencie bem a memória.

```
```C

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
// explicação:Esse arquivo de cabeçalho define as estruturas e funções básicas usadas para trabalhar com o mapa do jogo. Ele cria a estrutura **Mapa**, que armazena o número de linhas, colunas e uma matriz de caracteres que representa o layout do cenário, e a estrutura **Posicao**, que guarda uma linha e uma coluna para indicar onde algo está no mapa. Também declara a função que carrega o mapa a partir de um arquivo de texto — retornando um ponteiro para o mapa ou **NULL** caso ocorra erro — e outra função responsável por liberar toda a memória usada pelo mapa. Essas declarações organizam o que outras partes do jogo precisam saber para manipular o mapa corretamente e exibi-lo na tela.

```
### 6. mapa.txt/mapa2.txt
```C
########################################
#..................##..................#
#.####.#####.##.#####.####.#####.##.##.#
#O####.#####.##.#####.####.#####.##.##O#
#......................................#
#.####.##.########.##.########.##.####.#
#......##....##....##....##....##......#
######.#####.##.#####.#####.##.#####.###
T     #.##   F      F    ##.#      T
######.##.##############.##.############
#..........##      P     ##............#
######.##.##############.##.############
      #.##   F      F    ##.#
######.##.##############.##.############
#............##....##..................#
#.####.#####.##.##.#####.####.#####.##.#
#...##................##...............#
###.##.##.########.##.##.###.##.########
#O....................................O#
########################################
 //mapa 1


