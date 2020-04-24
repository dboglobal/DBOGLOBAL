#include "precomp_dboclient.h"
#include "PetitionSatisfactionGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// cleint
#include "DialogManager.h"
#include "DboGlobal.h"
#include "PetitionManager.h"

CPetitionSatisfaction::CPetitionSatisfaction(const RwChar* pName)
:CNtlPLGui( pName )
,m_iSelectedIndex(CUSTOMERSATISFACTION_INVALID)
{

}

CPetitionSatisfaction::~CPetitionSatisfaction()
{

}

RwBool CPetitionSatisfaction::Create()
{
	NTL_FUNCTION( "CPetitionSatisfaction::Create" );

	if(!CNtlPLGui::Create("gui\\PetitionSatisfaction.rsr", "gui\\PetitionSatisfaction.srf", "gui\\PetitionSatisfaction.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis			= (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_QUICKSLOT);

	// 다이얼로그 제목
	m_pDialogName	= (gui::CStaticBox*)GetComponent( "stbDialogName" );
	m_pDialogName->SetPosition(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y);
	m_pDialogName->SetText( GetDisplayStringManager()->GetString("DST_PETITION_SATISFACTION_TITLE") );

	// 안내 메세지
	m_pMessage		= (gui::CStaticBox*)GetComponent( "stbComment" );
	m_pMessage->SetText( GetDisplayStringManager()->GetString("DST_PETITION_SATISFACTION_COMMENT") );

	// 만족도 텍스트
	m_pToggleText[0] = (gui::CStaticBox*)GetComponent( "stbGreat" );
	m_pToggleText[0]->SetText( GetDisplayStringManager()->GetString("DST_PETITION_SATISFACTION_GREAT") );

	m_pToggleText[1] = (gui::CStaticBox*)GetComponent( "stbGood" );
	m_pToggleText[1]->SetText( GetDisplayStringManager()->GetString("DST_PETITION_SATISFACTION_GOOD") );

	m_pToggleText[2] = (gui::CStaticBox*)GetComponent( "stbNotBad" );
	m_pToggleText[2]->SetText( GetDisplayStringManager()->GetString("DST_PETITION_SATISFACTION_NOT_BAD") );

	m_pToggleText[3] = (gui::CStaticBox*)GetComponent( "stbBad" );
	m_pToggleText[3]->SetText( GetDisplayStringManager()->GetString("DST_PETITION_SATISFACTION_BAD") );

	m_pToggleText[4] = (gui::CStaticBox*)GetComponent( "stbWorst" );
	m_pToggleText[4]->SetText( GetDisplayStringManager()->GetString("DST_PETITION_SATISFACTION_WORST") );

	// 닫기 버튼
	m_pCloseButton	= (gui::CButton*)GetComponent( "ExitButton" );
	m_slotCloseButton = m_pCloseButton->SigClicked().Connect(this, &CPetitionSatisfaction::OnClicked_CloseButton);

	// 만족도 선택 라디오 버튼
	m_pRadioButton[0]	= (gui::CButton*)GetComponent( "btnGreat" );
	m_pRadioButton[0]->SetToggleMode(true);
	m_slotRadioButton[0] = m_pRadioButton[0]->SigToggled().Connect(this, &CPetitionSatisfaction::OnClicked_RadioToggleButton);

	m_pRadioButton[1]	= (gui::CButton*)GetComponent( "btnGood" );
	m_pRadioButton[1]->SetToggleMode(true);
	m_slotRadioButton[1] = m_pRadioButton[1]->SigToggled().Connect(this, &CPetitionSatisfaction::OnClicked_RadioToggleButton);

	m_pRadioButton[2]	= (gui::CButton*)GetComponent( "btnNotBad" );
	m_pRadioButton[2]->SetToggleMode(true);
	m_slotRadioButton[2] = m_pRadioButton[2]->SigToggled().Connect(this, &CPetitionSatisfaction::OnClicked_RadioToggleButton);

	m_pRadioButton[3]	= (gui::CButton*)GetComponent( "btnBad" );
	m_pRadioButton[3]->SetToggleMode(true);
	m_slotRadioButton[3] = m_pRadioButton[3]->SigToggled().Connect(this, &CPetitionSatisfaction::OnClicked_RadioToggleButton);

	m_pRadioButton[4]	= (gui::CButton*)GetComponent( "btnWorst" );
	m_pRadioButton[4]->SetToggleMode(true);
	m_slotRadioButton[4] = m_pRadioButton[4]->SigToggled().Connect(this, &CPetitionSatisfaction::OnClicked_RadioToggleButton);

	// 만족도 보내기 버튼
	m_pSendButton	= (gui::CButton*)GetComponent( "btnSend" );
	m_pSendButton->SetText( GetDisplayStringManager()->GetString("DST_PETITION_SEND") );
	m_slotSendButton = m_pSendButton->SigClicked().Connect(this, &CPetitionSatisfaction::OnClicked_SendButton);

	m_petitionID	= GetPetitionManager()->GetPetitionID();
	m_GMID			= GetPetitionManager()->GetChattingGMID();

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CPetitionSatisfaction::Destroy()
{
	NTL_FUNCTION("CPetitionSatisfaction::Destroy");

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CPetitionSatisfaction::OnClicked_CloseButton(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_PETITION_SATISFACTION);
}

VOID CPetitionSatisfaction::OnClicked_RadioToggleButton(gui::CComponent* pComponent, bool bToggled)
{
	for( RwUInt8 i = 0 ; i < dTEMP_GG ; ++i )
	{
		if( m_pRadioButton[i] == pComponent )
		{
			m_pRadioButton[i]->SetDown(true);
			m_iSelectedIndex = i;
		}
		else
		{
			m_pRadioButton[i]->SetDown(false);
		}
	}
}

VOID CPetitionSatisfaction::OnClicked_SendButton(gui::CComponent* pComponent)
{
	if( m_iSelectedIndex == CUSTOMERSATISFACTION_INVALID )
		return;	

	GetDboGlobal()->GetChatPacketGenerator()->SendPetitionSatisfaction(m_petitionID, (RwUInt8)m_iSelectedIndex);
	GetDialogManager()->CloseDialog(DIALOG_PETITION_SATISFACTION);
}

RwInt32 CPetitionSatisfaction::SwitchDialog(bool bOpen)
{
	Show(bOpen);

	if( !bOpen )
	{
		CDboEventGenerator::PetitionPostEvent(PETITON_FINISH_RESEARCH_SATISFACTION);
	}

	return 1;
}