#include "TTL.h"

TimeToLive::TimeToLive(int duration) {
    m_duration = duration;
    poke();
}

int TimeToLive::life_left() const {
    auto current = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return m_duration - (current - m_timestamp);
}

void TimeToLive::poke() {
    m_timestamp = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    std::cout << m_timestamp << std::endl;
}
