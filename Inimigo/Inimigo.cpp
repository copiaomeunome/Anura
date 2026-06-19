#include "Inimigo.h"
#include "../Protagonista/Protagonista.h"
#include "../Projetil/Projetil.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <optional>
#include <cstdlib>
#include <ctime>

using namespace std;

Inimigo::Inimigo(pair<int,int> p, pair<int,int> t, int ms, bool eR, int d, int r, int v, vector<Item> drs) {
    posicao = p;
    tamanho = t;
    move_speed = ms;
    eRanged = eR;
    dano = d;
    range = r;
    vida = v;
    cd_atack = 0;
    drop = drs;
}

int Inimigo::getX() {return posicao.first;}

int Inimigo::getY() {return posicao.second;}

int Inimigo::getTamanhoX() {return tamanho.first;}

int Inimigo::getTamanhoY() {return tamanho.second;}
        
int Inimigo::getMS(){return move_speed;}

bool Inimigo::getERanged(){return eRanged;}

int Inimigo::getDano(){return dano;}

int Inimigo::getVida(){return vida;}
        
void Inimigo::alteraVida(int d){vida += d;}

vector<Item> Inimigo::dropar(){
    vector<Item> itens;
    for(Item i : drop){
        float sorteio = (float)rand() / RAND_MAX; // número aleatório entre 0 e 1
        if(i.getDropRate()>sorteio)
            itens.push_back(i);
    }
    return itens;
}
