#pragma once


#include "Changer.h"


class CTextIdxChanger : public CChanger
{
public:
	CTextIdxChanger( CTSTrigger* pTSTrigger );
	~CTextIdxChanger( void );

public:
	virtual CString			Trigger( NTL_TS_T_ID tID, CAttrData* pAttr );
	virtual CString			Entity( NTL_TS_T_ID tID, NTL_TS_TG_ID tgID, NTL_TS_TC_ID tcID, CString strType, CAttrData* pAttr );

protected:
	CString					MakeString( unsigned int uiVal );
};
