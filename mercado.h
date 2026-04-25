#ifndef UNTITLED_MERCADO_H
#define UNTITLED_MERCADO_H

#include <string>

using namespace std;

struct Produto {
    string nome;
    string fornecedor;
    string area;
    float preco;

    Produto* proximo;
};

Produto* criarProduto(string nome, string fornecedor, string area, float preco);
void inserirNoArmazem(Produto*& cabeca, Produto* novo);

#endif //UNTITLED_MERCADO_H