#ifndef NZS_GAME_GUI_HPP
#define NZS_GAME_GUI_HPP

#include "game_of_life.hpp"
#include "draw_function.hpp"
#include "brush_tool.hpp"
#include "callback_system.hpp"

#include <GLFW/glfw3.h>

#include <cstddef>
#include <memory>
#include <iostream>
#include <string>
#include <chrono>

namespace nzs
{

namespace gol
{

class GameGui
{
public:
    GameGui(std::size_t window_width, std::size_t window_height,
            std::size_t row, std::size_t column, bool full_screen);

    // start the simulation
    void run();

private:
    using WindowUptr = std::unique_ptr<GLFWwindow, std::function<void(GLFWwindow *)> >;
    WindowUptr window_;
    GameOfLife game_table_;
    BrushTool brushs_;
    double window_width_;
    double window_height_;
    bool full_screen_;
    std::chrono::milliseconds wait_next_iter_;
    bool call_next_iter_;
    bool first_left_click_is_alive_;
    friend class details::Event<GameGui>;

    bool init();
    void update();
    void draw();

    void mouse_button_callback(GLFWwindow *, int button, int action, int mods);
    void keyboard_callback(GLFWwindow *, int key, int, int action, int);
    void scroll_callback(GLFWwindow *, double , double yoffset);
    void frame_buffer_callback(GLFWwindow *, int width, int height);

    // convert mouse position to grid coordinate
    Position mouse_to_index() const;

    // create a window
    WindowUptr make_window(int width, int height, const std::string &title,
                           GLFWmonitor *monitor, GLFWwindow *share);
};

} // gol

} // nzs

#endif // NZS_GAME_GUI_HPP
