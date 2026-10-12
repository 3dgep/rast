#pragma once

#include "Color.hpp"

namespace rast
{
inline namespace graphics
{
enum class BlendFactor : uint8_t
{
    Zero,  // Multiply the operand by 0.
    One,   // Multiply the operand by 1.
    SrcColor,
    OneMinusSrcColor,
    DstColor,
    OneMinusDstColor,
    SrcAlpha,
    OneMinusSrcAlpha,
    DstAlpha,
    OneMinusDstAlpha,
    SrcAlphaSat,  // Multiply the operand by min( As, 1 - Ad ) before applying the blend operation.
};

enum class BlendOperation : uint8_t
{
    Add,              // s + d
    Subtract,         // s - d
    ReverseSubtract,  // d - s
    Min,
    Max,
};

struct BlendMode
{
    bool blendEnable = false;

    uint8_t alphaThreshold = 0;

    BlendFactor srcFactor = BlendFactor::One;

    BlendFactor dstFactor = BlendFactor::Zero;

    BlendOperation blendOp = BlendOperation::Add;

    BlendFactor srcAlphaFactor = BlendFactor::One;

    BlendFactor dstAlphaFactor = BlendFactor::Zero;

    BlendOperation alphaOp = BlendOperation::Add;

    constexpr explicit BlendMode( bool           blendEnable    = false,
                                  uint8_t        alphaThreshold = 0,
                                  BlendFactor    srcFactor      = BlendFactor::One,
                                  BlendFactor    dstFactor      = BlendFactor::Zero,
                                  BlendOperation blendOp        = BlendOperation::Add,
                                  BlendFactor    srcAlphaFactor = BlendFactor::One,
                                  BlendFactor    dstAlphaFactor = BlendFactor::Zero,
                                  BlendOperation alphaOp        = BlendOperation::Add )
    : blendEnable { blendEnable }
    , alphaThreshold { alphaThreshold }
    , srcFactor { srcFactor }
    , dstFactor { dstFactor }
    , blendOp { blendOp }
    , srcAlphaFactor { srcAlphaFactor }
    , dstAlphaFactor { dstAlphaFactor }
    , alphaOp { alphaOp }
    {}

    constexpr Color blend( Color srcColor, Color dstColor ) const noexcept;
};

constexpr Color ComputeBlendFactor( const Color& src, const Color& dst, BlendFactor blendFactor )
{
    switch ( blendFactor )
    {
    case BlendFactor::Zero:
        return Color::Black;
    case BlendFactor::One:
        return Color::White;
    case BlendFactor::SrcColor:
        return src;
    case BlendFactor::OneMinusSrcColor:
        return Color::White - src;
    case BlendFactor::DstColor:
        return dst;
    case BlendFactor::OneMinusDstColor:
        return Color::White - dst;
    case BlendFactor::SrcAlpha:
        return { src.channels.a, src.channels.a, src.channels.a, src.channels.a };
    case BlendFactor::OneMinusSrcAlpha:
        return { 255 - src.channels.a, 255 - src.channels.a, 255 - src.channels.a, 255 - src.channels.a };
    case BlendFactor::DstAlpha:
        return { dst.channels.a, dst.channels.a, dst.channels.a, dst.channels.a };
    case BlendFactor::OneMinusDstAlpha:
        return { 255 - dst.channels.a, 255 - dst.channels.a, 255 - dst.channels.a, 255 - dst.channels.a };
    case BlendFactor::SrcAlphaSat:
        return min( Color { src.channels.a, src.channels.a, src.channels.a, src.channels.a }, Color { 255 - dst.channels.a, 255 - dst.channels.a, 255 - dst.channels.a, 255 - dst.channels.a } );
    }

    return src;
}

constexpr uint8_t ComputeBlendFactor( uint8_t sA, uint8_t dA, BlendFactor blendFactor ) noexcept
{
    switch ( blendFactor )
    {
    case BlendFactor::Zero:
        return 0;
    case BlendFactor::One:
        return 255;
    case BlendFactor::SrcColor:
        return sA;
    case BlendFactor::OneMinusSrcColor:
        return 255 - sA;
    case BlendFactor::DstColor:
        return dA;
    case BlendFactor::OneMinusDstColor:
        return 255 - dA;
    case BlendFactor::SrcAlpha:
        return sA;
    case BlendFactor::OneMinusSrcAlpha:
        return 255 - sA;
    case BlendFactor::DstAlpha:
        return dA;
    case BlendFactor::OneMinusDstAlpha:
        return 255 - dA;
    case BlendFactor::SrcAlphaSat:
        return math::min<uint8_t>( sA, 255 - dA );
    }

    return sA;
}

constexpr Color ComputeBlendOp(const Color& src, const Color& dst, BlendOperation op) noexcept
{
    switch (op)
    {
    case BlendOperation::Add:
        return src + dst;
    case BlendOperation::Subtract:
        return src - dst;
    case BlendOperation::ReverseSubtract:
        return dst - src;
    case BlendOperation::Min:
        return min( src, dst );
    case BlendOperation::Max:
        return max( src, dst );
    }

    return src;
}

constexpr uint8_t ComputeBlendOp( uint8_t sA, uint8_t dA, BlendOperation op ) noexcept
{
    switch ( op )
    {
    case BlendOperation::Add:
        return sA + dA;
    case BlendOperation::Subtract:
        return sA - dA;
    case BlendOperation::ReverseSubtract:
        return dA - sA;
    case BlendOperation::Min:
        return sA < dA ? sA : dA;
    case BlendOperation::Max:
        return sA > dA ? sA : dA;
    }

    return sA;
}

constexpr Color BlendMode::blend( Color srcColor, Color dstColor ) const noexcept
{
    if ( !blendEnable )
        return srcColor;

    if ( srcColor.channels.a < alphaThreshold )
        return dstColor;

    const Color   sRGB = ComputeBlendFactor( srcColor, dstColor, srcFactor ) * srcColor;
    const Color   dRGB = ComputeBlendFactor( srcColor, dstColor, dstFactor ) * dstColor;
    const uint8_t sA   = static_cast<uint8_t>( ComputeBlendFactor( srcColor.channels.a, dstColor.channels.a, srcAlphaFactor ) * srcColor.channels.a / 255 );
    const uint8_t dA   = static_cast<uint8_t>( ComputeBlendFactor( srcColor.channels.a, dstColor.channels.a, dstAlphaFactor ) * dstColor.channels.a / 255 );

    const Color RGB = ComputeBlendOp( sRGB, dRGB, blendOp );
    const uint8_t A   = ComputeBlendOp( sA, dA, alphaOp );

    return { RGB.channels.r, RGB.channels.g, RGB.channels.b, A };
}

}  // namespace graphics
}  // namespace rast