//
// Created by matouus on 27/11/18.
//
#define PICOBENCH_IMPLEMENT_WITH_MAIN
#include "picobench.hpp"
#include "autocorrect.hpp"
#include "test-helpers.hpp"

using namespace pjc;

auto wrongWords = generateWrongWords("/home/matouus/code/CTU-codes/PJC/autocorrect/words.txt");

static void serial_correct(picobench::state& s)
{
    std::fstream myfile;
    myfile.open("/home/matouus/code/CTU-codes/PJC/autocorrect/words.txt");
    autocorrect ac(myfile);
    myfile.close();

    std::cout << "Benchmarking serial correction" << std::endl;
    for (auto _ : s) {
        auto x = ac.correct(wrongWords);
    }
}
PICOBENCH(serial_correct).iterations({1,2,4}).baseline(true);

static void parallel_correct(picobench::state& s)
{
    std::fstream myfile;
    myfile.open("/home/matouus/code/CTU-codes/PJC/autocorrect/words.txt");
    autocorrect ac(myfile);
    myfile.close();

    std::cout << "Benchmarking parallel correction" << std::endl;
    for (auto _ : s) {
        auto x = ac.p_correct(wrongWords);
    }
}
PICOBENCH(parallel_correct).iterations({1,2,4});