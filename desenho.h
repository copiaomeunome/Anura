#ifndef DESENHO_H
#define DESENHO_H

#include <vector>
#include <memory>
#include <string>
#include <raylib.h>

#include "Mapa/Mapa.h"
#include "Protagonista/Protagonista.h"
#include "Projetil/Projetil.h"
#include "Inimigo/Inimigo.h"
#include "Item/Item_Arma_Ranged/Item_Arma_Ranged.h"
#include "Item/Item.h"

enum AcaoMenu {
    ACAO_NENHUMA,
    ACAO_JOGAR,
    ACAO_CONTINUAR,
    ACAO_VOLTAR_MENU,
    ACAO_SAIR,
    ACAO_JOGAR_NOVAMENTE
};

AcaoMenu desenharMenuPrincipal(int largura, int altura);
AcaoMenu desenharMenuPause(int largura, int altura);
AcaoMenu desenharMenuGameOver(int largura, int altura);
AcaoMenu desenharMenuVitoria(int largura, int altura);

void desenharFundo(Texture2D fundo, Mapa& mapa1, int largura, int altura);

void desenha_mapa_e_prota(
    Mapa& mapa1,
    Protagonista& p,
    int largura,
    int altura,
    Texture2D texturaProtagonista
);

void desenharInimigos(
    std::vector<std::unique_ptr<Inimigo>>& inimigos,
    Mapa& mapa1
);

void desenharProjeteis(
    std::vector<Projetil>& projeteis,
    Mapa& mapa1
);

void desenharHUD(
    Protagonista& p,
    Mapa& mapa1,
    int largura,
    int altura,
    int cooldown_dano,
    int cooldown_interacao,
    std::string s,
    std::vector<std::unique_ptr<Inimigo>>& inimigos
);

void desenharDialogo(std::string s, int largura, int altura);

void desenharMochila(Protagonista& p, int largura, int altura, int itemSelecionado, int abaMochila);
void desenharHotbar(Protagonista& p, int largura, int altura);
void descarregarTexturasItens();
void desenharItens(vector<Item>& itens, vector<pair<int,int>>& posicoes, Mapa& mapa1);
void desenharEquipamentosHUD(Protagonista& p, int largura, int altura);

bool desenharBotao(string texto, Rectangle botao);

void desenharMochila(
    Protagonista& p,
    int largura,
    int altura,
    int itemSelecionado,
    int abaMochila
);

#endif