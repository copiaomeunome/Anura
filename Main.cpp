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
#include "Animacao.h"

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

int obterIndiceAnimacaoSeguro(DadosJogo& jogo) {
    int indiceAnimacao = (int)jogo.animacaoProta;

    if (indiceAnimacao < 0 || indiceAnimacao >= ANIM_TOTAL) {
        jogo.animacaoProta = ANIM_PARADO;
        indiceAnimacao = ANIM_PARADO;
    }

    return indiceAnimacao;
}

void atualizarAnimacaoAtual(
    DadosJogo& jogo,
    Animacao animacoes[]
) {
    int indiceAnimacao = obterIndiceAnimacaoSeguro(jogo);
    animacoes[indiceAnimacao].atualizar();
}

void trocarAnimacaoProta(
    DadosJogo& jogo,
    Animacao animacoes[],
    EstadoAnimacaoProta novaAnimacao
) {
    if ((int)novaAnimacao < 0 || (int)novaAnimacao >= ANIM_TOTAL) {
        novaAnimacao = ANIM_PARADO;
    }

    if (jogo.animacaoProta != novaAnimacao) {
        jogo.animacaoProta = novaAnimacao;
        animacoes[jogo.animacaoProta].reiniciar();
    }
}

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

void desenharMundoAtual(
    DadosJogo& jogo,
    Texture2D fundo,
    Animacao animacoesProta[],
    int largura,
    int altura
) {
    int indiceAnimacao = obterIndiceAnimacaoSeguro(jogo);

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
        animacoesProta[indiceAnimacao].getTextura(),
        animacoesProta[indiceAnimacao].getSource()
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

    Texture2D texturaProtaParado = LoadTexture("Protagonista/Assets/sapo_sentado.jpg");
    Texture2D texturaProtaAndando = LoadTexture("Protagonista/Assets/sapo_sentado.jpg");
    Texture2D texturaProtaAtaqueMelee = LoadTexture("Protagonista/Assets/sapo_sentado.jpg");
    Texture2D texturaProtaAtaqueRanged = LoadTexture("Protagonista/Assets/sapo_sentado.jpg");
    Texture2D texturaProtaDano = LoadTexture("Protagonista/Assets/sapo_sentado.jpg");
    Texture2D texturaProtaMorte = LoadTexture("Protagonista/Assets/sapo_sentado.jpg");

    Animacao animacoesProta[ANIM_TOTAL];

    animacoesProta[ANIM_PARADO] = Animacao(
        texturaProtaParado,
        1,
        0.25f,
        true
    );

    animacoesProta[ANIM_ANDANDO] = Animacao(
        texturaProtaAndando,
        1,
        0.12f,
        true
    );

    animacoesProta[ANIM_ATAQUE_MELEE] = Animacao(
        texturaProtaAtaqueMelee,
        1,
        0.08f,
        false
    );

    animacoesProta[ANIM_ATAQUE_RANGED] = Animacao(
        texturaProtaAtaqueRanged,
        1,
        0.08f,
        false
    );

    animacoesProta[ANIM_DANO] = Animacao(
        texturaProtaDano,
        1,
        0.10f,
        false
    );

    animacoesProta[ANIM_MORTE] = Animacao(
        texturaProtaMorte,
        1,
        0.15f,
        false
    );

    Texture2D fundo = LoadTexture("Assets_gerais/background.png");

    unique_ptr<DadosJogo> jogoAtual = nullptr;

    bool podeUsarB = true;

    while (!WindowShouldClose()) {
        if (IsKeyReleased(KEY_B)) {
            podeUsarB = true;
        }

        if (status == MENU) {
            BeginDrawing();

            ClearBackground(RAYWHITE);

            AcaoMenu acao = desenharMenuPrincipal(largura, altura);

            EndDrawing();

            if (acao == ACAO_JOGAR) {
                jogoAtual = make_unique<DadosJogo>(
                    criarNovoJogo(largura, altura)
                );

                jogoAtual->animacaoProta = ANIM_PARADO;
                animacoesProta[ANIM_PARADO].reiniciar();

                podeUsarB = true;

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
                trocarAnimacaoProta(
                    jogo,
                    animacoesProta,
                    ANIM_MORTE
                );

                status = GAME_OVER;
                continue;
            }

            if (IsKeyPressed(KEY_ESCAPE)) {
                status = PAUSADO;
                continue;
            }

            bool protagonistaAndando =
                IsKeyDown(KEY_W) ||
                IsKeyDown(KEY_A) ||
                IsKeyDown(KEY_S) ||
                IsKeyDown(KEY_D);

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

            if (IsKeyPressed(KEY_B) && podeUsarB) {
                podeUsarB = false;

                jogo.abaMochila = 0;
                jogo.itemSelecionado = 0;

                trocarAnimacaoProta(
                    jogo,
                    animacoesProta,
                    ANIM_PARADO
                );

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

                trocarAnimacaoProta(
                    jogo,
                    animacoesProta,
                    ANIM_ATAQUE_MELEE
                );
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
                Vector2 mousePos = GetMousePosition();

                optional<Projetil> hitProta = jogo.p.bater_Ranged(
                    mousePos.x + jogo.mapa1.getCamera().first,
                    mousePos.y + jogo.mapa1.getCamera().second
                );

                if (hitProta.has_value()) {
                    jogo.projeteis.push_back(*hitProta);

                    trocarAnimacaoProta(
                        jogo,
                        animacoesProta,
                        ANIM_ATAQUE_RANGED
                    );
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

                    trocarAnimacaoProta(
                        jogo,
                        animacoesProta,
                        ANIM_DANO
                    );
                }
            }

            if (jogo.cooldown_dano > 0) {
                jogo.cooldown_dano--;
            }

            bool animacaoDeAcao =
                jogo.animacaoProta == ANIM_ATAQUE_MELEE ||
                jogo.animacaoProta == ANIM_ATAQUE_RANGED ||
                jogo.animacaoProta == ANIM_DANO;

            if (animacaoDeAcao) {
                int indiceAnimacao = obterIndiceAnimacaoSeguro(jogo);

                if (animacoesProta[indiceAnimacao].terminou()) {
                    if (protagonistaAndando) {
                        trocarAnimacaoProta(
                            jogo,
                            animacoesProta,
                            ANIM_ANDANDO
                        );
                    } else {
                        trocarAnimacaoProta(
                            jogo,
                            animacoesProta,
                            ANIM_PARADO
                        );
                    }
                }
            } else {
                if (protagonistaAndando) {
                    trocarAnimacaoProta(
                        jogo,
                        animacoesProta,
                        ANIM_ANDANDO
                    );
                } else {
                    trocarAnimacaoProta(
                        jogo,
                        animacoesProta,
                        ANIM_PARADO
                    );
                }
            }

            atualizarAnimacaoAtual(
                jogo,
                animacoesProta
            );

            BeginDrawing();

            ClearBackground(RAYWHITE);

            desenharMundoAtual(
                jogo,
                fundo,
                animacoesProta,
                largura,
                altura
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

            atualizarAnimacaoAtual(
                jogo,
                animacoesProta
            );

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

            desenharMundoAtual(
                jogo,
                fundo,
                animacoesProta,
                largura,
                altura
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

            if (IsKeyPressed(KEY_B) && podeUsarB) {
                podeUsarB = false;

                status = JOGANDO;
                continue;
            }

            if (IsKeyPressed(KEY_ESCAPE)) {
                status = PAUSADO;
                continue;
            }

            if (IsKeyPressed(KEY_Q)) {
                jogo.abaMochila = 0;
            }

            if (IsKeyPressed(KEY_E)) {
                jogo.abaMochila = 1;
            }

            if (jogo.abaMochila < 0 || jogo.abaMochila > 1) {
                jogo.abaMochila = 0;
            }

            int tamanhoMochila = (int)jogo.p.getMochila().size();

            if (tamanhoMochila <= 0) {
                jogo.itemSelecionado = 0;
            }
            else {
                if (jogo.itemSelecionado < 0) {
                    jogo.itemSelecionado = 0;
                }

                if (jogo.itemSelecionado >= tamanhoMochila) {
                    jogo.itemSelecionado = tamanhoMochila - 1;
                }

                if (IsKeyPressed(KEY_TAB)) {
                    jogo.itemSelecionado++;

                    if (jogo.itemSelecionado >= tamanhoMochila) {
                        jogo.itemSelecionado = 0;
                    }
                }
            }

            BeginDrawing();

            ClearBackground(Color{18, 18, 22, 255});

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

            atualizarAnimacaoAtual(
                jogo,
                animacoesProta
            );

            BeginDrawing();

            ClearBackground(RAYWHITE);

            desenharMundoAtual(
                jogo,
                fundo,
                animacoesProta,
                largura,
                altura
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

            desenharMundoAtual(
                jogo,
                fundo,
                animacoesProta,
                largura,
                altura
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
                podeUsarB = true;
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

                jogoAtual->animacaoProta = ANIM_PARADO;
                animacoesProta[ANIM_PARADO].reiniciar();

                podeUsarB = true;

                status = JOGANDO;
            }
            else if (acao == ACAO_VOLTAR_MENU) {
                jogoAtual.reset();
                podeUsarB = true;
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
                podeUsarB = true;
                status = MENU;
            }
            else if (acao == ACAO_SAIR) {
                break;
            }
        }
    }

    UnloadTexture(texturaProtaParado);
    UnloadTexture(texturaProtaAndando);
    UnloadTexture(texturaProtaAtaqueMelee);
    UnloadTexture(texturaProtaAtaqueRanged);
    UnloadTexture(texturaProtaDano);
    UnloadTexture(texturaProtaMorte);

    UnloadTexture(fundo);
    descarregarTexturasItens();

    CloseWindow();

    return 0;
}