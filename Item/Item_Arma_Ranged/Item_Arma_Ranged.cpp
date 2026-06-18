#include "Item_Arma_Ranged.h"
#include "../Item.h"
#include <vector>
#include <string>

Item_Arma_Ranged::Item_Arma_Ranged(string n, string r, float dr, int d, float ms) : Item(n,r,dr){
    dano = d;
    move_speed = ms;
}
int Item_Arma_Ranged::getDano(){
    return dano;
}
float Item_Arma_Ranged::getMs(){
    return move_speed;
}