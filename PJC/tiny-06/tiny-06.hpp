#pragma once

#include <memory>
#include <limits>

enum class generator_kind {
    random,
    mersenne,
    minstd
};


class numbers_generator {
private:
    int i_max = std::numeric_limits<int>::min(), i_min = std::numeric_limits<int>::max();
    double d_max = std::numeric_limits<double>::min(), d_min = std::numeric_limits<double>::max();
    size_t d_count = 0, i_count = 0, b_count = 0;
    size_t pos_count = 0;
    /**
     * Vygeneruje náhodné číslo v intervalu [0, 1)
     */
    virtual double random_double_impl() = 0;
    /**
     * Vygeneruje náhodné celé číslo v intervalu [lower, upper]
     */
    virtual int    random_int_impl(int lower, int upper) = 0;
    /**
     * Vygeneruje náhodnou pravdivostní hodnotu (true/false)
     */
    virtual bool   random_bool_impl() = 0;
public:
    /**
     * Vygeneruje náhodné číslo v intervalu [0, 1)
     */
    double random_double(){
        auto res = random_double_impl();
        d_count++;
        if(res > d_max) d_max = res;
        if(res < d_min) d_min = res;
        return res;
    };
    /**
     * Vygeneruje náhodné celé číslo v intervalu [lower, upper]
     */
    int    random_int(int lower, int upper){
        auto res = random_int_impl(lower, upper);
        i_count++;
        if(res > i_max) i_max = res;
        if(res < i_min) i_min = res;
        return res;
    };
    /**
     * Vygeneruje náhodnou pravdivostní hodnotu (true/false)
     */
    bool   random_bool(){
        bool res = random_bool_impl();
        if(res) pos_count++;
        b_count++;
        return res;
    }

    virtual ~numbers_generator() = default;

    static std::unique_ptr<numbers_generator> create_by_kind(generator_kind kind, size_t seed);

    double min_generated_double() const {return d_min;}
    double max_generated_double() const {return d_max;}
    size_t doubles_generated() const {return d_count;}
    int min_generated_int() const {return i_min;}
    int max_generated_int() const {return i_max;}
    size_t ints_generated() const {return i_count;}
    size_t trues_generated() const {return pos_count;}
    size_t falses_generated() const {return b_count - pos_count;}
    size_t bools_generated() const {return b_count;}
};
