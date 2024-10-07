#include <chrono>
#include <fstream>

#include "structures.cpp"

// Función para generar un número aleatorio de 64 bits
element numero_aleatorio_64bits() {
    return (((element)rand() << 32) | rand());  // Combina dos llamadas de rand() para generar 64 bits
}

// Función para generar una secuencia de 'N' números aleatorios de 64 bits
vector<element> generar_secuencia(uint N) {
    vector<element> secuencia;
    for (uint i = 0; i < N; i++) {
        secuencia.push_back(numero_aleatorio_64bits());
    }
    return secuencia;
}

int main() {
    ofstream csv_file("experiment_data.csv");
    csv_file << "N,c_max,costo_promedio_ios,tiempo_insercion,porcentaje_llenado\n";

    // generar secuencia de N números de 64 bits |N| pertenece a {2^10, 2^11, 2^12,...,2^24}
    srand(time(0));
    for (int i = 10; i <= 24; ++i) {
        cout << "Tests para N = 2^" << i << endl;
        uint N = 1 << i;
        vector<element> secuencia = generar_secuencia(N);

        for (int c = 1; c <= 10; c++) {
            cout << "Test para c = " << c << endl;
            HashTable H;  // crear tabla de hashing con espacio inicial 1

            H.c_max = c;

            int inserciones = 0;
            int ios = 0;
            auto start = chrono::high_resolution_clock::now();  // cronómetro

            // insertar cada número en la tabla de hash
            for (element y : secuencia) {
                H.insert(y);
                inserciones++;
                ios += H.accesses;
            }

            auto end = chrono::high_resolution_clock::now();  // terminar cronómetro
            chrono::duration<double> tiempo = end - start;

            double costo_promedio = static_cast<double>(ios) / inserciones;
            double llenado = H.porcentaje_llenado();

            csv_file << N << "," << c << "," << costo_promedio << "," << tiempo.count() << "," << llenado << "\n";
            csv_file.flush();
        }
    }
    csv_file.close();
    cout << "Tests finalizados y datos guardados." << endl;
    return 0;
}
