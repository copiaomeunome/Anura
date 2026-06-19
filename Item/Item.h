#ifndef ITEM_H
#define ITEM_H

#include <vector>
#include <utility>
#include <string>
using namespace std;

class Item {
    private:
        string nome;
        string raridade;
        float dropRate;
        string imagem;

    public:
        Item(string n, string r, float dr, string i);
        float getDropRate();
        string getNome();
        string getRaridade();
        string getImagem();
};

#endif