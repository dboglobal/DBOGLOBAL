#include "stdafx.h"
#include "QueryBuffer.h"


void QueryBuffer::AddQuery(const char * format, ...)
{
	char query[4096];
	va_list vlist;
	va_start(vlist, format);
	vsnprintf_s(query, 4096, format, vlist);
	va_end(vlist);

	queries.push_back(_strdup(query));
}

void QueryBuffer::AddQueryNA(const char * str)
{
	queries.push_back(_strdup(str));
}

void QueryBuffer::AddQueryStr(const std::string& str)
{
	queries.push_back(_strdup(str.c_str()));
}