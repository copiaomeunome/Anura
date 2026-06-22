#ifndef PROTAGONISTA_H
#define PROTAGONISTA_H

#include <utility>
#include <optional>
#include "../DamageArea/DamageArea.h"
#include "../Item/Item_Arma_Ranged/Item_Arma_Ranged.h"
#include "../Item/Item.h"
#include "../Projetil/Projetil.h"
using namespace std;

class Protagonista {
    private:
        int vida;
        pair<int,int> posicao;
        pair<int,int> tamanho;
        int move_speed;
        int range_melee;
        Item_Arma_Ranged arma_ranged;
        Item_Arma_Ranged arma_melee;
        vector<Item> mochila;
        int tam_mochila;
        Item capacete;
        Item armadura;

    public:
        Protagonista(int v, pair<int,int> p, pair<int,int> t, int ms, int rm, Item_Arma_Ranged arma_r, int tm);

        int getX();
        int getY();

        void andarX(int quantidade);
        void andarY(int quantidade);
        void alteraVida(int d);

        int getTamanhoX();
        int getTamanhoY();
        int getMS();
        int getVida();
        DamageArea bater_melee(int x, int y);
        optional<Projetil> bater_Ranged(int x, int y);
        bool adiciona_mochila(Item i);
        void troca_mochila(int tam);
        void tira_da_mochila(int i);
        vector<Item> getMochila();
        int getTamMochila();
        Item getCapacete();
        Item getArmadura();
        Item_Arma_Ranged getArmaRanged();
        Item getArmaMelee();
};

#endif
