#pragma once


using std::string;
#include <string>


class CRegistry
{
public:
	CRegistry(void);
	virtual ~CRegistry(void);

public:
	void SetRegistry(const string& SubKey, const string& ValueName, const string& Value);
	void GetRegistry(const string& SubKey, const string& ValueName, string& Value);
	void DelRegistry(const string& SubKey, const string& ValueName);
};