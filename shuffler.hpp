#ifndef SHUFFLER_HPP
# define SHUFFLER_HPP
# pragma once

#include <bit>

template <typename T, std::size_t N = 3>
class pixel_iterator
{
  T* ptr_;

public:
  using iterator_category = std::random_access_iterator_tag;
  using value_type = std::array<T, N>;
  using difference_type = std::ptrdiff_t;
  using pointer = value_type*;
  using reference = value_type&;

public:
  explicit pixel_iterator(T* const ptr) : ptr_(ptr) {}

  //
  auto& operator*() const noexcept
  {
    return *reinterpret_cast<pointer>(const_cast<void*>(
      static_cast<void const*>(std::addressof(*ptr_))));
  }

  //
  auto operator==(pixel_iterator const& other) const noexcept
  {
    return std::addressof(*ptr_ ) == std::addressof(*other.ptr_);
  }

  auto operator!=(pixel_iterator const& other) const noexcept
  {
    return !(*this == other);
  }

  //
  auto operator-(pixel_iterator const& other) const noexcept
  {
    return (ptr_ - other.ptr_) / N;
  }

  //
  auto& operator++() noexcept { return ptr_ += N, *this; }
  auto operator++(int) const noexcept { return pixel_iterator(ptr_ + N); }

  auto& operator--() noexcept { return ptr_ -= N, *this; }
  auto operator--(int) const noexcept { return pixel_iterator(ptr_ - N); }

  auto operator+(std::size_t const M) const noexcept
  {
    return pixel_iterator(ptr_ + N * M);
  }

  auto operator-(std::size_t const M) const noexcept
  {
    return pixel_iterator(ptr_ - N * M);
  }

  auto& operator[](std::size_t const I) const noexcept
  {
    return *(*this + I);
  }
};

namespace shuffler::detail
{

//////////////////////////////////////////////////////////////////////////////
template <std::size_t ...I, std::size_t ...J, typename T>
constexpr T shuffle1(T const i, std::index_sequence<J...>) noexcept
{
  return ((std::uint8_t(i >> 8 * I) << 8 * J) | ...);
}

template <std::size_t ...I, std::size_t ...J, typename T>
constexpr T shuffle2(T const i, std::index_sequence<J...>) noexcept
{
  return ((std::uint8_t(i >> 8 * I) << 8 * (sizeof(T) - 1 - J)) | ...);
}

}

template <std::size_t ...I, typename T>
constexpr T shuffle(T const i) noexcept
{
  if constexpr (std::endian::little == std::endian::native)
    return shuffler::detail::shuffle1<I...>(i,
      std::make_index_sequence<sizeof...(I)>());
  else if constexpr (std::endian::big == std::endian::native)
    return shuffler::detail::shuffle2<I...>(i,
      std::make_index_sequence<sizeof...(I)>());
}

#endif // SHUFFLER_HPP
