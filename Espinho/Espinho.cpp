#include "Espinho.h"

Espinho::Espinho(int d, pair<int,int> p, pair<int,int> t) {
    dano = d;
    posicao = p;
    tamanho = t;
}

void Espinho::andarX(int quantidade) {
    posicao.first += quantidade;
}

void Espinho::andarY(int quantidade) {
    posicao.second += quantidade;
}

int Espinho::getX() {
    return posicao.first;
}

int Espinho::getY() {
    return posicao.second;
}

int Espinho::getTamanhoX() {
    return tamanho.first;
}

int Espinho::getTamanhoY() {
    return tamanho.second;
}

int Espinho::getDano() {
    return dano;
}