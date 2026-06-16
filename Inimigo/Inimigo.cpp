#include "Inimigo.h"
#include "../Protagonista/Protagonista.h"
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

Inimigo::Inimigo(pair<int,int> p, pair<int,int> t, int ms, bool eR, int d, int r, int v) {
    posicao = p;
    tamanho = t;
    move_speed = ms;
    eRanged = eR;
    dano = d;
    range = r;
    vida = v;
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

int Inimigo::getVida(){
    return vida;
}
        
void Inimigo::mover(Protagonista p){
    pair<int,int> centro = {(posicao.first+tamanho.first)/2,(posicao.second+tamanho.second)/2};
    pair<int,int> centroP = {(p.getX()+p.getTamanhoX())/2,(p.getY()+p.getTamanhoY())/2};
    int distancia_player = sqrt(pow((centro.first-centroP.first),2) + pow((centro.second-centroP.second),2));

    if(distancia_player >= range){
        posicao.first-=(move_speed*(centro.first-centroP.first))/distancia_player;
        posicao.second-=(move_speed*(centro.second-centroP.second))/distancia_player;
    }
}

void Inimigo::alteraVida(int d){
    vida += d;
}

void Inimigo::bater(){

}