#include "precomp_dboclient.h"

// core
#include "NtlDebug.h"
#include "CEventHandler.h"

// presentation
#include "NtlPLDef.h"
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"

// framework
#include "NtlApplication.h"

// simulation
#include "InputActionMap.h" 
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLLogic.h"
#include "NtlSobAvatar.h"
#include "NtlSobManager.h"
#include "NtlSLEventFunc.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"

// table
#include "ItemTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// dbo
#include "ChatGui.h"
#include "DboGlobal.h"
#include "DboDef.h"
#include "DboEvent.h"
#include "DboLogic.h"
#include "DboEventGenerator.h"
#include "DboPacketGenerator.h"
#include "InfoWndManager.h"
#include "DisplayStringManager.h"
#include "DialogManager.h"
#include "IconMoveManager.h"
#include "MsgBoxManager.h"
#include "AlarmManager.h"
#include "InfoWndManager.h"
#include "MailSystemGui.h"

// etc

/**
* \brief eTYPE에 맞춰서 가변인자를 매개변수로 받아서 채팅창에 출력한다.
* 가변인자를 받아서 vswprintf_s() 함수를 사용하여 문자열을 만든 후 그 문자열을
* 채팅창에 출력하는 Event를 보낸다.
* \param eType	eMAILSYSTEM_MSG_SAY_TYPE
* \param ...	가변인자
*/
void CMailSystemMsg::MsgSay(eMAILSYSTEM_MSG_SAY_TYPE eType, ...)
{
	WCHAR awcMessageBuffer[BUFFER_SAY_SIZE];
	memset((char*)awcMessageBuffer, 0, sizeof(WCHAR) * (BUFFER_SAY_SIZE));

	va_list argList;
	va_start(argList, eType);
	switch (eType)
	{
	case MAIL_MSG_SAY_SEND_SUCCEED:
		vswprintf_s(awcMessageBuffer, BUFFER_SAY_SIZE, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MSG_SEND_SUCCEED"), argList);
		break;
	case MAIL_MSG_SAY_ATTACH_ZENNY:
		vswprintf_s(awcMessageBuffer, BUFFER_SAY_SIZE, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MSG_ATTACH_ZENNY"), argList);
		break;
	case MAIL_MSG_SAY_MAIL_NOT_LOCK:
		vswprintf_s(awcMessageBuffer, BUFFER_SAY_SIZE, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MSG_MAIL_NOT_LOCK"), argList);
		break;
	case MAIL_MSG_SAY_RECEIVE_ITEM:
		vswprintf_s(awcMessageBuffer, BUFFER_SAY_SIZE, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MSG_RECEIVE_ITEM"), argList);
		break;
	case MAIL_MSG_SAY_RECEIVE_ZENNY:
		vswprintf_s(awcMessageBuffer, BUFFER_SAY_SIZE, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MSG_RECEIVE_ZENNY"), argList);
		break;
	case MAIL_MSG_SAY_RECEIVE_ZENNY_REQ:
		vswprintf_s(awcMessageBuffer, BUFFER_SAY_SIZE, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MSG_RECEIVE_ZENNY_REQ"), argList);
		break;
	case MAIL_MSG_SAY_RECEIVE_ACCEPT_MAIL:
		vswprintf_s(awcMessageBuffer, BUFFER_SAY_SIZE, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MSG_RECEIVE_ACCEPT_MAIL"), argList);
		break;
	case MAIL_MSG_SAY_ITEM_NOT_ATTACH:
		vswprintf_s(awcMessageBuffer, BUFFER_SAY_SIZE, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MSG_ATTACH_ITEM_NOT"), argList);
		break;
	}
	va_end(argList);		
	CDboEventGenerator::SayMessage(CHAT_TYPE_SYSTEM, L"", (RwUInt16)wcslen(awcMessageBuffer), awcMessageBuffer);
}

/**
* \breif TYPE과 가변인자를 받아서 타입에 알맞은 MessageBox를 띄운다.
* \param eType	eMAILSYSTEM_MSG_BOX_TYPE
* \param ...	가변 인자
*/
void CMailSystemMsg::MsgBox(eMAILSYSTEM_MSG_BOX_TYPE eType, ...)
{
	va_list argList;
	va_start(argList, eType);
	switch (eType)
	{
	case MAIL_MSG_BOX_PLEASE_TARGETNAME:
		//eMsgBoxWork	= MBW_NONE;
		//eMsgBoxTypeFlag	= MBTF_OK;
		//bPopup		= FALSE;
		//vswprintf_s(awcMessageBuffer, BUFFER_BOX_SIZE, GetDisplayStringManager()->GetString(DST_MAILSYSTEM_MSG_PLEASE_TARGETNAME), argList);
		GetAlarmManager()->vFormattedAlarmMessage( "DST_MAILSYSTEM_MSG_PLEASE_TARGETNAME", FALSE, NULL, argList );
		break;
	case MAIL_MSG_BOX_PLEASE_MAILCONTENT:
		//eMsgBoxWork	= MBW_NONE;
		//eMsgBoxTypeFlag	= MBTF_OK;
		//bPopup		= FALSE;
		//vswprintf_s(awcMessageBuffer, BUFFER_BOX_SIZE, GetDisplayStringManager()->GetString(DST_MAILSYSTEM_MSG_PLEASE_MAILCONTENT), argList);
		GetAlarmManager()->vFormattedAlarmMessage( "DST_MAILSYSTEM_MSG_PLEASE_MAILCONTENT", FALSE, NULL, argList );
		break;
	case MAIL_MSG_BOX_SEND_BASIC:
		//eMsgBoxWork	= MBW_MAILSYSTEM_SEND_ASK;
		//eMsgBoxTypeFlag	= MBTF_OK | MBTF_CANCEL;
		//bPopup		= TRUE;
		//vswprintf_s(awcMessageBuffer, BUFFER_BOX_SIZE, GetDisplayStringManager()->GetString(DST_MAILSYSTEM_MSG_SEND_BASIC), argList);		
		GetAlarmManager()->vFormattedAlarmMessage( "DST_MAILSYSTEM_MSG_SEND_BASIC", FALSE, NULL, argList );
		break;
	case MAIL_MSG_BOX_SEND_ITEM:
		//eMsgBoxWork	= MBW_MAILSYSTEM_SEND_ASK;
		//eMsgBoxTypeFlag	= MBTF_OK | MBTF_CANCEL;
		//bPopup		= TRUE;
		//vswprintf_s(awcMessageBuffer, BUFFER_BOX_SIZE, GetDisplayStringManager()->GetString(DST_MAILSYSTEM_MSG_SEND_ITEM), argList);
		GetAlarmManager()->vFormattedAlarmMessage( "DST_MAILSYSTEM_MSG_SEND_ITEM", FALSE, NULL, argList );
		break;
	case MAIL_MSG_BOX_SEND_ZENNY:
		//eMsgBoxWork	= MBW_MAILSYSTEM_SEND_ASK;
		//eMsgBoxTypeFlag	= MBTF_OK | MBTF_CANCEL;
		//bPopup		= TRUE;
		//vswprintf_s(awcMessageBuffer, BUFFER_BOX_SIZE, GetDisplayStringManager()->GetString(DST_MAILSYSTEM_MSG_SEND_ZENNY), argList);
		GetAlarmManager()->vFormattedAlarmMessage( "DST_MAILSYSTEM_MSG_SEND_ZENNY", FALSE, NULL, argList );
		break;
	case MAIL_MSG_BOX_SEND_ITEM_ZENNY:
		//eMsgBoxWork	= MBW_MAILSYSTEM_SEND_ASK;
		//eMsgBoxTypeFlag	= MBTF_OK | MBTF_CANCEL;
		//bPopup		= TRUE;
		//vswprintf_s(awcMessageBuffer, BUFFER_BOX_SIZE, GetDisplayStringManager()->GetString(DST_MAILSYSTEM_MSG_SEND_ITEM_ZENNY), argList);
		GetAlarmManager()->vFormattedAlarmMessage( "DST_MAILSYSTEM_MSG_SEND_ITEM_ZENNY", FALSE, NULL, argList );
		break;
	case MAIL_MSG_BOX_SEND_ITEM_ZENNY_REQ:
		//eMsgBoxWork	= MBW_MAILSYSTEM_SEND_ASK;
		//eMsgBoxTypeFlag	= MBTF_OK | MBTF_CANCEL;
		//bPopup		= TRUE;
		//vswprintf_s(awcMessageBuffer, BUFFER_BOX_SIZE, GetDisplayStringManager()->GetString(DST_MAILSYSTEM_MSG_SEND_ITEM_ZENNY_REQ), argList);
		GetAlarmManager()->vFormattedAlarmMessage( "DST_MAILSYSTEM_MSG_SEND_ITEM_ZENNY_REQ", FALSE, NULL, argList );
		break;
	case MAIL_MSG_BOX_SEND_ZENNY_REQ:
		//eMsgBoxWork	= MBW_MAILSYSTEM_SEND_ASK;
		//eMsgBoxTypeFlag	= MBTF_OK | MBTF_CANCEL;
		//bPopup		= TRUE;
		//vswprintf_s(awcMessageBuffer, BUFFER_BOX_SIZE, GetDisplayStringManager()->GetString(DST_MAILSYSTEM_MSG_SEND_ZENNY_REQ), argList);
		GetAlarmManager()->vFormattedAlarmMessage( "DST_MAILSYSTEM_MSG_SEND_ZENNY_REQ", FALSE, NULL, argList );
		break;
	case MAIL_MSG_BOX_AWAY_ON:
		//eMsgBoxWork	= MBW_MAILSYSTEM_AWAY_ON;
		//eMsgBoxTypeFlag	= MBTF_OK | MBTF_CANCEL;
		//bPopup		= TRUE;
		//vswprintf_s(awcMessageBuffer, BUFFER_BOX_SIZE, GetDisplayStringManager()->GetString(DST_MAILSYSTEM_MSG_AWAY_ON), argList);
		GetAlarmManager()->vFormattedAlarmMessage( "DST_MAILSYSTEM_MSG_AWAY_ON", FALSE, NULL, argList );
		break;
	case MAIL_MSG_BOX_AWAY_OFF:
		//eMsgBoxWork	= MBW_MAILSYSTEM_AWAY_OFF;
		//eMsgBoxTypeFlag	= MBTF_OK | MBTF_CANCEL;
		//bPopup		= TRUE;
		//vswprintf_s(awcMessageBuffer, BUFFER_BOX_SIZE, GetDisplayStringManager()->GetString(DST_MAILSYSTEM_MSG_AWAY_OFF), argList);
		GetAlarmManager()->vFormattedAlarmMessage( "DST_MAILSYSTEM_MSG_AWAY_OFF", FALSE, NULL, argList );
		break;
	case MAIL_MSG_BOX_RECEIVE_ITEM:
		//eMsgBoxWork	= MBW_MAILSYSTEM_RECEIVE_ASK;
		//eMsgBoxTypeFlag	= MBTF_OK | MBTF_CANCEL;
		//bPopup		= TRUE;
		//vswprintf_s(awcMessageBuffer, BUFFER_BOX_SIZE, GetDisplayStringManager()->GetString(DST_MAILSYSTEM_MSG_RECEIVE_ITEM_ACCEPT), argList);
		GetAlarmManager()->vFormattedAlarmMessage( "DST_MAILSYSTEM_MSG_RECEIVE_ITEM_ACCEPT", FALSE, NULL, argList );
		break;
	case MAIL_MSG_BOX_RECEIVE_ZENNY:
		//eMsgBoxWork	= MBW_MAILSYSTEM_RECEIVE_ASK;
		//eMsgBoxTypeFlag	= MBTF_OK | MBTF_CANCEL;
		//bPopup		= TRUE;
		//vswprintf_s(awcMessageBuffer, BUFFER_BOX_SIZE, GetDisplayStringManager()->GetString(DST_MAILSYSTEM_MSG_RECEIVE_ZENNY_ACCEPT), argList);
		GetAlarmManager()->vFormattedAlarmMessage( "DST_MAILSYSTEM_MSG_RECEIVE_ZENNY_ACCEPT", FALSE, NULL, argList );
		break;
	case MAIL_MSG_BOX_RECEIVE_ITEM_ZENNY:
		//eMsgBoxWork	= MBW_MAILSYSTEM_RECEIVE_ASK;
		//eMsgBoxTypeFlag	= MBTF_OK | MBTF_CANCEL;
		//bPopup		= TRUE;
		//vswprintf_s(awcMessageBuffer, BUFFER_BOX_SIZE, GetDisplayStringManager()->GetString(DST_MAILSYSTEM_MSG_RECEIVE_ITEM_ZENNY_ACCEPT), argList);
		GetAlarmManager()->vFormattedAlarmMessage( "DST_MAILSYSTEM_MSG_RECEIVE_ITEM_ZENNY_ACCEPT", FALSE, NULL, argList );
		break;
	case MAIL_MSG_BOX_RECEIVE_ITEM_ZENNY_REQ:
		//eMsgBoxWork	= MBW_MAILSYSTEM_RECEIVE_ASK;
		//eMsgBoxTypeFlag	= MBTF_OK | MBTF_CANCEL;
		//bPopup		= TRUE;
		//vswprintf_s(awcMessageBuffer, BUFFER_BOX_SIZE, GetDisplayStringManager()->GetString(DST_MAILSYSTEM_MSG_RECEIVE_ITEM_ZENNY_REQ_ACCEPT), argList);
		GetAlarmManager()->vFormattedAlarmMessage( "DST_MAILSYSTEM_MSG_RECEIVE_ITEM_ZENNY_REQ_ACCEPT", FALSE, NULL, argList );
		break;
	case MAIL_MSG_BOX_RECEIVE_ZENNY_REQ:
		//eMsgBoxWork	= MBW_MAILSYSTEM_RECEIVE_ASK;
		//eMsgBoxTypeFlag	= MBTF_OK | MBTF_CANCEL;
		//bPopup		= TRUE;
		//vswprintf_s(awcMessageBuffer, BUFFER_BOX_SIZE, GetDisplayStringManager()->GetString(DST_MAILSYSTEM_MSG_RECEIVE_ZENNY_REQ_ACCEPT), argList);
		GetAlarmManager()->vFormattedAlarmMessage( "DST_MAILSYSTEM_MSG_RECEIVE_ZENNY_REQ_ACCEPT", FALSE, NULL, argList );
		break;
	case MAIL_MSG_BOX_DELETE:
		//eMsgBoxWork	= MBW_MAILSYSTEM_DELETE_ASK;
		//eMsgBoxTypeFlag	= MBTF_OK | MBTF_CANCEL;
		//bPopup		= TRUE;
		//vswprintf_s(awcMessageBuffer, BUFFER_BOX_SIZE, GetDisplayStringManager()->GetString(DST_MAILSYSTEM_MSG_DELETE), argList);
		GetAlarmManager()->vFormattedAlarmMessage( "DST_MAILSYSTEM_MSG_DELETE", FALSE, NULL, argList );
		break;
	case MAIL_MSG_BOX_RETURN:
		//eMsgBoxWork	= MBW_MAILSYSTEM_RETURN_ASK;
		//eMsgBoxTypeFlag	= MBTF_OK | MBTF_CANCEL;
		//bPopup		= TRUE;
		//vswprintf_s(awcMessageBuffer, BUFFER_BOX_SIZE, GetDisplayStringManager()->GetString(DST_MAILSYSTEM_MSG_RETURN), argList);
		GetAlarmManager()->vFormattedAlarmMessage( "DST_MAILSYSTEM_MSG_RETURN", FALSE, NULL, argList );
		break;
	case MAIL_MSG_BOX_SELECT_REQ_OR_SEND:
		//eMsgBoxWork	= MBW_NONE;
		//eMsgBoxTypeFlag	= MBTF_OK;
		//bPopup		= FALSE;
		//vswprintf_s(awcMessageBuffer, BUFFER_BOX_SIZE, GetDisplayStringManager()->GetString(DST_MAILSYSTEM_MSG_SELECT_REQ_OR_SEND), argList);
		GetAlarmManager()->vFormattedAlarmMessage( "DST_MAILSYSTEM_MSG_SELECT_REQ_OR_SEND", FALSE, NULL, argList );
		break;
	case MAIL_MSG_BOX_MAILCONTENT_SLANGDER:
		//eMsgBoxWork	= MBW_NONE;
		//eMsgBoxTypeFlag	= MBTF_OK;
		//bPopup		= FALSE;
		//vswprintf_s(awcMessageBuffer, BUFFER_BOX_SIZE, GetDisplayStringManager()->GetString(DST_MAILSYSTEM_MSG_MAIL_CONTENT_SLANGDER), argList);
		GetAlarmManager()->vFormattedAlarmMessage( "DST_MAILSYSTEM_MSG_MAIL_CONTENT_SLANGDER", FALSE, NULL, argList );
		break;
	}
	va_end(argList);

	//CDboEventGenerator::MsgBoxShow(awcMessageBuffer, eMsgBoxWork, eMsgBoxTypeFlag, bPopup);
}

void CMailSystemMsg::ModReturnMsg(WCHAR* pwcBuffer, RwUInt32 uiSize, const sMAIL_PROFILE* pMailProfile)
{
	swprintf_s(pwcBuffer, uiSize, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MSG_RETRUN_MAIL"), pMailProfile->wszFromName);
}

void CMailSystemMsg::ModReplyMsg(WCHAR* pwcBuffer, RwUInt32 uiSize, const sMAIL_PROFILE* pMailProfile)
{
	RwUInt32		uiMailType	= 0;		
	RwUInt32		uiZenny		= 0;
	RwUInt32		uiItem		= INVALID_TBLIDX;
	
	swscanf_s(pMailProfile->wszText, L"T:%uI:%uZ:%u", &uiMailType, &uiItem, &uiZenny);

	switch (uiMailType)
	{
	case eMAIL_TYPE_BASIC:
		break;
	case eMAIL_TYPE_ITEM:
		if (uiItem == INVALID_TBLIDX)
		{
			swprintf_s(pwcBuffer, uiSize, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MSG_REPLY_MAIL_ITEM"), pMailProfile->wszFromName, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_NOT_IDENTIFICATION"));
		}
		else
		{
			swprintf_s(pwcBuffer, uiSize, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MSG_REPLY_MAIL_ITEM"), pMailProfile->wszFromName, CMailSystemMsg::GetItemName(uiItem));
		}		
		break;
	case eMAIL_TYPE_ZENNY:
		swprintf_s(pwcBuffer, uiSize, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MSG_REPLY_MAIL_ZENNY"), pMailProfile->wszFromName, Logic_FormatZeni(uiZenny));
		break;
	case eMAIL_TYPE_ITEM_ZENNY:
		if (uiItem == INVALID_TBLIDX)
		{
			swprintf_s(pwcBuffer, uiSize, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MSG_REPLY_MAIL_ITEM_ZENNY"), pMailProfile->wszFromName, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_NOT_IDENTIFICATION"), Logic_FormatZeni(uiZenny));
		}
		else
		{
			swprintf_s(pwcBuffer, uiSize, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MSG_REPLY_MAIL_ITEM_ZENNY"), pMailProfile->wszFromName, CMailSystemMsg::GetItemName(uiItem), Logic_FormatZeni(uiZenny));
		}		
		break;
	case eMAIL_TYPE_ITEM_ZENNY_REQ:
		if (uiItem == INVALID_TBLIDX)
		{
			swprintf_s(pwcBuffer, uiSize, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MSG_REPLY_MAIL_ITEM_ZENNY_REQ"), pMailProfile->wszFromName, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_NOT_IDENTIFICATION"), Logic_FormatZeni(uiZenny));
		}
		else
		{
			swprintf_s(pwcBuffer, uiSize, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MSG_REPLY_MAIL_ITEM_ZENNY_REQ"), pMailProfile->wszFromName, CMailSystemMsg::GetItemName(uiItem), Logic_FormatZeni(uiZenny));
		}
		break;
	case eMAIL_TYPE_ZENNY_REQ:
		swprintf_s(pwcBuffer, uiSize, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MSG_REPLY_MAIL_ZENNY_REQ"), pMailProfile->wszFromName, Logic_FormatZeni(uiZenny));
		break;
	}
}

const WCHAR* CMailSystemMsg::GetItemName(const sMAIL_PROFILE* pMailProfile, RwBool bItemText)
{
	if (pMailProfile->sItemProfile.bNeedToIdentify)
	{
		if (bItemText)
		{
			return GetDisplayStringManager()->GetString("DST_MAILSYSTEM_NOT_IDENTIFICATION_ITEM");
		}
		else
		{
			return GetDisplayStringManager()->GetString("DST_MAILSYSTEM_NOT_IDENTIFICATION");
		}			
	}
	else
	{
		CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

		sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData(pMailProfile->sItemProfile.tblidx);

		return pItemTextTable->GetText(pItemTbldat->Name).c_str();
	}
}

const WCHAR * CMailSystemMsg::GetItemName(TBLIDX itemTblidx)
{
	CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

	sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData(itemTblidx);

	return pItemTextTable->GetText(pItemTbldat->Name).c_str();
}

//////////////////////////////////////////////////////////////////////////
// CMailSlotGui
//////////////////////////////////////////////////////////////////////////
CMailSlotGui::CMailSlotGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_pBtnSlot(NULL)
,m_pStbName(NULL)
,m_pStbContent(NULL)
,m_pStbDay(NULL)
,m_pIconMail(NULL)
,m_pBtnLock(NULL)
,m_bCheck(false)
{
}

CMailSlotGui::~CMailSlotGui()
{
}

RwBool CMailSlotGui::Create(gui::CComponent* pParent, RwInt32 iX, RwInt32 iY)
{
	if (!Create())
	{
		return FALSE;
	}

	m_pThis->SetPosition(iX, iY);
	m_pThis->SetParent(pParent, true);

	return TRUE;
}

RwBool CMailSlotGui::Create()
{
	NTL_FUNCTION("CMailSlotGui::Create");

	if (!CNtlPLGui::Create("", "gui\\MailSystemSlot.srf", "gui\\MailSystemSlot.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis			= (gui::CDialog*)GetComponent("dlgMain");
	m_pBtnSlot		= (gui::CButton*)GetComponent("btnSlot");
	m_pStbName		= (gui::CStaticBox*)GetComponent("stbName");
	m_pStbContent	= (gui::CStaticBox*)GetComponent("stbContent");
	m_pStbDay		= (gui::CStaticBox*)GetComponent("stbDay");
	m_pBtnLock		= (gui::CButton*)GetComponent("btnLock");
	m_pBtnCheck		= (gui::CButton*)GetComponent("btnCheckBox");

	m_slotMove				= m_pThis->SigMove().Connect(this, &CMailSlotGui::OnMove);
	m_slotPaintBtnSlot		= m_pBtnSlot->SigPaint().Connect(this, &CMailSlotGui::OnPaintBtnSlot);
	m_slotClickedBtnSlot	= m_pBtnSlot->SigClicked().Connect(this, &CMailSlotGui::OnClickedBtnSlot);
	m_slotClickedBtnLock	= m_pBtnLock->SigClicked().Connect(this, &CMailSlotGui::OnClickedBtnLock);
	m_slotToggledBtnCheck	= m_pBtnCheck->SigToggled().Connect(this, &CMailSlotGui::OnToggledBtnCheck);

	m_IconMail[ICON_MAIL].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MailSystemSlot.srf", "srfIconMail"));
	m_IconMail[ICON_MAIL_ATTACH].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MailSystemSlot.srf", "srfIconMailAttach"));
	m_IconMail[ICON_MAIL_READ].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MailSystemSlot.srf", "srfIconMailRead"));
	m_IconMail[ICON_MAIL_ATTACH_READ].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MailSystemSlot.srf", "srfIconMailAttachRead"));
	
	for (int i = 0; i < ICON_NUM; ++i)
	{
		m_IconMail[i].SetPositionfromParent(dMAILSYSTEM_UI_SLOT_ICON_LEFT, dMAILSYSTEM_UI_SLOT_ICON_TOP);
	}

	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);

	ClearMailSlot();
	Show(false);
	NTL_RETURN(TRUE);
}

void CMailSlotGui::Destroy()
{
	NTL_FUNCTION("CMailSlotGui::Destroy");

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

void CMailSlotGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rect = m_pThis->GetScreenRect();
	for (int i = 0; i < ICON_NUM; ++i)
	{
		m_IconMail[i].SetPositionbyParent(rect.left, rect.top);
	}
}

void CMailSlotGui::OnPaintBtnSlot()
{
	if (m_pIconMail)
	{
		m_pIconMail->Render();
	}
}

void CMailSlotGui::OnClickedBtnSlot(gui::CComponent* pComponent)
{
	CNtlMailSystem* pMailSystem = GetNtlSLGlobal()->GetSobAvatar()->GetMailSystem();
	if( !pMailSystem )
		return;

	if (m_sMailProfile.bIsRead)
	{
		CNtlSLEventGenerator::MailRead(pMailSystem->GetUseObjectSerial(), m_sMailProfile.mailID, m_sMailProfile.endTime);
	}
	else
	{		
		GetDboGlobal()->GetGamePacketGenerator()->SendMailReadReq(pMailSystem->GetUseObjectSerial(), m_sMailProfile.mailID);
	}
}

void CMailSlotGui::OnClickedBtnLock(gui::CComponent* pComponent)
{
	if ((m_sMailProfile.byMailType == eMAIL_TYPE_ITEM_ZENNY_REQ ||
		m_sMailProfile.byMailType == eMAIL_TYPE_ZENNY_REQ) && !m_sMailProfile.bIsAccept)
	{
		CMailSystemMsg::MsgSay(CMailSystemMsg::MAIL_MSG_SAY_MAIL_NOT_LOCK);
	}
	else
	{
		// Packet Send Lock bToggled
		CNtlMailSystem* pMailSystem = GetNtlSLGlobal()->GetSobAvatar()->GetMailSystem();
		GetDboGlobal()->GetGamePacketGenerator()->SendMailLockReq(pMailSystem->GetUseObjectSerial(), m_sMailProfile.mailID, !m_bLock);
	}
}

void CMailSlotGui::OnToggledBtnCheck(gui::CComponent* pComponent, bool bDown)
{
	if( bDown )
	{
		if( m_sMailProfile.byMailType == eMAIL_TYPE_BASIC )
		{
			if( m_bLock )
			{
				GetAlarmManager()->AlarmMessage( "DST_MAILSYSTEM_NEGATIVE_CHECK_LOCK" );
				m_bCheck = false;
				CDboEventGenerator::MailSlotUpdate( eMAILSLOT_UPDATE_LOCK, 0, 0 );
				m_pBtnCheck->SetDown( false );
				return;
			}
		}
		else
		{
			if( !m_sMailProfile.bIsAccept )
			{
				GetAlarmManager()->AlarmMessage( "DST_MAILSYSTEM_NEGATIVE_CHECK_ATTACH" );
				m_bCheck = false;
				CDboEventGenerator::MailSlotUpdate( eMAILSLOT_UPDATE_LOCK, 0, 0 );
				m_pBtnCheck->SetDown( false );
				return;
			}

			if( m_bLock )
			{
				GetAlarmManager()->AlarmMessage( "DST_MAILSYSTEM_NEGATIVE_CHECK_LOCK" );
				m_bCheck = false;
				CDboEventGenerator::MailSlotUpdate( eMAILSLOT_UPDATE_LOCK, 0, 0 );
				m_pBtnCheck->SetDown( false );
				return;
			}
		}
	}
	
	m_bCheck = bDown;
	CDboEventGenerator::MailSlotUpdate( eMAILSLOT_UPDATE_LOCK, 0, 0 );
}

void CMailSlotGui::SetMailSlot(RwUInt32 uiPos, MAILID mailIDReading)
{
	CNtlMailSystem* pMailSystem = GetNtlSLGlobal()->GetSobAvatar()->GetMailSystem();
	if( !pMailSystem )
		return;

	if (pMailSystem->GetMail(m_sMailProfile, uiPos, TRUE))
	{
		UpdateMailSlot();
	}
	else
	{
		ClearMailSlot();
	}

	/*m_pBtnSlot->ClickEnable(mailIDReading != m_sMailProfile.mailID);*/
}

void CMailSlotGui::ClearMailSlot()
{
	m_sMailProfile	= sMAIL_PROFILE();
	m_pIconMail		= NULL;	

	m_pBtnSlot->ClickEnable(true);

	SetLock(false);
	Show(false);
}

void CMailSlotGui::UpdateMailSlot()
{
	CSurfaceGui* pIconReadNot	= NULL;
	CSurfaceGui* pIconRead		= NULL;
	if (m_sMailProfile.byMailType == eMAIL_TYPE_NONE)
	{
		Show(false);
		return;
	}
	else if (m_sMailProfile.byMailType != eMAIL_TYPE_BASIC && !m_sMailProfile.bIsAccept)
	{
		pIconReadNot	= &m_IconMail[ICON_MAIL_ATTACH];
		pIconRead		= &m_IconMail[ICON_MAIL_ATTACH_READ];
	}
	else
	{
		pIconReadNot	= &m_IconMail[ICON_MAIL];
		pIconRead		= &m_IconMail[ICON_MAIL_READ];
	}

	m_pStbName->SetText(m_sMailProfile.wszFromName);
	if (m_sMailProfile.bIsRead)
	{
		m_pIconMail = pIconRead;
		m_pStbName->CreateFontStd( DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
		m_pStbContent->CreateFontStd( DETAIL_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR );
	}
	else
	{
		m_pIconMail = pIconReadNot;
		m_pStbName->CreateFontStd( DETAIL_FONT, DEFAULT_FONT_SIZE, EZ_ATTR_BOLD );
		m_pStbContent->CreateFontStd( DETAIL_FONT, DEFAULT_FONT_SIZE, EZ_ATTR_BOLD );

	}

	switch (m_sMailProfile.bySenderType)
	{
	case eMAIL_SENDER_TYPE_BASIC:
		m_pStbName->SetTextColor(INFOCOLOR_7);
		break;
	case eMAIL_SENDER_TYPE_REPLY:
	case eMAIL_SENDER_TYPE_RETURN:
	case eMAIL_SENDER_TYPE_SYSTEM:
		m_pStbName->SetTextColor(INFOCOLOR_8);
		break;
	case eMAIL_SENDER_TYPE_GM:
		m_pStbName->SetTextColor(INFOCOLOR_9);
		break;
	default:
		DBO_ASSERT( FALSE, "CMailSlotGui::UpdateMailSlot - Invalid mail sender type" );
	}

	m_pStbName->Redraw();
	m_pStbContent->Redraw();

	if (m_sMailProfile.bIsLock)
	{
		m_pStbDay->SetText(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAIL_EXPIRED_DAY_LOCK"));
	}
	else
	{
		if( Logic_IsMailRemainTimeOverDay( m_sMailProfile.endTime ) )
		{
			m_pStbDay->SetTextColor(RGB(255, 255, 255));
			
		}
		else
		{
			m_pStbDay->SetTextColor(RGB(237, 19, 45));
		}

		m_pStbDay->SetText( Logic_GetMailRemainTimeString( m_sMailProfile.endTime ) );
	}
	
	SetLock(m_sMailProfile.bIsLock);

	WCHAR*	pMailContent = m_sMailProfile.wszText;
	WCHAR	aMailContent[NTL_MAX_LENGTH_OF_MAIL_MESSAGE + 1];
	switch (m_sMailProfile.bySenderType)
	{
	case eMAIL_SENDER_TYPE_REPLY:
		CMailSystemMsg::ModReplyMsg(aMailContent, NTL_MAX_LENGTH_OF_MAIL_MESSAGE + 1, &m_sMailProfile);
		pMailContent = aMailContent;
		break;
	case eMAIL_SENDER_TYPE_RETURN:
		CMailSystemMsg::ModReturnMsg(aMailContent, NTL_MAX_LENGTH_OF_MAIL_MESSAGE + 1, &m_sMailProfile);
		pMailContent = aMailContent;
		break;
	}
	m_pStbContent->SetText(pMailContent);

	Show(true);
}

void CMailSlotGui::SetLock(bool bLock)
{
	m_bLock = bLock;

	m_pBtnLock->GetSurfaceUp()->clear();
	m_pBtnLock->GetSurfaceDown()->clear();
	m_pBtnLock->GetSurfaceFocus()->clear();
	m_pBtnLock->GetSurfaceDisable()->clear();

	if (bLock)
	{
		m_pBtnLock->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MailSystemSlot.srf", "srfLockBtnUp"));
		m_pBtnLock->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MailSystemSlot.srf", "srfLockBtnDown"));
		m_pBtnLock->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MailSystemSlot.srf", "srfLockBtnFocus"));
		m_pBtnLock->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MailSystemSlot.srf", "srfLockBtnDown"));
	}
	else
	{
		m_pBtnLock->AddSurfaceUp(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MailSystemSlot.srf", "srfUnLockBtnUp"));
		m_pBtnLock->AddSurfaceDown(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MailSystemSlot.srf", "srfUnLockBtnDown"));
		m_pBtnLock->AddSurfaceFocus(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MailSystemSlot.srf", "srfUnLockBtnFocus"));
		m_pBtnLock->AddSurfaceDisabled(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MailSystemSlot.srf", "srfUnLockBtnDown"));
	}
}

RwBool CMailSlotGui::IsLock()
{
	return m_bLock;
}

RwBool CMailSlotGui::IsCheck()
{
	return m_bCheck;
}

void CMailSlotGui::CheckClear()
{
	m_pBtnCheck->SetDown( false );
	m_bCheck = false;
}

RwUInt32 CMailSlotGui::GetMailID()
{
	return m_sMailProfile.mailID;
}



//////////////////////////////////////////////////////////////////////////
// CMailSystemGui
//////////////////////////////////////////////////////////////////////////
CMailSystemGui::CMailSystemGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_pStbTitle(NULL)
,m_pBtnClose(NULL)
,m_pBtnAway(NULL)
,m_pStbPage(NULL)
,m_pBtnPrevPage(NULL)
,m_pBtnNextPage(NULL)
,m_pStbDay(NULL)
,m_pBtnUpDay(NULL)
,m_pBtnDownDay(NULL)
,m_pStbTargetNameTitle(NULL)
,m_pIpbTargetName(NULL)
,m_pStbMailContentTitle(NULL)
,m_pIpbMailContent(NULL)
,m_pStbAttachTitle(NULL)
,m_pBtnAttachReq(NULL)
,m_pBtnAttachSend(NULL)
,m_pStbAttachZenny(NULL)
,m_pBtnSend(NULL)
,m_pBtnClear(NULL)
,m_pDlgAttachItem(NULL)
,m_pDlgAttach(NULL)
,m_uiMailBoxPageCur(0)
,m_uiMailBoxPageMax(0)
,m_uiSendMailDay(MAILSYSTEM_EXPIRED_DAY_MAX)
,m_uiSendMailZenny(0)
,m_bMailBoxAway(false)
,m_bMailBoxFocus(false)
,m_fReloadTime(0.0f)
,m_pBtnReload(NULL)
,m_pBtnDelete(NULL)
,m_eSendMailStamp(MAILSYSTEM_STAMP_BASIC)
,m_pStbMaxNumText(NULL)
,m_uiAbilityType(CNtlMailSystem::MAILSYSTEM_ABILITY_NONE)
,m_mailIDReading(INVALID_MAILID)
,m_pBtnAttachZenny(NULL)
,m_pStbAttachMessage(NULL)
,m_pMoneyIconTexture(NULL)
,m_pStbLeftDay(NULL)
,m_bEffectZenny(FALSE)
{
	for (int i = 0; i < TAB_NUM; ++i)
	{
		m_apDlgTab[i]	= NULL;
		m_uiIndexTab[i]	= TAB_NUM;
	}
	for (int i = 0; i < SLOT_NUM; ++i)
	{
		m_apMailSlot[i] = NULL;
	}	
}

CMailSystemGui::~CMailSystemGui()
{
}

RwBool CMailSystemGui::Create()
{
	NTL_FUNCTION("CMailSystemGui::Create");

	if (!CNtlPLGui::Create("", "gui\\MailSystem.srf", "gui\\MailSystem.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis						= (gui::CDialog*)GetComponent("dlgMain");
	m_pStbTitle					= (gui::CStaticBox*)GetComponent("stbTitle");
	m_pBtnClose					= (gui::CButton*)GetComponent("btnClose");
	m_pTabButton				= (gui::CTabButton*)GetComponent("tabButton");
	m_apDlgTab[TAB_SENDMAIL]	= (gui::CDialog*)GetComponent("dlgSendMail");
	m_apDlgTab[TAB_MAILBOX]		= (gui::CDialog*)GetComponent("dlgMailBox");

	// Dialog Priority
	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);

	m_slotClickedBtnClose	= m_pBtnClose->SigClicked().Connect(this, &CMailSystemGui::OnClickedBtnClose);
	m_slotChangedTabButton	= m_pTabButton->SigSelectChanged().Connect(this, &CMailSystemGui::OnChangedTabButton);
	m_slotCaptureMouseDown		= GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect(this, &CMailSystemGui::OnCaptureMouseDown);
	
	m_pStbTitle->SetPosition(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y);
	m_pStbTitle->SetText(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAILBOX"));

	m_pMoneyIconTexture = Logic_CreateTexture( MONEYICON_NAME );

	if (!CreateSendMail(m_apDlgTab[TAB_SENDMAIL]))
	{
		NTL_RETURN(FALSE);
	}
	if (!CreateMailBox(m_apDlgTab[TAB_MAILBOX]))
	{
		NTL_RETURN(FALSE);
	}

	// Dialog Moved Mode를 대비하여 OnMove 호출
	CRectangle rtDlgAttach = m_pDlgAttach->GetScreenRect();
	OnMoveDlgAttach( rtDlgAttach.left, rtDlgAttach.top );

	CRectangle rtDlgAttachItem = m_pDlgAttachItem->GetScreenRect();
	OnMoveDlgAttachItem( rtDlgAttach.left, rtDlgAttach.top );

	LinkMsg(g_EventSobInfoUpdate, 0);
	LinkMsg(g_EventNPCDialogOpen, 0);
	LinkMsg(g_EventCalcPopupResult, 0);
	LinkMsg(g_EventMsgBoxResult, 0);
	LinkMsg(g_EventOpenSendMail, 0);
	LinkMsg(g_EventMailStart, 0);
	LinkMsg(g_EventMailSend, 0);
	LinkMsg(g_EventMailRead, 0);
	LinkMsg(g_EventMailDel, 0);
	LinkMsg(g_EventMailMultiDelRes, 0);
	LinkMsg(g_EventMailReturn, 0);
	LinkMsg(g_EventMailItemReceive, 0);
	LinkMsg(g_EventMailLoad, 0);
	LinkMsg(g_EventMailLock, 0);
	LinkMsg(g_EventMailCloseNfy, 0);
	LinkMsg(g_EventCharAway, 0);
	LinkMsg(g_EventMailSlotUpdate, 0 );

	LinkMsg(g_EventReadingMail, 0);
	LinkMsg(g_EventRegMailAttachItem, 0 );
	LinkMsg(g_EventIconMoveClick, 0);

	LinkMsg(g_EventFriendList_Info);
	LinkMsg(g_EventFriendAdd_Res);
	LinkMsg(g_EventFriendDel_Res);
	LinkMsg(g_EventFriendMove_Res);


	GetNtlGuiManager()->AddUpdateFunc(this);

	ClearUserInterface();
	Show(false);
	NTL_RETURN(TRUE);
}

RwBool CMailSystemGui::CreateSendMail(gui::CComponent* pParent)
{
	m_pStbDay				= (gui::CStaticBox*)GetComponent("stbDay");
	/*m_pPnlIconDay			= (gui::CPanel*)GetComponent("pnlIconDay");*/
	m_pStbLeftDay			= (gui::CStaticBox*)GetComponent("stbLeftDay");
	m_pBtnUpDay				= (gui::CButton*)GetComponent("btnUpDay");
	m_pBtnDownDay			= (gui::CButton*)GetComponent("btnDownDay");
	m_pStbTargetNameTitle	= (gui::CStaticBox*)GetComponent("stbTargetNameTitle");
	m_pIpbTargetName		= (gui::CInputBox*)GetComponent("ipbTargetName");
	m_pStbMailContentTitle	= (gui::CStaticBox*)GetComponent("stbMailContentTitle");
	m_pIpbMailContent		= (gui::CInputBox*)GetComponent("ipbMailContent");

	m_pcbbFriendList = (gui::CComboBox*)GetComponent("cbbFriendList");

	m_pStbAttachTitle		= (gui::CStaticBox*)GetComponent("stbAttachTitle");
	m_pBtnAttachReq			= (gui::CButton*)GetComponent("btnAttachReq");
	m_pBtnAttachSend		= (gui::CButton*)GetComponent("btnAttachSend");
	m_pStbAttachZenny		= (gui::CStaticBox*)GetComponent("stbAttachZenny");
	m_pBtnSend				= (gui::CButton*)GetComponent("btnSend");
	m_pBtnClear				= (gui::CButton*)GetComponent("btnClear");
	m_pDlgAttachItem		= (gui::CDialog*)GetComponent("dlgAttachItem");
	m_pDlgAttach			= (gui::CDialog*)GetComponent("dlgAttach");
	/*m_pStbStamp				= (gui::CStaticBox*)GetComponent("stbStamp");*/
	m_pStbMaxNumText		= (gui::CStaticBox*)GetComponent("stbMaxNumText");
	m_pBtnAttachZenny		= (gui::CButton*)GetComponent("btnAttachZenny");
	m_pStbAttachMessage		= (gui::CStaticBox*)GetComponent("stbAttachMessage");

	m_AttachItemSlot.Create(m_pDlgAttachItem, DIALOG_MAILSYSTEM, REGULAR_SLOT_ITEM_SOB, SDS_COUNT);
	m_AttachItemSlot.SetPosition_fromParent(0, 0);
	m_AttachItemSlot.SetParentPosition(m_pDlgAttachItem->GetScreenRect().left, m_pDlgAttachItem->GetScreenRect().top);

	m_EffectFocus.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotFocusEffect"));
	m_EffectFocus.SetPositionfromParent(0, 0);
	m_EffectFocus.SetSize(DBOGUI_ICON_SIZE, DBOGUI_ICON_SIZE);

	m_AttachDisable.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MailSystem.srf", "srfAttachDisable"));
	m_AttachDisable.SetPositionfromParent(6, -1);

	m_EffectZenny.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MailSystem.srf", "srfBgAttachZennyEffect"));
	m_EffectZenny.SetPositionfromParent(0, 0);
	m_EffectZenny.SetSize(129, 20);

	m_slotKeyUpIpbMailContext		= m_pIpbMailContent->SigKeyUp().Connect(this, &CMailSystemGui::OnKeyUp);
	m_slotKeyUpIpbTargetName		= m_pIpbTargetName->SigKeyUp().Connect(this, &CMailSystemGui::OnKeyUp);
	m_slotCharChangedIpbMailCotent	= m_pIpbMailContent->SigCharChanged().Connect(this, &CMailSystemGui::OnCharChangedIpbMailContent);

	m_slotGotFocusIpbMailContent	= m_pIpbMailContent->SigGotFocus().Connect(this, &CMailSystemGui::OnGotFocus);
	m_slotLostFocusIpbMailContent	= m_pIpbMailContent->SigLostFocus().Connect(this, &CMailSystemGui::OnLostFocus);
	m_slotGotFocusIpbTargetName		= m_pIpbTargetName->SigGotFocus().Connect(this, &CMailSystemGui::OnGotFocus);
	m_slotLostFocusIpbTargetName	= m_pIpbTargetName->SigLostFocus().Connect(this, &CMailSystemGui::OnLostFocus);
	m_slotClickedBtnUpDay			= m_pBtnUpDay->SigClicked().Connect(this, &CMailSystemGui::OnClickedBtnUpDay);
	m_slotClickedBtnDownDay			= m_pBtnDownDay->SigClicked().Connect(this, &CMailSystemGui::OnClickedBtnDownDay);
	m_slotToggledBtnAttachSend		= m_pBtnAttachSend->SigToggled().Connect(this, &CMailSystemGui::OnToggledBtnAttachSend);
	m_slotToggledBtnAttachReq		= m_pBtnAttachReq->SigToggled().Connect(this, &CMailSystemGui::OnToggledBtnAttachReq);
	m_slotClickedBtnAttachZenny		= m_pBtnAttachZenny->SigClicked().Connect(this, &CMailSystemGui::OnClickedBtnAttachZenny);
	m_slotMoveDlgAttachItem			= m_pDlgAttachItem->SigMove().Connect(this, &CMailSystemGui::OnMoveDlgAttachItem);
	m_slotPaintDlgAttachItem		= m_pDlgAttachItem->SigPaint().Connect(this, &CMailSystemGui::OnPaintDlgAttachItem);

	m_slotMouseDownDlgAttachItem	= m_pDlgAttachItem->SigMouseDown().Connect(this, &CMailSystemGui::OnMouseDownDlgAttachItem);
	m_slotMouseUpDlgAttachItem		= m_pDlgAttachItem->SigMouseUp().Connect(this, &CMailSystemGui::OnMouseUpDlgAttachItem);
	m_slotMouseEnterDlgAttachItem	= m_pDlgAttachItem->SigMouseEnter().Connect(this, &CMailSystemGui::OnMouseEnterDlgAttachItem);
	m_slotMouseLeaveDlgAttachItem	= m_pDlgAttachItem->SigMouseLeave().Connect(this, &CMailSystemGui::OnMouseLeaveDlgAttachItem);

	m_slotClickedBtnSend			= m_pBtnSend->SigClicked().Connect(this, &CMailSystemGui::OnClickedBtnSend);
	m_slotClickedBtnClear			= m_pBtnClear->SigClicked().Connect(this, &CMailSystemGui::OnClickedBtnClear);

	m_slotMoveDlgAttach				= m_pDlgAttach->SigMove().Connect(this, &CMailSystemGui::OnMoveDlgAttach);
	m_slotPaintPostDlgAttach		= m_pDlgAttachItem->SigPaint().Connect(this, &CMailSystemGui::OnPaintPostDlgAttach);

	m_pIpbTargetName->SetMaxLength(NTL_MAX_SIZE_CHAR_NAME);
	m_pIpbTargetName->SetCaretSize(2, 13);
	m_pIpbMailContent->SetMultilineMode(TRUE);
	m_pIpbMailContent->SetMaxLength(NTL_MAX_LENGTH_OF_MAIL_MESSAGE);

	m_pStbTargetNameTitle->SetText(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAIL_TO"));
	m_pStbLeftDay->SetText(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAIL_EXPIRED_DAY_TITLE"));
	m_pStbMailContentTitle->SetText(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAIL_CONTENT"));
	m_pStbAttachTitle->SetText(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAIL_ATTACH"));
	m_pBtnAttachReq->SetText(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAIL_ATTACH_REQ"));
	m_pBtnAttachSend->SetText(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAIL_ATTACH_SEND"));
	m_pBtnSend->SetText(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAIL_SEND"));
	m_pBtnClear->SetText(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAIL_RESET"));
	m_pStbAttachMessage->SetText(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_ATTACH_DISABLED_MSG"));

	m_pcbbFriendList->GetButton()->SetToolTip(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_TOOLTIP_FRIEND_LIST"));
	m_slotSelectedFriend = m_pcbbFriendList->SigSelected().Connect(this, &CMailSystemGui::OnSelectedFriend);

	//char acBuffer[1024];
	//::WideCharToMultiByte(GetACP(), 0, GetDisplayStringManager()->GetString(DST_MAILSYSTEM_TOOLTIP_EXPIREDDAY), -1, acBuffer, 256, NULL, NULL);
	//m_pPnlIconDay->SetToolTip(acBuffer);

	return TRUE;
}

RwBool CMailSystemGui::CreateMailBox(gui::CComponent* pParent)
{
	m_pBtnAway		= (gui::CButton*)GetComponent("btnAway");
	m_pBtnPrevPage	= (gui::CButton*)GetComponent("btnPrevPage");
	m_pBtnNextPage	= (gui::CButton*)GetComponent("btnNextPage");
	m_pStbPage		= (gui::CStaticBox*)GetComponent("stbPage");
	m_pBtnReload	= (gui::CButton*)GetComponent("btnReload");
	m_pBtnDelete	= (gui::CButton*)GetComponent("btnDelete");
	m_pBtnDelete->SetToolTip( GetDisplayStringManager()->GetString( "DST_MAILSYSTEM_TOOLTIP_DELETE_BUTTON" ) );
	m_pPanEmptyMail	= (gui::CPanel*)GetComponent("panEmptyMail");
	m_pStbEmptyMail = (gui::CStaticBox*)GetComponent("stbEmptyMail");

	m_pStbEmptyMail->SetText( GetDisplayStringManager()->GetString( "DST_MAILSYSTEM_MAIL_EMPTYMAIL" ) );

	m_slotClickedBtnPrevPage	= m_pBtnPrevPage->SigClicked().Connect(this, &CMailSystemGui::OnClickedBtnPrevPage);
	m_slotClickedBtnNextPage	= m_pBtnNextPage->SigClicked().Connect(this, &CMailSystemGui::OnClickedBtnNextPage);
	m_slotClickedBtnReload		= m_pBtnReload->SigClicked().Connect(this, &CMailSystemGui::OnClickedBtnReload);
	m_slotClickedBtnDelete		= m_pBtnDelete->SigClicked().Connect(this, &CMailSystemGui::OnClickedBtnDelete);
	m_slotToggledBtnAway		= m_pBtnAway->SigToggled().Connect(this, &CMailSystemGui::OnToggledBtnAway);
	
	RwInt32	iX = dMAILSYSTEM_UI_SLOT_LEFT;
	RwInt32	iY = dMAILSYSTEM_UI_SLOT_TOP;
	for (int i = 0; i < SLOT_NUM; ++i, iY += dMAILSYSTEM_UI_SLOT_GAP)
	{
		char buf[256];
		sprintf_s(buf, 256, "MailSlotGui_%d", i);
		m_apMailSlot[i] = NTL_NEW CMailSlotGui(buf);
		if (!m_apMailSlot[i]->Create(pParent, iX, iY))
		{
			m_apMailSlot[i]->Destroy(); 
			NTL_DELETE(m_apMailSlot[i]);
			return FALSE;
		}
		CNtlPLGuiManager::GetInstance()->AddGui(m_apMailSlot[i]); 
	}

	m_pBtnAway->SetText(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAIL_AWAY"));
	
	char acBuffer[1024];
	::WideCharToMultiByte(GetACP(), 0, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_TOOLTIP_RELOAD"), -1, acBuffer, 256, NULL, NULL);
	m_pBtnReload->SetToolTip(acBuffer);

	return TRUE;
}

void CMailSystemGui::Destroy()
{
	NTL_FUNCTION("CMailSystemGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFunc(this);

	// 돈 아이콘
	Logic_DeleteTexture( m_pMoneyIconTexture );
	m_pMoneyIconTexture = NULL;
	
	for (int i = 0; i < SLOT_NUM; ++i)
	{
		if (m_apMailSlot[i])
		{
			CNtlPLGuiManager::GetInstance()->RemoveGui(m_apMailSlot[i]);
			m_apMailSlot[i]->Destroy();
			NTL_DELETE(m_apMailSlot[i]);
		}
	}

	m_AttachItemSlot.Destroy();

	UnLinkMsg(g_EventIconMoveClick);
	UnLinkMsg(g_EventSobInfoUpdate);
	UnLinkMsg(g_EventNPCDialogOpen);
	UnLinkMsg(g_EventCalcPopupResult);
	UnLinkMsg(g_EventMsgBoxResult);
	UnLinkMsg(g_EventOpenSendMail);
	UnLinkMsg(g_EventMailStart);
	UnLinkMsg(g_EventMailSend);
	UnLinkMsg(g_EventMailRead);
	UnLinkMsg(g_EventMailDel);
	UnLinkMsg(g_EventMailMultiDelRes);
	UnLinkMsg(g_EventMailReturn);
	UnLinkMsg(g_EventMailItemReceive);
	UnLinkMsg(g_EventMailLoad);
	UnLinkMsg(g_EventMailLock);
	UnLinkMsg(g_EventMailCloseNfy);
	UnLinkMsg(g_EventCharAway);
	UnLinkMsg(g_EventMailSlotUpdate);

	UnLinkMsg(g_EventReadingMail);

	UnLinkMsg(g_EventRegMailAttachItem );

	UnLinkMsg(g_EventHLShopEventGift);
	UnLinkMsg(g_EventFriendList_Info);
	UnLinkMsg(g_EventFriendAdd_Res);
	UnLinkMsg(g_EventFriendDel_Res);
	UnLinkMsg(g_EventFriendMove_Res);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

void CMailSystemGui::Update(RwReal fElapsed)
{
	if (m_pBtnReload->IsClickEnable())
	{
		return;
	}

	m_fReloadTime += fElapsed;
	if (m_fReloadTime > dMAILSYSTEM_REFRESH_TIME )
	{
		m_pBtnReload->ClickEnable(true);
		m_fReloadTime	= 0;
	}
}

RwInt32 CMailSystemGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);
	if (bOpen)
	{
	}
	else
	{
		ClearUserInterface();		
	}
	return TRUE;
}

void CMailSystemGui::HandleEvents(RWS::CMsg& msg)
{
	NTL_FUNCTION("CMailSystemGui::HandleEvents");

	if (msg.Id == g_EventSobInfoUpdate)
	{
		SNtlEventSobInfoUpdate* pEvent = reinterpret_cast<SNtlEventSobInfoUpdate*>(msg.pData);

		if( GetDialogManager()->IsOpenDialog( DIALOG_MAILSYSTEM ) )
		{
			if( Logic_IsUpdateType( EVENT_AIUT_ITEM, msg.pData ) )
			{
				if (pEvent->hSerialId == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID())
				{
					if (m_uiAbilityType != GetNtlSLGlobal()->GetSobAvatar()->GetMailSystem()->GetAbility())
					{
						GetDialogManager()->CloseDialog(DIALOG_MAILSYSTEM);
					}
				}
			}
		}
	}
	else if (msg.Id == g_EventNPCDialogOpen)
	{
		SDboNpcDialogOpen* pEvent = reinterpret_cast<SDboNpcDialogOpen*>(msg.pData);
	
		if (pEvent->eDialog == DIALOG_MAILSYSTEM)
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendMailStartReq(pEvent->hSerialId);
		}
	}
	// 계산기로 입력한 값이 나왔을 경우
	else if (msg.Id == g_EventCalcPopupResult)
	{
		SDboEventCalcPopupResult* pEvent = reinterpret_cast<SDboEventCalcPopupResult*>(msg.pData);

		if (pEvent->nSrcPlace == PLACE_SUB_MAILSYSTEM_ZENNY)
		{
			switch (pEvent->uiSerial)
			{
				case MAILSYSTEM_ATTACH_SEND:
				{
					SetSendMailZenny(true, false, pEvent->uiValue);
				}
				break;
				case MAILSYSTEM_ATTACH_REQ:
				{
					SetSendMailZenny(false, true, pEvent->uiValue);
				}
				break;
			}
		}
	}
	else if (msg.Id == g_EventMsgBoxResult)
	{
		SDboEventMsgBoxResult* pEvent = reinterpret_cast<SDboEventMsgBoxResult*>(msg.pData);

		if( pEvent->eResult == MBR_OK )
		{
			if (pEvent->strID == "DST_MAILSYSTEM_MSG_SEND_BASIC"
				|| pEvent->strID == "DST_MAILSYSTEM_MSG_SEND_ITEM"
				|| pEvent->strID == "DST_MAILSYSTEM_MSG_SEND_ZENNY"
				|| pEvent->strID == "DST_MAILSYSTEM_MSG_SEND_ITEM_ZENNY"
				|| pEvent->strID == "DST_MAILSYSTEM_MSG_SEND_ITEM_ZENNY_REQ"
				|| pEvent->strID == "DST_MAILSYSTEM_MSG_SEND_ZENNY_REQ")
			{
				SendMail();
			}
			else if (pEvent->strID == "DST_MAILSYSTEM_MSG_AWAY_ON")
				GetDboGlobal()->GetGamePacketGenerator()->SendCharAwayReq(true);
			else if (pEvent->strID == "DST_MAILSYSTEM_MSG_AWAY_OFF")
				GetDboGlobal()->GetGamePacketGenerator()->SendCharAwayReq(false);
		}
	}
	else if (msg.Id == g_EventOpenSendMail)
	{
		SDboEventOpenSendMail* pEvent = reinterpret_cast<SDboEventOpenSendMail*>(msg.pData);
		
		SetSendMailTargetName(pEvent->awcTargetName);
		if (GetDialogManager()->IsOpenDialog(DIALOG_MAILSYSTEM))
		{	
			if (lstrlenW(m_awcTargetName) > 0)
			{
				if ((RwUInt32)m_pTabButton->GetButtonCount() > m_uiIndexTab[TAB_SENDMAIL])
				{
					m_pIpbTargetName->SetText(m_awcTargetName);
					OpenSendMail();
				}
			}
		}
		else
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendMailStartReq(INVALID_SERIAL_ID);			
		}

	}
	else if (msg.Id == g_EventMailStart)
	{
		SNtlEventMailStart* pEvent = reinterpret_cast<SNtlEventMailStart*>(msg.pData);

		if (pEvent->bSucceeded && GetNtlSLGlobal()->GetSobAvatar()->GetMailSystem()->GetAbility() != CNtlMailSystem::MAILSYSTEM_ABILITY_NONE)
		{	
			if (GetDialogManager()->IsOpenDialog(DIALOG_MAILSYSTEM))
			{
				ClearUserInterface();
			}
			
			OpenUserInterface(GetNtlSLGlobal()->GetSobAvatar()->GetMailSystem()->GetAbility());
			if (lstrlenW(m_awcTargetName) > 0)
			{
				if (m_pTabButton->GetButtonCount() > (int)m_uiIndexTab[TAB_SENDMAIL])
				{
					m_pIpbTargetName->SetText(m_awcTargetName);
					OpenSendMail();
				}
			}
			UpdateUserInterface();
			GetDialogManager()->OpenDialog(DIALOG_MAILSYSTEM);
		}
		else
		{
			if (GetDialogManager()->IsOpenDialog(DIALOG_MAILSYSTEM))
			{
				GetDialogManager()->CloseDialog(DIALOG_MAILSYSTEM);				
			}
			else
			{
				SetSendMailTargetName(NULL);
			}
		}
	}
	else if (msg.Id == g_EventMailSend)
	{
		SNtlEventMailSend* pEvent = reinterpret_cast<SNtlEventMailSend*>(msg.pData);

		OpenMailBox();
		ClearSendMail();

		CMailSystemMsg::MsgSay(CMailSystemMsg::MAIL_MSG_SAY_SEND_SUCCEED, pEvent->pwcTargetName);
	}
	else if (msg.Id == g_EventMailRead ||
			 msg.Id == g_EventMailDel ||
			 msg.Id == g_EventMailMultiDelRes ||
			 msg.Id == g_EventMailReturn ||
			 msg.Id == g_EventMailLoad)
	{
		UpdateUserInterface();

		RwInt32 nDelIdx = 0;
		for (RwInt32 i = 0; i < SLOT_NUM; ++i)
		{
			if( m_apMailSlot[i]->IsCheck() )
				nDelIdx++;
		}

		if( nDelIdx <= 0 )
		{
			m_pBtnDelete->ClickEnable( FALSE );
		}
		else
		{
			m_pBtnDelete->ClickEnable( TRUE );
		}
	}
	else if( msg.Id == g_EventMailLock )
	{
		SNtlEventMailLock* pData = reinterpret_cast<SNtlEventMailLock*>(msg.pData);

		for( RwInt32 i = 0; i < SLOT_NUM; ++i )
		{
			if( pData->mailID == m_apMailSlot[i]->GetMailID() )
			{
				m_apMailSlot[i]->SetMailSlot( (m_uiMailBoxPageCur*SLOT_NUM) + i, pData->mailID );
				return;
			}
		}
	}
	else if (msg.Id == g_EventCharAway)
	{
		SNtlEventCharAway* pEvent = reinterpret_cast<SNtlEventCharAway*>(msg.pData);
		SetMailBoxAway(B2b(pEvent->bIsAway));
	}
	else if (msg.Id == g_EventReadingMail)
	{
		SDboEventReadingMail* pEvent = reinterpret_cast<SDboEventReadingMail*>(msg.pData);
		if (pEvent->bReading)
		{
			m_mailIDReading = pEvent->uiMailID;
		}
		else
		{
			m_mailIDReading = INVALID_MAILID;
		}
		/*UpdateUserInterface();*/
	}
	else if (msg.Id == g_EventMailCloseNfy)
	{
		GetDialogManager()->CloseDialog(DIALOG_MAILSYSTEM);
	}
	// Test
	else if( msg.Id == g_EventRegMailAttachItem )
	{
		SDboEventRegMailAttachItem* pData = reinterpret_cast<SDboEventRegMailAttachItem*>( msg.pData );

		if ((RwUInt32)m_pTabButton->GetSelectedButtonIndex() != m_uiIndexTab[TAB_SENDMAIL])
			NTL_RETURNVOID();

		if (m_AttachItemSlot.GetSerial() == INVALID_SERIAL_ID)
		{
			if( !m_pDlgAttach->IsEnabled() )
				NTL_RETURNVOID();

			CNtlSobItem*		pSobItem	= reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject( pData->hHandle ));
			if( !pSobItem )
				NTL_RETURNVOID();

			CNtlSobItemAttr*	pSobItemAttr= reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());
			if( !pSobItemAttr )
				NTL_RETURNVOID();

			// 탭 전환
			SelectTab( TAB_SENDMAIL );
			SetSendMailItem( pData->hHandle, (RwUInt8)Logic_WhichBagHasItem( pData->hHandle ), (RwUInt8)pData->uiPos, pSobItemAttr->GetStackNum() );
		}
	}
	else if( msg.Id == g_EventMailSlotUpdate )
	{
		SDboEventMailSlotUpdate* pData = reinterpret_cast<SDboEventMailSlotUpdate*>( msg.pData );

		// Mail slot Lock Update 일 경우
		if( pData->byType == eMAILSLOT_UPDATE_LOCK )
		{
			RwInt32 nDelIdx = 0;
			for (RwInt32 i = 0; i < SLOT_NUM; ++i)
			{
				if( m_apMailSlot[i]->IsCheck() )
					nDelIdx++;
			}

			if( nDelIdx <= 0 )
			{
				m_pBtnDelete->ClickEnable( FALSE );
			}
			else
			{
				m_pBtnDelete->ClickEnable( TRUE );
			}
		}
	}
	else if(msg.Id == g_EventIconMoveClick)
	{
		RwBool bPickUp = (RwBool)msg.pData;

		if( bPickUp )
		{
			if( m_pBtnAttachSend->IsDown() )
			{
				if( GetIconMoveManager()->GetSrcPlace() == PLACE_SUB_BAG_ZENNY )
				{
					m_bEffectZenny = TRUE;
				}
			}
		}
		else
		{
			m_bEffectZenny = FALSE;
		}
	}
	else if (msg.Id == g_EventFriendList_Info)
	{
		SNtlEventFriendListInfo* pData = reinterpret_cast<SNtlEventFriendListInfo*>(msg.pData);

		for (int i = 0; i < pData->byCount; i++)
		{
			if (!pData->asInfo[i].bIsBlack)
			{
				m_pcbbFriendList->AddItem(pData->asInfo[i].wchName, pData->asInfo[i].charID);
			}
		}
		}
		else if (msg.Id == g_EventFriendAdd_Res)
		{
		SNtlEventFriendAddRes* pData = reinterpret_cast<SNtlEventFriendAddRes*>(msg.pData);

		m_pcbbFriendList->AddItem(pData->wchName, pData->uiTargetID);
	}
	else if (msg.Id == g_EventFriendDel_Res)
	{
		SNtlEventFriendDelRes* pData = reinterpret_cast<SNtlEventFriendDelRes*>(msg.pData);

		int nCount = m_pcbbFriendList->GetListBox()->GetItemCount();
		for (int i = 0; i < nCount; i++)
		{
			if (m_pcbbFriendList->GetItemData(i) == pData->uiTargetID)
			{
				m_pcbbFriendList->RemoveItem(i);
				break;
			}
		}
	}
	else if (msg.Id == g_EventFriendMove_Res)
	{
		SNtlEventFriendMoveRes* pData = reinterpret_cast<SNtlEventFriendMoveRes*>(msg.pData);

		int nCount = m_pcbbFriendList->GetListBox()->GetItemCount();
		for (int i = 0; i < nCount; i++)
		{
			if (m_pcbbFriendList->GetItemData(i) == pData->uiTargetID)
			{
				m_pcbbFriendList->RemoveItem(i);
				break;
			}
		}
	}

	NTL_RETURNVOID();
}

void CMailSystemGui::SetSendMailTargetName(WCHAR* pwcTargetName)
{
	if (pwcTargetName)
	{
		wcscpy_s(m_awcTargetName, NTL_MAX_SIZE_CHAR_NAME + 1, pwcTargetName);
	}
	else
	{
		memset(m_awcTargetName, NULL, (NTL_MAX_SIZE_CHAR_NAME + 1) * sizeof(WCHAR));
	}
}


void CMailSystemGui::OnClickedBtnClose(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_MAILSYSTEM);
}

void CMailSystemGui::OnClickedBtnPrevPage(gui::CComponent* pComponent)
{
	if (m_uiMailBoxPageCur > 0)
	{
		SetMailBoxPage(m_uiMailBoxPageCur - 1);
	}
}

void CMailSystemGui::OnClickedBtnNextPage(gui::CComponent* pComponent)
{
	if (m_uiMailBoxPageCur < m_uiMailBoxPageMax)
	{
		SetMailBoxPage(m_uiMailBoxPageCur + 1);
	}
}

void CMailSystemGui::OnClickedBtnReload(gui::CComponent* pComponent)
{
	m_pBtnReload->ClickEnable(false);
	CNtlMailSystem* pMailSystem = GetNtlSLGlobal()->GetSobAvatar()->GetMailSystem();
	GetDboGlobal()->GetGamePacketGenerator()->SendMailReloadReq(pMailSystem->GetUseObjectSerial());
}

void CMailSystemGui::OnClickedBtnDelete( gui::CComponent* pComponent )
{
	MAILID aMultiDel[SLOT_NUM] = { INVALID_MAILID, };
	RwInt32 nDelIdx = 0;
	for (RwInt32 i = 0; i < SLOT_NUM; ++i)
	{
		if( m_apMailSlot[i]->IsCheck() )
		{
			aMultiDel[nDelIdx] = m_apMailSlot[i]->GetMailID();
			nDelIdx++;
		}
	}

	if( nDelIdx <= 0 )
	{
		//CNtlSLEventGenerator::SysMsg( GetDisplayStringManager()->GetString( DST_MAILSYSTEM_MSG_NOTFIND_SELECTED_MAIL), SNtlEventSysStringMsg::ACTION_NOTIFY | SNtlEventSysStringMsg::TYPE_CHAT_NOTICE );
		GetAlarmManager()->AlarmMessage( "DST_MAILSYSTEM_MSG_NOTFIND_SELECTED_MAIL" );
		return;
	}

	CNtlMailSystem* pMailSystem = GetNtlSLGlobal()->GetSobAvatar()->GetMailSystem();
	GetDboGlobal()->GetGamePacketGenerator()->SendMailMultiDelReq(pMailSystem->GetUseObjectSerial(), (RwUInt8)nDelIdx, aMultiDel);
}

void CMailSystemGui::OnToggledBtnAway(gui::CComponent* pComponent, bool bToggled)
{
	if (m_bMailBoxAway)
	{
		CMailSystemMsg::MsgBox(CMailSystemMsg::MAIL_MSG_BOX_AWAY_OFF);
	}
	else
	{
		CMailSystemMsg::MsgBox(CMailSystemMsg::MAIL_MSG_BOX_AWAY_ON);		
	}
}

void CMailSystemGui::OnChangedTabButton(RwInt32 nIndex, RwInt32 nOldIndex)
{
	if ((RwUInt32)nIndex == m_uiIndexTab[TAB_SENDMAIL])
	{
		OpenSendMail();
	}
	else if ((RwUInt32)nIndex == m_uiIndexTab[TAB_MAILBOX])
	{
		OpenMailBox();
	}
	UpdateUserInterface();
}

void CMailSystemGui::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE(DIALOG_MAILSYSTEM, key.m_fX, key.m_fY);
}

void CMailSystemGui::OnKeyUp(gui::CComponent* pComponenet, CInputDevice* pDevice, const CKey& key)
{
	if (key.m_dwVKey == VK_TAB)
	{
		if (pComponenet == m_pIpbTargetName)
		{
			if( m_pIpbMailContent )
				m_pIpbMailContent->SetFocus();
		}
		else if (pComponenet == m_pIpbMailContent)
		{
			if( m_pIpbTargetName )
				m_pIpbTargetName->SetFocus();
		}
	}
}

void CMailSystemGui::OnCharChangedIpbMailContent(int iLength)
{
	// 메일 콘텐츠 하단에 현재 글자수와 최대 글자수를 출력해주고 입력시 현재글자수가
	// 최대글자수를 넘었다면 더이상 입력이 안되게 한다.
	CalcSendMailContentMaxNum();
	if( iLength >= m_pIpbMailContent->GetMaxLength() )
	{
		GetAlarmManager()->AlarmMessage( "DST_MAILSYSTEM_MSG_MAIL_CONTENT_BUFFEROVER" );
	}
}

void CMailSystemGui::OnClickedBtnUpDay(gui::CComponent* pComponent)
{
	if (m_uiSendMailDay < MAILSYSTEM_EXPIRED_DAY_MAX)
	{
		SetSendMailDay(m_uiSendMailDay + 1);
	}
}

void CMailSystemGui::OnClickedBtnDownDay(gui::CComponent* pComponent)
{
	if (m_uiSendMailDay > MAILSYSTEM_EXPIRED_DAY_MIN)
	{
		SetSendMailDay(m_uiSendMailDay - 1);
	}
}

void CMailSystemGui::OnToggledBtnAttachSend(gui::CComponent* pComponent, bool bDown)
{
	SetSendMailZenny(bDown, false);
	GetNtlSLGlobal()->GetAdjustZennyInfo()->SetAdjustZenny( SAdjustZennyInfo::MAIL_BOX, 0 );
}

void CMailSystemGui::OnToggledBtnAttachReq(gui::CComponent* pComponent, bool bDown)
{
	SetSendMailZenny(false, bDown);
	GetNtlSLGlobal()->GetAdjustZennyInfo()->SetAdjustZenny( SAdjustZennyInfo::MAIL_BOX, 0 );
}

/**
* \brief Mail 쓰기에서 제니 버튼을 클릭하였을 때 
*/
void CMailSystemGui::OnClickedBtnAttachZenny(gui::CComponent* pComponent)
{
	RwUInt32 uiSerial;
	
	// 청구하기
	if (m_pBtnAttachReq->IsDown())
	{
		uiSerial = MAILSYSTEM_ATTACH_REQ;

		CRectangle rect = pComponent->GetScreenRect();
		CMailSystemMsg::MsgSay(CMailSystemMsg::MAIL_MSG_SAY_ATTACH_ZENNY, Logic_FormatZeni(NTL_MAX_MAIL_SEND_ZENNY_AMOUNTS));
		CDboEventGenerator::CalcPopupShow(TRUE, uiSerial, PLACE_SUB_MAILSYSTEM_ZENNY, rect.left - dMAILSYSTEM_UI_CALC_OFFSET, rect.top, NTL_MAX_MAIL_SEND_ZENNY_AMOUNTS);
	}

	// 송금하기
	else if (m_pBtnAttachSend->IsDown())
	{
		uiSerial = MAILSYSTEM_ATTACH_SEND;
					
		// Icon에 무엇인가 들고 있다면 PutDown 처리
		if( GetIconMoveManager()->IsActive() )
		{
			GetIconMoveManager()->IconMovePutDown( PLACE_SUB_MAILSYSTEM_ZENNY, INVALID_SERIAL_ID, INVALID_DWORD );
		}
		else
		{
			CRectangle rect = pComponent->GetScreenRect();
			CMailSystemMsg::MsgSay(CMailSystemMsg::MAIL_MSG_SAY_ATTACH_ZENNY, Logic_FormatZeni(NTL_MAX_MAIL_SEND_ZENNY_AMOUNTS));
			CDboEventGenerator::CalcPopupShow(TRUE, uiSerial, PLACE_SUB_MAILSYSTEM_ZENNY, rect.left - dMAILSYSTEM_UI_CALC_OFFSET, rect.top, NTL_MAX_MAIL_SEND_ZENNY_AMOUNTS);
		}
	}
	else
	{
		// 청구하기 또는 송금하기를 선택하지 않았을 경우 선택해달라는 메시지를 출력
		CMailSystemMsg::MsgBox(CMailSystemMsg::MAIL_MSG_BOX_SELECT_REQ_OR_SEND);
	}
}

void CMailSystemGui::OnMoveDlgAttachItem(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rect = m_pDlgAttachItem->GetScreenRect();
	m_AttachItemSlot.SetParentPosition(rect.left, rect.top);
	m_EffectFocus.SetPositionbyParent(rect.left, rect.top);
}

void CMailSystemGui::OnPaintDlgAttachItem()
{
	m_AttachItemSlot.Paint();
	if (m_bMailBoxFocus)
	{
		m_EffectFocus.Render();
	}
}

void CMailSystemGui::OnMoveDlgAttach(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rect = m_pDlgAttach->GetScreenRect();
	m_AttachDisable.SetPositionbyParent(rect.left, rect.top);
	
	CRectangle rectZenny = m_pBtnAttachZenny->GetScreenRect();
	m_EffectZenny.SetPositionbyParent(rectZenny.left, rectZenny.top);
}

void CMailSystemGui::OnPaintPostDlgAttach()
{
	if (!m_pDlgAttach->IsEnabled())
	{
		m_AttachDisable.Render();
	}

	if(m_bEffectZenny)
	{
		m_EffectZenny.Render();
	}
}

void CMailSystemGui::OnMouseEnterDlgAttachItem(gui::CComponent* pComponent)
{
	m_bMailBoxFocus = true;
	ShowInfoWindow(TRUE);
}

void CMailSystemGui::OnMouseLeaveDlgAttachItem(gui::CComponent* pComponent)
{
	m_bMailBoxFocus = false;
	ShowInfoWindow(FALSE);
}

void CMailSystemGui::OnSelectedFriend(INT nSelectIndex)
{
	m_pIpbTargetName->SetText(m_pcbbFriendList->GetSelectedItemText().c_str());
}

void CMailSystemGui::OnClickedBtnSend(gui::CComponent* pComponent)
{
	if (m_uiSendMailZenny <= 0)
	{
		SetSendMailZenny(false, false);
		GetNtlSLGlobal()->GetAdjustZennyInfo()->SetAdjustZenny( SAdjustZennyInfo::MAIL_BOX, 0 );
	}
	
	if (m_pIpbTargetName->GetLength() <= 0)
	{
		CMailSystemMsg::MsgBox(CMailSystemMsg::MAIL_MSG_BOX_PLEASE_TARGETNAME);
		return;
	}
	else if (m_pIpbMailContent->GetLength() <= 0)
	{
		CMailSystemMsg::MsgBox(CMailSystemMsg::MAIL_MSG_BOX_PLEASE_MAILCONTENT);
		return;
	}
	else if (GetChattingFilter()->IsSlang(m_pIpbMailContent->GetText()))
	{
		CMailSystemMsg::MsgBox(CMailSystemMsg::MAIL_MSG_BOX_MAILCONTENT_SLANGDER);
		return;
	}

	switch (GetSendMailType())
	{
	case eMAIL_TYPE_BASIC:
		CMailSystemMsg::MsgBox(CMailSystemMsg::MAIL_MSG_BOX_SEND_BASIC, Logic_FormatZeni(m_eSendMailStamp));
		break;
	case eMAIL_TYPE_ITEM:
		if (m_AttachItemSlot.GetSerialType() == REGULAR_SLOT_ITEM_SOB)
		{
			CMailSystemMsg::MsgBox(CMailSystemMsg::MAIL_MSG_BOX_SEND_ITEM, Logic_FormatZeni(m_eSendMailStamp), CMailSystemMsg::GetItemName(m_AttachItemSlot.GetItemTable()->tblidx));
		}
		else
		{
			CMailSystemMsg::MsgBox(CMailSystemMsg::MAIL_MSG_BOX_SEND_ITEM, Logic_FormatZeni(m_eSendMailStamp), GetDisplayStringManager()->GetString("DST_MAILSYSTEM_NOT_IDENTIFICATION_ITEM"));
		}
		break;
	case eMAIL_TYPE_ZENNY:
		CMailSystemMsg::MsgBox(CMailSystemMsg::MAIL_MSG_BOX_SEND_ZENNY, Logic_FormatZeni(m_eSendMailStamp), Logic_FormatZeni(m_uiSendMailZenny));
		break;
	case eMAIL_TYPE_ITEM_ZENNY:
		if (m_AttachItemSlot.GetSerialType() == REGULAR_SLOT_ITEM_SOB)
		{
			CMailSystemMsg::MsgBox(CMailSystemMsg::MAIL_MSG_BOX_SEND_ITEM_ZENNY, Logic_FormatZeni(m_eSendMailStamp), CMailSystemMsg::GetItemName(m_AttachItemSlot.GetItemTable()->tblidx), Logic_FormatZeni(m_uiSendMailZenny));
		}
		else
		{
			CMailSystemMsg::MsgBox(CMailSystemMsg::MAIL_MSG_BOX_SEND_ITEM_ZENNY, Logic_FormatZeni(m_eSendMailStamp), GetDisplayStringManager()->GetString("DST_MAILSYSTEM_NOT_IDENTIFICATION_ITEM"), Logic_FormatZeni(m_uiSendMailZenny));
		}
		break;
	case eMAIL_TYPE_ITEM_ZENNY_REQ:
		if (m_AttachItemSlot.GetSerialType() == REGULAR_SLOT_ITEM_SOB)
		{
			CMailSystemMsg::MsgBox(CMailSystemMsg::MAIL_MSG_BOX_SEND_ITEM_ZENNY_REQ, Logic_FormatZeni(m_eSendMailStamp), CMailSystemMsg::GetItemName(m_AttachItemSlot.GetItemTable()->tblidx), Logic_FormatZeni(m_uiSendMailZenny));
		}
		else
		{
			CMailSystemMsg::MsgBox(CMailSystemMsg::MAIL_MSG_BOX_SEND_ITEM_ZENNY_REQ, Logic_FormatZeni(m_eSendMailStamp), GetDisplayStringManager()->GetString("DST_MAILSYSTEM_NOT_IDENTIFICATION_ITEM"), Logic_FormatZeni(m_uiSendMailZenny));
		}
		break;
	case eMAIL_TYPE_ZENNY_REQ:
		CMailSystemMsg::MsgBox(CMailSystemMsg::MAIL_MSG_BOX_SEND_ZENNY_REQ, Logic_FormatZeni(m_eSendMailStamp), Logic_FormatZeni(m_uiSendMailZenny));
		break;
	}
}

void CMailSystemGui::SendMail()
{
	CNtlMailSystem* pMailSystem = GetNtlSLGlobal()->GetSobAvatar()->GetMailSystem();
	GetDboGlobal()->GetGamePacketGenerator()->SendMailSendReq(pMailSystem->GetUseObjectSerial(),
															  m_pIpbTargetName->GetText(),
															  m_pIpbMailContent->GetText(),
															  GetSendMailType(),
															  &m_sSendMailAttachItem,
															  m_uiSendMailZenny,
															  (RwUInt8)m_uiSendMailDay);
}

void CMailSystemGui::CalcSendMailStamp()
{
	bool	bAttachSend		= m_pBtnAttachSend->IsDown();
	bool	bAttachReq		= m_pBtnAttachReq->IsDown();
	bool	bAttachItem		= (m_sSendMailAttachItem.hItem != INVALID_SERIAL_ID);
	bool	bAttachZenny	= (m_uiSendMailZenny > 0);

	if (bAttachItem || ((bAttachSend || bAttachReq) && bAttachZenny))
	{
		SetSendMailStamp(MAILSYSTEM_STAMP_ATTACH);
	}
	else
	{
		SetSendMailStamp(MAILSYSTEM_STAMP_BASIC);
	}
}

void CMailSystemGui::CalcSendMailContentMaxNum()
{
	m_pStbMaxNumText->Format("(%u/%u)", m_pIpbMailContent->GetLength(),m_pIpbMailContent->GetMaxLength());
}

void CMailSystemGui::SetSendMailStamp(eMAILSYSTEM_STAMP eStamp)
{
	m_eSendMailStamp = eStamp;
	//m_pStbStamp->SetText((int)eStamp);
}

RwUInt8 CMailSystemGui::GetSendMailType()
{
	RwUInt8 uiMailType		= eMAIL_TYPE_BASIC;
	bool	bAttachSend		= m_pBtnAttachSend->IsDown();
	bool	bAttachReq		= m_pBtnAttachReq->IsDown();
	bool	bAttachItem		= (m_sSendMailAttachItem.hItem != INVALID_SERIAL_ID);
	bool	bAttachZenny	= (m_uiSendMailZenny > 0);

	if (bAttachZenny)
	{
		DBO_ASSERT(bAttachSend != bAttachReq, "SEND or REQ : SELECT FAILED");
	}

	if (bAttachItem && bAttachZenny)
	{
		if (bAttachSend)
		{
			uiMailType = eMAIL_TYPE_ITEM_ZENNY;
		}
		else if (bAttachReq)
		{
			uiMailType = eMAIL_TYPE_ITEM_ZENNY_REQ;
		}
	}
	else if (bAttachZenny)
	{
		if (bAttachSend)
		{
			uiMailType = eMAIL_TYPE_ZENNY;
		}
		else if (bAttachReq)
		{
			uiMailType = eMAIL_TYPE_ZENNY_REQ;
		}
	}
	else if (bAttachItem)
	{
		uiMailType = eMAIL_TYPE_ITEM;
	}
	
	return uiMailType;
}

void CMailSystemGui::OnClickedBtnClear(gui::CComponent* pComponent)
{
	ClearSendMail();
}

void CMailSystemGui::OnMouseDownDlgAttachItem(const CKey& key)
{
	if(!GetIconMoveManager()->IsActive())
	{
		return;
	}

	m_pDlgAttachItem->CaptureMouse();
}

void CMailSystemGui::OnMouseUpDlgAttachItem(const CKey& key)
{
	m_pDlgAttachItem->ReleaseMouse();
	if (!m_AttachItemSlot.PtInRect((RwInt32)key.m_fX, (RwInt32)key.m_fX))
	{
		return;
	}
	
	switch (key.m_nID)
	{
	case UD_LEFT_BUTTON:
		{
			if(!GetIconMoveManager()->IsActive() && GetIconMoveManager()->GetSrcPlace() == PLACE_BAG)
			{
				return;
			}

			// 가방이 아니면 Attach 되지 않도록 수정한다.
			if( GetIconMoveManager()->IsActive() && GetIconMoveManager()->GetSrcPlace() != PLACE_BAG )
			{
				return;
			}

			if (m_AttachItemSlot.GetSerial() == INVALID_SERIAL_ID && GetIconMoveManager()->GetSrcSerial() != INVALID_SERIAL_ID)
			{
				CNtlSobItem*		pSobItem	= reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(GetIconMoveManager()->GetSrcSerial()));
				CNtlSobItemAttr*	pSobItemAttr= reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());

				if (pSobItemAttr->GetStackNum() == GetIconMoveManager()->GetStackCount())
				{
					SetSendMailItem(GetIconMoveManager()->GetSrcSerial(), 
						(RwUInt8)Logic_WhichBagHasItem(GetIconMoveManager()->GetSrcSerial()),
						(RwUInt8)GetIconMoveManager()->GetSrcSlotIdx(),
						(RwUInt8)GetIconMoveManager()->GetStackCount());
				}
			}
			break;
		}
	case UD_RIGHT_BUTTON:
		{
			SetSendMailItem(INVALID_SERIAL_ID);
			ShowInfoWindow(FALSE);
			break;
		}
	}
}

void CMailSystemGui::OnGotFocus()
{
	GetDboGlobal()->GetGameData()->bChatMode = TRUE;
}

void CMailSystemGui::OnLostFocus()
{
	GetDboGlobal()->GetGameData()->bChatMode = FALSE;
}

void CMailSystemGui::OpenUserInterface(RwUInt32 uiAbilityType)
{
	m_uiAbilityType = uiAbilityType;
	if (uiAbilityType == CNtlMailSystem::MAILSYSTEM_ABILITY_NONE)
	{
		return;
	}

	if (uiAbilityType & CNtlMailSystem::MAILSYSTEM_ABILITY_READ)
	{
		m_uiIndexTab[TAB_MAILBOX] = m_pTabButton->AddTab(std::string());
		m_pTabButton->ChangeTabText(m_uiIndexTab[TAB_MAILBOX], std::wstring(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_TAB_MAILBOX")));
	}

	if (uiAbilityType & CNtlMailSystem::MAILSYSTEM_ABILITY_SEND)
	{
		m_uiIndexTab[TAB_SENDMAIL] = m_pTabButton->AddTab(std::string());
		m_pTabButton->ChangeTabText(m_uiIndexTab[TAB_SENDMAIL], std::wstring(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_TAB_SENDMAIL")));
	}

	if (m_uiIndexTab[TAB_MAILBOX] == 0)
	{
		OpenMailBox();
	}
	else if (m_uiIndexTab[TAB_SENDMAIL] == 0)
	{
		OpenSendMail();
	}
}

void CMailSystemGui::ClearUserInterface()
{
	m_pTabButton->ClearTab();
	for (int i = 0; i < TAB_NUM; ++i)
	{
		m_uiIndexTab[i] = TAB_NUM;
	}
	m_uiAbilityType = CNtlMailSystem::MAILSYSTEM_ABILITY_NONE;
	ClearSendMail();
	ClearMailBox();
}

void CMailSystemGui::UpdateUserInterface()
{
	if ((RwUInt32)m_pTabButton->GetSelectedButtonIndex() == m_uiIndexTab[TAB_MAILBOX])
	{
		UpdateMailBox();
	}
	else if ((RwUInt32)m_pTabButton->GetSelectedButtonIndex() == m_uiIndexTab[TAB_SENDMAIL])
	{
		UpdateSendMail();
	}
}

void CMailSystemGui::SetMailBoxPage(RwUInt32 uiPage)
{
	RwUInt32 uiMailCount = GetNtlSLGlobal()->GetSobAvatar()->GetMailSystem()->GetMailCount();
	if(  uiMailCount == 0 )
	{
		m_pStbEmptyMail->Show(true);
		m_pPanEmptyMail->Show(true);
		m_uiMailBoxPageMax = 1;
	}
	else if( uiMailCount > SLOT_NUM )
	{
		m_pStbEmptyMail->Show(false);
		m_pPanEmptyMail->Show(false);
		
		if( (uiMailCount % SLOT_NUM) == 0 )
			m_uiMailBoxPageMax = (uiMailCount / SLOT_NUM);
		else
			m_uiMailBoxPageMax = (uiMailCount / SLOT_NUM) + 1;
	}
	else
	{
		m_pStbEmptyMail->Show(false);
		m_pPanEmptyMail->Show(false);
		m_uiMailBoxPageMax = 1;
	}
	
	if (uiPage >= m_uiMailBoxPageMax)
	{
		m_uiMailBoxPageCur = m_uiMailBoxPageMax - 1;
	}
	else if (uiPage <= 0)
	{
		m_uiMailBoxPageCur = 0;
	}
	else
	{
		m_uiMailBoxPageCur = uiPage;
	}

	RwUInt32 uiTopMail = m_uiMailBoxPageCur * SLOT_NUM;
	for (int i = 0; i < SLOT_NUM; ++i)
	{
		m_apMailSlot[i]->SetMailSlot(uiTopMail + i, m_mailIDReading);
		m_apMailSlot[i]->CheckClear();
	}
	
	m_pStbPage->Format(L"%u / %u", m_uiMailBoxPageCur + 1, m_uiMailBoxPageMax);

	if( m_uiMailBoxPageCur + 1 >= m_uiMailBoxPageMax )
		m_pBtnNextPage->ClickEnable( FALSE );
	else
		m_pBtnNextPage->ClickEnable( TRUE );

	if( m_uiMailBoxPageCur <= 0 )
		m_pBtnPrevPage->ClickEnable( FALSE );
	else
		m_pBtnPrevPage->ClickEnable( TRUE );

}

void CMailSystemGui::SetSendMailZenny(bool bIsAttachSend, bool bIsAttachReq, RwUInt32 uiZenny)
{
	m_pBtnAttachSend->SetDown(bIsAttachSend);
	m_pBtnAttachReq->SetDown(bIsAttachReq);

	if( GetIconMoveManager()->GetSrcPlace() == PLACE_SUB_BAG_ZENNY && m_pBtnAttachSend->IsDown() )
		m_bEffectZenny = TRUE;
	else
		m_bEffectZenny = FALSE;

	m_uiSendMailZenny = uiZenny;
	
	// 둘다 같은 경우는 둘 다 False 일 경우
	if (bIsAttachSend == bIsAttachReq)
	{
		m_pStbAttachZenny->Clear();
		m_pBtnAttachZenny->ClickEnable( FALSE );
	}
	else
	{
		m_pBtnAttachZenny->ClickEnable( TRUE );
		m_pStbAttachZenny->Format(L"%s", Logic_FormatZeni(uiZenny));
	}

	CalcSendMailStamp();
}

void CMailSystemGui::SetSendMailItem(SERIAL_HANDLE hItem, RwUInt8 uiPlace, RwUInt8 uiPos, RwUInt8 uiStackCount)
{	
	if (hItem == INVALID_SERIAL_ID)
	{
		if (m_AttachItemSlot.GetSerial() != INVALID_SERIAL_ID)
		{
			CDboEventGenerator::DialogEvent(DIALOGEVENT_END_TRADING_ITEM_IN_BAG, PLACE_SUB_MAILSYSTEM_ITEM, PLACE_BAG, 
											Logic_ConvertContainerTypeToBagIdx(m_sSendMailAttachItem.byPlace),
											m_sSendMailAttachItem.byPos, m_sSendMailAttachItem.hItem);
		}	

		char acBuffer[1024];
		::WideCharToMultiByte(GetACP(), 0, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_TOOLTIP_NOT_ATTACH"), -1, acBuffer, _countof(acBuffer) - 1, NULL, NULL);
		m_pDlgAttachItem->SetToolTip(acBuffer);
		m_AttachItemSlot.Clear();
	}
	else
	{		
		eREGULAR_SLOT_TYPE eSerialType;
		
		// peessi : Logic changes due to the addition of expired attributes
		CNtlSobItemAttr* pSobItemAttr = Logic_GetItemSobAttrFromSobSerial( hItem );
		sITEM_TBLDAT* pItemTableData = NULL;
		
		if( pSobItemAttr )
			pItemTableData = pSobItemAttr->GetItemTbl();

		if (pItemTableData)
		{
			if (!(Logic_IsCanSellItem(pSobItemAttr) && Logic_IsCanUserTradeItem(pSobItemAttr) &&
				Logic_IsCanSaveWarehouse(pSobItemAttr) && Logic_IsCanSaveCommonWarehouse(pSobItemAttr)))
			{
				CMailSystemMsg::MsgSay(CMailSystemMsg::MAIL_MSG_SAY_ITEM_NOT_ATTACH);
				return;
			}

			eSerialType = REGULAR_SLOT_ITEM_SOB;
		}
		else
		{
			eSerialType = REGULAR_SLOT_ITEM_SOB_NOT_IDENTIFICATION;
		}
		if (eSerialType != m_AttachItemSlot.GetSerialType())
		{
			m_AttachItemSlot.Clear();
			m_AttachItemSlot.SetSerialType(eSerialType);
		}

		CDboEventGenerator::DialogEvent(DIALOGEVENT_BEGIN_TRADING_ITEM_IN_BAG, PLACE_SUB_MAILSYSTEM_ITEM, PLACE_BAG, 
			Logic_ConvertContainerTypeToBagIdx(uiPlace), uiPos, hItem);

		
		m_pDlgAttachItem->DeleteToolTip();
		m_AttachItemSlot.SetIcon(hItem, uiStackCount);
		GetIconMoveManager()->IconMoveEnd();
	}

	m_sSendMailAttachItem.hItem		= hItem;
	m_sSendMailAttachItem.byPlace	= uiPlace;
	m_sSendMailAttachItem.byPos		= uiPos;

	CalcSendMailStamp();
}

void CMailSystemGui::SetSendMailDay(RwUInt32 uiDay)
{
	if (uiDay >= MAILSYSTEM_EXPIRED_DAY_MAX)
	{
		m_uiSendMailDay = MAILSYSTEM_EXPIRED_DAY_MAX;
	}
	else if (uiDay <= MAILSYSTEM_EXPIRED_DAY_MIN)
	{
		m_uiSendMailDay = MAILSYSTEM_EXPIRED_DAY_MIN;
	}
	else
	{
		m_uiSendMailDay = uiDay;
	}
	
	m_pStbDay->Format(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAIL_EXPIRED_DAY"), m_uiSendMailDay);

	if( m_uiSendMailDay <= MAILSYSTEM_EXPIRED_DAY_MIN )
	{
		m_pBtnUpDay->ClickEnable( TRUE );
		m_pBtnDownDay->ClickEnable( FALSE );
	}
	else if( m_uiSendMailDay >= MAILSYSTEM_EXPIRED_DAY_MAX )
	{
		m_pBtnUpDay->ClickEnable( FALSE );
		m_pBtnDownDay->ClickEnable( TRUE );
	}
	else
	{
		m_pBtnUpDay->ClickEnable( TRUE );
		m_pBtnDownDay->ClickEnable( TRUE );
	}
}

void CMailSystemGui::SetMailBoxAway(bool bAway)
{
	m_pBtnAway->SetDown(bAway);
	m_bMailBoxAway = bAway;

	m_pBtnAway->GetSurfaceUp()->clear();
	m_pBtnAway->GetSurfaceDown()->clear();
	m_pBtnAway->GetSurfaceFocus()->clear();
	m_pBtnAway->GetSurfaceDisable();
	
	
	char acBuffer[1024];
	gui::CSurface surface;
	if (bAway)
	{
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MailSystem.srf", "srfCheckBoxOn");
		::WideCharToMultiByte(GetACP(), 0, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAIL_AWAY_ON"), -1, acBuffer, _countof(acBuffer) - 1, NULL, NULL);		
	}
	else
	{
		surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MailSystem.srf", "srfCheckBoxOff");
		::WideCharToMultiByte(GetACP(), 0, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAIL_AWAY_OFF"), -1, acBuffer, _countof(acBuffer) - 1, NULL, NULL);
	}
	
	m_pBtnAway->AddSurfaceUp(surface);
	m_pBtnAway->AddSurfaceDown(surface);
	m_pBtnAway->AddSurfaceFocus(surface);
	m_pBtnAway->AddSurfaceDisabled(surface);
	m_pBtnAway->SetToolTip(acBuffer);
}

void CMailSystemGui::SelectTab(RwUInt32 uiTabIndex)
{
	if ((RwUInt32)m_pTabButton->GetButtonCount() <= uiTabIndex)
	{
		return;
	}

	for (int i = 0; i < TAB_NUM; ++i)
	{
		m_apDlgTab[i]->Show(uiTabIndex == m_uiIndexTab[i]);
	}

	m_pBtnReload->Show(uiTabIndex == m_uiIndexTab[TAB_MAILBOX]);
	m_pBtnDelete->Show(uiTabIndex == m_uiIndexTab[TAB_MAILBOX]);
	m_pTabButton->SelectTab(uiTabIndex);

	UpdateUserInterface();
}

void CMailSystemGui::OpenSendMail()
{
	m_pIpbTargetName->SetFocus();
	SelectTab(m_uiIndexTab[TAB_SENDMAIL]);
}

void CMailSystemGui::OpenMailBox()
{
	SetMailBoxPage(m_uiMailBoxPageCur);
	SetMailBoxAway(GetNtlSLGlobal()->GetSobAvatar()->GetMailSystem()->IsAway() != 0);
	SelectTab(m_uiIndexTab[TAB_MAILBOX]);
}

void CMailSystemGui::ClearSendMail()
{
	SetSendMailTargetName(NULL);
	m_pIpbTargetName->Clear();
	m_pIpbMailContent->Clear();

	m_pcbbFriendList->SelectItem(0, false); // unselect friend

	SetSendMailDay(MAILSYSTEM_EXPIRED_DAY_MAX);
	SetSendMailItem(INVALID_SERIAL_ID);
	SetSendMailZenny(false, false);
	GetNtlSLGlobal()->GetAdjustZennyInfo()->SetAdjustZenny( SAdjustZennyInfo::MAIL_BOX, 0 );
	CalcSendMailStamp();
	CalcSendMailContentMaxNum();
	m_pBtnDelete->ClickEnable( FALSE );
}

void CMailSystemGui::ClearMailBox()
{
	for (int i = 0; i < SLOT_NUM; ++i)
	{
		m_apMailSlot[i]->ClearMailSlot();
	}
	m_uiMailBoxPageCur = 0;
	m_uiMailBoxPageMax = 0;
}

void CMailSystemGui::UpdateSendMail()
{
	// by daneos: fix unable to attach items when reload mail
//	m_uiAbilityType = GetNtlSLGlobal()->GetSobAvatar()->GetMailSystem()->GetAbility();
//	m_pDlgAttach->Enable((m_uiAbilityType & CNtlMailSystem::MAILSYSTEM_ABILITY_ATTACH_SEND) != 0);
//	m_pStbAttachMessage->Show((m_uiAbilityType & CNtlMailSystem::MAILSYSTEM_ABILITY_ATTACH_SEND) == 0);

	m_pDlgAttach->Enable();
	m_pStbAttachMessage->Show(false);
}

void CMailSystemGui::UpdateMailBox()
{
	SetMailBoxPage(m_uiMailBoxPageCur);
}

void CMailSystemGui::ShowInfoWindow(RwBool bIsShow)
{
	if (bIsShow && m_AttachItemSlot.GetSerial() != INVALID_SERIAL_ID)
	{
		CRectangle rect = m_pDlgAttachItem->GetScreenRect();
		if (Logic_GetItemDataFromSob(m_AttachItemSlot.GetSerial()))
		{
			GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_ITEM, rect.left, rect.top, m_AttachItemSlot.GetSobItem(), DIALOG_MAILSYSTEM);
		}
		else
		{
			GetInfoWndManager()->ShowInfoWindow( TRUE, CInfoWndManager::INFOWND_UNIDENTIFIED_ITEM, rect.left, rect.top, NULL, DIALOG_MAILSYSTEM);
		}
	}
	else
	{
		if( DIALOG_MAILSYSTEM == GetInfoWndManager()->GetRequestGui() )
			GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
}


//////////////////////////////////////////////////////////////////////////
// CMailSystemReadGui
//////////////////////////////////////////////////////////////////////////
CMailSystemReadGui::CMailSystemReadGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_pStbTitle(NULL)
,m_pBtnClose(NULL)
,m_pTabButton(NULL)
,m_pStbTargetNameTitle(NULL)
,m_pIpbTargetName(NULL)
,m_pStbDay(NULL)
,m_pStbMailContentTitle(NULL)
,m_pStbAttachTitle(NULL)
,m_pStbAttachZenny(NULL)
,m_pPnlIconZennySend(NULL)
,m_pPnlIconZennyReq(NULL)
,m_pDlgAttachItem(NULL)
,m_pStbAttachCategory(NULL)
,m_pBtnAttachAction(NULL)
,m_pBtnReply(NULL)
,m_pBtnReturn(NULL)
,m_pBtnDelete(NULL)
,m_pDlgAttach(NULL)
,m_pIpbMailContent(NULL)
,m_pDlgLock(NULL)
,m_pStbSendDate(NULL)
,m_mailIDTemp(INVALID_MAILID)
,m_pPnlIconDay(NULL)
,m_pStbAttachMessage(NULL)
,m_uiAbilityType(0)
{
}

CMailSystemReadGui::~CMailSystemReadGui()
{
}

RwBool CMailSystemReadGui::Create()
{
	NTL_FUNCTION("CMailSystemReadGui::Create");

	if (!CNtlPLGui::Create("", "gui\\MailSystemRead.srf", "gui\\MailSystemRead.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis					= (gui::CDialog*)GetComponent("dlgMain");
	m_pStbTitle				= (gui::CStaticBox*)GetComponent("stbTitle");
	m_pBtnClose				= (gui::CButton*)GetComponent("btnClose");
	m_pTabButton			= (gui::CTabButton*)GetComponent("tabButton");
	m_pStbTargetNameTitle	= (gui::CStaticBox*)GetComponent("stbTargetNameTitle");
	m_pIpbTargetName		= (gui::CInputBox*)GetComponent("ipbTargetName");
	m_pStbDay				= (gui::CStaticBox*)GetComponent("stbDay");
	m_pStbMailContentTitle	= (gui::CStaticBox*)GetComponent("stbMailContentTitle");
	m_pStbAttachTitle		= (gui::CStaticBox*)GetComponent("stbAttachTitle");
	m_pStbAttachZenny		= (gui::CStaticBox*)GetComponent("stbAttachZenny");
	m_pPnlIconZennySend		= (gui::CPanel*)GetComponent("pnlIconZennySend");
	m_pPnlIconZennyReq		= (gui::CPanel*)GetComponent("pnlIconZennyReq");
	m_pDlgAttachItem		= (gui::CDialog*)GetComponent("dlgAttachItem");
	m_pStbAttachCategory	= (gui::CStaticBox*)GetComponent("stbAttachCategory");
	m_pBtnAttachAction		= (gui::CButton*)GetComponent("btnAttachAction");
	m_pDlgAttach			= (gui::CDialog*)GetComponent("dlgAttach");
	m_pBtnReply				= (gui::CButton*)GetComponent("btnReply");
	m_pBtnReturn			= (gui::CButton*)GetComponent("btnReturn");
	m_pBtnDelete			= (gui::CButton*)GetComponent("btnDelete");
	m_pIpbMailContent		= (gui::CInputBox*)GetComponent("ipbMailContent");
	m_pDlgLock				= (gui::CDialog*)GetComponent("dlgLock");
	m_pStbSendDate			= (gui::CStaticBox*)GetComponent("stbSendDate");
	m_pPnlIconDay			= (gui::CPanel*)GetComponent("pnlIconDay");
	m_pStbAttachMessage		= (gui::CStaticBox*)GetComponent("stbAttachMessage");

	m_AttachItemSlot.Create(m_pDlgAttachItem, DIALOG_MAILSYSTEM, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT);
	m_AttachItemSlot.SetPosition_fromParent(0, 0);

	m_AttachItemAccept.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSlotGrayedEffect"));
	m_AttachItemAccept.SetPositionfromParent(0, 0);

	m_AttachDisable.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MailSystemRead.srf", "srfAttachDisable"));
	m_AttachDisable.SetPositionfromParent(9, -1);

	m_feAttachAction.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MailSystemRead.srf", "srfBtnAttachActionEffect"));
	m_feAttachAction.SetTime(1000.0f, 0.5f);
	m_feAttachAction.SetAlpha(0, 255);

	m_slotClickedBtnClose			= m_pBtnClose->SigClicked().Connect(this, &CMailSystemReadGui::OnClickedBtnClose);
	m_slotMoveDlgAttachItem			= m_pDlgAttachItem->SigMove().Connect(this, &CMailSystemReadGui::OnMoveDlgAttachItem);
	m_slotPaintDlgAttachItem		= m_pDlgAttachItem->SigPaint().Connect(this, &CMailSystemReadGui::OnPaintDlgAttachItem);
	m_SlotMouseEnterDlgAttachItem	= m_pDlgAttachItem->SigMouseEnter().Connect(this, &CMailSystemReadGui::OnMouseEnterDlgAttachItem);
	m_SlotMouseLeaveDlgAttachItem	= m_pDlgAttachItem->SigMouseLeave().Connect(this, &CMailSystemReadGui::OnMouseLeaveDlgAttachItem);
	m_slotClickedBtnReply			= m_pBtnReply->SigClicked().Connect(this, &CMailSystemReadGui::OnClickedBtnReply);
	m_slotClickedBtnReturn			= m_pBtnReturn->SigClicked().Connect(this, &CMailSystemReadGui::OnClickedBtnReturn);
	m_slotClickedBtnDelete			= m_pBtnDelete->SigClicked().Connect(this, &CMailSystemReadGui::OnClickedBtnDelete);
	m_slotClickedBtnAttachAction	= m_pBtnAttachAction->SigClicked().Connect(this, &CMailSystemReadGui::OnClickedBtnAttachAction);
	m_slotMoveDlgAttach				= m_pDlgAttach->SigMove().Connect(this, &CMailSystemReadGui::OnMoveDlgAttach);
	m_slotPaintPostDlgAttach		= m_pDlgAttachItem->SigPaint().Connect(this, &CMailSystemReadGui::OnPaintPostDlgAttach);
	m_slotMoveBtnAttachAction		= m_pBtnAttachAction->SigMove().Connect(this, &CMailSystemReadGui::OnMoveBtnAttachAction);
	m_slotPaintBtnAttachAction		= m_pBtnAttachAction->SigPaint().Connect(this, &CMailSystemReadGui::OnPaintBtnAttachAction);
	m_slotCaptureMouseDown				= GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect(this, &CMailSystemReadGui::OnCaptureMouseDown);

	m_pIpbMailContent->SetMultilineMode(TRUE);
	m_pIpbMailContent->SetMaxLength(NTL_MAX_LENGTH_OF_MAIL_MESSAGE);

	m_pStbTitle->SetText(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAILBOX"));
	m_pStbTitle->SetPosition(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y);
	m_pStbTargetNameTitle->SetText(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAIL_FROM"));
	m_pStbMailContentTitle->SetText(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAIL_CONTENT"));
	m_pStbAttachTitle->SetText(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAIL_ATTACH_2"));
	m_pBtnReply->SetText(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAIL_REPLY"));
	m_pBtnReturn->SetText(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAIL_RETURN"));
	m_pBtnDelete->SetText(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAIL_DELETE"));
	m_pStbAttachMessage->SetText(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_ATTACH_DISABLED_MSG"));
	
	char acBuffer[1024];
	::WideCharToMultiByte(GetACP(), 0, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_TOOLTIP_EXPIREDDAY"), -1, acBuffer, 256, NULL, NULL);
	m_pPnlIconDay->SetToolTip(acBuffer);
	::WideCharToMultiByte(GetACP(), 0, GetDisplayStringManager()->GetString("DST_MAILSYSTEM_TOOLTIP_ATTACHACTION"), -1, acBuffer, 256, NULL, NULL);
	m_pBtnAttachAction->SetToolTip(acBuffer);
	
	RwUInt32 uiTabIndex = m_pTabButton->AddTab(std::string());
	m_pTabButton->ChangeTabText(uiTabIndex, std::wstring(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_TAB_READMAIL")));
	m_pTabButton->SelectTab(uiTabIndex);


	CRectangle rtScreen = m_pDlgAttach->GetScreenRect();
	OnMoveDlgAttach(rtScreen.left, rtScreen.top);
	rtScreen = m_pDlgAttachItem->GetScreenRect();
	OnMoveDlgAttachItem(rtScreen.left, rtScreen.top);
	rtScreen = m_pBtnAttachAction->GetScreenRect();
	OnMoveBtnAttachAction(rtScreen.left, rtScreen.top);

	LinkMsg(g_EventSobInfoUpdate, 0);
	LinkMsg(g_EventMsgBoxResult, 0);
	LinkMsg(g_EventMailRead, 0);
	LinkMsg(g_EventMailDel, 0);
	LinkMsg(g_EventMailMultiDelRes, 0 );
	LinkMsg(g_EventMailLock, 0);
	LinkMsg(g_EventMailItemReceive, 0);
	LinkMsg(g_EventMailReturn, 0);
	LinkMsg(g_EventMailCloseNfy, 0);

	GetNtlGuiManager()->AddUpdateFunc(this);

	ClearReadMail();
	Show(false);
	NTL_RETURN(TRUE);
}

void CMailSystemReadGui::Destroy()
{
	NTL_FUNCTION("CMailSystemReadGui::Destroy");

	m_AttachItemSlot.Destroy();
	
	UnLinkMsg(g_EventSobInfoUpdate);
	UnLinkMsg(g_EventMsgBoxResult);
	UnLinkMsg(g_EventMailRead);
	UnLinkMsg(g_EventMailDel);
	UnLinkMsg(g_EventMailMultiDelRes);
	UnLinkMsg(g_EventMailLock);
	UnLinkMsg(g_EventMailItemReceive);
	UnLinkMsg(g_EventMailReturn);
	UnLinkMsg(g_EventMailCloseNfy);

	GetNtlGuiManager()->RemoveUpdateFunc(this);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

RwInt32 CMailSystemReadGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);
	if (bOpen)
	{
	}
	else
	{
		ClearReadMail();
		CDboEventGenerator::ReadingMail(m_sMailProfile.mailID, false);
	}

	return TRUE;
}

void CMailSystemReadGui::HandleEvents(RWS::CMsg& msg)
{
	NTL_FUNCTION("CMailSystemReadGui::HandleEvents");

	if (msg.Id == g_EventSobInfoUpdate)
	{
		SNtlEventSobInfoUpdate* pEvent = reinterpret_cast<SNtlEventSobInfoUpdate*>(msg.pData);
		if(GetDialogManager()->IsOpenDialog(DIALOG_MAILSYSTEM_READ) && Logic_IsUpdateType(EVENT_AIUT_ITEM, msg.pData))
		{
			if (pEvent->hSerialId == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID())
			{
				if (m_uiAbilityType != GetNtlSLGlobal()->GetSobAvatar()->GetMailSystem()->GetAbility())
				{
					GetDialogManager()->CloseDialog(DIALOG_MAILSYSTEM_READ);
				}
			}
		}
	}
	else if (msg.Id == g_EventMsgBoxResult)
	{
		SDboEventMsgBoxResult*	pEvent		= reinterpret_cast<SDboEventMsgBoxResult*>(msg.pData);
		CNtlMailSystem*			pMailSystem	= GetNtlSLGlobal()->GetSobAvatar()->GetMailSystem();

		if (pEvent->eResult == MBR_OK)
		{
			if (pEvent->strID == "DST_MAILSYSTEM_MSG_DELETE")
			{
				GetDboGlobal()->GetGamePacketGenerator()->SendMailDelReq(pMailSystem->GetUseObjectSerial(), m_mailIDTemp);
			}
			else if (pEvent->strID == "DST_MAILSYSTEM_MSG_RETURN")
			{
				GetDboGlobal()->GetGamePacketGenerator()->SendMailReturnReq(pMailSystem->GetUseObjectSerial(), m_mailIDTemp);
			}
			else if (pEvent->strID == "DST_MAILSYSTEM_MSG_RECEIVE_ITEM_ACCEPT"
				|| pEvent->strID == "DST_MAILSYSTEM_MSG_RECEIVE_ITEM_ZENNY_ACCEPT"
				|| pEvent->strID == "DST_MAILSYSTEM_MSG_RECEIVE_ITEM_ZENNY_REQ_ACCEPT"
				|| pEvent->strID == "DST_MAILSYSTEM_MSG_RECEIVE_ZENNY_ACCEPT"
				|| pEvent->strID == "DST_MAILSYSTEM_MSG_RECEIVE_ZENNY_REQ_ACCEPT")
			{
				GetDboGlobal()->GetGamePacketGenerator()->SendMailItemReceiveReq(pMailSystem->GetUseObjectSerial(), m_mailIDTemp);
			}
		}
		
		m_mailIDTemp = INVALID_MAILID;
	}
	else if (msg.Id == g_EventMailRead)
	{
		SNtlEventMailRead* pEvent = reinterpret_cast<SNtlEventMailRead*>(msg.pData);
		if (pEvent->mailID != m_sMailProfile.mailID)
		{
			if (GetDialogManager()->IsOpenDialog(DIALOG_MAILSYSTEM_READ))
			{
				ClearReadMail();
			}
			OpenReadMail(pEvent->mailID);
			GetDialogManager()->OpenDialog(DIALOG_MAILSYSTEM_READ);
		}
	}
	else if (msg.Id == g_EventMailItemReceive)
	{
		SNtlEventMailItemReceive* pEvent = reinterpret_cast<SNtlEventMailItemReceive*>(msg.pData);
		if (m_sMailProfile.mailID == pEvent->mailID)
		{
			OpenReadMail(pEvent->mailID);
			switch (m_sMailProfile.byMailType)
			{
			case eMAIL_TYPE_BASIC:
				break;
			case eMAIL_TYPE_ITEM:
				CMailSystemMsg::MsgSay(CMailSystemMsg::MAIL_MSG_SAY_RECEIVE_ITEM, CMailSystemMsg::GetItemName(&m_sMailProfile, TRUE));
				break;
			case eMAIL_TYPE_ZENNY:
				CMailSystemMsg::MsgSay(CMailSystemMsg::MAIL_MSG_SAY_RECEIVE_ZENNY, Logic_FormatZeni(m_sMailProfile.dwZenny));
				break;
			case eMAIL_TYPE_ITEM_ZENNY:
				CMailSystemMsg::MsgSay(CMailSystemMsg::MAIL_MSG_SAY_RECEIVE_ITEM, CMailSystemMsg::GetItemName(&m_sMailProfile, TRUE));
				CMailSystemMsg::MsgSay(CMailSystemMsg::MAIL_MSG_SAY_RECEIVE_ZENNY, Logic_FormatZeni(m_sMailProfile.dwZenny));
				break;
			case eMAIL_TYPE_ITEM_ZENNY_REQ:
				CMailSystemMsg::MsgSay(CMailSystemMsg::MAIL_MSG_SAY_RECEIVE_ITEM, CMailSystemMsg::GetItemName(&m_sMailProfile, TRUE));
				CMailSystemMsg::MsgSay(CMailSystemMsg::MAIL_MSG_SAY_RECEIVE_ZENNY_REQ, Logic_FormatZeni(m_sMailProfile.dwZenny), m_sMailProfile.wszFromName);
				break;
			case eMAIL_TYPE_ZENNY_REQ:
				CMailSystemMsg::MsgSay(CMailSystemMsg::MAIL_MSG_SAY_RECEIVE_ZENNY_REQ, Logic_FormatZeni(m_sMailProfile.dwZenny), m_sMailProfile.wszFromName);
				break;
			}						
			CMailSystemMsg::MsgSay(CMailSystemMsg::MAIL_MSG_SAY_RECEIVE_ACCEPT_MAIL, m_sMailProfile.wszFromName);
		}
	}
	else if (msg.Id == g_EventMailLock)
	{
		SNtlEventMailLock* pEvent = reinterpret_cast<SNtlEventMailLock*>(msg.pData);
		if (m_sMailProfile.mailID == pEvent->mailID)
		{
			OpenReadMail(pEvent->mailID);
		}
	}
	else if (msg.Id == g_EventMailDel || msg.Id == g_EventMailReturn )
	{
		SNtlEventMailDel* pEvent = reinterpret_cast<SNtlEventMailDel*>(msg.pData);
		if (m_sMailProfile.mailID == pEvent->mailID)
		{
			GetDialogManager()->CloseDialog(DIALOG_MAILSYSTEM_READ);
		}
	}
	else if( msg.Id == g_EventMailMultiDelRes )
	{
		SNtlEventMailMultiDelRes* pResult = reinterpret_cast<SNtlEventMailMultiDelRes*>(msg.pData);

		for( RwInt32 i = 0; i< pResult->byCount; ++i )
		{
			if( pResult->aMailID[i] == m_sMailProfile.mailID )
			{
				GetDialogManager()->CloseDialog(DIALOG_MAILSYSTEM_READ);
			}
		}
	}
	else if (msg.Id == g_EventMailCloseNfy)
	{
		GetDialogManager()->CloseDialog(DIALOG_MAILSYSTEM_READ);
	}
	

	NTL_RETURNVOID();
}

void CMailSystemReadGui::OnClickedBtnClose(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_MAILSYSTEM_READ);
}

void CMailSystemReadGui::OnMoveDlgAttachItem(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rect = m_pDlgAttachItem->GetScreenRect();
	m_AttachItemSlot.SetParentPosition(rect.left, rect.top);
	m_AttachItemAccept.SetPositionbyParent(rect.left, rect.top);
}

void CMailSystemReadGui::OnPaintDlgAttachItem()
{
	m_AttachItemSlot.Paint();
	
	if (!m_pBtnAttachAction->IsClickEnable() && m_AttachItemSlot.GetSerial() != INVALID_SERIAL_ID)
	{
		m_AttachItemAccept.Render();
	}
}

void CMailSystemReadGui::OnMouseEnterDlgAttachItem(gui::CComponent* pComponenet)
{
	ShowInfoWindow(TRUE);
}

void CMailSystemReadGui::OnMouseLeaveDlgAttachItem(gui::CComponent* pComponent)
{
	ShowInfoWindow(FALSE);
}

void CMailSystemReadGui::OnCaptureMouseDown(const CKey& key)
{
	CAPTURE_MOUSEDOWN_RAISE(DIALOG_MAILSYSTEM_READ, key.m_fX, key.m_fY);
}

void CMailSystemReadGui::OnMoveDlgAttach(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rect = m_pDlgAttach->GetScreenRect();
	m_AttachDisable.SetPositionbyParent(rect.left, rect.top);
}

void CMailSystemReadGui::OnPaintPostDlgAttach()
{
	if (!m_pDlgAttach->IsEnabled())
	{
		m_AttachDisable.Render();
	}
}

void CMailSystemReadGui::OnMoveBtnAttachAction(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle	rect		= m_pBtnAttachAction->GetScreenRect();
	RwInt32		iCenterX	= rect.left + ((rect.right - rect.left) / 2);
	RwInt32		iCenterY	= rect.top + ((rect.bottom - rect.top) / 2);

	m_feAttachAction.SetCenterPos(iCenterX, iCenterY);
}

void CMailSystemReadGui::OnPaintBtnAttachAction()
{
	m_feAttachAction.Render();
}

void CMailSystemReadGui::OnClickedBtnReply(gui::CComponent* pComponent)
{
	CDboEventGenerator::OpenSendMail(m_pIpbTargetName->GetText());
}

void CMailSystemReadGui::OnClickedBtnReturn(gui::CComponent* pComponent)
{
	CMailSystemMsg::MsgBox(CMailSystemMsg::MAIL_MSG_BOX_RETURN);
	m_mailIDTemp = m_sMailProfile.mailID;
}

void CMailSystemReadGui::OnClickedBtnDelete(gui::CComponent* pComponent)
{
	CMailSystemMsg::MsgBox(CMailSystemMsg::MAIL_MSG_BOX_DELETE);
	m_mailIDTemp = m_sMailProfile.mailID;
}

void CMailSystemReadGui::OnClickedBtnAttachAction(gui::CComponent* pComponent)
{
	switch (m_sMailProfile.byMailType)
	{
	case eMAIL_TYPE_BASIC:
		return;
	case eMAIL_TYPE_ITEM:
		CMailSystemMsg::MsgBox(CMailSystemMsg::MAIL_MSG_BOX_RECEIVE_ITEM, CMailSystemMsg::GetItemName(&m_sMailProfile, FALSE));
		break;
	case eMAIL_TYPE_ZENNY:
		CMailSystemMsg::MsgBox(CMailSystemMsg::MAIL_MSG_BOX_RECEIVE_ZENNY, Logic_FormatZeni(m_sMailProfile.dwZenny));
		break;
	case eMAIL_TYPE_ITEM_ZENNY:
		CMailSystemMsg::MsgBox(CMailSystemMsg::MAIL_MSG_BOX_RECEIVE_ITEM_ZENNY, Logic_FormatZeni(m_sMailProfile.dwZenny), CMailSystemMsg::GetItemName(&m_sMailProfile, FALSE));
		break;
	case eMAIL_TYPE_ITEM_ZENNY_REQ:
		CMailSystemMsg::MsgBox(CMailSystemMsg::MAIL_MSG_BOX_RECEIVE_ITEM_ZENNY_REQ, CMailSystemMsg::GetItemName(&m_sMailProfile, FALSE), Logic_FormatZeni(m_sMailProfile.dwZenny));
		break;
	case eMAIL_TYPE_ZENNY_REQ:
		CMailSystemMsg::MsgBox(CMailSystemMsg::MAIL_MSG_BOX_RECEIVE_ZENNY_REQ, Logic_FormatZeni(m_sMailProfile.dwZenny));
		break;
	}
	m_mailIDTemp = m_sMailProfile.mailID;
}

void CMailSystemReadGui::OpenReadMail(const MAILID& mailID)
{
	CNtlMailSystem* pMailSystem = GetNtlSLGlobal()->GetSobAvatar()->GetMailSystem();
	
	if (pMailSystem->GetMail(m_sMailProfile, mailID))
	{
		UpdateReadMail();
		CDboEventGenerator::ReadingMail(m_sMailProfile.mailID, true);
	}
	else
	{
		GetDialogManager()->CloseDialog(DIALOG_MAILSYSTEM_READ);
	}
}

void CMailSystemReadGui::ClearReadMail()
{
	m_pIpbTargetName->Clear();
	m_pIpbMailContent->Clear();
	
	m_pStbAttachCategory->Clear();
	m_pStbAttachZenny->Clear();
	m_pStbDay->Clear();

	m_pPnlIconZennyReq->Show(false);
	m_pPnlIconZennySend->Show(false);

	m_AttachItemSlot.Clear();
	m_bAttachAccept	= false;
	m_sMailProfile	= sMAIL_PROFILE();
	m_mailIDTemp	= INVALID_MAILID;
}

void CMailSystemReadGui::UpdateReadMail()
{
	DisplayReadMailInit();
	switch (m_sMailProfile.byMailType)
	{
	case eMAIL_TYPE_BASIC:
		DisplayReadMailBasic();
		break;
	case eMAIL_TYPE_ITEM:
		DisplayReadMailBasic();
		DisplayReadMailItem();
		break;
	case eMAIL_TYPE_ZENNY:
		DisplayReadMailBasic();
		DisplayReadMailZenny();
		break;
	case eMAIL_TYPE_ITEM_ZENNY:
		DisplayReadMailBasic();
		DisplayReadMailItem();
		DisplayReadMailZenny();
		break;
	case eMAIL_TYPE_ITEM_ZENNY_REQ:
		DisplayReadMailBasic();
		DisplayReadMailItem();
		DisplayReadMailZennyReq();
		break;			
	case eMAIL_TYPE_ZENNY_REQ:
		DisplayReadMailBasic();
		DisplayReadMailZennyReq();
		break;
	}

	CheckAttachEnable();
	CheckAttachActionEnable();
	CheckReplyEnable();
	CheckReturnEnable();
	CheckDeleteEnable();
	CheckAttachActionEffect(); // 꼭 마지막에 체크해야함.
}

void CMailSystemReadGui::DisplayReadMailInit()
{
	m_pIpbTargetName->Clear();
	m_pIpbMailContent->Clear();
	m_pStbSendDate->Clear();
	m_pStbAttachCategory->Clear();
	m_pStbAttachCategory->SetTextColor(RGB(255, 219, 71));
	m_pStbAttachZenny->Clear();
	m_pStbAttachZenny->SetTextColor(RGB(125, 125, 125));

	m_AttachItemSlot.Clear();

	m_pStbAttachMessage->Show(false);
	m_pPnlIconZennySend->Show(false);
	m_pPnlIconZennyReq->Show(false);
	m_pBtnAttachAction->ClickEnable(false);
	m_pDlgAttach->Enable(false);
	m_pBtnReply->ClickEnable(false);
	m_pBtnReturn->ClickEnable(false);
	m_pBtnDelete->ClickEnable(false);
}

void CMailSystemReadGui::DisplayReadMailBasic()
{
	m_pIpbTargetName->SetText(m_sMailProfile.wszFromName);
	if (m_sMailProfile.bIsLock)
	{
		m_pStbDay->SetText(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAIL_EXPIRED_DAY_LOCK"));
	}
	else
	{
		m_pStbDay->SetText( Logic_GetMailRemainTimeString( m_sMailProfile.endTime ) );
	}
	
	RwUInt32 uiYear		= m_sMailProfile.tCreateTime.year;
	RwUInt32 uiMonth	= m_sMailProfile.tCreateTime.month;
	RwUInt32 uiDay		= m_sMailProfile.tCreateTime.day;
	RwUInt32 uiMinute	= m_sMailProfile.tCreateTime.minute;
	RwUInt32 uiHour;

	//%u년 %u월 %u일 %s %u시 %u분
	if( m_sMailProfile.tCreateTime.hour > 12 )
	{
		uiHour = m_sMailProfile.tCreateTime.hour - 12;
		// 오후
		m_pStbSendDate->Format(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAIL_SENDDATE"), uiYear, uiMonth, uiDay,
			GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME_PM" ), uiHour, uiMinute);
	}
	else
	{
		uiHour = m_sMailProfile.tCreateTime.hour;
		// 오전
		m_pStbSendDate->Format(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAIL_SENDDATE"), uiYear, uiMonth, uiDay,
			GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME_AM" ), uiHour, uiMinute);
	}
	
	SetReadMailContent(m_sMailProfile.wszText);
	SetReadMailLock(m_sMailProfile.bIsLock);
}

void CMailSystemReadGui::DisplayReadMailItem()
{
	eREGULAR_SLOT_TYPE eSerialType = REGULAR_SLOT_ITEM_TABLE;
	if (m_sMailProfile.sItemProfile.bNeedToIdentify)
	{
		eSerialType = REGULAR_SLOT_ITEM_NOT_IDENTIFICATION;
	}

	if (eSerialType != m_AttachItemSlot.GetSerialType())
	{
		m_AttachItemSlot.Clear();
		m_AttachItemSlot.SetSerialType(eSerialType);
	}

	m_AttachItemSlot.SetIcon(m_sMailProfile.sItemProfile.tblidx, m_sMailProfile.sItemProfile.byStackcount);
	m_pStbAttachCategory->SetText(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAIL_ZENNY"));
	m_pStbAttachZenny->SetText(L"0");
}

void CMailSystemReadGui::DisplayReadMailZenny()
{
	m_pStbAttachCategory->SetText(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAIL_ZENNY_SEND"));
	m_pStbAttachZenny->Format(L"%s", Logic_FormatZeni(m_sMailProfile.dwZenny));
	m_pPnlIconZennySend->Show(true);
}

void CMailSystemReadGui::DisplayReadMailZennyReq()
{
	m_pStbAttachCategory->SetText(GetDisplayStringManager()->GetString("DST_MAILSYSTEM_MAIL_ZENNY_REQ"));
	m_pStbAttachZenny->Format(L"%s", Logic_FormatZeni(m_sMailProfile.dwZenny));
	m_pPnlIconZennyReq->Show(true);
}

void CMailSystemReadGui::CheckAttachEnable()
{
	m_uiAbilityType = GetNtlSLGlobal()->GetSobAvatar()->GetMailSystem()->GetAbility();
	if (m_sMailProfile.byMailType == eMAIL_TYPE_BASIC)
	{
		return;
	}
	// by daneos: fix unable to take items when reload mail
	/*if ((m_uiAbilityType & CNtlMailSystem::MAILSYSTEM_ABILITY_ATTACH_ACCEPT) == 0)
	{
		if (!m_sMailProfile.bIsAccept)
		{
			m_pStbAttachMessage->Show(true);
		}
		return;
	}*/

	m_pDlgAttach->Enable(true);		
}

void CMailSystemReadGui::CheckAttachActionEnable()
{
	if (m_sMailProfile.bIsAccept)
	{
		return;
	}
	
	m_pStbAttachCategory->SetTextColor(RGB(255, 255, 255));	
	m_pStbAttachZenny->SetTextColor(RGB(255, 255, 255));
	m_pBtnAttachAction->ClickEnable(true);
}

void CMailSystemReadGui::CheckReplyEnable()
{
	if (m_sMailProfile.bySenderType == eMAIL_SENDER_TYPE_SYSTEM)
	{
		return;
	}
	if (m_sMailProfile.bySenderType == eMAIL_SENDER_TYPE_GM)
	{
	}

	m_pBtnReply->ClickEnable(true);
}

void CMailSystemReadGui::CheckReturnEnable()
{
	if (m_sMailProfile.bySenderType == eMAIL_SENDER_TYPE_RETURN)
	{
		return;
	}
	if (m_sMailProfile.bySenderType == eMAIL_SENDER_TYPE_REPLY)
	{
		return;
	}
	if (m_sMailProfile.byMailType == eMAIL_TYPE_BASIC)
	{
		return;
	}
	if (m_sMailProfile.bIsLock)
	{
		return;
	}
	if (m_sMailProfile.bIsAccept)
	{
		return;
	}

	m_pBtnReturn->ClickEnable(true);
}

void CMailSystemReadGui::CheckDeleteEnable()
{
	if (m_sMailProfile.bIsLock)
	{
		return;
	}

	if (m_sMailProfile.byMailType != eMAIL_TYPE_BASIC && !m_sMailProfile.bIsAccept)
	{
		return;
	}

	m_pBtnDelete->ClickEnable(true);
}

void CMailSystemReadGui::ShowInfoWindow(RwBool bIsShow)
{
	if (bIsShow && m_AttachItemSlot.GetSerial() != INVALID_SERIAL_ID)
	{
		CRectangle rect = m_pDlgAttachItem->GetScreenRect();
		
		if (m_sMailProfile.sItemProfile.bNeedToIdentify)
		{
			GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_UNIDENTIFIED_ITEM, rect.left, rect.top, &m_sMailProfile.sItemProfile, DIALOG_MAILSYSTEM);
		}
		else
		{
			GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_PROFILE_ITEM, rect.left, rect.top, &m_sMailProfile.sItemProfile, DIALOG_MAILSYSTEM);
		}
	}
	else
	{
		GetInfoWndManager()->ShowInfoWindow(FALSE);
	}
}

void CMailSystemReadGui::SetReadMailLock(RwBool bIsLock)
{
	m_pDlgLock->GetSurface()->clear();
	if (bIsLock)
	{
		m_pDlgLock->AddSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MailSystemRead.srf", "srfLock"));
	}
	else
	{
		m_pDlgLock->AddSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MailSystemRead.srf", "srfUnLock"));
	}
}

void CMailSystemReadGui::SetReadMailContent(WCHAR* pwcMailContent)
{
	WCHAR	aMailContent[NTL_MAX_LENGTH_OF_MAIL_MESSAGE + 1];
	switch (m_sMailProfile.bySenderType)
	{
	case eMAIL_SENDER_TYPE_REPLY:
		CMailSystemMsg::ModReplyMsg(aMailContent, NTL_MAX_LENGTH_OF_MAIL_MESSAGE + 1, &m_sMailProfile);
		pwcMailContent = aMailContent;
		break;
	case eMAIL_SENDER_TYPE_RETURN:
		CMailSystemMsg::ModReturnMsg(aMailContent, NTL_MAX_LENGTH_OF_MAIL_MESSAGE + 1, &m_sMailProfile);
		pwcMailContent = aMailContent;
		break;
	}
	m_pIpbMailContent->SetText(pwcMailContent);
}

void CMailSystemReadGui::CheckAttachActionEffect()
{
	RunableAttachActionEffect(m_pBtnAttachAction->IsClickEnable() && m_pDlgAttach->IsEnabled());
}

void CMailSystemReadGui::RunableAttachActionEffect(RwBool bRunable)
{
	if (bRunable)
	{
		if (!m_feAttachAction.IsWork())
			m_feAttachAction.StartProc(TRUE);
	}
	else
	{
		if (m_feAttachAction.IsWork())
			m_feAttachAction.EndProc();
	}
}

void CMailSystemReadGui::Update(RwReal fElapsed)
{
	m_feAttachAction.Update(fElapsed);
}