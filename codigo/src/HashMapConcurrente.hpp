#ifndef HMC_HPP
#define HMC_HPP

#include <mutex>
#include <array>
#include <atomic>
#include <string>
#include <vector>

#include "ListaAtomica.hpp"

using namespace std;


struct Lightswitch{ 

    int counter = 0;
    mutex mtx;

    void lock(mutex &sem){
        mtx.lock();
        counter++;
        if(counter == 1) sem.lock();
        mtx.unlock();
    }

    void unlock(mutex &sem){
        mtx.lock();
        counter--;
        if(counter == 0) sem.unlock();
        mtx.unlock();
    }
};



typedef std::pair<std::string, unsigned int> hashMapPair;

class HashMapConcurrente {
 public:
    static constexpr int cantLetras = 26;

    HashMapConcurrente();

    void incrementar(std::string clave);
    std::vector<std::string> claves();
    unsigned int valor(std::string clave);

    float promedio();
    void sumarEnBucket(atomic_int &index, float &suma, unsigned int &cantItems);
    float promedioParalelo(unsigned int cantThreads);


 private:
    ListaAtomica<hashMapPair> *tabla[HashMapConcurrente::cantLetras];

    static unsigned int hashIndex(std::string clave);
    
    std::mutex bucketMutexes[HashMapConcurrente::cantLetras];
    std::mutex noReaders;
    std::mutex noWriters;
    Lightswitch readSwitch;
    Lightswitch writeSwitch;

    std::mutex sumMutex;

};

#endif  /* HMC_HPP */
