#include <iostream>
#include <random>
#include <vector>
#include <climits>

using namespace std;

#define uint unsigned long long
#define element long long

int MAX_PAGE_SIZE = 128; // cantidad máxima de elementos en una página

/* Función de hashing: devuelve valor aleatorio entre 0 y 2^64 - 1 para cualquier y */
uint h(element y) {
    mt19937 gen(y);
    uniform_int_distribution<uint> dis(0, UINT64_MAX);
    return dis(gen);
};

/* Página: Lista de elementos que tiene como máximo 1024 bytes */
struct Page {
    vector<element> elements; // lista de elementos de 64 bits
    int n; // cantidad de elementos en la página
    Page *overflow; // puntero a la página de desborde

    Page() : n(0), overflow(nullptr) {}; // inicializar una página vacía

    // insertar un elemento en la página actual
    void insert(element y) {
        // verificar si hay espacio para otro elemento
        if (n + 1 <= MAX_PAGE_SIZE) {
            elements.push_back(y);
            n++;
        } else {
            // si no hay espacio, se inserta en la página de desborde
            if (overflow == nullptr) {
                overflow = new Page();
            }
            overflow->insert(y);
        }
    }

    // mostrar contenido de la página y sus desbordes
    void display_page() const {
        cout << "Elementos en la página: ";
        for (auto& e : elements) {
            cout << e << " ";
        }
        cout << endl;

        if (overflow != nullptr) {
            cout << "Desbordes: " << endl;
            overflow->display_page(); // mostrar la página de desborde
        }
    }
};

/* Tabla de hashing: Lista indexada por el valor entregado por la función de hash */
struct HashTable {
    vector<Page*> table; // vector de listas de páginas (donde las listas de rebalse son páginas con un puntero a su rebalse)

    HashTable() {
        table.resize(1, nullptr); // inicializar tabla de hashing con páginas vacías
    }

    // liberar memoria ocupada por las páginas y sus desbordes
    ~HashTable() {
        for (Page *page : table) {
            delete page;
        }
    }

    // mostrar contenido de la tabla de hashing
    void display_table() const {
        cout << "Contenido de la tabla de hash: " << endl;
        for (size_t i = 0; i < table.size(); ++i) {
            if (table[i] != nullptr) {
                cout << "Página " << i << ":" << endl;
                table[i]->display_page(); // Mostrar el contenido de la página
            } else {
                cout << "Página " << i << ": vacía" << endl;
            }
        }
    }

    double porcentaje_llenado() const {
        int total = 0;
        int ocupado = 0;
        for (Page* page : table) {
            if (page != nullptr) {
                total++;
                ocupado += page->n;
                while (page->overflow != nullptr) {
                    page = page->overflow;
                    total++;
                    ocupado += page->n;
                }
            }
        }
        if (total == 0) return 0.0;
        return (double) ocupado / (total * MAX_PAGE_SIZE);
    }
};