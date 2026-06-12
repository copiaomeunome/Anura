#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <raylib.h>

#include "Espinho/Espinho.h"
#include "Projetil/Projetil.h"
#include "Mapa/Mapa.h"
#include "Protagonista/Protagonista.h"
#include "Main.h"

using namespace std;

int limitar(int valor, int minimo, int maximo){
    return max(minimo, min(valor, maximo));
}

bool checa_colisao_parede(Protagonista& p, Mapa mapa){
    for(Parede par : mapa.getParedes())
        if (CheckCollisionRecs(
            {
                (float)p.getX(),
                (float)p.getY(),
                (float)p.getTamanhoX(),
                (float)p.getTamanhoY()
            },
            {
                (float)par.getX(),
                (float)par.getY(),
                (float)par.getTamanhoX(),
                (float)par.getTamanhoY()
            }
        )){
            return true;
        }
    return false;
}

void andar(Protagonista& p, Mapa& mapa, int largura, int altura){
    int movimentoX = 0;
    int movimentoY = 0;
    int posicaoAnteriorX = p.getX();
    int posicaoAnteriorY = p.getY();

    if (IsKeyDown(KEY_D)){
        movimentoX += p.getMS();
    }
    if (IsKeyDown(KEY_A)){
        movimentoX -= p.getMS();
    }
    if (IsKeyDown(KEY_S)){
        movimentoY += p.getMS();
    }
    if (IsKeyDown(KEY_W)){
        movimentoY -= p.getMS();
    }

    if (movimentoX != 0) {
        p.andarX(movimentoX);
        int limiteX = mapa.getTamanho().first - p.getTamanhoX();
        int posicaoCorrigidaX = limitar(p.getX(), 0, limiteX);
        p.andarX(posicaoCorrigidaX - p.getX());
        if (checa_colisao_parede(p, mapa)) {
            p.andarX(posicaoAnteriorX - p.getX());
        }
    }

    if (movimentoY != 0) {
        p.andarY(movimentoY);
        int limiteY = mapa.getTamanho().second - p.getTamanhoY();
        int posicaoCorrigidaY = limitar(p.getY(), 0, limiteY);
        p.andarY(posicaoCorrigidaY - p.getY());
        if (checa_colisao_parede(p, mapa)) {
            p.andarY(posicaoAnteriorY - p.getY());
        }
    }

    mapa.atualizarCamera({p.getX() + p.getTamanhoX() / 2, p.getY() + p.getTamanhoY() / 2}, largura, altura);
}

bool checa_tomar_dano(Protagonista& p, vector<Espinho> espinhos){
    for(Espinho e : espinhos)
        if (CheckCollisionRecs(
            {
                (float)p.getX(),
                (float)p.getY(),
                (float)p.getTamanhoX(),
                (float)p.getTamanhoY()
            },
            {
                (float)e.getX(),
                (float)e.getY(),
                (float)e.getTamanhoX(),
                (float)e.getTamanhoY()
            }
        )){
            p.alteraVida(e.getDano()*-1);
            return true;
        }
    return false;
}

int main() {
    InitWindow(800, 600, "Jogo");

    int monitor = GetCurrentMonitor();
    int largura = GetMonitorWidth(monitor);
    int altura = GetMonitorHeight(monitor);

    SetWindowSize(largura, altura);
    ToggleFullscreen();

    SetTargetFPS(60);

    Protagonista p(4,{100,100}, {100,100},10);
    Texture2D texturaProtagonista = LoadTexture("Protagonista/Assets/sapo_sentado.jpg");
    vector<Espinho> eMapa1 = {
        {1,{200,200}, {50,50}},
        {1,{300,1200}, {50,50}},
        {1,{400,900}, {50,50}},
        {1,{500,700}, {50,50}}
    };
    vector<Parede> pMapa1 = {
        {{200,500}, {100,50}},
        {{300,100}, {100,50}},
        {{400,500}, {100,50}},
        {{900,900}, {200,50}}
    };
    Mapa mapa1 = {{3000,3000},{0,0}, eMapa1, pMapa1};
    mapa1.atualizarCamera({p.getX() + p.getTamanhoX() / 2, p.getY() + p.getTamanhoY() / 2}, largura, altura);
    int cooldown_dano=0;
    while (!WindowShouldClose()) {
        if(p.getVida()<=0) break;
        andar(p, mapa1, largura, altura);
        if(cooldown_dano==0){
            if(checa_tomar_dano(p, mapa1.getEspinhos()))cooldown_dano = 60;
        }
        if(cooldown_dano>0)cooldown_dano--;
        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawText("Use WASD para mover", 20, 20, 20, DARKGRAY);
        DrawText(
            TextFormat("Vida: %d | cooldown_dano: %d", p.getVida(), cooldown_dano),
            20,45,20,DARKGRAY
        );
        
        DrawText(
            TextFormat(
                "Jogador: x=%d y=%d | Camera: x=%d y=%d | Tamanho tela: largura=%d altura=%d",
                p.getX(),
                p.getY(),
                mapa1.getCamera().first,
                mapa1.getCamera().second,
                largura,
                altura
            ),20,70,20,DARKGRAY
        );
        Rectangle sourceProtagonista = { // MODIFICAR AQUI PARA A ANIMAÇÃO -------------------------------------------------------------------------------------
            0,
            0,
            (float)texturaProtagonista.width,
            (float)texturaProtagonista.height
        };

        Rectangle destProtagonista = {
            (float)(p.getX() - mapa1.getCamera().first),
            (float)(p.getY() - mapa1.getCamera().second),
            (float)p.getTamanhoX(),
            (float)p.getTamanhoY()
        };

        DrawTexturePro(
            texturaProtagonista,
            sourceProtagonista,
            destProtagonista,
            {0, 0},
            0.0f,
            WHITE
        );
        for(Espinho e : mapa1.getEspinhos())
            if(CheckCollisionRecs(
                {
                    (float)mapa1.getCamera().first,
                    (float)mapa1.getCamera().second,
                    (float)largura,
                    (float)altura
                },
                {
                    (float)e.getX(),
                    (float)e.getY(),
                    (float)e.getTamanhoX(),
                    (float)e.getTamanhoY()
                }
            ))
                DrawRectangle(e.getX() - mapa1.getCamera().first, e.getY() - mapa1.getCamera().second, e.getTamanhoX(), e.getTamanhoY(), BLACK);
        for(Parede e : mapa1.getParedes())
            if(CheckCollisionRecs(
                {
                    (float)mapa1.getCamera().first,
                    (float)mapa1.getCamera().second,
                    (float)largura,
                    (float)altura
                },
                {
                    (float)e.getX(),
                    (float)e.getY(),
                    (float)e.getTamanhoX(),
                    (float)e.getTamanhoY()
                }
            ))
                DrawRectangle(
                    e.getX() - mapa1.getCamera().first,
                    e.getY() - mapa1.getCamera().second,
                    e.getTamanhoX(),
                    e.getTamanhoY(),
                    GREEN
                );
        
            EndDrawing();
    }
    UnloadTexture(texturaProtagonista);
    CloseWindow();

    return 0;
}
