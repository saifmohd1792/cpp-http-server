#include "logger.hpp"
#include<chrono>
#include<ctime>
#include<iomanip>
#include<iostream>
#include<sstream>

std::string Logger::level_to_string(LogLevel level)
{
    switch(level) 
    {
      case LogLevel::Trace:
         return "TRACE";
      case LogLevel::Debug:
         return "DEBUG";
      case LogLevel::Info:
         return "INFO";
      case LogLevel::Warn:
          return "WARN";
      case LogLevel::Error:
          return "ERROR";
       case LogLevel::Fatal:
          return "FATAL";

    }
    return "UNKNOWN";
}
std::string Logger::current_time()
{
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm local_time{};
#ifdef _WIN32
   localtime_s(&local_time,&time);
#else 
   localtime_r(&time,&local_time);
#endif 
   std::ostringstream stream;
   stream<<std::put_time(&local_time,"%H:%M:%S");
   return stream.str();

}

void Logger::trace(const std::string& message)
{
    log(LogLevel::Trace, message);
}

void Logger::debug(const std::string& message)
{
    log(LogLevel::Debug, message);
}

void Logger::info(const std::string& message)
{
    log(LogLevel::Info, message);
}

void Logger::warn(const std::string& message)
{
    log(LogLevel::Warn, message);
}

void Logger::error(const std::string& message)
{
    log(LogLevel::Error, message);
}

void Logger::fatal(const std::string& message)
{
    log(LogLevel::Fatal, message);
}


void Logger::log(LogLevel level, const std::string& message)
{
    std::cout
        << "[" << current_time() << "] "
        << "[" << level_to_string(level) << "] "
        << message
        << '\n';
}