#ifndef PA9_RANDOMGENERATOR_H
#define PA9_RANDOMGENERATOR_H

#pragma once
// AI Gen, Libraries
#include <random>      // Gen AI, mt19937 and distributions
#include <chrono>      // Clock for seeding
#include <stdexcept>   // Exception handling

/** Gen AI, "We need a RandomGenerator class for our Minesweeper game to handle
    random mine placement and any possible future features that require randomness.
    Which libraries are needed for this to keep complexity as low as possible **/

namespace Minesweeper {
    class RandomGenerator {
    private:
        std::mt19937 rng;  // Gen AI: Mersenne Twister engine
    public:
        RandomGenerator();
        ~RandomGenerator() = default;

        // No copy constructor or assignment
        RandomGenerator(const RandomGenerator&) = delete;
        RandomGenerator& operator=(const RandomGenerator&) = delete;

        int getInt(int min, int max);              // Random int
        bool getBool(double probability = 0.5);    // 50/50 probability for true/false

        void reseed(unsigned int seed);            // Manual reseed
        static unsigned int getTimeSeed();         // Time-based seed
    };

}

#endif //PA9_RANDOMGENERATOR_H
