#include <iostream>
#include "Laboratorio01.h"

using namespace std;

int main() {
    ArbolDigital<string> b;

    b.insertar("o", "45");
    b.insertar("ave", "15");
    b.insertar("leon","123h");
    b.insertar("vaca","12h3h");
    b.insertar("alce","12h3ddh");
    b.insertar("a","12h3ddhrgg");
    b.insertar("oso","25a");


    cout << b.obtenerValor("alce") << endl;

    return 0;
}
