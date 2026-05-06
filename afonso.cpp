#include "mercado.h"
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;

string* listarFicheiro(string nomeFicheiro, int &totalNomes) {
    ifstream file(nomeFicheiro);
    string linha;
    totalNomes = 0;

    while (getline(file, linha)) {
        if (linha != "") totalNomes++;
    }

    file.clear();
    file.seekg(0);

    string* listaNomes = new string[totalNomes];
    int i = 0;
    while (getline(file, linha)) {
        if (linha != "") {
            listaNomes[i] = linha;
            i++;
        }
    }
    file.close();
    return listaNomes;
}

Produto* criarProduto(string nomes[], int tamanhoN, string fornecedores[], int tamanhoF, string areasSetores[], int tamanhoA) {
    Produto* novo = new Produto;

    novo->nome = nomes[rand() % tamanhoN];
    novo->fornecedor = fornecedores[rand() % tamanhoF];
    novo->area = areasSetores[rand() % tamanhoA];

    novo->preco = (1 + (rand() % 40)) * 2;
    novo->next = nullptr;

    return novo;
}

void inicializarSupermercado(Sector*& listaSectores) {
    int numSectores = 8 + rand() % 5;

    int numAreasNoFicheiro = 0;
    string* listaAreas = listarFicheiro("areas.txt", numAreasNoFicheiro);

    for (int i = 0; i < numSectores; i++) {
        Sector* novo = new Sector;

        novo->id = 'A' + i;
        novo->capacidade = 5 + rand() % 6;
        novo->ocupacao = 0;
        novo->listaProdutos = nullptr;
        novo->next = nullptr;

        cout << "Introduza o responsavel pelo Sector " << novo->id << ": ";
        cin >> novo->responsavel;
        system("cls");

        int randIndice = rand() % numAreasNoFicheiro;
        novo->area = listaAreas[randIndice];

        if (listaSectores == nullptr) {
            listaSectores = novo;
        }
        else {
            Sector* aux = listaSectores;
            while (aux->next != nullptr) aux = aux->next;
            aux->next = novo;
        }
    }

    delete[] listaAreas;
}

void inicializarArmazem(Produto*& armazem, Sector* listaSectores) {
    int tamanhoN = 0, tamanhoF = 0, tamanhoS = 0;

    string* nomes = listarFicheiro("produtos.txt", tamanhoN);
    string* fornecedores = listarFicheiro("fornecedores.txt", tamanhoF);

    string areasNoSuper[12]; //max 12 sectores
    Sector* atual = listaSectores;
    while (atual != nullptr) {
        areasNoSuper[tamanhoS] = atual->area;
        tamanhoS++;
        atual = atual->next;
    }

    for (int i = 0; i < 50; i++) {
        Produto* p = criarProduto(nomes, tamanhoN, fornecedores, tamanhoF, areasNoSuper, tamanhoS);

        if (armazem == nullptr) {
            armazem = p;
        }
        else {
            Produto* aux = armazem;
            while (aux->next != nullptr) aux = aux->next;
            aux->next = p;
        }
    }

    delete[] nomes;
    delete[] fornecedores;
}

void mostrarSuper(Sector* listaSectores, Produto* armazem) {
    Sector* sAtual = listaSectores;

    system("cls");
    cout << "\n================= SUPERMERCADO EDA =================" << endl;

    //percorrer cada sector
    while (sAtual != nullptr) {
        cout << "Sector: " << sAtual->id
             << " | Responsavel: " << sAtual->responsavel
             << " | Capacidade: " << sAtual->capacidade
             << " | Produtos: " << sAtual->ocupacao
             << " | Area: " << sAtual->area << endl;

        //percorrer os produtos
        Produto* pAtual = sAtual->listaProdutos;
        if (pAtual == nullptr) {
            cout << "  (Sector vazio)" << endl;
        }
        else {
            while (pAtual != nullptr) {
                cout << "  Produto: " << pAtual->nome
                     << " | Preco: " << pAtual->preco << " Euros" << endl;
                pAtual = pAtual->next;
            }
        }
        cout << "----------------------------------------------------" << endl;
        sAtual = sAtual->next;
    }

    system("pause");

    //armazem
    cout << "\n====================== ARMAZEM ======================" << endl;
    cout << left << setw(26) << "PRODUTO"
         << setw(16) << "PRECO"
         << setw(24) << "AREA" << endl;
    cout << "-----------------------------------------------------" << endl;

    Produto* aAtual = armazem;
    if (aAtual == nullptr) {
        cout << "   (Armazem vazio - Aguardando novos produtos)" << endl;
    }
    else {
        while (aAtual != nullptr) {
            cout << left << setw(26) << aAtual->nome
                 << right << setw(2) << aAtual->preco << left << setw(14) << " euros"
                 << left << setw(24) << aAtual->area << endl;
            aAtual = aAtual->next;
        }
    }
    cout << "=====================================================\n" << endl;

    system("pause");
}