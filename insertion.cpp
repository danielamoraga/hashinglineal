#include "expansion.hpp"

int p = 1;  // cantidad de páginas del archivo
int t = 0;
int accesses = 0;
int c_max;

/* Insertar elemento en la tabla de hashing */
void insertion(element y, HashTable& H) {
    int k = h(y) % (1 << (t + 1));  // Calculate page index
    accesses = 1;

    Page* page = nullptr;

    // Access the correct page
    if (k < p) {
        page = H.table[k].get();
        if (page == nullptr) {
            H.table[k] = make_unique<Page>();
            page = H.table[k].get();
            if (page == nullptr || page->elements.capacity() == 0) {
                return;
            }
        }
    } else {
        page = H.table[k - (1 << t)].get();
        if (page == nullptr) {
            H.table[k - (1 << t)] = make_unique<Page>();
            page = H.table[k - (1 << t)].get();
            if (page == nullptr || page->elements.capacity() == 0) {
                return;
            }
        }
    }

    // Ensure the page has valid capacity
    if (page->elements.capacity() == 0) {
        cerr << "ERROR: Page has invalid or zero capacity!" << endl;
        return;
    }

    // Insert element into the correct page
    if (k < p) {
        H.table[k]->insert(y);
    } else {
        H.table[k - (1 << t)]->insert(y);
    }

    // Trigger expansion if needed
    if (accesses > c_max) {
        cout << "Expanding hash table..." << endl;
        expand(H, p, t);
    }
}
