#ifndef NZS_CALLBACK_SYSTEM_HPP
#define NZS_CALLBACK_SYSTEM_HPP

#include <GLFW/glfw3.h>

#include <map>
#include <vector>
#include <algorithm>

namespace nzs
{

namespace gol
{

namespace details
{

// events redirection to the subscribed class
template<class T>
class Event
{
public:
    using link_cont = std::map<GLFWwindow *, T *>;

    static inline void subscribe_events(GLFWwindow *window, T *c)
    {
        cont_[window] = c;
        glfwSetMouseButtonCallback(window, &Event::mouse_button_link);
        glfwSetKeyCallback(window, &Event::keyboard_link);
        glfwSetFramebufferSizeCallback(window, &Event::frame_buffer_link);
        glfwSetScrollCallback(window, &Event::scroll_link);
    }

    static inline void unsubscribe_events(GLFWwindow *window)
    {
        if (cont_[window] != nullptr)
        {
            cont_.erase(window);
        }
    }
private:

    static link_cont cont_;

    static inline void mouse_button_link(GLFWwindow *window, int button, int action, int mods)
    {
        auto subscriber_ptr = cont_[window];
        if (subscriber_ptr != nullptr)
        {
            subscriber_ptr->mouse_button_callback(window, button, action, mods);
        }
    }

    static void keyboard_link(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        auto subscriber_ptr = cont_[window];
        if (subscriber_ptr != nullptr)
        {
            subscriber_ptr->keyboard_callback(window, key, scancode, action, mods);
        }
    }

    static void frame_buffer_link(GLFWwindow *window, int width, int height)
    {
        auto subscriber_ptr = cont_[window];
        if (subscriber_ptr != nullptr)
        {
            subscriber_ptr->frame_buffer_callback(window, width, height);
        }
    }

    static void scroll_link(GLFWwindow *window, double xoffset, double yoffset)
    {
        auto subscriber_ptr = cont_[window];
        if (subscriber_ptr != nullptr)
        {
            subscriber_ptr->scroll_callback(window, xoffset, yoffset);
        }
    }

};

template<class T>
typename nzs::gol::details::Event<T>::link_cont nzs::gol::details::Event<T>::cont_;

} // details

} // gol

} // nzs

#endif // NZS_CALLBACK_SYSTEM_HPP
