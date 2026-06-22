#include "Protagonista.h"
#include "../DamageArea/DamageArea.h"
#include "../Item/Item_Arma_Ranged/Item_Arma_Ranged.h"
#include "../Item/Item.h"
#include "../Projetil/Projetil.h"
#include <cmath>
Protagonista::Protagonista(int v, pair<int,int> p, pair<int,int> t, int ms, int rm, Item_Arma_Ranged arma_r, int tm)
    : vida(v),
      posicao(p),
      tamanho(t),
      move_speed(ms),
      range_melee(rm),
      arma_ranged(arma_r),
      arma_melee("Sem arma melee", "Comum", 0.0f, 0, 0.0f, "", {0, 0}),
      mochila(),
      tam_mochila(tm),
      capacete("Sem capacete", "Comum", 0.0f, ""),
      armadura("Sem armadura", "Comum", 0.0f, "") {}

int Protagonista::getX() {return posicao.first;}
int Protagonista::getY() {return posicao.second;}
void Protagonista::andarX(int quantidade) {posicao.first += quantidade;}
void Protagonista::andarY(int quantidade) {posicao.second += quantidade;}
void Protagonista::alteraVida(int d) {vida += d;}
int Protagonista::getTamanhoX() {return tamanho.first;}
int Protagonista::getTamanhoY() {return tamanho.second;}
int Protagonista::getMS() {return move_speed;}
int Protagonista::getVida() {return vida;}
vector<Item> Protagonista::getMochila(){return mochila;}
int Protagonista::getTamMochila(){return tam_mochila;}
Item Protagonista::getCapacete(){return capacete;}
Item Protagonista::getArmadura(){return armadura;}
Item_Arma_Ranged Protagonista::getArmaRanged(){return arma_ranged;}
Item Protagonista::getArmaMelee(){return arma_melee;}

DamageArea Protagonista::bater_melee(int x, int y){
    float centroX = posicao.first + tamanho.first / 2.0f;
    float centroY = posicao.second + tamanho.second / 2.0f;

    float deltaX = x - centroX;
    float deltaY = y - centroY;
    float distancia = sqrt(deltaX * deltaX + deltaY * deltaY);

    if (distancia == 0.0f) {
        deltaX = 1.0f;
        deltaY = 0.0f;
        distancia = 1.0f;
    }

    float direcaoX = deltaX / distancia;
    float direcaoY = deltaY / distancia;
    float deslocamento = max(tamanho.first, tamanho.second) / 2.0f + range_melee / 2.0f;

    int posx = (int)(round(centroX + direcaoX * deslocamento - range_melee / 2.0f));
    int posy = (int)(round(centroY + direcaoY * deslocamento - range_melee / 2.0f));

    DamageArea da(1,{posx,posy},{range_melee, range_melee},true,true);
    return da;
}

optional<Projetil> Protagonista::bater_Ranged(int x, int y){
    if (!arma_ranged.consumirMunicao()) {
        return nullopt;
    }

    pair<int,int> centro = {(posicao.first+tamanho.first/2),(posicao.second+tamanho.second/2)};
    pair<int,int> centroP = {x,y};

    float distancia_player = sqrt(pow((centro.first-centroP.first),2) + pow((centro.second-centroP.second),2));
    pair<float,float> ms = {
        (arma_ranged.getMs()*((float)centro.first-(float)centroP.first))/distancia_player,
        (arma_ranged.getMs()*((float)centro.second-(float)centroP.second))/distancia_player
    };

    Projetil pr({{posicao.first+tamanho.first/2,posicao.second+tamanho.second/2},{40,40},ms,arma_ranged.getDano(),1,false});
    return pr;
}

bool Protagonista::adiciona_mochila(Item i){
    if(!(mochila.size()>=tam_mochila)){
        mochila.push_back(i);
        return true;
    }
    return false;
}
void Protagonista::troca_mochila(int t){
    tam_mochila = t;
}
void Protagonista::tira_da_mochila(int i){
    mochila.erase(mochila.begin() + i);
}
