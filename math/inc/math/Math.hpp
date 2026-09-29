#pragma once

namespace rast
{
inline namespace math
{
    template<typename T>
    constexpr T min(T a, T b)
    {
        return a < b ? a : b;
    }

    template<typename T>
    constexpr T max(T a, T b)
    {
        return a > b ? a : b;
    }

    template<typename T>
    constexpr T clamp(T v, T minValue, T maxValue)
    {
        return min( max( v, minValue ), maxValue );
    }

    }
}  // namespace rast