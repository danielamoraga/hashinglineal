# Tarea 1: Inserción en Hashing Lineal

# Proyecto de Experimentación - Tarea 1

## Descripción

Este proyecto implementa una tabla de hash con expansión lineal y realiza experimentos variando el valor de `c_max` para observar su impacto en el rendimiento. La tabla de hash se expande cuando se excede el número máximo de accesos a una página (`c_max`). El experimento inserta diferentes cantidades de elementos en la tabla y registra métricas como el costo promedio de I/O, el tiempo de inserción y el porcentaje de llenado de la tabla.

Los resultados del experimento se guardan en un archivo CSV que luego puede ser utilizado para generar gráficos y análisis.

## Archivos Principales

- `main.cpp`: Archivo principal que ejecuta los experimentos, generando secuencias de números aleatorios y realizando inserciones en la tabla de hash. Los resultados se guardan en un archivo CSV.
- `insertion.cpp`: Contiene la lógica de inserción en la tabla de hash, así como la expansión de la tabla cuando se supera `c_max`.
- `structures.hpp`: Define las estructuras de datos, incluyendo la tabla de hash y las páginas.
- `Makefile`: Archivo para automatizar la compilación, ejecución y depuración del programa.

## Requisitos

- Compilador de C++ compatible con el estándar `C++20`.
- `g++` (GCC) o cualquier otro compilador que soporte `C++20`.
- Sistema operativo Linux o cualquier sistema que soporte GNU Make.

## Compilación y Ejecución

El proyecto incluye un `Makefile` para facilitar la compilación y ejecución del programa. A continuación se explican los comandos disponibles:

### Compilar el proyecto

Para compilar el proyecto, usa el siguiente comando:

```bash
make compile
```

### Ejecutar el programa

Este comando compila y ejecuta los experimentos:
```bash
make run
```

## Formato de salida

El programa genera un archivo CSV llamado experiment_data.csv con los resultados de cada experimento. El archivo contiene las siguientes columnas:
- N: Tamaño de la secuencia (número de elementos insertados).
- c_max: Valor de c_max utilizado para esa ejecución.
- costo_promedio_ios: El costo promedio de I/O (accesos a páginas) por inserción.
- tiempo_insercion: Tiempo total que tomó la inserción de todos los elementos en segundos.
- porcentaje_llenado: El porcentaje de llenado de la tabla de hash al finalizar la inserción.

