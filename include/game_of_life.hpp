#ifndef NZS_GAME_OF_LIFE_HPP
#define NZS_GAME_OF_LIFE_HPP

#include "position.hpp"
#include "cpp_features.hpp"

#include <cstddef>
#include <vector>
#include <algorithm>

namespace nzs
{

namespace gol
{

class GameOfLife
{
public:
    using row_type = std::vector<bool>;
    using grid_type = std::vector<row_type>;

    // create a width_X_height size grid where every cells is dead
    GameOfLife(std::size_t width, std::size_t height);

    // kill the cell
    void kill(const Position &pos);

    // mark the cell alive
    void born(const Position &pos);

    // flip the life
    void flip(const Position &pos);

    // calculate the next iteration
    void next(std::size_t iteration = 1);

    // kill all cells
    void clear();

    void resize(std::size_t width, std::size_t height);

    bool is_alive(const Position &pos) const;

    inline std::size_t get_width() const NOEXCEPT
    {
        return width_;
    }

    inline std::size_t get_height() const NOEXCEPT
    {
        return height_;
    }

    inline std::size_t generation() const NOEXCEPT
    {
        return generation_;
    }

    inline std::size_t population() const NOEXCEPT
    {
        return population_;
    }

    inline void toggle_boundary() NOEXCEPT
    {
        bounded_ = !bounded_;
    }

    inline bool is_bounded() const NOEXCEPT
    {
        return bounded_;
    }

    inline const row_type &operator[](int index) const
    {
        return grid_[index];
    }

private:
    std::size_t width_;
    std::size_t height_;
    std::size_t generation_;
    std::size_t population_;
    bool bounded_;
    grid_type grid_;

    std::size_t get_alive_neighbors(const Position &pos);
    void boundary_correction(Position &pos) NOEXCEPT;

    bool is_valid_position(const Position &pos) const NOEXCEPT
    {
        if (pos.get_x() < 0 || pos.get_y() < 0 ||
                static_cast<std::size_t>(pos.get_x()) >= width_ ||
                static_cast<std::size_t>(pos.get_y()) >= height_)
        {
            return false;
        }
        return true;
    }
};

} // gol

} // nzs

#endif // NZS_GAME_OF_LIFE_HPP
