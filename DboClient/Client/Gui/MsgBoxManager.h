////////////////////////////////////////////////////////////////////////////////
// File: MsgBoxManager.h
// Desc: MsgBox
//
// 2006.06.17 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __MSGBOX_MANAGER_H__
#define __MSGBOX_MANAGER_H__

#include "ceventhandler.h"
#include "eventtimer.h"
#include "DboDef.h"

class CMsgBoxGui;
class SDboEventMsgBoxShow;

class CMsgBoxManager : public RWS::CEventHandler
{
public:
	typedef std::list<CMsgBoxGui*>				LIST_MSGBOX;
	typedef std::list<CMsgBoxGui*>::iterator	LIST_MSGBOX_ITER;
private:
	//! Constructor & Destructor
	CMsgBoxManager(VOID);
public:
	~CMsgBoxManager(VOID);
	
	//! Access
	static RwBool CreateInstance(VOID);
	static VOID   DeleteInstance(VOID);

	static CMsgBoxManager* GetInstance(VOID);
	
	// 삭제는 절대로 삭제함수만을 이용.
	VOID DeleteMsgBox(const char* strStringID );
	VOID DeleteMsgBox( CMsgBoxGui* pDeletedMsgBox );
	VOID DeleteAllMsgBoxExceptNetConnectBox(VOID);
	VOID DeleteNetConnectBox(VOID);

	// 복수개로 나올 수도 있음. 그럴때는 리스트포인터를 이용. 리턴은 가장 마지막으로 발견된 값. 
	CMsgBoxGui* GetMsgBox(char* strStringID, LIST_MSGBOX* pList = NULL );

	// 나레이션등 메시지 박스들을 단지 보여주거나 보여주지 않는 기능.
	VOID ShowAllMsgBox( bool bShow );	

	RwBool IsShowMsgBox(VOID);			// 전체메시지박스가 보일때
	RwBool IsShowPopupMsgBox(VOID);		// 팝업메시지박스가 보일때

private:
	//! Implementation
	CMsgBoxGui* CreateMsgBox( RwBool bPopup, const char* StringID);
	VOID DeleteAllMsgBox(VOID);

	VOID MsgBoxShowProc( RwBool bPopup, RwBool bHasInput, std::list<sMsgBoxCustomBtn>* plistCustomBtn, const WCHAR* szText, const char* StringID,
						 sMsgBoxData* pData, RwReal fShowTime, RwBool bDuplicate, RwBool bUpdate );

	VOID SetMsgBoxAlpha( RwUInt8 byAlpha );

	//! Event
	VOID HandleEvents( RWS::CMsg &msg );

	VOID LinkEvent(VOID);
	VOID UnLinkEvent(VOID);

	//! CallBack

	//! Variable
	LIST_MSGBOX		m_listMsgBox;
	LIST_MSGBOX		m_listPopupMsgBox;

	static CMsgBoxManager* m_pInstance;
};

static CMsgBoxManager* GetMsgBoxManager(VOID)
{
	return CMsgBoxManager::GetInstance();
}

#endif//__ICON_MOVE_MANAGER_H__