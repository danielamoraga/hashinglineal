#include "insertion.cpp"

int main() {

    // inicializar tabla de hashing con una página
    HashTable H;
    max_accesses = 3; // cantidad de accesos permitidos antes de expandir

    // Test 1: Inserciones simples sin desbordes
    cout << "Test 1: Inserciones simples sin desbordes" << endl;
    for (int i = 0; i < 5; ++i) {
        insertion(i, H); // Usar la función insert del archivo insertion.cpp
    }
    H.display_table();

    // Test 2: Inserciones con desbordes
    cout << "\n\nTest 2: Inserciones con desbordes" << endl;
    for (int i = 5; i < 150; ++i) {
        insertion(i, H); // Insertar en la tabla de hash, se debe generar desbordes
    }
    H.display_table();

    // Test 3: Expansión automática de la tabla
    cout << "\n\nTest 3: Expansión automática de la tabla" << endl;
    for (int i = 150; i < 300; ++i) {
        insertion(i, H); // Insertar y expandir automáticamente si es necesario
    }
    H.display_table();

    return 0;
}