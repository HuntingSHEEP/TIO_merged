#pragma once

#include <functional>
#include <vector>

#include "TIO/Utils.hpp"

// Class representing an ant
class Ant
{
	// Public classes/structures
public:
	struct HuntingSite
	{
		Point s = {};
		int failureCount = 0;
	};

	// Public methods
public:
	Ant() = delete;

	Ant(Point* _nest, std::function<double(double, double)>* _fun, const int _antIdx, FunDomain* _funDomain)
		: m_nest(_nest)
		, m_fun(_fun)
		, m_aSite(std::pow(100, 1.0 / (_antIdx + 1)) / 100.0)	
		, m_aLocal(m_aSite / 10.0)
		, m_funDomain(_funDomain)
	{
		if (m_memory.empty())
		{
			m_memory.reserve(GlobalParams::ANT_MEMORY_SIZE);
		}
	}

	HuntingSite getSiteByIndex(int _index) const
	{
		return m_memory[_index];
	}

	void setSiteAtIndex(const HuntingSite& _site, int _index)
	{
		m_memory[_index] = _site;
	}

	void explore()
	{
		if (m_memory.size() < GlobalParams::ANT_MEMORY_SIZE)
		{
			HuntingSite newHuntingSite = { getRandomPointInCircle(*m_nest, m_funDomain->getNeighbourhoodSize() * m_aSite).clampToDomain(*m_funDomain) };
			exploreSite(newHuntingSite);
			m_memory.push_back(newHuntingSite);
			m_currentPosition = { newHuntingSite.s.x, newHuntingSite.s.y };
		}
		else
		{
			if (m_lastExplorationSuccessful)
			{
				exploreSite(m_lastExploredSite);
				m_currentPosition = { m_lastExploredSite.s.x, m_lastExploredSite.s.y };
			}
			else
			{
				int randomIdx = randomInt(0, GlobalParams::ANT_MEMORY_SIZE - 1);
				HuntingSite& siteFromMemory = m_memory[randomIdx];
				exploreSite(siteFromMemory);
				m_currentPosition = { siteFromMemory.s.x, siteFromMemory.s.y };
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
		std::vector<HuntingSite> onlySuccessfulSites = {};
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

	int getBestSiteIndex() const
	{
		int bestSiteIdx = -1;
		double max = -std::numeric_limits<double>::infinity();

		for (int i = 0; i < m_memory.size(); i++)
		{
			double valueAtCenterOfSite = (*m_fun)(m_memory[i].s.x, m_memory[i].s.y);
			if (valueAtCenterOfSite > max)
			{
				max = valueAtCenterOfSite;
				bestSiteIdx = i;
			}
		}

		return bestSiteIdx;
	}

	inline Point getCurrentPosition() const
	{
		return m_currentPosition;
	}

	// Private methods
private:
	void exploreSite(HuntingSite& _huntingSite)
	{
		double valueInS = (*m_fun)(_huntingSite.s.x, _huntingSite.s.y);

		auto [x, y] = getRandomPointInCircle(_huntingSite.s, m_funDomain->getNeighbourhoodSize() * m_aLocal).clampToDomain(*m_funDomain);
		double valueInExploredPoint = (*m_fun)(x, y);

		if (valueInExploredPoint > valueInS)
		{
			m_lastExplorationSuccessful = true;
			_huntingSite.s = { x, y };
			_huntingSite.failureCount = 0;
		}
		else
		{
			m_lastExplorationSuccessful = false;
			_huntingSite.failureCount++;
		}

		m_lastExploredSite = _huntingSite;
	}

	// Private memebers
private:
	Point* m_nest = nullptr;
	std::function<double(double, double)>* m_fun = nullptr;
	FunDomain* m_funDomain = nullptr;

	std::vector<HuntingSite> m_memory = {};
	HuntingSite m_lastExploredSite = {};
	bool m_lastExplorationSuccessful = false;
	const double m_aSite;
	const double m_aLocal;
	Point m_currentPosition = {};
};

// Class representing an API ant algorithm
class APIAntAlgorithm
{
	// Public methods
public:
	APIAntAlgorithm(const int _antsAmount, const std::function<double(double, double)>& _fun, const FunDomain& _funDomain)
		: m_fun(_fun)
		, m_funDomain(_funDomain)
	{
		if (m_ants.empty())
		{
			m_ants.reserve(_antsAmount);
			for (int i = 0; i < _antsAmount; i++)
			{
				m_ants.push_back(Ant(&m_nest, &m_fun, i, &m_funDomain));
			}
		}

		m_nest = { m_funDomain.xMin + m_funDomain.getSizeX() / 2, m_funDomain.yMin + m_funDomain.getSizeY() / 2 };
	}

	inline Point getNest()
	{
		return m_nest;
	}

	inline bool finished() const
	{
		return m_nestRelocationCounter >= GlobalParams::MAX_NEST_RELOCATIONS;
	}

	inline std::vector<Point> getAntsPositions()
	{
		std::vector<Point> positions;

		for (const auto& ant : m_ants)
		{
			positions.push_back(ant.getCurrentPosition());
		}

		return positions;
	}

	void update()
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
	}

	// Private methods
private:
	void performRecruitment()
	{
		if(m_ants.size() == 1)
			return;
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

			Ant& winnerAnt = m_ants[idxWinner];
			Ant& drawnAnt = m_ants[idxDrawn];

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
				const Ant::HuntingSite& winnerSite = winnerAnt.getSiteByIndex(winnerSiteIdx);
				const Ant::HuntingSite& drawnSite = drawnAnt.getSiteByIndex(drawnSiteIdx);

				const double valWinner = m_fun(winnerSite.s.x, winnerSite.s.y);
				const double valDrawn = m_fun(drawnSite.s.x, drawnSite.s.y);

				// Replace site depending on the results
				if (valWinner >= valDrawn)
				{
					// Value of winner is higher or equal to that of drawn - do not change the index of the winner
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
		double max = -std::numeric_limits<double>::infinity();
		Point bestSiteCenter = {};

		for (int i = 0; i < m_ants.size(); i++)
		{
			const Ant& ant = m_ants[i];
			const int siteIdx = ant.getBestSiteIndex();

			if (siteIdx == -1)
			{
				continue;
			}

			Ant::HuntingSite site = ant.getSiteByIndex(siteIdx);
			const double val = m_fun(site.s.x, site.s.y);

			if (val > max)
			{
				max = val;
				bestSiteCenter = site.s;
			}
		}

		m_nest = bestSiteCenter;
	}

	// Private members
private:
	std::vector<Ant> m_ants = {};
	Point m_nest = {};

	std::function<double(double, double)> m_fun = {};
	FunDomain m_funDomain = {};
	int m_nestRelocationCounter = 0;
	int m_antsExplorationCounter = 0;
};
