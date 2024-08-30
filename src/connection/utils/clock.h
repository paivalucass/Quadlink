#pragma once

#include <string>
#include <memory>
#include <cmath>
#include <chrono>
#include "clock.h"


namespace quadlink{
    class Clock {
        public:
            Clock();

            void reset();

            double watch();

        private:
            std::chrono::_V2::system_clock::time_point start;
            std::chrono::_V2::system_clock::time_point end;
            std::chrono::duration<double> duration;
    };
}