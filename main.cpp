#include <iostream>
#include "mercado.h"

using namespace std;

int main() {
    // Configura o gerador de números aleatórios
    srand(time(NULL));

    Sector* setores = nullptr;
    Produto* armazem = nullptr;

    // Inicialização do sistema
    inicializarSupermercado(setores);
    inicializarArmazem(armazem, setores);

    // NOVO: Garante que as árvores de vendas começam vazias (limpa lixo de memória)
    corrigirInicializacao(setores);

    int opcao;

    // Mostra o estado inicial antes do menu aparecer
    mostrarSuper(setores, armazem);

    do {
        cout << "\n======================================" << endl;
        cout << "1. Seguinte Ciclo (Vender e Repor)" << endl;
        cout << "0. Sair" << endl;
        cout << "Escolha: ";
        cin >> opcao;

        if (opcao == 1) {
            // 1. Executa a lógica (vendas e reposição)
            // Aqui vão aparecer as mensagens: "Produto X movido para Sector Y"
            verificarVendas(setores);
            reporStock(setores, armazem);

            cout << "\n concluido com sucesso." << endl;

            // Pausa para conseguires ler quem foi vendido e quem foi movido
            system("pause");

            // 2. Limpa o texto antigo e mostra o resultado final nas prateleiras
            system("cls");
            mostrarSuper(setores, armazem);

            cout << "\nO mapa acima reflete o estado atualizado do supermercado." << endl;
        }

    } while (opcao != 0);

    cout << "A sair do programa..." << endl;
    return 0;
}