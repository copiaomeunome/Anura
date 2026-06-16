#ifndef INIMIGO_H
#define INIMIGO_H

#include <vector>
#include <utility>
#include <optional>
#include <memory>
#include "../Protagonista/Protagonista.h"
#include "../projetil/Projetil.h"
#include "../DamageArea/DamageArea.h"

using namespace std;

struct AcaoInimigo {
    optional<Projetil> projetil;
    optional<DamageArea> areaDano;
};

class Inimigo {
    protected:
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
        virtual ~Inimigo() = default;

        int getX();
        int getY();

        int getTamanhoX();
        int getTamanhoY();
        
        int getMS();
        bool getERanged();
        int getDano();
        int getVida();
        
        virtual AcaoInimigo mover(Protagonista p) = 0;
        void alteraVida(int d);
};

#endif
