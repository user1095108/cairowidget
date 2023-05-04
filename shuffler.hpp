#ifndef SHUFFLER_HPP
# define SHUFFLER_HPP
# pragma once

#include <cstddef> // std::size_t
#include <cstdint> // std::uint8_t
#include <bit> // std::endian
#include <utility> // std::index_sequence

namespace shuffler
{

namespace detail
{

//////////////////////////////////////////////////////////////////////////////
template <std::size_t I, std::size_t J, typename T>
constexpr T shuffler(T const i) noexcept
{
//return T{std::uint8_t(i >> 8 * I)} << 8 * J;
  return (T{0xffu} << 8 * J) & (I < J ? i << 8 * (J - I) : i >> 8 * (I - J));
}

template <std::size_t ...I, std::size_t ...J, typename T>
constexpr T shuffle(T const i, std::index_sequence<J...>) noexcept
  requires(std::endian::native == std::endian::big)
{
  return (shuffler<I, sizeof(T) - 1 - J>(i) | ...);
}

template <std::size_t ...I, std::size_t ...J, typename T>
constexpr T shuffle(T const i, std::index_sequence<J...>) noexcept
  requires(std::endian::native == std::endian::little)
{
  return (shuffler<I, J>(i) | ...);
}

}

//////////////////////////////////////////////////////////////////////////////
template <std::size_t ...I, typename T>
constexpr T shuffle(T const i) noexcept
{
  return detail::shuffle<I...>(i, std::make_index_sequence<sizeof...(I)>());
}

}

#endif // SHUFFLER_HPP
