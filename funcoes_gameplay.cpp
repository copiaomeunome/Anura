#include "funcoes_gameplay.h"

#include <algorithm>
#include <raylib.h>

using namespace std;

int limitar(int valor, int minimo, int maximo){
    return max(minimo, min(valor, maximo));
}

bool checa_colisao_parede(Protagonista& p, Mapa mapa){
    for(Parede par : mapa.getParedes())
        if (CheckCollisionRecs(
            {
                (float)p.getX(),
                (float)p.getY(),
                (float)p.getTamanhoX(),
                (float)p.getTamanhoY()
            },
            {
                (float)par.getX(),
                (float)par.getY(),
                (float)par.getTamanhoX(),
                (float)par.getTamanhoY()
            }
        )){
            return true;
        }
    return false;
}

bool checa_colisao_projetil(Protagonista& p, Mapa mapa, Projetil& bala, vector<unique_ptr<Inimigo>>& inimigos){
    bool resp = false;

    if(bala.getX()<0 || bala.getY()<0 || bala.getX()>mapa.getTamanho().first || bala.getY()>mapa.getTamanho().second) {
        return true;
    }

    for(Parede par : mapa.getParedes())
        if (CheckCollisionRecs(
            {
                (float)bala.getX(),
                (float)bala.getY(),
                (float)bala.getTamanhoX(),
                (float)bala.getTamanhoY()
            },
            {
                (float)par.getX(),
                (float)par.getY(),
                (float)par.getTamanhoX(),
                (float)par.getTamanhoY()
            }
        )){
            bala.ricochetear(par);

            if(bala.ricochetes==0){
                resp = true;
            }
        }

    if(bala.ehInimigo){
        if(CheckCollisionRecs(
            {
                (float)bala.getX(),
                (float)bala.getY(),
                (float)bala.getTamanhoX(),
                (float)bala.getTamanhoY()
            },
            {
                (float)p.getX(),
                (float)p.getY(),
                (float)p.getTamanhoX(),
                (float)p.getTamanhoY()
            }
        )){
            p.alteraVida(-bala.getDano());
            resp = true;
        }
    }
    else{
        for(unique_ptr<Inimigo>& i : inimigos){
            if(CheckCollisionRecs(
                {
                    (float)bala.getX(),
                    (float)bala.getY(),
                    (float)bala.getTamanhoX(),
                    (float)bala.getTamanhoY()
                },
                {
                    (float)i->getX(),
                    (float)i->getY(),
                    (float)i->getTamanhoX(),
                    (float)i->getTamanhoY()
                }
            )){
                i->alteraVida(-bala.getDano());
                resp = true;
            }
        }
    }

    return resp;
}

void andar(Protagonista& p, Mapa& mapa, int largura, int altura){
    int movimentoX = 0;
    int movimentoY = 0;
    int posicaoAnteriorX = p.getX();
    int posicaoAnteriorY = p.getY();

    if (IsKeyDown(KEY_D)){
        movimentoX += p.getMS();
    }
    if (IsKeyDown(KEY_A)){
        movimentoX -= p.getMS();
    }
    if (IsKeyDown(KEY_S)){
        movimentoY += p.getMS();
    }
    if (IsKeyDown(KEY_W)){
        movimentoY -= p.getMS();
    }

    if (movimentoX != 0) {
        p.andarX(movimentoX);

        int limiteX = mapa.getTamanho().first - p.getTamanhoX();
        int posicaoCorrigidaX = limitar(p.getX(), 0, limiteX);

        p.andarX(posicaoCorrigidaX - p.getX());

        if (checa_colisao_parede(p, mapa)) {
            p.andarX(posicaoAnteriorX - p.getX());
        }
    }

    if (movimentoY != 0) {
        p.andarY(movimentoY);

        int limiteY = mapa.getTamanho().second - p.getTamanhoY();
        int posicaoCorrigidaY = limitar(p.getY(), 0, limiteY);

        p.andarY(posicaoCorrigidaY - p.getY());

        if (checa_colisao_parede(p, mapa)) {
            p.andarY(posicaoAnteriorY - p.getY());
        }
    }

    mapa.atualizarCamera(
        {
            p.getX() + p.getTamanhoX() / 2,
            p.getY() + p.getTamanhoY() / 2
        },
        largura,
        altura
    );
}

vector<int> checa_dar_dano_melee(vector<unique_ptr<Inimigo>>& inimigos, DamageArea dano){
    vector<int> resp;

    for(int indice = 0; indice < inimigos.size(); indice++)
        if (CheckCollisionRecs(
            {
                (float)dano.getX(),
                (float)dano.getY(),
                (float)dano.getTamanhoX(),
                (float)dano.getTamanhoY()
            },
            {
                (float)inimigos[indice]->getX(),
                (float)inimigos[indice]->getY(),
                (float)inimigos[indice]->getTamanhoX(),
                (float)inimigos[indice]->getTamanhoY()
            }
        )){
            inimigos[indice]->alteraVida(dano.getDano()*-1);

            if(inimigos[indice]->getVida()<=0) {
                resp.push_back(indice);
            }
        }

    return resp;
}

bool checa_tomar_dano(Protagonista& p, vector<DamageArea> areas_de_dano){
    for(DamageArea e : areas_de_dano)
        if (CheckCollisionRecs(
            {
                (float)p.getX(),
                (float)p.getY(),
                (float)p.getTamanhoX(),
                (float)p.getTamanhoY()
            },
            {
                (float)e.getX(),
                (float)e.getY(),
                (float)e.getTamanhoX(),
                (float)e.getTamanhoY()
            }
        )){
            if(!e.ehAliada){
                p.alteraVida(e.getDano()*-1);
                return true;
            }
        }

    return false;
}