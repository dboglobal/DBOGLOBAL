#include "precomp_ntlsimulation.h"
#include "ActionMap.h"

// framework
#include "NtlApplication.h"

// Simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"

//////////////////////////////////////////////////////////////////////////////////////////////
// CActionMap
//
//
/////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Construction
*/
CActionMap::CActionMap( void )
{
	
}

/**
* \brief Copy Construction
*/
CActionMap::CActionMap( const CActionMap& rhs ) 
{
	this->m_mapAction = rhs.m_mapAction;
	this->m_mapKey = rhs.m_mapKey;
}

CActionMap::CActionMap( CActionMapManager* pManager ) 
: m_pManager(NULL)
{
	m_pManager = pManager;
}

/**
* \brief Destruction
*/
CActionMap::~CActionMap( void ) 
{
	Clear();
}

/**
* \breif Key값으로 Action을 찾아서 리턴한다.
* \param wKey (WORD) 만들어진 NTL Key
* \return 찾은 ACTION의 ID ( Key가 없다면 ACTION_INVALID )
*/
WORD CActionMap::FindAction( WORD wKey ) 
{
	ACTIONMAP::iterator it = m_mapAction.find( wKey );
	if( it == m_mapAction.end() )
		return ACTION_INVALID;

	return it->second;
}

/**
* \brief Action값으로 Key를 찾아서 리턴한다.
* \param wAction (WORD) ACTION ID
* \return (WORD) KEY DATA
*/
WORD CActionMap::FindKey( WORD wAction ) 
{
	ACTIONMAP::iterator it = m_mapKey.find( wAction );
	if( it == m_mapKey.end() )
		return dNTL_KEY_INVALID;

	return it->second;
}

/**
* \brief 액션을 자료구조에 설정한다.
* \param wKey		(WORD) 만들어진 NTL key
* \param wAction	(WORD) ACTION ID
*/
BYTE CActionMap::SetAction( WORD wKey, WORD wAction ) 
{
	// 액션이 등록되어 있는지 확인한다.
	uNtlKey uUseKey; 
	uUseKey.key = FindKey( wAction );

	// ADD : 기존에 자료구조에 없을 경우
	if( uUseKey.key == dNTL_KEY_INVALID )
	{
		// 현재의 키가 사용되고 있는지 확인한다.
		ACTIONMAP::iterator itActionMap = m_mapAction.find( wKey );

		// 현재의 키가 사용되지 않는다면 추가를 한다.
		if( itActionMap == m_mapAction.end() )
		{
			m_mapKey[wAction] = wKey;
			m_mapAction[wKey] = wAction;

			return SET_RESULT_OK;
		}

		// 현재의 키가 사용되고 있다면 기존에 등록되어 있던 Action을 저장하고 지운다.
		WORD wOldAction = (*itActionMap).second;
		m_mapAction.erase( itActionMap );

		// KeyMap에서 해당하는 액션의 Key를 None으로 설정한다.
		ACTIONMAP::iterator itKeyMap = m_mapKey.find( wOldAction );
		(*itKeyMap).second = dNTL_KEY_EMPTY;

		// 키를 잃어버린 액션을 벡터에 보관한다.
		SetLastReleaseAction( wOldAction );

		// 키와 액션을 등록한다.
		m_mapKey[wAction] = wKey;
		m_mapAction[wKey] = wAction;
		
		return SET_RESULT_ALREADY_KEY;
	}
	// UPDATE : 액션은 등록되어 있는데 키가 없는 경우
	else if( uUseKey.key == dNTL_KEY_EMPTY )
	{
		// 현재의 키가 사용되고 있는지 확인한다.
		ACTIONMAP::iterator itActionMap = m_mapAction.find( wKey );
		
		// 현재의 키가 사용되지 않는다면 현재의 액션에 바로 변경을 해준다.
		if( itActionMap == m_mapAction.end() )
		{
			// 키를 등록
			m_mapAction[wKey] = wAction;

			// 액션을 등록
			ACTIONMAP::iterator itKeyMap = m_mapKey.find( wAction );
			if( itKeyMap != m_mapKey.end() )
				(*itKeyMap).second = wKey;

			return SET_RESULT_OK;
		}
		
		// 현재의 키가 사용되고 있다면 기존에 등록되어 있던 Action을 저장하고 지운다.
		WORD wOldAction = (*itActionMap).second;
		m_mapAction.erase( itActionMap );

		// KeyMap에서 해당하는 액션의 Key를 None으로 설정한다.
		ACTIONMAP::iterator itKeyMap = m_mapKey.find( wOldAction );
		(*itKeyMap).second = dNTL_KEY_EMPTY;

		// 키를 잃어버린 액션을 벡터에 보관한다.
		SetLastReleaseAction( wOldAction );

		// 액션에 있는 값을 변경해준다.
		itKeyMap = m_mapKey.find( wAction );
		if( itKeyMap != m_mapKey.end() )
			(*itKeyMap).second = wKey;

		// 키를 등록한다.
		m_mapAction[wKey] = wAction;

		return SET_RESULT_ALREADY_KEY;
	}
	// UPDATE : 액션도 등록되어 있고 액션에 KEY도 존재하는 경우
	// 그리고 그 KEY가 다른 액션에 존재하거나 하지 않는 경우
	else
	{
		// 기존에 등록되어 있던 키의 Action을 저장하고 지운다.
		ACTIONMAP::iterator itActionMap = m_mapAction.find( wKey );

		// 키가 다른 액션에 존재하는 경우
		if( itActionMap != m_mapAction.end() )
		{
			WORD wOldAction = (*itActionMap).second;

			// 만약 기존에 키와 액션이 똑같다면
			if( wOldAction == wAction && (*itActionMap).first == wKey)
				return SET_RESULT_SAME_KEY;

			m_mapAction.erase( itActionMap );

			// KeyMap에서 해당하는 액션의 Key를 None으로 설정한다.
			ACTIONMAP::iterator itKeyMap = m_mapKey.find( wOldAction );
			(*itKeyMap).second = dNTL_KEY_EMPTY;

			// 키를 잃어버린 액션을 벡터에 보관한다.
			SetLastReleaseAction( wOldAction );

			// 새로 등록할 ACTION에 있는 예전의 KEY를 저장하고 변경된 키를 넣는다.
			ACTIONMAP::iterator itNewKey = m_mapKey.find( wAction );
			WORD wOldKey = (*itNewKey).second;

			(*itNewKey).second = wKey;

			// 기존에 있던 ActionMap을 삭제해준다.
			itActionMap = m_mapAction.find( wOldKey );
			if( itActionMap != m_mapAction.end() )
				m_mapAction.erase( itActionMap );

			// 새로 등록한다.
			m_mapAction[wKey] = wAction;

			return SET_RESULT_ALREADY_KEY;
		}

		// 현재 등록하려는 키가 다른 액션에 존재하지 않는 경우 ( 정상적인 처리 )
		ACTIONMAP::iterator itNewKey = m_mapKey.find( wAction );
		WORD wOldKey = (*itNewKey).second;

		(*itNewKey).second = wKey;

		// 기존에 있던 ActionMap을 삭제해준다.
		itActionMap = m_mapAction.find( wOldKey );
		if( itActionMap != m_mapAction.end() )
			m_mapAction.erase( itActionMap );

		// 새로 등록한다.
		m_mapAction[wKey] = wAction;

		return SET_RESULT_OK;
	}
}

VOID CActionMap::SetLastReleaseAction( WORD wOldAction ) 
{
	if( (int)m_vecReleaseAction.size() > 10 )
		m_vecReleaseAction.clear();

	m_vecReleaseAction.push_back( wOldAction );
}

/**
* \brief 마지막으로 키가 해제된 Action을 리턴한다.
*/
WORD CActionMap::GetLastReleaseAction() 
{
	if( m_vecReleaseAction.empty() )
		return ACTION_INVALID;

	WORD usReleaseAction = m_vecReleaseAction.back();
	m_vecReleaseAction.pop_back();

	return usReleaseAction;
}

/**
* \brief 자료구조를 초기화한다.
*/
VOID CActionMap::Clear() 
{
	m_mapAction.clear();
	m_mapKey.clear();
	ClearReleaseAction();
}

/**
* \brief 해제된 액션을 저장하는 자료구조를 초기화
*/
VOID CActionMap::ClearReleaseAction() 
{
	m_vecReleaseAction.clear();
}

/**
* \brief 들어온 데이터에 현재 데이터를 저장한다.
*/
VOID CActionMap::CaptureMap( CActionMap* pCaptureMap ) 
{
	pCaptureMap->GetKeyMap() = m_mapKey;
	pCaptureMap->GetActionMap() = m_mapAction;
}

/**
* \brief 들어온 데이터로 현재 데이터를 구성한다.
*/
VOID CActionMap::RestoreMap( CActionMap* pCaptureMap ) 
{
	m_mapKey = pCaptureMap->GetKeyMap();
	m_mapAction = pCaptureMap->GetActionMap();
}

/**
* \brief = 연산자 오버로딩: rhs 항의 자료를 this 항으로 복사한다.
*/
CActionMap& CActionMap::operator=(const CActionMap& rhs)
{
	this->m_mapAction = rhs.m_mapAction;
	this->m_mapKey = rhs.m_mapKey;

	return (*this);
}

/**
* \brief 현재의 맵과 변동사항이 있는 것을 체크한다.
*/
BOOL CActionMap::CheckMap( ACTIONMAP& mapCapture ) 
{
	// m_mapAction과 m_mapCapture가 같다면 변경된 점이 없는 것.
	if( m_mapKey == mapCapture )
		return FALSE;

	ACTIONMAP::iterator itOriginal;
	ACTIONMAP::iterator itCapture;

	// 1. 액션맵을 원본으로 하여 검사
	for(itOriginal = m_mapKey.begin(); itOriginal != m_mapKey.end(); ++itOriginal)
	{
		itCapture = mapCapture.find( itOriginal->first );

		// 오리지날엔 있고 Capture엔 없는 경우
		if( itCapture == mapCapture.end() )
		{
			// 등록
			m_pManager->AddUpdateData( eSHORTCUT_CHANGE_TYPE_ADD, itOriginal->first, itOriginal->second );
		}
		// 둘다 있는 경우
		else
		{
			// 키가 다를 경우
			if( itOriginal->second != itCapture->second )
			{
				// 변경
				m_pManager->AddUpdateData( eSHORTCUT_CHANGE_TYPE_UPDATE, itOriginal->first, itOriginal->second );
			}
		}
	}

	// 2. 캡쳐맵을 원본으로 하여 검사
	for( itCapture = mapCapture.begin(); itCapture != mapCapture.end(); ++itCapture )
	{
		itOriginal = m_mapKey.find( itCapture->first );

		// 캡쳐에는 있는데 오리지날엔 없는 경우
		if( itOriginal == m_mapKey.end() )
		{
			// 삭제
			m_pManager->AddUpdateData( eSHORTCUT_CHANGE_TYPE_DEL, itCapture->first, itCapture->second );
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////
// CActionMapManager
//
//
/////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Contruction
*/
CActionMapManager::CActionMapManager(void)
: m_wInputAction(ACTION_INVALID)
, m_pActionMap(NULL)
, m_pCaptureMap(NULL)
, m_pDefaultMap(NULL)
{
	Create();
}

/**
* \brief Destruction
*/
CActionMapManager::~CActionMapManager(void)
{
	Destroy();
}

/**
* \brief Create
*/
VOID CActionMapManager::Create( VOID ) 
{
	m_pActionMap = NTL_NEW CActionMap( this );
	m_pCaptureMap = NTL_NEW CActionMap( this );
	m_pDefaultMap = NTL_NEW CActionMap( this );

	// Init Variable data
	memset( m_asUpdateData, 0, sizeof( sSHORTCUT_UPDATE_DATA ) * NTL_SHORTCUT_MAX_COUNT );
	m_byUpdateCount = 0;

	m_byActionMapMode = ACTIONMAP_MODE_USERSETTING;

	RegisterKeyName();
	RegisterActionType();
	RegisterKeyType();
}

/**
* \brief Destroy
*/
VOID CActionMapManager::Destroy( VOID ) 
{
	NTL_DELETE( m_pDefaultMap );
	NTL_DELETE( m_pCaptureMap );
	NTL_DELETE( m_pActionMap );
}

/**
* \brief 기본적으로 사용되는 액션맵을 정의한다.
*/
VOID CActionMapManager::InitDefaultActionMap() 
{
	m_pActionMap->Clear();

	// Avatar 행동
	SetSingleKey( 'W', ACTION_AVATAR_FORWARD );
	SetSingleKey( 'S', ACTION_AVATAR_BACKWARD );
	SetSingleKey( 'A', ACTION_AVATAR_LEFTTURN );
	SetSingleKey( 'D', ACTION_AVATAR_RIGHTTURN );
	SetSingleKey( 'Q', ACTION_AVATAR_LEFTSIDE);
	SetSingleKey( 'E', ACTION_AVATAR_RIGHTSIDE);
	SetSingleKey( 'X', ACTION_AVATAR_SITDOWN );
	SetSingleKey( ' ', ACTION_AVATAR_JUMP );
	SetSingleKey( '*', ACTION_AVATAR_AUTORUN );
	SetSingleKey( 'T', ACTION_AVATAR_SCOUTER );
	SetSingleKey( 'Z', ACTION_AVATAR_CHARGE );
	SetSingleKey( 'V', ACTION_AVATAR_LOOTING );
	SetSingleKey( 'R', ACTION_AVATAR_BLOCKING );

	// Target 선택 및 단축키 설정
	SetSingleKey(NTL_KEY_WAVE, ACTION_TARGET_SELF);
	SetCombineKey(NTL_ALT_BIT | NTL_KEY_NUMPAD1, ACTION_TARGET_1STPARTY);
	SetCombineKey(NTL_ALT_BIT | NTL_KEY_NUMPAD2, ACTION_TARGET_2NDPARTY);
	SetCombineKey(NTL_ALT_BIT | NTL_KEY_NUMPAD3, ACTION_TARGET_3RDPARTY);
	SetCombineKey(NTL_ALT_BIT | NTL_KEY_NUMPAD4, ACTION_TARGET_4THPARTY);

	SetCombineKey(NTL_CTRL_BIT | NTL_KEY_TAB, ACTION_TARGET_MEMBER);

	SetSingleKey(NTL_KEY_TAB, ACTION_TARGET_AUTO);
	SetSingleKey('F', ACTION_TARGET_AUTOATK);
	SetSingleKey('/', ACTION_TARGET_AUTOFOLLOW);
	SetCombineKey(NTL_ALT_BIT | NTL_KEY_F1, ACTION_TARGET_MARKING_1 );
	SetCombineKey(NTL_ALT_BIT | NTL_KEY_F2, ACTION_TARGET_MARKING_2 );
	SetCombineKey(NTL_ALT_BIT | NTL_KEY_F3, ACTION_TARGET_MARKING_3 );
	SetCombineKey(NTL_ALT_BIT | NTL_KEY_F4, ACTION_TARGET_MARKING_4 );
	SetCombineKey(NTL_ALT_BIT | NTL_KEY_F5, ACTION_TARGET_MARKING_5 );
	SetCombineKey(NTL_CTRL_BIT | NTL_KEY_F1, ACTION_TARGET_SELECTING_1 );
	SetCombineKey(NTL_CTRL_BIT | NTL_KEY_F2, ACTION_TARGET_SELECTING_2 );
	SetCombineKey(NTL_CTRL_BIT | NTL_KEY_F3, ACTION_TARGET_SELECTING_3 );
	SetCombineKey(NTL_CTRL_BIT | NTL_KEY_F4, ACTION_TARGET_SELECTING_4 );
	SetCombineKey(NTL_CTRL_BIT | NTL_KEY_F5, ACTION_TARGET_SELECTING_5 );
	SetCombineKey(NTL_CTRL_BIT | NTL_KEY_TAB, ACTION_TARGET_AUTOTARGET_PARTY );

	// Quick Slot 단축키 설정
	SetSingleKey('1', ACTION_QUICK_1);
	SetSingleKey('2', ACTION_QUICK_2);
	SetSingleKey('3', ACTION_QUICK_3);
	SetSingleKey('4', ACTION_QUICK_4);
	SetSingleKey('5', ACTION_QUICK_5);
	SetSingleKey('6', ACTION_QUICK_6);
	SetSingleKey('7', ACTION_QUICK_7);
	SetSingleKey('8', ACTION_QUICK_8);
	SetSingleKey('9', ACTION_QUICK_9);
	SetSingleKey('0', ACTION_QUICK_0);
	SetSingleKey(NTL_KEY_DASH, ACTION_QUICK_MIN);
	SetSingleKey(NTL_KEY_EQUAL, ACTION_QUICK_PLUS);
	SetCombineKey(NTL_KEY_F1, ACTION_QUICK_1_EX);
	SetCombineKey(NTL_KEY_F2, ACTION_QUICK_2_EX);
	SetCombineKey(NTL_KEY_F3, ACTION_QUICK_3_EX);
	SetCombineKey(NTL_KEY_F4, ACTION_QUICK_4_EX);
	SetCombineKey(NTL_KEY_F5, ACTION_QUICK_5_EX);
	SetCombineKey(NTL_KEY_F6, ACTION_QUICK_6_EX);
	SetCombineKey(NTL_KEY_F7, ACTION_QUICK_7_EX);
	SetCombineKey(NTL_KEY_F8, ACTION_QUICK_8_EX);
	SetCombineKey(NTL_KEY_F9, ACTION_QUICK_9_EX);
	SetCombineKey(NTL_KEY_F10, ACTION_QUICK_0_EX);
	SetCombineKey(NTL_KEY_F11, ACTION_QUICK_MIN_EX);
	SetCombineKey(NTL_KEY_F12, ACTION_QUICK_PLUS_EX);
	SetCombineKey(NTL_ALT_BIT | '1', ACTION_QUICK_1_EX2);
	SetCombineKey(NTL_ALT_BIT | '2', ACTION_QUICK_2_EX2);
	SetCombineKey(NTL_ALT_BIT | '3', ACTION_QUICK_3_EX2);
	SetCombineKey(NTL_ALT_BIT | '4', ACTION_QUICK_4_EX2);
	SetCombineKey(NTL_ALT_BIT | '5', ACTION_QUICK_5_EX2);
	SetCombineKey(NTL_ALT_BIT | '6', ACTION_QUICK_6_EX2);
	SetCombineKey(NTL_ALT_BIT | '7', ACTION_QUICK_7_EX2);
	SetCombineKey(NTL_ALT_BIT | '8', ACTION_QUICK_8_EX2);
	SetCombineKey(NTL_ALT_BIT | '9', ACTION_QUICK_9_EX2);
	SetCombineKey(NTL_ALT_BIT | '0', ACTION_QUICK_0_EX2);
	SetCombineKey(NTL_ALT_BIT | NTL_KEY_DASH, ACTION_QUICK_MIN_EX2);
	SetCombineKey(NTL_ALT_BIT | NTL_KEY_EQUAL, ACTION_QUICK_PLUS_EX2);
	SetSingleKey(0xDB, ACTION_QUICK_PREV); // '['
	SetSingleKey(0xDD, ACTION_QUICK_NEXT); // ']'
//	SetSingleKey('', ACTION_QUICK_2NDCAP);
//	SetSingleKey('', ACTION_QUICK_3RDCAP);
//	SetSingleKey('', ACTION_QUICK_4THCAP);
//	SetSingleKey('', ACTION_QUICK_5THCAP);
//	SetSingleKey('', ACTION_QUICK_SUMMON1);
//	SetSingleKey('', ACTION_QUICK_SUMMON2);


	// Window 단축키 설정
	SetSingleKey('C', ACTION_WINDOW_PROFILE);
	SetSingleKey('K', ACTION_WINDOW_SKILL);
	SetSingleKey('J', ACTION_WINDOW_QUEST);
	SetSingleKey('N', ACTION_WINDOW_MINIMAP);
	SetSingleKey('M', ACTION_WINDOW_WORLDMAP);
	SetSingleKey('I', ACTION_WINDOW_MAINCAP);
	SetSingleKey('P', ACTION_WINDOW_PARTY);
	SetSingleKey('G', ACTION_WINDOW_GUILD);
	SetSingleKey('L', ACTION_WINDOW_FRIEND);
	SetSingleKey('O', ACTION_WINDOW_OPTION);
	SetSingleKey('H', ACTION_WINDOW_HELP);
	SetCombineKey(NTL_CTRL_BIT | 'M', ACTION_WINDOW_MAIN);
	SetSingleKey('B', ACTION_WINDOW_RANKBOARD);
	SetSingleKey('.', ACTION_WINDOW_HOIPOIMIX);
	SetSingleKey(',', ACTION_WINDOW_MASCOT);

	// Fixed (unchangeable)
	SetSingleKey(NTL_KEY_SUBTRACT, ACTION_MINIMAP_ZOOMOUT);
	SetSingleKey(NTL_KEY_ADD, ACTION_MINIMAP_ZOOMIN);

	SetSingleKey(NTL_KEY_PRIOR, ACTION_CHAT_PGUP);
	SetSingleKey(NTL_KEY_NEXT, ACTION_CHAT_PGDN);

	SetSingleKey(NTL_KEY_ESCAPE, ACTION_GLOBAL_CANCLE);
	SetSingleKey(NTL_KEY_SNAPSHOT, ACTION_GLOBAL_SNAPSHOT);
	SetSingleKey(NTL_KEY_RETURN, ACTION_GLOBAL_CHAT);

	// Default Map을 만들어놓는다.
	m_pActionMap->CaptureMap( m_pDefaultMap );
}

/**
* \brief Set the key to be used as a single, not a combination.
* \param byKey (BYTE) KEY DATA
* \param wAction (WORD) ACTION ID
* \return eACTIONMAP_SET_RESULT
*/
BYTE CActionMapManager::SetSingleKey(BYTE byKey, WORD wAction )
{
	return m_pActionMap->SetAction( (WORD)byKey, wAction );
}

/**
* \brief Set the key used as the combination key.
* \param wKey (BYTE) KEY DATA
* \param wAction (WORD) ACTION ID
* \return eACTIONMAP_SET_RESULT
*/
BYTE CActionMapManager::SetCombineKey( WORD wKey, WORD wAction ) 
{
	return m_pActionMap->SetAction( wKey, wAction );
}

/**
* \brief Find the Action with the created key.
* \param wKey (WORD) KEY DATA
* \return ACTION ID
*/
WORD CActionMapManager::FindAction( WORD wKey ) 
{
	switch( m_byActionMapMode )
	{
	case ACTIONMAP_MODE_USERSETTING:
		return m_pActionMap->FindAction( wKey );
	case ACTIONMAP_MODE_DEFAULT:
		return m_pDefaultMap->FindAction( wKey );
	default:
		return m_pActionMap->FindAction( wKey );
	}
}

/**
* \brief 액션으로 키를 찾는다.
* \param wAction (WORD) ACTION ID
* \return KEY DATA
*/
WORD CActionMapManager::FindKey( WORD wAction ) 
{
	return m_pActionMap->FindKey( wAction );
}

/**
* \brief Specifies the input mode for the action map.
* \param wAction (WORD) ACTION ID to be the input mode
* \return Success
*/
BOOL CActionMapManager::SetInputAction( WORD wAction ) 
{
	// If the action is a fixed action
	if( IsFixedAction( wAction ) )
	{
		SetLastResult( SET_RESULT_FIXED_ACTION );
		m_wInputAction = ACTION_INVALID;
		return FALSE;
	}

	m_wInputAction = wAction;

	return TRUE;
}

/**
* \brief 마지막 세팅 결과를 기록한다.
* \param byResult (BYTE) eACTIONMAP_SET_RESULT
*/
VOID CActionMapManager::SetLastResult( BYTE byResult ) 
{
	// 혹시나 모른 방어 코드.
	if( (int)m_vecResult.size() > 10 )
		m_vecResult.clear();

	m_vecResult.push_back( byResult );
}

/**
* \brief 마지막 세팅 결과를 읽는다.
* \return eACTIONMAP_SET_RESULT
*/
BYTE CActionMapManager::GetLastResult() 
{
	if( m_vecResult.empty() )
		return SET_RESULT_INVALID;

	BYTE byLastResult = m_vecResult.back();
	m_vecResult.pop_back();

	return byLastResult;
}

/**
* \brief 에러 코드를 클리어한다.
*/
VOID CActionMapManager::ClearResult() 
{
	m_vecResult.clear();
}

/**
* \brief 액션 ID로 키의 이름을 찾아온다.
* \param usAction
* \return 키의 이름(유니코드)
*/
std::wstring CActionMapManager::GetKeyName( WORD wAction ) 
{
	uNtlKey uKey;
	memset( &uKey, 0, sizeof(uNtlKey) );

	// Default 일 경우
	if( m_byActionMapMode == ACTIONMAP_MODE_USERSETTING )
	{
		uKey.key = m_pActionMap->FindKey( wAction );
	}
	else if( m_byActionMapMode == ACTIONMAP_MODE_DEFAULT )
	{
		uKey.key = m_pDefaultMap->FindKey( wAction );
	}

	std::wstring wstrKeyName;

	// 키가 정의가 안되어 있으면 공백을 리턴
	if( uKey.key == dNTL_KEY_EMPTY )
		return wstrKeyName;

	// 조합키가 들어 가있다면 조합키의 이름을 넣어준다.
	if( uKey.BIT.bit8 == 0x1 )	// ALT
	{
		wstrKeyName += L"@";
	}
	if(	uKey.BIT.bit7 == 0x1 )	// CTRL
	{
		wstrKeyName += L"^";
	}

	// 키가 있다면 키의 이름을 더하고 아니라면 등록되지 않은 키 이름이다.
	if( m_mapKeyName.end() != m_mapKeyName.find( uKey.BIT.ori ) )
	{
		wstrKeyName += m_mapKeyName[uKey.BIT.ori];
	}
	else
	{
		wstrKeyName += L"UNKNOWN";
	}

	return wstrKeyName.c_str();
	
}

/**
* \brief Input Handler
* \param byKey (BYTE) Key Data
* \return ACTION ID
*/
WORD CActionMapManager::InputHandlerDown( BYTE byKey ) 
{
	// 액션맵의 매니저가 Input Mode라면 액션에 키를 대입한다.
	if( m_wInputAction != ACTION_INVALID )
	{
		//// 액션맵 매니저가 bInput이 TRUE일때만 키를 대입
		//if( bInput )
		//{
		//	// 조합키라면 그냥 리턴
		//	if( IsCombineKey( byKey ) )
		//		return ACTION_INVALID;

		//	// 주어진 키로 WORD형의 NTL Key Data를 만들고.
		//	WORD wMakeKey = MakeKey( byKey );

		//	// 키가 고정되어 있다면 에러 코드를 기록하고 리턴
		//	if( IsFixedKey( wMakeKey ) )
		//	{
		//		SetLastResult( SET_RESULT_FIXED_KEY );
		//		m_wInputAction = ACTION_INVALID;
		//		return ACTION_INVALID;
		//	}

		//	// 조합키를 사용하지 못하는 액션인데 조합키로 셋팅하려고 했다면
		//	if( IsExclusiveAction( m_wInputAction ) && wMakeKey > 0x00FF )
		//	{
		//		SetLastResult( SET_RESULT_NOT_COMBINE_ACTION );
		//		m_wInputAction = ACTION_INVALID;
		//		return ACTION_INVALID;
		//	}

		//	// SetAction
		//	SetLastResult( m_pActionMap->SetAction( wMakeKey, m_wInputAction ) );

		//	m_wInputAction = ACTION_INVALID;
		//}
		
		return ACTION_INVALID;
	}

	// NTL의 Key를 만든다.
	WORD wMakeKey = MakeKey( byKey );

	// 현재의 Key 중의 Original을 추출
	WORD wReturnAction = ACTION_INVALID;
	WORD wFindOriginalKeyAction = FindAction( wMakeKey & 0x00FF );	
	if( IsExclusiveAction( wFindOriginalKeyAction ) )
	{
		wReturnAction = wFindOriginalKeyAction;
	}
	else
	{
		wReturnAction = FindAction( wMakeKey );
	}

	InputActionDown( wReturnAction );

	// 아니라면 조합된 키의 액션을 찾아서 리턴해준다.
	return wReturnAction;
}

WORD CActionMapManager::InputHandlerUp( BYTE byKey, std::list< WORD >& listUpAction )
{
	// 액션맵의 매니저가 Input Mode라면 액션에 키를 대입한다.
	if( m_wInputAction != ACTION_INVALID )
	{
		// 조합키라면 그냥 리턴
		if( IsCombineKey( byKey ) )
			return ACTION_INVALID;

		// 주어진 키로 WORD형의 NTL Key Data를 만들고.
		WORD wMakeKey = MakeKey( byKey );

		// 키가 고정되어 있다면 에러 코드를 기록하고 리턴
		if( IsFixedKey( wMakeKey ) )
		{
			SetLastResult( SET_RESULT_FIXED_KEY );
			m_wInputAction = ACTION_INVALID;
			return ACTION_INVALID;
		}

		// 조합키를 사용하지 못하는 액션인데 조합키로 셋팅하려고 했다면
		if( IsExclusiveAction( m_wInputAction ) && wMakeKey > 0x00FF )
		{
			SetLastResult( SET_RESULT_NOT_COMBINE_ACTION );
			m_wInputAction = ACTION_INVALID;
			return ACTION_INVALID;
		}

		// SetAction
		SetLastResult( m_pActionMap->SetAction( wMakeKey, m_wInputAction ) );

		m_wInputAction = ACTION_INVALID;

		return ACTION_INVALID;
	}

	// If the currently pressed key is a combination key
	if( IsCombineKey( byKey ) )
	{
		// You must raise all the pushed actions that are triggered by the current combination key.
		CheckDownActionWithCombineKey( byKey, listUpAction );

		return ACTION_INVALID;
	}
	else
	{
		// NTL의 Key를 만든다.
		WORD wMakeKey = MakeKey( byKey );

		// 현재의 Key 중의 Original을 추출
		WORD wReturnAction = ACTION_INVALID;
		WORD wFindOriginalKeyAction = FindAction( wMakeKey & 0x00FF );	
		if( IsExclusiveAction( wFindOriginalKeyAction ) )
		{
			wReturnAction = wFindOriginalKeyAction;
		}
		else
		{
			wReturnAction = FindAction( wMakeKey );
		}

		InputActionUp( wReturnAction );

		// If not, it finds and returns the action of the combined key.
		return wReturnAction;
	}
}

void CActionMapManager::InputActionDown( WORD wAction )
{
	if( wAction == ACTION_INVALID )
		return;

	mapdef_InputAction::iterator it = m_mapInputAction.find( wAction );

	// 못찾음
	if( it == m_mapInputAction.end() )
	{
		m_mapInputAction[wAction] = 1;
	}
	else
	{
		(*it).second++;
	}
}

void CActionMapManager::InputActionUp( WORD wAction )
{
	if( wAction == ACTION_INVALID )
		return;

	mapdef_InputAction::iterator it = m_mapInputAction.find( wAction );

	// 못찾음
	if( it != m_mapInputAction.end() )
	{
		(*it).second--;
		if( (*it).second <= 0 )
		{
			m_mapInputAction.erase( it );
		}
	}
}

void CActionMapManager::CheckDownActionWithCombineKey( BYTE byKey, std::list< WORD >& listUpAction )
{
	mapdef_InputAction::iterator it = m_mapInputAction.begin();
	while( it != m_mapInputAction.end() )
	{
		WORD wKey = m_pActionMap->FindKey( (*it).first );
		if( wKey != dNTL_KEY_INVALID )
		{
			if( wKey & byKey )
			{
				listUpAction.push_back( (*it).first );
				it = m_mapInputAction.erase( it );
			}
			else
			{
				it++;
			}
		}
	}
}

/**
* \brief 액션맵의 복사본을 만들어 놓는다.
*/
VOID CActionMapManager::CaptureActionMap() 
{
	m_pActionMap->CaptureMap( m_pCaptureMap );
}

/**
* \brief 만들어둔 복사본으로 다시 돌린다.
*/
VOID CActionMapManager::RestoreActionMap() 
{
	m_pActionMap->RestoreMap( m_pCaptureMap );
}

/**
* \brief UpdateData를 추가한다.
* \param byType		(BYTE) 0: ADD, 1: DEL, 2: UPDATE
* \param wActionID	(WORD) ACTION ID
* \param wKey		(WORD) KEY DATA
*/
void CActionMapManager::AddUpdateData( BYTE byType, WORD wActionID, WORD wKey ) 
{
	m_asUpdateData[m_byUpdateCount].byType = byType;
	m_asUpdateData[m_byUpdateCount].wActionID = wActionID;
	m_asUpdateData[m_byUpdateCount].wKey = wKey;

	++m_byUpdateCount;
}

/**
* \breif UpdateData를 가져온다.
* \param pData		(sSHORTCUT_UPDATE_DATA*)
* \param byCount	(BYTE&)
* \return
*/
RwBool CActionMapManager::GetUpdateData( sSHORTCUT_UPDATE_DATA* pData , BYTE& byCount ) 
{
	if( m_pActionMap->CheckMap( m_pCaptureMap->GetKeyMap() ) )
	{
		byCount = m_byUpdateCount;
		memcpy( pData, m_asUpdateData, sizeof(sSHORTCUT_UPDATE_DATA) * m_byUpdateCount);
		
		// Clear Update Data
		m_byUpdateCount = 0;
		memset( m_asUpdateData, 0, sizeof(sSHORTCUT_UPDATE_DATA) * NTL_SHORTCUT_MAX_COUNT);
		return TRUE;
	}
	return FALSE;
}

/**
* \brief 키를 NTL에서 사용되는 형식으로 만든다.
* \param byKey	(BYTE) KEY DATA
* \return NTL KEY
*/
WORD CActionMapManager::MakeKey( BYTE byKey ) 
{
	uNtlKey uMakeKey;
	memset( &uMakeKey, 0, sizeof( uNtlKey ) );

	// 현재의 조합키를 셋팅
	MakeComposition( uMakeKey );

	// 키를 셋팅
	uMakeKey.BIT.ori = byKey;

	return uMakeKey.key;
}

/**
* \brief 조합키를 설정한다.
* \param uMakeKey	(uNtlKey&) NtlActionMapDef.h에 선언된 공용체
*/
void CActionMapManager::MakeComposition( uNtlKey& uMakeKey ) 
{
	// 조합키를 확인
	if( ( GetKeyState( VK_LMENU ) & 0x80 ) || ( GetKeyState( VK_RMENU ) & 0x80 ) )
		uMakeKey.BIT.bit8 = 0x1;

	if( ( GetKeyState( VK_LCONTROL ) & 0x80 ) || ( GetKeyState( VK_RCONTROL ) & 0x80 ) )
		uMakeKey.BIT.bit7 = 0x1;
}

/**
* \brief 각종 키의 이름을 하드 코딩 해놓는다.
*/
VOID CActionMapManager::RegisterKeyName( VOID ) 
{
	m_mapKeyName[NTL_KEY_BACKSPACE] = L"BackSP";
	m_mapKeyName[NTL_KEY_TAB] = L"TAB";
	m_mapKeyName[NTL_KEY_RETURN] = L"ENTER";

	m_mapKeyName[NTL_KEY_NUMPAD0] = L"NUM 0";
	m_mapKeyName[NTL_KEY_NUMPAD1] = L"NUM 1";
	m_mapKeyName[NTL_KEY_NUMPAD2] = L"NUM 2";
	m_mapKeyName[NTL_KEY_NUMPAD3] = L"NUM 3";
	m_mapKeyName[NTL_KEY_NUMPAD4] = L"NUM 4";
	m_mapKeyName[NTL_KEY_NUMPAD5] = L"NUM 5";
	m_mapKeyName[NTL_KEY_NUMPAD6] = L"NUM 6";
	m_mapKeyName[NTL_KEY_NUMPAD7] = L"NUM 7";
	m_mapKeyName[NTL_KEY_NUMPAD8] = L"NUM 8";
	m_mapKeyName[NTL_KEY_NUMPAD9] = L"NUM 9";
	m_mapKeyName[NTL_KEY_MULTIPLY] = L"NUM *";
	m_mapKeyName[NTL_KEY_ADD] = L"NUM +";
	m_mapKeyName[NTL_KEY_SUBTRACT] = L"NUM -";
	m_mapKeyName[NTL_KEY_DECIMAL] = L"NUM .";
	m_mapKeyName[NTL_KEY_DIVIDE] = L"NUM /";

	m_mapKeyName[NTL_KEY_F1] = L"F1";
	m_mapKeyName[NTL_KEY_F2] = L"F2";
	m_mapKeyName[NTL_KEY_F3] = L"F3";
	m_mapKeyName[NTL_KEY_F4] = L"F4";
	m_mapKeyName[NTL_KEY_F5] = L"F5";
	m_mapKeyName[NTL_KEY_F6] = L"F6";
	m_mapKeyName[NTL_KEY_F7] = L"F7";
	m_mapKeyName[NTL_KEY_F8] = L"F8";
	m_mapKeyName[NTL_KEY_F9] = L"F9";
	m_mapKeyName[NTL_KEY_F10] = L"F10";
	m_mapKeyName[NTL_KEY_F11] = L"F11";
	m_mapKeyName[NTL_KEY_F12] = L"F12";

	m_mapKeyName[NTL_KEY_PAUSE] = L"PAUSE";
	m_mapKeyName[NTL_KEY_TOCHINESE] = L"CHINESE";
	m_mapKeyName[NTL_KEY_ESCAPE] = L"ESC";
	m_mapKeyName[NTL_KEY_SPACE] = L"SPACE";
	m_mapKeyName[NTL_KEY_SHIFT] = L"SHIFT";

	m_mapKeyName[NTL_KEY_PRIOR] = L"PGUP";
	m_mapKeyName[NTL_KEY_NEXT] = L"PGDN";
	m_mapKeyName[NTL_KEY_END] = L"END";
	m_mapKeyName[NTL_KEY_HOME] = L"HOME";
	m_mapKeyName[NTL_KEY_INSERT] = L"INS";
	m_mapKeyName[NTL_KEY_DELETE] = L"DEL";	

	m_mapKeyName[NTL_KEY_LEFT] = L"◀";
	m_mapKeyName[NTL_KEY_UP] = L"▲";
	m_mapKeyName[NTL_KEY_RIGHT] = L"▶";
	m_mapKeyName[NTL_KEY_DOWN] = L"▼";

	m_mapKeyName[NTL_KEY_SELECT] = L"SELECT";
	m_mapKeyName[NTL_KEY_PRINT] = L"PRINT";
	m_mapKeyName[NTL_KEY_EXECUTE] = L"EXECUTE";
	m_mapKeyName[NTL_KEY_SNAPSHOT] = L"PRC/SRQ";
	m_mapKeyName[NTL_KEY_HELP] = L"HELP";
	m_mapKeyName[NTL_KEY_LEFTWINDOW] = L"LEFT WIN";
	m_mapKeyName[NTL_KEY_RIGHTWINDOW] = L"RIGHT WIN";

	m_mapKeyName[NTL_KEY_CAPITAL] = L"CAPS LOCK";
	m_mapKeyName[NTL_KEY_NUMLOCK] = L"NUMS LOCK";
	m_mapKeyName[NTL_KEY_SCROLL] = L"SCR LOCK";

	m_mapKeyName[NTL_KEY_DASH] = L"-";
	m_mapKeyName[NTL_KEY_EQUAL] = L"=";
	m_mapKeyName[NTL_KEY_WAVE] = L"~";

	m_mapKeyName[NTL_KEY_LBRACKET] = L"[";
	m_mapKeyName[NTL_KEY_RBRACKET] = L"]";

	m_mapKeyName[NTL_KEY_PERIOD] = L".";
	m_mapKeyName[NTL_KEY_COMMA] = L",";
	m_mapKeyName[NTL_KEY_OEM_2] = L"/";

	// 일반키 A~Z , 0 ~ 9
	WCHAR awcBuffer[32] = {0,};
	for( unsigned short uiChar = 0x41 ; uiChar <= 0x5A; ++uiChar)
	{
		swprintf_s( awcBuffer, 32, L"%c", uiChar);
		m_mapKeyName[uiChar] = awcBuffer;
	}

	for( unsigned short uiChar = 0x30 ; uiChar <= 0x39; ++uiChar )
	{
		swprintf_s( awcBuffer, 32, L"%c", uiChar);
		m_mapKeyName[uiChar] = awcBuffer;
	}

	// 지정안됨
	m_mapKeyName[NTL_KEY_EMPTY] = L" ";	
	
}

/**
* \brief 키의 속성을 정의한다.
*/
VOID CActionMapManager::RegisterKeyType( VOID ) 
{
	m_mapKeyType[NTL_KEY_SUBTRACT]	= TYPE_FIXED;	// NUM -
	m_mapKeyType[NTL_KEY_ADD]		= TYPE_FIXED;	// NUM +
	m_mapKeyType[NTL_KEY_PRIOR]		= TYPE_FIXED;	// PG UP
	m_mapKeyType[NTL_KEY_NEXT]		= TYPE_FIXED;	// PG DN
	m_mapKeyType[NTL_KEY_ESCAPE]	= TYPE_FIXED;	// ESC
	m_mapKeyType[NTL_KEY_SNAPSHOT]	= TYPE_FIXED;	// PRC/SRQ
	m_mapKeyType[NTL_KEY_RETURN]	= TYPE_FIXED;	// ENTER

//	m_mapKeyType[NTL_KEY_CONTROL]	= TYPE_COMBINE;	// CTRL	// disabled by daneos. If this is enabled, player stops moving when release CTRL
//	m_mapKeyType[NTL_KEY_MENU]		= TYPE_COMBINE; // ALT	// disabled by daneos. If this is enabled, player stops moving when release ALT
}

/**
* \brief 액션의 속성을 정의한다.
*/
VOID CActionMapManager::RegisterActionType( VOID ) 
{
	m_mapActionType[ACTION_MINIMAP_ZOOMOUT]		= TYPE_FIXED;
	m_mapActionType[ACTION_MINIMAP_ZOOMIN]		= TYPE_FIXED;
	m_mapActionType[ACTION_CHAT_PGUP]			= TYPE_FIXED;
	m_mapActionType[ACTION_CHAT_PGDN]			= TYPE_FIXED;
	m_mapActionType[ACTION_GLOBAL_CANCLE]		= TYPE_FIXED;
	m_mapActionType[ACTION_GLOBAL_SNAPSHOT]		= TYPE_FIXED;
	m_mapActionType[ACTION_GLOBAL_CHAT]			= TYPE_FIXED;

	// 이동 관련 액션은 다른 액션들과 배제되어야 
	m_mapActionType[ACTION_AVATAR_FORWARD]		= TYPE_EXCLUSIVE;
	m_mapActionType[ACTION_AVATAR_BACKWARD]		= TYPE_EXCLUSIVE;
	m_mapActionType[ACTION_AVATAR_LEFTTURN]		= TYPE_EXCLUSIVE;
	m_mapActionType[ACTION_AVATAR_RIGHTTURN]	= TYPE_EXCLUSIVE;
	m_mapActionType[ACTION_AVATAR_LEFTSIDE]		= TYPE_EXCLUSIVE;
	m_mapActionType[ACTION_AVATAR_RIGHTSIDE]	= TYPE_EXCLUSIVE;

	// 기 모으기 액션 역시 다른 액션들과의 배제
	m_mapActionType[ACTION_AVATAR_CHARGE]		= TYPE_EXCLUSIVE;
}

/**
* \brief 지정된 맵에서 속성이 정의되어 있는지 확인한다.
* \param nType		(int) NtlActionMapDef.h에 정의된 속성
* \param wKeyType	(WORD) 액션인지 키인지 맵에 정의된 Key Type
* \param mapType	(TYPEMAP&) 맵을 넘겨준다.
* \return (BOOL) 정의되어 있는지의 여부
*/
BOOL CActionMapManager::IsMapType( int nType, WORD wKeyType, TYPEMAP& mapType ) 
{
	TYPEMAP::iterator it = mapType.find( wKeyType );
	if( mapType.end() == it )
		return FALSE;

	return (*it).second & nType;
}

VOID CActionMapManager::ClearActionMap() 
{
	m_pActionMap->Clear();
	m_pCaptureMap->Clear();
	ClearResult();
	ClearInputAction();
	ClearReleaseAction();
}

/**
* \brief ActionMap이 사용되는 Mode를 지정한다.
*/
VOID CActionMapManager::SetActionMapMode( BYTE byActionMapMode )
{
	if( byActionMapMode >= ACTIONMAP_MODE_INVALID )
		assert( 0 && "Not exist actionmap mode of eACTIONMAP_MODE(enum)" );

	if( m_byActionMapMode != byActionMapMode )
	{
		m_byActionMapMode = byActionMapMode;
		CNtlSLEventGenerator::ActionMapClientNotify( SNtlEventActionMapClientNotify::ACTIONMAP_OK );
	}
}

void CActionMapManager::InputClear()
{
	m_mapInputAction.clear();
}

