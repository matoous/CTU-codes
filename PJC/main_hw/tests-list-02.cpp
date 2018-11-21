#include "catch.hpp"

#include "list.hpp"

#include "test-helpers.hpp"

/*
    Testy pro stage-02.

    Tento krok testuje metody
      * begin()
      * begin() const
      * cbegin()
      * end()
      * end() const
      * cend()
    A metody iterátorů z nich vytvořených, tj
      * iterator()
      * operátory prefix ++, postfix ++
      * operátory prefix --, postfix --
      * operátory ==, !=
      * operátory *, -> 


    Testy v tomto kroku předpokládají, že metody testované v minulém
    kroku fungují správně, a tudíž se na nich mohou stavět testy pro další
    funkcionalitu.

*/

using pjc::list;


TEST_CASE("Default constructed iterators are equal", "[stage2]") {
}
