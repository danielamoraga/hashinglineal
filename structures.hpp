#include <iostream>
#include <random>
#include <vector>
#include <climits>

using namespace std;

#define element long long
#define uint unsigned long long

int PAGE_SIZE = 1024; // tamaño máximo de una página

/* Función de hashing: devuelve valor aleatorio entre 0 y 2^64 - 1 para cualquier y */
uint h(int y) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<uint> dis(0, UINT64_MAX);
    return dis(gen);
};

/* Página: Lista de elementos que tiene como máximo 1024 bytes */
struct Page {
    vector<int> elements; // lista de elementos
    uint size; // espacio utilizado
    Page *overflow; // puntero a la página de desborde

    Page() : size(0), overflow(nullptr) {}; // inicializar una página vacía

    // insertar un elemento en la página actual
    void insert(element y) {
        // verificar si hay espacio para más elementos
        if (size + sizeof(y) <= PAGE_SIZE) {
            elements.push_back(y);
            size += sizeof(y);
        } else {
            // si no hay espacio, se inserta en la página de desborde
            if (overflow == nullptr) {
                overflow = new Page();
            }
            overflow->insert(y);
        }
    }
};

/* Tabla de hashing: Lista indexada por el valor entregado por la función de hash */
struct HashTable {
    vector<Page*> table; // vector de listas de páginas (donde las listas de rebalse son páginas con un puntero a su rebalse)

    HashTable(int initial_size) {
        table.resize(initial_size, nullptr); // inicializar tabla de hashing con páginas vacías
    }

    // liberar memoria ocupada por las páginas y sus desbordes
    ~HashTable() {
        for (Page *page : table) {
            delete page;
        }
    }
};