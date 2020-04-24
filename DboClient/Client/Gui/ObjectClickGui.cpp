#include "precomp_dboclient.h"
#include "ObjectClickGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLPlayerName.h"
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSobProxy.h"
#include "NtlSobCharProxy.h"
#include "NtlSobAvatar.h"
#include "NtlSobManager.h"
#include "NtlSLGlobal.h"
#include "NtlSobCharProxyDecoration.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "GuiFunctor.h"
#include "DboGlobal.h"


CObjectClickGui::CObjectClickGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_fLastTime(0.f)
,m_fRemainTime(0.f)
,m_bVisible(FALSE)
,m_bActive(FALSE)
{
}

CObjectClickGui::~CObjectClickGui()
{

}

RwBool CObjectClickGui::Create()
{
	NTL_FUNCTION( "CObjectClickGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\ObjectClick.srf", "gui\\ObjectClick.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_TimeEffect.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ObjectClick.srf", "srfBackCircle" ) );

	// 바늘 그림
	m_srfNeedle.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("ObjectClick.srf", "srfNeedle"));

	// 반사광
	m_srfReflectedLight.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("ObjectClick.srf", "srfReflectedLight"));

	// sig	
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CObjectClickGui::OnPaint );

	Show(false);

	LinkMsg(g_EventOperateObjectTimeBar);

	NTL_RETURN(TRUE);
}

VOID CObjectClickGui::Destroy()
{
	NTL_FUNCTION("CObjectClickGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFunc( this );
	GetSoundManager()->Stop(m_hSound);

	UnLinkMsg(g_EventOperateObjectTimeBar);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CObjectClickGui::Update(RwReal fElapsed)
{
	if( IsShow() == FALSE )
		return;

	if( !m_bActive )
		return;

	m_fLastTime -= fElapsed;

	if( m_fLastTime < 0.f )
		m_fLastTime = 0.f;

	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	CNtlSobCharProxy* pAvatarProxy = reinterpret_cast<CNtlSobCharProxy*>(pAvatar->GetSobProxy());
	CNtlSobCharDecorationProxy* pCharDecorationProxy = pAvatarProxy->GetDecorationProxy();
	CNtlPLPlayerName* pPLPlayerName = pCharDecorationProxy->GetPLPlayerName();
	RwV3d v3Pos			= pPLPlayerName->GetPosition();
	RwV3d v3GuiPos		= pAvatar->GetPosition();
	CPos Pos2d;
	
	v3GuiPos.y = v3Pos.y;
	
	m_TimeEffect.Update(m_fRemainTime - m_fLastTime);
	m_srfNeedle.SetZRot(360 * m_fLastTime / m_fRemainTime);
	
	if( GetGuiFuntor()->Calc3DPosTo2D(v3GuiPos, 0.f, NULL, &Pos2d) )
	{
		RwInt32 iXPos = Pos2d.x - m_TimeEffect.GetWidth()/2;
		RwInt32 iYPos = Pos2d.y - m_TimeEffect.GetHeight();

		m_pThis->SetPosition(iXPos, iYPos);
		CRectangle rtScreen = m_pThis->GetScreenRect();
		
		m_TimeEffect.SetPosition(rtScreen.left + 9, rtScreen.top + 18);
		m_srfNeedle.SetCenterPosition(iXPos + 27, iYPos + 36);
		m_srfReflectedLight.SetPosition(rtScreen.left, rtScreen.top);

		m_bVisible = TRUE;
	}
	else
		m_bVisible = FALSE;
}

VOID CObjectClickGui::OnPaint()
{
	if( m_bVisible == FALSE )
		return;

	m_TimeEffect.Render();
	m_srfNeedle.Render();
	m_srfReflectedLight.Render();
}

RwInt32 CObjectClickGui::SwitchDialog(bool bOpen)
{	
	if(bOpen)
	{
		Show(true);

		sNtlSoundPlayParameta tSoundParam;
		tSoundParam.iChannelGroup	= CHANNEL_GROUP_UI_SOUND;
		tSoundParam.pcFileName		= GSD_GENERAL_CLOCK_SECOND;
		tSoundParam.bLoop			= TRUE;

		GetSoundManager()->Play(&tSoundParam);

		m_hSound = tSoundParam.hHandle;
	}
	else
	{
		Show(false);
		GetSoundManager()->Stop(m_hSound);
	}
	
	return 1;
}

VOID CObjectClickGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CObjectClickGui::HandleEvents");
	
	if( msg.Id == g_EventOperateObjectTimeBar )
	{
		SNtlEventOperateObjectTimeBar* pEvent = reinterpret_cast<SNtlEventOperateObjectTimeBar*>(msg.pData);

		if( pEvent->bShow )
		{
			// 밀리세컨드 단위가 들어온다
			m_fLastTime		= (RwReal)pEvent->uiPlayTime/1000.f;
			m_fRemainTime	= (RwReal)pEvent->uiPlayTime/1000.f;

			m_TimeEffect.StartProc(m_fRemainTime, TRUE, TRUE);
			m_TimeEffect.Update(0.0f);

			m_bActive = TRUE;

			GetDialogManager()->OpenDialog(DIALOG_OBJECT_CLICK);
			GetNtlGuiManager()->RemoveUpdateFunc( this );
			GetNtlGuiManager()->AddUpdateFunc( this );
		}
		else
		{
			m_bActive = FALSE;

			GetDialogManager()->CloseDialog(DIALOG_OBJECT_CLICK);
			GetNtlGuiManager()->RemoveUpdateFunc( this );
		}
	}

	NTL_RETURNVOID();
}