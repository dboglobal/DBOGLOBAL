#pragma once


#include "Changer.h"


class CRwdTblIdxChanger : public CChanger
{
protected:
	std::map< unsigned int, unsigned int > mapdef_ConvertList;

public:
	CRwdTblIdxChanger( CTSTrigger* pTSTrigger );
	~CRwdTblIdxChanger( void );

public:
	virtual CString			Trigger( NTL_TS_T_ID tID, CAttrData* pAttr );
	virtual CString			Entity( NTL_TS_T_ID tID, NTL_TS_TG_ID tgID, NTL_TS_TC_ID tcID, CString strType, CAttrData* pAttr );
};
