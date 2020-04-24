#include "precomp_dboclient.h"
#include "GMGuiGroup.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// cleint
#include "GMQuickSlotGui.h"
#include "QuickSlotGui.h"

#include "DialogManager.h"
#include "DboEventGenerator.h"

CGMGuiGroup::CGMGuiGroup()
:m_pGMQuickSlot(NULL)
{

}

CGMGuiGroup::~CGMGuiGroup()
{

}

RwBool CGMGuiGroup::Create()
{
	LinkMsg( g_EventObserverMenu );

	return TRUE;
}

VOID CGMGuiGroup::Destroy()
{
	if(m_pGMQuickSlot)
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui(m_pGMQuickSlot);
		GetDialogManager()->UnRegistDialog(DIALOG_GM_QUICKSLOT);
		m_pGMQuickSlot->Destroy();
		NTL_DELETE(m_pGMQuickSlot);
	}

	UnLinkMsg( g_EventObserverMenu );
}

VOID CGMGuiGroup::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventObserverMenu )
	{
		SDboEventObserverMenu* pEvent = reinterpret_cast<SDboEventObserverMenu*>( msg.pData );
		if( pEvent->byEventType == OBSERVER_START_OBSERVER )
		{
			CQuickSlotGui* pQuickSlot = reinterpret_cast<CQuickSlotGui*>( GetDialogManager()->GetDialog(DIALOG_QUICKSLOT) );
			if( pQuickSlot )
				pQuickSlot->UnLinkActionMap();

			// GM Quick slot
			m_pGMQuickSlot = NTL_NEW CGMQuickSlotGui("GMQuickSlot");
			if(!m_pGMQuickSlot->Create())
			{
				m_pGMQuickSlot->Destroy(); 
				NTL_DELETE(m_pGMQuickSlot);
				return;
			}
			CNtlPLGuiManager::GetInstance()->AddGui(m_pGMQuickSlot);
			GetDialogManager()->RegistDialog(DIALOG_GM_QUICKSLOT, m_pGMQuickSlot, &CGMQuickSlotGui::SwitchDialog);	
		}
		else if( pEvent->byEventType == OBSERVER_END_OBSERVER )
		{
			if(m_pGMQuickSlot)
			{
				CNtlPLGuiManager::GetInstance()->RemoveGui(m_pGMQuickSlot);
				GetDialogManager()->UnRegistDialog(DIALOG_GM_QUICKSLOT);
				m_pGMQuickSlot->Destroy();
				NTL_DELETE(m_pGMQuickSlot);
			}

			CQuickSlotGui* pQuickSlot = reinterpret_cast<CQuickSlotGui*>( GetDialogManager()->GetDialog(DIALOG_QUICKSLOT) );
			if( pQuickSlot )
				pQuickSlot->LinkActionMap();
		}
	}
}