#include "game_of_life.hpp"
#include "cpp_features.hpp"

namespace nzs
{

namespace gol
{

GameOfLife::GameOfLife(std::size_t width, std::size_t height) :
    width_(width),
    height_(height),
    generation_(0),
    population_(0),
    bounded_(false),
    grid_(width, row_type(height, false))
{
}

void GameOfLife::kill(const Position &pos)
{
    if (is_valid_position(pos) && is_alive(pos))
    {
        --population_;
        grid_[pos.get_x()][pos.get_y()] = false;
    }
}

void GameOfLife::born(const Position &pos)
{
    if (is_valid_position(pos) && !is_alive(pos))
    {
        ++population_;
        grid_[pos.get_x()][pos.get_y()] = true;
    }
}

void GameOfLife::flip(const Position &pos)
{
    if (!is_valid_position(pos))
    {
        return;
    }

    if (is_alive(pos))
    {
        kill(pos);
    }
    else
    {
        born(pos);
    }
}

void GameOfLife::next(std::size_t iteration)
{
    for (std::size_t i = 0; i < iteration; i++)
    {
        std::vector<Position> has_to_die;
        std::vector<Position> has_to_born;
        has_to_die.reserve(population_ / 2);
        has_to_born.reserve(population_ / 2);

        for (std::size_t x = 0; x < width_; ++x)
        {
            for (std::size_t y = 0; y < height_; ++y)
            {
                Position pos(x, y);
                auto alive_neigbors = get_alive_neighbors(pos);
                auto cell_is_alive  = is_alive(pos);
                if (cell_is_alive && (alive_neigbors < 2 || alive_neigbors > 3))
                {
                    has_to_die.push_back(pos);
                }
                else if (alive_neigbors == 3 && !cell_is_alive)
                {
                    has_to_born.push_back(pos);
                }
            }
        }

        for (const auto &cell : has_to_die)
        {
            kill(cell);
        }

        for (const auto &cell : has_to_born)
        {
            born(cell);
        }

        ++generation_;
    }
}

void GameOfLife::clear()
{
    grid_.clear();
    grid_.resize(width_, row_type(height_, false));
    generation_ = 0;
    population_ = 0;
}

void GameOfLife::resize(std::size_t width, std::size_t height)
{
    grid_type grid_tmp(width, row_type(height, false));
    auto min_w = std::min(width_, width);
    auto min_h = std::min(height_, height);

    population_ = 0;
    for (std::size_t i = 0; i < min_w; ++i)
    {
        for (std::size_t j = 0; j < min_h; ++j)
        {
            grid_tmp[i][j] = grid_[i][j];
            if (grid_tmp[i][j])
            {
                ++population_;
            }
        }
    }

    width_ = width;
    height_ = height;
    grid_ = std::move(grid_tmp);
}

bool GameOfLife::is_alive(const Position &pos) const
{
    return grid_.at(pos.get_x()).at(pos.get_y());
}

std::size_t GameOfLife::get_alive_neighbors(const Position &pos)
{
    static std::vector<Position> neighbor_offset =
    {
        { +0, -1},
        { +1, -1},
        { +1, +0},
        { +1, +1},
        { +0, +1},
        { -1, +1},
        { -1, +0},
        { -1, -1},
    };

    std::size_t alive_num = 0;
    for (const auto &offset : neighbor_offset)
    {
        auto tmp = pos + offset;
        if (!bounded_)
        {
            boundary_correction(tmp);
        }
        if (is_valid_position(tmp) && is_alive(tmp))
        {
            ++alive_num;
        }
    }

    return alive_num;
}

void GameOfLife::boundary_correction(Position &pos) NOEXCEPT
{
    if (pos.get_x() <= -1)
    {
        pos.set_x(width_ - 1);
    }
    if (pos.get_x() >= static_cast<int>(width_))
    {
        pos.set_x(0);
    }
    if (pos.get_y() <= -1)
    {
        pos.set_y(height_ - 1);
    }
    if (pos.get_y() >= static_cast<int>(height_))
    {
        pos.set_y(0);
    }
}

} // gol

} // nzs
