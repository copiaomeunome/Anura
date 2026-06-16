#include "Mapa.h"
#include <algorithm>

Mapa::Mapa(pair<int,int> t, pair<int,int> c, vector<DamageArea> e, vector<Parede> par, vector<NPC> n) {
    tamanho = t;
    camera = c;
    espinhos = e;
    paredes = par;
    NPCs = n;
}

vector<DamageArea>& Mapa::getDamageAreas() {
    return espinhos;
}

vector<Parede> Mapa::getParedes() {
    return paredes;
}

void Mapa::moverX(int ms, int largura) {
    camera.first = max(0, min(camera.first + ms, tamanho.first - largura));
}

void Mapa::moverY(int ms, int altura) {
    camera.second = max(0, min(camera.second + ms, tamanho.second - altura));
}

void Mapa::atualizarCamera(pair<int,int> centro, int largura, int altura) {
    int limiteX = max(0, tamanho.first - largura);
    int limiteY = max(0, tamanho.second - altura);

    camera.first = max(0, min(centro.first - largura / 2, limiteX));
    camera.second = max(0, min(centro.second - altura / 2, limiteY));
}

pair<int,int> Mapa::getTamanho() {
    return tamanho;
}

pair<int,int> Mapa::getCamera() {
    return camera;
}

vector<NPC>& Mapa::getNPCs() {
    return NPCs;
}

void Mapa::addDamageAreas(DamageArea da){
    espinhos.push_back(da);
}

void Mapa::removeDamageArea(int i){
    espinhos.erase(espinhos.begin() + i);
}
