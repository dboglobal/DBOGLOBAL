#include "precomp_ntlsimulation.h"
#include "NtlSobNpcAttr.h"

// shared
#include "NPCTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLLogic.h"

DEFINITION_MEMORY_POOL(CNtlSobNpcAttr)


CNtlSobNpcAttr::CNtlSobNpcAttr()
{
	m_pNpcTbl = NULL;
}

void CNtlSobNpcAttr::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		SNtlEventSobNpcCreate *pSobNpcCreate = (SNtlEventSobNpcCreate*)pMsg.pData;

		CNPCTable *pNpcTbl		= API_GetTableContainer()->GetNpcTable();
		CTextTable *pTextTable	= API_GetTableContainer()->GetTextAllTable()->GetNPCTbl();
		m_pNpcTbl				= (sNPC_TBLDAT*)pNpcTbl->FindData(pSobNpcCreate->pNpcBrief->tblidx);
        if(!m_pNpcTbl)
        {
            char szBuf[64] = {0,};
            sprintf_s(szBuf, "NPC Table is Not Exist. ID - %d", pSobNpcCreate->pNpcBrief->tblidx);
            NTL_ASSERTFAIL(szBuf);

            return;
        }

		SetLevel(m_pNpcTbl->byLevel);
		SetLp((RwInt32)pSobNpcCreate->pNpcBrief->curLp);
		SetMaxLp((RwInt32)pSobNpcCreate->pNpcBrief->maxLp);
		SetRadius(m_pNpcTbl->fRadius);
		SetModelName(m_pNpcTbl->szModel);

		sTEXT_TBLDAT* pTextTblData;

		pTextTblData = reinterpret_cast<sTEXT_TBLDAT*>( pTextTable->FindData( m_pNpcTbl->Name ) );
		if ( pTextTblData )
		{
			SetName(pTextTblData->wstrText.c_str());
		}
		SetNameColor(NTL_NPC_NAME_COLOR);

		if( NPC_JOB_BUS == m_pNpcTbl->byJob )
		{
			sTEXT_TBLDAT* pNickTextTblData = reinterpret_cast<sTEXT_TBLDAT*>( pTextTable->FindData( pSobNpcCreate->pNpcBrief->nicknameTblidx ) );
			if ( pNickTextTblData )
			{
				SetNickName(pNickTextTblData->wstrText.c_str());
			}

			SetNickNameColor(NTL_BUS_NICK_NAME_COLOR);
		}
		else
		{
			const WCHAR* pwcNPCJobName = Logic_GetSLNPCJobName(m_pNpcTbl->byJob);

			if( pwcNPCJobName )
				SetNickName(pwcNPCJobName);

			SetNickNameColor(NTL_NPC_NICK_NAME_COLOR);
		}

		SetDefaultWalkSpeed(pSobNpcCreate->pNpcBrief->fLastWalkingSpeed);
		SetDefaultRunSpeed(pSobNpcCreate->pNpcBrief->fLastRunningSpeed);
		SetWalkSpeed(pSobNpcCreate->pNpcBrief->fLastWalkingSpeed);
		SetRunSpeed(pSobNpcCreate->pNpcBrief->fLastRunningSpeed);		        
        SetRunAnimSpeed(m_pNpcTbl->fRun_Speed / m_pNpcTbl->fRun_Speed_Origin);        
		if(pSobNpcCreate->pNpcBrief->wAttackSpeedRate > 0)
			SetAttackAnimSpeed((RwReal)1000/(RwReal)pSobNpcCreate->pNpcBrief->wAttackSpeedRate);
		SetFlyHeight(m_pNpcTbl->fFly_Height);
		SetScale(m_pNpcTbl->fScale);
		SetAttackRange(m_pNpcTbl->fAttack_Range);
		SetNameCreate(pSobNpcCreate->bCreateName);

		if(pTextTblData == NULL)
			SetNameCreate(FALSE);
	}
	else if(pMsg.Id == g_EventChangeNickName)
	{
		SNtlEventChangeNickName* pData = (SNtlEventChangeNickName*)pMsg.pData;

		CTextTable* pTextTable = API_GetTableContainer()->GetTextAllTable()->GetNPCTbl();
		sTEXT_TBLDAT* pTextTblData = reinterpret_cast<sTEXT_TBLDAT*>( pTextTable->FindData( pData->nicknameTblidx ) );
		if ( pTextTblData )
		{
			SetNickName(pTextTblData->wstrText.c_str());
		}

		if( NPC_JOB_BUS == m_pNpcTbl->byJob )
			SetNickNameColor(NTL_BUS_NICK_NAME_COLOR);
		else
			SetNickNameColor(NTL_NPC_NICK_NAME_COLOR);
	}
}

sNPC_TBLDAT* CNtlSobNpcAttr::GetNpcTbl(void) const
{
	return m_pNpcTbl;
}
