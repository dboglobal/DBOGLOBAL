#include "stdafx.h"
#include "ParameterReader.h"



CParameterReader::CParameterReader()
{
	AddSeparator(" ");
	AddSeparator("\"");
}

CParameterReader::~CParameterReader()
{

}


std::string CParameterReader::GetString(int nLoc)
{
	if (m_listParameter.size() > nLoc)
	{
		std::list<std::string>::iterator it = m_listParameter.begin();

		for (int j = 0; j < nLoc; j++)
			it++;

		return *it;
	}

	return NULL;
}

CParameterReader::sNUMBER CParameterReader::GetNumber(int nLoc)
{
	std::string rNumber = GetString(nLoc);

	if (rNumber.size())
	{
		if (CStringSeparator::IsFloat(rNumber))
		{
			sNUMBER num(true, (float)std::atof(rNumber.c_str()));
			return num;
		}
		else
		{
			sNUMBER num(false, -99.999f);
			return num;
		}
	}

	sNUMBER num(false, 0.0);
	return num;
}


bool CParameterReader::Analyze(TOKEN_IT itToken)
{
	std::string& strToken = *itToken;

	switch (m_eReadState)
	{
		case READ_STATE_START:
		{
			if (strcmp("\"", strToken.c_str()))
			{
				if (strcmp(" ", strToken.c_str()))
				{
					m_listParameter.push_back(strToken);
					m_eReadState = READ_STATE_WORD;
				}
				else
					m_eReadState = READ_STATE_SPACE;
			}
			else
				m_eReadState = READ_STATE_STRING_START;
		}
		break;
		case READ_STATE_WORD:
		{
			if (!strcmp("\"", strToken.c_str()))
				return false;

			if (strcmp(" ", strToken.c_str()))
				return false;

			m_eReadState = READ_STATE_SPACE;
		}
		break;
		case READ_STATE_STRING_START:
		{
			if (strcmp("\"", strToken.c_str()))
			{
				m_strString.operator+=(strToken.c_str());
			}
			else
			{
				m_listParameter.push_back(m_strString);
				m_strString.clear();
				m_eReadState = READ_STATE_STRING_END;
			}
		}
		break;
		case READ_STATE_STRING_END:
		{
			if (!strcmp("\"", strToken.c_str()))
				return false;

			if (strcmp(" ", strToken.c_str()))
				return false;

			m_eReadState = READ_STATE_SPACE;
		}
		break;
		case READ_STATE_SPACE:
		{
			if (strcmp("\"", strToken.c_str()))
			{
				if (strcmp(" ", strToken.c_str()))
				{
					m_listParameter.push_back(strToken);
					m_eReadState = READ_STATE_WORD;
				}
				else
					m_eReadState = READ_STATE_SPACE;
			}
			else
				m_eReadState = READ_STATE_STRING_START;
		}
		break;

		default: break;
	}

	return true;
}


bool CParameterReader::Analyze(std::string strBuffer)
{
	if (Separate(strBuffer))
	{
		m_eReadState = READ_STATE_START;

		for (TOKEN_LIST::iterator it = m_listToken.begin(); it != m_listToken.end(); it++)
		{
			if (!Analyze(it))
				return false;
		}

		return true;
	}
	
	return false;
}

