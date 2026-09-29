#pragma once

#include <math/Math.hpp>

#include <cassert>
#include <cmath>
#include <compare>
#include <cstdint>

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

    static Color fromFloats( float r, float g, float b, float a = 1.0f );
    static Color fromHSV( float H, float S, float V ) noexcept;

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

inline Color Color::operator*( float rhs ) const noexcept
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

Color Color::fromFloats( float r, float g, float b, float a )
{
    const auto red   = static_cast<uint8_t>( math::clamp( r * 255.0f, 0.0f, 255.0f ) );
    const auto green = static_cast<uint8_t>( math::clamp( g * 255.0f, 0.0f, 255.0f ) );
    const auto blue  = static_cast<uint8_t>( math::clamp( b * 255.0f, 0.0f, 255.0f ) );
    const auto alpha = static_cast<uint8_t>( math::clamp( a * 255.0f, 0.0f, 255.0f ) );

    return { red, green, blue, alpha };
}

Color Color::fromHSV( float H, float S, float V )
{
    H = fmodf( H, 360.0f );
    if ( H < 0.0f )
        H += 360.0f;

    S = math::clamp( S, 0.0f, 1.0f );
    V = math::clamp( V, 0.0f, 1.0f );

    float C  = V * S;
    float m  = V - C;
    float H2 = H / 60.0f;
    float X  = C * ( 1.0f - fabsf( fmodf( H2, 2.0f ) - 1.0f ) );

    float r = 0.0f, g = 0.0f, b = 0.0f;

    switch ( static_cast<int>( H2 ) )
    {
    case 0:
        r = C;
        g = X;
        b = 0;
        break;
    case 1:
        r = X;
        g = C;
        b = 0;
        break;
    case 2:
        r = 0;
        g = C;
        b = X;
        break;
    case 3:
        r = 0;
        g = X;
        b = C;
    case 4:
        r = X;
        g = 0;
        b = C;
        break;
    case 5:
        r = C;
        g = 0;
        b = X;
    default:
        r = 0;
        g = 0;
        b = 0;
        break;
    }

    r += m;
    g += m;
    b += m;

    return fromFloats( r, g, b );
}

inline Color operator*( float lhs, const Color& rhs ) noexcept
{
    return rhs * lhs;
}

inline Color min( const Color& c1, const Color& c2 )
{
    const auto r = math::min( c1.channels.r, c2.channels.r );
    const auto g = math::min( c1.channels.g, c2.channels.g );
    const auto b = math::min( c1.channels.b, c2.channels.b );
    const auto a = math::min( c1.channels.a, c2.channels.a );

    return { r, g, b, a };
}

inline Color max( const Color& c1, const Color& c2 )
{
    const auto r = math::max( c1.channels.r, c2.channels.r );
    const auto g = math::max( c1.channels.g, c2.channels.g );
    const auto b = math::max( c1.channels.b, c2.channels.b );
    const auto a = math::max( c1.channels.a, c2.channels.a );

    return { r, g, b, a };
}

}  // namespace graphics
}  // namespace rast