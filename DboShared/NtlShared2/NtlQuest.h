//***********************************************************************************
//
//	File		:	NtlQuest.h
//
//	Begin		:	2006-10-31
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlTSCore.h"
#include "NtlSharedType.h"
#include "NtlBitFlag.h"


//===================================================================================
//	Quest ID
//===================================================================================

// Define Usage Quest ID
enum eQUEST_ID_RANGE
{
	// General quest is should be used within the same category ID following a common quest.
	// If being extended by the Reserve quest range later expanded upon request.
	eQUEST_ID_RANGE_GENERAL_QUEST_MIN		= 0,
	eQUEST_ID_RANGE_GENERAL_QUEST_MAX		= 7000,
	eQUEST_ID_RANGE_GENERAL_QUEST_COUNT		= eQUEST_ID_RANGE_GENERAL_QUEST_MAX - eQUEST_ID_RANGE_GENERAL_QUEST_MIN,

	// Reserve quest assumption that is required in planning future expansion of the General quest
	// Placing a reservation as follows:
	eQUEST_ID_RANGE_RESERVE_QUEST_MIN		= 7000,
	eQUEST_ID_RANGE_RESERVE_QUEST_MAX		= 11000,
	eQUEST_ID_RANGE_RESERVE_QUEST_COUNT		= eQUEST_ID_RANGE_RESERVE_QUEST_MAX - eQUEST_ID_RANGE_RESERVE_QUEST_MIN,

	// Special quest is Tutorial, as a quest for identity in a particular system, such as TMQ
	// Used only in the quest to be automatically deleted when the system is shut down certain.
	eQUEST_ID_RANGE_SPECIAL_QUEST_MIN		= 11000,
	eQUEST_ID_RANGE_SPECIAL_QUEST_MAX		= 14000,
	eQUEST_ID_RANGE_SPECIAL_QUEST_COUNT		= eQUEST_ID_RANGE_SPECIAL_QUEST_MAX - eQUEST_ID_RANGE_SPECIAL_QUEST_MIN,

	eQUEST_ID_RANGE_QUEST_MAX				= 14000,
};


//===================================================================================
//	완료된 퀘스트 정보
//===================================================================================

//	퀘스트 완료 상태
//	주의) 아래에 상태 추가 필요성 발생 시 반드시
//		  퀘스트 담당자 ( lleo52@ntl-inc.com )에게 연락 바람
enum eCOMPLETE_QUEST_STATE
{
	eCOMPLETE_QUEST_STATE_CLEAR				= 0x1,

	eCOMPLETE_QUEST_STATE_MAX				= 1
};

enum
{
	eCOMPLETE_QUEST_QUEST_PER_BYTE			= 8 / eCOMPLETE_QUEST_STATE_MAX
};

enum
{
	eCOMPLETE_QUEST_QUEST_PER_STRUCT		= 1000
};

enum
{
	eCOMPLETE_QUEST_QUEST_STRUCT_COUNT		= eQUEST_ID_RANGE_QUEST_MAX / eCOMPLETE_QUEST_QUEST_PER_STRUCT
};

#pragma pack( 1 )
struct sCOMPLETE_QUEST_INFO
{
	unsigned char							abyQCInfo[eQUEST_ID_RANGE_QUEST_MAX/eCOMPLETE_QUEST_QUEST_PER_BYTE];
};


struct sCOMPLETE_QUEST_BIT_INFO
{
	unsigned char							abyQCBitInfo[eCOMPLETE_QUEST_QUEST_PER_STRUCT/eCOMPLETE_QUEST_QUEST_PER_BYTE];
};

#pragma pack()


//===================================================================================
//	진행 중인 퀘스트 정보
//===================================================================================

// 진행 가능한 최대 퀘스트 개수
enum
{
	eMAX_CAN_PROGRESS_QUEST_NUM				= 30, //

	eMAX_CAN_PROGRESS_GENERAL_QUEST_NUM		= 20,	// 일반적인 퀘스트 
	eMAX_CAN_PROGRESS_SPECIAL_QUEST_NUM		= 10,	// TMQ, Tutorial... 용 퀘스트
};

// The maximum number of possible triggers progression
enum 
{
	eMAX_CAN_PROGRESS_TRIGGER_NUM			= 20
};

// Status of the ongoing quest
// Week) that this space is reserved for storing the current state to the 2 Byte.
//	   Not to exceed an additional condition under the care needed during 2Byte.
enum eTS_SVR_STATE
{
	eTS_SVR_STATE_ERROR						= 0x0001,		// Error state
	eTS_SVR_STATE_FAILED					= 0x0002,		// Failed state Quest progress
	eTS_SVR_STATE_ESCORT					= 0x0004,		// Escort state
	eTS_SVR_STATE_SVR_EVT_WORLD_RANGE		= 0x0008,		// This applies in the server event (mopkil ...) World (If this flag is not turned on, handled by party standards)
	eTS_SVR_STATE_WPS_MODE					= 0x0010,		// World play script The action of the operation
	eTS_SVR_STATE_USER_OUT_FAILED_WPS		= 0x0020,		// World play script When a user logs out of the operation flag to store the case to be treated Failed
};

#pragma pack( 1 )
struct sMAIN_TSP
{
	NTL_TS_TC_ID							tcCurId;
	NTL_TS_TC_ID							tcPreId;
};
#pragma pack()

#pragma pack( 1 )
struct sEXCEPT_TIMER
{
	NTL_TS_TC_ID							tcId;
	NTL_TS_TA_ID							taId;

	unsigned int							uiRemainTime;
};
#pragma pack()


#pragma pack( 1 )
struct sEXCEPT_TIMER_SLOT
{
	enum { eTIMER_SLOT_MAX = 4 };

	sEXCEPT_TIMER							asExceptTimer[eTIMER_SLOT_MAX];
};
#pragma pack()

#pragma pack( 1 )
struct sSTORAGE_SLOT_MEMORY
{
	enum { eSTORAGE_SLOT_MEMORY_MAX = 8 };

	// The index of the array is used as ID of SSM
	unsigned int							auiSSM[eSTORAGE_SLOT_MEMORY_MAX];
};
#pragma pack()

#pragma pack( 1 )
struct sSTOC_EVT_DB_DATA
{
	enum { eSTOC_EVT_DB_DATA_USER_DATA_MAX = 4 };

	NTL_TS_TC_ID							tcId;
	NTL_TS_TA_ID							taId;
	unsigned int							m_aUserData[eSTOC_EVT_DB_DATA_USER_DATA_MAX];		// tcId, taId User-defined data that is affected by the

	void Init( void )
	{
		tcId = NTL_TS_TC_ID_INVALID;
		taId = NTL_TS_TA_ID_INVALID;
		memset( m_aUserData, 0, sizeof( m_aUserData ) );
	}
};
#pragma pack()

#pragma pack( 1 )
struct sPROGRESS_QUEST_INFO
{
	enum { ePROGRESS_QUEST_INFO_MAX_SIZE = 256 };
	enum { eCUR_VERSION = 0 };

	sPROGRESS_QUEST_INFO( void )
	{
		byVer = eCUR_VERSION;
		tId = NTL_TS_T_ID_INVALID;
		bySlotId = INVALID_BYTE;

		memset( &uData.sQInfoV0, 0xff, sizeof( uData.sQInfoV0 ) );
		uData.sQInfoV0.wQState = 0;
		uData.sQInfoV0.tcQuestInfo = NTL_TS_TC_ID_INVALID;
		uData.sQInfoV0.taQuestInfo = NTL_TS_TA_ID_INVALID;
	}

	BYTE									byVer;				// Version
	NTL_TS_T_ID								tId;				// Quest ID
	BYTE									bySlotId;			//new

	union uDATA
	{
		struct sEXTENDED_POSSIBLE_V0
		{
			WORD							wQState;			// Quest status
			sMAIN_TSP						sMainTSP;			// Quest progress pointer
			sEXCEPT_TIMER_SLOT				sETSlot;			// Storage slot the exception Timer
			sSTORAGE_SLOT_MEMORY			sSSM;				// Storage slot Memory
			sSTOC_EVT_DB_DATA				sSToCEvtData;		// Server event data
			NTL_TS_TG_ID					tgExcCGroup;		// Execute client group

			NTL_TS_TC_ID					tcQuestInfo;		// Quest Information Container ID
			NTL_TS_TA_ID					taQuestInfo;		// Quest Action ID information
		};
		sEXTENDED_POSSIBLE_V0				sQInfoV0;
	};
	uDATA									uData;
};
#pragma pack()

//===================================================================================
//
//===================================================================================
typedef NTL_TS_T_ID		QUESTID;
const	QUESTID			INVALID_QUESTID = NTL_TS_T_ID_INVALID;
//===================================================================================


//-----------------------------------------------------------------------------------
// 트리거 오브젝트의 상태
//-----------------------------------------------------------------------------------
typedef BYTE TOBJECT_STATE;

typedef BYTE TOBJECT_SUBSTATE;


enum eTOBJECT_STATE
{
	TOBJECT_STATE_NONE = 0xFF, // 상태 없음

	MAX_TOBJECT_STATE = 10
};


enum eTOBJECT_STATE_TYPE
{
	TOBJECT_STATE_TYPE_A,
	TOBJECT_STATE_TYPE_B,
	TOBJECT_STATE_TYPE_C,

	MAX_TOBJECT_STATE_TYPE,

	INVALID_TOBJECT_STATE_TYPE = 0xFF
};


enum eTOBJECT_STATE_TYPE_A
{
	TOBJECT_STATE_TYPE_A_CLOSED,
	TOBJECT_STATE_TYPE_A_OPEND,
	TOBJECT_STATE_TYPE_A_DESTROYED,

	MAX_TOBJECT_STATETYPE_A
};


enum eTOBJECT_STATE_TYPE_B
{
	TOBJECT_STATE_TYPE_B_CLOSING,
	TOBJECT_STATE_TYPE_B_CLOSED,
	TOBJECT_STATE_TYPE_B_OPENING,
	TOBJECT_STATE_TYPE_B_OPEND,
	TOBJECT_STATE_TYPE_B_DESTROYING,
	TOBJECT_STATE_TYPE_B_DESTROYED,

	MAX_TOBJECT_STATE_TYPE_B
};


enum eTOBJECT_STATE_TYPE_C
{
	TOBJECT_STATE_TYPE_C_NO_TEAM,
	TOBJECT_STATE_TYPE_C_ATTACK_TEAM,
	TOBJECT_STATE_TYPE_C_DEFENCE_TEAM,

	MAX_TOBJECT_STATE_TYPE_C
};


enum eTOBJECT_SUBSTATE
{
	TOBJECT_SUBSTATE_SHOW,
	TOBJECT_SUBSTATE_UNSELECT,

	MAX_TOBJECT_SUBSTATE,
};


enum eTOBJECT_SUBSTATE_FLAG
{
	TOBJECT_SUBSTATE_FLAG_SHOW = MAKE_BIT_FLAG( TOBJECT_SUBSTATE_SHOW ),
	TOBJECT_SUBSTATE_FLAG_UNSELECT = MAKE_BIT_FLAG( TOBJECT_SUBSTATE_UNSELECT ),
};


enum eTRIGGER_SERVER_EVENT_TYPE
{
	TRIGGER_SERVER_EVENT_TYPE_SPS,
	TRIGGER_SERVER_EVENT_TYPE_TQS,

	TRIGGER_SERVER_EVENT_TYPE_TMQ_START,
	TRIGGER_SERVER_EVENT_TYPE_TMQ_ARRIVE,
	TRIGGER_SERVER_EVENT_TYPE_TMQ_LEAVE,

	TRIGGER_SERVER_EVENT_TYPE_TS,

	TRIGGER_SERVER_EVENT_TYPE_WPS,

	TRIGGER_SERVER_EVENT_TYPE_INVALID = 0xffffffff
};


enum eTS_EVENT_SEND_TYPE
{
	TS_EVENT_SEND_TYPE_WORLD,				// Broadcast to the World
	TS_EVENT_SEND_TYPE_WORLD_AREA,			// Broadcast from a certain distance from a specific point in the World Cast
	TS_EVENT_SEND_TYPE_PARTY,				// All party members: If you do not have a private party
	TS_EVENT_SEND_TYPE_PARTY_AREA,			// Party members within a certain distance: If you do not have a private party.
	TS_EVENT_SEND_TYPE_PC,					// personal
	TS_EVENT_SEND_TYPE_OBJECT_TRIGGER,	//new
	TS_EVENT_SEND_TYPE_INVALID = 0xffffffff
};


//-----------------------------------------------------------------------------------
const BYTE		DBO_MAX_OBJECT_STATE = MAX_TOBJECT_STATE;
const BYTE		DBO_MAX_OBJECT_STATE_ANIMATION = 10;
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------


//===================================================================================
//
//===================================================================================
const int QUEST_REWARD_DEF_MAX_CNT = 4;			/* DboTSCoreDefine.h => MAX_DEFAULT_REWARD */
const int QUEST_REWARD_SEL_MAX_CNT = 4;			/* DboTSCoreDefine.h => MAX_SELECTION_REWARD */

const DWORD					DBO_MAX_COUNT_OF_QUEST_REWARD_SELECT = 20;
const unsigned int			NTL_QUEST_MAX_SELECT_REWARD = 4;
const unsigned int			NTL_QUEST_INVENTORY_WIDTH = 5;
const unsigned int			NTL_QUEST_TIMER_SLOT_MAX = 4;
const unsigned int			NTL_QUEST_INVENTORY_HEIGHT = 6;
const unsigned int			NTL_QUEST_EVENT_PARAM_MAX = 12;
const unsigned int			NTL_QUEST_INVENTORY_SLOT_COUNT = NTL_QUEST_INVENTORY_WIDTH * NTL_QUEST_INVENTORY_HEIGHT;
//===================================================================================



// 한번에 생성할 수 있는 퀘스트 아이템의 최대 종류
const int NTL_QUEST_ITEM_CREATE_MAX_COUNT = 5;

enum eQUEST_ITEM_UPDATE_TYPE
{
	eQUEST_ITEM_UPDATE_TYPE_CREATE = 0,	// 생성
	eQUEST_ITEM_UPDATE_TYPE_UPDATE,		// 업데이트
	eQUEST_ITEM_UPDATE_TYPE_DELETE		// 삭제
};


// Query Server 로 퀘스트 아이템 생성 및 삭제 요청 시 보낸다.
// res를 받았을 때 처리를 위해여 사용
enum eQUEST_ITEM_REQUEST_TYPE
{
	eQUEST_ITEM_REQUEST_TYPE_TS_CREATE,		// TS 에서 생성 요청	-> callback
	eQUEST_ITEM_REQUEST_TYPE_TS_DELETE,		// TS 에서 삭제 요청	-> callback
	eQUEST_ITEM_REQUEST_TYPE_MOB_DROP,		// 몹이 드랍			-> 없음
	eQUEST_ITEM_REQUEST_TYPE_USER_DELETE,	// 사용자 삭제 요청
	eQUEST_ITEM_REQUEST_TYPE_SERVER_EVENT,	// Server Event에서 have quest item 퀘스트 포기시 사용
	eQUEST_ITEM_REQUEST_TYPE_VISIT_EVENT,	// Visit event에서 사용

	eQUEST_ITEM_REQUEST_TYPE_INVALID = 0xFF
};


// 
enum eQUEST_DROP_PERMISSION_TYPE
{
	eQUEST_DROP_PERMISSION_TYPE_PERSONAL,
	eQUEST_DROP_PERMISSION_TYPE_PARTY,

	eQUEST_DROP_PERMISSION_TYPE_INVALID = 0xFF
};


// 에스코트 타입
enum eESCORT_TYPE
{
	ESCORT_TYPE_UNDER_ESCORT, //	pc has to escort this
	ESCORT_TYPE_TARGET_FOLLOW, //	follow target

	MAX_ESCORT_TYPE,
	INVALID_ESCORT_TYPE,
};


// Select reward type
#define INVALID_SELECT_REWARD_SLOT_INDEX	(0xffffffff)

//===================================================================================
// new
//===================================================================================


//===================================================================================
//
//===================================================================================
#pragma pack(1)


struct sQUESTITEM_DATA
{
	TBLIDX			tblidx;
	BYTE			byCurStackCount;
	BYTE			byPos;
};


// Quesy Server ↔ Game Server
typedef sCOMPLETE_QUEST_INFO		sQUEST_COMPLETE_DATA;		//Quest Information
typedef sPROGRESS_QUEST_INFO		sQUEST_PROGRESS_DATA;		//Quest progress information
typedef sQUESTITEM_DATA				sQUEST_INVENTORY_DATA;		//Quest Inventory


// Quest Inventory - used to store all the quest items from client
struct sQUEST_INVENTORY_FULLDATA
{
	BYTE				byQuestItemCount;
	sQUESTITEM_DATA		aQuestItemData[NTL_QUEST_INVENTORY_SLOT_COUNT];
};


// 서버내의 구조체와 서버 ↔ 클라이언트의 구조체를 같이 사용하므로 다음과 같이 typedef
typedef sCOMPLETE_QUEST_INFO	sQUEST_COMPLETE_INFO;
typedef sPROGRESS_QUEST_INFO	sQUEST_PROGRESS_INFO;
typedef sQUESTITEM_DATA			sQUEST_INVENTORY_INFO;



struct sTOBJECT_BRIEF
{
	TBLIDX		objectID; // = trigger object table index
};


struct sTOBJECT_STATE
{
	BYTE		byState; // Main State
	BYTE		bySubStateFlag; // Sub State Flag
	DWORD		dwStateTime; // The past time come to that State
};


#pragma pack()


#pragma pack( 1 )


// Update the quest item [Create, modify, delete] is a quest item information
struct sQUEST_ITEM_UPDATE_DATA
{
	BYTE		byUpdateType;	// eQUEST_ITEM_UPDATE_TYPE
	TBLIDX		itemTblidx;		// Quest items index
	BYTE		byPos;			// 퀘스트 아이템의 위치
	BYTE		byCurCount;		// 퀘스트 아이템의 개수
};


// Trigger System Middle Serial
struct sTSM_SERIAL
{
	DWORD		dwKey;				// TSMiddle Serial
	DWORD		dwSub;				// TSMiddle SubSerial
};

//---------------------------------------------------------------------------------------
// Sub Data

//
struct sQUEST_ITEM_TS_SUB_DATA
{
	BYTE		byTSType;				// eTSM_MANAGER_TYPE
	HOBJECT		hObject;				// Trigger Object Handle : ObjectTrigger에서만 쓰인다.
	sTSM_SERIAL	sSerial;
};


// 퀘스트 아이템을 업데이트[생성,변경,삭제]시 보내어지는 서브데이터
// enum eQUEST_ITEM_REQUEST_TYPE 과 매치된다.
union uQUEST_ITEM_UPDATE_SUB_DATA
{
	sQUEST_ITEM_TS_SUB_DATA	sTSCreate;	// 퀘스트 아이템 생성
	sQUEST_ITEM_TS_SUB_DATA	sTSDelete;	// 퀘스트 아이템 삭제
	DWORD					dwReserve;	// 사용하지 않음 - 몹 사냥으로 인한 아이템 생성
};


//---------------------------------------------------------------------------------------


// Quest Server Event Update 시 사용하는 구조
union uSTOC_EVT_UPDATE_DATA
{
	// 몹 죽이기
	struct sMOB_KILL_CNT
	{
		int											nCurMobCnt;
	};

	// 몹 아이템 루팅 하기
	struct sMOB_KILL_ITEM_CNT
	{
		int											nCurMobLICnt;
	};

	// 아이템 배달하기
	struct sDELIVERY_ITEM
	{
		int											nCurItemCnt;
	};

	// 오브젝트 아이템
	struct sOBJECT_ITEM
	{
		int											nCurItemCnt;
	};

	// public 몹 아이템 루팅 - 몹 공통 드랍 이벤트
	struct sPUBLIC_MOB_ITEM_CNT
	{
		int											nCurItemCnt;
	};

	// Custom event counter
	struct sCUSTOM_EVT_CNT
	{
		int											nCurCnt;
	};

	struct sVISIT_EVT
	{
		bool										bCompleted;
	};

	struct sNPC_KILL_CNT
	{
		int											nCurNPCCnt;
	};


	sMOB_KILL_CNT				sMobKillCnt;
	sMOB_KILL_ITEM_CNT			sMobKillItemCnt;
	sDELIVERY_ITEM				sDeliveryItemCnt;
	sOBJECT_ITEM				sObjectItemCnt;
	sPUBLIC_MOB_ITEM_CNT		sPublicMobItemCnt;
	sCUSTOM_EVT_CNT				sCustomEvtCnt;
	sVISIT_EVT					sVisitEvt;
	sNPC_KILL_CNT				sNPCKillCnt;
};

#pragma pack()


#pragma pack(push, 4)
struct sQUEST_REWARD_DATASET
{
	BYTE						byRewardType;
	DWORD						dwRewardIdx;
	DWORD						dwRewardVal;
};
#pragma pack(pop)