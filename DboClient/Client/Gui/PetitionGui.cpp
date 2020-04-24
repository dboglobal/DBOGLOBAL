#include "precomp_dboclient.h"
#include "PetitionGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLEvent.h"
#include "NtlPLGuiManager.h"

// cleint
#include "DialogManager.h"
#include "DboGlobal.h"
#include "DisplayStringManager.h"
#include "PetitionManager.h"

CPetitionGui::CPetitionGui(const RwChar* pName)
:CNtlPLGui( pName )
{

}

CPetitionGui::~CPetitionGui()
{

}

RwBool CPetitionGui::Create()
{
	NTL_FUNCTION( "CPetitionGui::Create" );

	if(!CNtlPLGui::Create("gui\\Petition.rsr", "gui\\Petition.srf", "gui\\Petition.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis			= (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_QUICKSLOT);

	// 닫기 버튼
	m_pCloseButton	= (gui::CButton*)GetComponent("ExitButton");
	m_slotCloseButton = m_pCloseButton->SigClicked().Connect(this, &CPetitionGui::OnClicked_CloseButton);

	// 다이얼로그 제목
	m_pDialogName	= (gui::CStaticBox*)GetComponent( "stbDialogName" );
	m_pDialogName->SetPosition(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y);
	m_pDialogName->SetText( GetDisplayStringManager()->GetString( "DST_PETITION_TITLE" ) );

	// 대분류 카테고리 스테틱
	m_pCategoryStatic = (gui::CStaticBox*)GetComponent( "stbCategory" );
	m_pCategoryStatic->SetText( GetDisplayStringManager()->GetString( "DST_PETITION_GREAT_CATEGORY" ) );

	// 대분류 카테고리
	m_pCategory		= (gui::CComboBox*)GetComponent( "cbbCategory" );
	m_slotItemSelect = m_pCategory->SigSelected().Connect( this, &CPetitionGui::OnItemSelect );

	m_pCategory->ClearAllItems();

	m_pCategory->AddItem( GetDisplayStringManager()->GetString("DST_PETITION_CATEGORY1_ACCUSE") );
	m_pCategory->AddItem(GetDisplayStringManager()->GetString("DST_PETITION_CATEGORY1_BUG"));
	m_pCategory->AddItem(GetDisplayStringManager()->GetString("DST_PETITION_CATEGORY1_GAMEINFO"));
	m_pCategory->AddItem(GetDisplayStringManager()->GetString("DST_PETITION_CATEGORY1_STUCK"));

	m_pCategory->SelectItem( 0 );	

	// 소분류 카테고리 스테틱
	m_pCategoryStatic2 = (gui::CStaticBox*)GetComponent( "stbCategory2" );
	m_pCategoryStatic2->SetText( GetDisplayStringManager()->GetString( "DST_PETITION_SMALL_CATEGORY" ) );

	// 소분류 카테고리
	m_pCategory2	= (gui::CComboBox*)GetComponent( "cbbCategory2" );
	m_slotItemSelect2 = m_pCategory2->SigSelected().Connect( this, &CPetitionGui::OnItemSelect2 );

	// 전송버튼
	m_pSendButton	= (gui::CButton*)GetComponent("btnSend");
	m_pSendButton->SetText(GetDisplayStringManager()->GetString( "DST_PETITION_SEND" ));
	m_slotSendButton = m_pSendButton->SigClicked().Connect(this, &CPetitionGui::OnClicked_SendButton);

	// 진정내용
	m_pInput	= (gui::CInputBox*)GetComponent("Input");	
	m_pInput->SetMultilineMode(TRUE);
	m_pInput->SetMaxLength(NTL_MAX_SIZE_QUESTION_CONTENT);
	m_pInput->SetCaretSize(dINPUTBOX_CARET_WIDTH, dINPUTBOX_CARET_HEIGHT);

	// 배경
	m_BackPanel.SetType(CWindowby3::WT_VERTICAL);
	m_BackPanel.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Petition.srf", "srfPanelLeft" ) );
	m_BackPanel.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Petition.srf", "srfPanelCenter" ) );
	m_BackPanel.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Petition.srf", "srfPanelRight" ) );
	m_BackPanel.SetSize(269, 286);
	m_BackPanel.SetPositionfromParent(12, 71);	

	// 카테고리 1 배경
	m_srfCaregoryBack.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Petition.srf", "srfCategoryBack" ) );
	m_srfCaregoryBack.SetPositionfromParent(12, 47);

	// 카테고리 2 배경
	m_srfCaregoryBack2.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "Petition.srf", "srfCategoryBack" ) );
	m_srfCaregoryBack2.SetPositionfromParent(150, 47);

	// 대분류 카테고리 선택 메세지
	m_pCategoryMessage = (gui::CStaticBox*)GetComponent( "stbBigClassfication" );

	//소분류 카테고리 선택 메세지
	m_pCategoryMessage2 = (gui::CStaticBox*)GetComponent( "stbSmallClassfication" );

	if( GetPetitionManager()->GetCategory2() != PETITION_CATEGORY_2_INVALID )
	{
		m_pCategory->SelectItem( GetPetitionManager()->GetCategory() );
		OnItemSelect( GetPetitionManager()->GetCategory() );
		OnItemSelect2( GetPetitionManager()->GetCategory2() );
		m_pInput->SetText( GetPetitionManager()->GetPetitionContent() );
	}
	else
	{
		m_pCategory->SelectItem(0);
		SetTextCategory();
	}

	// sig
	m_slotGotFocus			= m_pInput->SigGotFocus().Connect( this, &CPetitionGui::OnGotFocus );
	m_slotLostFocus			= m_pInput->SigLostFocus().Connect( this, &CPetitionGui::OnLostFocus );
	m_slotMove				= m_pThis->SigMove().Connect( this, &CPetitionGui::OnMove );
	m_slotPaint				= m_pThis->SigPaint().Connect( this, &CPetitionGui::OnPaint );

	// Surface들을 위치시키기 위해
	CRectangle rtScreen = m_pThis->GetScreenRect();
	OnMove(rtScreen.left, rtScreen.top);

	Show(false);

	LinkMsg(g_EventResize);
	LinkMsg(g_EventPetition);

	NTL_RETURN(TRUE);
}

VOID CPetitionGui::Destroy()
{
	NTL_FUNCTION("CPetitionGui::Destroy");

	if( GetNtlGuiManager()->GetGuiManager()->GetFocus() == m_pInput )
		GetNtlGuiManager()->GetGuiManager()->SetFocus( GetNtlGuiManager()->GetGuiManager() );

	UnLinkMsg(g_EventResize);
	UnLinkMsg(g_EventPetition);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CPetitionGui::EnableControll(bool bEnable)
{
	m_pCloseButton	->ClickEnable(bEnable);
	m_pCategory		->Enable(bEnable);
	m_pCategory2	->Enable(bEnable);
	m_pInput		->Enable(bEnable);
	m_pSendButton	->ClickEnable(bEnable);
}

VOID CPetitionGui::SetTextCategory(RwInt32 iSelect /* = INVALID_INDEX */)
{
	RwInt32 iSelectedIndex = iSelect;
		
	if( iSelect == INVALID_INDEX )
		iSelectedIndex = m_pCategory->GetSelectedItemIndex();

	std::vector<RwInt32> vecTemp = PetitionCategory1To2( iSelectedIndex );

	m_pCategory2->ClearAllItems();

	for( RwInt32 i = 0 ; i < (RwInt32)vecTemp.size() ; ++i )
	{
		m_pCategory2->AddItem( GetCategory2Text(vecTemp[i]) );
	}


	const WCHAR* pwcText = GetDisplayStringManager()->GetString("DST_PETITION_GREAT_CATEGORY");
	const WCHAR* pwcText2 = GetCategory1Text(iSelectedIndex);

	m_pCategoryMessage->Format(L"%s : %s", pwcText, pwcText2);
	m_pCategoryMessage2->Clear();
}

VOID CPetitionGui::SetTextCategory2(RwInt32 iSelect /* = INVALID_INDEX */)
{
	if( iSelect != INVALID_INDEX )
		m_pCategory2->SelectItem(iSelect);

	const WCHAR* pwcText = GetDisplayStringManager()->GetString("DST_PETITION_SMALL_CATEGORY");
	std::wstring wstrText2 = m_pCategory2->GetSelectedItemText();

	m_pCategoryMessage2->Format(L"%s : %s", pwcText, wstrText2.c_str());
}

const WCHAR * CPetitionGui::GetCategory1Text(RwInt32 iSelect)
{
	switch (iSelect)
	{
		case PETITION_CATEGORY_1_ACCUSE: GetDisplayStringManager()->GetString("DST_PETITION_CATEGORY1_ACCUSE"); break;
		case PETITION_CATEGORY_1_BUG: GetDisplayStringManager()->GetString("DST_PETITION_CATEGORY1_BUG"); break;
		case PETITION_CATEGORY_1_GAMEINFO: GetDisplayStringManager()->GetString("DST_PETITION_CATEGORY1_GAMEINFO"); break;
		case PETITION_CATEGORY_1_STUCK: GetDisplayStringManager()->GetString("DST_PETITION_CATEGORY1_STUCK"); break;
	}

	DBO_ASSERT(true, "Not defined petition categoty1: " << iSelect);

	return L"";
}

const WCHAR* CPetitionGui::GetCategory2Text(RwInt32 iSelect)
{
	switch (iSelect)
	{
		case PETITION_CATEGORY_2_ACCUSE_AUTOPROGRAM: return GetDisplayStringManager()->GetString("DST_PETITION_CATEGORY2_ACCUSE_AUTOPROGRAM"); break;
		case PETITION_CATEGORY_2_ACCUSE_FRAUD: return GetDisplayStringManager()->GetString("DST_PETITION_CATEGORY2_ACCUSE_FRAUD"); break;
		case PETITION_CATEGORY_2_ACCUSE_PAPERING: return GetDisplayStringManager()->GetString("DST_PETITION_CATEGORY2_ACCUSE_PAPERING"); break;
		case PETITION_CATEGORY_2_BUG_SYSTEM: return GetDisplayStringManager()->GetString("DST_PETITION_CATEGORY2_BUG_SYSTEM"); break;
		case PETITION_CATEGORY_2_BUG_GRAPHIC: return GetDisplayStringManager()->GetString("DST_PETITION_CATEGORY2_BUG_GRAPHIC"); break;
		case PETITION_CATEGORY_2_BUG_ETC: return GetDisplayStringManager()->GetString("DST_PETITION_CATEGORY2_BUG_ETC"); break;
		case PETITION_CATEGORY_2_GAMEINFO: return GetDisplayStringManager()->GetString("DST_PETITION_CATEGORY2_GAME_INFO"); break;
		case PETITION_CATEGORY_2_STUCK: return GetDisplayStringManager()->GetString("DST_PETITION_CATEGORY2_STUCK"); break;
		case PETITION_CATEGORY_2_CANCEL: return GetDisplayStringManager()->GetString("DST_PETITION_CANCEL_PETITION"); break;
	}

	DBO_ASSERT(true, "Not defined petition categoty2: " << iSelect);

	return L"";
}

VOID CPetitionGui::OnClicked_CloseButton(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_PETITION);
	CDboEventGenerator::PetitionPostEvent(PETITON_CLOSE_DILAOG);
}

VOID CPetitionGui::OnClicked_SendButton(gui::CComponent* pComponent)
{
	const WCHAR* pwcCharName = Logic_GetAvatarName();
	const WCHAR* pwcContent = m_pInput->GetText();
	RwInt32 iSelectedCategory = m_pCategory->GetSelectedItemIndex();
	RwInt32 iSelectedCategory2 = m_pCategory2->GetSelectedItemIndex();

	if( iSelectedCategory < 0 )
	{
		// 대분류를 선택하십시요
		GetAlarmManager()->AlarmMessage("DST_PETITION_CHOICE_GREATE_CATEGORY");
		return;
	}

	if( iSelectedCategory2 < 0 )
	{
		// 소분류를 선택하십시요
		GetAlarmManager()->AlarmMessage("DST_PETITION_CHOICE_SMALL_CATEGORY");
		return;
	}

	if(!pwcContent)
		return;

	size_t contentSize = wcslen(pwcContent);

	if( contentSize == 0 )
	{
		// 내용을 입력하세요
		GetAlarmManager()->AlarmMessage("DST_PETITION_INPUT_CONTENT");
		return;
	}

	if( contentSize > NTL_MAX_SIZE_QUESTION_CONTENT )
	{
		// 내용이 너무 깁니다
		GetAlarmManager()->AlarmMessage("DST_PETITION_TOO_LONG_CONTENT");
		return;
	}

	if( GetPetitionManager()->GetPetitionID() == INVALID_PETITIONID )
	{
		if( GetDboGlobal()->GetChatPacketGenerator()->SendPetitionUserInsertReq(pwcCharName, pwcContent, iSelectedCategory, iSelectedCategory2) )
			EnableControll(false);	
	}
	else
	{
		if( GetDboGlobal()->GetChatPacketGenerator()->SendPetitionModifyReq(GetPetitionManager()->GetPetitionID(), pwcContent, iSelectedCategory, iSelectedCategory2) )
			EnableControll(false);
	}	
}

VOID CPetitionGui::OnItemSelect( INT nIndex )
{
	SetTextCategory(nIndex);
}

VOID CPetitionGui::OnItemSelect2( INT nIndex )
{
	SetTextCategory2(nIndex);
}

VOID CPetitionGui::OnGotFocus()
{
	GetDboGlobal()->GetGameData()->bChatMode = TRUE;
}

VOID CPetitionGui::OnLostFocus()
{
	GetDboGlobal()->GetGameData()->bChatMode = FALSE;
}

VOID CPetitionGui::OnMove( RwInt32 iOldX, RwInt32 iOldY )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_BackPanel.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfCaregoryBack.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfCaregoryBack2.SetPositionbyParent(rtScreen.left, rtScreen.top);
}

VOID CPetitionGui::OnPaint()
{
	m_BackPanel.Render();
	m_srfCaregoryBack.Render();
	m_srfCaregoryBack2.Render();
}

RwInt32 CPetitionGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);
	return 1;
}

VOID CPetitionGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CPetitionGui::HandleEvents");

	if( msg.Id == g_EventResize )
	{
		Logic_LocateDialog_in_CleintRect(this, TRUE);
	}
	else if( msg.Id == g_EventPetition )
	{
		SDboEventPetition* pEvent = reinterpret_cast<SDboEventPetition*>( msg.pData );

		switch( pEvent->uiEventType )
		{
		case PETITON_ENABLE_PETITON_GUI:
			{
				EnableControll(true);
				break;
			}
		case PETITON_REQUEST_OK:
		case PETITON_MODIFY_OK:
			{
				const WCHAR* pwcContent = m_pInput->GetText();
				RwInt32 iSelectedCategory = m_pCategory->GetSelectedItemIndex();
				RwInt32 iSelectedCategory2 = m_pCategory2->GetSelectedItemIndex();

				GetPetitionManager()->SetPetitionContent(iSelectedCategory, iSelectedCategory2, pwcContent);
				break;
			}
		}
	}

	NTL_RETURNVOID();
}