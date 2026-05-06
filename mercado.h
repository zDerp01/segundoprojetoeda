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

// Criar a estrutura para a Árvore de Vendidos
struct NoVenda {
    string nome;
    int preco;
    NoVenda *esq, *dir;
};

struct Sector {
    char id;
    string responsavel;
    string area;
    int capacidade;
    int ocupacao;
    Produto* listaProdutos;

    //Ponteiro para a árvore de vendas deste setor
    NoVenda* produtosVendidos;

    Sector* next;
};

string* listarFicheiro(string nomeFicheiro, int &totalNomes);
Produto* criarProduto(string nomes[], int tamanhoN, string fornecedores[], int tamanhoF, string areasSetores[], int tamanhoA);
void inicializarSupermercado(Sector*& listaSectores);
void inicializarArmazem(Produto*& armazem, Sector* listaSectores);
void mostrarSuper(Sector* listaSectores, Produto* armazem);
NoVenda* inserirNaArvore(NoVenda* raiz, string nome, int preco);
void verificarVendas(Sector* listaSectores);
Sector* encontrarSectorParaProduto(Sector* listaSectores, string areaProduto);
void reporStock(Sector* listaSectores, Produto*& armazem);
void corrigirInicializacao(Sector* listaSectores);

#endif