#include "precomp_dboclient.h"
#include "BackboardGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"
#include "DialogManager.h"

namespace
{
	#define	dDEFAULT_ALPHA		150
	#define dDEFAULT_RED		0
	#define dDEFAULT_GREEN		0
	#define dDEFAULT_BLUE		0
};

CBackboardGui::CBackboardGui(const RwChar* pName)
:CNtlPLGui(pName)
{
	
}

CBackboardGui::~CBackboardGui()
{

}

RwBool CBackboardGui::Create()
{
	NTL_FUNCTION( "CBackboardGui::Create" );

	if(!CNtlPLGui::Create("", "", "gui\\Backboard.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_EFDIALOG_BACKGROUND);

	m_srfBack.SetRectWH(0, 0, GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());

	Show(false);

	// default
	SetColor(dDEFAULT_RED, dDEFAULT_GREEN, dDEFAULT_BLUE);
	SetAlpha(dDEFAULT_ALPHA);

	// sig
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CBackboardGui::OnPaint );

	LinkMsg(g_EventResize);
	LinkMsg(g_EventBackboard);

	NTL_RETURN(TRUE);
}

VOID CBackboardGui::Destroy()
{
	NTL_FUNCTION("CBackboardGui::Destroy");

	UnLinkMsg(g_EventResize);
	UnLinkMsg(g_EventBackboard);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CBackboardGui::SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	m_srfBack.SetColorOnly(byRed, byGreen, byBlue);
}

VOID CBackboardGui::SetAlpha(RwUInt8 byAlpha)
{
	m_srfBack.SetAlpha(byAlpha);
}

VOID CBackboardGui::OnPaint()
{
	m_srfBack.Render();
}

RwInt32 CBackboardGui::SwitchDialog(bool bOpen)
{
	if( bOpen )
	{
		Show(true);
	}
	else
	{
		// 다이얼로그가 닫힐 때 초기화
		Show(false);
		SetColor(dDEFAULT_RED, dDEFAULT_GREEN, dDEFAULT_BLUE);
		SetAlpha(dDEFAULT_ALPHA);
	}

	return 1;
}

VOID CBackboardGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CBackboardGui::HandleEvents");

	if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize* pPacket = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		m_pThis->SetPosition(CRectangle(0, 0, pPacket->iWidht, pPacket->iHeight));
		m_srfBack.SetRectWH(0, 0, pPacket->iWidht, pPacket->iHeight);
	}
	else if( msg.Id == g_EventBackboard )
	{
		SDboEventBackboard* pEvent = reinterpret_cast<SDboEventBackboard*>( msg.pData );

		switch( pEvent->uiMessage )
		{
		case BET_COLOR:
			{
				SetColor(pEvent->byValue, pEvent->byValue2, pEvent->byValue3);
				break;
			}
		case BET_ALPHA:
			{
				SetAlpha(pEvent->byValue);
				break;
			}
		}
	}

	NTL_RETURNVOID();
}