#ifndef ANIMACAO_H
#define ANIMACAO_H

#include <raylib.h>

enum EstadoAnimacaoProta {
    ANIM_PARADO,
    ANIM_ANDANDO,
    ANIM_ATAQUE_MELEE,
    ANIM_ATAQUE_RANGED,
    ANIM_DANO,
    ANIM_MORTE,
    ANIM_TOTAL
};

class Animacao {
private:
    Texture2D textura;
    int totalFrames;
    int frameAtual;
    float tempo;
    float intervalo;
    bool loop;
    bool finalizada;

public:
    Animacao();

    Animacao(
        Texture2D tex,
        int frames,
        float intervaloFrames,
        bool repetir
    );

    void atualizar();
    void reiniciar();

    Rectangle getSource();
    Texture2D getTextura();

    bool terminou();
};

#endif