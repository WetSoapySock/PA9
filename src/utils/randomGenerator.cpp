#include "../include/utils/RandomGenerator.h"

Minesweeper::RandomGenerator::RandomGenerator()
{
	rng.seed(getTimeSeed());
}

int Minesweeper::RandomGenerator::getInt(int min, int max)
{
	if (min > max)
	{
		throw std::invalid_argument("min cannot be greater than max");
	}

	std::uniform_int_distribution<int> dist(min, max);

	return dist(rng);
}

bool Minesweeper::RandomGenerator::getBool(double probability)
{
	if (probability < 0.0 || probability > 1.0)
	{
		throw std::invalid_argument("Probability must be between 0 and 1");
	}

	std::bernoilli_distribution dist(probability);

	return dist(rng);
}

void Minesweeper::RandomGenerator::reseed(unsigned int seed)
{
	rng.seed(seed);
}

unsigned int Minesweeper::RandomGenerator::getTimeSeed()
{
	return static_cast<unsigned int> (
		std::chrono::system_clock::now().time_since_epoch().count());
}