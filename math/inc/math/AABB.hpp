#pragma once

#include "Rect.hpp"

#include <glm/common.hpp>
#include <glm/vec2.hpp>

#include <limits>
#include <optional>

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

    AABB& expand(const glm::vec2& p) noexcept
    {
        min = glm::min( min, p );
        max = glm::max( max, p );

        return *this;
    }

    AABB& expand(const AABB& aabb) noexcept
    {
        min = glm::min( min, aabb.min );
        max = glm::max( max, aabb.max );

        return *this;
    }

    AABB& clamp(const AABB& aabb) noexcept
    {
        min = glm::max( min, aabb.min );
        max = glm::min( max, aabb.max );

        return *this;
    }

    AABB clamped(const AABB& aabb) const noexcept
    {
        return { glm::max( min, aabb.min ), glm::min( max, aabb.max ) };
    }

    bool intersect(const AABB& aabb) const noexcept
    {
        return glm::all( glm::lessThanEqual( min, aabb.max ) ) &&
               glm::all( glm::greaterThanEqual( max, aabb.min ) );
        //return min.x <= aabb.max.x && min.y <= aabb.max.y &&
        //       max.x >= aabb.min.x && max.y >= aabb.min.y;
    }

    bool intersect(const glm::vec2& p) const noexcept
    {
        return glm::all( glm::greaterThanEqual( p, min ) ) &&
               glm::all( glm::lessThanEqual( p, max ) );
        // return p.x >= min.x && p.y >= min.y && p.x <= max.x && p.y <= max.y;
    }

    std::optional<glm::vec2> overlap(const AABB& aabb) const noexcept
    {
        glm::vec2 overlap = glm::min( max, aabb.max ) - glm::max( min, aabb.min );
        if (overlap.x > 0.0f && overlap.y > 0.0f)
        {
            if (overlap.x < overlap.y)
            {
                return glm::vec2 { center().x < aabb.center().x ? max.x - aabb.min.x : min.x - aabb.max.x, 0.0f };
            }

            return glm::vec2 { 0.0f, center().y < aabb.center().y ? max.y - aabb.min.y : min.y - aabb.max.y };
        }

        return {};
    }

    glm::vec2 min { std::numeric_limits<float>::max() };
    glm::vec2 max { std::numeric_limits<float>::lowest() };
};
}  // namespace math
}  // namespace rast