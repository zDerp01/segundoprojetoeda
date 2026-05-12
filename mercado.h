#ifndef UNTITLED_MERCADO_H
#define UNTITLED_MERCADO_H

#include <string>

using namespace std;

struct Produto {
    string nome;
    string fornecedor;
    string area;
    int preco;
    int precoOriginal;
    int ciclosPromo;

    Produto* next;
};

// criar a estrutura para a Árvore de Vendidos
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

    NoVenda* produtosVendidos;

    Sector* next;
};

string* listarFicheiro(string nomeFicheiro, int &totalNomes);
Produto* criarProduto(string nomes[], int tamanhoN, string fornecedores[], int tamanhoF, string areasSetores[], int tamanhoA);
void inicializarSupermercado(Sector*& listaSectores);
void inicializarArmazem(Produto*& armazem, Sector* listaSectores);
void mostrarSuper(Sector* listaSectores, Produto* armazem);
void exibirMenu(Sector* setores, Produto* armazem);
void exibirMenuGestao(Sector* setores, Produto* armazem);
void removerProduto(Sector* listaSectores, Produto*& armazem, string nomeProduto);
void alterarPreco(Produto*& armazem, string nomeProduto, int novoPreco);
void criarArea(Sector*& listaSectores, string nomeArea, string respSector, int capSector);
void atualizarCiclosCampanha(Sector* listaSectores, Produto* armazem);
bool produtoJaExisteNaArea(Sector* listaSectores, string nomeProduto, string areaAlvo);
void dezProdutosRandom(Produto*& armazem, Sector* listaSectores);
NoVenda* inserirNaArvore(NoVenda* raiz, string nome, int preco);
void verificarVendas(Sector* listaSectores);
Sector* encontrarSectorParaProduto(Sector* listaSectores, string areaProduto);
void reporStock(Sector* listaSectores, Produto*& armazem);
void corrigirInicializacao(Sector* listaSectores);
void imprimirVendasOrdem(NoVenda* raiz);
void mostrarRegistoVendas(Sector* listaSectores);
#endif