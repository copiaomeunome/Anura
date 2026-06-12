#ifndef PROTAGONISTA_H
#define PROTAGONISTA_H

#include <utility>
using namespace std;

class Protagonista {
    private:
        int vida;
        pair<int,int> posicao;
        pair<int,int> tamanho;
        int move_speed;

    public:
        Protagonista(int v, pair<int,int> p, pair<int,int> t, int ms);

        int getX();
        int getY();

        void andarX(int quantidade);
        void andarY(int quantidade);
        void alteraVida(int d);

        int getTamanhoX();
        int getTamanhoY();
        int getMS();
        int getVida();
};

#endif