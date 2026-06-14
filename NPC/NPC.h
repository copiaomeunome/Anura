#ifndef NPC_H
#define NPC_H

#include <vector>
#include <utility>
#include "../Dialogo/Dialogo.h"

using namespace std;

class NPC {
    private:
        pair<int,int> posicao;
        pair<int,int> tamanho;

    public:
        vector<Dialogo> dialogos_desbloqueados;
        NPC(pair<int,int> p, pair<int,int> t, vector<Dialogo> d);

        int getX();
        int getY();

        int getTamanhoX();
        int getTamanhoY();
        Dialogo& getNextDialogo();
};

#endif