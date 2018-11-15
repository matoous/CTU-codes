#pragma once

namespace pjc {

/**
 * Jednoduchá implementace komplexních čísel, viz třeba
 * https://cs.wikipedia.org/wiki/Komplexn%C3%AD_%C4%8D%C3%ADslo
 *
 * Vaším úkolem je pro ni přidat binární operátory +, -, *, fungující
 * pro pár instancí complex, ale i pro kombinaci instance complex a
 * double.
 */
class complex {
public:

    complex(double real, double imaginary);

    // Vrátí reálnou část
    double real() const;
    // Nastaví reálnou část na d
    void real(double d);

    // Vrátí imaginární část
    double imag() const;
    // Nastaví imaginární část na d
    void imag(double d);

    complex operator+(const complex& rhs) {
        auto result = *this;
        result.m_real += rhs.m_real;
        result.m_imag += rhs.m_imag;
        return result;
    }

    const complex operator+(const double& rhs) const {
        auto result = *this;
        result.m_real += rhs;
        return result;
    }

    friend complex operator+(double x, const complex& y) {
        return {
            y.m_real + x,
            y.m_imag
        };
    };  //so it can access the M private objects

    complex operator-(const complex& rhs) {
        auto result = *this;
        result.m_real -= rhs.m_real;
        result.m_imag -= rhs.m_imag;
        return result;
    }

    const complex operator-(const double& rhs) const {
        auto result = *this;
        result.m_real -= rhs;
        return result;
    }

    friend complex operator-(double x, const complex& y) {
        return {
                x - y.m_real,
                -y.m_imag
        };
    };  //so it can access the M private objects

    complex operator*(const complex& rhs) {
        auto result = *this;
        auto rl = m_real;
        auto ima = m_imag;
        result.m_real = rl * rhs.m_real - ima * rhs.m_imag;
        result.m_imag = rl * rhs.m_imag + ima * rhs.m_real;
        return result;
    }

    const complex operator*(const double& rhs) const {
        auto result = *this;
        result.m_real *= rhs;
        result.m_imag *= rhs;
        return result;
    }

    friend complex operator*(double x, const complex& y) {
        return {
                y.m_real * x,
                y.m_imag * x
        };
    };  //so it can access the M private objects
private:
    double m_real;
    double m_imag;
};

} // end namespace pjc
