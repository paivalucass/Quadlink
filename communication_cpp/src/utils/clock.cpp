#include <communication/clock.hpp>

namespace rcpilot {
rcpilot::Clock::Clock() {
    reset();
}

void rcpilot::Clock::reset() {
    start_time = std::chrono::high_resolution_clock::now();
}

double rcpilot::Clock::elapsed() const {
    using namespace std::chrono;
    return duration<double>(high_resolution_clock::now() - start_time).count();
}
}
