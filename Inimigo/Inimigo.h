#ifndef INIMIGO_H
#define INIMIGO_H

#include <vector>
#include <utility>
#include <optional>
#include "../Protagonista/Protagonista.h"
#include "../projetil/Projetil.h"

using namespace std;

class Inimigo {
    private:
        pair<int,int> posicao;
        pair<int,int> tamanho;
        int move_speed;
        bool eRanged;
        int dano;
        int range;
        int vida;
        int cd_atack;

    public:
        Inimigo(pair<int,int> p, pair<int,int> t, int ms, bool eR, int d, int r, int v);

        int getX();
        int getY();

        int getTamanhoX();
        int getTamanhoY();
        
        int getMS();
        bool getERanged();
        int getDano();
        int getVida();
        
        optional<Projetil> mover_ranged(Protagonista p);
        void alteraVida(int d);
};

#endif
