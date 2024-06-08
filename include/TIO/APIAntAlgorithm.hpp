#pragma once

#include <functional>
#include <vector>

#include "Utils.hpp"
#include "functions.h"

// Class representing an ant
template<size_t Dims>
class Ant
{
	// Public methods
public:
	Ant() = delete;

	Ant(Point<Dims>* _nest, std::function<double(double*, size_t)>* _fun, const int _antIdx, FunDomain<Dims>* _funDomain)
		: m_nest(_nest)
		, m_fun(_fun)
		, m_aSite(std::pow(100, 1.0 / (_antIdx + 1)) / 100.0)
		, m_aLocal(m_aSite / 10.0)
		, m_funDomain(_funDomain)
	{
		m_bestVal = std::numeric_limits<double>::infinity() * (GlobalParams::MINIMIZE ? 1 : -1); 
		if (m_memory.empty())
		{
			m_memory.reserve(GlobalParams::ANT_MEMORY_SIZE);
		}
	}

	HuntingSite<Dims> getSiteByIndex(int _index) const
	{
		return m_memory[_index];
	}

	void setSiteAtIndex(const HuntingSite<Dims>& _site, int _index)
	{
		m_memory[_index] = _site;
	}

	void explore()
	{
		if (m_memory.size() < GlobalParams::ANT_MEMORY_SIZE)
		{
			HuntingSite<Dims> newHuntingSite = { getRandomPointInNeighbourhood(*m_nest, m_funDomain->getNeighbourhoodSize() * m_aSite).clampToDomain(*m_funDomain) };
			exploreSite(newHuntingSite);
			m_memory.push_back(newHuntingSite);
			m_currentPosition = newHuntingSite.s;
		}
		else
		{
			if (m_lastExplorationSuccessful)
			{
				exploreSite(m_lastExploredSite);
				m_currentPosition = m_lastExploredSite.s;
			}
			else
			{
				int randomIdx = randomInt(0, GlobalParams::ANT_MEMORY_SIZE - 1);
				HuntingSite siteFromMemory = m_memory[randomIdx];
				exploreSite(siteFromMemory);
				m_currentPosition = siteFromMemory.s;
			}
		}
	}

	inline void reset()
	{
		m_memory.clear();
		m_lastExploredSite = {};
		m_lastExplorationSuccessful = false;
	}

	inline void removeUnsuccessfulSites()
	{
		std::vector<HuntingSite<Dims>> onlySuccessfulSites = {};
		onlySuccessfulSites.reserve(GlobalParams::ANT_MEMORY_SIZE);

		for (int i = 0; i < m_memory.size(); i++)
		{
			if (m_memory[i].failureCount < GlobalParams::MAX_FAILED_EXPLORATIONS_PER_SITE)
			{
				onlySuccessfulSites.push_back(m_memory[i]);
			}
		}

		m_memory = onlySuccessfulSites;
	}

	int getBestSiteIndex()
	{
		int bestSiteIdx = -1;
		double bestValue = std::numeric_limits<double>::infinity() * (GlobalParams::MINIMIZE ? 1 : -1);

		for (int i = 0; i < m_memory.size(); i++)
		{
			double valueAtCenterOfSite = (*m_fun)(m_memory[i].s.pos, Dims);
			if (isBetter(valueAtCenterOfSite, bestValue))
			{
				bestValue = valueAtCenterOfSite;
				bestSiteIdx = i;
			}
		}

		return bestSiteIdx;
	}

	inline Point<Dims> getCurrentPosition() const
	{
		return m_currentPosition;
	}

	inline Point<Dims> getBestPos() const
	{
		return m_bestPos;
	}

	inline double getBestVal() const
	{
		return m_bestVal;
	}

	// Private methods
private:
	void exploreSite(HuntingSite<Dims>& _huntingSite)
	{
		double valueInS = (*m_fun)(_huntingSite.s.pos, Dims);

		Point<Dims> point = getRandomPointInNeighbourhood(_huntingSite.s, m_funDomain->getNeighbourhoodSize() * m_aLocal).clampToDomain(*m_funDomain);
		double valueInExploredPoint = (*m_fun)(point.pos, Dims);

		if (isBetter(valueInExploredPoint, valueInS))
		{
			m_lastExplorationSuccessful = true;
			_huntingSite.s = point;
			_huntingSite.failureCount = 0;

			if(isBetter(valueInExploredPoint, m_bestVal))
			{
				m_bestVal = valueInExploredPoint;
				m_bestPos = point;
			}
		}
		else
		{
			m_lastExplorationSuccessful = false;
			_huntingSite.failureCount++;

			if(isBetter(valueInS, m_bestVal))
			{
				m_bestVal = valueInS;
				m_bestPos = _huntingSite.s;
			}
		}

		if(valueInExploredPoint)

		m_lastExploredSite = _huntingSite;
	}

	// Private memebers
private:
	Point<Dims>* m_nest = nullptr;
	std::function<double(double*, size_t)>* m_fun = nullptr;
	FunDomain<Dims>* m_funDomain = nullptr;

	std::vector<HuntingSite<Dims>> m_memory = {};
	HuntingSite<Dims> m_lastExploredSite = {};
	bool m_lastExplorationSuccessful = false;
	const double m_aSite;
	const double m_aLocal;
	Point<Dims> m_currentPosition = {};
	double m_bestVal = {};
	Point<Dims> m_bestPos = {};
};

// Class representing an API ant algorithm
template<size_t Dims>
class APIAntAlgorithm
{
	// Public methods
public:
	APIAntAlgorithm(const int _antsAmount, FunctionInfo _functionInfo)
		: APIAntAlgorithm(_antsAmount, _functionInfo.functionPointer, {_functionInfo.xMin, _functionInfo.xMax, _functionInfo.yMin, _functionInfo.yMax}){}


	APIAntAlgorithm(const int _antsAmount, const std::function<double(double*, size_t)>& _fun, const FunDomain<Dims>& _funDomain)
		: m_fun(_fun)
		, m_funDomain(_funDomain)
	{
		m_nest = m_funDomain.getMiddlePoint();

		if (m_ants.empty())
		{
			m_ants.reserve(_antsAmount);
			for (int i = 0; i < _antsAmount; i++)
			{
				m_ants.push_back(Ant(&m_nest, &m_fun, i, &m_funDomain));
			}
		}
	}

	inline Point<Dims> getNest()
	{
		return m_nest;
	}

	inline bool finished() const
	{
		return m_finished;
	}

	inline std::vector<Point<Dims>> getAntsPositions()
	{
		std::vector<Point<Dims>> positions;

		for (const auto& ant : m_ants)
		{
			positions.push_back(ant.getCurrentPosition());
		}

		return positions;
	}

	void update(const FunctionInfo& _functionInfo)
	{
		for (auto& ant : m_ants)
		{
			ant.explore();
		}

		m_antsExplorationCounter++;

		for (auto& ant : m_ants)
		{
			ant.removeUnsuccessfulSites();
		}

		performRecruitment();

		if (m_antsExplorationCounter >= GlobalParams::MAX_ANTS_EXPLORATIONS_PER_NEST)
		{
			relocateNest();
			m_nestRelocationCounter++;

			for (auto& ant : m_ants)
			{
				ant.reset();
			}

			m_antsExplorationCounter = 0;
		}

		if(Dims == 2)
		{
			auto [point, value] = getBest();
			glm::vec2 vecPoint(point.pos[0], point.pos[1]);
			int optimumIdx = 0;
			float optimumDist = glm::length(_functionInfo.LUT2D[0].first - vecPoint);
			for(int i = 1; i < _functionInfo.LUT2D.size(); i++)
			{
				const auto [lutPoint, lutValue] = _functionInfo.LUT2D[i];
				float dist = glm::length(lutPoint - vecPoint);

				if(dist < optimumDist)
				{
					optimumDist = dist;
					optimumIdx = i;
				}
			}
		
			if(optimumDist <= 0.1 || m_nestRelocationCounter >= GlobalParams::MAX_NEST_RELOCATIONS)
			{
				displayInfo(value, optimumDist, optimumIdx, _functionInfo, point);
				m_finished = true;
			}
		}
		else
		{
			auto [point, value] = getBest();
			int optimumIdx = 0;
			float optimumDist = 1.0f;
			if(_functionInfo.LUT10D.size() > 0)
			{
				Point<Dims> lutPoint;
				std::copy(_functionInfo.LUT10D[0].first.data(), _functionInfo.LUT10D[0].first.data() + Dims, lutPoint.pos);
				
				optimumDist = static_cast<float>(Point<Dims>::dist<Dims>(lutPoint, point));

				for(int i = 1; i < _functionInfo.LUT10D.size(); i++)
				{
					const auto [nextLUTPoint, nextLUTValue] = _functionInfo.LUT10D[i];
					Point<Dims> nextPoint;
					std::copy(_functionInfo.LUT10D[i].first.data(), _functionInfo.LUT10D[i].first.data() + Dims, nextPoint.pos);
					
					float dist = static_cast<float>(Point<Dims>::dist<Dims>(nextPoint, point));

					if(dist < optimumDist)
					{
						optimumDist = dist;
						optimumIdx = i;
					}
				}
			}
		
			if(optimumDist <= 0.1 || m_nestRelocationCounter >= GlobalParams::MAX_NEST_RELOCATIONS)
			{
				displayInfo(value, optimumDist, optimumIdx, _functionInfo, point);
				m_finished = true;
			}
		}
	}

	std::pair<Point<Dims>, double> getBest()
	{
		Point<Dims> point;
		double val = std::numeric_limits<double>::infinity() * (GlobalParams::MINIMIZE ? 1 : -1);

		for(const auto& ant : m_ants)
		{
			double antValue = ant.getBestVal();
			if(isBetter(antValue, val))
			{
				val = antValue;
				point = ant.getBestPos();
			}
		}

		return { point, val };
	}

	// Private methods
private:
	void displayInfo(double value, float optimumDist, int optimumIdx, FunctionInfo _functionInfo, Point<Dims> point)
	{
			if(m_nestRelocationCounter >= GlobalParams::MAX_NEST_RELOCATIONS)
			{
				std::cout << "Stop criterion reached!\n";
			}

			std::cout << "Relocations: " << m_nestRelocationCounter << ".\nExplorations: " << m_antsExplorationCounter << ".\n";
			std::cout << "Best value found in point (";
			for(int i = 0; i < Dims; i++)
			{
				std::cout << point.pos[i] << ( i != Dims - 1 ? ", " : "");
			}
			std::cout << ") with value: " << value << ".\n";
		
			if(Dims == 2 && _functionInfo.LUT2D.size() > 0 || Dims == 10 && _functionInfo.LUT10D.size() > 0)
			{
				std::cout << "Distance from the closest global minimum: " << optimumDist << ".\n";
				std::cout << "Differernce between minimum and found value: " << std::abs((Dims == 2 ? _functionInfo.LUT2D[optimumIdx].second : _functionInfo.LUT10D[optimumIdx].second) - value) << ".\n";
			}
			else if(Dims == 10 && _functionInfo.LUT10D.size() == 0)
			{
				std::cout << "Unknown global minimum - cannot compare.\n";
			}
	}

	void performRecruitment()
	{
		if (m_ants.size() == 1)
		{
			return;
		}
		// idxWinner will hold the idx of an ant which had higher value of bench function.
		// At the beginning it is choosen randomly.
		int idxWinner = randomInt(0, static_cast<int>(m_ants.size() - 1));

		// idxDrawn will hold the idx of an ant which will be drawn at every iteration.
		int idxDrawn;

		for (int i = 0; i < GlobalParams::MAX_NUMBER_OF_RECRUITMENTS; i++)
		{
			// Draw second ant to be compared to with the winner
			do
			{
				idxDrawn = randomInt(0, static_cast<int>(m_ants.size() - 1));
			} while (idxWinner == idxDrawn);

			Ant<Dims>& winnerAnt = m_ants[idxWinner];
			Ant<Dims>& drawnAnt = m_ants[idxDrawn];

			const int winnerSiteIdx = winnerAnt.getBestSiteIndex();
			const int drawnSiteIdx = drawnAnt.getBestSiteIndex();

			// Edge case - both memories are empty (not sure if possible)
			if (winnerSiteIdx == -1 && drawnSiteIdx == -1)
			{
				return;
			}

			if (winnerSiteIdx == -1)
			{
				// Edge case - winner's memory is empty (might be impossible too)
				winnerAnt.setSiteAtIndex(drawnAnt.getSiteByIndex(drawnSiteIdx), 0);
				idxWinner = idxDrawn;
			}
			else if (drawnSiteIdx == -1)
			{
				// Edge case - drawn's memory is empty (might be impossible too)
				drawnAnt.setSiteAtIndex(winnerAnt.getSiteByIndex(winnerSiteIdx), 0);
			}
			else
			{
				HuntingSite<Dims> winnerSite = winnerAnt.getSiteByIndex(winnerSiteIdx);
				HuntingSite<Dims> drawnSite = drawnAnt.getSiteByIndex(drawnSiteIdx);

				const double valWinner = m_fun(winnerSite.s.pos, Dims);
				const double valDrawn = m_fun(drawnSite.s.pos, Dims);

				// Replace site depending on the results
				if (isBetter(valWinner, valDrawn))
				{
					// Value of winner is higher (or lower) to that of drawn - do not change the index of the winner
					drawnAnt.setSiteAtIndex(winnerSite, drawnSiteIdx);
				}
				else
				{
					winnerAnt.setSiteAtIndex(drawnSite, winnerSiteIdx);
					idxWinner = idxDrawn;	// Value in drawn is higher than winner's - change the index of the winner
				}
			}
		}
	}

	void relocateNest()
	{
		double bestVal = std::numeric_limits<double>::infinity() * (GlobalParams::MINIMIZE ? 1 : -1);
		Point<Dims> bestSiteCenter = {};

		for (int i = 0; i < m_ants.size(); i++)
		{
			Ant<Dims>& ant = m_ants[i];
			const int siteIdx = ant.getBestSiteIndex();

			if (siteIdx == -1)
			{
				continue;
			}

			HuntingSite<Dims> site = ant.getSiteByIndex(siteIdx);
			const double val = m_fun(site.s.pos, Dims);

			if (isBetter(val, bestVal))
			{
				bestVal = val;
				bestSiteCenter = site.s;
			}
		}

		m_nest = bestSiteCenter;
	}

	// Private members
private:
	std::vector<Ant<Dims>> m_ants = {};
	Point<Dims> m_nest = {};
	FunDomain<Dims> m_funDomain = {};
	std::function<double(double*, size_t)> m_fun = {};
	int m_nestRelocationCounter = 0;
	int m_antsExplorationCounter = 0;
	bool m_finished = false;
};
