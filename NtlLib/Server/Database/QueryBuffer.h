#ifndef _QUERYBUFFER_H
#define _QUERYBUFFER_H

#include <vector>
#include <string>

class QueryBuffer
{
public:
	std::vector<char*> queries;

	friend class Database;
	void AddQuery(const char * format, ...);
	void AddQueryNA(const char * str);
	void AddQueryStr(const std::string& str);
};




#endif