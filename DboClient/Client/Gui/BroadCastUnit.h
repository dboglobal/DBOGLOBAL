/******************************************************************************
* File			: BroadCastUnit.h
* Author		: Haesung Cho
* Copyright		: (주)NTL
* Date			: 2008. 10. 27
* Abstract		: 
*****************************************************************************
* Desc			: 게임 TS message, Server emergency, Mini narration을 담당하는 GUI의 Unit
*****************************************************************************/

#ifndef __BROAD_CAST_UNIT_H__
#define __BROAD_CAST_UNIT_H__

#include "NtlSLEvent.h"
#include "DboTSCoreDefine.h"

class CBroadCastUnit
{
public:
	CBroadCastUnit();
	CBroadCastUnit( RwUInt8 byMsgType, vecdef_BroadMsgDataList& vecMsgData );
	CBroadCastUnit( const CBroadCastUnit& unit );
	virtual ~CBroadCastUnit();

	VOID					SetData( RwUInt8 byMsgType, vecdef_BroadMsgDataList& vecMsgData );

	RwUInt8					m_byMsgType;
	vecdef_BroadMsgDataList	m_vecMsgDataList;
};

#endif//__BROAD_CAST_UNIT_H__