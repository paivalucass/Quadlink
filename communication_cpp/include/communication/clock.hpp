#pragma once

#include <chrono>


namespace rcpilot{
    class Clock {
        public:
            Clock();

            void reset();

            double elapsed() const;

        private:
        std::chrono::high_resolution_clock::time_point start_time;
    };
}