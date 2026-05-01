#include "mercado.h"
#include <iostream>

void exibirMenu(Sector* setores, Produto* armazem) {
    char opcao;
    cout << "\n======================================" << endl;
    cout << "(S)eguinte Ciclo (Vender e Repor)" << endl;
    cout << "(G)estao" << endl;
    cout << "(Q)uit" << endl;
    cout << "Escolha: ";
    cin >> opcao;

    switch (opcao) {
        case 's':
        case 'S':
            system("cls");
            verificarVendas(setores);
            reporStock(setores, armazem);

            system("pause");
            mostrarSuper(setores, armazem);
            break;
        case 'g':
        case 'G':
            system("cls");
            cout << "em desenvolvimento";

            system("pause");
            mostrarSuper(setores, armazem);
            break;
        default:
            break;
    }
}