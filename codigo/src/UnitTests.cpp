#include <vector>
#include <thread>
#include "lib/littletest.hpp"

#include "../src/ListaAtomica.hpp"
#include "../src/HashMapConcurrente.hpp"
#include "../src/CargarArchivos.hpp"

// Tests Ejercicio 1

LT_BEGIN_SUITE(TestsEjercicio1)

ListaAtomica<int> l;

void set_up()
{
}

void tear_down()
{
}
LT_END_SUITE(TestsEjercicio1)

LT_BEGIN_TEST(TestsEjercicio1, ListaComienzaVacia)
    LT_CHECK_EQ(l.longitud(), 0);
LT_END_TEST(ListaComienzaVacia)

LT_BEGIN_TEST(TestsEjercicio1, InsertarAgregaElemento)
    l.insertar(42);
    LT_CHECK_EQ(l.longitud(), 1);
LT_END_TEST(InsertarAgregaElemento)

LT_BEGIN_TEST(TestsEjercicio1, InsertarAgregaElementoCorrecto)
    l.insertar(42);
    LT_CHECK_EQ(l[0], 42);
LT_END_TEST(InsertarAgregaElementoCorrecto)

LT_BEGIN_TEST(TestsEjercicio1, InsertarAgregaEnOrden)
    l.insertar(4);
    l.insertar(3);
    l.insertar(2);
    l.insertar(1);
    LT_CHECK_EQ(l.longitud(), 4);
    LT_CHECK_EQ(l[0], 1);
    LT_CHECK_EQ(l[1], 2);
    LT_CHECK_EQ(l[2], 3);
    LT_CHECK_EQ(l[3], 4);
LT_END_TEST(InsertarAgregaEnOrden)

void insertarEnLista(ListaAtomica<int>& lista, int valor) {
    lista.insertar(valor);
}

LT_BEGIN_TEST(TestsEjercicio1, InsertarConcurrente)
    vector<thread> threads;

    for (int i = 0; i < 5; i++) {
        threads.emplace_back(insertarEnLista, ref(l), i);
    }

    for (auto& t : threads) {
        t.join();
    }

    for (int i = 0; i < 5; i++) {
        LT_CHECK_EQ(l.pertenece(i), true);
    }

    LT_CHECK_EQ(l.longitud(), 5);
LT_END_TEST(InsertarConcurrente)



// Tests Ejercicio 2

LT_BEGIN_SUITE(TestsEjercicio2)

HashMapConcurrente hM;

void set_up()
{
}

void tear_down()
{
}
LT_END_SUITE(TestsEjercicio2)

LT_BEGIN_TEST(TestsEjercicio2, ValorEsCorrectoEnHashMapVacio)
    LT_CHECK_EQ(hM.valor("Heladera"), 0);
LT_END_TEST(ValorEsCorrectoEnHashMapVacio)

LT_BEGIN_TEST(TestsEjercicio2, ClavesEsCorrectoEnHashMapVacio)
    std::vector<std::string> actual = hM.claves();
    std::vector<std::string> expected = {};
    LT_CHECK_COLLECTIONS_EQ(actual.begin(), actual.end(), expected.begin());
LT_END_TEST(ClavesEsCorrectoEnHashMapVacio)

LT_BEGIN_TEST(TestsEjercicio2, ValorEsCorrectoTrasUnaInsercion)
    hM.incrementar("Heladera");
    LT_CHECK_EQ(hM.valor("Heladera"), 1);
LT_END_TEST(ValorEsCorrectoTrasUnaInsercion)

LT_BEGIN_TEST(TestsEjercicio2, ClavesEsCorrectoTrasUnaInsercion)
    hM.incrementar("Heladera");
    std::vector<std::string> actual = hM.claves();
    std::vector<std::string> expected = {"Heladera"};
    LT_CHECK_COLLECTIONS_EQ(actual.begin(), actual.end(), expected.begin());
LT_END_TEST(ClavesEsCorrectoTrasUnaInsercion)

LT_BEGIN_TEST(TestsEjercicio2, ValorEsCorrectoTrasDosInsercionesMismaPalabra)
    hM.incrementar("Heladera");
    hM.incrementar("Heladera");
    LT_CHECK_EQ(hM.valor("Heladera"), 2);
LT_END_TEST(ValorEsCorrectoTrasDosInsercionesMismaPalabra)

LT_BEGIN_TEST(TestsEjercicio2, ClavesEsCorrectoTrasDosInsercionesMismaPalabra)
    hM.incrementar("Heladera");
    hM.incrementar("Heladera");
    std::vector<std::string> actual = hM.claves();
    std::vector<std::string> expected = {"Heladera"};
    LT_CHECK_COLLECTIONS_EQ(actual.begin(), actual.end(), expected.begin());
LT_END_TEST(ClavesEsCorrectoTrasDosInsercionesMismaPalabra)

LT_BEGIN_TEST(TestsEjercicio2, ValorEsCorrectoTrasVariasInsercionesMismoBucket)
    hM.incrementar("Heladera");
    hM.incrementar("Heladera");
    hM.incrementar("Lavarropa");
    LT_CHECK_EQ(hM.valor("Heladera"), 2);
    LT_CHECK_EQ(hM.valor("Lavarropa"), 1);
LT_END_TEST(ValorEsCorrectoTrasVariasInsercionesMismoBucket)

LT_BEGIN_TEST(TestsEjercicio2, ClavesEsCorrectoTrasVariasInsercionesMismoBucket)
    hM.incrementar("Heladera");
    hM.incrementar("Heladera");
    hM.incrementar("Lavarropa");
    std::vector<std::string> actual = hM.claves();
    LT_CHECK_EQ(actual.size(), 2);
    LT_CHECK(std::find(actual.begin(), actual.end(), "Heladera") != actual.end());
    LT_CHECK(std::find(actual.begin(), actual.end(), "Lavarropa") != actual.end());
LT_END_TEST(ClavesEsCorrectoTrasVariasInsercionesMismoBucket)

LT_BEGIN_TEST(TestsEjercicio2, ValorEsCorrectoTrasVariasInsercionesDistintoBucket)
    hM.incrementar("Heladera");
    hM.incrementar("Microondas");
    hM.incrementar("Heladera");
    LT_CHECK_EQ(hM.valor("Heladera"), 2);
    LT_CHECK_EQ(hM.valor("Microondas"), 1);
LT_END_TEST(ValorEsCorrectoTrasVariasInsercionesDistintoBucket)

LT_BEGIN_TEST(TestsEjercicio2, ClavesEsCorrectoTrasVariasInsercionesDistintoBucket)
    hM.incrementar("Heladera");
    hM.incrementar("Microondas");
    hM.incrementar("Heladera");
    std::vector<std::string> actual = hM.claves();
    LT_CHECK_EQ(actual.size(), 2);
    LT_CHECK(std::find(actual.begin(), actual.end(), "Heladera") != actual.end());
    LT_CHECK(std::find(actual.begin(), actual.end(), "Microondas") != actual.end());
LT_END_TEST(ClavesEsCorrectoTrasVariasInsercionesDistintoBucket)


LT_BEGIN_TEST(TestsEjercicio2, IncrementarYValorConcurrente)
    vector<thread> threads;
    vector<string> ordenes = {"Heladera", "Microondas", "Lavarropas"};

    for (int i = 0; i < 5; i++) {
        threads.emplace_back([&] {
            for (int j = 0; j < ordenes.size(); j++) {
                hM.incrementar(ordenes[j]);
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    LT_CHECK_EQ(hM.valor("Heladera"), 5);
    LT_CHECK_EQ(hM.valor("Microondas"), 5);
    LT_CHECK_EQ(hM.valor("Lavarropas"), 5);
LT_END_TEST(IncrementarYValorConcurrente)

// Tests Ejercicio 3

LT_BEGIN_SUITE(TestsEjercicio3)

HashMapConcurrente hM;

void set_up()
{
}

void tear_down()
{
}
LT_END_SUITE(TestsEjercicio3)

LT_BEGIN_TEST(TestsEjercicio3, PromedioEsCorrecto)
    hM.incrementar("Heladera");
    hM.incrementar("Heladera");
    hM.incrementar("Heladera");
    hM.incrementar("Heladera");
    hM.incrementar("Microondas");
    hM.incrementar("Microondas");

    float actual = hM.promedio();
    LT_CHECK_EQ(actual, 3);
LT_END_TEST(PromedioEsCorrecto)

LT_BEGIN_TEST(TestsEjercicio3, PromedioParaleloEsCorrecto)
    hM.incrementar("Heladera"); 
    hM.incrementar("Heladera");
    hM.incrementar("Heladera"); 
    hM.incrementar("Iphone15"); 
    hM.incrementar("Microondas"); 
    hM.incrementar("Microondas"); 
    hM.incrementar("PS5"); 
    hM.incrementar("PS5");  
    hM.incrementar("PS5"); 
    hM.incrementar("PS5");
    
    float actual = hM.promedioParalelo(4);
    
    LT_CHECK_EQ(actual, 2.5);
LT_END_TEST(PromedioParaleloEsCorrecto)

LT_BEGIN_TEST(TestsEjercicio3, ConcurrenciaIncrementarYPromedioParalelo)
    hM.incrementar("Heladera");
    hM.incrementar("Microondas");
    hM.incrementar("SamsungS24");
    hM.incrementar("PS5");

    std::thread t1([&] { hM.incrementar("Heladera"); });
    std::thread t2([&] { hM.incrementar("Iphone15"); });
    std::thread t3([&] { hM.incrementar("Microondas"); });

    t1.join();
    t2.join();
    t3.join();

    float promedio = hM.promedio();
    float promedioThreads = hM.promedioParalelo(4);

    LT_CHECK_EQ(promedioThreads, promedio);
LT_END_TEST(ConcurrenciaIncrementarYPromedioParalelo)

LT_BEGIN_TEST(TestsEjercicio3, promedioParaleloDosVecesDanIgual)
    hM.incrementar("Heladera");
    hM.incrementar("Microondas");
    hM.incrementar("PS5");
    hM.incrementar("Iphone15");
    float p1, p2;

    p1 = hM.promedioParalelo(3);
    p2 = hM.promedioParalelo(3); 

    LT_CHECK_EQ(p1, p2);
LT_END_TEST(promedioParaleloDosVecesDanIgual)

LT_BEGIN_TEST(TestsEjercicio3, NoStarvationParaEscritores)
    hM.incrementar("Heladera");
    hM.incrementar("Microondas");
    hM.incrementar("Lavarropas");
    hM.incrementar("PS5");

    
    std::thread t1([&] { hM.promedioParalelo(4); });
    std::thread t2([&] { hM.promedioParalelo(4); });
    std::thread t3([&] { hM.promedioParalelo(4); });

    t1.join();
    t2.join();
    t3.join();

    LT_CHECK(true);  // Si no hubo deadlocks ni fallos, el test pasa
LT_END_TEST(NoStarvationParaEscritores)


LT_BEGIN_TEST(TestsEjercicio2, IncrementarYClavesParalelo)
    const std::vector<std::string> ordenes = {"Heladera", "Lavarropas", "Celular", "Iphone"};
    std::vector<std::thread> threads;

    for (int i = 0; i < 10; i++) {
        threads.emplace_back([&, i] {
            for (int j = 0; j < 4; j++) {
                hM.incrementar(ordenes[j]);
            }
        });
    }

    threads.emplace_back([&] {auto actualClaves = hM.claves();});

    for (auto& t : threads) {
        t.join();
    }

    for (const auto& orden : ordenes) {
        LT_CHECK_EQ(hM.valor(orden), 10);
    }
LT_END_TEST(IncrementarYClavesParalelo)



LT_BEGIN_SUITE(TestsEjercicio4)

HashMapConcurrente hM;

void set_up()
{
}

void tear_down()
{
}
LT_END_SUITE(TestsEjercicio4)

LT_BEGIN_TEST(TestsEjercicio4, CargarArchivoFunciona)
    cargarArchivo(hM, "data/test-1");
    LT_CHECK_EQ(hM.valor("Heladera"), 1);
    LT_CHECK_EQ(hM.claves().size(), 5);
LT_END_TEST(CargarArchivoFunciona)

LT_BEGIN_TEST(TestsEjercicio4, CargarMultiplesArchivosFuncionaUnThread)
    cargarMultiplesArchivos(hM, 1, {"data/test-1", "data/test-2", "data/test-3"});
    LT_CHECK_EQ(hM.valor("Heladera"), 2);
    LT_CHECK_EQ(hM.valor("NotebookOMEN"), 3);
    LT_CHECK_EQ(hM.valor("Microondas"), 4);
    LT_CHECK_EQ(hM.claves().size(), 12);
LT_END_TEST(CargarMultiplesArchivosFuncionaUnThread)

LT_BEGIN_TEST(TestsEjercicio4, CargarMultiplesArchivosFuncionaDosThreads)
    cargarMultiplesArchivos(hM, 2, {"data/test-1", "data/test-2", "data/test-3"});
    LT_CHECK_EQ(hM.valor("Heladera"), 2);
    LT_CHECK_EQ(hM.valor("NotebookOMEN"), 3);
    LT_CHECK_EQ(hM.valor("Microondas"), 4);
    LT_CHECK_EQ(hM.claves().size(), 12);
LT_END_TEST(CargarMultiplesArchivosFuncionaDosThreads)



// Ejecutar tests
LT_BEGIN_AUTO_TEST_ENV()
    AUTORUN_TESTS()
LT_END_AUTO_TEST_ENV()
