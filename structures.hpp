#include <climits>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

using namespace std;

#define uint unsigned long long
#define element long long

int MAX_PAGE_SIZE = 128;  // cantidad máxima de elementos en una página

/* Función de hashing: devuelve valor aleatorio entre 0 y 2^64 - 1 para cualquier y */
uint h(element y) {
    mt19937 gen(y);
    uniform_int_distribution<uint> dis(0, UINT64_MAX);
    return dis(gen);
};

/* Página: Lista de elementos que tiene como máximo 1024 bytes */
struct Page {
    vector<element> elements;   // lista de elementos de 64 bits
    int n;                      // cantidad de elementos en la página
    unique_ptr<Page> overflow;  // puntero a la página de desborde

    Page() : n(0), overflow(nullptr) {};  // inicializar una página vacía

    // insertar un elemento en la página actual
    void insert(const element& y) {
        if (n < MAX_PAGE_SIZE) {
            elements.push_back(y);
            n++;
        } else if (!overflow) {
            overflow = make_unique<Page>();
            overflow->insert(y);
        } else {
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

        if (overflow) {
            cout << "Desbordes: " << endl;
            overflow->display_page();  // mostrar la página de desborde
        }
    }
};

/* Tabla de hashing: Lista indexada por el valor entregado por la función de hash */
struct HashTable {
    vector<unique_ptr<Page>> table;  // vector de listas de páginas (donde las listas de rebalse son páginas con un puntero a su rebalse)

    HashTable() {
        table.resize(1);  // inicializar tabla de hashing con páginas vacías
    }

    // mostrar contenido de la tabla de hashing
    void display_table() const {
        cout << "Contenido de la tabla de hash: " << endl;
        for (size_t i = 0; i < table.size(); ++i) {
            if (table[i]) {
                cout << "Página " << i << ":" << endl;
                table[i]->display_page();  // Mostrar el contenido de la página
            } else {
                cout << "Página " << i << ": vacía" << endl;
            }
        }
    }

    double porcentaje_llenado() const {
        int total = 0;
        int ocupado = 0;
        for (auto& page : table) {
            if (page) {
                total++;
                ocupado += page->n;
                auto* overflow_page = page->overflow.get();
                while (overflow_page) {
                    total++;
                    ocupado += overflow_page->n;
                    overflow_page = overflow_page->overflow.get();
                }
            }
        }
        if (total == 0) return 0.0;
        return static_cast<double>(ocupado) / (total * MAX_PAGE_SIZE);
    }
};