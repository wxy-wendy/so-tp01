#ifndef CHM_CPP
#define CHM_CPP

#include <fstream>
#include <iostream>
#include <thread>

#include "HashMapConcurrente.hpp"

using namespace std;

HashMapConcurrente::HashMapConcurrente() {
	for (unsigned int i = 0; i < HashMapConcurrente::cantLetras; i++) {
		tabla[i] = new ListaAtomica<hashMapPair>();
	}
}
unsigned int HashMapConcurrente::hashIndex(string clave) {
	char firstChar = clave[0];

	if (firstChar >= 'a' && firstChar <= 'z') {
		return (unsigned int)(firstChar - 'a');
	}

	if (firstChar >= 'A' && firstChar <= 'Z') {
		return (unsigned int)(firstChar - 'A');
	}

	cerr << "la clave no tiene una letra válida: " << clave << endl;
	return 0;
}


void HashMapConcurrente::incrementar(string clave) {  // writer

	writeSwitch.lock(noReaders);

	unsigned int bucketIndex = hashIndex(clave);

	bucketMutexes[bucketIndex] .lock();
	for (auto &p : *(tabla[bucketIndex])) {
		if (p.first == clave) {
			p.second++;
			bucketMutexes[bucketIndex].unlock();

			writeSwitch.unlock(noReaders);

			return;
		}
	}
	hashMapPair nuevoPar = make_pair(clave, 1);
	tabla[bucketIndex]->insertar(nuevoPar);
	bucketMutexes[bucketIndex].unlock();

	writeSwitch.unlock(noReaders);
}


vector<string> HashMapConcurrente::claves() {
	vector<string> res;
	for (int i = 0; i < cantLetras; i++) bucketMutexes[i].lock();
	for (int i = 0; i < cantLetras; i++) {
		for (auto p : *tabla[i]) { res.push_back(p.first); }
		bucketMutexes[i].unlock();
	}
	return res;
}

unsigned int HashMapConcurrente::valor(string clave) {
	unsigned int bucketIndex = hashIndex(clave);

	ListaAtomica<hashMapPair> *bucket = tabla[bucketIndex];

	if (bucket == nullptr || bucket->estaVacia()) { 
		return 0;  // trap antisegfault
	}

	unsigned int res = 0;

	bucketMutexes[bucketIndex].lock();
	for (auto &p : *(bucket)) {
		if (p.first == clave) {
			res = p.second;
			break;
		}
	}
	bucketMutexes[bucketIndex].unlock();

	return res;
}


float HashMapConcurrente::promedio() {  // reader

	noReaders.lock();

	readSwitch.lock(noWriters); 

	noReaders.unlock();

	float sum = 0.0;
	unsigned int count = 0;
	for (unsigned int index = 0; index < HashMapConcurrente::cantLetras;
	     index++) {
		for (const auto &p : *tabla[index]) {
			sum += p.second;
			count++;
		}
	}

	readSwitch.unlock(noWriters);

	if (count > 0) return sum / count;
	return 0;
}


void HashMapConcurrente::sumarEnBucket(atomic_int &bucketDisponible, float &suma, unsigned int &cantItems) {
	int index;
	while ((index = bucketDisponible++) < cantLetras) {
		for (const auto &p : *tabla[index]) {
			lock_guard<mutex> sumLock(sumMutex);
			suma += p.second;
			cantItems++;
		}
	}

}

float HashMapConcurrente::promedioParalelo(unsigned int cantThreads) {

	noReaders.lock();
	readSwitch.lock(noWriters);
	noReaders.unlock();

	vector<thread> threads;
	atomic_int bucketDisponible(0);
	float suma = 0.0;
	unsigned int cantItems = 0;

	for (unsigned int i = 0; i < cantThreads; i++) {
		threads.emplace_back(&HashMapConcurrente::sumarEnBucket, this, ref(bucketDisponible), ref(suma), ref(cantItems));
	}
	for (auto &thread : threads) { thread.join(); }

	readSwitch.unlock(noWriters);

	if (cantItems > 0) { return suma / cantItems; }
	return 0.0;

}

#endif
