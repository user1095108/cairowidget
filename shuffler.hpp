#ifndef SHUFFLER_HPP
# define SHUFFLER_HPP
# pragma once

#include <cstddef> // std::size_t
#include <cstdint> // std::uint8_t
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

#if defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN || \
    defined(__LITTLE_ENDIAN__) || \
    defined(__ARMEL__) || \
    defined(__THUMBEL__) || \
    defined(__AARCH64EL__) || \
    defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__)
template <std::size_t ...I, std::size_t ...J, typename T>
constexpr T shuffle(T const i, std::index_sequence<J...>) noexcept
//requires(std::endian::native == std::endian::little)
{
  return (shuffler<I, J>(i) | ...);
}
#else
template <std::size_t ...I, std::size_t ...J, typename T>
constexpr T shuffle(T const i, std::index_sequence<J...>) noexcept
//requires(std::endian::native == std::endian::big)
{
  return (shuffler<I, sizeof(T) - 1 - J>(i) | ...);
}
#endif

}

//////////////////////////////////////////////////////////////////////////////
template <std::size_t ...I, typename T>
constexpr T shuffle(T const i) noexcept
{
  return detail::shuffle<I...>(i, std::make_index_sequence<sizeof...(I)>());
}

}

#endif // SHUFFLER_HPP
