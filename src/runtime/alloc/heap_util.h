#include <cstdint>
#include <type_traits>
namespace heap {

template <typename T>
struct Identity {
  using type = T;
};
// For rounding integers.
// Note: Omit the `n` from T type deduction, deduce only from the `x` argument.
template<typename T>
constexpr T RoundDown(T x, typename Identity<T>::type n);

template<typename T>
constexpr T RoundDown(T x, typename Identity<T>::type n) {
  
  return (x & -n);
}

template<typename T>
constexpr T RoundUp(T x, typename std::remove_reference<T>::type n);

template<typename T>
constexpr T RoundUp(T x, typename std::remove_reference<T>::type n) {
  return RoundDown(x + n - 1, n);
}

// For aligning pointers.
template<typename T>
inline T* AlignDown(T* x, uintptr_t n);

template<typename T>
inline T* AlignDown(T* x, uintptr_t n) {
  return reinterpret_cast<T*>(RoundDown(reinterpret_cast<uintptr_t>(x), n));
}

template<typename T>
inline T* AlignUp(T* x, uintptr_t n);

template<typename T>
inline T* AlignUp(T* x, uintptr_t n) {
  return reinterpret_cast<T*>(RoundUp(reinterpret_cast<uintptr_t>(x), n));
}
}