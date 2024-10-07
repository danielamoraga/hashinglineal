#include <iostream>
#include <random>
#include <vector>
#include <climits>

using namespace std;

#define uint unsigned long long
#define element long long

int MAX_PAGE_SIZE = 1; // cantidad máxima de elementos en una página

/* Función de hashing: devuelve valor aleatorio entre 0 y 2^64 - 1 para cualquier y */
uint h(element y) {
    mt19937 gen(y);
    uniform_int_distribution<uint> dis(0, UINT64_MAX);
    return dis(gen);
};

/* Página: Lista de elementos que tiene como máximo 1024 bytes */
struct Page {
    vector<element> elements; // lista de elementos de 64 bits
    int n = 0; // cantidad de elementos en la página
    Page *overflow = nullptr; // puntero a la página de desborde
    int size = 0; // cantidad de elementos en la página y sus desbordes
    int accesses = 0;

    Page* search(element y) {
        Page* page = this;
        Page* prev = nullptr;
        while (page != nullptr) {
            //cout << "Buscando elemento en la página" << endl;
            accesses++;
            for (element e : elements) {
                if (e == y) {
                    cout << "El elemento ya está en la página" << endl;
                    return nullptr;
                }
            }
            prev = page;
            page = page->overflow;
            //if (page != nullptr) cout << "Buscando en desborde" << endl;
            //else cout << "No hay más desbordes donde buscar" << endl;
        }
        return prev;
    }

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
            //cout << "Insertando elemento en desborde" << endl;
            overflow->insert(y);
        }
    }

    vector<element> get_elements() {
        vector<element> all_elements = elements;
        if (overflow != nullptr) {
            vector<element> overflow_elements = overflow->get_elements();
            all_elements.insert(all_elements.end(), overflow_elements.begin(), overflow_elements.end());
        }
        return all_elements;
    }

    void compactar() {
        vector<element> all_elements = get_elements();
        elements = {};
        overflow = nullptr;
        for (element y : all_elements) {
            insert(y);
        }
    }
};

/* Tabla de hashing: Lista indexada por el valor entregado por la función de hash */
struct HashTable {
    vector<Page*> table = {new Page()}; // vector de listas de páginas (donde las listas de rebalse son páginas con un puntero a su rebalse)
    int p = 1;
    int t = 0;
    int c_max;
    int accesses = 0;

    void insert(element y) {

        int k = h(y) % (1 << (t + 1));
        //cout << "Hash del elemento: " << h(y) << endl;

        if (k < p) {
            //cout << "Insertando en la página " << h(y) << " mod 2^(" << t << " + 1) = " << k << endl;
            Page* searched = table[k]->search(y);
            if (searched == nullptr) {
                //cout << "Elemento ya existe" << endl;
                return;
            } else {
                //cout << "Elemento no encontrado. Se inserta." << endl;
                searched->insert(y);
            }
        } else {
            k = k - (1 << t);
            //cout << "Insertando en la página " << h(y) << " mod 2^(" << t << " + 1) - 2^" << t << " = " << k << endl;
            Page* searched = table[k]->search(y);
            if (searched == nullptr) {
                //cout << "Elemento ya existe" << endl;
                return;
            } else {
                //cout << "Elemento no encontrado. Se inserta." << endl;
                searched->insert(y);
            }
        }

        accesses = table[k]->accesses;
        if (accesses > c_max) {
            //cout << "Se excedió el límite de accesos: accesos = " << accesses  << " > " << c_max << endl;
            expand();
        }
    }

    void expand() {
        //cout << "Expandiendo la tabla" << endl;

        //cout << "Antiguo tamaño: ¿" << table.size() << " = " << p << "?" << endl;

        table.push_back(new Page()); // página p

        //cout << "Nuevo tamaño: ¿" << table.size() << " = " << p + 1 << "?" << endl;   

        int i = p - (1 << t);

        //cout << "Índice de la página a redistribuir: " << i << endl;

        //cout << "Página " << i << " tiene " << table[i]->get_elements().size() << " elementos" << endl;
        
        vector <element> redistribute = table[i]->get_elements();
        table[i] = new Page();

        //cout << "Página " << i << " vaciada, ahora tiene " << table[i]->get_elements().size() << " elementos" << endl;
        //cout << "Elementos a redistribuir: " << redistribute.size() << endl;

        for (element y : redistribute) {
            
            //cout << "Redistribuyendo elemento " << y << endl;
            
            int k = h(y) % (1 << (t + 1));

            if (k == p) {
                //cout << "Insertando en la página " << p << endl;
                table[p]->insert(y);
            } else {
                //cout << "Insertando en la página " << i << endl;
                table[i]->insert(y);
            }
        }

        // compactar páginas de desborde

        p++;
        if (p == (1 << (t + 1))) t++;

        //cout << "Tabla expandida. Nuevo tamaño: " << p << endl;
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