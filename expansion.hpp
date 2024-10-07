#include "structures.hpp"

void expand(HashTable& H, int& p, int& t) {
    // Expand the hash table by adding a new page
    H.table.push_back(make_unique<Page>());

    // Página a expandir
    int i = p - (1 << t);
    unique_ptr<Page>& old_page = H.table[i];
    vector<element> all;

    // Gather all elements from the old page and its overflow
    Page* current = old_page.get();
    while (current != nullptr) {
        for (element y : current->elements) {
            all.push_back(y);
        }
        current = current->overflow.get();
    }

    // Clear the old page
    old_page->elements.clear();
    old_page->n = 0;

    // Redistribute the elements between the old and new pages
    for (element y : all) {
        int hash = h(y) % (1 << (t + 1));  // Rehash with expanded table size
        if (hash == p) {
            if (!H.table[p]) {
                H.table[p] = make_unique<Page>();  // Initialize if needed
                cout << "New page at index " << p << " initialized." << endl;
            }
            H.table[p]->insert(y);
        } else {
            H.table[i]->insert(y);
        }
    }

    // Handle overflow and increment p
    p++;
    if (p == (1 << (t + 1))) {
        t++;  // Increase table size
    }
}
