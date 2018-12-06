/*
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 */

/**
 * Protože výsledky rozdělení nejsou přenosné mezi různými standardními
 * knihovnami, tento soubor obsahuje jejich minimalistickou reimplementaci.
 */

#pragma once

#include <type_traits>
#include <algorithm>
#include <utility>

/**
 * Instantiations of class template \bernoulli_distribution model a
 * \random_distribution. Such a random distribution produces bool values
 * distributed with probabilities P(true) = p and P(false) = 1-p. p is
 * the parameter of the distribution.
 */
class bernoulli_distribution {
public:
    /**
     * Constructs a \bernoulli_distribution object.
     * p is the parameter of the distribution.
     *
     * Requires: 0 <= p <= 1
     */
    explicit bernoulli_distribution(double p_arg = 0.5):
        m_p(p_arg)
    {}

    /**
     * Returns: a random variate distributed according to the
     * \bernoulli_distribution.
     */
    template<class Engine>
    bool operator()(Engine& eng) const {
        if (m_p == 0) {
            return false;
        } else {
            return static_cast<double>(eng() - eng.min()) <= m_p * static_cast<double>(eng.max() - eng.min());
        }
    }

private:
    double m_p;
};


namespace detail {

template<class Engine, class T>
T generate_uniform_real(
    Engine& eng, T min_value, T max_value,
    std::false_type  /** is_integral<Engine::result_type> */) {
    for (;;) {
        T numerator = static_cast<T>(eng() - (eng.min)());
        T divisor = static_cast<T>((eng.max)() - (eng.min)());
        T result = numerator / divisor * (max_value - min_value) + min_value;
        if (result < max_value) return result;
    }
}

template<class Engine, class T>
T generate_uniform_real(
    Engine& eng, T min_value, T max_value,
    std::true_type  /** is_integral<Engine::result_type> */) {
    for (;;) {
        T numerator = static_cast<T>(eng() - eng.min());
        T divisor = static_cast<T>(eng.max() - eng.min()) + 1;
        T result = numerator / divisor * (max_value - min_value) + min_value;
        if (result < max_value) return result;
    }
}

template<class Engine, class T>
T generate_uniform_real(Engine& eng, T min_value, T max_value) {
    if (max_value / 2 - min_value / 2 > std::numeric_limits<T>::max() / 2) {
        return 2 * generate_uniform_real(eng, T(min_value / 2), T(max_value / 2));
    } else {
        typedef typename Engine::result_type base_result;
        return generate_uniform_real(eng, min_value, max_value,
                                     std::is_integral<base_result>());
    }
}

}

/**
 * The class template uniform_real_distribution models a \random_distribution.
 * On each invocation, it returns a random floating-point value uniformly
 * distributed in the range [min..max).
 */
class uniform_real_distribution {
public:
    /**
     * Constructs a uniform_real_distribution. @c min and @c max are
     * the parameters of the distribution.
     *
     * Requires: min <= max
     */
    explicit uniform_real_distribution(double min_arg = 0.0, double max_arg = 1.0):
        m_min(min_arg),
        m_max(max_arg)
    {}

    /** Returns a value uniformly distributed in the range [min, max). */
    template<class Engine>
    double operator()(Engine& eng) const {
        return detail::generate_uniform_real(eng, m_min, m_max);
    }

private:
    double m_min;
    double m_max;
};



namespace detail {


#ifdef _MSC_VER
#pragma warning(push)
// disable division by zero warning, since we can't
// actually divide by zero. Same for mod
#pragma warning(disable:4723 4724 4307)
#endif

template<class Engine, class T>
T generate_uniform_int(
    Engine& eng, T min_value, T max_value) {
    static_assert(std::is_integral<typename Engine::result_type>::value, "");
    using range_type = typename std::make_unsigned<T>::type;
    typedef typename Engine::result_type base_result;
    // ranges are always unsigned or unbounded
    using base_unsigned = typename std::make_unsigned<T>::type;
    const range_type range = max_value - min_value;
    const base_result bmin = eng.min();
    const base_unsigned brange = static_cast<base_unsigned>(eng.max() - eng.min());

    if (range == 0) {
        return min_value;
    } else if (brange == range) {
        // this will probably never happen in real life
        // basically nothing to do; just take care we don't overflow / underflow
        base_unsigned v = static_cast<base_unsigned>(eng() - bmin);
        return v + min_value;
    } else if (brange < range) {
        // use rejection method to handle things like 0..3 --> 0..4
        for (;;) {
            // concatenate several invocations of the base RNG
            // take extra care to avoid overflows

            //  limit == floor((range+1)/(brange+1))
            //  Therefore limit*(brange+1) <= range+1
            range_type limit;
            if (range == (std::numeric_limits<range_type>::max)()) {
                limit = range / (range_type(brange) + 1);
                if (range % (range_type(brange) + 1) == range_type(brange))
                    ++limit;
            } else {
                limit = (range + 1) / (range_type(brange) + 1);
            }
            range_type result = range_type(0);
            range_type mult = range_type(1);

            while (mult <= limit) {
                result += static_cast<range_type>(static_cast<range_type>(eng() - bmin) * mult);

                if (mult * range_type(brange) == range - mult + 1) {
                    return(result);
                }
                mult *= range_type(brange) + range_type(1);
            }

            range_type result_increment =
                generate_uniform_int(
                    eng,
                    static_cast<range_type>(0),
                    static_cast<range_type>(range / mult)
                );
            if (std::numeric_limits<range_type>::is_bounded && ((std::numeric_limits<range_type>::max)() / mult < result_increment)) {
                // The multiplcation would overflow.  Reject immediately.
                continue;
            }
            result_increment *= mult;
            // unsigned integers are guaranteed to wrap on overflow.
            result += result_increment;
            if (result < result_increment) {
                // The addition overflowed.  Reject.
                continue;
            }
            if (result > range) {
                // Too big.  Reject.
                continue;
            }
            return result + min_value;
        }
    } else {                   // brange > range
        using mixed_range_type = base_unsigned;

        mixed_range_type bucket_size;
        if (brange == (std::numeric_limits<base_unsigned>::max)()) {
            bucket_size = static_cast<mixed_range_type>(brange) / (static_cast<mixed_range_type>(range) + 1);
            if (static_cast<mixed_range_type>(brange) % (static_cast<mixed_range_type>(range) + 1) == static_cast<mixed_range_type>(range)) {
                ++bucket_size;
            }
        } else {
            bucket_size = static_cast<mixed_range_type>(brange + 1) / (static_cast<mixed_range_type>(range) + 1);
        }
        for (;;) {
            mixed_range_type result = static_cast<mixed_range_type>(eng() - bmin);
            result /= bucket_size;
            // result and range are non-negative, and result is possibly larger
            // than range, so the cast is safe
            if (result <= static_cast<mixed_range_type>(range))
                return result + min_value;
        }
    }
}

}

/**
 * The class template uniform_int_distribution models a \random_distribution.
 * On each invocation, it returns a random integer value uniformly
 * distributed in the set of integers {min, min+1, min+2, ..., max}.
 */
class uniform_int_distribution {
public:

    /**
     * Constructs a uniform_int_distribution. @c min and @c max are
     * the parameters of the distribution.
     *
     * Requires: min <= max
     */
    explicit uniform_int_distribution(
        int min_arg = 0,
        int max_arg = std::numeric_limits<int>::max()):
        m_min(min_arg),
        m_max(max_arg) {}

    /** Returns an integer uniformly distributed in the range [min, max]. */
    template<class Engine>
    int operator()(Engine& eng) const {
        return detail::generate_uniform_int(eng, m_min, m_max);
    }


private:
    int m_min;
    int m_max;
};


#ifdef _MSC_VER
#pragma warning(pop)
#endif
