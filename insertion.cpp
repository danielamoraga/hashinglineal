#include "expansion.hpp"

int p = 1; // cantidad de páginas del archivo
int t = 0;
int accesses = 0;
int max_accesses;

/* Insertar elemento en la tabla de hashing */
void insertion(element y, HashTable& H) {

    int k = h(y) % (1 << (t + 1));; // índice página k
    cout << "Función de hash: " << h(y) << endl;
    cout << "Página: " << k << endl;
    cout << "Intentando insertar: " << y << " en la página: " << k << endl;

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
                    cout << "Elemento ya existe: " << y << endl;
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
        cout << "Página no existe, insertando en página: " << k - (1 << t) << endl;
        if (H.table[k - (1 << t)] == nullptr) {
            H.table[k - (1 << t)] = new Page(); // Inicializar la página si es necesario
        }
        H.table[k - (1 << t)]->insert(y);
    }

    if (accesses > max_accesses) {
        expand(H, p, t); // se expande la siguiente página p - 2^t
    }
}