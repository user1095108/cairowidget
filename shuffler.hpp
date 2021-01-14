#ifndef SHUFFLER_HPP
# define SHUFFLER_HPP
# pragma once

#include <cstddef>

#include <cstdint>

#include <bit>

namespace shuffler
{

namespace detail
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

}

#endif // SHUFFLER_HPP
