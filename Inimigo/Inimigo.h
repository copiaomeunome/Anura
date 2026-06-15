#ifndef INIMIGO_H
#define INIMIGO_H

#include <vector>
#include <utility>
#include "../Protagonista/Protagonista.h"

using namespace std;

class Inimigo {
    private:
        pair<int,int> posicao;
        pair<int,int> tamanho;
        int move_speed;
        bool eRanged;
        int dano;
        int range;

    public:
        Inimigo(pair<int,int> p, pair<int,int> t, int ms, bool eR, int d, int r);

        int getX();
        int getY();

        int getTamanhoX();
        int getTamanhoY();
        
        int getMS();
        bool getERanged();
        int getDano();
        
        void mover(Protagonista p);
        void bater();
};

#endif