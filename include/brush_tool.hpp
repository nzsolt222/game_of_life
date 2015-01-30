#ifndef NZS_BRUSH_TOOL_HPP
#define NZS_BRUSH_TOOL_HPP

#include "position.hpp"
#include "cpp_features.hpp"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional>

namespace nzs
{

namespace gol
{

using Brush = std::vector<Position>;

// Brushs container
class BrushTool
{
public:

    // add an empty brush
    BrushTool();

    // add a new brush
    void add(Brush brush);

    // move to the next brush
    void next() NOEXCEPT;

    // move to the previous brush
    void previous() NOEXCEPT;

    // use the brush
    void use(int id) NOEXCEPT;

    // get the actual brush
    const Brush &get() const;

    // return the number of brushes
    inline std::size_t size() const NOEXCEPT
    {
        return brushs_.size();
    }

    // remove all brushes except the empty one
    inline void clear() NOEXCEPT
    {
        brushs_.clear();
        add({});
    }

    // load brushes from file
    static void load_from_file(const std::string &file_path, BrushTool &bt);

private:
    int brush_id_;
    std::vector<Brush> brushs_;

    inline bool good_id(int id) const NOEXCEPT
    {
        if (id >= 0 && static_cast<std::size_t>(id) < size())
        {
            return true;
        }

        return false;
    }
};

} // gol

} // nzs

#endif // NZS_BRUSH_TOOL_HPP
