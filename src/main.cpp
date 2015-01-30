#include "game_gui.hpp"
#include "log.hpp"

#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

std::size_t WINDOW_WIDTH = 1280;
std::size_t WINDOW_HEIGHT = 720;
std::size_t ROW = 80;
std::size_t COLUMN = 48;
bool IS_FULL_SCREEN = false;

class initGLFW
{
public:
    initGLFW()
    {
        glfwSetErrorCallback([](int error, const char *description)
        {
            Log::error("code:", error, "description:", description);
        });

        if (!glfwInit())
        {
            Log::error("GLFW failed to initialize");
            std::exit(EXIT_FAILURE);
        }
        Log::debug("GLFW initialized");
    }

    ~initGLFW()
    {
        Log::debug("GLFW terminated");
        glfwTerminate();
    }

};

int string_to_int(const std::string &number)
{
    static std::stringstream convert;
    convert << number;
    int return_value;
    if (!(convert >> return_value))
    {
        Log::error("cannot convert", number, "to int");
        return_value = -1;
    }
    convert.str("");
    convert.clear();
    return return_value;
}

void fetch_value(const std::string &number, std::size_t &value)
{
    int tmp = string_to_int(number);
    if (tmp == -1)
    {
        Log::warning("Invalid parameter value:", number);
    }
    else
    {
        value = tmp;
    }
}

void parseCLA(int argc, const char *argv[])
{
    std::vector<std::string> args(argv + 1, argv + argc);

    for (std::size_t i = 0; i < args.size(); ++i)
    {
        if (args[i] == "--help")
        {
            std::cout << "USAGE: " + std::string(argv[0])
                      << " [-w|--width ARG] [-h|--height ARG] [-r|--row ARG]"
                      << " [-c|--column ARG] [-f|--fullscreen 0|1|false|true] [--help]" << std::endl;

            std::cout << std::endl << "Option Descriptions" << std::endl << std::endl;

            std::cout << std::left;
            std::cout << std::setw(15) << "\t-w [ --width ]"  << "\t\t"  << "Set the window width." << std::endl;
            std::cout << std::setw(15) << "\t-h [ --height ]" << "\t\t"  << "Set the window height." << std::endl;
            std::cout << std::setw(15) << "\t-r [ --row ]"    << "\t\t"  << "Set the number of rows." << std::endl;
            std::cout << std::setw(15) << "\t-c [ --column ]" << "\t\t" << "Set the number of columns." << std::endl;
            std::cout << std::setw(15) << "\t--help"         << "\t\t"   << "Print this message and exit." << std::endl;
            std::exit(EXIT_SUCCESS);
        }
        // && ++i < args.size() = next argument is exist?
        else if ((args[i] == "-w" || args[i] == "--width") && ++i < args.size())
        {
            fetch_value(args[i], WINDOW_WIDTH);
            Log::verbose("width set to:", WINDOW_WIDTH);
        }
        else if ((args[i] == "-h" || args[i] == "--height") && ++i < args.size())
        {
            fetch_value(args[i], WINDOW_HEIGHT);
            Log::verbose("height set to:", WINDOW_HEIGHT);
        }
        else if ((args[i] == "-r" || args[i] == "--row") && ++i < args.size())
        {
            fetch_value(args[i], ROW);
            Log::verbose("row set to:", ROW);
        }
        else if ((args[i] == "-c" || args[i] == "--column") && ++i < args.size())
        {
            fetch_value(args[i], COLUMN);
            Log::verbose("column set to:", COLUMN);
        }
        else if ((args[i] == "-f" || args[i] == "--fullscreen") && ++i < args.size())
        {
            int is_fullscreen = string_to_int(args[i]);
            if (is_fullscreen == 0 || args[i] == "false")
            {
                IS_FULL_SCREEN = false;
            }
            else if (is_fullscreen == 1 || args[i] == "true")
            {
                IS_FULL_SCREEN = true;
            }
            else
            {
                Log::warning("Invalid is_fullscreen:", args[i]);
            }
        }
        else
        {
            Log::warning("Invalid parameter:", args[i]);
        }
    }
}

int main(int argc, char const *argv[])
{
    Log::init(argc, argv);
    parseCLA(argc, argv);
    initGLFW raii;

    nzs::gol::GameGui game {WINDOW_WIDTH, WINDOW_HEIGHT, ROW, COLUMN, IS_FULL_SCREEN};
    game.run();

    return EXIT_SUCCESS;
}
