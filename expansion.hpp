#include "structures.hpp"

void expand(HashTable& H, int& p, int& t) {
    // se expande la siguiente página p - 2^t

    int k = p - (1 << t); // indice de la página a expandir
    Page* page = H.table[k]; // página a expandir

    if (page == nullptr) return; // no hay elementos en la página

    H.table.push_back(new Page()); // crear nueva página p en la tabla

    vector<element> all; // todos los elementos de la página y sus desbordes

    // se recorre la página junto con las de desborde
    Page* current = page;
    while(current != nullptr) {
        for (element e : current->elements) {
            all.push_back(e);
        }
        current = current->overflow;
    }

    page->elements.clear(); // limpiar la página a expandir
    page->n = 0;

    // redistribuir elementos
    for (element e : all) {
        int hash = h(e) % (1 << (t + 1)); // los elementos y se insertan en la página h(y) % (2^(t + 1))
        // se reparten los elementos en las páginas p - 2^t y p (al final del archivo)
        if (hash == p) {
            H.table[p]->insert(e); // insertar en la nueva página
        } else {
            H.table[k]->insert(e); // insertar en la página p - 2^t
        }
    }

    // se compactan los elementos que quedaron en la página p - 2^t
    // y se eliminan páginas de rebalse innecesarias
    current = page; // volver a la página a expandir
    while (current->overflow != nullptr) {
        Page* next = current->overflow;
        vector<element> new_elements;
        // intentamos mover los elementos de la página de rebalse a la actual
        for (element e : next->elements) {
            if (current->n + 1 <= MAX_PAGE_SIZE) {
                current->elements.push_back(e); // mover elemento a la pág actual
                current->n++; // actualizar contador
            } else {
                new_elements.push_back(e); // aún hay elementos que no caben en la pág actual
            }
        }
        if (new_elements.empty()) {
            // si la pág de rebalse está vacía la eliminamos
            delete next;
            current->overflow = nullptr;
        } else {
            // si quedan elementos los agregamos a la pág de rebalse
            next->elements = new_elements;
            break;
        }

        current = current->overflow;
    }

    p++;
    if (p == (1 << (t + 1))) t++;
}