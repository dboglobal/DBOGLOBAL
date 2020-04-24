#include "precomp_dboclient.h"
#include "CommunityRaid.h"
/*
#include "ItemTable.h"
#include "NtlDebug.h"
#include "NtlPLGuiManager.h"
#include "InfoWndManager.h"
#include "IconMoveManager.h"

#include "DboGlobal.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"

CCommunityGui::CCommunityGui()
{
	Init();
}

CCommunityGui::CCommunityGui(const RwChar* pName)
:CNtlPLGui(pName)
{
	Init();
}

CCommunityGui::~CCommunityGui()
{
	Destroy();
}

VOID CCommunityGui::Init()
{
	m_iMouseDownSlot = -1;

	m_pSlotFocusEffect = NULL;
}

RwBool CCommunityGui::Create()
{
	NTL_FUNCTION( "CCommunityGui::Create" );
/*
	if(!CNtlPLGui::Create("gui\\ScouterSlot.rsr", "gui\\ScouterSlot.srf", "gui\\ScouterSlot.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	// ½½·Ô Æ÷Ä¿½º ÀÌÆåÆ®
	m_pSlotFocusEffect = (gui::CPanel*)GetComponent("SlotEffect");
	m_pSlotFocusEffect->Enable(false);

	// sig
	m_slotMouseDown		= m_pThis->SigMouseDown().Connect( this, &CCommunityGui::OnMouseDown );
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect( this, &CCommunityGui::OnMouseUp );
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CCommunityGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CCommunityGui::OnMouseLeave );
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CCommunityGui::OnPaint );
*//*
	NTL_RETURN(TRUE);
}

VOID CCommunityGui::Destroy()
{
	NTL_FUNCTION("CExpGui::Destroy");

	m_slot.Destroy();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CCommunityGui::OnMouseDown(const CKey& key)
{
}

VOID CCommunityGui::OnMouseUp(const CKey& key)
{	
}

VOID CCommunityGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
}

VOID CCommunityGui::OnMouseLeave(gui::CComponent* pComponent)
{
	m_slot.ClickEffect(false);
	GetInfoWndManager()->ShowInfoWindow( FALSE );
}

VOID CCommunityGui::OnPaint()
{
	m_slot.Paint();
}

RwInt32 CCommunityGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);
	return 1;
}

VOID CCommunityGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CCommunityParty::HandleEvents");
	NTL_RETURNVOID();
}*/