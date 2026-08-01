#pragma once
#include <string>
enum class LogLevel
{
    Trace,
    Debug,
    Info,
    Warn,
    Error,
    Fatal
};

class Logger 
{
   public:
    static void log(LogLevel level, const std::string& message);
    static void debug(const std::string&message);
    static void info(const std::string&message);
    static void warn(const std::string&message );
    static void error(const std::string& message);
    static void fatal(const std::string & message);
    static void trace(const std::string& message);
  private:
     static std::string level_to_string(LogLevel level);
     static std::string current_time();
};