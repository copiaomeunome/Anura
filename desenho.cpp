#include "desenho.h"

#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

#include "Item/Item_Arma_Ranged/Item_Arma_Ranged.h"

using namespace std;

static unordered_map<string, Texture2D> cacheTexturasItens;

Color pegarCorRaridade(string raridade);
Texture2D pegarTexturaItem(string caminho);
void desenharTexturaNoRetangulo(Texture2D textura, Rectangle destino);
void desenharTextoQuebrado(string texto, int x, int y, int larguraMaxima, int tamanhoFonte, Color cor);

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

Color pegarCorRaridade(string raridade) {
    if (raridade == "Comum") {
        return Color{190, 190, 190, 255};
    }
    else if (raridade == "Raro") {
        return Color{80, 150, 255, 255};
    }
    else if (raridade == "Epico" || raridade == "Épico") {
        return Color{180, 90, 255, 255};
    }
    else if (raridade == "Lendario" || raridade == "Lendário") {
        return Color{255, 170, 40, 255};
    }

    return RAYWHITE;
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

bool desenharBotao(string texto, Rectangle botao) {
    Vector2 mouse = GetMousePosition();

    bool mouseEmCima = CheckCollisionPointRec(mouse, botao);

    Color corFundo = mouseEmCima
        ? Color{95, 75, 45, 255}
        : Color{45, 40, 35, 255};

    Color corBorda = mouseEmCima
        ? Color{255, 220, 120, 255}
        : Color{180, 150, 90, 255};

    DrawRectangleRounded(
        botao,
        0.18f,
        12,
        corFundo
    );

    DrawRectangleLinesEx(
        botao,
        3,
        corBorda
    );

    int tamanhoFonte = 26;
    int textoW = MeasureText(texto.c_str(), tamanhoFonte);

    DrawText(
        texto.c_str(),
        (int)(botao.x + botao.width / 2 - textoW / 2),
        (int)(botao.y + botao.height / 2 - tamanhoFonte / 2),
        tamanhoFonte,
        RAYWHITE
    );

    return mouseEmCima && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
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
    int tamanhoSlot = 46;
    int tamanhoIcone = 34;

    int total = min((int)itens.size(), (int)posicoes.size());

    for (int i = 0; i < total; i++) {
        int telaX = posicoes[i].first - mapa1.getCamera().first;
        int telaY = posicoes[i].second - mapa1.getCamera().second;

        Rectangle fundoItem = {
            (float)telaX,
            (float)telaY,
            (float)tamanhoSlot,
            (float)tamanhoSlot
        };

        Rectangle icone = {
            (float)telaX + (tamanhoSlot - tamanhoIcone) / 2.0f,
            (float)telaY + (tamanhoSlot - tamanhoIcone) / 2.0f,
            (float)tamanhoIcone,
            (float)tamanhoIcone
        };

        DrawRectangleRounded(
            {
                fundoItem.x + 4,
                fundoItem.y + 4,
                fundoItem.width,
                fundoItem.height
            },
            0.20f,
            8,
            Fade(BLACK, 0.35f)
        );

        DrawRectangleRounded(
            fundoItem,
            0.20f,
            8,
            Color{35, 32, 28, 230}
        );

        DrawRectangleLinesEx(
            fundoItem,
            2,
            pegarCorRaridade(itens[i].getRaridade())
        );

        Texture2D texturaItem = pegarTexturaItem(itens[i].getImagem());

        desenharTexturaNoRetangulo(
            texturaItem,
            icone
        );
    }
}

void desenharEquipamentoHUD(
    string titulo,
    Item item,
    Rectangle slot,
    float textoX
) {
    DrawRectangleRounded(
        slot,
        0.15f,
        8,
        Color{35, 32, 28, 220}
    );

    DrawRectangleLinesEx(
        slot,
        2,
        pegarCorRaridade(item.getRaridade())
    );

    Texture2D textura = pegarTexturaItem(item.getImagem());

    Rectangle icone = {
        slot.x + slot.width * 0.18f,
        slot.y + slot.height * 0.18f,
        slot.width * 0.64f,
        slot.height * 0.64f
    };

    desenharTexturaNoRetangulo(
        textura,
        icone
    );

    DrawText(
        titulo.c_str(),
        (int)textoX,
        (int)(slot.y + 5),
        17,
        Color{210, 210, 210, 255}
    );

    DrawText(
        item.getNome().c_str(),
        (int)textoX,
        (int)(slot.y + 28),
        16,
        RAYWHITE
    );
}

void desenharEquipamentoRangedHUD(
    string titulo,
    Item_Arma_Ranged item,
    Rectangle slot,
    float textoX
) {
    DrawRectangleRounded(
        slot,
        0.15f,
        8,
        Color{35, 32, 28, 220}
    );

    DrawRectangleLinesEx(
        slot,
        2,
        pegarCorRaridade(item.getRaridade())
    );

    Texture2D textura = pegarTexturaItem(item.getImagem());

    Rectangle icone = {
        slot.x + slot.width * 0.18f,
        slot.y + slot.height * 0.18f,
        slot.width * 0.64f,
        slot.height * 0.64f
    };

    desenharTexturaNoRetangulo(
        textura,
        icone
    );

    DrawText(
        titulo.c_str(),
        (int)textoX,
        (int)(slot.y + 2),
        17,
        Color{210, 210, 210, 255}
    );

    DrawText(
        item.getNome().c_str(),
        (int)textoX,
        (int)(slot.y + 24),
        16,
        RAYWHITE
    );

    pair<int,int> municao = item.getMunicao();

    DrawText(
        TextFormat("%d/%d", municao.first, municao.second),
        (int)textoX,
        (int)(slot.y + 46),
        16,
        Color{230, 220, 170, 255}
    );
}

void desenharEquipamentosHUD(Protagonista& p, int largura, int altura) {
    float slotSize = (altura * 0.10f) * 0.70f;
    float espaco = 14.0f;

    float painelW = 245.0f;
    float painelH = 60.0f + 4 * slotSize + 3 * espaco;

    float painelX = largura - painelW - 25.0f;
    float painelY = (altura - painelH) / 2.0f;

    Rectangle painel = {
        painelX,
        painelY,
        painelW,
        painelH
    };

    DrawRectangleRounded(
        painel,
        0.08f,
        12,
        Color{20, 20, 25, 205}
    );

    DrawRectangleLinesEx(
        painel,
        2,
        Color{220, 190, 120, 255}
    );

    DrawText(
        "Equipados",
        (int)(painelX + 18),
        (int)(painelY + 16),
        22,
        RAYWHITE
    );

    float slotX = painelX + 18.0f;
    float textoX = slotX + slotSize + 14.0f;

    float primeiroY = painelY + 55.0f;

    Rectangle slotRanged = {
        slotX,
        primeiroY,
        slotSize,
        slotSize
    };

    Rectangle slotMelee = {
        slotX,
        primeiroY + 1 * (slotSize + espaco),
        slotSize,
        slotSize
    };

    Rectangle slotArmadura = {
        slotX,
        primeiroY + 2 * (slotSize + espaco),
        slotSize,
        slotSize
    };

    Rectangle slotCapacete = {
        slotX,
        primeiroY + 3 * (slotSize + espaco),
        slotSize,
        slotSize
    };

    Item_Arma_Ranged armaRanged = p.getArmaRanged();
    Item armaMelee = p.getArmaMelee();
    Item armadura = p.getArmadura();
    Item capacete = p.getCapacete();

    desenharEquipamentoRangedHUD(
        "Ranged",
        armaRanged,
        slotRanged,
        textoX
    );

    desenharEquipamentoHUD(
        "Melee",
        armaMelee,
        slotMelee,
        textoX
    );

    desenharEquipamentoHUD(
        "Armadura",
        armadura,
        slotArmadura,
        textoX
    );

    desenharEquipamentoHUD(
        "Capacete",
        capacete,
        slotCapacete,
        textoX
    );
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

    desenharEquipamentosHUD(p, largura, altura);
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

void desenharSlotEquipado(
    string titulo,
    Item item,
    Rectangle slot
) {
    DrawRectangleRounded(
        slot,
        0.12f,
        8,
        Color{35, 34, 42, 235}
    );

    DrawRectangleLinesEx(
        slot,
        2,
        pegarCorRaridade(item.getRaridade())
    );

    DrawText(
        titulo.c_str(),
        (int)(slot.x),
        (int)(slot.y - 22),
        18,
        Color{210, 210, 210, 255}
    );

    Rectangle icone = {
        slot.x + slot.width * 0.18f,
        slot.y + 8,
        slot.width * 0.64f,
        slot.width * 0.64f
    };

    Texture2D texturaItem = pegarTexturaItem(item.getImagem());

    desenharTexturaNoRetangulo(
        texturaItem,
        icone
    );

    DrawText(
        item.getNome().c_str(),
        (int)(slot.x + 6),
        (int)(slot.y + slot.height - 24),
        16,
        RAYWHITE
    );
}

void desenharSlotEquipadoRanged(
    string titulo,
    Item_Arma_Ranged item,
    Rectangle slot
) {
    DrawRectangleRounded(
        slot,
        0.12f,
        8,
        Color{35, 34, 42, 235}
    );

    DrawRectangleLinesEx(
        slot,
        2,
        pegarCorRaridade(item.getRaridade())
    );

    DrawText(
        titulo.c_str(),
        (int)(slot.x),
        (int)(slot.y - 22),
        18,
        Color{210, 210, 210, 255}
    );

    Rectangle icone = {
        slot.x + slot.width * 0.18f,
        slot.y + 8,
        slot.width * 0.64f,
        slot.width * 0.64f
    };

    Texture2D texturaItem = pegarTexturaItem(item.getImagem());

    desenharTexturaNoRetangulo(
        texturaItem,
        icone
    );

    DrawText(
        item.getNome().c_str(),
        (int)(slot.x + 6),
        (int)(slot.y + slot.height - 42),
        16,
        RAYWHITE
    );

    pair<int,int> municao = item.getMunicao();

    DrawText(
        TextFormat("Municao: %d/%d", municao.first, municao.second),
        (int)(slot.x + 6),
        (int)(slot.y + slot.height - 22),
        15,
        Color{220, 220, 180, 255}
    );
}

void desenharMochila(
    Protagonista& p,
    int largura,
    int altura,
    int itemSelecionado,
    int abaMochila
) {
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

    Rectangle abaItens = {
        painelX + 35,
        painelY + 78,
        140,
        38
    };

    Rectangle abaEquipados = {
        painelX + 185,
        painelY + 78,
        170,
        38
    };

    DrawRectangleRounded(
        abaItens,
        0.20f,
        10,
        abaMochila == 0 ? Color{85, 65, 35, 255} : Color{35, 34, 42, 245}
    );

    DrawRectangleRounded(
        abaEquipados,
        0.20f,
        10,
        abaMochila == 1 ? Color{85, 65, 35, 255} : Color{35, 34, 42, 245}
    );

    DrawRectangleLinesEx(
        abaItens,
        2,
        abaMochila == 0 ? Color{255, 220, 110, 255} : Color{110, 100, 85, 255}
    );

    DrawRectangleLinesEx(
        abaEquipados,
        2,
        abaMochila == 1 ? Color{255, 220, 110, 255} : Color{110, 100, 85, 255}
    );

    DrawText(
        "Q: Itens",
        (int)(abaItens.x + 18),
        (int)(abaItens.y + 9),
        18,
        RAYWHITE
    );

    DrawText(
        "E: Equipados",
        (int)(abaEquipados.x + 18),
        (int)(abaEquipados.y + 9),
        18,
        RAYWHITE
    );

    vector<Item> mochila = p.getMochila();

    float conteudoY = painelY + 140.0f;

    if (abaMochila == 0) {
        int colunas = 5;
        int totalSlots = p.getTamMochila();
        int linhas = (totalSlots + colunas - 1) / colunas;

        float espaco = 14.0f;
        float areaGradeW = painelW * 0.60f;
        float areaGradeH = painelH - 180.0f;

        float slotW = (areaGradeW - espaco * (colunas - 1)) / colunas;
        float slotH = linhas > 0
            ? (areaGradeH - espaco * (linhas - 1)) / linhas
            : areaGradeH;

        float tamanhoSlot = min(slotW, slotH);

        float gridX = painelX + 35.0f;
        float gridY = conteudoY;

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
        float detalhesY = conteudoY;
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
                pegarCorRaridade(item.getRaridade())
            );

            DrawText(
                item.getNome().c_str(),
                (int)(detalhesX + 20),
                (int)(detalhesY + 180),
                24,
                RAYWHITE
            );

            DrawText(
                TextFormat("Raridade: %s", item.getRaridade().c_str()),
                (int)(detalhesX + 20),
                (int)(detalhesY + 220),
                20,
                pegarCorRaridade(item.getRaridade())
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
    }
    else if (abaMochila == 1) {
        Item_Arma_Ranged armaRanged = p.getArmaRanged();
        Item armaMelee = p.getArmaMelee();
        Item armadura = p.getArmadura();
        Item capacete = p.getCapacete();

        float slotW = 185.0f;
        float slotH = 150.0f;
        float espacoX = 35.0f;
        float espacoY = 55.0f;

        float startX = painelX + 70.0f;
        float startY = conteudoY + 35.0f;

        Rectangle slotRanged = {
            startX,
            startY,
            slotW,
            slotH
        };

        Rectangle slotMelee = {
            startX + slotW + espacoX,
            startY,
            slotW,
            slotH
        };

        Rectangle slotArmadura = {
            startX,
            startY + slotH + espacoY,
            slotW,
            slotH
        };

        Rectangle slotCapacete = {
            startX + slotW + espacoX,
            startY + slotH + espacoY,
            slotW,
            slotH
        };

        desenharSlotEquipadoRanged("Arma ranged", armaRanged, slotRanged);
        desenharSlotEquipado("Arma melee", armaMelee, slotMelee);
        desenharSlotEquipado("Armadura", armadura, slotArmadura);
        desenharSlotEquipado("Capacete", capacete, slotCapacete);

        float infoX = startX + 2 * slotW + 2 * espacoX;
        float infoY = startY;
        float infoW = painelX + painelW - infoX - 45.0f;
        float infoH = 2 * slotH + espacoY;

        Rectangle info = {
            infoX,
            infoY,
            infoW,
            infoH
        };

        DrawRectangleRounded(
            info,
            0.05f,
            12,
            Color{35, 34, 42, 245}
        );

        DrawRectangleLinesEx(
            info,
            2,
            Color{110, 100, 85, 255}
        );

        DrawText(
            "Resumo",
            (int)(infoX + 20),
            (int)(infoY + 20),
            28,
            RAYWHITE
        );

        pair<int,int> municao = armaRanged.getMunicao();

        DrawText(
            TextFormat("Ranged: %s", armaRanged.getNome().c_str()),
            (int)(infoX + 20),
            (int)(infoY + 75),
            20,
            RAYWHITE
        );

        DrawText(
            TextFormat("Municao: %d/%d", municao.first, municao.second),
            (int)(infoX + 20),
            (int)(infoY + 105),
            20,
            Color{230, 220, 170, 255}
        );

        DrawText(
            TextFormat("Melee: %s", armaMelee.getNome().c_str()),
            (int)(infoX + 20),
            (int)(infoY + 150),
            20,
            RAYWHITE
        );

        DrawText(
            TextFormat("Armadura: %s", armadura.getNome().c_str()),
            (int)(infoX + 20),
            (int)(infoY + 195),
            20,
            RAYWHITE
        );

        DrawText(
            TextFormat("Capacete: %s", capacete.getNome().c_str()),
            (int)(infoX + 20),
            (int)(infoY + 240),
            20,
            RAYWHITE
        );
    }

    DrawText(
        "B: voltar | Q/E: trocar aba",
        (int)(painelX + painelW - 270),
        (int)(painelY + painelH - 45),
        20,
        Color{210, 210, 210, 255}
    );
}
AcaoMenu desenharMenuPrincipal(int largura, int altura) {
    DrawText(
        "ANURA",
        largura / 2 - MeasureText("ANURA", 70) / 2,
        altura * 0.20f,
        70,
        DARKGREEN
    );

    Rectangle botaoJogar = {
        largura / 2.0f - 160,
        altura / 2.0f - 90,
        320,
        70
    };

    Rectangle botaoContinuar = {
        largura / 2.0f - 160,
        altura / 2.0f,
        320,
        70
    };

    Rectangle botaoSair = {
        largura / 2.0f - 160,
        altura / 2.0f + 90,
        320,
        70
    };

    if (desenharBotao("Jogar", botaoJogar)) {
        return ACAO_JOGAR;
    }

    if (desenharBotao("Continuar", botaoContinuar)) {
        return ACAO_CONTINUAR;
    }

    if (desenharBotao("Sair", botaoSair)) {
        return ACAO_SAIR;
    }

    return ACAO_NENHUMA;
}
AcaoMenu desenharMenuPause(int largura, int altura) {
    DrawRectangle(
        0,
        0,
        largura,
        altura,
        Fade(BLACK, 0.60f)
    );

    DrawText(
        "PAUSADO",
        largura / 2 - MeasureText("PAUSADO", 55) / 2,
        altura * 0.20f,
        55,
        RAYWHITE
    );

    Rectangle botaoContinuar = {
        largura / 2.0f - 160,
        altura / 2.0f - 90,
        320,
        70
    };

    Rectangle botaoMenu = {
        largura / 2.0f - 160,
        altura / 2.0f,
        320,
        70
    };

    Rectangle botaoSair = {
        largura / 2.0f - 160,
        altura / 2.0f + 90,
        320,
        70
    };

    if (desenharBotao("Continuar", botaoContinuar)) {
        return ACAO_CONTINUAR;
    }

    if (desenharBotao("Voltar ao menu", botaoMenu)) {
        return ACAO_VOLTAR_MENU;
    }

    if (desenharBotao("Sair", botaoSair)) {
        return ACAO_SAIR;
    }

    return ACAO_NENHUMA;
}
AcaoMenu desenharMenuGameOver(int largura, int altura) {
    DrawText(
        "GAME OVER",
        largura / 2 - MeasureText("GAME OVER", 70) / 2,
        altura * 0.22f,
        70,
        RED
    );

    Rectangle botaoJogarNovamente = {
        largura / 2.0f - 180,
        altura / 2.0f - 45,
        360,
        70
    };

    Rectangle botaoMenu = {
        largura / 2.0f - 180,
        altura / 2.0f + 45,
        360,
        70
    };

    if (desenharBotao("Jogar novamente", botaoJogarNovamente)) {
        return ACAO_JOGAR_NOVAMENTE;
    }

    if (desenharBotao("Voltar ao menu", botaoMenu)) {
        return ACAO_VOLTAR_MENU;
    }

    return ACAO_NENHUMA;
}
AcaoMenu desenharMenuVitoria(int largura, int altura) {
    DrawText(
        "VITORIA",
        largura / 2 - MeasureText("VITORIA", 70) / 2,
        altura * 0.22f,
        70,
        DARKGREEN
    );

    Rectangle botaoMenu = {
        largura / 2.0f - 180,
        altura / 2.0f,
        360,
        70
    };

    if (desenharBotao("Voltar ao menu", botaoMenu)) {
        return ACAO_VOLTAR_MENU;
    }

    return ACAO_NENHUMA;
}