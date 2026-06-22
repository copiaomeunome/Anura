#include "DadosJogo.h"

DadosJogo::DadosJogo(
    Protagonista protagonista,
    Mapa mapa,
    vector<unique_ptr<Inimigo>> inimigosIniciais
)
    : p(protagonista),
      mapa1(mapa),
      inimigos(move(inimigosIniciais))
{
    cooldown_dano = 0;
    cooldown_interacao = 0;
    itemSelecionado = 0;
    abaMochila = 0;
    current_map = 0;
    falaAtual = "nada";

    animacaoProta = ANIM_PARADO;
}

DadosJogo criarNovoJogo(int largura, int altura) {
    Item_Arma_Ranged arma_prota_ranged(
        "arma default",
        "Comum",
        0.0,
        1,
        2,
        "arco.png",
        {10, 10}
    );

    Item_Arma_Ranged arma_drop(
        "arma dropada",
        "Comum",
        0.6,
        1,
        2,
        "arco.png",
        {10, 10}
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

    return DadosJogo(
        p,
        mapa1,
        move(inimigos)
    );
}
