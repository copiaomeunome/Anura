#include "desenho.h"

#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

using namespace std;

static unordered_map<string, Texture2D> cacheTexturasItens;

Texture2D pegarTexturaItem(string caminho) {
    if (caminho.empty()) {
        return Texture2D{};
    }

    if (cacheTexturasItens.find(caminho) == cacheTexturasItens.end()) {
        cacheTexturasItens[caminho] = LoadTexture(caminho.c_str());
    }

    return cacheTexturasItens[caminho];
}

void descarregarTexturasItens() {
    for (auto& par : cacheTexturasItens) {
        UnloadTexture(par.second);
    }

    cacheTexturasItens.clear();
}

void desenharTexturaNoRetangulo(Texture2D textura, Rectangle destino) {
    if (textura.id == 0) {
        DrawRectangleRounded(
            destino,
            0.20f,
            8,
            Color{150, 105, 45, 255}
        );

        DrawRectangleLinesEx(
            destino,
            2,
            Color{240, 210, 120, 255}
        );

        return;
    }

    DrawTexturePro(
        textura,
        {
            0,
            0,
            (float)textura.width,
            (float)textura.height
        },
        destino,
        {0, 0},
        0.0f,
        WHITE
    );
}

void desenharFundo(Texture2D fundo, Mapa& mapa1, int largura, int altura) {
    Rectangle sourceFundo = {
        0,
        0,
        (float)fundo.width,
        (float)fundo.height
    };

    Rectangle destFundo = {
        (float)-mapa1.getCamera().first,
        (float)-mapa1.getCamera().second,
        (float)mapa1.getTamanho().first,
        (float)mapa1.getTamanho().second
    };

    DrawTexturePro(
        fundo,
        sourceFundo,
        destFundo,
        {0, 0},
        0.0f,
        WHITE
    );
}

void desenha_mapa_e_prota(
    Mapa& mapa1,
    Protagonista& p,
    int largura,
    int altura,
    Texture2D texturaProtagonista
) {
    Rectangle sourceProtagonista = {
        0,
        0,
        (float)texturaProtagonista.width,
        (float)texturaProtagonista.height
    };

    Rectangle destProtagonista = {
        (float)(p.getX() - mapa1.getCamera().first),
        (float)(p.getY() - mapa1.getCamera().second),
        (float)p.getTamanhoX(),
        (float)p.getTamanhoY()
    };

    DrawTexturePro(
        texturaProtagonista,
        sourceProtagonista,
        destProtagonista,
        {0, 0},
        0.0f,
        WHITE
    );

    for (DamageArea e : mapa1.getDamageAreas()) {
        if (CheckCollisionRecs(
            {
                (float)mapa1.getCamera().first,
                (float)mapa1.getCamera().second,
                (float)largura,
                (float)altura
            },
            {
                (float)e.getX(),
                (float)e.getY(),
                (float)e.getTamanhoX(),
                (float)e.getTamanhoY()
            }
        )) {
            DrawRectangle(
                e.getX() - mapa1.getCamera().first,
                e.getY() - mapa1.getCamera().second,
                e.getTamanhoX(),
                e.getTamanhoY(),
                RED
            );
        }
    }

    for (Parede e : mapa1.getParedes()) {
        if (CheckCollisionRecs(
            {
                (float)mapa1.getCamera().first,
                (float)mapa1.getCamera().second,
                (float)largura,
                (float)altura
            },
            {
                (float)e.getX(),
                (float)e.getY(),
                (float)e.getTamanhoX(),
                (float)e.getTamanhoY()
            }
        )) {
            DrawRectangle(
                e.getX() - mapa1.getCamera().first,
                e.getY() - mapa1.getCamera().second,
                e.getTamanhoX(),
                e.getTamanhoY(),
                GREEN
            );
        }
    }

    for (NPC n : mapa1.getNPCs()) {
        if (CheckCollisionRecs(
            {
                (float)mapa1.getCamera().first,
                (float)mapa1.getCamera().second,
                (float)largura,
                (float)altura
            },
            {
                (float)n.getX(),
                (float)n.getY(),
                (float)n.getTamanhoX(),
                (float)n.getTamanhoY()
            }
        )) {
            DrawRectangle(
                n.getX() - mapa1.getCamera().first,
                n.getY() - mapa1.getCamera().second,
                n.getTamanhoX(),
                n.getTamanhoY(),
                BLACK
            );
        }
    }
}

void desenharInimigos(vector<unique_ptr<Inimigo>>& inimigos, Mapa& mapa1) {
    for (const unique_ptr<Inimigo>& i : inimigos) {
        DrawRectangle(
            i->getX() - mapa1.getCamera().first,
            i->getY() - mapa1.getCamera().second,
            i->getTamanhoX(),
            i->getTamanhoY(),
            BLACK
        );
    }
}

void desenharProjeteis(vector<Projetil>& projeteis, Mapa& mapa1) {
    for (Projetil pr : projeteis) {
        DrawRectangle(
            pr.getX() - mapa1.getCamera().first,
            pr.getY() - mapa1.getCamera().second,
            pr.getTamanhoX(),
            pr.getTamanhoY(),
            BLACK
        );
    }
}

void desenharItens(vector<Item>& itens, vector<pair<int,int>>& posicoes, Mapa& mapa1) {
    for (int i = 0; i < itens.size(); i++) {
        DrawRectangle(
            posicoes[i].first - mapa1.getCamera().first,
            posicoes[i].second - mapa1.getCamera().second,
            50,
            50,
            BLACK
        );
    }
}

void desenharHUD(
    Protagonista& p,
    Mapa& mapa1,
    int largura,
    int altura,
    int cooldown_dano,
    int cooldown_interacao,
    string s,
    vector<unique_ptr<Inimigo>>& inimigos
) {
    DrawText("Use WASD para mover", 20, 20, 20, DARKGRAY);

    DrawText(
        TextFormat(
            "Vida: %d | cooldown_dano: %d | cooldown_interacao: %d",
            p.getVida(),
            cooldown_dano,
            cooldown_interacao
        ),
        20,
        45,
        20,
        DARKGRAY
    );

    DrawText(
        TextFormat(
            "Jogador: x=%d y=%d | Camera: x=%d y=%d | Tamanho tela: largura=%d altura=%d",
            p.getX(),
            p.getY(),
            mapa1.getCamera().first,
            mapa1.getCamera().second,
            largura,
            altura
        ),
        20,
        70,
        20,
        DARKGRAY
    );

    DrawText(
        s.c_str(),
        20,
        95,
        20,
        DARKGRAY
    );

    DrawText(
        TextFormat(
            "Vida Inimigo: %d",
            inimigos.size() == 0 ? 0 : inimigos[0]->getVida()
        ),
        20,
        120,
        20,
        DARKGRAY
    );
}

void desenharTextoQuebrado(
    string texto,
    int x,
    int y,
    int larguraMaxima,
    int tamanhoFonte,
    Color cor
) {
    stringstream ss(texto);
    string palavra;
    string linha = "";

    int linhaAtual = 0;
    int espacamentoLinha = tamanhoFonte + 8;

    while (ss >> palavra) {
        string tentativa;

        if (linha == "") {
            tentativa = palavra;
        } else {
            tentativa = linha + " " + palavra;
        }

        if (MeasureText(tentativa.c_str(), tamanhoFonte) > larguraMaxima) {
            DrawText(
                linha.c_str(),
                x,
                y + linhaAtual * espacamentoLinha,
                tamanhoFonte,
                cor
            );

            linha = palavra;
            linhaAtual++;
        } else {
            linha = tentativa;
        }
    }

    if (linha != "") {
        DrawText(
            linha.c_str(),
            x,
            y + linhaAtual * espacamentoLinha,
            tamanhoFonte,
            cor
        );
    }
}

void desenharDialogo(string s, int largura, int altura) {
    float caixaW = largura * 0.72f;
    float caixaH = altura * 0.22f;

    float caixaX = (largura - caixaW) / 2.0f;
    float caixaY = altura - caixaH - 55.0f;

    Rectangle sombra = {
        caixaX + 8,
        caixaY + 8,
        caixaW,
        caixaH
    };

    Rectangle caixa = {
        caixaX,
        caixaY,
        caixaW,
        caixaH
    };

    DrawRectangleRounded(
        sombra,
        0.08f,
        16,
        Fade(BLACK, 0.45f)
    );

    DrawRectangleRounded(
        caixa,
        0.08f,
        16,
        Color{22, 24, 34, 245}
    );

    DrawRectangleLinesEx(
        caixa,
        3,
        Color{220, 190, 120, 255}
    );

    Rectangle faixaTitulo = {
        caixaX + 25,
        caixaY - 22,
        210,
        44
    };

    DrawRectangleRounded(
        faixaTitulo,
        0.25f,
        12,
        Color{55, 45, 35, 255}
    );

    DrawRectangleLinesEx(
        faixaTitulo,
        2,
        Color{220, 190, 120, 255}
    );

    DrawText(
        "Dialogo",
        (int)(faixaTitulo.x + 22),
        (int)(faixaTitulo.y + 10),
        22,
        RAYWHITE
    );

    int textoX = (int)(caixaX + 35);
    int textoY = (int)(caixaY + 45);
    int textoW = (int)(caixaW - 70);

    desenharTextoQuebrado(
        s,
        textoX,
        textoY,
        textoW,
        24,
        RAYWHITE
    );

    DrawText(
        "E: continuar",
        (int)(caixaX + caixaW - 170),
        (int)(caixaY + caixaH - 38),
        18,
        Color{190, 190, 190, 255}
    );
}

void desenharHotbar(Protagonista& p, int largura, int altura) {
    float barraW = largura * 0.42f;
    float barraH = altura * 0.10f;
    float barraX = (largura - barraW) / 2.0f;
    float barraY = altura - barraH - 25.0f;

    Rectangle barra = {
        barraX,
        barraY,
        barraW,
        barraH
    };

    DrawRectangleRounded(
        barra,
        0.18f,
        12,
        Color{25, 22, 18, 210}
    );

    DrawRectangleLinesEx(
        barra,
        2,
        Color{220, 190, 120, 255}
    );

    vector<Item> mochila = p.getMochila();

    int totalSlots = 5;
    float espaco = 10.0f;
    float slotSize = barraH * 0.70f;

    float inicioX = barraX + 20.0f;
    float slotY = barraY + (barraH - slotSize) / 2.0f;

    for (int i = 0; i < totalSlots; i++) {
        float slotX = inicioX + i * (slotSize + espaco);

        Rectangle slot = {
            slotX,
            slotY,
            slotSize,
            slotSize
        };

        bool ocupado = i < (int)mochila.size();

        DrawRectangleRounded(
            slot,
            0.15f,
            8,
            ocupado ? Color{65, 55, 40, 255} : Color{35, 35, 40, 255}
        );

        DrawRectangleLinesEx(
            slot,
            2,
            ocupado ? Color{230, 200, 110, 255} : Color{90, 90, 100, 255}
        );

        DrawText(
            TextFormat("%d", i + 1),
            (int)(slotX + 6),
            (int)(slotY + 4),
            16,
            Color{200, 200, 200, 255}
        );

        if (ocupado) {
            Rectangle icone = {
                slotX + slotSize * 0.20f,
                slotY + slotSize * 0.20f,
                slotSize * 0.60f,
                slotSize * 0.60f
            };

            Texture2D texturaItem = pegarTexturaItem(mochila[i].getImagem());

            desenharTexturaNoRetangulo(
                texturaItem,
                icone
            );
        }
    }

    DrawText(
        "B: mochila",
        (int)(barraX + barraW - 120),
        (int)(barraY + barraH / 2 - 10),
        18,
        RAYWHITE
    );
}

void desenharMochila(Protagonista& p, int largura, int altura, int itemSelecionado) {
    DrawRectangle(
        0,
        0,
        largura,
        altura,
        Fade(BLACK, 0.55f)
    );

    float painelW = largura * 0.78f;
    float painelH = altura * 0.72f;
    float painelX = (largura - painelW) / 2.0f;
    float painelY = (altura - painelH) / 2.0f;

    Rectangle painel = {
        painelX,
        painelY,
        painelW,
        painelH
    };

    DrawRectangleRounded(
        painel,
        0.04f,
        16,
        Color{25, 22, 18, 245}
    );

    DrawRectangleLinesEx(
        painel,
        3,
        Color{220, 190, 120, 255}
    );

    DrawText(
        "MOCHILA",
        (int)(painelX + 35),
        (int)(painelY + 25),
        36,
        RAYWHITE
    );

    DrawText(
        "Itens carregados",
        (int)(painelX + 38),
        (int)(painelY + 70),
        20,
        Color{190, 190, 190, 255}
    );

    int colunas = 5;
    int linhas = 3;
    int totalSlots = p.getTamMochila();

    float espaco = 14.0f;
    float areaGradeW = painelW * 0.60f;
    float areaGradeH = painelH - 140.0f;

    float slotW = (areaGradeW - espaco * (colunas - 1)) / colunas;
    float slotH = (areaGradeH - espaco * (linhas - 1)) / linhas;

    float tamanhoSlot = min(slotW, slotH);

    float gridX = painelX + 35.0f;
    float gridY = painelY + 115.0f;

    vector<Item> mochila = p.getMochila();

    for (int i = 0; i < totalSlots; i++) {
        int coluna = i % colunas;
        int linha = i / colunas;

        float x = gridX + coluna * (tamanhoSlot + espaco);
        float y = gridY + linha * (tamanhoSlot + espaco);

        Rectangle slot = {
            x,
            y,
            tamanhoSlot,
            tamanhoSlot
        };

        bool ocupado = i < (int)mochila.size();
        bool selecionado = ocupado && i == itemSelecionado;

        Color corSlot;

        if (selecionado) {
            corSlot = Color{85, 65, 35, 255};
        } else if (ocupado) {
            corSlot = Color{55, 48, 38, 255};
        } else {
            corSlot = Color{32, 31, 35, 255};
        }

        DrawRectangleRounded(
            slot,
            0.12f,
            8,
            corSlot
        );

        DrawRectangleLinesEx(
            slot,
            selecionado ? 4 : 2,
            selecionado
                ? Color{255, 220, 110, 255}
                : ocupado
                    ? Color{210, 180, 100, 255}
                    : Color{80, 80, 90, 255}
        );

        DrawText(
            TextFormat("%d", i + 1),
            (int)(x + 8),
            (int)(y + 6),
            18,
            Color{180, 180, 180, 255}
        );

        if (ocupado) {
            Rectangle icone = {
                x + tamanhoSlot * 0.18f,
                y + tamanhoSlot * 0.18f,
                tamanhoSlot * 0.64f,
                tamanhoSlot * 0.64f
            };

            Texture2D texturaItem = pegarTexturaItem(mochila[i].getImagem());

            desenharTexturaNoRetangulo(
                texturaItem,
                icone
            );
        }
    }

    float detalhesX = gridX + areaGradeW + 45.0f;
    float detalhesY = gridY;
    float detalhesW = painelX + painelW - detalhesX - 35.0f;
    float detalhesH = areaGradeH;

    Rectangle detalhes = {
        detalhesX,
        detalhesY,
        detalhesW,
        detalhesH
    };

    DrawRectangleRounded(
        detalhes,
        0.05f,
        12,
        Color{35, 34, 42, 245}
    );

    DrawRectangleLinesEx(
        detalhes,
        2,
        Color{110, 100, 85, 255}
    );

    DrawText(
        "Detalhes",
        (int)(detalhesX + 20),
        (int)(detalhesY + 20),
        26,
        RAYWHITE
    );

    if (mochila.size() == 0) {
        DrawText(
            "Mochila vazia",
            (int)(detalhesX + 20),
            (int)(detalhesY + 70),
            22,
            Color{170, 170, 170, 255}
        );
    }
    else if (itemSelecionado < 0 || itemSelecionado >= (int)mochila.size()) {
        DrawText(
            "Nenhum item selecionado",
            (int)(detalhesX + 20),
            (int)(detalhesY + 70),
            20,
            Color{170, 170, 170, 255}
        );
    }
    else {
        Item item = mochila[itemSelecionado];

        Rectangle preview = {
            detalhesX + 20,
            detalhesY + 70,
            90,
            90
        };

        DrawRectangleRounded(
            preview,
            0.15f,
            10,
            Color{65, 55, 40, 255}
        );

        Texture2D texturaPreview = pegarTexturaItem(item.getImagem());

        Rectangle imagemPreview = {
            preview.x + 10,
            preview.y + 10,
            preview.width - 20,
            preview.height - 20
        };

        desenharTexturaNoRetangulo(
            texturaPreview,
            imagemPreview
        );

        DrawRectangleLinesEx(
            preview,
            2,
            Color{220, 190, 120, 255}
        );

        DrawText(
            item.getNome().c_str(),
            (int)(detalhesX + 20),
            (int)(detalhesY + 180),
            24,
            RAYWHITE
        );

        Color corRaridade = RAYWHITE;

        if (item.getRaridade() == "Comum") {
            corRaridade = Color{190, 190, 190, 255};
        }
        else if (item.getRaridade() == "Raro") {
            corRaridade = Color{80, 150, 255, 255};
        }
        else if (item.getRaridade() == "Epico" || item.getRaridade() == "Épico") {
            corRaridade = Color{180, 90, 255, 255};
        }
        else if (item.getRaridade() == "Lendario" || item.getRaridade() == "Lendário") {
            corRaridade = Color{255, 170, 40, 255};
        }

        DrawText(
            TextFormat("Raridade: %s", item.getRaridade().c_str()),
            (int)(detalhesX + 20),
            (int)(detalhesY + 220),
            20,
            corRaridade
        );

        DrawText(
            TextFormat("Drop rate: %.2f", item.getDropRate()),
            (int)(detalhesX + 20),
            (int)(detalhesY + 255),
            20,
            Color{190, 190, 190, 255}
        );

        DrawText(
            "Imagem:",
            (int)(detalhesX + 20),
            (int)(detalhesY + 295),
            20,
            Color{190, 190, 190, 255}
        );

        desenharTextoQuebrado(
            item.getImagem(),
            (int)(detalhesX + 20),
            (int)(detalhesY + 325),
            (int)(detalhesW - 40),
            18,
            Color{150, 150, 150, 255}
        );
    }

    DrawText(
        "B: voltar",
        (int)(painelX + painelW - 150),
        (int)(painelY + painelH - 45),
        20,
        Color{210, 210, 210, 255}
    );
}