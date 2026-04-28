#ifndef PA9_RANDOMGENERATOR_H
#define PA9_RANDOMGENERATOR_H

#pragma once
#include <random>
#include <chrono>
#include <stdexcept>  // For error handling

namespace Minesweeper {
    /**
     * @brief Random number generator for Minesweeper
     *
     * [AI-Gen] Handles all random number needs for mine placement
     * and game randomization.
     *
     * Prompt used: "Create a RandomGenerator class for Minesweeper
     * using mt19937 with methods for random integers, booleans,
     * reseeding, and time-based seed generation."
     */
    class RandomGenerator {
    private:
        // AI Gen, "How can we implement a random seed for initialization?"
        std::mt19937 rng;  ///< Mersenne Twister engine

    public:
        RandomGenerator();
        ~RandomGenerator() = default;

        // Ai Gen, Deletes copy to prevent accidental duplication
        RandomGenerator(const RandomGenerator&) = delete;
        RandomGenerator& operator=(const RandomGenerator&) = delete;

        // Random number generation methods
        int getInt(int min, int max);

        // AI Gen, Generates random boolean 50/50
        bool getBool(double probability = 0.5);

        void reseed(unsigned int seed);
        static unsigned int getTimeSeed();
    };

}

#endif //PA9_RANDOMEGENERATOR_H
