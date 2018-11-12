#ifndef PJC_LIST_HPP
#define PJC_LIST_HPP

#include <cstddef>
#include <vector>
#include <iterator>

using std::size_t;

namespace pjc {

class list {
private:
    struct node {
        double val = 0;
        node* prev = nullptr;
        node* next = nullptr;
    };

    node* head = nullptr;
    node* tail = nullptr;
    size_t num_elements = 0;

public:

    class const_iterator {
        node* current_ptr = nullptr;
        const list* o_list = nullptr;
    public:
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = const double;
        using reference = const double&;
        using pointer = const double*;

        const_iterator() = default;
        const_iterator(node* ptr, const list* gen);

        const_iterator& operator++();
        const_iterator operator++(int);
        const_iterator& operator--();
        const_iterator operator--(int);

        reference operator*() const;
        pointer operator->() const;

        bool operator==(const const_iterator& rhs) const;
        bool operator!=(const const_iterator& rhs) const;

        friend class list;
    };

    class iterator {
        node* current_ptr = nullptr;
        const list* o_list = nullptr;
    public:
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = double;
        using reference = double&;
        using pointer = double*;

        iterator() = default;
        iterator(node* ptr, const list* gen);

        iterator& operator++();
        iterator operator++(int);
        iterator& operator--();
        iterator operator--(int);

        reference operator*() const;
        pointer operator->() const;

        operator const_iterator() const;

        bool operator==(const iterator& rhs) const;
        bool operator!=(const iterator& rhs) const;

        friend class list;
    };

    list() = default;
    list(const list& rhs);
    list& operator=(const list& rhs);
    list(list&& rhs);
    list& operator=(list&& rhs);
    ~list();

    /**
     * Vytvoří list obsahující všechny prvky z vec
     */
    list(const std::vector<double>& vec);

    /**
     * Přidá elem na konec listu
     *
     * Složitost: O(1)
     */
    void push_back(double elem);

    /**
     * Vyřadí poslední prvek z listu
     *
     * Předpoklad: List obsahuje alespoň jeden prvek
     * Složitost: O(1)
     */
    void pop_back();

    /**
     * Vrací referenci k poslednímu prvku v listu
     *
     * Předpoklad: List obsahuje alespoň jeden prvek
     * Složitost: O(1)
     */
    double& back();
    /**
     * Vrací referenci k poslednímu prvku v listu
     *
     * Předpoklad: List obsahuje alespoň jeden prvek
     * Složitost: O(1)
     */
    double const& back() const;

    /**
     * Přidá elem na začátek listu
     *
     * Složitost: O(1)
     */
    void push_front(double elem);

    /**
     * Vyřadí první prvek z listu
     *
     * Předpoklad: List obsahuje alespoň jeden prvek
     * Složitost: O(1)
     */
    void pop_front();

    /**
     * Vrací referenci k prvnímu prvku v listu
     *
     * Předpoklad: List obsahuje alespoň jeden prvek
     * Složitost: O(1)
     */
    double& front();
    /**
     * Vrací referenci k prvnímu prvku v listu
     *
     * Předpoklad: List obsahuje alespoň jeden prvek
     * Složitost: O(1)
     */
    double const& front() const;

    /**
     * Obrátí pořadí všech obsažených prvků
     *
     * To znamená, že pokud list obsahoval prvky [1, 2, 3, 4, 2, 1]
     * v tomto pořadí, pak po zavolání list::reverse() obsahuje list
     * prvky [1, 2, 4, 3, 2, 1].
     *
     * Složitost: O(list.size())
     */
    void reverse();

    /**
     * Vrací velikost listu
     *
     * Složitost: O(1)
     */
    size_t size() const;

    /**
     * Vrací `true` pokud v listu nejsou žádné prvky
     *
     * Složitost: O(1)
     */
    bool empty() const;

    /**
     * Vymaže všechny prvky listu s hodnotou rovnou value
     *
     * Složitost: O(list.size())
     */
    void remove(double value);

    /**
     * Prohodí všechny prvky mezi tímto a rhs listem
     *
     * Složitost: O(1)
     */
    void swap(list& rhs);

    // Základní metody pro vytvoření iterátorů.
    // reverse_iterator jsou záměrně přeskočeny

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    /**
     * Vrací `true` pokud je rhs list roven tomuto
     *
     * Dva listy si jsou rovny pokud obsahují stejné prvky ve stejném pořadí
     *
     * Složitost: O(this.size() + rhs.size())
     */
    bool operator==(const list& rhs) const;
    /**
     * Vrací `true` pokud je tento list menší než rhs
     *
     * Listy jsou porovnávány lexikograficky, to znamená, že [1, 2, 3] < [3]
     * Složitost: O(this.size() + rhs.size())
     */
    bool operator<(const list& rhs) const;

    /**
     * Destruktivně rozdělí list do dvou částí.
     *
     * První list obsahuje prvky které byly v intervalu [begin, place),
     * druhý list obsahuje prvky které byly v intervalu [place, end).
     *
     * Po skončení této metody původní list neobsahuje žádné prvky.
     *
     * Složitost: lineární ve vzdálenosti place od begin().
     */
    std::pair<list, list> split(const_iterator place);


    /**
     * Zařadí prvky z listu rhs do tohoto
     *
     * Předpoklad: Tento list i rhs list jsou seřazené
     * Složitost: O(lhs.size() + rhs.size())
     */
    void merge(list& rhs);

    /**
     * Seřadí prvky obsažené v listu.
     *
     * Prvky musí být seřazené vzestupně a řazení by mělo být stabilní
     * (ale v tomto úkolu to není kontrolováno). Obvykle se používá merge
     * sort.
     *
     * Složitost: O(n log n), kde n == list.size()
     */
    void sort();
};

/**
 * Vrací `true` pokud je lhs list různý od rhs
 *
 * viz operator==
 */
bool operator!=(const list& lhs, const list& rhs);
/**
 * Vrací `true` pokud je lhs větší než rhs
 *
 * Větší viz operator<
 */
bool operator>(const list& lhs, const list& rhs);
/**
 * Vrací `true` pokud je lhs menší nebo roven rhs
 *
 * Větší viz operator<
 */
bool operator<=(const list& lhs, const list& rhs);
/**
 * Vrací `true` pokud je lhs větší nebo roven rhs
 *
 * Větší viz operator<
 */
bool operator>=(const list& lhs, const list& rhs);


/**
 * ADL customization point pro std::swap.
 * Výsledek `swap(lhs, rhs);` by měl být ekvivalentní s výsledkem
 * `lhs.swap(rhs);`
 */
void swap(list& lhs, list& rhs);

}
#endif
