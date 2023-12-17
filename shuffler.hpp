#ifndef SHUFFLER_HPP
# define SHUFFLER_HPP
# pragma once

#include <utility> // std::size_t, std::index_sequence

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
{
#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN || \
    defined(__BIG_ENDIAN__) || \
    defined(__ARMEB__) || \
    defined(__THUMBEB__) || \
    defined(__AARCH64EB__) || \
    defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)
  return (shuffler<I, sizeof(T) - 1 - J>(i) | ...);
#else
  return (shuffler<I, J>(i) | ...);
#endif
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
