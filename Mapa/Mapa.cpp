#include "Mapa.h"

Mapa::Mapa(pair<int,int> t, pair<int,int> pp, vector<Espinho> e) {
    tamanho = t;
    posicao_protagonista = pp;
    espinhos = e;
}

vector<Espinho> Mapa::getEspinhos() {
    return espinhos;
}

void Mapa::moverX(int ms) {
    for (Espinho& e : espinhos) {
        e.andarX(-ms);
    }

    posicao_protagonista.first += ms;
}

void Mapa::moverY(int ms) {
    for (Espinho& e : espinhos) {
        e.andarY(-ms);
    }

    posicao_protagonista.second += ms;
}

pair<int,int> Mapa::getTamanho() {
    return tamanho;
}

pair<int,int> Mapa::getPosicao_protagonista() {
    return posicao_protagonista;
}

void Mapa::movePPX(int ms) {
    posicao_protagonista.first += ms;
}

void Mapa::movePPY(int ms) {
    posicao_protagonista.second += ms;
}