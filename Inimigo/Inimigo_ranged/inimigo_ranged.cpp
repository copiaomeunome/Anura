#include "Inimigo_ranged.h"
#include "../../Projetil/Projetil.h"
#include <cmath>
#include <optional>

Inimigo_ranged::Inimigo_ranged(pair<int,int> p, pair<int,int> t, int ms, bool eR, int d, int r, int v)
    : Inimigo(p, t, ms, eR, d, r, v) {}

AcaoInimigo Inimigo_ranged::mover(Protagonista p){
    pair<int,int> centro = {(posicao.first+tamanho.first/2),(posicao.second+tamanho.second/2)};
    pair<int,int> centroP = {(p.getX()+p.getTamanhoX()/2),(p.getY()+p.getTamanhoY()/2)};

    float distancia_player = sqrt(pow((centro.first-centroP.first),2) + pow((centro.second-centroP.second),2));
    pair<float,float> ms = {
        (move_speed*((float)centro.first-(float)centroP.first))/distancia_player,
        (move_speed*((float)centro.second-(float)centroP.second))/distancia_player
    };

    if(distancia_player >= range){
        posicao.first-=ms.first;
        posicao.second-=ms.second;
        return {};
    }

    Projetil pr({{posicao.first+tamanho.first/2,posicao.second+tamanho.second/2},{40,40},ms,1,1,true});
    if(cd_atack<=0){
        cd_atack = 60;
        return {pr, nullopt};
    }

    cd_atack--;
    return {};
}
