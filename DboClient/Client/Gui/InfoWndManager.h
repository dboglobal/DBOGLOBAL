////////////////////////////////////////////////////////////////////////////////
// File: InfoWndManager.h
// Desc: InfoWnd
//
// 2006.06.17 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __INFOWND_MANAGER_H__
#define __INFOWND_MANAGER_H__

#include "ceventhandler.h"
#include "DialogDefine.h"
#include "NtlBattle.h"
#include "NtlSharedType.h"

class CInfoWindowGui;
struct sITEM_TBLDAT;

struct stINFOWND_REWARD
{
	RwInt32		eType;
	RwInt32		nValue;
};

struct stINFOWND_UPGRADE
{
	enum MAXLINE { MAXLINECOUNT = 4 };

	stINFOWND_UPGRADE::stINFOWND_UPGRADE(VOID)
	{
		for( RwInt32 i = 0 ; i < MAXLINECOUNT ; ++i )
		{
			wszText[i] = NULL;
			uiColor[i] = RGB( 255, 255, 255 );
		}
	}

	WCHAR*		wszText[MAXLINECOUNT];
	RwUInt32	uiColor[MAXLINECOUNT];
};

/**
* \brief Battle Attribute의 계산된 속성을 출력하기 위하여
*/
struct stINFOWND_BATTLEATTR
{
	enum eTYPE 
	{ 
		TYPE_ATTR_WEAPON_INFO = 0,			///< 무기 속성이 가지는 상성 관계
		TYPE_ATTR_UPGRADE,					///< 업그레이드시 상성 관계
		TYPE_ATTR_NUM,						///< 전투속성 툴팁의 타입 갯수
		TYPE_ATTR_INVALID = TYPE_ATTR_NUM
	};

	eTYPE	eBattleAttrInfoType;
	RwUInt8 bySourceWeaponAttr;
	RwUInt8 byTargetWeaponAttr;
	RwReal	afSourceOffenceBonus[BATTLE_ATTRIBUTE_COUNT];
	RwReal	afTargetOffenceBonus[BATTLE_ATTRIBUTE_COUNT];

	stINFOWND_BATTLEATTR::stINFOWND_BATTLEATTR(VOID)
	{
		eBattleAttrInfoType = TYPE_ATTR_INVALID;
		bySourceWeaponAttr = INVALID_BYTE;
		byTargetWeaponAttr = INVALID_BYTE;

		for(int i=0; i <BATTLE_ATTRIBUTE_COUNT; ++i ) 
		{
			afSourceOffenceBonus[i] = 0.0f;
			afTargetOffenceBonus[i] = 0.0f;
		}
	}
};

struct stINFOWND_QUESTINDICATOR
{
	RwUInt32	uiQuestTitle;
	RwUInt32	uiQuestGoal;
};

class CInfoWndManager : public RWS::CEventHandler
{
private:
	//! Constructor & Destructor
	CInfoWndManager(VOID);
public:
	//! Enum
	enum eINFOWNDSTATE
	{
		INFOWND_NOTSHOW = 0,
		INFOWND_ITEM,							// General items.	
		INFOWND_ITEM_DATA,						// An item of type sITEM_DATA.
		INFOWND_PROFILE_ITEM,					// An item in profile form.	
		INFOWND_TABLE_ITEM,						// Item on the table
		INFOWND_JUST_ITEM_NAME,					// Output only item names
		INFOWND_UNIDENTIFIED_ITEM,
		INFOWND_SKILL,
		INFOWND_NOTLEARN_SKILL, 
		INFOWND_SKILLRQ,
		INFOWND_BUFF,
		INFOWND_HTBSKILL,
		INFOWND_NOTLEARN_HTBSKILL,
		INFOWND_JUST_WTEXT,
		INFOWND_ACTIONSKILL,
		INFOWND_QUESTITEM,
		INFOWND_NOTSOBBUFF,
		INFOWND_REWARD,
		INFOWND_FAKEBUFF,						//! 파티 버프
		INFOWND_MINIMAP,
        INFOWND_PORTAL,                         ///< NPC를 이용한 포탈시 사용되는 맵의 툴팁
        INFOWND_FRIEND_LIST,                    ///< 친구리스트에서 세부 정보를 표시하는 툴팁
		INFOWND_UPGRADE,						// 업그레이드 아이템 장착시 호이포이스톤.
		INFOWND_BATTLEATTRIBUTE,				///< Battle Attribute의 계산된 속성을 표시하는 툴팁
		INFOWND_QUEST_SEARCH,
		INFOWND_QUEST_INDICATOR,				/// 퀘스트알리미 마우스오버 정보.
		INFOWND_SKILL_SPUPGRADE,				/// SP 업그레이드 버튼.
		INFOWND_RECIPE,
		INFOWND_HLS_ITEM,
	};

	~CInfoWndManager(VOID);
	//! Access
	static RwBool CreateInstance(VOID);
	static VOID   DeleteInstance(VOID);

	static CInfoWndManager* GetInstance(VOID);

	VOID ShowInfoWindow( RwBool bShow, RwInt32 eState = INFOWND_NOTSHOW, RwInt32 nXPos = 0, RwInt32 nYPos = 0, VOID* pInstance = NULL, RwInt32 eRequestGui = DIALOG_UNKNOWN );
	eINFOWNDSTATE GetInfoWndState(VOID) { return m_eWndState; }
	RwInt32 GetRequestGui(VOID) { return m_eRequestGui; }

private:
	//! Implementation
	VOID LinkEvent(VOID);
	VOID UnLinkEvent(VOID);

	//! Event
	VOID HandleEvents( RWS::CMsg &msg );

	//! CallBack

	//! Variable
	CInfoWindowGui*	m_pInfoWnd;

	eINFOWNDSTATE	m_eWndState;
	RwInt32			m_eRequestGui;;

	static CInfoWndManager* m_pInstance;
};

static CInfoWndManager* GetInfoWndManager(VOID)
{
	return CInfoWndManager::GetInstance();
}

#endif//__INFOWND_MANAGER_H__