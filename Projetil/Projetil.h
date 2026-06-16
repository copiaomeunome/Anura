#ifndef PROJETIL_H
#define PROJETIL_H

#include <vector>
#include <utility>
#include "../Protagonista/Protagonista.h"
#include "../Parede/Parede.h"

using namespace std;

class Projetil {
    private:
        pair<float,float> posicao;
        pair<int,int> tamanho;
        pair<float,float> move_speed;
        int dano;
        bool ehInimigo;
    public:
        int ricochetes;
        Projetil(pair<float,float> p, pair<int,int> t, pair<float,float> ms, int d, int r, bool ei);

        int getX();
        int getY();

        int getTamanhoX();
        int getTamanhoY();
        
        pair<float,float> getMS();
        int getDano();
        void ricochetear(Parede par);
        
        void mover();
};

#endif