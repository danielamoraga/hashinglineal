#include "insertion.cpp"

int main() {

    // inicializar tabla de hashing con una página
    HashTable H;
    max_accesses = 3; // cantidad de accesos permitidos antes de expandir

    // insertar elementos en la tabla de hashing
    cout << "Test 1: Inserciones simples sin desbordes" << endl;
    for (int i = 0; i < 5; ++i) {
        insert(i, H);
    }
    H.display_table();

    // Test 2: Inserciones con desbordes
    cout << "\nTest 2: Inserciones con desbordes" << endl;
    for (int i = 5; i < 150; ++i) {
        insert(i, H); // Insertar en la tabla de hash, se debe generar desbordes
    }
    H.display_table();

    // Test 3: Expansión automática de la tabla
    cout << "\nTest 3: Expansión automática de la tabla" << endl;
    for (int i = 150; i < 300; ++i) {
        insert(i, H); // Insertar y expandir automáticamente si es necesario
    }
    H.display_table();

    return 0;
}