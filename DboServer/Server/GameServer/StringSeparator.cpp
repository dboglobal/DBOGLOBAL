#include "stdafx.h"
#include "StringSeparator.h"



CStringSeparator::CStringSeparator()
{
}

CStringSeparator::~CStringSeparator()
{
}

void CStringSeparator::ClearToken()
{
	m_listToken.clear();
}

bool CStringSeparator::AddSeparator(std::string strSeparator)
{
	//check if already added
	for (SEPARATOR_IT it = m_vecSeparator.begin(); it != m_vecSeparator.end(); it++)
	{
		std::string itStr = *it;

		if (itStr == strSeparator)
			return false;
	}

	//add separator
	m_vecSeparator.push_back(strSeparator);

	return true;
}

bool CStringSeparator::AddRemover(std::string strSeparator)
{
	//check if already added
	for (SEPARATOR_IT it = m_vecRemover.begin(); it != m_vecRemover.end(); it++)
	{
		std::string itStr = *it;

		if (itStr.compare(strSeparator) == 0)
			return false;
	}

	//add remover
	m_vecRemover.push_back(strSeparator);

	return true;
}

bool CStringSeparator::Separate(std::string& strBuffer)
{
	ClearToken();

	const char* szBuffer = strBuffer.c_str();
	int nCurrentLoc = 0;
	int nLoc = 0;

	while (nLoc < strBuffer.size())
	{
		bool bFound = false;

		for (SEPARATOR_IT itRem = m_vecRemover.begin(); itRem != m_vecRemover.end(); itRem++)
		{
			std::string& strRem = *itRem;

			if (strncmp(&szBuffer[nLoc], strRem.c_str(), strRem.size()) == 0)
			{
				if (nCurrentLoc != nLoc)
				{
					m_listToken.push_back(strBuffer.substr(nCurrentLoc, nLoc - nCurrentLoc));
				}

				nLoc += (int)strRem.size();
				nCurrentLoc = nLoc;
				bFound = true;
				break;
			}
		}

		if (bFound == false)
		{
			for (SEPARATOR_IT itSepa = m_vecSeparator.begin(); itSepa != m_vecSeparator.end(); itSepa++)
			{
				std::string& strSep = *itSepa;

				if (strncmp(&szBuffer[nLoc], strSep.c_str(), strSep.size()) == 0)
				{
					if (nCurrentLoc != nLoc)
					{
						m_listToken.push_back(strBuffer.substr(nCurrentLoc, nLoc - nCurrentLoc));
					}

					m_listToken.push_back(strSep);

					nLoc += (int)strSep.size();
					nCurrentLoc = nLoc;
					bFound = true;
					break;
				}
			}

			if (bFound == false)
				++nLoc;
		}
	}

	if (nCurrentLoc != strBuffer.size())
	{
		m_listToken.push_back(strBuffer.substr(nCurrentLoc, strBuffer.size() - nCurrentLoc));
	}

	////test
	//for (TOKEN_LIST::iterator it = m_listToken.begin(); it != m_listToken.end(); it++)
	//{
	//	std::string & token = *it;
	//	printf("token [%s] \n", token.c_str());
	//}

	return true;
}

bool CStringSeparator::IsFloat(std::string & rNumber)
{
	int v7 = 0;

	const char* strNumb = rNumber.c_str();

	for (int j = 0; rNumber.size() > j; j++)
	{
		if (v7)
		{
			if (v7 == 1)
			{
				if (strNumb[j] < 48 && strNumb[j] > 57)
				{
					if (strNumb[j] != 46)
						return false;
					v7 = 2;
				}
			}
			else if (v7 == 2)
			{
				if (strNumb[j] < 48 || strNumb[j] > 57)
					return false;
				v7 = 3;
			}
			else
			{
				if (v7 != 3)
					return false;
				if (strNumb[j] < 48 || strNumb[j] > 57)
					return false;
			}
		}
		else if (j || *strNumb != 45)
		{
			if (strNumb[j] < 48 || strNumb[j] > 57)
				return false;
			v7 = 1;
		}
		else
		{
			v7 = 1;
		}
	}

	return v7 == 1 || v7 == 3;
}


bool CStringSeparator::IsInteger(std::string & rNumber)
{
	for (int j = 0; rNumber.size() > j; j++)
	{
		if ((j || *rNumber.c_str() != 45) && (rNumber.c_str()[j] < 48 || rNumber.c_str()[j] > 57))
			return false;
	}

	return true;
}

bool CStringSeparator::IsUnsignedInteger(std::string & rNumber)
{
	for (int j = 0; rNumber.size() > j; j++)
	{
		if (rNumber.c_str()[j] < 48 || rNumber.c_str()[j] > 57)
			return false;
	}

	return true;
}



















