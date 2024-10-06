#include "structures.hpp"

void expand(HashTable& H, int& p, int& t) {
    // Duplicar el tamaño de la tabla
    H.table.push_back(make_unique<Page>());

    // Evitar la expansión repetida del mismo índice
    if (H.table[p] && H.table[p]->elements.size() == 0) {
        return;
    }

    // Página a expandir
    int i = p - (1 << t);
    unique_ptr<Page>& old_page = H.table[i];
    vector<element> all;

    // Recorre la página y las de desborde
    Page* current = old_page.get();
    while (current != nullptr) {
        for (element y : current->elements) {
            all.push_back(y);
        }
        current = current->overflow.get();
    }

    // Limpiar la página antigua
    old_page->elements.clear();
    old_page->n = 0;

    // Redistribuir los elementos
    for (element y : all) {
        int hash = h(y) % (1 << (t + 1));  // Rehash with expanded table size
        if (hash == p) {
            if (!H.table[p]) {
                H.table[p] = make_unique<Page>();  // Initialize if needed
            }
            std::cout << "Inserting element in expanded page at " << p << std::endl;
            H.table[p]->insert(y);
        } else {
            std::cout << "Inserting element in page at " << i << std::endl;
            H.table[i]->insert(y);
        }
    }

    // Compactar las páginas de desborde
    unique_ptr<Page>& new_page = H.table[i];
    if (new_page != nullptr) {
        while (new_page->overflow != nullptr) {
            Page* next = new_page->overflow.get();
            vector<element> remaining;
            for (element y : next->elements) {
                if (new_page->n + 1 <= MAX_PAGE_SIZE) {
                    new_page->elements.push_back(y);
                    new_page->n++;
                } else {
                    remaining.push_back(y);
                }
            }
            // Ensure overflow cleanup
            new_page->overflow.reset();
            std::cout << "Overflow cleaned up for page at " << i << std::endl;
        }
    }
    std::cout << "Expansion complete for page " << p << std::endl;
}
