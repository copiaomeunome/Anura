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
#include "Item/Item.h"

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

void desenharMochila(Protagonista& p, int largura, int altura, int itemSelecionado);
void desenharHotbar(Protagonista& p, int largura, int altura);
void descarregarTexturasItens();
void desenharItens(vector<Item>& itens, vector<pair<int,int>>& posicoes, Mapa& mapa1);

#endif