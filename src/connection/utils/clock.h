#pragma once

#include <string>
#include <memory>
#include <mavlink/common/mavlink.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cmath>
#include <chrono>



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