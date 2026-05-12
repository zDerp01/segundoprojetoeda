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

void dezProdutosRandom(Produto*& armazem, Sector* listaSectores) {
    int tamanhoN = 0, tamanhoF = 0, tamanhoS = 0;
    string* nomes = listarFicheiro("produtos.txt", tamanhoN);
    string* fornecedores = listarFicheiro("fornecedores.txt", tamanhoF);

    string areasNoSuper[12];
    Sector* atual = listaSectores;
    while (atual != nullptr && tamanhoS < 12) {
        areasNoSuper[tamanhoS] = atual->area;
        tamanhoS++;
        atual = atual->next;
    }

    for (int i = 0; i < 10; i++) {
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

bool produtoJaExisteNaArea(Sector* listaSectores, string nomeProduto, string areaAlvo) {
    Sector* sAtual = listaSectores;

    while (sAtual != nullptr) {
        if (sAtual->area == areaAlvo) {
            Produto* pAtual = sAtual->listaProdutos;
            while (pAtual != nullptr) {
                if (pAtual->nome == nomeProduto) {
                    return true;
                }
                pAtual = pAtual->next;
            }
        }
        sAtual = sAtual->next;
    }
    return false;
}

void mostrarSuper(Sector* listaSectores, Produto* armazem) {
    Sector* sAtual = listaSectores;
    int idProduto = 1;

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

    //armazem
    cout << "\n====================== ARMAZEM ======================" << endl;
    cout << left << setw(26) << "PRODUTO"
         << setw(18) << "PRECO"
         << setw(24) << "AREA" << endl;
    cout << "-----------------------------------------------------" << endl;

    Produto* aAtual = armazem;
    if (aAtual == nullptr) {
        cout << "   (Armazem vazio - Aguardando novos produtos)" << endl;
    }
    else {
        while (aAtual != nullptr) {
            cout << right << setw(2) << idProduto << ". " << left << setw(24) << aAtual->nome
                 << right << setw(2) << aAtual->preco << left << setw(14) << " euros"
                 << left << setw(24) << aAtual->area << endl;
            aAtual = aAtual->next;
            idProduto++;
        }
    }
    cout << "=====================================================\n" << endl;

    system("pause");

    exibirMenu(listaSectores, armazem);
}

void removerProduto(Sector* listaSectores, Produto*& armazem, string nomeProduto) {
    Sector* sAtual = listaSectores;

    while (sAtual != nullptr) {
        Produto* pAtual = sAtual->listaProdutos;
        Produto* pAnterior = nullptr;

        while (pAtual != nullptr) {
            if (pAtual->nome == nomeProduto) {
                if (pAnterior == nullptr)
                    sAtual->listaProdutos = pAtual->next;
                else
                    pAnterior->next = pAtual->next;
                    pAtual = pAtual->next;

                    sAtual->ocupacao--;
                    cout << "Produto removido do sector " << sAtual->id << endl;
            }
            else {
                pAnterior = pAtual;
                pAtual = pAtual->next;
            }
        }

        sAtual = sAtual->next;
    }

    Produto* apAtual = armazem;
    Produto* apAnterior = nullptr;

    while (apAtual != nullptr) {
        if (apAtual->nome == nomeProduto) {
            if (apAnterior == nullptr) {
                apAtual = apAtual->next;
            }
            else {
                apAnterior->next = apAtual->next;
                apAtual = apAtual->next;

                cout << "Produto removido do armazem" << endl << endl;
            }
        }
        else {
            apAnterior = apAtual;
            apAtual = apAtual->next;
        }
    }
}

void alterarPreco(Produto*& armazem, string nomeProduto, int novoPreco) {

    Produto* apAtual = armazem;

    while (apAtual != nullptr) {
        if (apAtual->nome == nomeProduto) {
            apAtual->preco = novoPreco;
            apAtual = apAtual->next;

            cout << "Preco atualizado!" << endl;
        }
        else {
            apAtual = apAtual->next;
        }
    }
}

void iniciarCampanha(Sector* listaSectores, Produto* armazem, string nomeArea, int desconto, int numCiclos) {
    Sector* sAtual = listaSectores;
    while (sAtual != nullptr) {
        Produto* pAtual = sAtual->listaProdutos;
        while (pAtual != nullptr) {
            if (pAtual->area == nomeArea && pAtual->ciclosPromo == 0) {
                pAtual->precoOriginal = pAtual->preco;
                pAtual->preco -= (pAtual->preco * desconto / 100);
                pAtual->ciclosPromo = numCiclos;
            }
            pAtual = pAtual->next;
        }
        sAtual = sAtual->next;
    }

    Produto* apAtual = armazem;
    while (apAtual != nullptr) {
        if (apAtual->area == nomeArea && apAtual->ciclosPromo == 0) {
            apAtual->precoOriginal = apAtual->preco;
            apAtual->preco -= (apAtual->preco * desconto / 100);
            apAtual->ciclosPromo = numCiclos;
        }
        apAtual = apAtual->next;
    }
    cout << "Campanha de " << desconto << "% aplicada a area " << nomeArea << " por " << numCiclos << " ciclos" << endl;
}

void atualizarCiclosCampanha(Sector* listaSectores, Produto* armazem) {
    cout << "\n--- Campanhas ---" << endl;
    Sector* sAtual = listaSectores;
    while (sAtual != nullptr) {
        Produto* pAtual = sAtual->listaProdutos;
        bool terminouNesteSector = false;

        while (pAtual != nullptr) {
            if (pAtual->ciclosPromo > 0) {
                pAtual->ciclosPromo--;
                if (pAtual->ciclosPromo == 0) {
                    pAtual->preco = pAtual->precoOriginal;
                    terminouNesteSector = true;
                }
            }
            pAtual = pAtual->next;
        }

        if (terminouNesteSector) {
            cout << "A campanha no Sector " << sAtual->id << " (" << sAtual->area << ") terminou" << endl;
        }

        sAtual = sAtual->next;
    }

    Produto* apAtual = armazem;
    while (apAtual != nullptr) {
        if (apAtual->ciclosPromo) {
            apAtual->ciclosPromo--;
            if (apAtual->ciclosPromo == 0) apAtual->preco = apAtual->precoOriginal;
        }
        apAtual = apAtual->next;
    }
}

void criarArea(Sector*& listaSectores, string nomeArea, string respSector, int capSector) {
    Sector* novo = new Sector;

    Sector* aux = listaSectores;
    while (aux->next != nullptr) {
        aux = aux->next;
    }

    novo->id = aux->id + 1;
    novo->area = nomeArea;
    novo->responsavel = respSector;
    novo->capacidade = capSector;
    novo->ocupacao = 0;
    novo->listaProdutos = nullptr;
    novo->produtosVendidos = nullptr;
    novo->next = nullptr;

    Sector* aux2 = listaSectores;
    while (aux2->next != nullptr) aux2 = aux2->next;
    aux2->next = novo;

    cout << "Sector " << novo->id << " criado com a area " << novo->area << endl << endl;
}

void exibirMenuGestao(Sector* setores, Produto* armazem) {
    system("cls");

    int opcao, preco, desconto, dias, capSector;
    string nome, nome2;

    cout << "\n***** Bem Vindo Gestor *****" << endl;
    cout << "(1) - Remover Produto" << endl;
    cout << "(2) - Atualizar Preco" << endl;
    cout << "(3) - Iniciar Campanha" << endl;
    cout << "(4) - Gravar supermercado" << endl;
    cout << "(5) - Carregar supermercado" << endl;
    cout << "(6) - Imprimir produtos" << endl;
    cout << "(7) - Criar nova area" << endl;
    cout << "(8) - Mostrar registo de vendas" << endl;
    cout << "(0) - Voltar" << endl;
    cout << "\n****************************" << endl << endl;
    cout << "Escolha: ";
    cin >> opcao;

    switch (opcao) {
        case 1:
            system("cls");
            cout << "\n****************************" << endl;
            cout << "Nome do produto a remover: ";
            cin.ignore();
            getline(cin, nome);
            system("cls");
            removerProduto(setores, armazem, nome);

            system("pause");
            exibirMenuGestao(setores, armazem);
            break;
        case 2:
            system("cls");
            cout << "\n****************************" << endl;
            cout << "Nome do produto: ";
            cin.ignore();
            getline(cin, nome);

            cout << "\nNovo preco: ";
            cin >> preco;

            system("cls");
            alterarPreco(armazem, nome, preco);

            system("pause");
            exibirMenuGestao(setores, armazem);
            break;
        case 3:
            system("cls");
            cout << "\n****************************" << endl;
            cout << "Nome da area: ";
            cin.ignore();
            getline(cin, nome);

            cout << "\nDesconto (%): ";
            cin >> desconto;

            cout << "\nDuracao da campanha (em ciclos): ";
            cin >> dias;

            system("cls");
            iniciarCampanha(setores, armazem, nome, desconto, dias);

            system("pause");
            exibirMenuGestao(setores, armazem);
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            break;
        case 7:
            system("cls");
            cout << "\n****************************" << endl;
            cout << "Sera necessaria a criacao de um novo setor para adicionar uma area!" << endl << endl;
            system("pause");
            system("cls");
            cout << "\n****************************" << endl;
            cout << "Nome da Nova Area: ";
            cin.ignore();
            getline(cin, nome);
            cout << "Responsavel pelo Sector: ";
            cin.ignore();
            getline(cin, nome2);
            cout << "Capacidade do Sector: ";
            cin >> capSector;
            system("cls");
            criarArea(setores, nome, nome2, capSector);

            system("pause");
            exibirMenuGestao(setores, armazem);
            break;
        case 8:
            system("cls");
            mostrarRegistoVendas(setores);

            system("pause");
            exibirMenuGestao(setores, armazem);
            break;
        default:
            mostrarSuper(setores, armazem);
            break;
    }
}