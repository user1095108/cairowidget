#ifndef PIXELITERATOR_HPP
# define PIXELITERATOR_HPP
# pragma once

#include <cstddef>

#include <array>

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

  //
  auto& operator*() const noexcept
  {
    return *const_cast<pointer>(reinterpret_cast<value_type const*>(
      std::addressof(*ptr_)));
  }

  //
  auto operator==(pixel_iterator const other) const noexcept
  {
    return std::addressof(*ptr_ ) == std::addressof(*other.ptr_);
  }

  auto operator!=(pixel_iterator const other) const noexcept
  {
    return !(*this == other);
  }

  auto operator<(pixel_iterator const other) const noexcept
  {
    return std::addressof(*ptr_ ) < std::addressof(*other.ptr_);
  }

  //
  auto operator-(pixel_iterator const other) const noexcept
  {
    return (ptr_ - other.ptr_) / N;
  }

  //
  auto& operator++() noexcept { return ptr_ += N, *this; }
  auto operator++(int) const noexcept { return pixel_iterator(ptr_ + N); }

  auto& operator--() noexcept { return ptr_ -= N, *this; }
  auto operator--(int) const noexcept { return pixel_iterator(ptr_ - N); }

  //
  auto operator+(std::size_t const M) const noexcept
  {
    return pixel_iterator(ptr_ + N * M);
  }

  auto operator-(std::size_t const M) const noexcept
  {
    return pixel_iterator(ptr_ - N * M);
  }

  //
  auto& operator[](std::size_t const I) const noexcept
  {
    return *(*this + I);
  }
};

template <std::size_t N, typename T>
inline auto make_pixel_iterator(T* const p) noexcept
{
  return pixel_iterator<T, N>(p);
}

#endif // PIXELITERATOR_HPP
