#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <optional>
#include <cstdlib>
#include <ctime>

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

#include "funcoes_gameplay.h"
#include "desenho.h"
#include "DadosJogo.h"

using namespace std;

enum StatusJogo {
    MENU,
    DIALOGO,
    JOGANDO,
    CUTSCENE,
    PAUSADO,
    MOCHILA,
    GAME_OVER,
    VITORIA
};

void processarMortesInimigos(
    vector<unique_ptr<Inimigo>>& inimigos,
    vector<Item>& itens,
    vector<pair<int,int>>& posicoesItens
) {
    for (int i = (int)inimigos.size() - 1; i >= 0; i--) {
        if (inimigos[i]->getVida() <= 0) {
            vector<Item> dropados = inimigos[i]->dropar();

            itens.insert(
                itens.end(),
                dropados.begin(),
                dropados.end()
            );

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
}

int main() {
    srand(time(NULL));

    StatusJogo status = MENU;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(1280, 720, "Jogo");

    SetExitKey(KEY_NULL);

    int monitor = GetCurrentMonitor();
    int largura = GetMonitorWidth(monitor);
    int altura = GetMonitorHeight(monitor);

    SetWindowSize(largura, altura);
    SetWindowPosition(0, 0);
    SetWindowState(FLAG_WINDOW_UNDECORATED);

    SetTargetFPS(60);

    Texture2D texturaProtagonista = LoadTexture("Protagonista/Assets/sapo_sentado.jpg");
    Texture2D fundo = LoadTexture("Assets_gerais/background.png");

    unique_ptr<DadosJogo> jogoAtual = nullptr;

    while (!WindowShouldClose()) {
        if (status == MENU) {
            BeginDrawing();

            ClearBackground(RAYWHITE);

            AcaoMenu acao = desenharMenuPrincipal(largura, altura);

            EndDrawing();

            if (acao == ACAO_JOGAR) {
                jogoAtual = make_unique<DadosJogo>(
                    criarNovoJogo(largura, altura)
                );

                status = JOGANDO;
            }
            else if (acao == ACAO_CONTINUAR) {
                // Futuramente:
                // jogoAtual = make_unique<DadosJogo>(
                //     criarJogoDoSave("save.json", largura, altura)
                // );
                // status = JOGANDO;
            }
            else if (acao == ACAO_SAIR) {
                break;
            }
        }

        else if (status == JOGANDO) {
            if (jogoAtual == nullptr) {
                status = MENU;
                continue;
            }

            DadosJogo& jogo = *jogoAtual;

            if (jogo.p.getVida() <= 0) {
                status = GAME_OVER;
                continue;
            }

            if (IsKeyPressed(KEY_ESCAPE)) {
                status = PAUSADO;
                continue;
            }

            if (IsKeyDown(KEY_E)) {
                if (jogo.cooldown_interacao <= 0) {
                    for (NPC& n : jogo.mapa1.getNPCs()) {
                        if (CheckCollisionRecs(
                            {
                                (float)n.getX(),
                                (float)n.getY(),
                                (float)n.getTamanhoX(),
                                (float)n.getTamanhoY()
                            },
                            {
                                (float)jogo.p.getX(),
                                (float)jogo.p.getY(),
                                (float)jogo.p.getTamanhoX(),
                                (float)jogo.p.getTamanhoY()
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
                continue;
            }

            checa_pegar_itens(
                jogo.p,
                jogo.itens,
                jogo.posicoesItens
            );

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                Vector2 mousePos = GetMousePosition();

                DamageArea hitProta = jogo.p.bater_melee(
                    mousePos.x + jogo.mapa1.getCamera().first,
                    mousePos.y + jogo.mapa1.getCamera().second
                );

                checa_dar_dano_melee(
                    jogo.inimigos,
                    hitProta
                );

                jogo.mapa1.addDamageAreas(hitProta);
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                Vector2 mousePos = GetMousePosition();

                optional<Projetil> hitProta = jogo.p.bater_Ranged(
                    mousePos.x + jogo.mapa1.getCamera().first,
                    mousePos.y + jogo.mapa1.getCamera().second
                );

                if (hitProta.has_value()) {
                    jogo.projeteis.push_back(*hitProta);
                }
            }

            for (size_t i = 0; i < jogo.projeteis.size(); ) {
                jogo.projeteis[i].mover();

                if (checa_colisao_projetil(
                    jogo.p,
                    jogo.mapa1,
                    jogo.projeteis[i],
                    jogo.inimigos
                )) {
                    jogo.projeteis.erase(jogo.projeteis.begin() + i);
                } else {
                    i++;
                }
            }

            processarMortesInimigos(
                jogo.inimigos,
                jogo.itens,
                jogo.posicoesItens
            );

            andar(
                jogo.p,
                jogo.mapa1,
                largura,
                altura
            );

            if (jogo.cooldown_interacao > 0) {
                jogo.cooldown_interacao--;
            }

            for (unique_ptr<Inimigo>& i : jogo.inimigos) {
                AcaoInimigo acao = i->mover(jogo.p);

                if (acao.projetil.has_value()) {
                    jogo.projeteis.push_back(*acao.projetil);
                }

                if (acao.areaDano.has_value()) {
                    jogo.mapa1.addDamageAreas(*acao.areaDano);
                }
            }

            if (jogo.cooldown_dano == 0) {
                if (checa_tomar_dano(jogo.p, jogo.mapa1.getDamageAreas())) {
                    jogo.cooldown_dano = 60;
                }
            }

            if (jogo.cooldown_dano > 0) {
                jogo.cooldown_dano--;
            }

            BeginDrawing();

            ClearBackground(RAYWHITE);

            desenharFundo(
                fundo,
                jogo.mapa1,
                largura,
                altura
            );

            desenha_mapa_e_prota(
                jogo.mapa1,
                jogo.p,
                largura,
                altura,
                texturaProtagonista
            );

            desenharInimigos(
                jogo.inimigos,
                jogo.mapa1
            );

            desenharProjeteis(
                jogo.projeteis,
                jogo.mapa1
            );

            desenharItens(
                jogo.itens,
                jogo.posicoesItens,
                jogo.mapa1
            );

            desenharHUD(
                jogo.p,
                jogo.mapa1,
                largura,
                altura,
                jogo.cooldown_dano,
                jogo.cooldown_interacao,
                jogo.falaAtual,
                jogo.inimigos
            );

            desenharHotbar(
                jogo.p,
                largura,
                altura
            );

            EndDrawing();

            for (int i = jogo.mapa1.getDamageAreas().size() - 1; i >= 0; i--) {
                if (jogo.mapa1.getDamageAreas()[i].temporaria) {
                    jogo.mapa1.removeDamageArea(i);
                }
            }
        }

        else if (status == DIALOGO) {
            if (jogoAtual == nullptr) {
                status = MENU;
                continue;
            }

            DadosJogo& jogo = *jogoAtual;

            if (IsKeyDown(KEY_E)) {
                if (jogo.cooldown_interacao <= 0) {
                    for (NPC& n : jogo.mapa1.getNPCs()) {
                        if (CheckCollisionRecs(
                            {
                                (float)n.getX(),
                                (float)n.getY(),
                                (float)n.getTamanhoX(),
                                (float)n.getTamanhoY()
                            },
                            {
                                (float)jogo.p.getX(),
                                (float)jogo.p.getY(),
                                (float)jogo.p.getTamanhoX(),
                                (float)jogo.p.getTamanhoY()
                            }
                        )) {
                            if (n.getNextDialogo().index >= n.getNextDialogo().getFalas().size()) {
                                n.getNextDialogo().index = 0;
                                jogo.cooldown_interacao = 30;
                                status = JOGANDO;
                            } else {
                                jogo.falaAtual = n.getNextDialogo().getFalas()[n.getNextDialogo().index];
                                n.getNextDialogo().index++;
                            }
                        }
                    }

                    jogo.cooldown_interacao = 30;
                }
            }

            if (jogo.cooldown_interacao > 0) {
                jogo.cooldown_interacao--;
            }

            BeginDrawing();

            ClearBackground(RAYWHITE);

            desenharFundo(
                fundo,
                jogo.mapa1,
                largura,
                altura
            );

            desenha_mapa_e_prota(
                jogo.mapa1,
                jogo.p,
                largura,
                altura,
                texturaProtagonista
            );

            desenharInimigos(
                jogo.inimigos,
                jogo.mapa1
            );

            desenharProjeteis(
                jogo.projeteis,
                jogo.mapa1
            );

            desenharItens(
                jogo.itens,
                jogo.posicoesItens,
                jogo.mapa1
            );

            desenharHUD(
                jogo.p,
                jogo.mapa1,
                largura,
                altura,
                jogo.cooldown_dano,
                jogo.cooldown_interacao,
                jogo.falaAtual,
                jogo.inimigos
            );

            desenharDialogo(
                jogo.falaAtual,
                largura,
                altura
            );

            EndDrawing();
        }

        else if (status == MOCHILA) {
            if (jogoAtual == nullptr) {
                status = MENU;
                continue;
            }

            DadosJogo& jogo = *jogoAtual;

            if (IsKeyPressed(KEY_B)) {
                status = JOGANDO;
                continue;
            }

            if (IsKeyPressed(KEY_TAB)) {
                if (
                    jogo.itemSelecionado >= jogo.p.getTamMochila() ||
                    jogo.itemSelecionado >= (int)jogo.p.getMochila().size() - 1
                ) {
                    jogo.itemSelecionado = 0;
                } else {
                    jogo.itemSelecionado++;
                }
            }

            if (IsKeyPressed(KEY_Q)) {
                jogo.abaMochila = 0;
            }

            if (IsKeyPressed(KEY_E)) {
                jogo.abaMochila = 1;
            }

            BeginDrawing();

            ClearBackground(RAYWHITE);

            desenharFundo(
                fundo,
                jogo.mapa1,
                largura,
                altura
            );

            desenha_mapa_e_prota(
                jogo.mapa1,
                jogo.p,
                largura,
                altura,
                texturaProtagonista
            );

            desenharInimigos(
                jogo.inimigos,
                jogo.mapa1
            );

            desenharProjeteis(
                jogo.projeteis,
                jogo.mapa1
            );

            desenharItens(
                jogo.itens,
                jogo.posicoesItens,
                jogo.mapa1
            );

            desenharMochila(
                jogo.p,
                largura,
                altura,
                jogo.itemSelecionado,
                jogo.abaMochila
            );

            EndDrawing();
        }

        else if (status == CUTSCENE) {
            if (jogoAtual == nullptr) {
                status = MENU;
                continue;
            }

            DadosJogo& jogo = *jogoAtual;

            BeginDrawing();

            ClearBackground(RAYWHITE);

            desenharFundo(
                fundo,
                jogo.mapa1,
                largura,
                altura
            );

            desenha_mapa_e_prota(
                jogo.mapa1,
                jogo.p,
                largura,
                altura,
                texturaProtagonista
            );

            desenharInimigos(
                jogo.inimigos,
                jogo.mapa1
            );

            desenharProjeteis(
                jogo.projeteis,
                jogo.mapa1
            );

            desenharItens(
                jogo.itens,
                jogo.posicoesItens,
                jogo.mapa1
            );

            EndDrawing();
        }

        else if (status == PAUSADO) {
            if (jogoAtual == nullptr) {
                status = MENU;
                continue;
            }

            DadosJogo& jogo = *jogoAtual;

            BeginDrawing();

            ClearBackground(RAYWHITE);

            desenharFundo(
                fundo,
                jogo.mapa1,
                largura,
                altura
            );

            desenha_mapa_e_prota(
                jogo.mapa1,
                jogo.p,
                largura,
                altura,
                texturaProtagonista
            );

            desenharInimigos(
                jogo.inimigos,
                jogo.mapa1
            );

            desenharProjeteis(
                jogo.projeteis,
                jogo.mapa1
            );

            desenharItens(
                jogo.itens,
                jogo.posicoesItens,
                jogo.mapa1
            );

            AcaoMenu acao = desenharMenuPause(
                largura,
                altura
            );

            EndDrawing();

            if (acao == ACAO_CONTINUAR) {
                status = JOGANDO;
            }
            else if (acao == ACAO_VOLTAR_MENU) {
                jogoAtual.reset();
                status = MENU;
            }
            else if (acao == ACAO_SAIR) {
                break;
            }
        }

        else if (status == GAME_OVER) {
            BeginDrawing();

            ClearBackground(RAYWHITE);

            AcaoMenu acao = desenharMenuGameOver(
                largura,
                altura
            );

            EndDrawing();

            if (acao == ACAO_JOGAR_NOVAMENTE) {
                jogoAtual = make_unique<DadosJogo>(
                    criarNovoJogo(largura, altura)
                );

                status = JOGANDO;
            }
            else if (acao == ACAO_VOLTAR_MENU) {
                jogoAtual.reset();
                status = MENU;
            }
            else if (acao == ACAO_SAIR) {
                break;
            }
        }

        else if (status == VITORIA) {
            BeginDrawing();

            ClearBackground(RAYWHITE);

            AcaoMenu acao = desenharMenuVitoria(
                largura,
                altura
            );

            EndDrawing();

            if (acao == ACAO_VOLTAR_MENU) {
                jogoAtual.reset();
                status = MENU;
            }
            else if (acao == ACAO_SAIR) {
                break;
            }
        }
    }

    UnloadTexture(texturaProtagonista);
    UnloadTexture(fundo);
    descarregarTexturasItens();

    CloseWindow();

    return 0;
}