#pragma once

#include <iosfwd>
#include <string>
#include <memory>
#include <map>
#include <exception>
#include <cassert>

// forward declarations
class expr;
struct fmt_expr;

/**
 * The base class of all expressions. Note that all expression classes
 * (including this base) are private to the implementation and should not be
 * exposed to other code. The rest of the program should use expressions
 * *only* via `expr`.
 *
 * This subclasses `std::enable_shared_from_this` to (surprisingly) enable
 * getting shared_ptr to this from a method (see reference for details).
 */
class expr_base : public std::enable_shared_from_this<expr_base>
{
    friend class expr;
protected:
    //! protected constructor to disallow direct construction outside of
    //! class hierarchy and `expr`
    expr_base() = default;
public:
    using variable_map_t = std::map<std::string, double>;
    enum class WriteFormat {
        Prefix,
        Infix,
        Postfix
    };

    virtual ~expr_base() = default;

    /**
     * Evaluate the expression. Variables are substituted according to the
     * supplied variable map. If a variable is not present in the map,
     * unbound_variable_exception should be thrown.
     */
    virtual double evaluate(const variable_map_t &variables) const = 0;

    /**
     * Return derivative of the expression. Although the exact order of
     * operands does not matter in reality (for commutative operations), it
     * matters for tests, so implement it exactly as stated below:
     *
     * (f+g)' = f' + g'
     * (f-g)' = f' - g'
     * (f*g)' = f'*g + f*g'
     * (f/g)' = (f'*g - f*g')/g^2
     * (f^g)' = f^g * ( (f'*g)/f + log(f)*g' )
     * sin(f)' = cos(f) * f'
     * cos(f)' = (0-sin(f)) * f'
     * log(f)' = f' / f
     */
    virtual expr derive(std::string const &variable) const = 0;

    /**
     * Simplify the expression by the following rules:
     * - 0+x = x
     * - x+0 = x
     * - x-0 = x

     * - 0*x = 0
     * - x*0 = 0
     * - 1*x = x
     * - x*1 = x

     * - 0/0 = 0/0
     * - 0/x = 0
     * - x/1 = x
     *
     * - x^1 = x
     * - 0^0 = 1 (not strictly true, but consistent with std::pow)
     * - x^0 = 1
     * - 0^x = 0
     *
     * - log(1) = 0
     *
     * where x stands for any expression.
     * To check the value of an expression, use operator== (e.g. with
     * expr::ZERO, expr::ONE).
     */
    virtual expr simplify() const = 0;
private:
    /**
     * Write expression in prefix form (LISP-like), e.g. "(+ (sin x) 12.3)".
     * Should only be used to implement operator<<(std::ostream&, const expr&)
     * or operator<<(std::ostream&, const fmt_expr&).
     *
     * You may ignore `fmt` if you are not implementing the bonus variant.
     */
    virtual void write(std::ostream& out, WriteFormat fmt) const = 0;

    // operator<< calls write, which is private (and shouldn't be called from anywhere else)
    friend std::ostream& operator<<(std::ostream&, const expr&);
    friend std::ostream& operator<<(std::ostream&, const fmt_expr&);

private:
    /**
     * Check if the expressions are equal. You may use dynamic_cast
     * to check if the argument is of the same type as `this`.
     *
     * Should only be used to implement operator==(expr const&, expr const&).
     */
    virtual bool equals(const expr_base& b) const = 0;

    // operator== calls equals, which is private (and shouldn't be called from anywhere else)
    friend bool operator==(const expr &a, const expr &b);
};

/**
 * Wrapper around dynamically allocated instances of expr_base. This type has
 * value semantics and since all subclasses of expr_base are immutable,
 * shallow copies are made.
 *
 * This type has overloaded functions and operators, so that expression
 * construction is easy and readable.
 */
class expr final {
private:
    using const_pointer = std::shared_ptr<const expr_base>;
    friend bool operator==(const expr &a, const expr &b);
public:
    using variable_map_t = expr_base::variable_map_t;
    using WriteFormat = expr_base::WriteFormat;

    static const expr ZERO;
    static const expr ONE;

    /*
     * Implicitly convertible from the internal shared ptr.
     *
     * This is a little black magic. Intuitively, this is the equivalent of
     *   expr(const_pointer e) : ptr(std::move(e)) {}
     * However, that would cause trouble when trying to implicitly convert e.g. from
     * shared_ptr<Number>, because that would require 2 user-defined conversions
     * (shared_ptr<Number> -> shared_ptr<const expr_base> -> expr), whereas C++
     * only allows one (see https://stackoverflow.com/questions/8610511/chaining-implicit-conversion-operators)
     *
     * This merges the 2 conversions into one, so you may write e.g.
     * `expr e = std::make_shared<Number>(42);`.
     */
    template <typename T, typename = std::enable_if_t<std::is_convertible<T*, const expr_base*>::value>>
    expr(std::shared_ptr<T> e) : ptr(std::static_pointer_cast<const expr_base>(std::move(e))) {}

    // default constructible
    expr() = default;

    // expression builders
    static expr number(double n);
    static expr variable(std::string name);

    // implicitly convertible to the internal shared ptr, implement operator->
    // that means you can call methods directly via `expr`, e.g. `e->simplify()`
    operator const_pointer const &() const {return ptr;}
    const expr_base* operator->() const {assert(ptr.get() != nullptr); return ptr.get();}
private:
    const_pointer ptr;
};

//! expression builders
expr operator+(expr a, expr b);
expr operator-(expr a, expr b);
expr operator*(expr a, expr b);
expr operator/(expr a, expr b);
expr pow(expr m, expr e);
expr sin(expr e);
expr cos(expr e);
expr log(expr e);

bool operator==(const expr &a, const expr &b);
static inline bool operator!=(const expr &a, const expr &b) {return !(a == b);}

std::ostream& operator<<(std::ostream &os, const expr &e);

// Pro bonus print
struct fmt_expr {
    const expr &e;
    expr::WriteFormat fmt;
};
std::ostream& operator<<(std::ostream &os, fmt_expr const &f);

//--- Exceptions
class math_exception : public std::exception {
public:
    math_exception(std::string s) : text(std::move(s)) {}
    virtual const char* what() const noexcept override {
        return text.c_str();
    }
protected:
    std::string text;
};

class domain_exception : public math_exception {
public:
    using math_exception::math_exception;
};

class unknown_function_exception : public math_exception {
public:
    using math_exception::math_exception;
};

class invalid_expression_exception : public math_exception {
public:
    using math_exception::math_exception;
};

class tokenize_error : public invalid_expression_exception {
public:
    using invalid_expression_exception::invalid_expression_exception;
};

class parse_error : public invalid_expression_exception {
public:
    using invalid_expression_exception::invalid_expression_exception;
};

class unbound_variable_exception : public math_exception {
public:
    using math_exception::math_exception;
};


/*
 * Create expression tree from mathematical expression.
 *
 * Must respect the usual mathematical ordering of operators and parentheses.
 * For details see the assignment text on CW.
 */
expr create_expression_tree(const std::string& expression);
