#ifndef INIMIGO_MELEE_H
#define INIMIGO_MELEE_H

#include "../Inimigo.h"

class Inimigo_melee : public Inimigo {
    public:
        Inimigo_melee(pair<int,int> p, pair<int,int> t, int ms, bool eR, int d, int r, int v, vector<Item> dr = {});
        AcaoInimigo mover(Protagonista p) override;
};

#endif
