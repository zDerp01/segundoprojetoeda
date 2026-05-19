#include "mercado.h"
#include <iostream>

using namespace std;

/**
 * Função que permite inserir um produto vendido na Árvore Binária de Pesquisa (BST) do respetivo setor.
 * Organiza os nós de forma automática utilizando o preço como chave de ordenação.
 * @param raiz - Ponteiro para o nó atual da árvore de vendas.
 * @param nome - Nome do produto vendido a registar.
 * @param preco - Preço do produto, utilizado para determinar a posição na árvore.
 * @return Ponteiro para a raiz da árvore atualizada.
 */
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

/**
 * Função que permite executar a simulação probabilística de vendas em todos os setores do supermercado.
 * Remove os produtos vendidos das prateleiras e regista-os no histórico do setor.
 * @param listaSectores - Ponteiro para o início da lista ligada de setores.
 */
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
                cout << "O produto " << pAtual->nome << " foi vendido" << endl;

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

/**
 * Função que permite procurar um setor disponível que corresponda à área do produto e possua capacidade livre.
 * @param listaSectores - Ponteiro para o início da lista ligada de setores.
 * @param areaProduto - A categoria/área de mercado do produto a alocar.
 * @return Ponteiro para o Sector elegível encontrado, ou nullptr se não houver espaço.
 */
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

/**
 * Função que permite transferir até 10 produtos do armazém central para as prateleiras dos setores correspondentes.
 * Controla duplicações de mercado e faz a rotação de stock redundante.
 * @param listaSectores - Ponteiro para o início da lista ligada de setores.
 * @param armazem - Ponteiro de referência para a lista ligada de produtos no armazém.
 */
void reporStock(Sector* listaSectores, Produto*& armazem) {
    cout << "\n--- Reposicao de Stock ---" << endl;

    for (int i = 0; i < 10; i++) {
        if (armazem == nullptr) {
            cout << "Armazem ficou vazio!" << endl;
            break;
        }

        Produto* pCandidato = armazem;

        if (produtoJaExisteNaArea(listaSectores, pCandidato->nome, pCandidato->area)) {
            armazem = armazem->next;
            pCandidato->next = nullptr;
            Produto* ultimo = armazem;
            while (ultimo->next != nullptr) {
                ultimo = ultimo->next;
            }
            ultimo->next = pCandidato;

            cout << "Produto repetido na area " << pCandidato->area << ": " << pCandidato->nome << " (fica no armazem)" << endl;
            continue;
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
            cout << "Sem espaco nos sectores para: " << pAposentado->nome << " (Area: " << pAposentado->area << ")" << endl;
            delete pAposentado;
        }
    }
    dezProdutosRandom(armazem, listaSectores);
    cout << "---------------------------------------------" << endl;
    cout << "Adicionalmente, 10 produtos foram adicionados ao armazem" << endl << endl;
}

/**
 * Função que permite garantir que todos os ponteiros de árvores binárias de vendas são inicializados a vazio.
 * Previne falhas graves de segmentação (Segmentation Faults) decorrentes de lixo de memória.
 * @param listaSectores - Ponteiro para o início da lista ligada de setores.
 */
void corrigirInicializacao(Sector* listaSectores) {
    Sector* aux = listaSectores;
    while (aux != nullptr) {
        aux->produtosVendidos = nullptr;
        aux = aux->next;
    }
}

/**
 * Função que permite percorrer recursivamente a árvore binária de vendas em modo In-Order.
 * Imprime os dados formatados dos produtos por ordem crescente de preço.
 * @param raiz - Ponteiro para o nó atual da árvore de vendas que está a ser lido.
 */
void imprimirVendasRecursivo(NoVenda* raiz) {
    if (raiz == nullptr) return;

    imprimirVendasRecursivo(raiz->esq);
    cout << "  - " << raiz->nome << " | Preco: " << raiz->preco << " Euros" << endl;
    imprimirVendasRecursivo(raiz->dir);
}

/**
 * Função que permite procurar um responsável no supermercado e disparar a impressão do seu histórico de vendas.
 * @param listaSectores - Ponteiro para o início da lista ligada de setores.
 */
void mostrarRegistoVendas(Sector* listaSectores) {
    string nomeResp;
    cout << "\n>>> Historico de Vendas por Responsavel <<<" << endl;
    cout << "Introduza o nome do responsavel: ";
    cin.ignore();
    getline(cin, nomeResp);

    Sector* atual = listaSectores;
    bool encontrado = false;

    while (atual != nullptr) {
        if (atual->responsavel == nomeResp) {
            encontrado = true;
            cout << "\nResponsavel: " << atual->responsavel << " [Sector " << atual->id << "]" << endl;

            if (atual->produtosVendidos == nullptr) {
                cout << "  (Sem vendas registadas)" << endl;
            } else {
                // Chama a tua função de travessia da árvore
                imprimirVendasRecursivo(atual->produtosVendidos);
            }
        }
        atual = atual->next;
    }

    if (!encontrado) {
        cout << "\nResponsavel nao encontrado." << endl;
    }
    cout << "\n------------------------------------------" << endl;
}