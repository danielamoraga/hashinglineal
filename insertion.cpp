#include "expansion.hpp"

int p = 1; // cantidad de páginas del archivo
int t = 0;
int accesses = 0;

/* Insertar elemento en la tabla de hashing */
void insertion(element y, HashTable& H, int c_max) {

    int k = h(y) % (1 << (t + 1));; // índice página k

    if (k < p) {
        // insertar en la página k
        accesses = 1;
        Page* page = H.table[k];
        if (page == nullptr) {
            H.table[k] = new Page(); // Inicializar la página si es necesario
            page = H.table[k];
        }

        while(page != nullptr) {
            for (element e : page->elements) {
                if (e == y) {
                    return; // elemento ya existe en la página, no se inserta
                }
            }
            page = page->overflow;
            if (page != nullptr) accesses++; // acceso a página de desborde
        }
        // si no estaba, salimos del ciclo y podemos insertar
        H.table[k]->insert(y); // insertar en la última página de la cadena de desbordes
        // o en una nueva página si la actual se rebalsa (eso lo hace insert de Page)
    } else { // k >= p, significa que la página k aún no ha sido creada
        // se inserta en la página k - 2^t
        if (H.table[k - (1 << t)] == nullptr) {
            H.table[k - (1 << t)] = new Page(); // Inicializar la página si es necesario
        }
        H.table[k - (1 << t)]->insert(y);
    }

    if (accesses > c_max) {
        expand(H, p, t); // se expande la siguiente página p - 2^t
    }
}