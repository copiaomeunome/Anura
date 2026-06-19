#include "Item.h"
#include <vector>
#include <string>

Item::Item(string n, string r, float dr, string i){
    nome = n;
    raridade = r;
    dropRate = dr;
    imagem = i;
}
float Item::getDropRate(){return dropRate;}
string Item::getNome(){return nome;}
string Item::getRaridade(){return raridade;}
string Item::getImagem(){return imagem;}