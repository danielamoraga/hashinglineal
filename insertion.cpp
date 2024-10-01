#include "structures.hpp"
#include "expansion.hpp"

int p = 1; // cantidad de páginas del archivo
int t = 0;
int accesses = 0;

/* Insertar elemento en la tabla de hashing */
void insert(element y, HashTable& H, int max_accesses) {

    int k = h(y) % (1 << (t + 1));; // índice página k

    if (k < p) {
        // insertar en la página k
        accesses = 1;
        Page* page = H.table[k];
        Page* prev = nullptr;
        while(page != nullptr) {
            for (element e : page->elements) {
                if (e == y) {
                    return; // elemento ya existe en la página, no se inserta
                }
            }
            prev = page;
            page = page->overflow;
            if (page != nullptr) accesses++; // acceso a página de desborde
        }
        // si no estaba, salimos del ciclo y podemos insertar
        prev->insert(y); // insertar en la última página de la cadena de desbordes
        // o en una nueva página si la actual se rebalsa (eso lo hace insert de Page)
    } else { // k >= p, significa que la página k aún no ha sido creada
        // se inserta en la página k - 2^t
        H.table[k - (1 << t)]->insert(y);
    }

    if (accesses > max_accesses) {
        expand(H, p, t); // se expande la siguiente página p - 2^t
    }
}