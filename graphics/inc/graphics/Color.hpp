#pragma once

#include <math/Math.hpp>

#include <cassert>
#include <cstdint>
#include <compare>

namespace rast
{
inline namespace graphics
{
union Color
{
    constexpr Color() noexcept;
    constexpr explicit Color( uint32_t rgba ) noexcept;
    constexpr Color( uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255u );

    ~Color() noexcept                          = default;
    constexpr Color( const Color& ) noexcept   = default;
    constexpr Color( Color&& ) noexcept        = default;
    constexpr Color& operator=( const Color& ) = default;
    constexpr Color& operator=( Color&& )      = default;

    constexpr auto operator<=>( const Color& rhs ) const noexcept;
    constexpr bool operator==( const Color& rhs ) const noexcept;

    Color  operator+( const Color& rhs ) const noexcept;
    Color& operator+=( const Color& rhs ) noexcept;
    Color  operator-( const Color& rhs ) const noexcept;
    Color& operator-=( const Color& rhs ) noexcept;
    Color  operator*( const Color& rhs ) const noexcept;
    Color& operator*=( const Color& rhs ) noexcept;
    Color  operator*( float rhs ) const noexcept;
    Color& operator*=( float rhs ) noexcept;
    Color  operator/( float rhs ) const noexcept;
    Color& operator/=( float rhs ) noexcept;

    uint32_t rgba;
    struct RGBA
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    } channels;
};

constexpr Color::Color() noexcept
: channels { 0, 0, 0, 255 }
{}

constexpr Color::Color( uint32_t rgba ) noexcept
: rgba { rgba }
{}

constexpr Color::Color( uint8_t r, uint8_t g, uint8_t b, uint8_t a ) noexcept
: channels { r, g, b, a }
{}

constexpr bool Color::operator==( const Color& rhs ) const noexcept
{
    return rgba == rhs.rgba;
}

constexpr auto Color::operator<=>( const Color& rhs ) const noexcept
{
    if ( const auto cmp = channels.r <=> rhs.channels.r; cmp != 0 )
        return cmp;
    if ( const auto cmp = channels.g <=> rhs.channels.g; cmp != 0 )
        return cmp;
    if ( const auto cmp = channels.b <=> rhs.channels.b; cmp != 0 )
        return cmp;

    return channels.a <=> rhs.channels.a;
}

inline Color Color::operator+( const Color& rhs ) const noexcept
{
    const auto r = static_cast<uint8_t>( math::min( channels.r + rhs.channels.r, 255 ) );
    const auto g = static_cast<uint8_t>( math::min( channels.g + rhs.channels.g, 255 ) );
    const auto b = static_cast<uint8_t>( math::min( channels.b + rhs.channels.b, 255 ) );
    const auto a = static_cast<uint8_t>( math::min( channels.a + rhs.channels.a, 255 ) );

    return { r, g, b, a };
}

inline Color& Color::operator+=( const Color& rhs ) noexcept
{
    *this = *this + rhs;
    return *this;
}

inline Color Color::operator-( const Color& rhs ) const noexcept
{
    const auto r = static_cast<uint8_t>( math::max( channels.r - rhs.channels.r, 0 ) );
    const auto g = static_cast<uint8_t>( math::max( channels.g - rhs.channels.g, 0 ) );
    const auto b = static_cast<uint8_t>( math::max( channels.b - rhs.channels.b, 0 ) );
    const auto a = static_cast<uint8_t>( math::max( channels.a - rhs.channels.a, 0 ) );

    return { r, g, b, a };
}

inline Color& Color::operator-=( const Color& rhs ) noexcept
{
    *this = *this - rhs;
    return *this;
}

inline Color Color::operator*( const Color& rhs ) const noexcept
{
    const auto r = static_cast<uint8_t>( channels.r * rhs.channels.r / 255 );
    const auto g = static_cast<uint8_t>( channels.g * rhs.channels.g / 255 );
    const auto b = static_cast<uint8_t>( channels.b * rhs.channels.b / 255 );
    const auto a = static_cast<uint8_t>( channels.a * rhs.channels.a / 255 );
}

inline Color& Color::operator*=( const Color& rhs ) noexcept
{
    *this = *this * rhs;
    return *this;
}

inline Color Color::operator*(float rhs) const noexcept
{
    const auto red   = static_cast<uint8_t>( math::clamp( static_cast<float>( channels.r ) * rhs, 0.0f, 255.0f ) );
    const auto green = static_cast<uint8_t>( math::clamp( static_cast<float>( channels.g ) * rhs, 0.0f, 255.0f ) );
    const auto blue  = static_cast<uint8_t>( math::clamp( static_cast<float>( channels.b ) * rhs, 0.0f, 255.0f ) );
    const auto alpha = static_cast<uint8_t>( math::clamp( static_cast<float>( channels.a ) * rhs, 0.0f, 255.0f ) );

    return { red, green, blue, alpha };
}

inline Color& Color::operator*=( float rhs ) noexcept
{
    *this = *this * rhs;
    return *this;
}

inline Color Color::operator/( float rhs ) const noexcept
{
    assert( rhs != 0.0f );

    rhs = 1.0f / rhs;

    return operator*( rhs );
}

inline Color& Color::operator/=( float rhs ) noexcept
{
    assert( rhs != 0.0f );

    rhs = 1.0f / rhs;

    return operator*=( rhs );
}

inline Color operator*(float lhs, const Color& rhs) noexcept
{
    return rhs * lhs;
}

}  // namespace graphics
}  // namespace rast