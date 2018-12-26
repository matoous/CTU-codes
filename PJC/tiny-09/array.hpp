#ifndef ARRAY_HPP_INCLUDED
#define ARRAY_HPP_INCLUDED

#include <cstddef>
#include <memory>
#include <iostream>
#include <algorithm>
#include <type_traits>
#include <cstring>

using std::size_t;

#include "pjc-traits.hpp"


template <typename T>
void print_array(const T* arr, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        std::cout << arr[i] << ' ';
    }
    std::cout << '\n';
}

template <typename T>
void copy_array(const T* from, T* to, size_t size) {
    std::copy(from, from + size, to);
}

template <typename T>
void resize_array(std::unique_ptr<T[]>& arr, size_t oldSize, size_t newSize) {
    auto newArr = std::make_unique<T[]>(newSize);
    size_t minSize = (oldSize < newSize) ? oldSize : newSize;
    copy_array(arr.get(), newArr.get(), minSize);
    newArr.swap(arr);
}

#endif
