#ifndef PIXELITERATOR_HPP
# define PIXELITERATOR_HPP
# pragma once

#include <cstddef>

#include <array>

#include <iterator>

#include <memory>

template <typename T, std::size_t N>
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
  explicit pixel_iterator(T* const ptr) noexcept : ptr_(ptr) {}

  // increment, decrement
  auto& operator++() noexcept { return ptr_ += N, *this; }
  auto operator++(int) const noexcept { return pixel_iterator(ptr_ + N); }

  auto& operator--() noexcept { return ptr_ -= N, *this; }
  auto operator--(int) const noexcept { return pixel_iterator(ptr_ - N); }

  // arithmetic
  difference_type operator-(pixel_iterator const& o) const noexcept
  {
    return (ptr_ - o.ptr_) / N;
  }

  auto operator+(difference_type const M) const noexcept
  {
    return pixel_iterator(ptr_ + N * M);
  }

  auto operator-(difference_type const M) const noexcept
  {
    return pixel_iterator(ptr_ - N * M);
  }

  // comparison
  bool operator==(pixel_iterator const& o) const noexcept
  {
    return std::addressof(*ptr_ ) == std::addressof(*o.ptr_);
  }

  bool operator<(pixel_iterator const& o) const noexcept
  {
    return std::addressof(*ptr_ ) < std::addressof(*o.ptr_);
  }

  // member access
  auto& operator[](difference_type const i) const noexcept
  {
    return *(*this + i);
  }

  auto& operator*() const noexcept
  {
    return *const_cast<pointer>(reinterpret_cast<value_type const*>(
      std::addressof(*ptr_)));
  }
};

template <std::size_t N, typename T>
inline auto make_pixel_iterator(T* const p) noexcept
{
  return pixel_iterator<T, N>(p);
}

#endif // PIXELITERATOR_HPP

/*
  std::transform(std::execution::unseq,
    make_pixel_iterator<4>(src),
    make_pixel_iterator<4>(src + pixels_),
    make_pixel_iterator<4>(src),
    [](auto&& s) noexcept ->
      typename pixel_iterator<unsigned char, 4>::value_type
    {
      if constexpr (std::endian::little == std::endian::native)
      {
        // BGRA -> RGBx
        return {s[2], s[1], s[0]};
      }
      else if constexpr (std::endian::big == std::endian::native)
      {
        // ARGB -> RGBx
        return {s[1], s[2], s[3]};
      }
    }
  );
*/
