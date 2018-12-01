#include "expr_impl.hpp"

#include <iostream>
#include <cmath>
#include <limits>

namespace exprs {
    // TODO

    double expr_plus::evaluate(const expr_base::variable_map_t &variables) const {
        throw std::logic_error("not implemented yet");
    }

    expr expr_plus::derive(std::string const &variable) const {
        throw std::logic_error("not implemented yet");
    }

    expr expr_plus::simplify() const {
        throw std::logic_error("not implemented yet");
    }

    expr_plus::expr_plus(const expr &a, const expr &b) : one(a), two(b) {}

    void expr_plus::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        out << "(+ " << one << " " << two << ")";
    }

    bool expr_plus::equals(const expr_base &b) const {
        return false;
    }

    double expr_minus::evaluate(const expr_base::variable_map_t &variables) const {
        throw std::logic_error("not implemented yet");
    }

    expr expr_minus::derive(std::string const &variable) const {
        throw std::logic_error("not implemented yet");
    }

    expr expr_minus::simplify() const {
        throw std::logic_error("not implemented yet");
    }

    void expr_minus::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        out << "(- " << one << " " << two << ")";
    }

    bool expr_minus::equals(const expr_base &b) const {
        throw std::logic_error("not implemented yet");
    }

    expr_minus::expr_minus(const expr &left, const expr &right) : one(left), two(right) {};

    double expr_multiply::evaluate(const expr_base::variable_map_t &variables) const {
        throw std::logic_error("not implemented yet");
    }

    expr expr_multiply::derive(std::string const &variable) const {
        throw std::logic_error("not implemented yet");
    }

    expr expr_multiply::simplify() const {
        throw std::logic_error("not implemented yet");
    }

    void expr_multiply::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        out << "(* " << one << " " << two << ")";
    }

    bool expr_multiply::equals(const expr_base &b) const {
        throw std::logic_error("not implemented yet");
    }

    expr_multiply::expr_multiply(const expr &a, const expr &b) : one(a), two(b) {};

    double expr_divide::evaluate(const expr_base::variable_map_t &variables) const {
        throw std::logic_error("not implemented yet");
    }

    expr expr_divide::derive(std::string const &variable) const {
        throw std::logic_error("not implemented yet");
    }

    expr expr_divide::simplify() const {
        throw std::logic_error("not implemented yet");
    }

    void expr_divide::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        out << "(/ " << one << " " << two << ")";
    }

    bool expr_divide::equals(const expr_base &b) const {
        throw std::logic_error("not implemented yet");
    }

    expr_divide::expr_divide(const expr &a, const expr &b) : one(a), two(b) {};

    double expr_pow::evaluate(const expr_base::variable_map_t &variables) const {
        throw std::logic_error("not implemented yet");
    }

    expr expr_pow::derive(std::string const &variable) const {
        throw std::logic_error("not implemented yet");
    }

    expr expr_pow::simplify() const {
        throw std::logic_error("not implemented yet");
    }

    void expr_pow::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        out << "(^ " << one << " " << two << ")";
    }

    bool expr_pow::equals(const expr_base &b) const {
        throw std::logic_error("not implemented yet");
    }

    expr_pow::expr_pow(const expr &a, const expr &b) : one(a), two(b) {};

    double expr_sin::evaluate(const expr_base::variable_map_t &variables) const {
        throw std::logic_error("not implemented yet");
    }

    expr expr_sin::derive(std::string const &variable) const {
        throw std::logic_error("not implemented yet");
    }

    expr expr_sin::simplify() const {
        throw std::logic_error("not implemented yet");
    }

    void expr_sin::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        out << "(sin " << one << ")";
    }

    bool expr_sin::equals(const expr_base &b) const {
        throw std::logic_error("not implemented yet");
    }

    expr_sin::expr_sin(const expr &a) : one(a) {};

    double expr_cos::evaluate(const expr_base::variable_map_t &variables) const {
        throw std::logic_error("not implemented yet");
    }

    expr expr_cos::derive(std::string const &variable) const {
        throw std::logic_error("not implemented yet");
    }

    expr expr_cos::simplify() const {
        throw std::logic_error("not implemented yet");
    }

    void expr_cos::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        out << "(cos " << one << ")";
    }

    bool expr_cos::equals(const expr_base &b) const {
        throw std::logic_error("not implemented yet");
    }

    expr_cos::expr_cos(const expr &a) : one(a) {};

    double expr_log::evaluate(const expr_base::variable_map_t &variables) const {
        throw std::logic_error("not implemented yet");
    }

    expr expr_log::derive(std::string const &variable) const {
        throw std::logic_error("not implemented yet");
    }

    expr expr_log::simplify() const {
        throw std::logic_error("not implemented yet");
    }

    expr_log::expr_log(const expr &a) : one(a) {};

    void expr_log::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        out << "(log " << one << ")";
    }

    bool expr_log::equals(const expr_base &b) const {
        throw std::logic_error("not implemented yet");
    }

    number::number(double a): num(a) {}

    void number::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        out << num;
    }

    double number::evaluate(const expr_base::variable_map_t &variables) const {
        return num;
    }

    expr number::derive(std::string const &variable) const {
        return expr();
    }

    expr number::simplify() const {
        return expr();
    }

    bool number::equals(const expr_base &b) const {
        return false;
    }

    variable::variable(std::string a) : var(a) {}

    void variable::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        out << var;
    }

    double variable::evaluate(const expr_base::variable_map_t &variables) const {
        return 0;
    }

    expr variable::derive(std::string const &variable) const {
        return expr();
    }

    expr variable::simplify() const {
        return expr();
    }

    bool variable::equals(const expr_base &b) const {
        return false;
    }
} // namespace exprs
