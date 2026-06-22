#ifndef ITEM_ARMA_RANGED_H
#define ITEM_ARMA_RANGED_H

#include "../Item.h"
#include <vector>
#include <utility>
#include <string>
using namespace std;

class Item_Arma_Ranged : public Item {
    private:
        int dano;
        float move_speed;
        pair<int,int> municao;

    public:
        Item_Arma_Ranged(string n, string r, float dr, int d, float ms, string i, pair<int,int> m);
        int getDano();
        float getMs();
        pair<int,int> getMunicao();
        bool temMunicao();
        bool consumirMunicao();
};

#endif
