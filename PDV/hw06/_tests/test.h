#ifndef CODE_TEST_H
#define CODE_TEST_H

#include <vector>

// Trida SortingTest spusti razeni nad daty a zkontroluje razeni
template<typename element_t>
class SortingTest {
public:
    std::vector<element_t *> sortedData;
    SortingAlgorithm<element_t> sortingAlgorithm;

    // konstruktor
    SortingTest(std::vector<element_t> &data, SortingAlgorithm<element_t> algorithm) {
        sortingAlgorithm = algorithm;

        // nakopiruj data jako pointers - pro rychlejsi razeni
        unsigned long size = data.size();
        for (int i = 0; i < size; ++i) {
            sortedData.push_back(&data[i]);
        }
    }

    // metoda spusti razeni naddaty
    void run_sort() {
        sortingAlgorithm(sortedData);
    }

    // kontrola vysledku
    bool verify(const std::vector<element_t> &data) {
        // kontrola, ze vyseldku jsou serazeny. u odevzdane verze budeme kontrolovat nejenom ze jsou vysledky serazeny,
        // ale ze jsou i spravne
        return std::is_sorted(sortedData.begin(), sortedData.end(), [&](element_t *first, element_t *second) {
            return (*first) < (*second);
        });
    }
};

#endif //CODE_TEST_H
