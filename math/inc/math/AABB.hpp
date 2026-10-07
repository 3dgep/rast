#pragma once

#include "Rect.hpp"

#include <glm/common.hpp>
#include <glm/vec2.hpp>

#include <limits>

namespace rast
{
inline namespace math
{
struct AABB
{
    AABB() = default;

    AABB( const glm::vec2& a, const glm::vec2& b ) noexcept
    {
        min = glm::min( a, b );
        max = glm::max( a, b );
    }

    AABB( const glm::vec2& a, const glm::vec2& b, const glm::vec2& c ) noexcept
    {
        min = glm::min( a, glm::min( b, c ) );
        max = glm::max( a, glm::max( b, c ) );
    }

    AABB( const glm::vec2& a, const glm::vec2& b, const glm::vec2& c, const glm::vec2& d ) noexcept
    {
        min = glm::min( glm::min(a, b), glm::min( c, d ) );
        max = glm::max( glm::max(a, b), glm::max( c, d ) );
    }

    template<typename T>
    AABB(const Rect<T>& rect)
    {
        min = glm::vec2 { rect.left, rect.top };
        max = glm::vec2 { rect.left + rect.width, rect.top + rect.height };
    }

    AABB operator+( const glm::vec2& rhs ) const noexcept
    {
        return { min + rhs, max + rhs };
    }

    AABB& operator+=( const glm::vec2& rhs ) noexcept
    {
        min += rhs;
        max += rhs;

        return *this;
    }

    AABB operator-( const glm::vec2& rhs ) const noexcept
    {
        return { min - rhs, max - rhs };
    }

    AABB& operator-=( const glm::vec2& rhs ) noexcept
    {
        min -= rhs;
        max -= rhs;

        return *this;
    }

    float left() const noexcept
    {
        return min.x;
    }

    float right() const noexcept
    {
        return max.x;
    }

    float top() const noexcept
    {
        return min.y;
    }

    float bottom() const noexcept
    {
        return max.y;
    }

    glm::vec2 center() const noexcept
    {
        return ( min + max ) * 0.5f;
    }

    float width() const noexcept
    {
        return max.x - min.x;
    }

    float height() const noexcept
    {
        return max.y - min.y;
    }

    bool isValid() const noexcept
    {
        return glm::all( glm::lessThanEqual( min, max ) );
        // return min.x <= max.x && min.y <= max.y;
    }

    glm::vec2 min { std::numeric_limits<float>::max() };
    glm::vec2 max { std::numeric_limits<float>::lowest() };
};
}  // namespace math
}  // namespace rast