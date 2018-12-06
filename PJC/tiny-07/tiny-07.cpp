#include "tiny-07.hpp"

#include <cstdint>

mersenne_twister_generator::mersenne_twister_generator(size_t seed):
    rng(seed)
{}

double mersenne_twister_generator::random_double() {
    return double_dist(rng);
}

int mersenne_twister_generator::random_int(int lower, int upper) {
    return uniform_int_distribution(lower, upper)(rng);
}

bool mersenne_twister_generator::random_bool() {
    return bool_dist(rng);
}

minstd_generator::minstd_generator(size_t seed):
    rng(static_cast<uint32_t>(seed))
{}

double minstd_generator::random_double() {
    return double_dist(rng);
}

int minstd_generator::random_int(int lower, int upper) {
    return uniform_int_distribution(lower, upper)(rng);
}

bool minstd_generator::random_bool() {
    return bool_dist(rng);
}

std::unique_ptr<mersenne_twister_generator> mersenne_twister_generator::clone() const {
    return std::make_unique<mersenne_twister_generator>(*this);
}

std::unique_ptr<minstd_generator> minstd_generator::clone() const {
    return std::make_unique<minstd_generator>(*this);
}

std::unique_ptr<fixed_generator> fixed_generator::clone() const {
    return std::make_unique<fixed_generator>(*this);
}
