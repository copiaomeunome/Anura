#include "Item_Arma_Ranged.h"
#include "../Item.h"
#include <vector>
#include <string>

Item_Arma_Ranged::Item_Arma_Ranged(string n, string r, float dr, int d, float ms, string i, pair<int,int> m) : Item(n,r,dr, i){
    dano = d;
    move_speed = ms;
    municao = m;
}
int Item_Arma_Ranged::getDano(){
    return dano;
}
float Item_Arma_Ranged::getMs(){
    return move_speed;
}
pair<int,int> Item_Arma_Ranged::getMunicao(){
    return municao;
}
bool Item_Arma_Ranged::temMunicao(){
    return municao.first > 0;
}
bool Item_Arma_Ranged::consumirMunicao(){
    if (!temMunicao()) {
        return false;
    }

    municao.first--;
    return true;
}
