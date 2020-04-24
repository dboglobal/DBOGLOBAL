#include "precomp_ntlsimulation.h"
#include "NtlSobBuffIcon.h"

// core
#include "NtlDebug.h"

// shared
#include "NtlObject.h"
#include "SkillTable.h"
#include "ItemTable.h"
#include "UseItemTable.h"
#include "TableContainer.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSLEvent.h"
#include "NtlSobBuff.h"
#include "NtlSobBuffAttr.h"

DEFINITION_MEMORY_POOL(CNtlSobBuffIcon)

CNtlSobBuffIcon::CNtlSobBuffIcon()
{
}

CNtlSobBuffIcon::~CNtlSobBuffIcon()
{
}

void CNtlSobBuffIcon::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		CNtlEventSobBuffCreate *pSobItemCreate = reinterpret_cast<CNtlEventSobBuffCreate*>(pMsg.pData);
		{
			CNtlSobBuff *pSobBuff = reinterpret_cast<CNtlSobBuff*>(m_pSobObj);
			CNtlSobBuffAttr *pSobBuffAttr = reinterpret_cast<CNtlSobBuffAttr*>(pSobBuff->GetSobAttr());

			if (DBO_OBJECT_SOURCE_SKILL == pSobBuffAttr->GetBuffType())
			{
				sSKILL_TBLDAT* pSkillTbl = pSobBuffAttr->GetSkillTbl();

				CreateImage(pSkillTbl->szIcon_Name);

				m_fMaxCoolTime = pSobItemCreate->uiKeepTime / 1000.0f;				
			}
			else if (DBO_OBJECT_SOURCE_ITEM == pSobBuffAttr->GetBuffType())
			{
				sITEM_TBLDAT* pItemTbl = pSobBuffAttr->GetItemTbl();

				CreateImage(pItemTbl->szIcon_Name);	

				sUSE_ITEM_TBLDAT* pUseItemData = reinterpret_cast<sUSE_ITEM_TBLDAT*>( API_GetTableContainer()->GetUseItemTable()->FindData( pItemTbl->Use_Item_Tblidx ) );
				NTL_ASSERT( pUseItemData, "UseItemTbl Index" << pItemTbl->Use_Item_Tblidx << "is Invalid" );

				if (pSobItemCreate->aBuffParameter[0].byBuffParameterType == DBO_BUFF_PARAMETER_TYPE_ACTIVE_LP_EP_AUTO_RECOVER)
				{
					m_fMaxCoolTime = (float)pSobItemCreate->aBuffParameter[1].buffParameter.dwRemainTime / 1000.0f;

					if (pSobItemCreate->aBuffParameter[0].buffParameter.dwRemainTime > 0)
					{
						SetIconState(ICON_STATE_COOLING, m_fMaxCoolTime - ((float)pSobItemCreate->aBuffParameter[0].buffParameter.dwRemainTime / 1000.0f));
					}

					return;
				}

				m_fMaxCoolTime = (float)pUseItemData->dwKeep_Time;
			}

			if( pSobItemCreate->uiRemainTime > 0 )
			{
				SetIconState( ICON_STATE_COOLING, m_fMaxCoolTime - ( pSobItemCreate->uiRemainTime / 1000.0f ) );
			}
		}
	}
	else if (pMsg.Id == g_EventSobBuffAutoRecover)
	{
		SNtlEventSobBuffAutoRecover *pBuff = reinterpret_cast<SNtlEventSobBuffAutoRecover*>(pMsg.pData);

		m_fMaxCoolTime = (float)pBuff->dwRemainTime / 1000.0f;

		SetIconState(ICON_STATE_COOLING);
	}
	else if (pMsg.Id == g_EventSobBuffActivate)
	{
		SNtlEventSobBuffActivate *pBuff = reinterpret_cast<SNtlEventSobBuffActivate*>(pMsg.pData);

		if (pBuff->pBuffInfo->bActive)
		{
			if (pBuff->pBuffInfo->aBuffParameter[0].byBuffParameterType == DBO_BUFF_PARAMETER_TYPE_ACTIVE_LP_EP_AUTO_RECOVER)
			{
				m_fMaxCoolTime = (float)pBuff->pBuffInfo->aBuffParameter[1].buffParameter.dwRemainTime / 1000.0f;

				if (pBuff->pBuffInfo->aBuffParameter[0].buffParameter.dwRemainTime > 0)
				{
					SetIconState(ICON_STATE_COOLING, m_fMaxCoolTime - ((float)pBuff->pBuffInfo->aBuffParameter[0].buffParameter.dwRemainTime / 1000.0f));
				}

				return;
			}

			m_fMaxCoolTime = (float)pBuff->pBuffInfo->dwInitialDuration / 1000.0f;

			if (pBuff->pBuffInfo->dwTimeRemaining > 0)
			{
				SetIconState(ICON_STATE_COOLING, m_fMaxCoolTime - (pBuff->pBuffInfo->dwTimeRemaining / 1000.0f));
			}
			else
			{
				SetIconState(ICON_STATE_USEABLE);
			}
		}
		else
		{
			SetIconState(ICON_STATE_DISABLED);
		}
	}
}

RwBool CNtlSobBuffIcon::IsDragAndDropPossible(void)
{
	return FALSE;
}

RwBool CNtlSobBuffIcon::IsUsePossible(void)
{
	return FALSE;
}
