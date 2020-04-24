/*****************************************************************************
 *
 * File			: Global.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 9. 14	
 * Abstract		: DBO global.h
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __DBO_GLOBAL_H__
#define __DBO_GLOBAL_H__

#include "set"
#include "NtlClientNet.h"
#include "NtlItem.h"
#include "NtlService.h"
#include "NtlPacketCU.h"
#include "NtlPacketGU.h"
#include "DboPacketHandler.h"
#include "DboPacketGenerator.h"
#include "NtlSLDef.h"
#include "NtlSlEvent.h"
#include "DBOUIConfig.h"

// ChattingFilter
#include "SayFilter.h"

static DWORD sm_cameraMoveCount = 0;

typedef void (*CallbackLoadContract)(const RwChar* pFileName, void** pData, int* iSize);
extern CallbackLoadContract g_fnCallback_LoadContract;

void LinkLoadContract(CallbackLoadContract fn);
void UnLinkLoadContract();


class CNtlClientNet;
class CNtlSobAvatar;

#define dMAX_SIZE_DEV_FUNC_ARRAY							(DBO_ALLOWED_FUNC_FOR_DEV_COUNT/8 + 1)

// config data
struct SConfigData
{
	std::string strAddr;		// account server ip address(해킹의 문제로 하드코딩을 고려한다?)
	DWORD dwPort;				// account server ip port(해킹의 문제로 하드코딩을 고려한다?)
    std::string strBugTrapServerIP;         
    DWORD       dwBugTrapServerPort;
    std::string strBugTrapFolder;
	std::string strLocalize;

    SConfigData()
    {
        strBugTrapFolder = "DBOG Client";
    }
};

typedef struct _SConnectAtomic
{
	enum 
	{
		SOCKET_STATE_DISCONNECT,
		SOCKET_STATE_CONNECT,
		SOCKET_STATE_DISCONNECT_REQ
	};

	HSERVER		hSocket;
	RwUInt8		byConnState;
	RwChar		chServerIP[NTL_MAX_LENGTH_OF_IP+1];
	RwUInt16	wServerPort;
	RwChar		chAuthKey[NTL_MAX_SIZE_AUTH_KEY+1];
	RwBool		bBanishment;	///< 운영상의 제재를 당했는지 여부

	void Init(void)
	{
		hSocket			= INVALID_HSERVER;
		byConnState		= SOCKET_STATE_DISCONNECT;
		memset(chServerIP, 0, sizeof(RwChar)*(NTL_MAX_LENGTH_OF_IP+1));
		wServerPort		= 0;
		memset(chAuthKey, 0, sizeof(RwChar)*(NTL_MAX_SIZE_AUTH_KEY+1));
		bBanishment		= false;
	}

}SConnectAtomic;


typedef struct _SCharServerInfo
{
	RwChar		chServerIP[NTL_MAX_LENGTH_OF_IP + 1];
	RwUInt16	wServerPort;
	RwUInt32	uiLoad;
}SCharServerInfo;

// network connect info
typedef struct _SConnectData
{
	SConnectAtomic sAuthCon;
	SConnectAtomic sCharCon;
	SConnectAtomic sGameCon;
	SConnectAtomic sChatCon;

	// character server 종류.
	typedef std::list<SCharServerInfo*> ListCharServer;
	ListCharServer listCharServer;

	//uuid
	RwUInt32	uiAccountId;
	RwUInt8		byLastServerFarmID;

	RwBool		bChangingGameServer;

	// Dev func 정보
	RwChar			m_acAllowFuncForDev[dMAX_SIZE_DEV_FUNC_ARRAY];
	
	void			Init(void);
	void			Destroy(void);

	void			ResetCharacterServer(void);
	void			AddCharacterServer(const RwChar *pServerIP, RwUInt16 wServerPort, RwUInt32 uiLoad);
	RwBool			PopCharacterServer(SCharServerInfo *pInfo);

}SConnectData;


// user info
typedef struct _SUserData
{
	WCHAR wchUserID[NTL_MAX_SIZE_USERID+1];			// user id
	WCHAR wchPassword[NTL_MAX_SIZE_USERPW+1];		// user password

	void Init(void)
	{
		Clear();
	}

	void Clear(void)
	{
		memset(wchUserID, 0, sizeof(WCHAR)*(NTL_MAX_SIZE_USERID+1));
		memset(wchPassword, 0, sizeof(WCHAR)*(NTL_MAX_SIZE_USERPW+1));
	}

}SUserData;

// game data
typedef struct _SGameData
{
	RwBool			bChatMode;
	RwBool			bFirstWorldEnter;			///< 처음으로 world에 진입하는가?
	RwBool			bInputActive;		
	RwBool			bDebugDisplay;
	SERIAL_HANDLE	hAskedPCSerial;				///< 뭔가를 질문해온 PC의 핸들
    SERIAL_HANDLE   hCommunityTargetSerial;     ///< 커뮤니티 단계에서 선택(타겟팅)한 PC의 핸들
	RwBool			bAvatarReCreate;			///< 다른 물리적 game server로 진입할 때 avatar를 삭제하고 다시 생성한다.

	void Init(void)
	{
		bChatMode			= FALSE;
		bFirstWorldEnter	= TRUE;
		bInputActive		= TRUE;
		bDebugDisplay = TRUE; // FALSE;
		hAskedPCSerial		= INVALID_SERIAL_ID;
        hCommunityTargetSerial = INVALID_SERIAL_ID;
		bAvatarReCreate		= FALSE;
	}

}SGameData;

struct sContractInfo
{
	char*			pcContract;
	WCHAR*			pwcContract;
	WCHAR*			pwcRealContract;
	RwInt32			uiContractVersion;
	RwBool			bAcceptGameContract;			///< 약관 수락 여부
};


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

class CDboGlobal
{
private:

	static CDboGlobal *m_pInstance;

private:

	/**
	* screen size 변수.
	*/
	RwUInt32		m_uiScreenWidth;				/** screen width */
	RwUInt32		m_uiScreenHeight;				/** screen height */

	/**
	* game lobby 간략 정보
	*/
	RwUInt8			m_byPostSelectedServerHandle;	/** 이전에 선택된 서버의 핸들 */
	RwUInt8			m_bySelectedServerHandle;		/** 현재 선택된 서버의 핸들 */
	RwUInt8			m_bySelectedCharIndex;			/** 선택된 캐릭터 인덱스 */

	/**
	* game data  처리 변수.
	*/
	SUserData			m_sUser;					/** dbo user data */
	SConfigData			m_sConfig;					/** dbo config data */	
	SConnectData		m_sConnect;					/** dbo network connect data */		
	SGameData			m_sGame;					/** dbo game data */

	// 드래곤볼 컬렉션 기간 정보 구조체
	SNtlEventDBCSchedule_Info m_sDBCSheduleInfo;
	    
	/**
	* network lib 변수.
	*/
	CNtlClientNet *m_pNetwork;						/** network lib */

	/**
	* packet 처리 변수.
	*/

	CNtlNetSender *m_pLoginNetSender;					/** login network sender */
	CNtlNetSender *m_pLobbyNetSender;					/** lobby network sender */
	CNtlNetSender *m_pGameNetSender;					/** game network sender */
	CNtlNetSender *m_pChatNetSender;					/** chatting network sender */	

	CLoginPacketGenerator *m_pLoginPacketGenerator;		/** login packet generator */
	CLobbyPacketGenerator *m_pLobbyPacketGenerator;		/** lobby packet generator */
	CGamePacketGenerator *m_pGamePacketGenerator;		/** game packet generator */
	CChatPacketGenerator *m_pChatPacketGenerator;		/** chatting packet generator */

    CDBOUIConfig*   m_pDBOUIConfig;                     ///< DBO UI Config 정보 객체 (2007.11.15 by agebreak)

	sContractInfo*	m_pContractInfo;
	RwBool			m_bEnterTutorial;					/** 튜토리얼 진입 인터페이스에서 취소 버튼을 눌러 게임에 진입시 */
	RwBool			m_bDirectionFirstConnect;			/** 처음 접속해서 연출을 보여줄 것인지 여부 */

private:

	RwBool LoadConfigOptions(const RwChar *pFileName);

public:

	CDboGlobal();
	~CDboGlobal();

	static CDboGlobal* GetInstance(void);

	RwBool	Create(HWND hWnd);
	void	Destroy(void);

	void	Reset(void);		

public:

	void			SetScreenSize(RwUInt32 uiWidth, RwUInt32 uiHeight) { m_uiScreenWidth = uiWidth, m_uiScreenHeight = uiHeight; } 
	RwUInt32		GetScreenWidth(void) { return m_uiScreenWidth; }
	RwUInt32		GetScreenHeight(void) { return m_uiScreenHeight; }
	
	void			LoadingContractInfo();
			
	/**
	* game data  처리 함수.
	*/
	SUserData*		GetUserData(void)		{ return &m_sUser; }
	SConfigData*	GetConfigData(void)		{ return &m_sConfig; }
	SConnectData*	GetConnectData(void)	{ return &m_sConnect; }	
	SGameData*		GetGameData(void)		{ return &m_sGame; }

	// 드래곤볼 컬렉션 정보 처리 함수
	void			SetDBCScheduleInfo(SNtlEventDBCSchedule_Info sDBCScheduleInfo) {m_sDBCSheduleInfo = sDBCScheduleInfo;}
	SNtlEventDBCSchedule_Info* GetDBCScheduleInfo() {return &m_sDBCSheduleInfo;}
	
	/**
	* packet 처리 함수.
	*/
	CNtlClientNet*			GetNetwork(void)					{ return m_pNetwork; }
	CLoginPacketGenerator*	GetLoginPacketGenerator(void)		{ return m_pLoginPacketGenerator; }
	CLobbyPacketGenerator*	GetLobbyPacketGenerator(void)		{ return m_pLobbyPacketGenerator; }
	CGamePacketGenerator*	GetGamePacketGenerator(void)		{ return m_pGamePacketGenerator; }
	CChatPacketGenerator*	GetChatPacketGenerator(void)		{ return m_pChatPacketGenerator; }

	/**
	* 
	*/
	
	void            SetCommunityTarget(SERIAL_HANDLE hSerial)   { m_sGame.hCommunityTargetSerial = hSerial;}
	SERIAL_HANDLE   GetCommunityTarget()                        { return m_sGame.hCommunityTargetSerial;}

	void			SetAskedPCSerial( SERIAL_HANDLE hSerial )	{ m_sGame.hAskedPCSerial = hSerial; }
	SERIAL_HANDLE	GetAskedPCSerial(void)						{ return m_sGame.hAskedPCSerial; }
	
	/**
	* debug display
	*/
	void			ToggleDebugDisplay( void )					{ m_sGame.bDebugDisplay = !m_sGame.bDebugDisplay; }
	RwBool			IsDebugDisplay(void)						{ return m_sGame.bDebugDisplay; }

    CDBOUIConfig*   GetDBOUIConfig()                            {return m_pDBOUIConfig;}        ///< DBO UI Config 객체를 반환한다.
	
	void			SetEnterTutorial( RwBool bEnter )			{ m_bEnterTutorial = bEnter; }
	RwBool			IsEnterTutorial()							{ return m_bEnterTutorial; }

	void			SetDirectionFirstConnect(RwBool bDirection) { m_bDirectionFirstConnect = bDirection; }
	RwBool			IsDirectionFirstConnect()					{ return m_bDirectionFirstConnect; }

	void			SetGameContract(RwBool bAccept);
	RwBool			IsAcceptGameContract();
	RwUInt32		GetContractVerstion();
	WCHAR*			GetContract();

	RwBool			HaveFunction(eDBO_ALLOWED_FUNC_FOR_DEV eAllowFunc);
};

static CDboGlobal* GetDboGlobal(void)
{
	return CDboGlobal::GetInstance(); 
}

#endif

