#ifndef INIMIGO_RANGED_H
#define INIMIGO_RANGED_H

#include <utility>
#include "../Inimigo.h"

using namespace std;

class Inimigo_ranged : public Inimigo {
    public:
        Inimigo_ranged(pair<int,int> p, pair<int,int> t, int ms, bool eR, int d, int r, int v, vector<Item> dr = {});
        AcaoInimigo mover(Protagonista p) override;
};

#endif
