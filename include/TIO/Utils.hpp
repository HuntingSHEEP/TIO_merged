#pragma once

#include <random>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

// Global parameters used in algorithm
namespace GlobalParams
{
	constexpr size_t ANT_MEMORY_SIZE = 2;
	constexpr double NEIGHBORHOOD_SIZE = 10.0;
	constexpr int MAX_FAILED_EXPLORATIONS_PER_SITE = 6;
	constexpr int MAX_NUMBER_OF_RECRUITMENTS = 6;
	constexpr int MAX_NEST_RELOCATIONS = 10;
	constexpr int MAX_ANTS_EXPLORATIONS_PER_NEST = 10;
}

// Structure representing a point in real space
struct Point
{
	double x = 0;
	double y = 0;
};

// Structure used to save domain of parameters for function
struct FunDomain
{
	double xMin = 0.0;
	double xMax = 0.0;
	double yMin = 0.0;
	double yMax = 0.0;

	inline double getSizeX()
	{
		return std::abs(xMax - xMin);
	}

	inline double getSizeY()
	{
		return std::abs(yMax - yMin);
	}
};

namespace
{
    std::random_device device;
    std::mt19937 gen = std::mt19937(device());
}

// Function that returns random real number in range [_min, _max)
inline double random(const double _min = 0.0, const double _max = 1.0)
{
    return std::uniform_real_distribution<double>(_min, _max)(gen);
}

// Function that returns random integer in range [_min, _max]
inline int randomInt(const int _min = 0, const int _max = 1)
{
	return std::uniform_int_distribution<int>(_min, _max)(gen);
}

// Function that returns a random point inside a circle defined by _center and _radius
inline Point getRandomPointInCircle(const Point& _center, double _radius)
{
	const double phase = random(0, 2 * M_PI);
	const double dist = random(0, _radius);

	return { _center.x + dist * cos(phase), _center.y + dist * sin(phase) };
}
