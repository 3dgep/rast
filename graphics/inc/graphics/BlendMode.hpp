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
    Add,        // s + d
    Subtract,   // s - d
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
};
}  // namespace graphics
}  // namespace rast