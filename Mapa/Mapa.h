#ifndef MAPA_H
#define MAPA_H

#include <vector>
#include <utility>
#include "../Espinho/Espinho.h"

using namespace std;

class Mapa {
private:
    pair<int,int> tamanho;
    pair<int,int> posicao_protagonista;
    vector<Espinho> espinhos;

public:
    Mapa(pair<int,int> t, pair<int,int> pp, vector<Espinho> e);

    vector<Espinho> getEspinhos();

    void moverX(int ms);
    void moverY(int ms);

    pair<int,int> getTamanho();
    pair<int,int> getPosicao_protagonista();

    void movePPX(int ms);
    void movePPY(int ms);
};

#endif