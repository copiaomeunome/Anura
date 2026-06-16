#include "Inimigo.h"
#include "../Protagonista/Protagonista.h"
#include "../Projetil/Projetil.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <optional>

using namespace std;

Inimigo::Inimigo(pair<int,int> p, pair<int,int> t, int ms, bool eR, int d, int r, int v) {
    posicao = p;
    tamanho = t;
    move_speed = ms;
    eRanged = eR;
    dano = d;
    range = r;
    vida = v;
    cd_atack = 0;
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
        
optional<Projetil> Inimigo::mover_ranged(Protagonista p){
    
    pair<int,int> centro = {(posicao.first+tamanho.first/2),(posicao.second+tamanho.second/2)};
    pair<int,int> centroP = {(p.getX()+p.getTamanhoX()/2),(p.getY()+p.getTamanhoY()/2)};
    
    float distancia_player = sqrt(pow((centro.first-centroP.first),2) + pow((centro.second-centroP.second),2));
    pair<float,float> ms = {(move_speed*((float)centro.first-(float)centroP.first))/distancia_player,(move_speed*((float)centro.second-(float)centroP.second))/distancia_player};
    
    if(distancia_player >= range){
        posicao.first-=ms.first;
        posicao.second-=ms.second;
        return nullopt;
    }
    Projetil pr({{posicao.first+tamanho.first/2,posicao.second+tamanho.second/2},{40,40},ms,1,1,true});
    if(cd_atack<=0){
        cd_atack = 60;
        return pr;
    }
    else{
        cd_atack--;
        return nullopt;
    }
    
    
}

void Inimigo::alteraVida(int d){
    vida += d;
}
