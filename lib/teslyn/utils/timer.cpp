#include <iostream>
#include <iomanip>
#include <chrono>

#include "teslyn/utils/timer.hpp"

namespace Teslyn::time
{

Timer::Timer(std::string t_name) : m_name(t_name)
{
    m_start_time_point = std::chrono::high_resolution_clock::now();
}

Timer::~Timer()
{
    stop();
}

void Timer::stop()
{
    auto end_time_point = std::chrono::high_resolution_clock::now();

    auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_start_time_point).time_since_epoch().count();
    auto end = std::chrono::time_point_cast<std::chrono::microseconds>(end_time_point).time_since_epoch().count();

    auto duration = end - start;
    double ms = duration * 0.001;

    std::cout << std::setprecision(0) << m_name << " took " << duration << " us (" << ms << " ms)\n"
              << std::setprecision(6);
}
} // namespace Teslyn::time