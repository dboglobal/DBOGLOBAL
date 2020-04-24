#include "precomp_ntlsimulation.h"
#include "NtlSobDynamicObjectAttr.h"
#include "NtlDebug.h"

// shared
#include "TableContainer.h"
#include "DynamicObjectTable.h"
#include "ItemMixMachineTable.h"
#include "TextAllTable.h"

// Simualtion
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"

#include "NtlSLLogic.h"
#include "NtlSLApi.h" 

DEFINITION_MEMORY_POOL(CNtlSobDynamicObjectAttr)

CNtlSobDynamicObjectAttr::CNtlSobDynamicObjectAttr()
{
	m_pDynamicObjTbl = NULL;
	m_byDynamicObjectType = eDBO_DYNAMIC_OBJECT_TYPE_NONE;
	m_idxTableForType = INVALID_TBLIDX;
}

void CNtlSobDynamicObjectAttr::HandleEvents( RWS::CMsg& pMsg )
{
	if(pMsg.Id == g_EventSobCreate)
	{
		SNtlEventSobDynamicObjectCreate* pDynamicObjectCreate = reinterpret_cast<SNtlEventSobDynamicObjectCreate*>( pMsg.pData );

		// 1. Type 저장 TableIdx 저장
		m_byDynamicObjectType = pDynamicObjectCreate->byType;
		m_idxTableForType = pDynamicObjectCreate->uiTableIndexForType;
		
		// 2. Type 에 맞춰서 테이블을 꺼낸다.
		//  2-1. 이름을 세팅한다. ( 누구누구의 무슨무슨 머신을 고려해본다. )
		
		CDynamicObjectTable* pDynamicObjectTbl = API_GetTableContainer()->GetDynamicObjectTable();
		NTL_ASSERT(pDynamicObjectTbl, "CNtlSobDynamicObjectAttr::HandleEvents => Dynamic ObjectTable is null" );

		sDYNAMIC_OBJECT_TBLDAT* pDynamicObjTblData = NULL;
		switch( m_byDynamicObjectType )
		{
		case eDBO_DYNAMIC_OBJECT_TYPE_HOIPOI_MIX_MACHINE:
			{
				CItemMixMachineTable* pMachineTable = API_GetTableContainer()->GetItemMixMachineTable();
				NTL_ASSERT( pMachineTable, "ItemMixMachine Table is null");
				
				sITEM_MIX_MACHINE_TBLDAT* pTblData = (sITEM_MIX_MACHINE_TBLDAT*)pMachineTable->FindData( m_idxTableForType );
				NTL_ASSERT( pTblData, "ItemMixMachine Table index is invalid." << m_idxTableForType );

				pDynamicObjTblData = (sDYNAMIC_OBJECT_TBLDAT*)pDynamicObjectTbl->FindData( pTblData->dynamicObjectTblidx );
				NTL_ASSERT( pDynamicObjTblData, "Dynamic object table index is invalid." << pTblData->dynamicObjectTblidx );

				CTextTable *pTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();
				sTEXT_TBLDAT* pTextTblData = reinterpret_cast<sTEXT_TBLDAT*>( pTextTable->FindData( pTblData->name ) );

				if(pTextTblData && pTextTblData->wstrText.size() > 0 )
				{
					SetName(pTextTblData->wstrText.c_str());
					SetNameCreate( TRUE );
				}
				else
				{
					WCHAR awcErrBuffer[32];
					swprintf_s( awcErrBuffer, 32, L"(ITEM_T_TBL)%d", pTblData->name );
					SetName( awcErrBuffer );
					SetNameCreate(TRUE);
				}

				SetNameColor(NTL_OBJ_NAME_COLOR);
			}
			break;
		default:
			{
				NTL_ASSERTFAIL( "Invalid dynamic object type - " << m_byDynamicObjectType );
			}
			break;
		}

		if( pDynamicObjTblData == NULL )
		{
			NTL_ASSERTFAIL( "Dynamic Object Create Failed - Table data is invalid" );
			return;
		}

		SetDynamicObjectTbl( pDynamicObjTblData );
	}
}

void CNtlSobDynamicObjectAttr::SetDynamicObjectTbl( const sDYNAMIC_OBJECT_TBLDAT* pDynamicObjTbl )
{
	m_pDynamicObjTbl = const_cast<sDYNAMIC_OBJECT_TBLDAT*>( pDynamicObjTbl );
}

RwUInt8 CNtlSobDynamicObjectAttr::GetDynamicObjectType()
{
	return m_byDynamicObjectType;
}

TBLIDX CNtlSobDynamicObjectAttr::GetTableIndexForType()
{
	return m_idxTableForType;
}