#ifndef __SERVER_PARAMETER_READER__
#define __SERVER_PARAMETER_READER__


#include "StringSeparator.h"

class CParameterReader : public CStringSeparator
{
	enum eREAD_STATE
	{
		READ_STATE_START,
		READ_STATE_WORD,
		READ_STATE_STRING_START,
		READ_STATE_STRING_END,
		READ_STATE_SPACE,
		READ_STATE_END,
	};

public:

	struct sNUMBER
	{
		sNUMBER(bool _bSuccess, float _fNumber)
		{
			bSuccess = _bSuccess;
			fNumber = _fNumber;
		}

		bool bSuccess;
		float fNumber;
	};


	CParameterReader();
	virtual ~CParameterReader();


public:

	std::string				GetString(int nLoc);

	sNUMBER					GetNumber(int nLoc);

	int						GetCount(){ return (int)m_listParameter.size(); }


protected:

	bool					Analyze(TOKEN_IT itToken);

public:

	bool					Analyze(std::string strBuffer);


private:

	eREAD_STATE				m_eReadState;

	std::list<std::string>	m_listParameter;

	std::string				m_strString;

};


#endif