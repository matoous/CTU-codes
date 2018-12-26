#pragma once

#include <iterator>
#include <type_traits>
#include <functional>
#include <utility>
#include <vector>
#include <algorithm>

template <typename T, typename Comparator = std::less<T>>
class flat_set {
public:
    using iterator = typename std::vector<T>::iterator;
    using const_iterator = typename std::vector<T>::const_iterator;
    using size_type = typename std::vector<T>::size_type;
    using value_type = T;
    using key_compare = Compare;

public:
    class value_compare {
        friend class flat_set;

    protected:
        value_compare(Comparator c) : comp(c) {
        }

    public:
        bool operator()(const value_type &lhs, const value_type &rhs) const {
            return comp(lhs, rhs);
        }

    protected:
        Comparator comp;
    };

private:
    std::vector<T> data;
    value_compare comp_;

public:
    // Speciální member funkce se chovají běžným stylem
    flat_set() : flat_set(Comparator()){};

    flat_set(Comparator const& cmp) : comp_(cmp) {};

    flat_set(flat_set const& rhs);
    flat_set(flat_set && rhs);
    flat_set& operator=(flat_set const& rhs);
    flat_set& operator=(flat_set && rhs);
    ~flat_set() = default;

    // Konstruktory co vytvoří flat_set z prvků definovaných jako
    // [first, last). Alokují pouze jednou pokud je to možné.
    template <typename InputIterator>
    flat_set(InputIterator first, InputIterator last) {
        insert(first, last);
    }

    template <typename InputIterator>
    flat_set(InputIterator first, InputIterator last, Comparator const& cmp);


    // Vloží prvek do setu, buďto pomocí kopie, nebo pomocí přesunu.
    std::pair<iterator, bool> insert(T const& v) {
        auto x = lower_bound(v);
        if(x != data.end() && !cmp(*x, v)) return std::pair <iterator, bool > (data.end(), false);
        return std::pair <iterator, bool > (data.insert(x, v), true);
    }

    std::pair<iterator, bool> insert(T&& v) {
        auto x = lower_bound(v);
        if(x != data.end() && !cmp(*x, v)) return std::pair <iterator, bool > (data.end(), false);
        return std::pair <iterator, bool > (data.insert(x, v), true);
    }

    // Vloží prvky z [first, last), alokuje pouze jednou pokud je to možné
    template <typename InputIterator>
    void insert(InputIterator first, InputIterator last) {
        reserve(size() + std::distance(first, last));
        while(first != last){
            insert(*first);
            first++;
        }
    }

    // Smaže prvek na který ukazuje i, vrátí iterátor k dalšímu prvku
    iterator erase(const_iterator i) {
        return data.erase(i);
    }

    // Smaže všechny prvky z [from, to), vrátí iterátor k dalšímu prvku
    iterator erase(const_iterator from, const_iterator to) {
        return data.erase(from, to);
    }
    // Iterátory předané dovnitř erase odkazují dovnitř setu.

    // Smaže prvek rovný klíči pokud existuje.
    // Vrátí kolik prvků bylo smazáno
    size_type erase(value_type const& key) {
        return data.erase(std::remove(data.begin(), data.end(), key), data.end()) == data.end() ? 0 : 1;
    }

    // Běžné funkce k vytvoření iterátorů
    iterator begin() noexcept {
        return data.begin();
    }

    iterator end() noexcept {
        return data.end();
    }

    const_iterator begin() const noexcept {
        return data.begin();
    }

    const_iterator end() const noexcept {
        return data.end();
    }
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    bool empty() const {
        return size() == 0;
    }

    size_type size() const {
        return data.size();
    }

    size_type capacity() const {
        return data.capacity();
    }

    // Zajistí, aby se do instance flat_setu dalo vložit c prvků
    // aniž by byla potřeba realokace
    void reserve(size_type c) {
        data.reserve(c);
    }

    // Vymaže všechny prvky ze setu
    void clear() {
        data.clear();
    }

    iterator binary_find(iterator begin, iterator end, const T& val){
        // Finds the lower bound in at most log(last - first) + 1 comparisons
        iterator i = std::lower_bound(begin, end, val);

        if (i != end && !(val < *i))
            return i; // found
        else
            return end; // not found
    }

    // Vrátí iterátor ukazující na prvek ekvivalentní s v, nebo end(),
    // pokud takový prvek není uvnitř setu
    iterator find(T const& v) {
        return binary_find(data.begin(), data.end(), v);
    }

    const_iterator binary_find(const_iterator begin, const_iterator end, const T& val){
        // Finds the lower bound in at most log(last - first) + 1 comparisons
        iterator i = std::lower_bound(begin, end, val);

        if (i != end && !(val < *i))
            return i; // found
        else
            return end; // not found
    }

    const_iterator find(T const& v) const {
        return binary_find(data.begin(), data.end(), v, cmp);
    };

    // Vrátí iterátor k prvnímu prvku, který není menší nežli t,
    // nebo end() pokud takový prvek neexistuje.
    iterator lower_bound(T const& t) {
        return std::lower_bound(data.begin(), data.end(), t);
    }

    const_iterator lower_bound(T const& t) const {
        return std::lower_bound(data.begin(), data.end(), t);
    }

    // Vrátí iterátor k prvnímu prvku, který je větší nežli t,
    // nebo end() pokud takový prvek neexistuje.
    iterator upper_bound(T const& t) {
        return std::upper_bound(data.begin(), data.end(), t);
    }
    const_iterator upper_bound(T const& t) const {
        return std::upper_bound(data.begin(), data.end(), t);
    };

    // Prohodí obsah dvou setů (včetně komparátoru)
    void swap(flat_set& o){
        std::swap(data, o.data);
    };
};

// porovnání probíhá lexikograficky
template <typename T>
bool operator==(flat_set<T> const& a, flat_set<T> const& b);

template <typename T>
bool operator!=(flat_set<T> const& a, flat_set<T> const& b);

template <typename T>
bool operator< (flat_set<T> const& a, flat_set<T> const& b);

template <typename T>
bool operator<=(flat_set<T> const& a, flat_set<T> const& b);

template <typename T>
bool operator>=(flat_set<T> const& a, flat_set<T> const& b);

template <typename T>
bool operator> (flat_set<T> const& a, flat_set<T> const& b);

// Prohodí obsah dvou setů (včetně komparátoru)
template <typename T>
void swap(flat_set<T> const& a, flat_set<T> const& b);