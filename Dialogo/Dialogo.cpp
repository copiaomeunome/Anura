#include "Dialogo.h"
#include <vector>
#include <string>

Dialogo::Dialogo(vector<string> f){
    falas = f;
    index = -1;
    esta_ativo=false;
}

int Dialogo::getIndex(){
    return index;
}
vector<string> Dialogo::getFalas(){
    return falas;
}