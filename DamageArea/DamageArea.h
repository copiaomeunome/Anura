#ifndef DAMAGEAREA_H
#define DAMAGEAREA_H

#include <utility>
using namespace std;

class DamageArea {
    private:
        int dano;
        pair<int,int> posicao;
        pair<int,int> tamanho;

    public:
        bool ehAliada;
        bool temporaria;
        DamageArea(int d, pair<int,int> p, pair<int,int> t, bool ea, bool temp = false);


        int getX();
        int getY();
        int getTamanhoX();
        int getTamanhoY();
        int getDano();
};

#endif
