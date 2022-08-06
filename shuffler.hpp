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
constexpr bool compare(auto const c, auto const ...a) noexcept
{
  return [&]<auto ...I>(std::index_sequence<I...>) noexcept
    {
      return ((std::uint8_t(c >> 8 * I) == a) && ...);
    }(std::make_index_sequence<sizeof...(a)>());
}

static constexpr auto is_big_endian_v{
  compare(std::uint32_t(0x01234567), 0x01, 0x23, 0x45, 0x67)
};

static constexpr auto is_little_endian_v{
  compare(std::uint32_t(0x01234567), 0x67, 0x45, 0x23, 0x01)
};

static constexpr auto is_pdp_endian_v{
  compare(std::uint32_t(0x01234567), 0x23, 0x01, 0x67, 0x45)
};

//////////////////////////////////////////////////////////////////////////////
template <std::size_t ...I, std::size_t ...J, typename T>
constexpr T shuffle(T const i, std::index_sequence<J...>) noexcept
  requires(is_big_endian_v)
{
  return ((std::uint8_t(i >> 8 * I) << 8 * (sizeof(T) - 1 - J)) | ...);
}

template <std::size_t ...I, std::size_t ...J, typename T>
constexpr T shuffle(T const i, std::index_sequence<J...>) noexcept
  requires(is_little_endian_v)
{
  return ((std::uint8_t(i >> 8 * I) << 8 * J) | ...);
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
