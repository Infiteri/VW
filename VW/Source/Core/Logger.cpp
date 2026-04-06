#include "Logger.h"
#include <cstdarg>
#include <iostream>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

namespace VW
{
    static Logger::State s_State;
    static const char *s_LevelStrings[] = {"Debug", "Info", "Warn", "Error"};

    void Logger::Init()
    {
#ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE)
        {
            DWORD dwMode = 0;
            if (GetConsoleMode(hOut, &dwMode))
            {
                dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(hOut, dwMode);
            }
        }
#endif
    }

    void Logger::Shutdown()
    {
    }

    Logger::Settings &Logger::GetSettings()
    {
        return s_State.Settings;
    }

    static void Output(LogColor color, const char *msg)
    {
        struct RGB
        {
            int r, g, b;
        };
        RGB r;

        switch (color)

        {
        case LogColor::Blue:
            r = {0, 125, 255};
            break;
        case LogColor::Green:
            r = {0, 255, 0};
            break;
        case LogColor::Yellow:
            r = {255, 255, 0};
            break;
        case LogColor::Red:
            r = {255, 0, 0};
            break;
        case LogColor::White:
            r = {255, 255, 255};
            break;
        default:
            r = {0, 0, 0};
            break;
        }
        std::cout << "\033[38;2;" << r.r << ";" << r.g << ";" << r.b << "m";
        std::cout << msg << "\033[0m";
    }

    void Logger::AddCategory(const LogCategory &category)
    {
        s_State.Categories[category.Name] = category;
    }

    void Logger::Log(LogLevel level, const std::string &category, const char *fmt, ...)
    {
#define LOG_MSG_SIZE 10000
#define LOG_FINAL_SIZE (LOG_MSG_SIZE + 129)

        // * avoids a copy
        LogCategory *cat = s_State.Categories.find(category) == s_State.Categories.end()
                               ? nullptr
                               : &s_State.Categories[category];

        const char *strLevel = s_LevelStrings[(int)level];

        char message[LOG_MSG_SIZE];
        va_list args;
        va_start(args, fmt);
        std::vsnprintf(message, LOG_MSG_SIZE, fmt, args);
        va_end(args);

        char finalMsg[LOG_FINAL_SIZE];

        if (!s_State.Settings.FancyFormat)
        {
            std::snprintf(finalMsg, LOG_FINAL_SIZE, "[%s%s%s]: %s\n",
                          cat ? cat->Prefix.c_str() : "", cat ? " " : "", strLevel, message);
        }
        else
        {
            std::string format = s_State.Settings.Format;

            auto prefixPos = format.find("PREFIX");
            if (prefixPos != std::string::npos)
            {
                format.replace(prefixPos, 6, cat->Prefix);
            }

            auto levelPos = format.find("LEVEL");
            if (levelPos != std::string::npos)
            {
                format.replace(levelPos, 5, strLevel);
            }

            auto msgPos = format.find("MSG");
            if (msgPos != std::string::npos)
            {
                format.replace(msgPos, 3, message);
            }
            std::snprintf(finalMsg, format.size() + 1, "%s\n", format.c_str());
        }

        Output(s_State.ColorLevelMap[level], finalMsg);
    }
} // namespace VW
