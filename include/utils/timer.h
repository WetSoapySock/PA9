#ifndef PA9_TIMER_H
#define PA9_TIMER_H

#pragma once
#include <SFML/System/Clock.h>

namespace Minesweeper {
    class Timer {
    private:
        sf::Clock clock;
        float pausedTime;
        bool isPaused;
        float startTime;

    public:
        Timer();
        ~Timer() = default;

        void start();
        void pause();
        void resume();
        void reset();

        float getElapsedSeconds() const;
        int getElapsedSecondsInt() const;
        std::string getFormattedTime() const;

        bool isRunning() const { return !isPaused; }
    };
}

#endif //PA9_TIMER_H
