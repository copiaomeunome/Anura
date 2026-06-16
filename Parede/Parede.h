#ifndef PAREDE_H
#define PAREDE_H

#include <vector>
#include <utility>
//#include "../Espinho/Espinho.h"

using namespace std;

class Parede {
    private:
        pair<int,int> tamanho;
        pair<int,int> posicao;

    public:
        Parede(pair<int,int> p, pair<int,int> t);

        void moverX(int ms);
        void moverY(int ms);

        int getTamanhoX();
        int getTamanhoY();

        int getX();
        int getY();
};

#endif