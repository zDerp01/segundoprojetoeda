#ifndef UNTITLED_MERCADO_H
#define UNTITLED_MERCADO_H

#include <string>

using namespace std;

struct Produto {
    string nome;
    string fornecedor;
    string area;
    float preco;

    Produto* next;
};

struct Sector {
    char id;
    string responsavel;
    string area;
    int capacidade;
    int ocupacao;

    Produto* listaProdutos;
    Sector* next;
};

string* listarFicheiro(string nomeFicheiro, int &totalNomes);
Produto* criarProduto(string nomes[], int tamanhoN, string fornecedores[], int tamanhoF, string areasSetores[], int tamanhoA);
void inicializarSupermercado(Sector*& listaSectores);
void inicializarArmazem(Produto*& armazem, Sector* listaSectores);
void mostrarSuper(Sector* listaSectores, Produto* armazem);

#endif