#pragma once

#include <vector>
#include <string>
#include <iterator>
#include <iosfwd>

// Předpokládáme pouze základní ASCII znaky
static const size_t num_chars = 128;

struct trie_node {
    trie_node *children[num_chars] = {}; //!< pole ukazatelů na přímé následovníky daného uzlu
    trie_node *parent = nullptr;         //!< ukazatel na přímého předka daného uzlu
    char payload = 0;                    //!< znak, který tento uzel reprezentuje
    bool is_terminal = false;            //!< označuje, jestli v tomto uzlu končí slovo
};

class trie {
public:
    class const_iterator {
        const trie_node *current_node = nullptr;

        friend std::ostream &operator<<(std::ostream &os, trie::const_iterator const &i);

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::string;
        using reference = std::string;
        using pointer = std::string;
        using difference_type = std::ptrdiff_t;

        const_iterator() = default;

        const_iterator(const trie_node *node);

        const_iterator &operator++();

        const_iterator operator++(int);

        reference operator*() const;

        bool operator==(const const_iterator &rhs) const;

        bool operator!=(const const_iterator &rhs) const;
    };

    /**
     * Vytvoří trii obsahující všechny řetězce z daného vektoru
     */
    trie(const std::vector<std::string> &strings);

    trie();

    trie(const trie &rhs);

    trie &operator=(const trie &rhs);

    trie(trie &&rhs);

    trie &operator=(trie &&rhs);

    ~trie();

    /**
     * Odstraní daný řetězec z trie.
     * Vrátí true, pokud byl řetězec odstraněn (byl předtím v trii), jinak false.
     */
    bool erase(const std::string &str);

    /**
     * Vloží řetězec do trie.
     * Vrátí true, pokud byl řetězec vložen (nebyl předtím v trii), jinak false.
     */
    bool insert(const std::string &str);

    /**
     * Vrátí true, pokud je řetězec v trii, jinak false.
     */
    bool contains(const std::string &str) const;

    /**
     * Vrátí počet unikátních řetězců v trii.
     */
    size_t size() const;

    /**
     * Vrací `true` pokud v listu nejsou žádné prvky.
     */
    bool empty() const;

    /**
     * Vrátí všechny prvky trie, které začínají daným prefixem.
     *
     * Prefixy jsou inkluzivní, tj. search_by_prefix("abc") vrátí mezi výsledky
     * i "abc", pokud je "abc" v trii.
     */
    std::vector<std::string> search_by_prefix(const std::string &prefix) const;

    /**
     * Vrátí všechny řetězce z trie, jež jsou prefixem daného řetězce.
     *
     * Prefixy jsou inkluzivní, tj. get_prefixes("abc") vrátí mezi výsledky
     * i "abc", pokud je "abc" v trii.
     */
    std::vector<std::string> get_prefixes(const std::string &str) const;

    const_iterator begin() const;

    const_iterator end() const;

    /**
     * Prohodí všechny prvky mezi touto trií a `rhs`.
     *
     * Složitost: O(1)
     */
    void swap(trie &rhs);

    // Relační operátory

    /**
     * Vrací `true` pokud je trie `rhs` roven této.
     *
     * Dvě trie si jsou rovny, pokud obsahují stejné prvky.
     */
    bool operator==(const trie &rhs) const;

    /**
     * Vrací `true` pokud je tato trie menší než `rhs`.
     *
     * Trie jsou porovnávány tak, že všechny řetězce v lexikografickém pořadí
     * se porovnají lexikograficky.
     * To znamená, že ["abc"] < ["abc", "b"], ["aac", "b"] < ["abc", "b"].
     */
    bool operator<(const trie &rhs) const;

    /**
     * Vrátí novou trii, která obsahuje průnik této a `rhs`, tj. řežezce
     * přítomné v obou.
     *
     * Složitost: Implementace by neměla zbytečně procházet prvky, o kterých už ví,
     *            že v druhé trii být nemohou (např. ["aa", "ab", "ac"...], ["x"]).
     */
    trie operator&(trie const &rhs) const;

    /**
     * Vrátí novou trii, která obsahuje sjednocení této a `rhs`, tj. řežezce
     * přítomné alespoň v jedné z nich.
     */
    trie operator|(trie const &rhs) const;

private:
    //! ukazatel na kořenový uzel stromu
    trie_node *m_root = nullptr;

    //! počet unikátních slov, které trie obsahuje
    size_t m_size = 0;
};

//! 2 trie jsou si nerovné právě tehdy, když si nejsou rovné (viz operator==)
bool operator!=(const trie &lhs, const trie &rhs);

//! Lexicografické uspořádání, viz operator<
bool operator<=(const trie &lhs, const trie &rhs);

//! Lexicografické uspořádání, viz operator<
bool operator>(const trie &lhs, const trie &rhs);

//! Lexicografické uspořádání, viz operator<
bool operator>=(const trie &lhs, const trie &rhs);

/**
 * ADL customization point pro std::swap.
 * Výsledek `swap(lhs, rhs);` by měl být ekvivalentní s výsledkem
 * `lhs.swap(rhs);`
 */
void swap(trie &lhs, trie &rhs);


/**
 * Operátor výpisu do proudu.
 *
 * Tuto funkci netestujeme, ale pokud ji vhodně implementujete, budete mít
 * ve výstupech z testů užitěčně vypsaný obsah trie.
 */
std::ostream &operator<<(std::ostream &out, trie const &trie);
