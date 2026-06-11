#include <iostream>
#include <string>
#include <vector>
#include <raylib.h>

#include "Espinho/Espinho.h"
#include "Projetil/Projetil.h"
#include "Mapa/Mapa.h"
#include "Protagonista/Protagonista.h"
#include "Main.h"

using namespace std;

void andar(Protagonista& p, Mapa& mapa, int largura, int altura){
    if (IsKeyDown(KEY_D)){
        if(mapa.getPosicao_protagonista().first<mapa.getTamanho().first){
            if((mapa.getPosicao_protagonista().first >= (mapa.getTamanho().first - largura/2)) || p.getX()<largura/2){
                p.andarX(p.getMS());
                mapa.movePPX(p.getMS());
            }
            else{mapa.moverX(p.getMS());}
        }
    }
    if (IsKeyDown(KEY_A)){
        if(mapa.getPosicao_protagonista().first>0){
            if((mapa.getPosicao_protagonista().first <= largura/2) || p.getX()> largura/2){
                p.andarX(-p.getMS());
                mapa.movePPX(-p.getMS());
            }
            else{mapa.moverX(-p.getMS());}
        }
    }
    if (IsKeyDown(KEY_S)){
        if(mapa.getPosicao_protagonista().second<mapa.getTamanho().second){
            if((mapa.getPosicao_protagonista().second >= (mapa.getTamanho().second - altura/2)) || p.getY()<altura/2){
                p.andarY(p.getMS());
                mapa.movePPY(p.getMS());
            }
            else{mapa.moverY(p.getMS());}
        }
    }
    if (IsKeyDown(KEY_W)){
        if(mapa.getPosicao_protagonista().second>0){
            if((mapa.getPosicao_protagonista().second <= altura/2) || p.getY()> altura/2){
                p.andarY(-p.getMS());
                mapa.movePPY(-p.getMS());
            }
            else{mapa.moverY(-p.getMS());}
        }
    }
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
    Mapa mapa1 = {{3000,3000},{p.getX(),p.getY()}, eMapa1};
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
                "Jogador: x=%d y=%d | Jogador no mundo: x=%d y=%d | Tamanho tela: largura=%d altura=%d",
                p.getX(),
                p.getY(),
                mapa1.getPosicao_protagonista().first,
                mapa1.getPosicao_protagonista().second,
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
            (float)p.getX(),
            (float)p.getY(),
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
            DrawRectangle(e.getX(), e.getY(), e.getTamanhoX(), e.getTamanhoY(), BLACK);
        EndDrawing();
    }
    UnloadTexture(texturaProtagonista);
    CloseWindow();

    return 0;
}