#ifndef NZS_LOG_HPP
#define NZS_LOG_HPP

#include "cpp_features.hpp"

#include <string>
#include <ostream>
#include <iostream>
#include <map>

namespace Log
{

enum Level
{
    OFF = 0,
    ERROR,
    WARNING,
    DEBUG,
    VERBOSE
};

namespace details
{

#ifndef NZS_DISABLE_LOG
inline std::ostream &get_ostream()
{
    static std::ostream os(std::cerr.rdbuf());
    return os;
}

inline ::Log::Level &get_level()
{
    static ::Log::Level level = ::Log::Level::VERBOSE;
    return level;
}

inline std::map<std::string, std::string> &get_modifiers()
{
    static std::map<std::string, std::string> modifiers =
    {
        // Controls
        {"RESET_COLOR"      , "\033[0m"},
        {"CLEAR_SCREEN"     , "[H[2J"},
        {"CLEAR_LINE"       , "\r\033[0K"},

        // Prefixes
        {"ERROR_PREFIX"     , "[ ERROR ] "},
        {"WARNING_PREFIX"   , "[WARNING] "},
        {"DEBUG_PREFIX"     , "[ DEBUG ] "},
        {"VERBOSE_PREFIX"   , "[VERBOSE] "},
    };
    return modifiers;
}
#endif

inline void print() NOEXCEPT
{
}

template<typename T, typename... Args>
inline void print(T head, Args... args)
{
#ifndef NZS_DISABLE_LOG
    details::get_ostream() << head << " ";
    print(args...);
#else
    (void)head;
    print(args...);
#endif
}

} // details

inline void set_log(std::streambuf *sb)
{
#ifndef NZS_DISABLE_LOG
    details::get_ostream().rdbuf(sb);
#else
    (void)sb;
#endif
}

inline void set_level(Level level)
{
#ifndef NZS_DISABLE_LOG
    details::get_level() = level;
#else
    (void)level;
#endif
}

inline void set_color(bool enabled)
{
#if defined(_WIN32)
    enabled = false;
#endif

#ifndef NZS_DISABLE_LOG
    auto &modifiers = details::get_modifiers();
    if (enabled)
    {
        modifiers["ERROR_COLOR"] =   "\033[1;31m";
        modifiers["WARNING_COLOR"] = "\033[1;35m";
        modifiers["DEBUG_COLOR"] =   "\033[1;33m";
        modifiers["VERBOSE_COLOR"] = "\033[1;32m";
        modifiers["RESET_COLOR"] =   "\033[0m";
        modifiers["CLEAR_SCREEN"] =  "[H[2J";
        modifiers["CLEAR_LINE"] =    "\r\033[0K";

    }
    else
    {
        modifiers["ERROR_COLOR"] =    "";
        modifiers["WARNING_COLOR"] =    "";
        modifiers["DEBUG_COLOR"] =    "";
        modifiers["VERBOSE_COLOR"] =  "";
        modifiers["RESET_COLOR"] = "";
        modifiers["CLEAR_SCREEN"] =  "";
        modifiers["CLEAR_LINE"] =    "";
    }
#else
    (void)enabled;
#endif
}

inline void clear_line()
{
#ifndef NZS_DISABLE_LOG
    auto &modifiers = details::get_modifiers();
    details::get_ostream() << modifiers["CLEAR_LINE"];
#endif
}

template<typename... Args>
inline void error(Args... args) NOEXCEPT // LEVEL 1
{
#ifndef NZS_DISABLE_LOG
    if (details::get_level() < Level::ERROR)
    {
        return;
    }

    auto &modifiers = details::get_modifiers();
    details::get_ostream() << modifiers["ERROR_PREFIX"] << modifiers["ERROR_COLOR"];
    details::print(args...);
    details::get_ostream() << modifiers["RESET_COLOR"] << std::endl;
#else
    details::print(args...);
#endif
}

template<typename... Args>
inline void warning(Args... args) NOEXCEPT // LEVEL 2
{
#ifndef NZS_DISABLE_LOG
    if (details::get_level() < Level::WARNING)
    {
        return;
    }

    auto &modifiers = details::get_modifiers();
    details::get_ostream() << modifiers["WARNING_COLOR"] << modifiers["WARNING_PREFIX"];
    details::print(args...);
    details::get_ostream() << modifiers["RESET_COLOR"] << std::endl;
#else
    details::print(args...);
#endif
}

template<typename... Args>
inline void debug(Args... args) NOEXCEPT // LEVEL 3
{
#ifndef NZS_DISABLE_LOG
    if (details::get_level() < Level::DEBUG)
    {
        return;
    }

    auto &modifiers = details::get_modifiers();
    details::get_ostream() << modifiers["DEBUG_COLOR"] << modifiers["DEBUG_PREFIX"];
    details::print(args...);
    details::get_ostream() << modifiers["RESET_COLOR"] << std::endl;
#else
    details::print(args...);
#endif
}

template<typename... Args>
inline void verbose(Args... args) NOEXCEPT // LEVEL 4
{
#ifndef NZS_DISABLE_LOG
    if (details::get_level() < Level::VERBOSE)
    {
        return;
    }

    auto &modifiers = details::get_modifiers();
    details::get_ostream() << modifiers["VERBOSE_COLOR"] << modifiers["VERBOSE_PREFIX"];
    details::print(args...);
    details::get_ostream() << details::get_modifiers()["RESET_COLOR"] << std::endl;
#else
    details::print(args...);
#endif
}

inline void init(int argc, const char *argv[])
{
    (void)argc;
    (void)argv;
#ifndef NZS_DISABLE_LOG
    set_color(true);
#endif
}

} // log

#endif // NZS_LOG_HPP
