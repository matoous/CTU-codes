#ifndef CODE_SORT_H
#define CODE_SORT_H

#include <vector>
#include <algorithm>
#include <iostream>

// sablona pro volani radiciho algoritmu nad daty typu element_t *, tuto metodu vola testovaci trida
template<typename element_t>
using SortingAlgorithm = void (*)(std::vector<element_t *> &vector_to_sort);

// sablona pro mapovaci funkci. funkce se vola se znakem abecedy. funkce vam vrati prislusne poradi znaku v abecede.
// poradi muzete pouzit k urceni bucketu. implementace je v namespace Alphabet v souboru "main.cpp" jako funkce
// "get_bucket"
using MappingFunction = const unsigned long (*)(const char &character_of_alphabet);

// implementace vaseho algoritmu bude lexikograficky radit retezce velice kratke delky. Pro tento ukol je zvlaste vhodny
// Radix sort. S radix sortem jste se jiz setkali v predmetu ALG.
//
// Pri implementaci tohoto algoritmu se muzete inspirovat prave ze slajdu predmetu ALG. V pripade radix sortu je
// dulezity fakt, ze tento algoritmus nepracuje s primymi porovnavanimi prvku jako algoritmy mezi ktere patri treba
// merge sort.
//
// Na testovacich datech, ktera jsou k teto uloze k dispozici by mel byt Radix sort rychlejsi nez porovnavaci algoritmy.
//
// 1. vsimnete si, ze jako parameter vector_to_sort dostavate vector pointru na stringy, ktere mate seradit. to neni nahoda,
// pracovat s pameti je v tomto pripade daleko rychlejsi nez pracovat primo s retezci.
// 2. dostavate mapovaci funkci, ktera kazdemu znaku abecedy priradi jeho poradi v abecede.
// 3. dalsimi parametry jsou pocet retezcu a delka retezcu. vsechny stringy maji prave tuto delku. nemusite osetrovat
// zadne specialni pripady, ze by retezce byli jinak dlouhe. toto vam usetri osetrovani specialnich pripadu.
void radix_par(std::vector<std::string *> &vector_to_sort, const MappingFunction &mappingFunction,
               unsigned long alphabet_size, unsigned long string_lengths);

#endif //CODE_SORT_H
