#pragma once


#include "Changer.h"


class CMobGroupIdxChanger : public CChanger
{
protected:
	std::map< unsigned int, unsigned int > mapdef_ConvertList;

public:
	CMobGroupIdxChanger( CTSTrigger* pTSTrigger );
	~CMobGroupIdxChanger( void );

public:
	virtual CString			Trigger( NTL_TS_T_ID tID, CAttrData* pAttr );
	virtual CString			Entity( NTL_TS_T_ID tID, NTL_TS_TG_ID tgID, NTL_TS_TC_ID tcID, CString strType, CAttrData* pAttr );

protected:
	CString					ConvertMobGroupIdx( unsigned int uiVal );
	CString					MakeString( unsigned int uiVal );
	unsigned int			MakeUnsignedInt( CString& clVal );
};
