#include "catch.hpp"

#include "list.hpp"

#include <vector>
#include <algorithm>

/*
   Testy pro stage-01.

   Tyto testy po kouskách zkontrolují metody
     * list() (základní konstruktor)
     * ~list() (destruktor)
     * push_back()
     * back()
     * pop_back()
     * push_front()
     * front()
     * pop_front()

   Kvůli omezení se na tuto sadu metod vám tyto testy neodhalí všechny
   chyby, ale měly by jich najít většinu. V dalších krocích se očekává,
   že vám metody z tohoto kroku fungují.
*/


using pjc::list;

TEST_CASE("Default constructed list passes basic accessors", "[stage1]") {

}
