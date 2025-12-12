#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include "pacman.h"
#include "Fantasma.h"

#define TILE 40
#define TILE_MORTO 40
#define VEL_BASE 100.0f

Pacman criaPacman(char *andarCima, char *andarCima2, char *andarEsquerda, char *andarEsquerda2, char *andarBaixo, char *andarBaixo2, char *andarDireita, char *andarDireita2, char *parado, Vector2 posInicial){ //é chamado somente na inicializacao do programa

    Pacman p;

    p.andarCima = LoadTexture(andarCima);
    p.andarCima2 = LoadTexture(andarCima2);
    p.andarBaixo = LoadTexture(andarBaixo);
    p.andarBaixo2 = LoadTexture(andarBaixo2);
    p.andarEsquerda = LoadTexture(andarEsquerda);
    p.andarEsquerda2 = LoadTexture(andarEsquerda2);
    p.andarDireita = LoadTexture(andarDireita);
    p.andarDireita2 = LoadTexture(andarDireita2);
    p.spriteParado = LoadTexture(parado);
    
    for (int i = 0; i < 4; i++){
        char nome[128];
        sprintf(nome, "resources/sprites/pac/deathAnim/death%d.png", i+1);
        p.spritesMorte[i] = LoadTexture(nome);
    }

    p.passoAberto = false;
    p.temporizadoBoca = 0.0f;

    p.velocidade = VEL_BASE;
    p.vidas = 3;
    p.pontos = 0;
    p.pos = posInicial;
    p.direcao = PARADO;   
    p.rotacao = 0.0f;
    p.movimento = false;
    p.condicao = VIVO;

    return p;
}

void resetarPac(Pacman *p, Vector2 posInicial){
    p->pos = posInicial;
    p->vidas = 3;
    p->direcao = PARADO;
    p->condicao = VIVO;
    p->temporizadorMorte = 0;
}

void attPacman(Pacman *p, float dt){

    if(p->condicao == MORTO){
        p->temporizadorMorte += dt;

        if (p->temporizadorMorte >= 0.15f){
            p->frameAtualMorte++;
            p->temporizadorMorte = 0.0f;

            if (p->frameAtualMorte >= 4){   
                p->frameAtualMorte = 3;
            }
        }
        return;
    }


    if(p->movimento){
        p->temporizadoBoca += dt;
        if(p->temporizadoBoca >= 0.3f){
            p->passoAberto = !p->passoAberto;
            p->temporizadoBoca = 0.0f;
        }
    }else{
        p->passoAberto = false;
        p->temporizadoBoca = 0.0f;
    }

}

void desenhaPacman(Pacman *p){

    if (p->condicao == MORTO){
        Texture2D spriteMorte = p->spritesMorte[p->frameAtualMorte];

        if (spriteMorte.id != 0){
            Rectangle sourceRec = {
                0, 0, (float)spriteMorte.width, (float)spriteMorte.height
            };
            Rectangle destRec = {
                p->pos.x,
                p->pos.y,
                (float)TILE_MORTO,
                (float)TILE_MORTO
            };
            Vector2 origin = {TILE_MORTO / 2.0f, TILE_MORTO / 2.0f};
            
            DrawTexturePro(spriteMorte, sourceRec, destRec, origin, 0.0f, WHITE);
        }
        return;
    }



    Texture2D spriteAtual;

    switch (p->direcao)
    {
    case CIMA:{
            if (p->passoAberto){
            spriteAtual = p->andarCima;
        } else{
            spriteAtual = p->andarCima2;
        }
    } break;
    case BAIXO:{
            if(p->passoAberto){
                spriteAtual = p->andarBaixo;
            } else{
                spriteAtual = p->andarBaixo2;
            }
    }break;
    case ESQUERDA:{
            if(p->passoAberto){
                spriteAtual = p->andarEsquerda;
            } else{
                spriteAtual = p->andarEsquerda2;
            }
    } break;
    case DIREITO:{
            if(p->passoAberto){
                spriteAtual = p->andarDireita;
            } else{
                spriteAtual = p->andarDireita2;
            }
    } break;

    case PARADO:
        spriteAtual = p->spriteParado;
        break;
    }

    
    if (spriteAtual.id != 0){
        Rectangle sourceRec = {
            0, 0, (float)spriteAtual.width, (float)spriteAtual.height
        };
        Rectangle destRec = {
            p->pos.x,
            p->pos.y,
            (float) TILE,
            (float) TILE
        };

        Vector2 origin = {TILE / 2.0f, TILE / 2.0f};

        DrawTexturePro(spriteAtual, sourceRec, destRec, origin, p->rotacao, WHITE);
    }


    
}
