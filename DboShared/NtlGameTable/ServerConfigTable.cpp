#include "StdAfx.h"
#include "ServerConfigTable.h"
#include "NtlDebug.h"
#include "NtlSerializer.h"


#define SERVERCONFIG_TBLDAT_START(textname)							\
	if( 0 == wcscmp( pTbldat->wstrName.c_str(), textname) )			\
	{

#define SERVERCONFIG_TBLDAT_END()									\
	}															


#define SERVERCONFIG_TBLDAT_START_SEC(textname)						\
	if( 0 == wcscmp( pTbldat->wstrValue[0].c_str(), textname) )		\
	{

#define SERVERCONFIG_TBLDAT_END_SEC()								\
	}																


// BYTE
#define SERVERCONFIG_TBLDAT_SET_BYTE( table_loc, valuename, maxvalue)								\
	if( false == ReadByte( valuename, pTbldat->wstrValue[table_loc], maxvalue) )					\
	{																								\
		CTable::CallErrorCallbackFunction(															\
			L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s, Value = %s)",					\
			m_wszXmlFileName, pTbldat->wstrName.c_str(), pTbldat->wstrValue[table_loc].c_str() );	\
		return false;																				\
	}

// DWORD
#define SERVERCONFIG_TBLDAT_SET_DWORD( table_loc, valuename, maxvalue)								\
	if( false == ReadDWORD( valuename, pTbldat->wstrValue[table_loc], maxvalue) )					\
	{																								\
		CTable::CallErrorCallbackFunction(															\
			L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s, Value = %s)",					\
			m_wszXmlFileName, pTbldat->wstrName.c_str(), pTbldat->wstrValue[table_loc].c_str() );	\
		return false;																				\
	}

// TBLIDX
#define SERVERCONFIG_TBLDAT_SET_TBLIDX( table_loc, valuename, maxvalue)								\
	if( false == ReadTBLIDX( valuename, pTbldat->wstrValue[table_loc], maxvalue) )					\
	{																								\
		CTable::CallErrorCallbackFunction(															\
			L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s, Value = %s)",					\
			m_wszXmlFileName, pTbldat->wstrName.c_str(), pTbldat->wstrValue[table_loc].c_str() );	\
		return false;																				\
	}


#define SERVERCONFIG_TBLDAT_SET_FLOAT( table_loc, valuename, maxvalue)								\
	if( false == ReadFLOAT( valuename, pTbldat->wstrValue[table_loc], maxvalue) )					\
	{																								\
		CTable::CallErrorCallbackFunction(															\
			L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s, Value = %s)",					\
			m_wszXmlFileName, pTbldat->wstrName.c_str(), pTbldat->wstrValue[table_loc].c_str() );	\
		return false;																				\
	}


#define SERVERCONFIG_TBLDAT_SET_STR( table_loc, valuename)													\
	if( false == ReadSTR( valuename, BUDOKAI_MAX_TBLDAT_FILE_LENGTH, pTbldat->wstrValue[table_loc] ) )		\
	{																										\
		CTable::CallErrorCallbackFunction(																	\
			L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s, Value = %s)",							\
			m_wszXmlFileName, pTbldat->wstrName.c_str(), pTbldat->wstrValue[table_loc].c_str() );			\
		return false;																						\
	}

#define SERVERCONFIG_TBLDAT_SET_BOOL( table_loc, valuename)												\
	if( false == ReadBOOL( valuename, pTbldat->wstrValue[table_loc]) )									\
	{																									\
		CTable::CallErrorCallbackFunction(																\
			L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s, Value = %s)",						\
			m_wszXmlFileName, pTbldat->wstrName.c_str(), pTbldat->wstrValue[table_loc].c_str() );		\
		return false;																					\
	}


WCHAR* CServerConfigTable::m_pwszSheetList[] =
{
	L"Table_Data_KOR",
	NULL
};

CServerConfigTable::CServerConfigTable(void)
{
	Init();
}

CServerConfigTable::~CServerConfigTable(void)
{
	Destroy();
}

bool CServerConfigTable::Create(DWORD dwCodePage)
{
	return CTable::Create(dwCodePage);
}

void CServerConfigTable::Destroy()
{
}

void CServerConfigTable::Init()
{
	::memset( &m_sServerConfigData, 0xff, sizeof(m_sServerConfigData));
	for(int i = 0; i < MASCOT_LUMINOSITY_DROP_COUNT; i++)
		::memset( &m_aMascotLuminosityDropData[i], 0xff, sizeof(m_aMascotLuminosityDropData[i]));
}

void* CServerConfigTable::AllocNewTable(WCHAR* pwszSheetName, DWORD dwCodePage)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sSERVERCONFIG_TBLDAT* pNewHelp = new sSERVERCONFIG_TBLDAT;
		if (NULL == pNewHelp)
			return NULL;

		CPINFO cpInfo;
		if(false == GetCPInfo(dwCodePage, &cpInfo))
		{
			delete pNewHelp;
			return NULL;
		}
		
		m_dwCodePage = dwCodePage;
		return pNewHelp;
	}

	return NULL;
}

bool CServerConfigTable::DeallocNewTable(void* pvTable, WCHAR* pwszSheetName)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sSERVERCONFIG_TBLDAT* pHelp = (sSERVERCONFIG_TBLDAT*)pvTable;
		if (FALSE != IsBadReadPtr(pHelp, sizeof(*pHelp)))
			return false;

		delete pHelp;

		return true;
	}

	return false;
}

bool CServerConfigTable::AddTable(void * pvTable, bool bReload, bool bUpdate)
{
	UNREFERENCED_PARAMETER(bReload);
	UNREFERENCED_PARAMETER(bUpdate);

	sSERVERCONFIG_TBLDAT* pTbldat = (sSERVERCONFIG_TBLDAT*) pvTable;

	//byDragonballScrambleCoolTime
	SERVERCONFIG_TBLDAT_START(L"DragonballScrambleCoolTime_Hour")
		SERVERCONFIG_TBLDAT_SET_BYTE( 0, m_sServerConfigData.byDragonballScrambleCoolTime, INVALID_BYTE)
	SERVERCONFIG_TBLDAT_END()

	//sMASCOT_LUMINOSITY_DROP_DATA
	SERVERCONFIG_TBLDAT_START(L"Luminosity_Ring_Rate")
		SERVERCONFIG_TBLDAT_SET_BYTE( 0, m_aMascotLuminosityDropData[0].byCount, INVALID_BYTE)
		SERVERCONFIG_TBLDAT_SET_DWORD( 1, m_aMascotLuminosityDropData[0].dwRate, INVALID_DWORD)
		SERVERCONFIG_TBLDAT_SET_BYTE( 2, m_aMascotLuminosityDropData[1].byCount, INVALID_BYTE)
		SERVERCONFIG_TBLDAT_SET_DWORD( 3, m_aMascotLuminosityDropData[1].dwRate, INVALID_DWORD)
		SERVERCONFIG_TBLDAT_SET_BYTE( 4, m_aMascotLuminosityDropData[2].byCount, INVALID_BYTE)
		SERVERCONFIG_TBLDAT_SET_DWORD( 5, m_aMascotLuminosityDropData[2].dwRate, INVALID_DWORD)
		SERVERCONFIG_TBLDAT_SET_BYTE( 6, m_aMascotLuminosityDropData[3].byCount, INVALID_BYTE)
		SERVERCONFIG_TBLDAT_SET_DWORD( 7, m_aMascotLuminosityDropData[3].dwRate, INVALID_DWORD)
		SERVERCONFIG_TBLDAT_SET_BYTE( 8, m_aMascotLuminosityDropData[4].byCount, INVALID_BYTE)
		SERVERCONFIG_TBLDAT_SET_DWORD( 9, m_aMascotLuminosityDropData[4].dwRate, INVALID_DWORD)
	SERVERCONFIG_TBLDAT_END()

	//Common
	//Daily_Limit_Initial_Time
	SERVERCONFIG_TBLDAT_START(L"Common")
		SERVERCONFIG_TBLDAT_START_SEC(L"Daily_Limit_Initial_Time")
			SERVERCONFIG_TBLDAT_SET_BYTE( 1, m_sServerConfigData.byDailyLimitInitialTime, INVALID_BYTE)
		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()

	//TMQ
	//Daily_Limit_TMQ
	SERVERCONFIG_TBLDAT_START(L"TMQ")
		SERVERCONFIG_TBLDAT_START_SEC(L"Daily_Limit_TMQ")
			SERVERCONFIG_TBLDAT_SET_BYTE( 1, m_sServerConfigData.byDailyLimitTMQ, INVALID_BYTE)
		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()

	//BattleDungeon
	//BattleDungeon_World
	SERVERCONFIG_TBLDAT_START(L"BattleDungeon")
		SERVERCONFIG_TBLDAT_START_SEC(L"BattleDungeon_World")
			SERVERCONFIG_TBLDAT_SET_TBLIDX( 1, m_sServerConfigData.sBattleDungeonData.battleDungeonWorld, INVALID_TBLIDX)
		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()
	//EnterDirection
	SERVERCONFIG_TBLDAT_START(L"BattleDungeon")
		SERVERCONFIG_TBLDAT_START_SEC(L"EnterDirection")
			SERVERCONFIG_TBLDAT_SET_DWORD( 1, m_sServerConfigData.sBattleDungeonData.directPlay_EnterWorld, INVALID_DWORD)
		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()
	//StageChangeDirection
	SERVERCONFIG_TBLDAT_START(L"BattleDungeon")
		SERVERCONFIG_TBLDAT_START_SEC(L"StageChangeDirection")
			SERVERCONFIG_TBLDAT_SET_DWORD( 1, m_sServerConfigData.sBattleDungeonData.directPlay_StageChange, INVALID_DWORD)
		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()
	//StageChangeDirection
	SERVERCONFIG_TBLDAT_START(L"BattleDungeon")
		SERVERCONFIG_TBLDAT_START_SEC(L"StageChangeDirection_PlayTime")
			SERVERCONFIG_TBLDAT_SET_FLOAT( 1, m_sServerConfigData.sBattleDungeonData.fDirectPlay_StageChange_PlayTime, INVALID_FLOAT)
		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()
	//StageTriggerObject
	SERVERCONFIG_TBLDAT_START(L"BattleDungeon")
		SERVERCONFIG_TBLDAT_START_SEC(L"StageTriggerObject")
			SERVERCONFIG_TBLDAT_SET_TBLIDX( 1, m_sServerConfigData.sBattleDungeonData.stageTriggerObjectTblidx, INVALID_TBLIDX)
		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()

	//NormalStageLoc
	SERVERCONFIG_TBLDAT_START(L"BattleDungeon")
		SERVERCONFIG_TBLDAT_START_SEC(L"NormalStageLoc")
			SERVERCONFIG_TBLDAT_SET_FLOAT( 1, m_sServerConfigData.sBattleDungeonData.sEnterLoc_NormalStage.sLoc.x, INVALID_FLOAT)
			SERVERCONFIG_TBLDAT_SET_FLOAT( 2, m_sServerConfigData.sBattleDungeonData.sEnterLoc_NormalStage.sLoc.y, INVALID_FLOAT)
			SERVERCONFIG_TBLDAT_SET_FLOAT( 3, m_sServerConfigData.sBattleDungeonData.sEnterLoc_NormalStage.sLoc.z, INVALID_FLOAT)
			SERVERCONFIG_TBLDAT_SET_FLOAT( 4, m_sServerConfigData.sBattleDungeonData.sEnterLoc_NormalStage.sDir.x, INVALID_FLOAT)
											  m_sServerConfigData.sBattleDungeonData.sEnterLoc_NormalStage.sDir.y = 0.0f;
			SERVERCONFIG_TBLDAT_SET_FLOAT( 5, m_sServerConfigData.sBattleDungeonData.sEnterLoc_NormalStage.sDir.z, INVALID_FLOAT)
		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()

	//BossStageLoc
	SERVERCONFIG_TBLDAT_START(L"BattleDungeon")
		SERVERCONFIG_TBLDAT_START_SEC(L"BossStageLoc")
			
			DWORD dwIdx = INVALID_DWORD;
			if( false == ReadDWORD( dwIdx, pTbldat->wstrValue[1], INVALID_DWORD) )
			{
				CTable::CallErrorCallbackFunction( L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s, Value = %s)",
					m_wszXmlFileName, pTbldat->wstrName.c_str(), pTbldat->wstrValue[1].c_str() );
				return false;
			}

			BYTE byBossStageCount;
			switch(dwIdx)
			{
			case 5:		byBossStageCount = 0;	break;
			case 10:	byBossStageCount = 1;	break;
			case 15:	byBossStageCount = 2;	break;
			case 20:	byBossStageCount = 3;	break;
			case 25:	byBossStageCount = 4;	break;
			case 30:	byBossStageCount = 5;	break;
			case 35:	byBossStageCount = 6;	break;
			case 40:	byBossStageCount = 7;	break;
			case 45:	byBossStageCount = 8;	break;
			case 50:	byBossStageCount = 9;	break;
			case 55:	byBossStageCount = 10;	break;
			case 60:	byBossStageCount = 11;	break;
			case 65:	byBossStageCount = 12;	break;
			case 70:	byBossStageCount = 13;	break;
			case 75:	byBossStageCount = 14;	break;
			case 80:	byBossStageCount = 15;	break;
			case 85:	byBossStageCount = 16;	break;
			case 90:	byBossStageCount = 17;	break;
			case 95:	byBossStageCount = 18;	break;
			case 100:	byBossStageCount = 19;	break;
			case 105:	byBossStageCount = 20;	break;
			case 110:	byBossStageCount = 21;	break;
			case 115:	byBossStageCount = 22;	break;
			case 120:	byBossStageCount = 23;	break;
			case 125:	byBossStageCount = 24;	break;
			case 130:	byBossStageCount = 25;	break;
			case 135:	byBossStageCount = 26;	break;
			case 140:	byBossStageCount = 27;	break;
			case 145:	byBossStageCount = 28;	break;
			case 150:	byBossStageCount = 29;	break;
			default:
			//	CTable::CallErrorCallbackFunction( L"[File] : %s\n[Error] : Invalid Value. (Field Name = %s, Value = %s)",
			//		m_wszXmlFileName, pTbldat->wstrName.c_str(), pTbldat->wstrValue[1].c_str() );
				printf("FAAAAIL dwIdx %d \n", dwIdx);
				return false;
			}

			SERVERCONFIG_TBLDAT_SET_FLOAT( 2, m_sServerConfigData.sBattleDungeonData.aEnterLoc_BossStage[byBossStageCount].sLoc.x, INVALID_FLOAT)
			SERVERCONFIG_TBLDAT_SET_FLOAT( 3, m_sServerConfigData.sBattleDungeonData.aEnterLoc_BossStage[byBossStageCount].sLoc.y, INVALID_FLOAT)
			SERVERCONFIG_TBLDAT_SET_FLOAT( 4, m_sServerConfigData.sBattleDungeonData.aEnterLoc_BossStage[byBossStageCount].sLoc.z, INVALID_FLOAT)
			SERVERCONFIG_TBLDAT_SET_FLOAT( 5, m_sServerConfigData.sBattleDungeonData.aEnterLoc_BossStage[byBossStageCount].sDir.x, INVALID_FLOAT)
											  m_sServerConfigData.sBattleDungeonData.aEnterLoc_BossStage[byBossStageCount].sDir.y = 0.0f;
			SERVERCONFIG_TBLDAT_SET_FLOAT( 6, m_sServerConfigData.sBattleDungeonData.aEnterLoc_BossStage[byBossStageCount].sDir.z, INVALID_FLOAT)

		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()

	//LpRegen
	SERVERCONFIG_TBLDAT_START(L"BattleDungeon")
		SERVERCONFIG_TBLDAT_START_SEC(L"LpRegen")
			SERVERCONFIG_TBLDAT_SET_BOOL( 1, m_sServerConfigData.sBattleDungeonData.bLpRegen)
		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()
	//EpRegen
	SERVERCONFIG_TBLDAT_START(L"BattleDungeon")
		SERVERCONFIG_TBLDAT_START_SEC(L"EpRegen")
			SERVERCONFIG_TBLDAT_SET_BOOL( 1, m_sServerConfigData.sBattleDungeonData.bEpRegen)
		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()
	//ItemDrop
	SERVERCONFIG_TBLDAT_START(L"BattleDungeon")
		SERVERCONFIG_TBLDAT_START_SEC(L"ItemDrop")
			SERVERCONFIG_TBLDAT_SET_BOOL( 1, m_sServerConfigData.sBattleDungeonData.bItemDrop)
		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()
	//ItemDrop
	SERVERCONFIG_TBLDAT_START(L"BattleDungeon")
		SERVERCONFIG_TBLDAT_START_SEC(L"ItemUse")
			SERVERCONFIG_TBLDAT_SET_BOOL( 1, m_sServerConfigData.sBattleDungeonData.bItemUse)
		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()
	//ExpAdd
	SERVERCONFIG_TBLDAT_START(L"BattleDungeon")
		SERVERCONFIG_TBLDAT_START_SEC(L"ExpAdd")
			SERVERCONFIG_TBLDAT_SET_BOOL( 1, m_sServerConfigData.sBattleDungeonData.bExpAdd)
		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()
	//Mail_Sender_Tblidx
	SERVERCONFIG_TBLDAT_START(L"BattleDungeon")
		SERVERCONFIG_TBLDAT_START_SEC(L"Mail_Sender_Tblidx")
			SERVERCONFIG_TBLDAT_SET_TBLIDX( 1, m_sServerConfigData.sBattleDungeonData.mailSenderTblidx, INVALID_TBLIDX)
		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()
	//Mail_Text_Tblidx
	SERVERCONFIG_TBLDAT_START(L"BattleDungeon")
		SERVERCONFIG_TBLDAT_START_SEC(L"Mail_Text_Tblidx")
			SERVERCONFIG_TBLDAT_SET_TBLIDX( 1, m_sServerConfigData.sBattleDungeonData.mailTextTblidx, INVALID_TBLIDX)
		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()


	//DWC
	//Enter_Proposal_Wait_Time
	SERVERCONFIG_TBLDAT_START(L"DWC")
		SERVERCONFIG_TBLDAT_START_SEC(L"Enter_Proposal_Wait_Time")
			SERVERCONFIG_TBLDAT_SET_DWORD( 1, m_sServerConfigData.sDWCData.dwEnterProposalWaitTime, INVALID_DWORD)
		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()
	//Enter_CharDirection_Tblidx
	SERVERCONFIG_TBLDAT_START(L"DWC")
		SERVERCONFIG_TBLDAT_START_SEC(L"Enter_CharDirection_Tblidx")
			SERVERCONFIG_TBLDAT_SET_TBLIDX( 1, m_sServerConfigData.sDWCData.enterCharDirectionTblidx, INVALID_TBLIDX)
		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()
	//Enter_FailDirection_Tblidx
	SERVERCONFIG_TBLDAT_START(L"DWC")
		SERVERCONFIG_TBLDAT_START_SEC(L"Enter_FailDirection_Tblidx")
			SERVERCONFIG_TBLDAT_SET_TBLIDX( 1, m_sServerConfigData.sDWCData.enterFailDirectionTblidx, INVALID_TBLIDX)
		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()
	//Enter_Direction_Play_Time
	SERVERCONFIG_TBLDAT_START(L"DWC")
		SERVERCONFIG_TBLDAT_START_SEC(L"Enter_Direction_Play_Time")
			SERVERCONFIG_TBLDAT_SET_DWORD( 1, m_sServerConfigData.sDWCData.dwEnterDirectionPlayTime, INVALID_DWORD)
		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()
	//Mail_Sender_Tblidx
	SERVERCONFIG_TBLDAT_START(L"DWC")
		SERVERCONFIG_TBLDAT_START_SEC(L"Mail_Sender_Tblidx")
			SERVERCONFIG_TBLDAT_SET_TBLIDX( 1, m_sServerConfigData.sDWCData.mailSenderTblidx, INVALID_TBLIDX)
		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()
	//Mail_Text_Tblidx
	SERVERCONFIG_TBLDAT_START(L"DWC")
		SERVERCONFIG_TBLDAT_START_SEC(L"Mail_Text_Tblidx")
			SERVERCONFIG_TBLDAT_SET_TBLIDX( 1, m_sServerConfigData.sDWCData.mailTextTblidx, INVALID_TBLIDX)
		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()
	//Daily_Limit_DWC
	SERVERCONFIG_TBLDAT_START(L"DWC")
		SERVERCONFIG_TBLDAT_START_SEC(L"Daily_Limit_DWC")
			SERVERCONFIG_TBLDAT_SET_BYTE( 1, m_sServerConfigData.byDailyLimitDWC, INVALID_BYTE)
		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()
	//Fail_Flash_Tblidx
	SERVERCONFIG_TBLDAT_START(L"DWC")
		SERVERCONFIG_TBLDAT_START_SEC(L"Fail_Flash_Tblidx")
			SERVERCONFIG_TBLDAT_SET_TBLIDX( 1, m_sServerConfigData.sDWCData.failFlashTblidx, INVALID_TBLIDX)
		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()
	//Fail_State_Wait_Time
	SERVERCONFIG_TBLDAT_START(L"DWC")
		SERVERCONFIG_TBLDAT_START_SEC(L"Fail_State_Wait_Time")
			SERVERCONFIG_TBLDAT_SET_DWORD( 1, m_sServerConfigData.sDWCData.dwFailStateWaitTime, INVALID_DWORD)
		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()


	//SKDungeon
	//Daily_Limit_SKDungeon
	SERVERCONFIG_TBLDAT_START(L"SKDungeon")
		SERVERCONFIG_TBLDAT_START_SEC(L"Daily_Limit_SKDungeon")
			SERVERCONFIG_TBLDAT_SET_BYTE( 1, m_sServerConfigData.byDailyLimitSKDungeon, INVALID_BYTE)
		SERVERCONFIG_TBLDAT_END_SEC()
	SERVERCONFIG_TBLDAT_END()


	//ItemUpgradeSpecialSource
	SERVERCONFIG_TBLDAT_START(L"ItemUpgradeSpecialSource")
		SERVERCONFIG_TBLDAT_SET_BYTE( 0, m_sServerConfigData.byItemUpgradeSpecialSource[0], INVALID_BYTE)
		SERVERCONFIG_TBLDAT_SET_BYTE( 1, m_sServerConfigData.byItemUpgradeSpecialSource[1], INVALID_BYTE)
		SERVERCONFIG_TBLDAT_SET_BYTE( 2, m_sServerConfigData.byItemUpgradeSpecialSource[2], INVALID_BYTE)
		SERVERCONFIG_TBLDAT_SET_BYTE( 3, m_sServerConfigData.byItemUpgradeSpecialSource[3], INVALID_BYTE)
	SERVERCONFIG_TBLDAT_END()


	if ( false == m_mapTableList.insert(std::pair<TBLIDX, sTBLDAT*>(pTbldat->tblidx, pTbldat)).second )
	{
		CTable::CallErrorCallbackFunction(L"[File] : %s\r\n Table Tblidx[%u] is Duplicated ",m_wszXmlFileName, pTbldat->tblidx );
		_ASSERTE( 0 );
		return false;
	}


	return true;
}

bool CServerConfigTable::SetTableData(void* pvTable, WCHAR* pwszSheetName, std::wstring* pstrDataName, BSTR bstrData)
{
	if (0 == wcscmp(pwszSheetName, L"Table_Data_KOR"))
	{
		sSERVERCONFIG_TBLDAT* pHelp = (sSERVERCONFIG_TBLDAT*)pvTable;

		if (0 == wcscmp(pstrDataName->c_str(), L"Tblidx"))
		{
			pHelp->tblidx = READ_DWORD( bstrData );
		}


		else
		{
			CTable::CallErrorCallbackFunction(L"[File] : %s\n[Error] : Unknown field name found!(Field Name = %s)", m_wszXmlFileName, pstrDataName->c_str());
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}


sTBLDAT* CServerConfigTable::FindData(TBLIDX tblidx)
{
	if (0 == tblidx)
		return NULL;

	TABLEIT iter;
	iter = m_mapTableList.find(tblidx);
	if (End() == iter)
		return NULL;
	
	return (sTBLDAT*)(iter->second); 
}

//#include <iostream>
bool CServerConfigTable::LoadFromBinary(CNtlSerializer& serializer, bool bReload, bool bUpdate)
{
	if( false == bReload )
	{
		Reset();
	}

	BYTE byMargin = 1;
	serializer >> byMargin;

	while (0 < serializer.GetDataSize())
	{
		sSERVERCONFIG_TBLDAT* pTableData = new sSERVERCONFIG_TBLDAT;
		if (NULL == pTableData)
		{
			Destroy();
			return false;
		}

		// tblidx
		if( serializer.GetDataSize() < sizeof(pTableData->tblidx) )
		{
			delete pTableData;
			Destroy();
			return false;
		}

		serializer >> pTableData->tblidx;

		// name
		if( false == GetBinaryText( pTableData->wstrName, serializer))
		{
			delete pTableData;
			Destroy();
			return false;
		}


		// value
		if( false == GetBinaryText( pTableData->wstrValue[0], serializer))
		{
			delete pTableData;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTableData->wstrValue[1], serializer))
		{
			delete pTableData;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTableData->wstrValue[2], serializer))
		{
			delete pTableData;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTableData->wstrValue[3], serializer))
		{
			delete pTableData;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTableData->wstrValue[4], serializer))
		{
			delete pTableData;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTableData->wstrValue[5], serializer))
		{
			delete pTableData;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTableData->wstrValue[6], serializer))
		{
			delete pTableData;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTableData->wstrValue[7], serializer))
		{
			delete pTableData;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTableData->wstrValue[8], serializer))
		{
			delete pTableData;
			Destroy();
			return false;
		}

		if( false == GetBinaryText( pTableData->wstrValue[9], serializer))
		{
			delete pTableData;
			Destroy();
			return false;
		}


		/*printf("pTableData->tblidx %d \n", pTableData->tblidx);
		std::wcout << pTableData->wstrName << "\n";
		std::wcout << pTableData->wstrValue[0] << "\n";
		std::wcout << pTableData->wstrValue[1] << "\n\n";*/

		if( false == AddTable(pTableData, bReload, bUpdate) )
		{
			delete pTableData;
		}

	}

	return true;
}

bool CServerConfigTable::SaveToBinary(CNtlSerializer& serializer)
{
	serializer.Refresh();

	BYTE byMargin = 1;
	serializer << byMargin;

	TABLE::iterator iter;
	for (iter = Begin() ; End() != iter ; iter++)
	{
		sSERVERCONFIG_TBLDAT* pTableData = (sSERVERCONFIG_TBLDAT*)(iter->second);

		pTableData->SaveToBinary(serializer);
	}

	return true;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CServerConfigTable::ReadByte( BYTE & rDest, std::wstring & wstrValue, BYTE byMaxValue)
{
	__int64 nTemp = INVALID_BYTE;

	if( 0 != wstrValue.size())
	{
		nTemp = _wtoi64(wstrValue.c_str());
	}
	
	if( 0 > nTemp || byMaxValue < nTemp)
	{
		rDest = INVALID_BYTE;
		return false;
	}

	rDest = (BYTE) nTemp;
	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CServerConfigTable::ReadDWORD( DWORD & rDest, std::wstring & wstrValue, DWORD dwMaxValue)
{
	__int64 nTemp = INVALID_DWORD;
	if(0 != wstrValue.size())
	{
		nTemp = _wtoi64(wstrValue.c_str());
	}

	if( dwMaxValue < nTemp)
	{
		rDest = INVALID_DWORD;
		return false;
	}

	rDest = (DWORD)nTemp;

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CServerConfigTable::ReadTBLIDX( TBLIDX & rDest, std::wstring & wstrValue, TBLIDX dwMaxTblidx)
{
	__int64 nTemp = INVALID_TBLIDX;
	if(0 != wstrValue.size())
	{
		nTemp = _wtoi64(wstrValue.c_str());
	}

	if( dwMaxTblidx < nTemp)
	{
		rDest = INVALID_TBLIDX;

		return false;
	}

	rDest = (TBLIDX)nTemp;

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CServerConfigTable::ReadFLOAT( float &rDest, std::wstring & wstrValue, float fMaxValue)
{
	double fTemp = INVALID_FLOAT;

	if(0 != wstrValue.size())
	{
		fTemp = _wtof(wstrValue.c_str());
	}

	if( fMaxValue < fTemp)
	{
		rDest = INVALID_FLOAT;
		return false;
	}

	rDest = (float)fTemp;

	return true;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CServerConfigTable::ReadSTR( WCHAR * pDest, DWORD dwDestLength, std::wstring & wstrSrc)
{
#ifdef _DEVEL
	if (FALSE != IsBadWritePtr( pDest, sizeof(WCHAR) * dwDestLength) )
#else
	if (NULL == pDest)
#endif
	{
		//- yoshiki : To log system!
		return false;
	}

	if ( wstrSrc.c_str()[0] != L'@')
	{
		if ( dwDestLength < (wstrSrc.length() + 1))
		{
			if (NULL != m_pfnErrorCallback)
			{
				CallErrorCallbackFunction(L"[File] : %s\n[Error] : The string[%s]'s length[%u] is bigger than the max. length[%u]",
					m_wszXmlFileName, wstrSrc.c_str(), wstrSrc.length(), dwDestLength - 1);
			}

			return false;
		}
		else
		{
			wcscpy_s( pDest, dwDestLength, wstrSrc.c_str());
		}
	}
	else
	{
		pDest[0] = L'\0';
	}

	return true;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CServerConfigTable::ReadBOOL( bool & rDest, std::wstring & wstrValue)
{
	if(0 == wcscmp( wstrValue.c_str(), L"false"))
		rDest = false;
	else if (0 == wcscmp( wstrValue.c_str(), L"true"))
		rDest = true;
	else
		return false;

	return false;
}

//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
bool CServerConfigTable::GetBinaryText(std::wstring & wstrValue, CNtlSerializer& serializer)
{
	// text length
	WORD wTextLength = 0;
	if(serializer.GetDataSize() < sizeof(wTextLength))
	{
		return false;
	}

	serializer >> wTextLength;


	// value
	if (serializer.GetDataSize() < (int)(wTextLength * sizeof(WCHAR)))
	{
		return false;
	}

	WCHAR* pwszText = new WCHAR[wTextLength + 1];
	if (NULL == pwszText)
	{
		return false;
	}

	serializer.Out(pwszText, wTextLength * sizeof(WCHAR));
	pwszText[wTextLength] = L'\0';

	wstrValue = pwszText;

	delete [] pwszText;

	return true;
}
