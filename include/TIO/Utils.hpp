#pragma once

#include <random>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

template<size_t Dims>
struct Point;

// Global parameters used in algorithm
namespace GlobalParams
{
	constexpr size_t ANT_MEMORY_SIZE = 2;
	constexpr double NEIGHBOURHOOD_RATIO = 0.8;
	constexpr int MAX_FAILED_EXPLORATIONS_PER_SITE = 6;
	constexpr int MAX_NUMBER_OF_RECRUITMENTS = 6;
	constexpr int MAX_NEST_RELOCATIONS = 10;
	constexpr int MAX_ANTS_EXPLORATIONS_PER_NEST = 10;
	constexpr bool MINIMIZE = false;
}

inline bool isBetter(double _first, double _second)
{
	return GlobalParams::MINIMIZE ? _first < _second : _first > _second;
}

// Structure used to save domain of parameters for function
template<size_t Dims>
struct FunDomain
{
	double from[Dims];
	double to[Dims];

	// Constructor which sets all domains to the same values
	FunDomain(double _from, double _to)
	{
		std::fill_n(from, Dims, _from);
		std::fill_n(to, Dims, _to);
	}

	// Constructor for when the values of domains are not the same
	FunDomain(double* _from, double* _to)
	{
		std::copy(_from, _from + Dims, from);
		std::copy(_to, _to + Dims, to);
	}

	inline Point<Dims> getMiddlePoint()
	{
		double pos[Dims];
		for (int i = 0; i < Dims; i++)
		{
			pos[i] = from[i] + std::abs(to[i] - from[i]) / 2;
		}

		Point<Dims> result;
		std::copy(pos, pos + Dims, result.pos);
		return result;
	}

	inline double getNeighbourhoodSize() const
	{
		double sum = 0;
		for (int i = 0; i < Dims; i++)
		{
			sum += (to[i] - from[i]) * (to[i] - from[i]);
		}

		return std::sqrt(sum) / Dims * GlobalParams::NEIGHBOURHOOD_RATIO;
	}
};

// Structure representing a point in real space
template<size_t Dims>
struct Point
{
	double pos[Dims];

	inline void operator=(const Point<Dims> _other)
	{
		std::copy(_other.pos, _other.pos + Dims, pos);
	}

	inline Point<Dims> clampToDomain(const FunDomain<Dims>& _domain)
	{
		for (int i = 0; i < Dims; i++)
		{
			if (pos[i] < _domain.from[i])
			{
				pos[i] = _domain.from[i];
			}
			else if (pos[i] > _domain.to[i])
			{
				pos[i] = _domain.to[i];
			}
		}

		return *this;
	}
};

template<size_t Dims>
struct HuntingSite
{
	Point<Dims> s = {};
	int failureCount = 0;
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

// Function that returns random real number from normal distribution in range [_min, _max)
inline double randomNormal(const double _min = 0.0, const double _max = 1.0)
{
	return std::normal_distribution<double>(_min, _max)(gen);
}

// Function that returns random integer in range [_min, _max]
inline int randomInt(const int _min = 0, const int _max = 1)
{
	return std::uniform_int_distribution<int>(_min, _max)(gen);
}

// Function that returns a random point inside a neighbourhood defined by _center and _radius
template<size_t Dims>
inline Point<Dims> getRandomPointInNeighbourhood(const Point<Dims>& _center, double _radius)
{
	// Generate random n-dimensional vector
	double randomVec[Dims];
	for (int i = 0; i < Dims; i++)
	{
		randomVec[i] = randomNormal();
	}

	// Normalization, so that vector lie on the surface on n-dimensional sphere
	double norm = 0.0;
	for (int i = 0; i < Dims; i++)
	{
		norm += randomVec[i] * randomVec[i];
	}
	norm = std::sqrt(norm);
	for (int i = 0; i < Dims; i++)
	{
		randomVec[i] /= norm;
	}

	// Scaling by random value of radius
	const double randRadius = std::pow(random(), 1.0 / Dims);
	for (int i = 0; i < Dims; i++)
	{
		randomVec[i] *= randRadius * _radius;
	}

	// Translating to the target location
	for (int i = 0; i < Dims; i++) {
		randomVec[i] = randomVec[i] + _center.pos[i];
	}

	Point<Dims> result;
	std::copy(randomVec, randomVec + Dims, result.pos);
	return result;
}
