#ifndef MAIN_H
#define MAIN_H

#include <vector>
#include "Protagonista/Protagonista.h"
#include "Mapa/Mapa.h"
#include "Espinho/Espinho.h"

using namespace std;

void andar(Protagonista& p, Mapa& mapa, int largura, int altura);

bool checa_tomar_dano(Protagonista& p, vector<Espinho> espinhos);

#endif