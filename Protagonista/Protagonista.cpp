#include "Protagonista.h"
#include "../DamageArea/DamageArea.h"
Protagonista::Protagonista(int v, pair<int,int> p, pair<int,int> t, int ms, int rm) {
    posicao = p;
    vida = v;
    tamanho = t;
    move_speed = ms;
    range_melee = rm;
}

int Protagonista::getX() {
    return posicao.first;
}

int Protagonista::getY() {
    return posicao.second;
}

void Protagonista::andarX(int quantidade) {
    posicao.first += quantidade;
}

void Protagonista::andarY(int quantidade) {
    posicao.second += quantidade;
}

void Protagonista::alteraVida(int d) {
    vida += d;
}

int Protagonista::getTamanhoX() {
    return tamanho.first;
}

int Protagonista::getTamanhoY() {
    return tamanho.second;
}

int Protagonista::getMS() {
    return move_speed;
}

int Protagonista::getVida() {
    return vida;
}

DamageArea Protagonista::bater_melee(){
    DamageArea da(1,posicao,tamanho,true);
    return da;
}