#include "precomp_dboclient.h"
#include "BroadCastUnit.h"

CBroadCastUnit::CBroadCastUnit()
{
	m_byMsgType = INVALID_BYTE;
}

CBroadCastUnit::CBroadCastUnit( RwUInt8 byMsgType, vecdef_BroadMsgDataList& vecMsgData )
{
	m_byMsgType = byMsgType;
	m_vecMsgDataList = vecMsgData;	
}

CBroadCastUnit::CBroadCastUnit( const CBroadCastUnit& unit )
{
	m_byMsgType = unit.m_byMsgType;
	
	vecdef_BroadMsgDataList::const_iterator it = unit.m_vecMsgDataList.begin();
	while( it != unit.m_vecMsgDataList.end() )
	{
		sBROAD_MSG_DATA data;
		data.eUIDirType				= (*it).eUIDirType;
		data.eUIShowHideType		= (*it).eUIShowHideType;
		data.eUIBalloonShapeType	= (*it).eUIBalloonShapeType;
		data.eUISpeechDirType		= (*it).eUISpeechDirType;
		data.eOwnerCondition		= (*it).eOwnerCondition;
		data.eOwnerType				= (*it).eOwnerType;
		data.uiOwnerTblIdx			= (*it).uiOwnerTblIdx;
		data.wstrSpeech				= (*it).wstrSpeech;
		data.fDisplayTime			= (*it).fDisplayTime;

		m_vecMsgDataList.push_back( data );
			
		it++;
	}
}

CBroadCastUnit::~CBroadCastUnit()
{

}

VOID CBroadCastUnit::SetData( RwUInt8 byMsgType, vecdef_BroadMsgDataList& vecMsgData )
{
	m_byMsgType			= byMsgType;
	
	m_vecMsgDataList.clear();
	vecdef_BroadMsgDataList::const_iterator it = vecMsgData.begin();
	while( it != vecMsgData.end() )
	{
		sBROAD_MSG_DATA data;
		data.eUIDirType				= (*it).eUIDirType;
		data.eUIShowHideType		= (*it).eUIShowHideType;
		data.eUIBalloonShapeType	= (*it).eUIBalloonShapeType;
		data.eUISpeechDirType		= (*it).eUISpeechDirType;
		data.eOwnerCondition		= (*it).eOwnerCondition;
		data.eOwnerType				= (*it).eOwnerType;
		data.uiOwnerTblIdx			= (*it).uiOwnerTblIdx;
		data.wstrSpeech				= (*it).wstrSpeech;
		data.fDisplayTime			= (*it).fDisplayTime;

		m_vecMsgDataList.push_back( data );

		it++;
	}
}