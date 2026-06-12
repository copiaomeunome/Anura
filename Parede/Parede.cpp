#include "Parede.h"

Parede::Parede(pair<int,int> p, pair<int,int> t) {
    posicao = p;
    tamanho = t;
}

void Parede::andarX(int quantidade) {posicao.first += quantidade;}

void Parede::andarY(int quantidade) {posicao.second += quantidade;}

int Parede::getX() {return posicao.first;}

int Parede::getY() {return posicao.second;}

int Parede::getTamanhoX() {return tamanho.first;}

int Parede::getTamanhoY() {return tamanho.second;}