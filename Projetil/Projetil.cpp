#include "Projetil.h"
#include "../Protagonista/Protagonista.h"
#include "../Parede/Parede.h"
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

Projetil::Projetil(pair<float,float> p, pair<int,int> t, pair<float,float> ms, int d, int r, bool ei) {
    posicao = p;
    tamanho = t;
    move_speed = ms;
    dano = d;
    ricochetes = r;
    ehInimigo = ei;
}

int Projetil::getX() {
    return posicao.first;
}

int Projetil::getY() {
    return posicao.second;
}

int Projetil::getTamanhoX() {
    return tamanho.first;
}

int Projetil::getTamanhoY() {
    return tamanho.second;
}
        
pair<float,float> Projetil::getMS(){
    return move_speed;
}

int Projetil::getDano(){
    return dano;
}

void Projetil::ricochetear(Parede par) {
    int prevX = posicao.first + move_speed.first;
    int prevY = posicao.second + move_speed.second;

    int projLeftPrev = prevX;
    int projRightPrev = prevX + tamanho.first;
    int projTopPrev = prevY;
    int projBottomPrev = prevY + tamanho.second;

    int wallLeft = par.getX();
    int wallRight = par.getX() + par.getTamanhoX();
    int wallTop = par.getY();
    int wallBottom = par.getY() + par.getTamanhoY();

    // Bateu vindo da esquerda ou da direita da parede
    if (projRightPrev <= wallLeft || projLeftPrev >= wallRight) {
        move_speed.first *= -1;
    }

    // Bateu vindo de cima ou de baixo da parede
    if (projBottomPrev <= wallTop || projTopPrev >= wallBottom) {
        move_speed.second *= -1;
    }
    ricochetes--;
}
        
void Projetil::mover(){
    posicao.first-=move_speed.first;
    posicao.second-=move_speed.second;
}
