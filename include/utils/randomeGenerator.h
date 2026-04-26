#ifndef PA9_RANDOMEGENERATOR_H
#define PA9_RANDOMEGENERATOR_H

#pragma once
#include <random>

namespace Minesweeper {
    class RandomGenerator {
    private:
        std::mt19937 rng;

    public:
        RandomGenerator();
        ~RandomGenerator() = default;

        // Generate random integer in range [min, max]
        int getInt(int min, int max);

        // Generate random boolean with given probability
        bool getBool(double probability = 0.5);

        // Seed the generator
        void reseed(unsigned int seed);

        // Get a unique seed based on current time
        static unsigned int getTimeSeed();
    };
}

#endif //PA9_RANDOMEGENERATOR_H
