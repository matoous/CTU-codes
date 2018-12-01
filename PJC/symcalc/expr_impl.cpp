#include "expr_impl.hpp"

#include <iostream>
#include <cmath>
#include <limits>

namespace exprs {
    // TODO

    double expr_plus::evaluate(const expr_base::variable_map_t &variables) const {
        return one->evaluate(variables) + two->evaluate(variables);
    }

    expr expr_plus::derive(std::string const &variable) const {
        return std::make_shared<expr_plus>(expr_plus(one->derive(variable), two->derive(variable)));
    }

    expr expr_plus::simplify() const {
        auto l = one->simplify(); auto r = two->simplify();
        if(l == expr::ZERO){
            return r;
        }
        if(r == expr::ZERO){
            return l;
        }
        return std::make_shared<exprs::expr_plus>(expr_plus(l,r));
    }

    expr_plus::expr_plus(const expr &a, const expr &b) : one(a), two(b) {}

    void expr_plus::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        out << "(+ " << one << " " << two << ")";
    }

    bool expr_plus::equals(const expr_base &b) const {
        if(const expr_plus* v = dynamic_cast<expr_plus const*>(b.shared_from_this().get())) {
            return v->one == one && v->two == two;
        }
        return false;
    }

    double expr_minus::evaluate(const expr_base::variable_map_t &variables) const {
        return one->evaluate(variables) - two->evaluate(variables);
    }

    expr expr_minus::derive(std::string const &variable) const {
        return std::make_shared<expr_minus>(expr_minus(one->derive(variable), two->derive(variable)));
    }

    expr expr_minus::simplify() const {
        auto l = one->simplify(); auto r = two->simplify();
        if(r == expr::ZERO){
            return l;
        }
        return std::make_shared<exprs::expr_minus>(expr_minus(l,r));
    }

    void expr_minus::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        out << "(- " << one << " " << two << ")";
    }

    bool expr_minus::equals(const expr_base &b) const {
        if(const expr_minus* v = dynamic_cast<expr_minus const*>(b.shared_from_this().get())) {
            return v->one == one && v->two == two;
        }
        return false;
    }

    expr_minus::expr_minus(const expr &left, const expr &right) : one(left), two(right) {};

    double expr_multiply::evaluate(const expr_base::variable_map_t &variables) const {
        return one->evaluate(variables) * two->evaluate(variables);
    }

    expr expr_multiply::derive(std::string const &variable) const {
        auto l = std::make_shared<expr_multiply>(one->derive(variable), two);
        auto r = std::make_shared<expr_multiply>(one, two->derive(variable));
        return std::make_shared<expr_plus>(expr_plus(l, r));
    }

    expr expr_multiply::simplify() const {
        auto l = one->simplify(); auto r = two->simplify();
        if(l == expr::ZERO){
            return expr::ZERO;
        }
        if(r == expr::ZERO){
            return expr::ZERO;
        }
        if(l == expr::ONE){
            return r;
        }
        if(r == expr::ONE){
            return l;
        }
        return std::make_shared<exprs::expr_multiply>(expr_multiply(l,r));
    }

    void expr_multiply::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        out << "(* " << one << " " << two << ")";
    }

    bool expr_multiply::equals(const expr_base &b) const {
        if(const expr_multiply* v = dynamic_cast<expr_multiply const*>(b.shared_from_this().get())) {
            return v->one == one && v->two == two;
        }
        return false;
    }

    expr_multiply::expr_multiply(const expr &a, const expr &b) : one(a), two(b) {};

    double expr_divide::evaluate(const expr_base::variable_map_t &variables) const {
        return one->evaluate(variables) / two->evaluate(variables);
    }

    expr expr_divide::derive(std::string const &variable) const {
        auto l = std::make_shared<expr_multiply>(one->derive(variable), two);
        auto r = std::make_shared<expr_multiply>(one, two->derive(variable));
        auto top = std::make_shared<expr_minus>(expr_minus(l, r));
        auto bot = std::make_shared<expr_pow>(expr_pow(two, expr::number(2)));
        return std::make_shared<expr_divide>(expr_divide(top, bot));
    }

    expr expr_divide::simplify() const {
        auto l = one->simplify(); auto r = two->simplify();
        if(l == expr::ZERO && r == expr::ZERO){
            return shared_from_this();
        }
        if(l == expr::ZERO){
            return expr::ZERO;
        }
        if(r == expr::ONE){
            return l;
        }
        return std::make_shared<exprs::expr_divide>(expr_divide(l,r));
    }

    void expr_divide::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        out << "(/ " << one << " " << two << ")";
    }

    bool expr_divide::equals(const expr_base &b) const {
        if(const expr_divide* v = dynamic_cast<expr_divide const*>(b.shared_from_this().get())) {
            return v->one == one && v->two == two;
        }
        return false;
    }

    expr_divide::expr_divide(const expr &a, const expr &b) : one(a), two(b) {};

    double expr_pow::evaluate(const expr_base::variable_map_t &variables) const {
        return pow(one->evaluate(variables), two->evaluate(variables));
    }

    expr expr_pow::derive(std::string const &variable) const {
        auto mul = shared_from_this();
        auto top = std::make_shared<expr_multiply>(expr_multiply(one->derive(variable), two));
        auto l = std::make_shared<expr_divide>(expr_divide(top, one));
        auto r = std::make_shared<expr_multiply>(expr_multiply(log(one),two->derive(variable)));
        auto mul2 = std::make_shared<expr_plus>(expr_plus(l,r));
        return std::make_shared<expr_multiply>(expr_multiply(mul, mul2));
    }

    expr expr_pow::simplify() const {
        auto l = one->simplify(); auto r = two->simplify();
        if(r == expr::ONE){
            return l;
        }
        if(l == expr::ZERO && r == expr::ZERO){
            return expr::ONE;
        }
        if(r == expr::ZERO){
            return expr::ONE;
        }
        if(l == expr::ZERO){
            return expr::ZERO;
        }
        return std::make_shared<exprs::expr_pow>(expr_pow(l,r));
    }

    void expr_pow::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        out << "(^ " << one << " " << two << ")";
    }

    bool expr_pow::equals(const expr_base &b) const {
        if(const expr_pow* v = dynamic_cast<expr_pow const*>(b.shared_from_this().get())) {
            return v->one == one && v->two == two;
        }
        return false;
    }

    expr_pow::expr_pow(const expr &a, const expr &b) : one(a), two(b) {};

    double expr_sin::evaluate(const expr_base::variable_map_t &variables) const {
        return sin(one->evaluate(variables));
    }

    expr expr_sin::derive(std::string const &variable) const {
        return std::make_shared<expr_multiply>(cos(one), one->derive(variable));
    }

    expr expr_sin::simplify() const {
        auto l = one->simplify();
        return std::make_shared<exprs::expr_sin>(expr_sin(l));
    }

    void expr_sin::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        out << "(sin " << one << ")";
    }

    bool expr_sin::equals(const expr_base &b) const {
        if(const expr_sin* v = dynamic_cast<expr_sin const*>(b.shared_from_this().get())) {
            return v->one == one;
        }
        return false;
    }

    expr_sin::expr_sin(const expr &a) : one(a) {};

    double expr_cos::evaluate(const expr_base::variable_map_t &variables) const {
        return cos(one->evaluate(variables));
    }

    expr expr_cos::derive(std::string const &variable) const {
        auto l = std::make_shared<expr_minus>(expr::ZERO, sin(one));
        return std::make_shared<expr_multiply>(l, one->derive(variable));
    }

    expr expr_cos::simplify() const {
        auto l = one->simplify();
        return std::make_shared<exprs::expr_cos>(expr_cos(l));
    }

    void expr_cos::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        out << "(cos " << one << ")";
    }

    bool expr_cos::equals(const expr_base &b) const {
        if(const expr_cos* v = dynamic_cast<expr_cos const*>(b.shared_from_this().get())) {
            return v->one == one;
        }
        return false;
    }

    expr_cos::expr_cos(const expr &a) : one(a) {};

    double expr_log::evaluate(const expr_base::variable_map_t &variables) const {
        auto of = one->evaluate(variables);
        try {
            auto res = std::log(of);
            if(std::isnan(res)){
                throw domain_exception("log of negative number");
            }
            if(std::isinf(res)){
                throw domain_exception("log of zero");
            }
            return res;
        }
        catch(...){
            throw domain_exception("log of zero");
        }
    }

    expr expr_log::derive(std::string const &variable) const {
        return std::make_shared<expr_divide>(expr_divide(one->derive(variable), one));
    }

    expr expr_log::simplify() const {
        expr x = one->simplify();
        if(x == expr::ONE) {
            return expr::ZERO;
        }
        return std::make_shared<exprs::expr_log>(expr_log(x));
    }

    expr_log::expr_log(const expr &a) : one(a) {};

    void expr_log::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        out << "(log " << one << ")";
    }

    bool expr_log::equals(const expr_base &b) const {
        if(const expr_log* v = dynamic_cast<expr_log const*>(b.shared_from_this().get())) {
            return v->one == one;
        }
        return false;
    }

    number::number(double a): num(a) {}

    void number::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        out << num;
    }

    double number::evaluate(const expr_base::variable_map_t &variables) const {
        return num;
    }

    expr number::derive(std::string const &variable) const {
        return expr::ZERO;
    }

    expr number::simplify() const {
        return shared_from_this();
    }

    bool number::equals(const expr_base &b) const {
        if(const number* v = dynamic_cast<number const*>(b.shared_from_this().get())) {
            return v->num == num;
        }
        return false;
    }

    variable::variable(std::string a) : var(a) {}

    void variable::write(std::ostream &out, expr_base::WriteFormat fmt) const {
        out << var;
    }

    double variable::evaluate(const expr_base::variable_map_t &variables) const {
        if(variables.find(var) == variables.end()){
            throw unbound_variable_exception("");
        }
        return variables.at(var);
    }

    expr variable::derive(std::string const &variable) const {
        if(variable.compare(var) == 0){
            return expr::number(1);
        }
        return expr::ZERO;
    }

    expr variable::simplify() const {
        return shared_from_this();
    }

    bool variable::equals(const expr_base &b) const {
        if(const variable* v = dynamic_cast<variable const*>(b.shared_from_this().get())) {
            return v->var == var;
        }
        return false;
    }
} // namespace exprs
