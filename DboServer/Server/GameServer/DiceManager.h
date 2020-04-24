#pragma once

#include <set>
#include "NtlRandom.h"


class CDiceManager
{
public:

	CDiceManager() {}
	~CDiceManager() { m_setDice.clear(); }

public:

	inline unsigned char		RollDice()
	{
		bool bDiceSuccess = false;
		int nDice = 0;

		while (bDiceSuccess == false)
		{
			nDice = RandomRange(1, 100);

			if (m_setDice.find(nDice) == m_setDice.end())
			{
				m_setDice.insert(nDice);
				bDiceSuccess = true;
			}
		}

		return (unsigned char)nDice;
	}

	inline void Init() { m_setDice.clear(); }

private:

	std::set<int> m_setDice;
};