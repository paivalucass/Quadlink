#include "clock.h"


namespace quadlink{
    quadlink::Clock::Clock(){}

    void quadlink::Clock::reset(){
        quadlink::Clock::start = std::chrono::high_resolution_clock::now();
    }

    double quadlink::Clock::watch(){
        quadlink::Clock::end = std::chrono::high_resolution_clock::now();
        quadlink::Clock::duration = quadlink::Clock::end - quadlink::Clock::start;
        return quadlink::Clock::duration.count();
    }
}








