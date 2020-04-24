#include "precomp_dboclient.h"
#include "SkillHTBRpResult.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"

// DBO 
#include "DboGlobal.h"
#include "DialogPriority.h"


CSkillHTBRpResult::CSkillHTBRpResult(const RwChar* pName)
{
}

CSkillHTBRpResult::~CSkillHTBRpResult(void)
{
}

RwBool CSkillHTBRpResult::Create(void)
{
	NTL_FUNCTION( "CSkillHTBRpResult::Create" );

	if( !CNtlPLGui::Create( "", "", "gui\\HTBRpResult.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = reinterpret_cast<gui::CDialog*>( GetComponent( "dlgMain" ) );
	m_pThis->SetPriority(dDIALOGPRIORITY_NOTIFY);

	m_pflaResult	= reinterpret_cast<gui::CFlash*>( GetComponent( "flaResult" ) ) ;
	m_pflaResult->SetFrameCallback(FALSE);
	m_slotMovieEnd	= m_pflaResult->SigMovieEnd().Connect( this, &CSkillHTBRpResult::OnMovieEnd );

	m_bAddUpdate = FALSE;

	Show( false );

	LinkMsg(g_EventHTBRPResult);

	NTL_RETURN( TRUE );
}

void CSkillHTBRpResult::Destroy(void)
{
	UnLinkMsg(g_EventHTBRPResult);

	if(m_bAddUpdate)
	{
		GetNtlGuiManager()->RemoveUpdateFunc( this );
		m_bAddUpdate = FALSE;
	}

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

void CSkillHTBRpResult::Update( RwReal fElapsed )
{
	if(m_pflaResult)
		m_pflaResult->Update( fElapsed );
}

void CSkillHTBRpResult::HandleEvents( RWS::CMsg& msg )
{
	if(msg.Id == g_EventHTBRPResult)
	{
		SNtlEventHTPRpResult *pResult = reinterpret_cast<SNtlEventHTPRpResult*>( msg.pData );

		RwBool bSuccess = FALSE;

		if( ( pResult->bAttacker && pResult->bWin ) ||
			( !pResult->bAttacker && !pResult->bWin ) )
			bSuccess = TRUE;

		LoadFlash("HTB_UI.swf");

		m_pflaResult->Invoke( "Show_HTB_UI", "%d,%d,%d,%d", 
							(RwInt32)pResult->iAttackPoint, 
							(RwInt32)pResult->iDefenderPoint,
							(RwInt32)pResult->bAttacker,
							(RwInt32)bSuccess);
	}
}

void CSkillHTBRpResult::LoadFlash(const RwChar *pFlashName)
{
	if(m_pflaResult)
	{
		m_pflaResult->Load( pFlashName, TRUE );
		CRectangle rt = m_pflaResult->GetFrameResolution();

		RwUInt32 uiWidth	= GetDboGlobal()->GetScreenWidth();
		RwUInt32 uiHeight	= GetDboGlobal()->GetScreenHeight();

		CRectangle rtRect;

		rtRect.SetRect( uiWidth/2 - rt.GetWidth()/2, uiHeight/2 - rt.GetHeight()/2,
						  uiWidth/2 + rt.GetWidth()/2, uiHeight/2 + rt.GetHeight()/2 );

		m_pflaResult->SetPosition( rtRect );
		m_pflaResult->Show( true );
		m_pflaResult->PlayMovie( true );

		Show(true);

		if(!m_bAddUpdate)
		{
			GetNtlGuiManager()->AddUpdateFunc( this );
			m_bAddUpdate = TRUE;
		}
	}
}

void CSkillHTBRpResult::DestroyFlash(void)
{
	if(m_pflaResult)
		m_pflaResult->Unload();

	GetNtlGuiManager()->RemoveUpdateFunc( this );
	m_bAddUpdate = FALSE;
}

void CSkillHTBRpResult::OnMovieEnd(gui::CComponent* pComponent)
{
	if(m_pflaResult)
		m_pflaResult->Unload();

	Show(false);

	GetNtlGuiManager()->AddUpdateFunc( this );
	m_bAddUpdate = FALSE;
}