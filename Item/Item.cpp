#include "Item.h"
#include <vector>
#include <string>

Item::Item(string n, string r, float dr){
    nome = n;
    raridade = r;
    dropRate = dr;
}
float Item::getDropRate(){return dropRate;}
string Item::getNome(){return nome;}
string Item::getRaridade(){return raridade;}