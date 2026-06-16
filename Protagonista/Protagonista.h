#ifndef PROTAGONISTA_H
#define PROTAGONISTA_H

#include <utility>
#include "../DamageArea/DamageArea.h"
using namespace std;

class Protagonista {
    private:
        int vida;
        pair<int,int> posicao;
        pair<int,int> tamanho;
        int move_speed;
        int range_melee;

    public:
        Protagonista(int v, pair<int,int> p, pair<int,int> t, int ms, int rm);

        int getX();
        int getY();

        void andarX(int quantidade);
        void andarY(int quantidade);
        void alteraVida(int d);

        int getTamanhoX();
        int getTamanhoY();
        int getMS();
        int getVida();
        DamageArea bater_melee(int x, int y);
};

#endif