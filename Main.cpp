#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <raylib.h>

#include "DamageArea/DamageArea.h"
#include "Projetil/Projetil.h"
#include "Mapa/Mapa.h"
#include "Protagonista/Protagonista.h"
#include "Inimigo/Inimigo.h"
#include "Dialogo/Dialogo.h"
#include "NPC/NPC.h"
#include "Main.h"

using namespace std;


int current_map = 0;
vector<Mapa> mapas = {

};

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

bool checa_colisao_projetil(Protagonista& p, Mapa mapa, Projetil& bala){
    for(Parede par : mapa.getParedes())
        if (CheckCollisionRecs(
            {
                (float)bala.getX(),
                (float)bala.getY(),
                (float)bala.getTamanhoX(),
                (float)bala.getTamanhoY()
            },
            {
                (float)par.getX(),
                (float)par.getY(),
                (float)par.getTamanhoX(),
                (float)par.getTamanhoY()
            }
        )){
            bala.ricochetear(par);
            if(bala.ricochetes==0){
                return true;
            }
        }
    if(CheckCollisionRecs(
        {
            (float)bala.getX(),
            (float)bala.getY(),
            (float)bala.getTamanhoX(),
            (float)bala.getTamanhoY()
        },
        {
            (float)p.getX(),
            (float)p.getY(),
            (float)p.getTamanhoX(),
            (float)p.getTamanhoY()
        }
    )){
        p.alteraVida(-bala.getDano());
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

void desenha_mapa_e_prota(Mapa mapa1, Protagonista p, int largura, int altura, Texture2D texturaProtagonista){
    
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
    for(DamageArea e : mapa1.getDamageAreas())
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
    DrawRectangle(e.getX() - mapa1.getCamera().first, e.getY() - mapa1.getCamera().second, e.getTamanhoX(), e.getTamanhoY(), RED);
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
    for(NPC n : mapa1.getNPCs())
        if(CheckCollisionRecs(
            {
                (float)mapa1.getCamera().first,
                (float)mapa1.getCamera().second,
                (float)largura,
                (float)altura
            },
            {
                (float)n.getX(),
                (float)n.getY(),
                (float)n.getTamanhoX(),
                (float)n.getTamanhoY()
            }
        ))
            DrawRectangle(
                n.getX() - mapa1.getCamera().first,
                n.getY() - mapa1.getCamera().second,
                n.getTamanhoX(),
                n.getTamanhoY(),
                BLACK
            );
}

bool checa_dar_dano(vector<Inimigo>& inimigos, DamageArea dano){
    for(Inimigo& e : inimigos)
        if (CheckCollisionRecs(
            {
                (float)dano.getX(),
                (float)dano.getY(),
                (float)dano.getTamanhoX(),
                (float)dano.getTamanhoY()
            },
            {
                (float)e.getX(),
                (float)e.getY(),
                (float)e.getTamanhoX(),
                (float)e.getTamanhoY()
            }
        )){
            e.alteraVida(dano.getDano()*-1);
            return true;
        }
    return false;
}

bool checa_tomar_dano(Protagonista& p, vector<DamageArea> areas_de_dano){
    for(DamageArea e : areas_de_dano)
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
enum StatusJogo {
    DIALOGO,
    JOGANDO,
    CUTSCENE,
    PAUSADO
};
int main() {
    
    StatusJogo status = JOGANDO;
    // CONFIGURAÇÕES INICIAIS
    InitWindow(800, 600, "Jogo");
    int monitor = GetCurrentMonitor();
    int largura = GetMonitorWidth(monitor);
    int altura = GetMonitorHeight(monitor);
    SetWindowSize(largura, altura);
    ToggleFullscreen();
    SetTargetFPS(60);

    

    // VARIÁVEIS
    Protagonista p(4,{100,100}, {100,100},10, 100);
    Texture2D texturaProtagonista = LoadTexture("Protagonista/Assets/sapo_sentado.jpg");
    vector<Projetil> projeteis;
    Inimigo inimigo({600,100}, {100,100}, 3, true, 1, 5, 3);
    vector<Inimigo> inimigos;
    inimigos.push_back(inimigo);
    vector<DamageArea> eMapa1 = {
        {1,{200,200}, {50,50}, false},
        {1,{300,1200}, {50,50}, false},
        {1,{400,900}, {50,50}, false},
        {1,{500,700}, {50,50}, false}
    };
    vector<Parede> pMapa1 = {
        {{200,500}, {100,50}},
        {{300,100}, {100,50}},
        {{400,500}, {100,50}},
        {{900,900}, {200,50}}
    };
    vector<string> falas = {"Olá", "Bom dia", "Como está?"};
    Dialogo dialogo1(falas);
    vector<Dialogo> dialogos;
    dialogos.push_back(dialogo1);
    NPC npc1({1500,1500}, {100,100}, dialogos);
    NPC npc2({1900,1900}, {100,100}, dialogos);
    NPC npc3({2100,1500}, {100,100}, dialogos);

    vector<NPC> npcMapa1 = {npc1, npc2, npc3};
    Mapa mapa1 = {{3000,3000},{0,0}, eMapa1, pMapa1, npcMapa1};
    mapa1.atualizarCamera({p.getX() + p.getTamanhoX() / 2, p.getY() + p.getTamanhoY() / 2}, largura, altura);
    int cooldown_dano=0;
    int cooldown_interacao = 0;
    string s = "nada";
    while (!WindowShouldClose()) {
        
        if(p.getVida()<=0) break; // verifica se morreu

        if(status == JOGANDO){
            
            
            if(IsKeyDown(KEY_E)){
                if(cooldown_interacao<=0){
                    for(NPC& n : mapa1.getNPCs())
                        if(CheckCollisionRecs(
                            {(float)n.getX(),(float)n.getY(),(float)n.getTamanhoX(),(float)n.getTamanhoY()},
                            {(float)p.getX(),(float)p.getY(),(float)p.getTamanhoX(),(float)p.getTamanhoY()}
                        )){
                            n.getNextDialogo().index = 0;
                            status = DIALOGO;
                        }
                }
            }

            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                checa_dar_dano(inimigos,p.bater_melee());
            }
            int iPr=0;
            for(Projetil& pr : projeteis){
                pr.mover();
                if(checa_colisao_projetil(p,mapa1,pr)){
                    projeteis.erase(projeteis.begin() + iPr);
                }
                iPr++;
            }

            andar(p, mapa1, largura, altura);
            
            if(cooldown_dano==0){
                if(checa_tomar_dano(p, mapa1.getDamageAreas()))cooldown_dano = 60;
            }
            if(cooldown_dano>0)cooldown_dano--;
            if(cooldown_interacao>0) cooldown_interacao--;
            for(Inimigo& i : inimigos) i.mover(p);
            BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawText("Use WASD para mover", 20, 20, 20, DARKGRAY);
            DrawText(
                TextFormat("Vida: %d | cooldown_dano: %d | cooldown_interacao: %d", p.getVida(), cooldown_dano, cooldown_interacao),
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
            DrawText(s.c_str(),20,95,20,DARKGRAY);
            DrawText(
                TextFormat("Vida Inimigo: %d",inimigos[0].getVida()),
                20,120,20,DARKGRAY
            );

            desenha_mapa_e_prota(mapa1,p,largura,altura, texturaProtagonista);
            for(Inimigo i : inimigos){
                DrawRectangle(
                    i.getX() - mapa1.getCamera().first,
                    i.getY() - mapa1.getCamera().second,
                    i.getTamanhoX(),
                    i.getTamanhoY(),
                    BLACK
                );
            }
            for(Projetil pr : projeteis){
                DrawRectangle(
                    pr.getX() - mapa1.getCamera().first,
                    pr.getY() - mapa1.getCamera().second,
                    pr.getTamanhoX(),
                    pr.getTamanhoY(),
                    BLACK
                );
            }
            EndDrawing();


        }
        if(status == DIALOGO){
            if(IsKeyDown(KEY_E)){
                if(cooldown_interacao<=0){
                    for(NPC& n : mapa1.getNPCs())
                        if(CheckCollisionRecs(
                            {(float)n.getX(),(float)n.getY(),(float)n.getTamanhoX(),(float)n.getTamanhoY()},
                            {(float)p.getX(),(float)p.getY(),(float)p.getTamanhoX(),(float)p.getTamanhoY()}
                        )){
                            
                            if(n.getNextDialogo().index >= n.getNextDialogo().getFalas().size()){
                                n.getNextDialogo().index = 0;
                                cooldown_interacao =30;
                                status = JOGANDO;
                            }
                            s = n.getNextDialogo().getFalas()[n.getNextDialogo().index];
                            n.getNextDialogo().index++;
                        }
                    cooldown_interacao = 30;
                }
            }
            if(cooldown_interacao>0) cooldown_interacao--;
            BeginDrawing();

            ClearBackground(RAYWHITE);
            DrawText("Use WASD para mover", 20, 20, 20, DARKGRAY);
            DrawText(
                TextFormat("Vida: %d | cooldown_dano: %d | cooldown_interacao: %d", p.getVida(), cooldown_dano, cooldown_interacao),
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
            
            desenha_mapa_e_prota(mapa1,p,largura,altura, texturaProtagonista);
            DrawRectangle(100, altura-400, 2*largura/3, 100, BLUE);
            DrawText(s.c_str(),200,altura-350,20,BLACK);
            EndDrawing();
        }
    }
    UnloadTexture(texturaProtagonista);
    CloseWindow();

    return 0;
}
