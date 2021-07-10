#pragma once

#include <chrono>
#include <iostream>

class TimeToLive {
public:
    TimeToLive(int duration);

    int get_duration() const { return m_duration; }
    void set_duration(int duration) {m_duration = duration; }

    int get_timestamp() const { return m_timestamp; }
    void set_timestamp(unsigned int ts) { m_timestamp = ts; }

    int life_left() const;

    void poke();

private:
    int m_duration;
    unsigned int m_timestamp;
};
