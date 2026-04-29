#ifndef PA9_TIMER_H
#define PA9_TIMER_H

#pragma once


namespace Minesweeper {
    // Simple 999 second max timer
    class Timer {
    private:
        int seconds;
        bool running;
        bool started;   // true means first click
    public:
        Timer() : seconds(0), running(false), started(false) {}
        ~Timer() = default;
        
        void start() {if (!started) started = true, running = true;}
        void stop() {running = false;}
        void reset() {seconds = 0; running = false, started = false;}

        void tick() {
            if (running && seconds < 999){
                seconds++;
                if (seconds >= 999) {
                    running = false;    // Timer stops at 999 seconds
                }
            }
        }

        // Getter for seconds
        int getSeconds() const {return seconds;}

        bool isRunning() const {return running;}
        bool isMaxed() const {return seconds >= 999;}
        bool hasStarted() const {return started;}
    };

}
#endif //PA9_TIMER_H
