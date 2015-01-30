#ifndef NZS_DRAW_FUNCTION_HPP
#define NZS_DRAW_FUNCTION_HPP

#include <GLFW/glfw3.h>
#include <iostream>

namespace nzs
{

namespace gol
{

namespace details
{

inline void draw_quad(float x, float y, float width, float height)
{
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

inline void draw_grid(int row, int column)
{
    float grid_width = 1.f / row;
    float grid_height = 1.f / column;

    glBegin(GL_LINES);
    for (int i = 0; i < column; ++i)
    {
        glVertex2f(0, i * grid_height);
        glVertex2f(1.f, i * grid_height);
    }
    glEnd();

    glBegin(GL_LINES);
    for (int i = 0; i < row; ++i)
    {
        glVertex2f(i * grid_width, 0);
        glVertex2f(i * grid_width, 1.f);
    }
    glEnd();
}

} // details

} // gol

} // nzs


#endif // NZS_DRAW_FUNCTION_HPP
