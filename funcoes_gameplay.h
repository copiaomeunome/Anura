#ifndef FUNCOES_GAMEPLAY_H
#define FUNCOES_GAMEPLAY_H

#include <vector>
#include <memory>

#include "DamageArea/DamageArea.h"
#include "Projetil/Projetil.h"
#include "Mapa/Mapa.h"
#include "Protagonista/Protagonista.h"
#include "Inimigo/Inimigo.h"
#include "Item/Item.h"

int limitar(int valor, int minimo, int maximo);

bool checa_colisao_parede(Protagonista& p, Mapa mapa);

bool checa_colisao_projetil(
    Protagonista& p,
    Mapa mapa,
    Projetil& bala,
    std::vector<std::unique_ptr<Inimigo>>& inimigos
);

void andar(Protagonista& p, Mapa& mapa, int largura, int altura);

std::vector<int> checa_dar_dano_melee(
    std::vector<std::unique_ptr<Inimigo>>& inimigos,
    DamageArea dano
);

bool checa_tomar_dano(
    Protagonista& p,
    std::vector<DamageArea> areas_de_dano
);

int checa_pegar_itens(
    Protagonista& p,
    vector<Item>& itens,
    vector<pair<int,int>>& posicoesItens
);

#endif