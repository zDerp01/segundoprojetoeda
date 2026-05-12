#include <iostream>
#include "mercado.h"

using namespace std;

int main(int argc, char* argv[]) {
    srand(time(NULL));

    Sector* setores = nullptr;
    Produto* armazem = nullptr;

    if (argc > 1) {
        carregarSupermercado(setores, armazem, argv[1]);
    }

    if (setores == nullptr) {
        inicializarSupermercado(setores);
        inicializarArmazem(armazem, setores);
        corrigirInicializacao(setores);
    }

    mostrarSuper(setores, armazem);

    return 0;
}