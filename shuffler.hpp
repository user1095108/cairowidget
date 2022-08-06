#ifndef SHUFFLER_HPP
# define SHUFFLER_HPP
# pragma once

#include <cstddef> // std::size_t
#include <cstdint>
#include <utility> // std::index_sequence

namespace shuffler
{

namespace detail
{

//////////////////////////////////////////////////////////////////////////////
template <typename C, typename ...A>
constexpr bool compare(C const c, A const ...a) noexcept
{
  return [&]<auto ...I>(std::index_sequence<I...>) noexcept
    {
      return ((std::uint8_t(c >> 8 * I) == a) && ...);
    }(std::make_index_sequence<sizeof...(a)>());
}

}

static constexpr auto is_big_endian_v{
  detail::compare(std::uint32_t(0x01234567), 0x01, 0x23, 0x45, 0x67)
};

static constexpr auto is_little_endian_v{
  detail::compare(std::uint32_t(0x01234567), 0x67, 0x45, 0x23, 0x01)
};

static constexpr auto is_pdp_endian_v{
  detail::compare(std::uint32_t(0x01234567), 0x23, 0x01, 0x67, 0x45)
};

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
constexpr auto shuffle(T const i, std::index_sequence<J...>) noexcept ->
  decltype(std::enable_if_t<is_big_endian_v, T>{}) 
{
  return (shuffler<I, sizeof(T) - 1 - J>(i) | ...);
}

template <std::size_t ...I, std::size_t ...J, typename T>
constexpr auto shuffle(T const i, std::index_sequence<J...>) noexcept ->
  decltype(std::enable_if_t<is_little_endian_v, T>{}) 
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
