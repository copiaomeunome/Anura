#include "Inimigo_melee.h"
#include <cmath>

Inimigo_melee::Inimigo_melee(pair<int,int> p, pair<int,int> t, int ms, bool eR, int d, int r, int v, vector<Item> dr)
    : Inimigo(p, t, ms, eR, d, r, v, dr) {}

AcaoInimigo Inimigo_melee::mover(Protagonista p){
    pair<int,int> centro = {posicao.first + tamanho.first / 2, posicao.second + tamanho.second / 2};
    pair<int,int> centroP = {p.getX() + p.getTamanhoX() / 2, p.getY() + p.getTamanhoY() / 2};

    float deltaX = static_cast<float>(centroP.first - centro.first);
    float deltaY = static_cast<float>(centroP.second - centro.second);
    float distancia_player = sqrt(deltaX * deltaX + deltaY * deltaY);

    if(distancia_player == 0.0f){
        return {};
    }

    if(distancia_player > range){
        float passoX = (deltaX / distancia_player) * move_speed;
        float passoY = (deltaY / distancia_player) * move_speed;
        posicao.first += static_cast<int>(round(passoX));
        posicao.second += static_cast<int>(round(passoY));
        return {};
    }

    if(cd_atack > 0){
        cd_atack--;
        return {};
    }

    float direcaoX = deltaX / distancia_player;
    float direcaoY = deltaY / distancia_player;
    float deslocamento = max(tamanho.first, tamanho.second) / 2.0f + range / 2.0f;

    int posx = static_cast<int>(round(centro.first + direcaoX * deslocamento - range / 2.0f));
    int posy = static_cast<int>(round(centro.second + direcaoY * deslocamento - range / 2.0f));

    cd_atack = 30;
    DamageArea golpe(dano, {posx, posy}, {range, range}, false, true);
    return {nullopt, golpe};
}
