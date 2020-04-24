/*****************************************************************************
* File			: ActionMap.h
* Author		: Haesung, Cho
* Copyright		: (주)NTL
* Date			: 2007. 9. 20
* Abstract		: Action Map
*****************************************************************************
* Desc			: 액션맵의 자료구조를 정의하고 액션맵의 매니저를 작성한다.
* History  -----
* 2008.  2. 22	: 리팩토링, 기존의 액션맵 방식에서 자료구조와 매니저를 분리( 속성 추가 )
*****************************************************************************/

#ifndef __ACTION_MAP_H__
#define __ACTION_MAP_H__

// Simulation
#include "NtlActionMapDef.h"
#include "InputHandler.h"

// shared
#include "NtlCharacter.h"

#include <map>
#include <vector>
#include <set>

class CActionMapManager;

/**
* \ingroup simulation
* \brief NTL ActionMap의 자료구조
*/
class CActionMap
{
public:
	// 생성자 & 소멸자
	CActionMap(void);
	CActionMap( CActionMapManager* pManager );
	CActionMap(const CActionMap& rhs);
	~CActionMap(void);

	WORD	FindAction( WORD wKey );			// Key 값으로 Action을 찾는다.
	WORD	FindKey( WORD wAction );			// Action 으로 Key를 찾는다.

	BYTE			SetAction( WORD wKey, WORD wAction );	// 키를 지정
	VOID			SetLastReleaseAction( WORD wOldAction );
	WORD			GetLastReleaseAction();

	VOID			Clear();									// ActionMap을 초기화한다.
	VOID			ClearReleaseAction();

	VOID			CaptureMap( CActionMap* pCaptureMap );
	VOID			RestoreMap( CActionMap* pCaptureMap );
	BOOL			CheckMap( ACTIONMAP& mapCapture );

	ACTIONMAP&		GetActionMap()							{	return m_mapAction;	}
	ACTIONMAP&		GetKeyMap()							{	return m_mapKey;	}

	CActionMap& operator=(const CActionMap& rhs);				// 연산자 오버로딩

protected:
	ACTIONMAP		m_mapKey;				// 자료구조 < ACTION , KEY >
	ACTIONMAP		m_mapAction;			// 자료구조 < KEY, ACTION >
	ACTIONVEC		m_vecReleaseAction;		// 해제된 액션을 기억하기 위한 자료구조
	CActionMapManager*	m_pManager;	
};

/**
* \ingroup simulation
* \brief NTL ActionMap의 Manager
*/
class CActionMapManager
{
public:
	///< Construction & Destruction
	CActionMapManager(void);
	~CActionMapManager(void);

	///< Create & Destroy
	VOID	Create(VOID);
	VOID	Destroy(VOID);

	///< Basic action map settings (hard-coding)
	VOID	InitDefaultActionMap();

	BYTE	SetSingleKey( BYTE byKey, WORD wAction );		///< Single key setting
	BYTE	SetCombineKey( WORD wKey, WORD wAction );		///< Combination key setting

	WORD	FindAction( WORD wKey );
	WORD	FindKey( WORD wAction );

	WORD	GetInputAction()						{ return m_wInputAction; }
	BOOL	SetInputAction( WORD wAction );					///< Specify input mode
	VOID	ClearInputAction()						{ m_wInputAction = ACTION_INVALID; }

	BYTE	GetLastResult();
	VOID	SetLastResult( BYTE byResult );

	VOID	ClearResult();

	WORD	GetLastReleaseAction()					{ return m_pActionMap->GetLastReleaseAction(); }
	VOID			ClearReleaseAction()			{ m_pActionMap->ClearReleaseAction(); }

	std::wstring	GetKeyName( WORD wAction );

	WORD	InputHandlerDown( BYTE byKey );
	WORD	InputHandlerUp( BYTE byKey, std::list< WORD >& listUpAction );
	void	InputActionDown( WORD wAction );
	void	InputActionUp( WORD wAction );

	void	CheckDownActionWithCombineKey( BYTE byKey, std::list< WORD >& listUpAction );

	void	InputClear();

	VOID	CaptureActionMap();
	VOID	RestoreActionMap();

	void	AddUpdateData(BYTE byType, WORD wActionID, WORD wKey);
	RwBool	GetUpdateData(sSHORTCUT_UPDATE_DATA* pData , BYTE& byCount);

	VOID	ClearActionMap();

	VOID	SetActionMapMode( BYTE byActionMapMode );
	BYTE	GetActionMapMode()					{ return m_byActionMapMode; }
protected:
	WORD	MakeKey( BYTE byKey );
	virtual void	MakeComposition( uNtlKey& uMakeKey );

	VOID	RegisterKeyName(VOID);
	
	VOID	RegisterKeyType(VOID);
	VOID	RegisterActionType(VOID);

public:
	BOOL	IsFixedKey( WORD wKey )				{ return IsMapType( TYPE_FIXED, wKey, m_mapKeyType ); }
	BOOL	IsFixedAction( WORD wAction )		{ return IsMapType( TYPE_FIXED, wAction, m_mapActionType ); }
	BOOL	IsCombineKey( WORD wKey )			{ return IsMapType( TYPE_COMBINE, wKey, m_mapKeyType ); }
	BOOL	IsExclusiveAction( WORD wAction )	{ return IsMapType( TYPE_EXCLUSIVE, wAction, m_mapActionType ); }

	BOOL	IsMapType( int nType, WORD wKeyType, TYPEMAP& mapType );

protected:
	KEYNAMEMAP	m_mapKeyName;
	TYPEMAP		m_mapKeyType;
	TYPEMAP		m_mapActionType;

	typedef std::map< unsigned short, unsigned int > mapdef_InputAction;
	mapdef_InputAction	m_mapInputAction;							///< 현재 InputHandlerDown으로 검출된 Action들의 참조 리스트

	RESULTVEC	m_vecResult;

	WORD		m_wInputAction;										///< 키 입력을 받을 때는 ACTION_INVALID 이외의 값을 가지고 있는다.
	
	CActionMap*		m_pActionMap;									///< 클라이언트에서 사용되는 ActionMap
	CActionMap*		m_pCaptureMap;									///< 서버에 저장된 것을 가지고 있는다.
	CActionMap*		m_pDefaultMap;									///< 가장 기본적으로 지정되는 ActionMap

	sSHORTCUT_UPDATE_DATA	m_asUpdateData[NTL_SHORTCUT_MAX_COUNT];	///< 변경된 사항을 저장하는 배열
	BYTE					m_byUpdateCount;						///< 키의 변경사항이 있을때	

	BYTE					m_byActionMapMode;						///< ACTIONMAP의 사용 모드
	
};

#endif // __ACTION_MAP_H__