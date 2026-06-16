#ifndef PROJETIL_H
#define PROJETIL_H

#include <vector>
#include <utility>
#include "../Protagonista/Protagonista.h"

using namespace std;

class Projetil {
    private:
        pair<int,int> posicao;
        pair<int,int> tamanho;
        pair<int,int> move_speed;
        int dano;
        bool ehInimigo;
    public:
        int ricochetes;
        Projetil(pair<int,int> p, pair<int,int> t, pair<int,int> ms, int d, int r, bool ei);

        int getX();
        int getY();

        int getTamanhoX();
        int getTamanhoY();
        
        pair<int,int> getMS();
        int getDano();
        void ricochetear(Parede par);
        
        void mover();
};

#endif