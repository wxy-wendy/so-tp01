#ifndef CHM_CPP
#define CHM_CPP

#include <vector>
#include <iostream>
#include <fstream>
#include <thread>

#include "CargarArchivos.hpp"


int cargarArchivo(HashMapConcurrente &hashMap, string filePath) {
    fstream file;
    int cant = 0;
    string palabraActual;

    // Abro el archivo.
    file.open(filePath, file.in);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo '" << filePath << "'" << endl;
        return -1;
    }
    while (file >> palabraActual) {

        hashMap.incrementar(palabraActual);

        cant++;
    }
    // Cierro el archivo.
    if (!file.eof()) {
        cerr << "Error al leer el archivo" << endl;
        file.close();
        return -1;
    }
    file.close();
    return cant;
}


void procesamientoArchivos(HashMapConcurrente &hashMap, vector<string> &filePaths, atomic_int &index, unsigned int cantPaths){
    index++;
    while (index - 1 < cantPaths) {
        cargarArchivo(hashMap, filePaths[index - 1]);
        index++;
    }
}

void cargarMultiplesArchivos(HashMapConcurrente &hashMap, unsigned int cantThreads, vector<string> filePaths) {
    vector<thread> threads;
    atomic_int filePathDisponible(0);
    unsigned int cantPaths = filePaths.size();

    for(unsigned int i = 0; i < cantThreads; i++) {
        threads.emplace_back(procesamientoArchivos, ref(hashMap), ref(filePaths), ref(filePathDisponible), cantPaths);
    }    
    
    for(auto& thread: threads){
        thread.join();
    }

}

#endif
