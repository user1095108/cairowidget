#ifndef SHUFFLER_HPP
# define SHUFFLER_HPP
# pragma once

#include <cassert>
#include <cstddef>
#include <cstdint>

namespace shuffler
{

namespace detail
{

//////////////////////////////////////////////////////////////////////////////
constexpr bool is_big_endian() noexcept
{
  std::uint32_t const c(0x01234567);

  return (std::uint8_t(c) == 0x01) &&
    (std::uint8_t(c >> 8) == 0x23) &&
    (std::uint8_t(c >> 16) == 0x45) &&
    (std::uint8_t(c >> 24) == 0x67);
}

constexpr bool is_little_endian() noexcept
{
  std::uint32_t const c(0x01234567);

  return (std::uint8_t(c) == 0x67) &&
    (std::uint8_t(c >> 8) == 0x45) &&
    (std::uint8_t(c >> 16) == 0x23) &&
    (std::uint8_t(c >> 24) == 0x01);
}

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
  if constexpr(detail::is_little_endian())
    return detail::shuffle1<I...>(i,
      std::make_index_sequence<sizeof...(I)>());
  else if constexpr(detail::is_big_endian())
    return detail::shuffle2<I...>(i,
      std::make_index_sequence<sizeof...(I)>());
  else
  {
    assert(!"unsupported endianess");
    return {};
  }
}

}

#endif // SHUFFLER_HPP
