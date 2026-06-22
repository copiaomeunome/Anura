#ifndef DADOS_JOGO_H
#define DADOS_JOGO_H

#include <vector>
#include <memory>
#include <string>
#include <utility>

#include "DamageArea/DamageArea.h"
#include "Projetil/Projetil.h"
#include "Mapa/Mapa.h"
#include "Protagonista/Protagonista.h"
#include "Inimigo/Inimigo.h"
#include "Inimigo/Inimigo_melee/Inimigo_melee.h"
#include "Inimigo/Inimigo_ranged/Inimigo_ranged.h"
#include "Item/Item.h"
#include "Item/Item_Arma_Ranged/Item_Arma_Ranged.h"
#include "Dialogo/Dialogo.h"
#include "NPC/NPC.h"
#include "Animacao.h"

using namespace std;

struct DadosJogo {
    Protagonista p;
    Mapa mapa1;

    vector<Projetil> projeteis;
    vector<Item> itens;
    vector<pair<int, int>> posicoesItens;
    vector<unique_ptr<Inimigo>> inimigos;

    int cooldown_dano;
    int cooldown_interacao;
    int itemSelecionado;
    int abaMochila;
    int current_map;
    EstadoAnimacaoProta animacaoProta;

    string falaAtual;

    DadosJogo(
        Protagonista protagonista,
        Mapa mapa,
        vector<unique_ptr<Inimigo>> inimigosIniciais
    );
};

DadosJogo criarNovoJogo(int largura, int altura);

#endif