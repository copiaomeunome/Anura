#ifndef MAPA_H
#define MAPA_H

#include <vector>
#include <utility>
#include "../Espinho/Espinho.h"
#include "../Parede/Parede.h"

using namespace std;

class Mapa {
    private:
        vector<Parede> paredes; 
        pair<int,int> tamanho;
        vector<Espinho> espinhos;
        pair<int,int> camera;

    public:
        Mapa(pair<int,int> t, pair<int,int> pp, vector<Espinho> e, vector<Parede> par);

        vector<Espinho> getEspinhos();
        vector<Parede> getParedes();

        void moverX(int ms, int largura);
        void moverY(int ms, int altura);
        void atualizarCamera(pair<int,int> centro, int largura, int altura);

        pair<int,int> getTamanho();
        pair<int,int> getCamera();
};

#endif
