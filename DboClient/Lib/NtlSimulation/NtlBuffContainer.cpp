#include "precomp_ntlsimulation.h"
#include "NtlBuffContainer.h"

// core
#include "NtlDebug.h"

// shared
#include "SkillTable.h"
#include "ItemTable.h"
#include "UseItemTable.h"
#include "SystemEffectTable.h"
#include "TableContainer.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSobBuff.h"
#include "NtlSobBuffAttr.h"
#include "NtlSLApi.h"

// gui
#include "gui_texturemanager.h"


//////////////////////////////////////////////////////////////////////////
// CNtlBuffContainer
//////////////////////////////////////////////////////////////////////////


CNtlBuffContainer::CNtlBuffContainer()
{
	m_pSobObj = NULL;
}

CNtlBuffContainer::~CNtlBuffContainer()
{

}

RwBool CNtlBuffContainer::Create(void)
{
	return TRUE;
}

void CNtlBuffContainer::Destroy(void)
{	
	CNtlSobBuff *pSobBuff;
	ListBuff::iterator it;

	for(it = m_listBuff.begin(); it != m_listBuff.end(); ++it)
	{
		pSobBuff = (*it);
		CNtlSLEventGenerator::SobDelete(pSobBuff->GetSerialID());
	}

	m_listBuff.clear();
}

void CNtlBuffContainer::HandleEvents(RWS::CMsg &pMsg) 
{
	if(pMsg.Id == g_EventSobBuffAdd)
	{
		AddEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobBuffDrop)
	{
		DropEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobBuffRefreshAll)
	{
		RefreshAllEventHandler(pMsg);
	}
	else if (pMsg.Id == g_EventSobBuffActivate)
	{
		ActivateEventHandler(pMsg);
	}
	else if (pMsg.Id == g_EventSobBuffAutoRecover)
	{
		AutoRecoverEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobCreate)
	{
		CreateEventHandler(pMsg);
	}
}

void CNtlBuffContainer::CreateEventHandler(RWS::CMsg &pMsg)
{
	for( RwInt32 i = 0 ; i < GetNtlSLGlobal()->GetAvatarInfo()->byBuffCount ; ++i )
	{
		sBUFF_INFO* pBuffInfo = &GetNtlSLGlobal()->GetAvatarInfo()->sBuffPf[i];
		CNtlSobBuff *pSobBuff = reinterpret_cast<CNtlSobBuff*>(CNtlSLEventGenerator::SobBuffCreate(SLCLASS_BUFF, INVALID_SERIAL_ID, pBuffInfo->buffIndex , pBuffInfo->bySourceType, pBuffInfo->sourceTblidx, pBuffInfo->dwTimeRemaining, pBuffInfo->dwInitialDuration, pBuffInfo->aBuffParameter, pBuffInfo->bActive));
		pSobBuff->SetOwnerID(m_pSobObj->GetSerialID());
		m_listBuff.push_back(pSobBuff);	
	}
}

void CNtlBuffContainer::AddEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventSobBuffAdd *pBuffAdd = reinterpret_cast<SNtlEventSobBuffAdd*>(pMsg.pData);

	CNtlSobBuff *pSobBuff = reinterpret_cast<CNtlSobBuff*>( CNtlSLEventGenerator::SobBuffCreate(SLCLASS_BUFF, INVALID_SERIAL_ID, pBuffAdd->byBuffIndex, pBuffAdd->byBuffType, pBuffAdd->uiTblId, pBuffAdd->uiRemainTime, pBuffAdd->uiKeepTime, pBuffAdd->aBuffParameter, pBuffAdd->bActive) );
	pSobBuff->SetOwnerID(m_pSobObj->GetSerialID());
	m_listBuff.push_back(pSobBuff);

	CNtlSLEventGenerator::SobUpdate(m_pSobObj->GetSerialID(), EVENT_AIUT_BUFF, 0);
}

void CNtlBuffContainer::DropEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventSobBuffDrop *pBuffDrop = reinterpret_cast<SNtlEventSobBuffDrop*>(pMsg.pData);

	CNtlSobBuff *pSobBuff;
	CNtlSobBuffAttr *pSobBuffAttr;
	ListBuff::iterator it;

	for(it = m_listBuff.begin(); it != m_listBuff.end(); ++it)
	{
		pSobBuff = (*it);
		pSobBuffAttr = reinterpret_cast<CNtlSobBuffAttr*>( pSobBuff->GetSobAttr() );
		if(pSobBuffAttr->GetBuffType() != pBuffDrop->byBuffType)
			continue;

		if(pSobBuffAttr->GetBuffIndex() == pBuffDrop->byBuffIndex)
		{
			CNtlSLEventGenerator::SobDelete(pSobBuff->GetSerialID());
			m_listBuff.erase(it);
			break;
		}
	}

	CNtlSLEventGenerator::SobUpdate(m_pSobObj->GetSerialID(), EVENT_AIUT_BUFF, 0);
}

void CNtlBuffContainer::RefreshAllEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventSobBuffRefreshAll *pBuffRefreshAll = reinterpret_cast<SNtlEventSobBuffRefreshAll*>(pMsg.pData);

	// Buff 지우기.
	CNtlSobBuff *pSobBuff;
	ListBuff::iterator it;

	for(it = m_listBuff.begin(); it != m_listBuff.end(); ++it)
	{
		pSobBuff = (*it);
		CNtlSLEventGenerator::SobDelete(pSobBuff->GetSerialID());
	}

	m_listBuff.clear();

	// Buff 채우기.
	for (int i = 0; i < pBuffRefreshAll->byCount; ++i)
	{
		pSobBuff = reinterpret_cast<CNtlSobBuff*>(CNtlSLEventGenerator::SobBuffCreate(SLCLASS_BUFF, INVALID_SERIAL_ID, pBuffRefreshAll->pBuffInfo[i].buffIndex, pBuffRefreshAll->pBuffInfo[i].bySourceType, pBuffRefreshAll->pBuffInfo[i].sourceTblidx, 
			pBuffRefreshAll->pBuffInfo[i].dwTimeRemaining, pBuffRefreshAll->pBuffInfo[i].dwInitialDuration, pBuffRefreshAll->pBuffInfo[i].aBuffParameter, pBuffRefreshAll->pBuffInfo[i].bActive));
		
		pSobBuff->SetOwnerID(m_pSobObj->GetSerialID());

		m_listBuff.push_back(pSobBuff);
	}

	CNtlSLEventGenerator::SobUpdate(m_pSobObj->GetSerialID(), EVENT_AIUT_BUFF, 0);
}

void CNtlBuffContainer::ActivateEventHandler(RWS::CMsg & pMsg)
{
	SNtlEventSobBuffActivate *pBuff = reinterpret_cast<SNtlEventSobBuffActivate*>(pMsg.pData);

	CNtlSobBuff *pSobBuff;
	CNtlSobBuffAttr *pSobBuffAttr;
	ListBuff::iterator it;

	for (it = m_listBuff.begin(); it != m_listBuff.end(); ++it)
	{
		pSobBuff = (*it);
		pSobBuffAttr = reinterpret_cast<CNtlSobBuffAttr*>(pSobBuff->GetSobAttr());

		if (pSobBuffAttr->GetBuffIndex() == pBuff->pBuffInfo->buffIndex)
		{
			pSobBuffAttr->SetBuffActive(pBuff->pBuffInfo->bActive);

			//CSobBuff::Update will show/hide the disable icon
			pSobBuff->HandleEvents(pMsg);

			break;
		}
	}
}

void CNtlBuffContainer::AutoRecoverEventHandler(RWS::CMsg & pMsg)
{
	SNtlEventSobBuffAutoRecover *pBuff = reinterpret_cast<SNtlEventSobBuffAutoRecover*>(pMsg.pData);

	CNtlSobBuff *pSobBuff;
	CNtlSobBuffAttr *pSobBuffAttr;
	ListBuff::iterator it;

	for (it = m_listBuff.begin(); it != m_listBuff.end(); ++it)
	{
		pSobBuff = (*it);
		pSobBuffAttr = reinterpret_cast<CNtlSobBuffAttr*>(pSobBuff->GetSobAttr());

		if (pSobBuffAttr->GetBuffIndex() == pBuff->byBuffIndex)
		{
			sDBO_BUFF_PARAMETER* pBuffParam_0 = pSobBuffAttr->GetBuffParameter(0);

			pBuffParam_0->buffParameter.dwRemainValue = pBuff->dwRemainValue;
			pBuffParam_0->buffParameter.dwRemainTime = pBuff->dwRemainTime;

			pSobBuff->HandleEvents(pMsg);

			break;
		}
	}
}


void CNtlBuffContainer::SetObj(const CNtlSob *pSobObj)
{
	m_pSobObj = const_cast<CNtlSob*>(pSobObj);
}

RwInt32 CNtlBuffContainer::GetBuffCount(void)
{
	return (RwInt32)m_listBuff.size();
}

CNtlBuffContainer::ListBuff::iterator CNtlBuffContainer::GetBeginBuff(void)
{
	return m_listBuff.begin();
}

CNtlBuffContainer::ListBuff::iterator CNtlBuffContainer::GetEndBuff(void)
{
	return m_listBuff.end();
}

//////////////////////////////////////////////////////////////////////////
// sNtlFakeBuff
//////////////////////////////////////////////////////////////////////////

void sNtlFakeBuff::CreateImage(const RwChar* pFileName)
{
	DeleteImage();

	std::string buf = "icon\\";
	buf += pFileName;

	pImage = reinterpret_cast<void*>(g_TextureMgr.CreateTexture(buf.c_str()));

	if(!pImage)
	{
		pImage = reinterpret_cast<void*>(g_TextureMgr.CreateTexture("icon\\DEF_NTL_ICO.png"));
	}
}

void sNtlFakeBuff::DeleteImage(void)
{
	if(pImage)
	{
		g_TextureMgr.DeleteTexture(reinterpret_cast<gui::CTexture*>( pImage ));
		pImage = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
// CNtlFakeBuffContainer
//////////////////////////////////////////////////////////////////////////

CNtlFakeBuffContainer::CNtlFakeBuffContainer()
{

}

CNtlFakeBuffContainer::~CNtlFakeBuffContainer()
{

}

RwBool CNtlFakeBuffContainer::Create(SERIAL_HANDLE hOwner)
{
	NTL_FUNCTION("CNtlFakeBuffContainer::Create");

	m_hOwner = hOwner;

	NTL_RETURN(TRUE);
}

void CNtlFakeBuffContainer::Destroy()
{
	NTL_FUNCTION("CNtlFakeBuffContainer::Destroy");

	LIST_FAKE_BUFF_ITER it = m_listBuff.begin();
	for( ; it != m_listBuff.end() ; ++it )
	{
		sNtlFakeBuff* pNtlFakeBuff = *it;
		NTL_DELETE(pNtlFakeBuff);
	}

	m_listBuff.clear();

	NTL_RETURNVOID();
}

void CNtlFakeBuffContainer::Update(RwReal fElapsed)
{
	sNtlFakeBuff* pFakeBuff;
	LIST_FAKE_BUFF_ITER it = m_listBuff.begin();
	for( ; it != m_listBuff.end() ; ++it )
	{
		pFakeBuff = *it;

		pFakeBuff->fTime += fElapsed;

		if( pFakeBuff->fTime >= pFakeBuff->fMaxTime )
			pFakeBuff->fTime = pFakeBuff->fMaxTime;		
	}
}

void CNtlFakeBuffContainer::AddBuff(eDBO_OBJECT_SOURCE eType, BYTE byBuffIndex, RwUInt32 uiTableIndex, RwReal fRemainTime, bool bActive, sDBO_BUFF_PARAMETER* aBuffParameter /* = NULL */)
{
	sNtlFakeBuff* pNewFakeBuff = NTL_NEW sNtlFakeBuff;

	pNewFakeBuff->byBuffIndex = byBuffIndex;
	pNewFakeBuff->eType = eType;
	pNewFakeBuff->bActive = bActive;
	
	if(pNewFakeBuff->eType == DBO_OBJECT_SOURCE_SKILL)
	{
		pNewFakeBuff->pSkillTbl = reinterpret_cast<sSKILL_TBLDAT*>( API_GetTableContainer()->GetSkillTable()->FindData(uiTableIndex) );
		if( !pNewFakeBuff->pSkillTbl )
		{
			DBO_FAIL("Not exist skill table of index : " << uiTableIndex);
			return;
		}
		
		pNewFakeBuff->fMaxTime = (RwReal)pNewFakeBuff->pSkillTbl->wKeep_Time;
		pNewFakeBuff->CreateImage( pNewFakeBuff->pSkillTbl->szIcon_Name );

		for( RwInt32 i = 0 ; i < NTL_MAX_EFFECT_IN_SKILL ; ++i )
		{
			sSYSTEM_EFFECT_TBLDAT* pSystemEffectTable = reinterpret_cast<sSYSTEM_EFFECT_TBLDAT*>( API_GetTableContainer()->GetSystemEffectTable()->FindData( pNewFakeBuff->pSkillTbl->skill_Effect[i] ) );

			if( !pSystemEffectTable )
				continue;

			if( i == 0 )
				pNewFakeBuff->bBlessType = Dbo_IsForBlessBuff( pSystemEffectTable->byActive_Effect_Type );

			if (aBuffParameter)
				memcpy(&pNewFakeBuff->aBuffParameter[i], &aBuffParameter[i], sizeof(sDBO_BUFF_PARAMETER));
		}	
	}
	else if(pNewFakeBuff->eType == DBO_OBJECT_SOURCE_ITEM)
	{
		pNewFakeBuff->pItemTbl = reinterpret_cast<sITEM_TBLDAT*>( API_GetTableContainer()->GetItemTable()->FindData(uiTableIndex) );
		if( !pNewFakeBuff->pItemTbl )
		{
			DBO_FAIL("Not exist item table of index : " << uiTableIndex);
			return;
		}		

		sUSE_ITEM_TBLDAT* pUseItemData = reinterpret_cast<sUSE_ITEM_TBLDAT*>( API_GetTableContainer()->GetUseItemTable()->FindData( pNewFakeBuff->pItemTbl->Use_Item_Tblidx ) );
		if( !pUseItemData )
		{
			DBO_FAIL("Not USE ITEM table of index : " << pNewFakeBuff->pItemTbl->Use_Item_Tblidx);
			return;
		}		

		pNewFakeBuff->fMaxTime = (RwReal)pUseItemData->dwKeep_Time;
		pNewFakeBuff->CreateImage( pNewFakeBuff->pItemTbl->szIcon_Name );

		for( RwInt32 i = 0 ; i < NTL_MAX_EFFECT_IN_ITEM ; ++i )
		{				
			sSYSTEM_EFFECT_TBLDAT* pSystemEffectTable = reinterpret_cast<sSYSTEM_EFFECT_TBLDAT*>(API_GetTableContainer()->GetSystemEffectTable()->FindData( pUseItemData->aSystem_Effect[i] ) );

			if( !pSystemEffectTable )
				continue;

			pNewFakeBuff->bBlessType = Dbo_IsForBlessBuff( pSystemEffectTable->byActive_Effect_Type );
			break;
		}	
	}

	pNewFakeBuff->fTime = pNewFakeBuff->fMaxTime - fRemainTime;

	m_listBuff.push_back(pNewFakeBuff);
}

void CNtlFakeBuffContainer::DropBuff(eDBO_OBJECT_SOURCE eType, BYTE byBuffIndex)
{
	sNtlFakeBuff* pFakeBuff;
	LIST_FAKE_BUFF_ITER it = m_listBuff.begin();
	for( ; it != m_listBuff.end() ; ++it )
	{
		pFakeBuff = *it;

		if( pFakeBuff->eType != eType )
			continue;

		if( pFakeBuff->byBuffIndex == byBuffIndex)
			break;
	}

	if( it != m_listBuff.end() )
	{
		sNtlFakeBuff* pNtlFakeBuff = *it;
		NTL_DELETE(pNtlFakeBuff);
		m_listBuff.erase(it);
	}
}

RwInt32 CNtlFakeBuffContainer::GetBuffCount(void)
{
	return m_listBuff.size();
}

LIST_FAKE_BUFF_ITER	CNtlFakeBuffContainer::GetBeginBuff(void)
{
	return m_listBuff.begin();
}

LIST_FAKE_BUFF_ITER	CNtlFakeBuffContainer::GetEndBuff(void)
{
	return m_listBuff.end();
}

void CNtlFakeBuffContainer::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlFakeBuffContainer::HandleEvents");

	if(pMsg.Id == g_EventSobFakeBuffAdd)
	{
		SNtlEventFakeBuffAdd *pBuffAdd = reinterpret_cast<SNtlEventFakeBuffAdd*>(pMsg.pData);

		if( pBuffAdd->hSerialId == m_hOwner )
		{
			AddBuff((eDBO_OBJECT_SOURCE)pBuffAdd->byBuffType, pBuffAdd->byBuffIndex, pBuffAdd->uiTblId, (RwReal)pBuffAdd->uiRemainTime / 1000.0f, pBuffAdd->bActive, pBuffAdd->aBuffParameter);
			CNtlSLEventGenerator::SobUpdate(m_hOwner, EVENT_AIUT_BUFF, 0);
		}		
	}
	else if(pMsg.Id == g_EventSobFakeBuffDrop)
	{
		SNtlEventFakeBuffDrop *pBuffDrop = reinterpret_cast<SNtlEventFakeBuffDrop*>(pMsg.pData);

		if( pBuffDrop->hSerialId == m_hOwner )
		{
			DropBuff((eDBO_OBJECT_SOURCE)pBuffDrop->byBuffType, pBuffDrop->byBuffIndex);
			CNtlSLEventGenerator::SobUpdate(m_hOwner, EVENT_AIUT_BUFF, 0);
		}		
	}
	else if(pMsg.Id == g_EventSobFakeBuffRefreshAll)
	{
		// 기존 버프 전부 삭제
		LIST_FAKE_BUFF_ITER it = m_listBuff.begin();
		for( ; it != m_listBuff.end() ; ++it )
		{
			sNtlFakeBuff* pNtlFakeBuff = *it;
			NTL_DELETE(pNtlFakeBuff);
		}

		m_listBuff.clear();

		// 버프 세로 설정
		SNtlEventSobBuffRefreshAll *pBuffRefreshAll = reinterpret_cast<SNtlEventSobBuffRefreshAll*>(pMsg.pData);

		if( pBuffRefreshAll->hSerialId == m_hOwner )
		{
			for( RwInt32 i = 0 ; i < pBuffRefreshAll->byCount ; ++i )
			{
				AddBuff((eDBO_OBJECT_SOURCE)pBuffRefreshAll->pBuffInfo[i].bySourceType, pBuffRefreshAll->pBuffInfo[i].buffIndex,
					    pBuffRefreshAll->pBuffInfo[i].sourceTblidx,
					    (RwReal)pBuffRefreshAll->pBuffInfo[i].dwTimeRemaining / 1000.0f, pBuffRefreshAll->pBuffInfo[i].bActive,
						pBuffRefreshAll->pBuffInfo[i].aBuffParameter);
			}

			CNtlSLEventGenerator::SobUpdate(m_hOwner, EVENT_AIUT_BUFF, 0);
		}		
	}
	else if (pMsg.Id == g_EventSobFakeBuffActivate)
	{
		SNtlEventSobBuffActivate* pBuff = reinterpret_cast<SNtlEventSobBuffActivate*>(pMsg.pData);

		if (pBuff->hSerialId == m_hOwner)
		{
			sNtlFakeBuff* pFakeBuff;
			LIST_FAKE_BUFF_ITER it = m_listBuff.begin();
			for (; it != m_listBuff.end(); ++it)
			{
				pFakeBuff = *it;

				if (pFakeBuff->eType != pBuff->pBuffInfo->bySourceType)
					continue;

				if (pFakeBuff->byBuffIndex == pBuff->pBuffInfo->buffIndex)
				{
					pFakeBuff->bActive = pBuff->pBuffInfo->bActive;

					CNtlSLEventGenerator::SobUpdate(m_hOwner, EVENT_AIUT_BUFF, 0);
					break;
				}
			}
		}
	}

	NTL_RETURNVOID();
}