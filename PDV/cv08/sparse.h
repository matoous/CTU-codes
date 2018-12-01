#ifndef PDV08_SPARSE_H
#define PDV08_SPARSE_H

#include <vector>
#include <cmath>
#include <iostream>

// Trida zabalujici jednotlive prvky vektoru / radky matice
class entry {
public:
    unsigned int index;   // Pozice nenuloveho prvku v ramci vektoru
    double value;         // Jeho hodnota

    entry(const unsigned int index, const double value) : index(index), value(value) {}

    // Pri porovnavani rovnosti dvou prvku vektoru dovolujeme urcitou nepresnost
    // pri vypoctu floating-point hodnoty.
    bool operator==(const entry & other) const {
        return index == other.index && std::abs(value - other.value) < 1e-6;
    }

    entry(const entry & other) : index(index), value(value) {}
    entry & operator=(const entry & other) {
        index = other.index;
        value = other.value;
    }

    entry(entry && other) : index(index), value(value) {}
    entry & operator=(entry && other) {
        index = other.index;
        value = other.value;
    }
};

// Radek matice
class matrix_row {
private:
    // Sparse operace nad vektory/maticemi funguji dobre proto, ze jsou nenulove
    // prvky vektoru/radku matice ulozeny v serazenem poradi (podle indexu prvku).
    // V nasem pripade je radime vzestupne. Abychom Vam pomohli, kontrolujeme, ze
    // prvky skutecne v tomto poradi vkladate. V promenne 'lastIdx' je index po-
    // sledniho jeste nevlozeneho prvku.
    unsigned int lastIdx = 0;

    // Vlastni seznam jednotlivych nenulovych prvku. K tomuto vektoru nemate primo
    // pristup. Pro ziskani seznamu nenulovych prvku pouzijte funkci 'entries(...)'.
    std::vector<entry> data;

public:

    // Index udavajici o kolikaty radek matice se jedna.
    unsigned int index;

    matrix_row(const unsigned int index) : index(index) {}

    // Metodu 'set(...)' pouzivejte pro nastavovani nenulovych prvku vektoru.
    // POZOR! Prvky skutecne musite vkladat ve vzestupnem poradi, nebo dojde
    // k vyhozeni vyjimky!
    void set(unsigned int idx, double value) {
        if(idx < lastIdx) throw "Adding elements in wrong order!";
        else {
            lastIdx = idx + 1;
            data.emplace_back(idx, value);
        }
    }

    // Metoda pro ziskani seznamu nenulovych prvku radku matice/vektoru.
    const std::vector<entry> & entries() const {
        return data;
    }

    bool operator==(const matrix_row & other) const {
        return data == other.data;
    }

    void reserve(unsigned int size) {
        data.reserve(size);
    }

    matrix_row(const matrix_row & other) : index(other.index),
                                           lastIdx(other.lastIdx),
                                           data(other.data) {}
    matrix_row(matrix_row && other) : index(other.index),
                                      lastIdx(other.lastIdx),
                                      data(std::move(other.data)) {}

    matrix_row & operator=(const matrix_row & other) {
        index = other.index;
        lastIdx = other.lastIdx;
        data = other.data;

        return *this;
    }

    matrix_row & operator=(matrix_row && other) {
        index = other.index;
        lastIdx = other.lastIdx;
        data = std::move(other.data);

        return *this;
    }
};


// Reprezentace ridke matice je jednoducha - jedna se o vektor radku ('matrix_row')
using sparse_matrix = std::vector<matrix_row>;


// Sparse reprezentaci pouzivame i pro reprezentaci vektoru. Technicky je vektor
// uplne stejny jako radek matice - proto od tridy 'matrix_row' dedime. Jedinou
// zmenu je to, ze nepotrebujeme informaci, reprezentujici o kolikaty radek matice
// se jedna. Proto clenskou promennou 'index' muzeme nastavit na libovolnou hodnotu
// (v nasem pripade -1).
class sparse_vector : public matrix_row {
public:
    sparse_vector() : matrix_row(-1) {}
};


#endif //PDV08_SPARSE_H
