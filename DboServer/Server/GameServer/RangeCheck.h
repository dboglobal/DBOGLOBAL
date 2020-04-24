#ifndef __INC_DBOG_UTIL_RANGECHECK_H__
#define __INC_DBOG_UTIL_RANGECHECK_H__


class CCharacter;
#include <vector>
#include <map>

class CRangeCheck
{
	struct sRANGECHECK_DATA
	{
		sRANGECHECK_DATA::sRANGECHECK_DATA()
		{
			pChar = NULL;
			fRangeDiff = 0.0f;
		}
		sRANGECHECK_DATA::sRANGECHECK_DATA(CCharacter* _pChar, float _fRangeDiff)
		{
			pChar = _pChar;
			fRangeDiff = _fRangeDiff;
		}

		CCharacter* pChar;
		float fRangeDiff;
	};

	typedef std::multimap<float, sRANGECHECK_DATA> RANGECHECK_MAP;
	typedef RANGECHECK_MAP::value_type RANGECHECK_VALUE;


public:

	enum eRANGECHECK_TYPE
	{
		eSINGLENESS = 0x0,
		eSEVERAL = 0x1,
	};

public:
	CRangeCheck(CCharacter* pChar, CRangeCheck::eRANGECHECK_TYPE eRangeCheckType);
	virtual	~CRangeCheck();

public:

	void				AddBot(CCharacter* pChar);
	CCharacter*			ResultSinglenessBot();
	void				ResultSeveralBot(std::vector<CCharacter*>& rBotList, int nContainerSize);
	bool				IsEmptySeveral();

private:

	void				AddSinglenessBot(CCharacter* pChar);
	void				AddSeveralBot(CCharacter* pChar);

private:

	CCharacter*			m_pParent;
	eRANGECHECK_TYPE	m_eRangeCheckType;
	sRANGECHECK_DATA	m_sRangeCheckData_Singleness;
	RANGECHECK_MAP		m_mapRangeCheck;

};

#endif
