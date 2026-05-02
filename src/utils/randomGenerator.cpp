#include "../include/utils/randomGenerator.h"  

/*
 Function: RandomGenerator (constructor)
 Description: Seeds the Mersenne Twister random number engine using a
              time-based seed to ensure different random sequences each run.
*/
Minesweeper::RandomGenerator::RandomGenerator()
{
    rng.seed(getTimeSeed());
}

/*
 Function: getInt
 Description: Generates a random integer between min and max (inclusive).
              Validates that min is not greater than max to prevent errors.
              Parameters: min = lower bound, max = upper bound (inclusive)
              Returns: Random integer in range [min, max]
              Throws: std::invalid_argument if min > max
*/
int Minesweeper::RandomGenerator::getInt(int min, int max)
{
    if (min > max)
    {
        throw std::invalid_argument("min cannot be greater than max");
    }

    std::uniform_int_distribution<int> dist(min, max);

    return dist(rng);
}

/*
 Function: getBool
 Description: Generates a random boolean value with specified probability of true.
              Default probability is 0.5 (50% chance).
              Parameters: probability = chance of returning true (0.0 to 1.0)
              Returns: true with given probability, false otherwise
              Throws: std::invalid_argument if probability outside [0,1]
*/
bool Minesweeper::RandomGenerator::getBool(double probability)
{
    if (probability < 0.0 || probability > 1.0)
    {
        throw std::invalid_argument("Probability must be between 0 and 1");
    }

    std::bernoulli_distribution dist(probability);

    return dist(rng);
}

/*
 Function: reseed
 Description: Manually reseeds the random number generator with a specific seed.
              Useful for reproducible random sequences (e.g., debugging).
              Parameters: seed = new seed value for the generator
*/
void Minesweeper::RandomGenerator::reseed(unsigned int seed)
{
    rng.seed(seed);
}

/*
 Function: getTimeSeed
 Description: Static method that returns a seed value derived from current system time.
              Uses high-resolution system clock to ensure unique seeds across runs.
              Returns: Unsigned integer seed based on system time since epoch
*/
unsigned int Minesweeper::RandomGenerator::getTimeSeed()
{
    return static_cast<unsigned int> (
        std::chrono::system_clock::now().time_since_epoch().count());
}
