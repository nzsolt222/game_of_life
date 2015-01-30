#ifndef NZS_POSITION_HPP
#define NZS_POSITION_HPP

#include "cpp_features.hpp"

#include <iostream>

namespace nzs
{

namespace gol
{

class Position
{
public:
    Position() = default;

    inline Position(int x, int y) :
        x_(x),
        y_(y)
    {
    }

    inline void set_x(int x) NOEXCEPT
    {
        x_ = x;
    }

    inline void set_y(int y) NOEXCEPT
    {
        y_ = y;
    }

    inline int get_x() const NOEXCEPT
    {
        return x_;
    }

    inline int get_y() const NOEXCEPT
    {
        return y_;
    }

    inline Position &operator+=(const Position &other) NOEXCEPT
    {
        x_ += other.x_;
        y_ += other.y_;
        return *this;
    }
private:
    int x_;
    int y_;
};

inline bool operator==(const Position &lhs, const Position &rhs) NOEXCEPT
{
    return lhs.get_x() == rhs.get_x() && lhs.get_y() == rhs.get_y();
}

inline bool operator!=(const Position &lhs, const Position &rhs) NOEXCEPT
{
    return !operator==(lhs, rhs);
}

inline Position operator+(Position lhs, const Position &rhs) NOEXCEPT
{
    lhs += rhs;
    return lhs;
}

} // gol

} // nzs

#endif // NZS_POSITION_HPP
