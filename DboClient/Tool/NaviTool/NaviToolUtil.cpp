#include "stdafx.h"
#include "NaviToolUtil.h"

void StringTokenize(std::vector<std::string>& vecstrOutToken, const std::string& strText, const std::string& strDelimiters)
{
	std::string::size_type lastPos	= strText.find_first_not_of(strDelimiters, 0);
	std::string::size_type pos		= strText.find_first_of(strDelimiters, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		vecstrOutToken.push_back(strText.substr(lastPos, pos - lastPos));

		lastPos	= strText.find_first_not_of(strDelimiters, pos);
		pos		= strText.find_first_of(strDelimiters, lastPos);
	}
}