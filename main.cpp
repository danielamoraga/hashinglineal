#include <chrono>
#include <fstream>  // For file I/O

#include "insertion.cpp"

// Función para generar un número aleatorio de 64 bits
element numero_aleatorio_64bits() {
    return (((element)rand() << 32) | rand());  // Combina dos llamadas de rand() para generar 64 bits
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
    ofstream csv_file("experiment_data.csv");
    csv_file << "N,c_max,costo_promedio_ios,tiempo_insercion,porcentaje_llenado\n";  // CSV header

    srand(time(0));
    for (int i = 10; i <= 24; ++i) {
        uint N = 1 << i;
        // generar secuencia de N números de 64 bits |N| pertenece a {2^10, 2^11, 2^12,...,2^24}
        vector<element> secuencia = generar_secuencia(N);
        cout << "Procesando secuencia de tamaño N = 2^" << i << " (" << N << " elementos)" << endl;
        for (int j = 1; j <= 5; j++) {
            c_max = j;  // cantidad máxima de accesos antes de expandir

            // crear tabla de hashing con espacio inicial 1
            HashTable H;

            int inserciones = 0;
            int ios = 0;
            auto start = chrono::high_resolution_clock::now();  // cronómetro

            // insertar cada número en la tabla de hash
            for (element e : secuencia) {
                insertion(e, H);
                inserciones++;
                ios += accesses;
            }

            auto end = chrono::high_resolution_clock::now();  // terminar cronómetro
            chrono::duration<double> tiempo = end - start;

            double costo_promedio = static_cast<double>(ios) / inserciones;
            double llenado = H.porcentaje_llenado();

            cout << "Terminado N = 2^" << i << ", c_max = " << j << endl;

            // Write data to CSV
            csv_file << N << "," << c_max << "," << costo_promedio << "," << tiempo.count() << "," << llenado << "\n";

            // Store data to the .csv file
            csv_file.flush();
        }
    }
    csv_file.close();
    return 0;
}
