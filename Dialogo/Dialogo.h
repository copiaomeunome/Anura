#ifndef DIALOGO_H
#define DIALOGO_H

#include <vector>
#include <utility>
#include <string>
using namespace std;

class Dialogo {
    private:
        vector<string> falas;
        

    public:
        int index;
        bool esta_ativo;
        Dialogo(vector<string> f);

        int getIndex();
        vector<string> getFalas();
};

#endif