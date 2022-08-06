#ifndef SHUFFLER_HPP
# define SHUFFLER_HPP
# pragma once

#include <cassert>
#include <cstddef> // std::size_t
#include <cstdint>
#include <utility> // std::index_sequence

namespace shuffler
{

namespace detail
{

//////////////////////////////////////////////////////////////////////////////
constexpr bool compare(auto const c, auto const ... a) noexcept
{
  return [&]<auto ...I>(std::index_sequence<I...>) noexcept
    {
      return ((std::uint8_t(c >> 8 * I) == a) && ...);
    }(std::make_index_sequence<sizeof...(a)>());
}

constexpr auto is_big_endian() noexcept
{
  return compare(std::uint32_t(0x01234567), 0x01, 0x23, 0x45, 0x67);
}

constexpr auto is_little_endian() noexcept
{
  return compare(std::uint32_t(0x01234567), 0x67, 0x45, 0x23, 0x01);
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
