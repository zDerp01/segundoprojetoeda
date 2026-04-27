#include <iostream>
#include "mercado.h"

using namespace std;

int main() {
    srand(time(NULL));

    Sector* setores = nullptr;
    Produto* armazem = nullptr;

    inicializarSupermercado(setores);
    inicializarArmazem(armazem, setores);

    mostrarSuper(setores, armazem);

    return 0;
}