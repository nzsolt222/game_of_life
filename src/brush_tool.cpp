#include "brush_tool.hpp"
#include "log.hpp"
#include "cpp_features.hpp"

#include <locale>
#include <string>

namespace nzs
{

namespace gol
{

BrushTool::BrushTool() :
    brush_id_(0)
{
    add({});
}

void BrushTool::add(Brush brush)
{
    brushs_.push_back(std::move(brush));
    Log::verbose("new brush added");
}

const Brush &BrushTool::get() const
{
    return brushs_[brush_id_];
}

void BrushTool::next() NOEXCEPT
{
    ++brush_id_;
    if (!good_id(brush_id_))
    {
        brush_id_ = 0;
    }
}

void BrushTool::previous() NOEXCEPT
{
    --brush_id_;
    if (!good_id(brush_id_))
    {
        brush_id_ = size() - 1;
    }
}

void BrushTool::use(int id) NOEXCEPT
{
    if (good_id(id))
    {
        brush_id_ = id;
    }
}

// read brush offset coordinates
bool read_offset(const std::string &line, int &offset_x, int &offset_y)
{
    static std::istringstream ss;
    ss.clear();
    ss.str(line);
    return (ss >> offset_x >> offset_y && !ss.fail());
}

void BrushTool::load_from_file(const std::string &file_path, BrushTool &bt)
{
    std::ifstream file;
    file.open(file_path);
    if (!file.is_open())
    {
        Log::error("file not found: " + file_path);
        return;
    }

    std::string line;
    Brush actual_brush;
    int offset_x, offset_y;
    while (!file.eof())
    {
        std::getline(file, line);

        // ignore empty line or comment
        if (line.size() == 0 || line[0] == '#')
        {
            continue;
        }
        // add the brush and create an empty brush
        else if (line == "end")
        {
            bt.add(std::move(actual_brush));
            actual_brush.clear();
        }
        // read the x,y offset and add to the brush
        else if (read_offset(line, offset_x, offset_y))
        {
            actual_brush.push_back({offset_x, offset_y});
        }
        // error
        else
        {
            Log::warning("bad format: " + line);
        }
    }
    Log::debug("bursh file loaded: " + file_path);
}

} // gol

} // nzs
