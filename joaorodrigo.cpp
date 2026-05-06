#include "mercado.h"
#include <iostream>

using namespace std;

// Função para meter os vendidos na árvore (simples e direta)
NoVenda* inserirNaArvore(NoVenda* raiz, string nome, int preco) {
    // Se o lugar estiver vazio, cria o nó e mete lá os dados
    if (raiz == nullptr) {
        NoVenda* novo = new NoVenda;
        novo->nome = nome;
        novo->preco = preco;
        novo->esq = nullptr;
        novo->dir = nullptr;
        return novo;
    }

    // Se o preço for mais barato, vai para a esquerda, se não vai para a direita
    if (preco < raiz->preco) {
        raiz->esq = inserirNaArvore(raiz->esq, nome, preco);
    }
    else {
        raiz->dir = inserirNaArvore(raiz->dir, nome, preco);
    }

    return raiz;
}

void verificarVendas(Sector* listaSectores) {
    Sector* sAtual = listaSectores;

    cout << "\n--- Vendas ---" << endl;

    // Percorre todos os setores do supermercado
    while (sAtual != nullptr) {
        Produto* pAtual = sAtual->listaProdutos;
        Produto* pAnterior = nullptr;

        // Percorre os produtos do setor atual
        while (pAtual != nullptr) {
            // Gera um número de 0 a 99. Se for menos de 25, vendeu!
            if (rand() % 100 < 25) {
                cout << "O produto " << pAtual->nome << " foi vendido!" << endl;

                // 1. Guarda na árvore de vendidos do setor
                sAtual->produtosVendidos = inserirNaArvore(sAtual->produtosVendidos, pAtual->nome, pAtual->preco);

                // 2. Tira o produto da lista ligada (parte chata)
                Produto* paraApagar = pAtual;

                if (pAnterior == nullptr) {
                    // Era o primeiro da lista
                    sAtual->listaProdutos = pAtual->next;
                }
                else {
                    // Estava no meio ou no fim
                    pAnterior->next = pAtual->next;
                }

                pAtual = pAtual->next; // Passa para o próximo antes de apagar
                delete paraApagar;     // Liberta a memória
                sAtual->ocupacao--;    // Agora o setor tem mais um lugar livre
            }
            else {
                // Não vendeu, continua a andar
                pAnterior = pAtual;
                pAtual = pAtual->next;
            }
        }
        sAtual = sAtual->next;
    }
}

// Função que tenta encontrar um setor para o produto
Sector* encontrarSectorParaProduto(Sector* listaSectores, string areaProduto) {
    Sector* aux = listaSectores;
    while (aux != nullptr) {
        if (aux->area == areaProduto && aux->ocupacao < aux->capacidade) {
            return aux;
        }
        aux = aux->next;
    }
    return nullptr; // Não encontrou nenhum setor livre para esta área
}


void reporStock(Sector* listaSectores, Produto*& armazem) {
    cout << "\n--- Reposicao de Stock ---" << endl;

    for (int i = 0; i < 10; i++) {
        if (armazem == nullptr) {
            cout << "Armazem ficou vazio!" << endl;
            break;
        }

        // 1. Tirar o primeiro produto do armazém (O mais antigo)
        Produto* pAposentado = armazem;
        armazem = armazem->next; // O segundo passa a ser o primeiro
        pAposentado->next = nullptr; // Desliga o produto da lista do armazém

        // 2. Tentar encontrar um setor para ele
        Sector* sDestino = encontrarSectorParaProduto(listaSectores, pAposentado->area);

        if (sDestino != nullptr) {

            pAposentado->next = sDestino->listaProdutos;
            sDestino->listaProdutos = pAposentado;
            sDestino->ocupacao++;
            cout << "Produto " << pAposentado->nome << " movido para Sector " << sDestino->id << endl;
        }
        else {
            cout << "Sem espaco nos setores para: " << pAposentado->nome << " (Area: " << pAposentado->area << ")" << endl;
            delete pAposentado;
        }
    }
    dezProdutosRandom(armazem, listaSectores);
    cout << "---------------------------------------------" << endl;
    cout << "Adicionalmente, 10 produtos foram adicionados ao armazem" << endl << endl;
}

void corrigirInicializacao(Sector* listaSectores) {
    Sector* aux = listaSectores;
    while (aux != nullptr) {
        aux->produtosVendidos = nullptr;
        aux = aux->next;
    }
}