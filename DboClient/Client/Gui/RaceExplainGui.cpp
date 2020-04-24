#include "precomp_dboclient.h"
#include "RaceExplainGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLDef.h"
#include "NtlPLGuiManager.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DisplayStringManager.h"
#include "DboLogic.h"

CRaceExplainGui::CRaceExplainGui(const RwChar* pName)
:CNtlPLGui(pName)
{

}

CRaceExplainGui::~CRaceExplainGui()
{

}

RwBool CRaceExplainGui::Create()
{
	NTL_FUNCTION( "CRaceExplainGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\CharCreate_Explain.srf", "gui\\CharCreate_Race.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	// 배경
	m_Background.SetType(CWindowby3::WT_HORIZONTAL);
	m_Background.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate_Explain.srf", "srfBackgroundUp" ));
	m_Background.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate_Explain.srf", "srfBackgroundCenter" ));
	m_Background.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate_Explain.srf", "srfBackgroundBottom" ));	
	m_Background.SetSize(225, 298);

	// 종족 이름 배경
	m_srtRaceNameBack.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharCreate_Explain.srf", "srfNameBar" ));

	// 종족 이름
	rect.SetRect(52, 6, 172, 26);
	m_pTitleStatic = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT);
	m_pTitleStatic->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pTitleStatic->SetText(GetDisplayStringManager()->GetString("DST_RACE"));
	m_pTitleStatic->Enable(false);

	// 종족 이름
	rect.SetRect(28, 37, 197, 62);
	m_pRaceName = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_pRaceName->CreateFontStd( DEFAULT_FONT, 110, DEFAULT_FONT_ATTR);
	m_pRaceName->SetTextColor( RGB(255, 192, 0) );
	m_pRaceName->Enable(false);

	// 종족 설명
	m_pExplainHtml = (gui::CHtmlBox*)GetComponent("hbxConv");
	m_pExplainHtml->SetLineSpace(8);

	Show(false);

	// sig
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CRaceExplainGui::OnPaint );

	LinkMsg(g_EventCharMake);

	NTL_RETURN(TRUE);
}

VOID CRaceExplainGui::Destroy()
{
	NTL_FUNCTION("CRaceExplainGui::Destroy");

	UnLinkMsg(g_EventCharMake);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CRaceExplainGui::SetRace(RwUInt8 byRace)
{
}

VOID CRaceExplainGui::SetPosition(RwInt32 iXPos, RwInt32 iYPos)
{
	m_pThis->SetPosition(iXPos, iYPos);

	m_Background.SetPosition(iXPos, iYPos);
	m_srtRaceNameBack.SetPosition(iXPos + 11, iYPos + 60);
}

VOID CRaceExplainGui::OnPaint()
{
	m_Background.Render();
	m_srtRaceNameBack.Render();
}

VOID CRaceExplainGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CRaceExplainGui::HandleEvents");

	if( msg.Id == g_EventCharMake )
	{
		SNtlEventCharMake* pCharMake = reinterpret_cast<SNtlEventCharMake*>(msg.pData);

		const WCHAR* pwcText = Logic_GetPlayerRaceName(pCharMake->byRace);

		m_pRaceName->SetText(pwcText);

		switch (pCharMake->byRace)
		{
			case RACE_HUMAN: pwcText = GetDisplayStringManager()->GetString("DST_LOBBY_HUMAN_EXPLAIN"); break;
			case RACE_NAMEK: pwcText = GetDisplayStringManager()->GetString("DST_LOBBY_NAMEK_EXPLAIN"); break;
			case RACE_MAJIN: pwcText = GetDisplayStringManager()->GetString("DST_LOBBY_MAJIN_EXPLAIN"); break;

			default: DBO_ASSERT(FALSE, "CRaceExplainGui::HandleEvents Not defined: " << pCharMake->byRace);
		}

		m_pExplainHtml->Clear();
		m_pExplainHtml->SetHtmlFromMemory(pwcText, wcslen(pwcText));
	}

	NTL_RETURNVOID();
}