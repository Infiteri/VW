#pragma once

#include "Base.h"
#include <string>
#include <unordered_map>

namespace VW
{
    enum class LogColor
    {
        White,
        Blue,
        Green,
        Yellow,
        Red
    };

    enum class LogLevel
    {
        Debug,
        Info,
        Warn,
        Error
    };

    struct LogCategory
    {
        std::string Name, Prefix;
    };

    class VW_API Logger
    {
    public:
        struct Settings
        {
            bool FancyFormat = false;
            std::string Format = "[PREFIX LEVEL]: MSG";
        };

        struct State
        {
            std::unordered_map<std::string, LogCategory> Categories;
            std::unordered_map<LogLevel, LogColor> ColorLevelMap;
            struct Settings Settings;
        };

    public:
        Logger() = delete;
        ~Logger() = delete;

        static void Init();
        static void Shutdown();

        static Settings &GetSettings();

        static void AddCategory(const LogCategory &category);

        static void Log(LogLevel level, const std::string &category, const char *fmt, ...);
    };
} // namespace VW

#define VW_LOG_ADD_CATEGORY(name, prefix) VW::Logger::AddCategory({.Name = name, .Prefix = prefix});
#define VW_LOG(lvl, cat, msg, ...) VW::Logger::Log(VW::LogLevel::lvl, cat, msg, ##__VA_ARGS__)
#define VW_DEBUG(cat, msg, ...) VW_LOG(Debug, cat, msg, ##__VA_ARGS__)
#define VW_INFO(cat, msg, ...) VW_LOG(Info, cat, msg, ##__VA_ARGS__)
#define VW_WARN(cat, msg, ...) VW_LOG(Warn, cat, msg, ##__VA_ARGS__)
#define VW_ERROR(cat, msg, ...) VW_LOG(Error, cat, msg, ##__VA_ARGS__)
