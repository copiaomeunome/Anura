#include "Inimigo.h"
#include "../Protagonista/Protagonista.h"
#include <vector>


Inimigo::Inimigo(pair<int,int> p, pair<int,int> t, int ms, bool eR, int d, int r) {
    posicao = p;
    tamanho = t;
    move_speed = ms;
    eRanged = eR;
    dano = d;
    range = r;
}

int Inimigo::getX() {
    return posicao.first;
}

int Inimigo::getY() {
    return posicao.second;
}

int Inimigo::getTamanhoX() {
    return tamanho.first;
}

int Inimigo::getTamanhoY() {
    return tamanho.second;
}
        
int Inimigo::getMS(){
    return move_speed;
}

bool Inimigo::getERanged(){
    return eRanged;
}

int Inimigo::getDano(){
    return dano;
}
        
void Inimigo::mover(Protagonista p){
    if(eRanged){
        pair<int,int> centro = {(posicao.first+tamanho.first)/2,(posicao.second+tamanho.second)/2};
        pair<int,int> centroP = {(p.getX()+p.getTamanhoX())/2,(p.getY()+p.getTamanhoY())/2};
        int distancia_player = pow((centro.first-centroP.first),2) + pow((centro.second-centroP.second),1/2);
        if(distancia_player >= range){
            posicao.first+=move_speed*((centro.first-centroP.first)/(distancia_player));
            posicao.second+=move_speed*((centro.second-centroP.second)/(distancia_player));
        }
    }
    else{

    }
}
void Inimigo::bater(){

}