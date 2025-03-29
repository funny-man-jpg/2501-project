#include <GLFW/glfw3.h>
#include <iostream>

#include "timer.h"

namespace game {

Timer::Timer(void)
{
    // set the ending time of the timer to 0 by default
    end_time = 0;
}


Timer::~Timer(void)
{
}


void Timer::Start(float end_time)
{
    double current_time = glfwGetTime();

    // determine the end time for the timer
    this->end_time = current_time + end_time;
}


bool Timer::Finished(void) const
{
    // check if the current time is greater than the end time of the timer and if so, the timer is finished
    if (glfwGetTime() > end_time) {
        return true;
    }

    return false;
}

} // namespace game
