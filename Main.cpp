#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <raylib.h>

#include "DamageArea/DamageArea.h"
#include "Projetil/Projetil.h"
#include "Mapa/Mapa.h"
#include "Protagonista/Protagonista.h"
#include "Inimigo/Inimigo.h"
#include "Inimigo/Inimigo_melee/Inimigo_melee.h"
#include "Inimigo/Inimigo_ranged/Inimigo_ranged.h"
#include "Item/Item_Arma_Ranged/Item_Arma_Ranged.h"
#include "Item/Item.h"
#include "Dialogo/Dialogo.h"
#include "NPC/NPC.h"
#include <cstdlib>
#include <ctime>

#include "funcoes_gameplay.h"
#include "desenho.h"

using namespace std;

enum StatusJogo {
    DIALOGO,
    JOGANDO,
    CUTSCENE,
    PAUSADO,
    MOCHILA
};

int current_map = 0;

vector<Mapa> mapas = {

};

int main() {
    StatusJogo status = JOGANDO;
    srand(time(NULL));
    // CONFIGURAÇÕES INICIAIS
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(1280, 720, "Jogo");

    int monitor = GetCurrentMonitor();
    int largura = GetMonitorWidth(monitor);
    int altura = GetMonitorHeight(monitor);

    SetWindowSize(largura, altura);
    SetWindowPosition(0, 0);
    SetWindowState(FLAG_WINDOW_UNDECORATED);

    SetTargetFPS(60);

    // VARIÁVEIS
    Item_Arma_Ranged arma_prota_ranged(
        "arma default",
        "Comum",
        0.0,
        1,
        2,
        "arco.png"
    );
    Item_Arma_Ranged arma_drop(
        "arma dropada",
        "Comum",
        0.6,
        1,
        2,
        "arco.png"
    );
    

    Protagonista p(
        4,
        {100, 100},
        {100, 100},
        10,
        100,
        arma_prota_ranged,
        5
    );

    p.adiciona_mochila(arma_prota_ranged);
    p.adiciona_mochila(arma_prota_ranged);
    p.adiciona_mochila(arma_prota_ranged);

    Texture2D texturaProtagonista = LoadTexture("Protagonista/Assets/sapo_sentado.jpg");
    Texture2D fundo = LoadTexture("Assets_gerais/background.png");

    vector<Projetil> projeteis;
    vector<Item> itens;
    vector<Item> dropsdosmobs;
    dropsdosmobs.push_back(arma_drop);
    dropsdosmobs.push_back(arma_drop);
    dropsdosmobs.push_back(arma_drop);
    vector<unique_ptr<Inimigo>> inimigos;

    inimigos.push_back(
        make_unique<Inimigo_ranged>(
            pair<int, int>{600, 100},
            pair<int, int>{100, 100},
            3,
            true,
            1,
            500,
            3,
            dropsdosmobs
        )
    );
    
    inimigos.push_back(
        make_unique<Inimigo_melee>(
            pair<int, int>{900, 300},
            pair<int, int>{100, 100},
            4,
            false,
            1,
            80,
            3,
            dropsdosmobs
        )
    );

    vector<DamageArea> eMapa1 = {
        {1, {200, 200}, {50, 50}, false},
        {1, {300, 1200}, {50, 50}, false},
        {1, {400, 900}, {50, 50}, false},
        {1, {500, 700}, {50, 50}, false}
    };

    vector<Parede> pMapa1 = {
        {{200, 500}, {100, 50}},
        {{300, 100}, {100, 50}},
        {{400, 500}, {100, 50}},
        {{900, 900}, {200, 50}}
    };

    vector<string> falas = {
        "Olá",
        "Bom dia",
        "Como está?"
    };

    Dialogo dialogo1(falas);

    vector<Dialogo> dialogos;
    dialogos.push_back(dialogo1);

    NPC npc1({1500, 1500}, {100, 100}, dialogos);
    NPC npc2({1900, 1900}, {100, 100}, dialogos);
    NPC npc3({2100, 1500}, {100, 100}, dialogos);
    
    vector<pair<int,int>> posicoesItens;
    vector<NPC> npcMapa1 = {
        npc1,
        npc2,
        npc3
    };

    Mapa mapa1 = {
        {3000, 3000},
        {0, 0},
        eMapa1,
        pMapa1,
        npcMapa1
    };

    mapa1.atualizarCamera(
        {
            p.getX() + p.getTamanhoX() / 2,
            p.getY() + p.getTamanhoY() / 2
        },
        largura,
        altura
    );

    int cooldown_dano = 0;
    int cooldown_interacao = 0;
    int itemSelecionado = 0;
    string s = "nada";

    while (!WindowShouldClose()) {
        if (p.getVida() <= 0) {
            break;
        }

        if (status == JOGANDO) {
            if (IsKeyDown(KEY_E)) {
                if (cooldown_interacao <= 0) {
                    for (NPC& n : mapa1.getNPCs()) {
                        if (CheckCollisionRecs(
                            {
                                (float)n.getX(),
                                (float)n.getY(),
                                (float)n.getTamanhoX(),
                                (float)n.getTamanhoY()
                            },
                            {
                                (float)p.getX(),
                                (float)p.getY(),
                                (float)p.getTamanhoX(),
                                (float)p.getTamanhoY()
                            }
                        )) {
                            n.getNextDialogo().index = 0;
                            status = DIALOGO;
                        }
                    }
                }
            }

            if (IsKeyPressed(KEY_B)) {
                status = MOCHILA;
            }
            checa_pegar_itens(p, itens, posicoesItens);

            for (int i = (int)inimigos.size() - 1; i >= 0; i--) {
                if (inimigos[i]->getVida() <= 0) {
                    vector<Item> dropados = inimigos[i]->dropar();
                    itens.insert(itens.end(), dropados.begin(), dropados.end());
                    int totalDrops = dropados.size();
                    int espacamento = 55;

                    for (int j = 0; j < totalDrops; j++) {
                        int coluna = j % 3;
                        int linha = j / 3;

                        int colunasNaLinha = totalDrops < 3 ? totalDrops : 3;

                        int offsetX = (coluna - (colunasNaLinha - 1) / 2.0f) * espacamento;
                        int offsetY = linha * espacamento;

                        posicoesItens.push_back({
                            inimigos[i]->getX() + offsetX,
                            inimigos[i]->getY() + offsetY
                        });
                    }
                    inimigos.erase(inimigos.begin() + i);
                }
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePos = GetMousePosition();

                DamageArea hitProta = p.bater_melee(
                    mousePos.x + mapa1.getCamera().first,
                    mousePos.y + mapa1.getCamera().second
                );

                checa_dar_dano_melee(
                    inimigos,
                    hitProta
                );

                mapa1.addDamageAreas(hitProta);
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                Vector2 mousePos = GetMousePosition();

                Projetil hitProta = p.bater_Ranged(
                    mousePos.x + mapa1.getCamera().first,
                    mousePos.y + mapa1.getCamera().second
                );

                projeteis.push_back(hitProta);
            }

            for (size_t i = 0; i < projeteis.size(); ) {
                projeteis[i].mover();

                if (checa_colisao_projetil(p, mapa1, projeteis[i], inimigos)) {
                    projeteis.erase(projeteis.begin() + i);
                } else {
                    i++;
                }
            }

            andar(p, mapa1, largura, altura);

            if (cooldown_interacao > 0) {
                cooldown_interacao--;
            }

            for (unique_ptr<Inimigo>& i : inimigos) {
                AcaoInimigo acao = i->mover(p);

                if (acao.projetil.has_value()) {
                    projeteis.push_back(*acao.projetil);
                }

                if (acao.areaDano.has_value()) {
                    mapa1.addDamageAreas(*acao.areaDano);
                }
            }

            if (cooldown_dano == 0) {
                if (checa_tomar_dano(p, mapa1.getDamageAreas())) {
                    cooldown_dano = 60;
                }
            }

            if (cooldown_dano > 0) {
                cooldown_dano--;
            }

            BeginDrawing();

            ClearBackground(RAYWHITE);

            desenharFundo(fundo, mapa1, largura, altura);

            desenha_mapa_e_prota(
                mapa1,
                p,
                largura,
                altura,
                texturaProtagonista
            );

            desenharInimigos(inimigos, mapa1);

            desenharProjeteis(projeteis, mapa1);
            desenharItens(itens, posicoesItens, mapa1);

            desenharHUD(
                p,
                mapa1,
                largura,
                altura,
                cooldown_dano,
                cooldown_interacao,
                s,
                inimigos
            );
            desenharHotbar(p, largura, altura);
            EndDrawing();

            for (int i = mapa1.getDamageAreas().size() - 1; i >= 0; i--) {
                if (mapa1.getDamageAreas()[i].temporaria) {
                    mapa1.removeDamageArea(i);
                }
            }
        }

        else if (status == DIALOGO) {
            if (IsKeyDown(KEY_E)) {
                if (cooldown_interacao <= 0) {
                    for (NPC& n : mapa1.getNPCs()) {
                        if (CheckCollisionRecs(
                            {
                                (float)n.getX(),
                                (float)n.getY(),
                                (float)n.getTamanhoX(),
                                (float)n.getTamanhoY()
                            },
                            {
                                (float)p.getX(),
                                (float)p.getY(),
                                (float)p.getTamanhoX(),
                                (float)p.getTamanhoY()
                            }
                        )) {
                            if (n.getNextDialogo().index >= n.getNextDialogo().getFalas().size()) {
                                n.getNextDialogo().index = 0;
                                cooldown_interacao = 30;
                                status = JOGANDO;
                            } else {
                                s = n.getNextDialogo().getFalas()[n.getNextDialogo().index];
                                n.getNextDialogo().index++;
                            }
                        }
                    }

                    cooldown_interacao = 30;
                }
            }

            if (cooldown_interacao > 0) {
                cooldown_interacao--;
            }

            BeginDrawing();

            ClearBackground(RAYWHITE);

            desenharFundo(fundo, mapa1, largura, altura);

            desenha_mapa_e_prota(
                mapa1,
                p,
                largura,
                altura,
                texturaProtagonista
            );

            desenharInimigos(inimigos, mapa1);

            desenharProjeteis(projeteis, mapa1);

            desenharHUD(
                p,
                mapa1,
                largura,
                altura,
                cooldown_dano,
                cooldown_interacao,
                s,
                inimigos
            );

            desenharDialogo(s, largura, altura);

            EndDrawing();
        }

        else if (status == MOCHILA) {
            if (IsKeyPressed(KEY_B)) {
                status = JOGANDO;
            }
            if(IsKeyPressed(KEY_TAB))
                if(itemSelecionado>=p.getTamMochila() || itemSelecionado>=p.getMochila().size()-1)
                    itemSelecionado=0;
                else
                    itemSelecionado++;
            BeginDrawing();

            ClearBackground(RAYWHITE);

            desenharFundo(fundo, mapa1, largura, altura);

            desenha_mapa_e_prota(
                mapa1,
                p,
                largura,
                altura,
                texturaProtagonista
            );

            desenharInimigos(inimigos, mapa1);

            desenharProjeteis(projeteis, mapa1);

            desenharMochila(p, largura, altura, itemSelecionado);

            EndDrawing();
        }

        else if (status == CUTSCENE) {
            BeginDrawing();

            ClearBackground(RAYWHITE);

            desenharFundo(fundo, mapa1, largura, altura);

            desenha_mapa_e_prota(
                mapa1,
                p,
                largura,
                altura,
                texturaProtagonista
            );

            desenharInimigos(inimigos, mapa1);

            desenharProjeteis(projeteis, mapa1);

            EndDrawing();
        }

        else if (status == PAUSADO) {
            BeginDrawing();

            ClearBackground(RAYWHITE);

            desenharFundo(fundo, mapa1, largura, altura);

            desenha_mapa_e_prota(
                mapa1,
                p,
                largura,
                altura,
                texturaProtagonista
            );

            desenharInimigos(inimigos, mapa1);

            desenharProjeteis(projeteis, mapa1);

            DrawRectangle(0, 0, largura, altura, Fade(BLACK, 0.5f));
            DrawText("PAUSADO", largura / 2 - 80, altura / 2 - 20, 40, RAYWHITE);

            EndDrawing();
        }
    }

    UnloadTexture(texturaProtagonista);
    UnloadTexture(fundo);

    CloseWindow();

    return 0;
}