#include <iostream>
#include <list>
#include <random>
#include "ThreadPool.h"

// Nastaveni poctu konzumentu a producentu
const unsigned int NWORKERS = 2;
const unsigned int NPRODUCERS = 10;

// Nastaveni poctu uloh pro kazdeho producenta a rozsah,
// ze ktereho se budou ulohy generovat
const unsigned int NDATA = 5;
const unsigned int MIN_DATA = 1000;
const unsigned int MAX_DATA = 10000;

// Delka sleepu pro producenty a konzumenty, ovlivnujici
// jak rychle se ulohy vytvari a zpracovavaji
const unsigned int SLEEP_PER_STEP_WORKER = 2;
const unsigned int SLEEP_PER_FASTEST_PRODUCER = 0;

// Povoleni kontrolnich vypisu
const bool VERBOSE = true;
// Mutex pro vypisy na stdout
std::mutex cout_mu;

int main() {
    const auto worker = [](unsigned long long data) {
        unsigned long long steps = 0;
        const unsigned long long origdata = data;
        std::thread::id this_id = std::this_thread::get_id();
        while (data > 1) {
            steps++;
            if (data % 2) data = 3 * data + 1;
            else data /= 2;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_PER_STEP_WORKER * steps));
        if (VERBOSE){
            cout_mu.lock();
            std::cout << "Thread " << this_id << " working on: " << origdata << " " << steps << std::endl;
            cout_mu.unlock();
        }
        return;
    };

    // Inicializace poolu vlaken, ktere zpracovavaji ulohy vytvarene producenty
    ThreadPool<unsigned long long, typeof(worker)> pool(NWORKERS, worker);

    // Vytvareni vlaken producentu, produkujicich ulohy
    std::vector<std::thread> threads;
    for(unsigned int i = 1 ; i < NPRODUCERS+1 ; i++) {
        threads.emplace_back([&pool, i]() {
            // ziskani id vlaknapool
            std::thread::id this_id = std::this_thread::get_id();
            // inicializace generatoru nahodnych cisel
            std::mt19937 RND(i);
            std::uniform_int_distribution<unsigned int> dist(MIN_DATA,MAX_DATA);

            for(unsigned int j = 1 ; j < NDATA ; j++) {
                // vytvorime nahodnou ulohu
                int data = dist(RND);
                int sleepTime = i * SLEEP_PER_FASTEST_PRODUCER;
                if (VERBOSE) {
                    cout_mu.lock();
                    std::cout << "Thread " << this_id << " pushing to queue: " << data << ", sleeping for: "
                              << sleepTime << std::endl;
                    cout_mu.unlock();
                }

                // vkladani ulohy do fronty uloh
                pool.process(data);

                // a pockame chvili, nez zadame dalsi ulohu ke zpracovani
                std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
            }
        });
    }

    // Join pro producenty
    for(unsigned int i = 0 ; i < NPRODUCERS ; i++) threads[i].join();

    // Ukoncovani cinnosti konzumentu
    for(unsigned int i = 0 ; i < NWORKERS ; i++) pool.process(0);

    // Join pro konzumenty
    pool.join();

    return 0;
}