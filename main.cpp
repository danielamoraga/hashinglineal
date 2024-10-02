#include "insertion.cpp"
#include <chrono>
#include <fstream>

// Función para generar un número aleatorio de 64 bits
element numero_aleatorio_64bits() {
    return (((element) rand() << 32) | rand());  // Combina dos llamadas de rand() para generar 64 bits
}

// Función para generar una secuencia de 'n' números aleatorios de 64 bits
vector<element> generar_secuencia(int n) {
    vector<element> secuencia;
    for (int i = 0; i < n; ++i) {
        secuencia.push_back(numero_aleatorio_64bits());
    }
    return secuencia;
}

int main() {
    // experimentación
    const double c_max = 0.9;

    // generar secuencia de N números de 64 bits |N| pertenece a {2^10, 2^11, 2^12,...,2^24}
    srand(time(0));
    for (int i = 10; i <= 24; ++i) {
        uint N = 1 << i;
        vector<element> secuencia = generar_secuencia(N);
        cout << "Generando secuencia de tamaño " << N << endl;

        // crear tabla de hashing con espacio inicial 1
        HashTable H;

        int inserciones = 0;
        int ios = 0;
        auto start = chrono::high_resolution_clock::now(); // cronómetro

        // insertar cada número en la tabla de hash
        for (element e : secuencia) {
            insertion(e, H);
            inserciones++;
            ios += accesses;
        }

        auto end = chrono::high_resolution_clock::now(); // terminar cronómetro
        chrono::duration<double> tiempo = end - start;

        double costo_promedio = static_cast<double>(ios) / inserciones;
        double llenado = H.porcentaje_llenado();

        // guardar resultados en archivos
        string filename = "resultados_" + to_string(N) + ".txt";
        ofstream outfile(filename);
        outfile << "Resultados para N = " << N << ":" << endl;
        outfile << "Costo promedio de inserción (I/Os): " << costo_promedio
                << (costo_promedio > c_max ? "* Costo promedio de inserción excede el límite de " + to_string(c_max) : "")
                << endl;
        outfile << "Tiempo total de inserción: " << tiempo.count() << " segundos" << endl;
        outfile << "Porcentaje de llenado de las páginas: " << llenado << "%" << endl;
        outfile.close();
    }

    return 0;
}