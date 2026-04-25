#include "mercado.h"
#include <iostream>

using namespace std;

Produto* criarProduto(string nome, string fornecedor, string area, float preco) {
    Produto* novo = new Produto;
    novo->nome = nome;
    novo->fornecedor = fornecedor;
    novo->area = area;
    novo->preco = preco;
    novo->proximo = nullptr;

    return novo;
}

void inserirNoArmazem(Produto*& head, Produto* novo) {
    if (head == nullptr) {
        head = novo;
    }
    else {
        Produto* atual = head;

        while (atual->proximo != nullptr) {
            atual = atual->proximo;
        }

        atual->proximo = novo;
    }
}