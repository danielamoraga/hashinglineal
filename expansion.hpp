#include "structures.hpp"

void expand(HashTable& H, int& p, int& t) {
    // Duplicar el tamaño de la tabla a 2^(t + 1)
    H.table.push_back(new Page()); // Redimensionar la tabla correctamente

    // Se expande la siguiente página p - 2^t
    int i = p - (1 << t); // Índice de la página a expandir

    Page* old_page = H.table[i]; // Página a expandir

    vector<element> all; // Todos los elementos de la página y sus desbordes

    // Recolectar todos los elementos de la página y sus desbordes
    Page* current = old_page;
    while (current != nullptr) {
        for (element y : current->elements) {
            all.push_back(y);
        }
        current = current->overflow;
    }

    old_page->elements.clear(); // Limpiar la página a expandir
    old_page->n = 0;

    // Redistribuir los elementos
    for (element y : all) {
        int hash = h(y) % (1 << (t + 1)); // Calcular el nuevo hash con el tamaño duplicado
        if (hash == p) {
            if (H.table[p] == nullptr) {
                H.table[p] = new Page(); // Inicializar la nueva página si es necesario
            }
            H.table[p]->insert(y); // Insertar en la nueva página p
        } else {
            H.table[i]->insert(y); // Insertar en la página original (p - 2^t)
        }
    }

    // Compactar las páginas de desbordes
    Page* new_page = H.table[i]; // Página original
    while (new_page != nullptr && new_page->overflow != nullptr) {
        Page* next = new_page->overflow;
        vector<element> remaining;

        // Mover elementos de overflow a la página actual
        for (element y : next->elements) {
            if (new_page->n + 1 <= MAX_PAGE_SIZE) {
                new_page->elements.push_back(y);
                new_page->n++;
            } else {
                remaining.push_back(y);
            }
        }

        if (remaining.empty()) {
            // Si la página de overflow está vacía, eliminarla
            new_page->overflow = next->overflow;
            delete next;
        } else {
            // Si hay elementos restantes, mantener la página de overflow
            next->elements = remaining;
            next->n = remaining.size();
            break; // Ya no podemos mover más elementos
        }

        new_page = new_page->overflow;
    }

    // Incrementar p y t si es necesario
    p++;
    if (p == (1 << (t + 1))) t++;
}
