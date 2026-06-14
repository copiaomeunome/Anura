#include "NPC.h"
#include <vector>
#include "../Dialogo/Dialogo.h"

NPC::NPC(pair<int,int> p, pair<int,int> t, vector<Dialogo> d) {
    posicao = p;
    tamanho = t;
    dialogos_desbloqueados = d;
}

int NPC::getX() {
    return posicao.first;
}

int NPC::getY() {
    return posicao.second;
}

int NPC::getTamanhoX() {
    return tamanho.first;
}

int NPC::getTamanhoY() {
    return tamanho.second;
}

Dialogo& NPC::getNextDialogo(){
    return dialogos_desbloqueados[0];
}