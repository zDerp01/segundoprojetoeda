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

    // Mostra o estado inicial antes do menu aparecer
    mostrarSuper(setores, armazem);

    return 0;
}