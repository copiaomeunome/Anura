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

    public:
        Item_Arma_Ranged(string n, string r, float dr, int d, float ms, string i);
        int getDano();
        float getMs();
};

#endif