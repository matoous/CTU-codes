
#include "DataGenerator.h"


void DataGenerator::generateData(vector<long> &solution, vector<vector<int8_t>> &data) const {

    // Nahodny generator je inicializovan pomoci aktualniho casu
    srand(time(nullptr));

    if (data.size() != solution.size()) {
        throw invalid_argument("Solution vector and count of vectors in data lengths differ.");
    }

    const unsigned long countOfVectors = data.size();

    // Vygenerujeme data
    for (unsigned long i = 0; i < countOfVectors; i++) {
        long sum = 0, number;
        for (int8_t &j : data[i]) {
            number = rand() % 128;
            j = number;
            sum += number;
        }
        solution[i] = sum;
    }
}

// Prevzato z https://stackoverflow.com/questions/38244877/how-to-use-stdnormal-distribution
void DataGenerator::generateDistribution(vector<int> &vector, const int mean, const int sigma, const int beginIndex) const {

    // random device class instance, source of 'true' randomness for initializing random seed
    std::random_device rd;

    // Mersenne twister PRNG, initialized with seed from previous random device instance
    std::mt19937 gen(rd());

    for (unsigned long i = beginIndex; i < vector.size(); ++i) {

        // instance of class std::normal_distribution with specific mean and stddev
        std::normal_distribution<float> d(mean, sigma);

        // get random number with normal distribution using gen as random source
        vector[i] = max(static_cast<int>(d(gen)), 1);
    }

}