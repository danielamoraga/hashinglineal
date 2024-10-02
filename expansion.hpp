#include "structures.hpp"

void expand(HashTable& H, int& p, int& t) {
    // duplicar el tamaño de la tabla a 2^(t + 1)
    H.table.push_back(new Page());

    // se expande la siguiente página p - 2^t
    int i = p - (1 << t); // indice de la página a expandir

    Page* old_page = H.table[i]; // página a expandir
    vector<element> all; // todos los elementos de la página y sus desbordes

    // se recorre la página junto con las de desborde
    Page* current = old_page;
    while(current != nullptr) {
        for (element y : current->elements) {
            all.push_back(y);
        }
        current = current->overflow;
    }

    old_page->elements.clear(); // limpiar la página a expandir
    old_page->n = 0;

    // redistribuir elementos
    for (element y : all) {
        int hash = h(y) % (1 << (t + 1)); // los elementos y se insertan en la página h(y) % (2^(t + 1))
        // se reparten los elementos en las páginas p - 2^t y p (al final del archivo)
        if (hash == p) {
            if (H.table[p] == nullptr) {
                H.table[p] = new Page(); // inicializar la página si es necesario
            }
            H.table[p]->insert(y); // insertar en la nueva página
        } else {
            H.table[i]->insert(y); // insertar en la página p - 2^t
        }
    }

    // se compactan los elementos que quedaron en la página p - 2^t
    // y se eliminan páginas de rebalse innecesarias
    Page* new_page = H.table[i];
    if (new_page != nullptr) {
        while (new_page->overflow != nullptr) {
            Page* next = new_page->overflow;
            vector<element> remaining;
            // intentamos mover los elementos de la página de rebalse a la actual
            for (element y : next->elements) {
                if (new_page->n + 1 <= MAX_PAGE_SIZE) {
                    new_page->elements.push_back(y); // mover elemento a la pág actual
                    new_page->n++; // actualizar contador
                    next->n--; // actualizar contador
                } else {
                    remaining.push_back(y); // si no cabe, se deja en la pág de rebalse
                }
            }
            if (remaining.empty()) {
                // si la pág de rebalse queda vacía la eliminamos
                new_page->overflow = next->overflow;
                delete next;
                if (new_page->overflow == nullptr) break;
            } else {
                next->elements = remaining; // actualizamos los elementos de la pág de rebalse
                break;
            }
            new_page = new_page->overflow;
        }
    }
    p++;
    if (p == (1 << (t + 1))) t++;
}