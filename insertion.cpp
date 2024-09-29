#include "structures.hpp"

int p = 1; // cantidad de páginas del archivo
int t = 0;

/* Búsqueda en tabla de hashing */
bool search(HashTable& H, uint k, element y) {
    Page* page = H.table[k]; // página k
    while (page != nullptr) {
        for (element e : page->elements) {
            if (e == y) {
                return true; // elemento encontrado
            }
        }
        page = page->overflow;
    }
    return false; // elemento no encontrado
}

/* Insertar elemento en la tabla de hashing */
void insert(element y, HashTable& H) {

    int k = h(y) % (2^(t + 1));

    // primero verificar previamente si el elemento ya existe (utilizando función búsqueda)
    if (search(H, k, y)) {
        return; // elemento ya existe
    }

    if (k < p) {
        // insertar en la página k
        if (H.table[k] == nullptr) {
            H.table[k] = new Page();
        }
        H.table[k]->insert(y);
        // o en una nueva página si la actual se rebalsa (eso lo hace insert de Page)
    } else { // k >= p, significa que la página k aún no ha sido creada
        // se inserta en la página k - 2^t
        if (H.table[k - (2^t)] == nullptr) {
            H.table[k - (2^t)] = new Page();
        }
        H.table[k - (2^t)]->insert(y);
    }

    //if (se supera el número máximo de accesos permitido en la búsqueda) {
        // se expande la siguiente página p - 2^t
            // se recorre la página junto con las de desborde
            // los elementos y se insertan en la página h(y) % (2^(t + 1)) -> se reparten los elementos en las páginas p - 2^t y p (al final del archivo)
            // se compactan los elementos que quedaron en la página p - 2^t
            // y se eliminan páginas de rebalse innecesarias
        // p = p + 1
        // si p = 2^(t + 1)
            // t = t + 1
    //}
}