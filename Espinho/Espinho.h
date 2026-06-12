#ifndef ESPINHO_H
#define ESPINHO_H

#include <utility>
using namespace std;

class Espinho {
    private:
        int dano;
        pair<int,int> posicao;
        pair<int,int> tamanho;

    public:
        Espinho(int d, pair<int,int> p, pair<int,int> t);


        int getX();
        int getY();
        int getTamanhoX();
        int getTamanhoY();
        int getDano();
};

#endif