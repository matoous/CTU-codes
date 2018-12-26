#pragma once

#include <type_traits>

namespace pjc {

template <typename T>
struct is_trivially_copyable : std::is_trivially_copyable<T> {};

}
