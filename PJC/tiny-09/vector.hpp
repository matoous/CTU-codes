#ifndef VECTOR_HPP_INCLUDED
#define VECTOR_HPP_INCLUDED

#include "array.hpp"
#include "pjc-traits.hpp"
#include <cstddef>
#include <memory>
#include <iterator>
#include <iostream>


template <typename T>
class vector {
public:
    vector() = default;

    vector(const vector& rhs);
    vector& operator=(const vector& rhs);

    void reserve(size_t cap);
    void push_back(const T& val) {
        if (m_size == m_capacity) {
            reserve((m_capacity + 1) * 2);
        }

        m_data[m_size] = val;
        m_size++;
    }

private:
    template <typename InputIterator>
    void push_back(InputIterator from, InputIterator to, std::input_iterator_tag) {
        while (from != to) {
            push_back(*from);
            ++from;
        }
    }

    template <typename ForwardIterator>
    void push_back(ForwardIterator from, ForwardIterator to, std::forward_iterator_tag) {
        reserve(m_size + std::distance(from, to)); // std::distance je O(1) pro random access iterátory, O(n) pro forward
        while (from != to) {
            push_back(*from);
            ++from;
        }
    }



public:

    template <typename InputIterator>
    void push_back(InputIterator from, InputIterator to) {
        push_back(from, to, typename std::iterator_traits<InputIterator>::iterator_category{});
    }

    void clear() { m_size = 0; }
    void pop_back() { --m_size; }

    size_t size() const { return m_size; }
    size_t capacity() const { return m_capacity; }
    void swap(vector& rhs);

    T& operator[](size_t i) { return m_data[i]; }
    const T& operator[](size_t i) const { return m_data[i]; }

    using iterator = T*;
    using const_iterator = const T*;

    iterator begin() { return iterator(m_data.get()); }
    iterator end() { return iterator(m_data.get() + m_size); }

private:
    std::unique_ptr<T[]> m_data;
    size_t m_capacity = 0;
    size_t m_size = 0;
};

template <typename T>
vector<T>::vector(const vector& rhs) :
    m_data(std::make_unique<T[]>(rhs.m_size)),
    m_capacity(rhs.m_capacity),
    m_size(rhs.m_size) {
    copy_array(rhs.m_data.get(), m_data.get(), m_size);
}


template <typename T>
vector<T>& vector<T>::operator=(const vector& rhs) {
    vector temp(rhs);
    swap(temp);
    return *this;
}

template <typename T>
void vector<T>::reserve(size_t cap) {
    if (m_capacity < cap) {
        resize_array(m_data, m_size, cap);
        m_capacity = cap;
    }
}

template <typename T>
void vector<T>::swap(vector& rhs) {
    std::swap(m_data, rhs.m_data);
    std::swap(m_capacity, rhs.m_capacity);
    std::swap(m_size, rhs.m_size);
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const vector<T>& v) {
    out << '(';
    if (v.size() != 0) {
        out << v[0];
    }
    for (size_t i = 1; i < v.size(); i++) {
        out << ", " << v[i];
    }
    return out << ')';
}

#endif
