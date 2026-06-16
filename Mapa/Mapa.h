#ifndef MAPA_H
#define MAPA_H

#include <vector>
#include <utility>
#include "../DamageArea/DamageArea.h"
#include "../Parede/Parede.h"
#include "../NPC/NPC.h"

using namespace std;

class Mapa {
    private:
        vector<Parede> paredes; 
        pair<int,int> tamanho;
        vector<DamageArea> espinhos;
        pair<int,int> camera;
        vector<NPC> NPCs;

    public:
        Mapa(pair<int,int> t, pair<int,int> pp, vector<DamageArea> e, vector<Parede> par, vector<NPC> n);

        vector<DamageArea>& getDamageAreas();
        vector<Parede> getParedes();

        void moverX(int ms, int largura);
        void moverY(int ms, int altura);
        void atualizarCamera(pair<int,int> centro, int largura, int altura);

        pair<int,int> getTamanho();
        pair<int,int> getCamera();
        vector<NPC>& getNPCs();
        void addDamageAreas(DamageArea da);
        void removeDamageArea(int i);
};

#endif
