#pragma once


#include <vector>
#include <string>
using std::vector;
using std::string;


#include "Registry.h"


class CAppConfigManager : CRegistry
{
public:
	static CAppConfigManager& GetInstance(void);

	CAppConfigManager(void);
	~CAppConfigManager(void);

	void Init(void);
	void Free(void);

	bool Load();
	bool Save();

private:
	vector<string> m_vecItem;
};