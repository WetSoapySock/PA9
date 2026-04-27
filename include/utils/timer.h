#ifndef PA9_TIMER_H
#define PA9_TIMER_H

#pragma once
#include <SFML/System/Clock.h>
#include <string>
#include <sstream>

namespace Minesweeper {
    // Simple 999 second max timer
    class Timer {
    private:
        int seconds;
        bool running;
    public:
        Timer() : seconds(0), running(false) {}

        void start() {running = true;}
        void pause() {running = false;}
        void reset() {seconds = 0; running = false;}

        void tick() {if (running && seconds < 999) seconds++;}

        // Getter for seconds
        int getSeconds() const {return seconds;}

        bool isRunning() const {return running;}
        bool isMaxed() const {return seconds >= 999;}
    };

}
#endif //PA9_TIMER_H
