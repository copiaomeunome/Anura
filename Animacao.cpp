#include "Animacao.h"

Animacao::Animacao() {
    textura = Texture2D{};
    totalFrames = 1;
    frameAtual = 0;
    tempo = 0.0f;
    intervalo = 0.15f;
    loop = true;
    finalizada = false;
}

Animacao::Animacao(
    Texture2D tex,
    int frames,
    float intervaloFrames,
    bool repetir
) {
    textura = tex;
    totalFrames = frames;

    if (totalFrames <= 0) {
        totalFrames = 1;
    }

    frameAtual = 0;
    tempo = 0.0f;
    intervalo = intervaloFrames;
    loop = repetir;
    finalizada = false;
}

void Animacao::atualizar() {
    if (finalizada) {
        return;
    }

    tempo += GetFrameTime();

    if (tempo >= intervalo) {
        tempo = 0.0f;
        frameAtual++;

        if (frameAtual >= totalFrames) {
            if (loop) {
                frameAtual = 0;
            } else {
                frameAtual = totalFrames - 1;
                finalizada = true;
            }
        }
    }
}

void Animacao::reiniciar() {
    frameAtual = 0;
    tempo = 0.0f;
    finalizada = false;
}

Rectangle Animacao::getSource() {
    if (textura.id == 0 || textura.width <= 0 || textura.height <= 0) {
        return {0, 0, 0, 0};
    }

    float larguraFrame = (float)textura.width / totalFrames;

    return {
        frameAtual * larguraFrame,
        0,
        larguraFrame,
        (float)textura.height
    };
}

Texture2D Animacao::getTextura() {
    return textura;
}

bool Animacao::terminou() {
    return finalizada;
}