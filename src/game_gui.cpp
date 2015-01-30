#include "game_gui.hpp"
#include "log.hpp"

#include <chrono>
#include <thread>

namespace nzs
{

namespace gol
{

GameGui::GameGui(std::size_t window_width, std::size_t window_height,
                 std::size_t row, std::size_t column, bool full_screen):
    game_table_(row, column),
    window_width_(window_width),
    window_height_(window_height),
    full_screen_(full_screen),
    wait_next_iter_(std::chrono::milliseconds(200)),
    call_next_iter_(false),
    first_left_click_is_alive_(false)
{
    BrushTool::load_from_file("./brushs.txt", brushs_);
    brushs_.use(1);
}

void GameGui::run()
{
    if (!init())
    {
        Log::error("initialization failed");
        return;
    }
    Log::debug("successful initialization");

    using frame_duration = std::chrono::duration<int, std::ratio<1, 60>>;
    while (!glfwWindowShouldClose(window_.get()))
    {
        auto start_time = std::chrono::high_resolution_clock::now();

        update();
        draw();

        auto end_time = start_time + frame_duration(1);
        std::this_thread::sleep_until(end_time);

        glfwSwapBuffers(window_.get());
        glfwPollEvents();
    }
}

bool GameGui::init()
{
    // create window
    window_ = nullptr; // destroy previous window if exists
    window_ = make_window(window_width_, window_height_, "Game of Life",
                          full_screen_ ? glfwGetPrimaryMonitor() : nullptr, nullptr);
    Log::verbose("window created");

    // init OpenGL
    glClearColor(1, 1, 1, 1);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, window_width_, window_height_);
    glOrtho(0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);

    return true;
}

void GameGui::update()
{
    // next iteration calculation
    static auto now = std::chrono::system_clock::now();
    if (std::chrono::system_clock::now() - now >= wait_next_iter_)
    {
        if (call_next_iter_)
        {
            game_table_.next();
            Log::verbose("generation:", game_table_.generation(),
                         "population:", game_table_.population());
        }
        now = std::chrono::system_clock::now();
    }

    // mouse handling
    int left_click = glfwGetMouseButton(window_.get(), GLFW_MOUSE_BUTTON_1);
    if (left_click == GLFW_PRESS)
    {
        const auto &brush = brushs_.get();
        Position index = mouse_to_index();
        for (const auto &pos_offset : brush)
        {
            if (first_left_click_is_alive_)
            {
                game_table_.born(index + pos_offset);
            }
            else
            {
                game_table_.kill(index + pos_offset);
            }
        }
    }

    // set the actual brush to an empty one
    int middle_click = glfwGetMouseButton(window_.get(), GLFW_MOUSE_BUTTON_3);
    if (middle_click == GLFW_PRESS)
    {
        brushs_.use(0);
    }

}

void GameGui::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // draw grid
    glColor4f(0.6, 0.6, 0.6, 0.3);
    details::draw_grid(game_table_.get_width(), game_table_.get_height());

    // draw alive cells
    glColor4f(0.6f, 1.f, 0.6f, 1.f);
    float width = 1.f / game_table_.get_width();
    float height = 1.f / game_table_.get_height();

    for (unsigned i = 0; i < game_table_.get_width(); ++i)
    {
        for (unsigned j = 0; j < game_table_.get_height(); ++j)
        {
            if (game_table_[i][j] == true)
            {
                details::draw_quad(i * width, j * height, width, height);
            }
        }
    }

    // draw the actual brush
    const auto &brush = brushs_.get();
    Position index = mouse_to_index();
    glColor4f(0.7, 1, 0.4, 0.5);
    for (const auto &pos_offset : brush)
    {
        auto tmp = index + pos_offset;
        details::draw_quad(tmp.get_x() * width, tmp.get_y() * height, width, height);
    }
}

void GameGui::mouse_button_callback(GLFWwindow *, int /*button*/, int action, int /*mods*/)
{
    if (action == GLFW_PRESS)
    {
        first_left_click_is_alive_ = !game_table_.is_alive(mouse_to_index());
    }
}

void GameGui::keyboard_callback(GLFWwindow *, int key, int, int action, int)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
    {
        Log::debug("exit");
        glfwSetWindowShouldClose(window_.get(), GL_TRUE);
    }
    if (key == GLFW_KEY_N && action == GLFW_RELEASE)
    {
        Log::debug("call next iteration");
        game_table_.next();
    }
    if (key == GLFW_KEY_R && action == GLFW_RELEASE)
    {
        Log::debug("clear the grid");
        game_table_.clear();
    }
    if (key == GLFW_KEY_B && action == GLFW_RELEASE)
    {
        Log::debug("boundary toogled");
        game_table_.toggle_boundary();
    }
    if (key == GLFW_KEY_F && action == GLFW_RELEASE)
    {
        full_screen_ = !full_screen_;
        Log::debug("full screen:", full_screen_ ? "true" : "false");
        init();
    }
    if (key == GLFW_KEY_KP_ADD && action == GLFW_RELEASE)
    {
        // speed up the iteration
        Log::debug("speed up the iteration");
        wait_next_iter_ -= std::chrono::milliseconds(25);
    }
    if (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_RELEASE)
    {
        // slow down the iteration
        Log::debug("slow down the iteration");
        wait_next_iter_ += std::chrono::milliseconds(25);
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
    {
        // play/stop the iteration
        Log::debug("playing:", call_next_iter_ ? "true" : "false");
        call_next_iter_ = ! call_next_iter_;
    }
}

void GameGui::scroll_callback(GLFWwindow *, double , double yoffset)
{
    // resize the grid
    if (glfwGetKey(window_.get(), GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        Log::debug("resize the grid");
        if (yoffset < 0)
        {
            game_table_.resize(game_table_.get_width() + 1, game_table_.get_height() + 1);
        }
        else
        {
            if (game_table_.get_width() < 2 || game_table_.get_height() < 2)
            {
                return;
            }
            game_table_.resize(game_table_.get_width() - 1, game_table_.get_height() - 1);
        }
    }
    // next/previous brush
    else
    {
        if (yoffset < 0)
        {
            Log::debug("use next brush");
            brushs_.next();
        }
        else
        {
            Log::debug("use previous brush");
            brushs_.previous();
        }
    }
}

void GameGui::frame_buffer_callback(GLFWwindow *, int width, int height)
{
    window_width_ = width;
    window_height_ = height;
    glViewport(0, 0, window_width_, window_height_);
}

Position GameGui::mouse_to_index() const
{
    double mouse_x, mouse_y;
    glfwGetCursorPos(window_.get(), &mouse_x, &mouse_y);

    double width = window_width_ / game_table_.get_width();
    double height = window_height_ / game_table_.get_height();

    Position index;
    index.set_x(mouse_x / width);
    index.set_y(mouse_y / height);

    if (index.get_x() >= (int)game_table_.get_width())
    {
        index.set_x(game_table_.get_width() - 1);
    }

    if (index.get_x() < 0)
    {
        index.set_x(0);
    }

    if (index.get_y() >= (int)game_table_.get_height())
    {
        index.set_y(game_table_.get_height() - 1);
    }

    if (index.get_y() < 0)
    {
        index.set_y(0);
    }
    return index;
}

GameGui::WindowUptr GameGui::make_window(int width, int height, const std::string &title,
        GLFWmonitor *monitor, GLFWwindow *share)
{
    auto creator = [ &, this]()
    {
        GLFWwindow *window = glfwCreateWindow(width, height, title.c_str(), monitor, share);
        if (window == nullptr)
        {
            Log::error("window failed to create");
            std::exit(-1);
        }
        else
        {
            glfwMakeContextCurrent(window);
            details::Event<GameGui>::subscribe_events(window, this);
        }

        return window;
    };

    auto deleter = [](GLFWwindow * window)
    {
        if (window != nullptr)
        {
            details::Event<GameGui>::unsubscribe_events(window);
            glfwDestroyWindow(window);
        }
    };

    return GameGui::WindowUptr(creator(), deleter);
}

} // gol

} // nzs
