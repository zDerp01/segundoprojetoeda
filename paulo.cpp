#include "mercado.h"
#include <iostream>
#include <fstream>

using namespace std;

// Mostra o menu principal e trata as escolhas
void exibirMenu(Sector* setores, Produto* armazem) {
    char opcao;
    cout << "\n======================================" << endl;
    cout << "(S)eguinte Ciclo (Vender e Repor)" << endl;
    cout << "(G)estao" << endl;
    cout << "(Q)uit" << endl << endl;
    cout << "Escolha: ";
    cin >> opcao;

    switch (opcao) {
        case 's':
        case 'S':
            system("cls");
            verificarVendas(setores);
            atualizarCiclosCampanha(setores, armazem);
            reporStock(setores, armazem);

            system("pause");
            mostrarSuper(setores, armazem);
            break;
        case 'g':
        case 'G':
            system("cls");
            exibirMenuGestao(setores, armazem);
            break;
        default:
            break;
    }
}

// Guarda a arvore de vendas no ficheiro
void guardarArvore(ofstream& file, NoVenda* raiz) {
    if (raiz == nullptr) {
        file << "#" << endl;
        return;
    }
    file << raiz->nome << endl;
    file << raiz->preco << endl;
    guardarArvore(file, raiz->esq);
    guardarArvore(file, raiz->dir);
}

// Carrega a arvore de vendas do ficheiro
NoVenda* carregarArvore(ifstream& file) {
    string nome;
    if (!(getline(file, nome)) || nome == "#") {
        return nullptr;
    }
    int preco;
    file >> preco;
    file.ignore();
    NoVenda* novo = new NoVenda;
    novo->nome = nome;
    novo->preco = preco;
    novo->esq = carregarArvore(file);
    novo->dir = carregarArvore(file);
    return novo;
}

// Guarda a lista de produtos no ficheiro
void guardarListaProdutos(ofstream& file, Produto* p) {
    while (p != nullptr) {
        file << "P" << endl;
        file << p->nome << endl;
        file << p->fornecedor << endl;
        file << p->area << endl;
        file << p->preco << endl;
        file << p->precoOriginal << endl;
        file << p->ciclosPromo << endl;
        p = p->next;
    }
    file << "E" << endl;
}

// Carrega a lista de produtos do ficheiro
Produto* carregarListaProdutos(ifstream& file) {
    string marker;
    Produto* cabeca = nullptr;
    Produto* atual = nullptr;

    while (getline(file, marker) && marker == "P") {
        Produto* novo = new Produto;
        getline(file, novo->nome);
        getline(file, novo->fornecedor);
        getline(file, novo->area);
        file >> novo->preco;
        file >> novo->precoOriginal;
        file >> novo->ciclosPromo;
        file.ignore();
        novo->next = nullptr;

        if (cabeca == nullptr) {
            cabeca = novo;
            atual = novo;
        } else {
            atual->next = novo;
            atual = novo;
        }
    }
    return cabeca;
}

// Apaga a lista de produtos da memoria
void libertarProdutos(Produto*& p) {
    while (p != nullptr) {
        Produto* aux = p;
        p = p->next;
        delete aux;
    }
}

// Apaga a arvore de vendas da memoria
void libertarArvore(NoVenda* raiz) {
    if (raiz == nullptr) return;
    libertarArvore(raiz->esq);
    libertarArvore(raiz->dir);
    delete raiz;
}

// Limpa toda a memoria do supermercado
void libertarSupermercado(Sector*& setores, Produto*& armazem) {
    libertarProdutos(armazem);
    while (setores != nullptr) {
        Sector* aux = setores;
        setores = setores->next;
        libertarProdutos(aux->listaProdutos);
        libertarArvore(aux->produtosVendidos);
        delete aux;
    }
}

// Guarda todo o estado do supermercado num ficheiro
void gravarSupermercado(Sector* setores, Produto* armazem, string nomeFicheiro) {
    ofstream file(nomeFicheiro);
    if (!file.is_open()) {
        cout << "Erro ao criar o ficheiro!" << endl;
        return;
    }

    Sector* s = setores;
    while (s != nullptr) {
        file << "S" << endl;
        file << s->id << endl;
        file << s->responsavel << endl;
        file << s->area << endl;
        file << s->capacidade << endl;
        file << s->ocupacao << endl;
        guardarListaProdutos(file, s->listaProdutos);
        guardarArvore(file, s->produtosVendidos);
        s = s->next;
    }
    file << "END_SECTORES" << endl;
    guardarListaProdutos(file, armazem);
    file.close();
    cout << "Supermercado gravado com sucesso!" << endl;
}

// Carrega o estado do supermercado de um ficheiro
void carregarSupermercado(Sector*& setores, Produto*& armazem, string nomeFicheiro) {
    ifstream file(nomeFicheiro);
    if (!file.is_open()) {
        cout << "Erro ao abrir o ficheiro!" << endl;
        return;
    }

    libertarSupermercado(setores, armazem);

    string marker;
    Sector* sCabeca = nullptr;
    Sector* sAtual = nullptr;

    while (getline(file, marker) && marker == "S") {
        Sector* novo = new Sector;
        file >> novo->id;
        file.ignore();
        getline(file, novo->responsavel);
        getline(file, novo->area);
        file >> novo->capacidade;
        file >> novo->ocupacao;
        file.ignore();
        novo->listaProdutos = carregarListaProdutos(file);
        novo->produtosVendidos = carregarArvore(file);
        novo->next = nullptr;

        if (sCabeca == nullptr) {
            sCabeca = novo;
            sAtual = novo;
        } else {
            sAtual->next = novo;
            sAtual = novo;
        }
    }
    
    armazem = carregarListaProdutos(file);
    setores = sCabeca;
    file.close();
    cout << "Supermercado carregado com sucesso!" << endl;
}

// Lista todos os produtos (setores e armazem)
void imprimirProdutos(Sector* setores, Produto* armazem) {
    cout << "\n======= LISTA DE TODOS OS PRODUTOS =======" << endl;

    Sector* sAtual = setores;
    while (sAtual != nullptr) {
        cout << "\nSetor: " << sAtual->id << " (" << sAtual->area << ")" << endl;
        
        Produto* pAtual = sAtual->listaProdutos;
        if (pAtual == nullptr) {
            cout << "  (Vazio)" << endl;
        } else {
            while (pAtual != nullptr) {
                cout << "  - " << pAtual->nome << " [" << pAtual->fornecedor << "] : " << pAtual->preco << " euros" << endl;
                pAtual = pAtual->next;
            }
        }
        sAtual = sAtual->next;
    }

    cout << "\nArmazem:" << endl;
    Produto* aAtual = armazem;
    if (aAtual == nullptr) {
        cout << "  (Vazio)" << endl;
    } else {
        while (aAtual != nullptr) {
            cout << "  - " << aAtual->nome << " [" << aAtual->fornecedor << "] : " << aAtual->preco << " euros (" << aAtual->area << ")" << endl;
            aAtual = aAtual->next;
        }
    }
    cout << "==========================================" << endl;
}