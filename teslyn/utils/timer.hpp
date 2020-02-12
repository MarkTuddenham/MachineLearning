#pragma once

#include <iostream>
#include <iomanip>
#include <chrono>

namespace Teslyn::time
{

class Timer
{
public:
    Timer(std::string);
    ~Timer();
    void stop();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start_time_point;
    std::string m_name;
};

} // namespace Teslyn::time