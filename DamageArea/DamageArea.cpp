#include "DamageArea.h"

DamageArea::DamageArea(int d, pair<int,int> p, pair<int,int> t, bool ea) {
    dano = d;
    posicao = p;
    tamanho = t;
    ehAliada = ea;
}


int DamageArea::getX() {
    return posicao.first;
}

int DamageArea::getY() {
    return posicao.second;
}

int DamageArea::getTamanhoX() {
    return tamanho.first;
}

int DamageArea::getTamanhoY() {
    return tamanho.second;
}

int DamageArea::getDano() {
    return dano;
}