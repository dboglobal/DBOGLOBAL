#pragma once
//***********************************************************************************
//
//	File		:	NtlPetition.h
//
//	Begin		:	2008-04-10 오후 4:27:43
//
//	Copyright	:	ⓒ NTL-Inc Co. Ltd
//
//	strAuthor	:	Seung Goon. Chi   ( sgpro@ntl-inc.com )
//
//	strDesc		:	
//
//***********************************************************************************
//-- include ------------------------------------------------------------------------
#include "NtlSharedType.h"
#include "NtlSharedDef.h"
#include "NtlMail.h"
//-- define -------------------------------------------------------------------------
#define NTL_MIN_SIZE_QUESTION_CONTENT							(4)
#define NTL_MAX_SIZE_QUESTION_CONTENT							(256)

#define NTL_MIN_SIZE_CUSTOMER_SATISFACTION						(4)
#define NTL_MAX_SIZE_CUSTOMER_SATISFACTION						(512)

#define NTL_MIN_SIZE_PETITION_RESULTTEXT						(4)
#define NTL_MAX_SIZE_PETITION_RESULTTEXT						(512)

#define NTL_MIN_SIZE_NOTE										(0)
#define NTL_MAX_SIZE_NOTE										(512)

#define NTL_MAX_PETITION_GETLIST_COUNT							(20)

#define NTL_MAX_SIZE_RESERVEGMID								(256)//보류 GM ID
#define NTL_MAX_SIZE_RESERVEGMID_UNICODE						(256)//보류 GM ID


#define NTL_MAX_SIZE_STARTGMID									(256)// 대응 GM ID


#define NTL_MAX_SIZE_CHANGECATEGORYGMID							(256)// 전달 GM ID


#define NTL_MAX_SIZE_LOG_TEXT									(1024)

#define NTL_MAX_SIZE_PETITIONCOUNSEL_USERID						( NTL_MAX_SIZE_USERID + NTL_MAX_SIZE_CHAR_NAME )


//-----------------------------------------------------------------------------------
//int Category2SelToEnum( int nCurrentSel1, int nCurrentSel2=0 );
std::vector<int>	PetitionCategory1To2( const int nCategory1index );
int					PetitionCategory2To1( const int nCategory2index );
int					PetitionCategory2ToSelectionPos( const int nCategory2EnumValue );
//-- Petition Data ------------------------------------------------------------------

typedef unsigned int						LINKDATAID;
#define	INVALID_LINKDATAID					(0xFFFFFFFFFFFFFFFFui32)

typedef	LINKDATAID							PETITIONID;
#define	INVALID_PETITIONID					INVALID_LINKDATAID

typedef	LINKDATAID							PETITION_CONSULTATIONID;
#define	INVALID_PETITION_CONSULTATIONID		INVALID_LINKDATAID

typedef	LINKDATAID							PETITION_CHATTINGID;
#define	INVALID_PETITION_CHATTINGID			INVALID_LINKDATAID

typedef	LINKDATAID							PETITION_MAILID;
#define	INVALID_PETITION_MAILID				INVALID_LINKDATAID

typedef	LINKDATAID							PETITION_CATEGORYID;
#define	INVALID_PETITION_CATEGORYID			INVALID_LINKDATAID

typedef LINKDATAID							PETITION_INVOLVEDGMLISTID;
#define	INVALID_PETITION_INVOLVEDGMLISTID	INVALID_LINKDATAID

typedef	int									PETITIONCATEGORY;

typedef sDBO_TIME							PETITIONTIME;

// Petition list Control Column information[ by sooshia 2008-09-10 ]
enum ePetitionListColumn
{
	PETITION_LIST_STATE = 0
	,PETITION_LIST_NUMBER
	,PETITION_LIST_CATEGORY1
	,PETITION_LIST_CATEGORY2
	,PETTTION_LIST_SERVER
	,PETITION_LIST_CHARACTERNAME
	,PETITION_LIST_DATE
	,PETITION_LIST_PROGRESSSTATE
	,PETITION_LIST_IMPORTANCE

	,PETITION_LIST_COUNT
	,PETITION_LIST_INVALID = 0xFFFFFFFF
};

enum ePETITIONFINISHTYPE
{
	 PETITION_FINISHTYPE_WRONG = 0
	,PETITION_FINISHTYPE_FINISH

	,PETITION_FINISHTYPE_COUNT
	,INVALID_FINISHTYPE = 0xFFFFFFFF
};

enum ePETITIONSYSTEM_PETITIONINSERTTYPE
{
	 PETITION_INSERT_TYPE_USER = 0
	,PETITION_INSERT_TYPE_GM

	,PETITION_INSERT_TYPE_COUNT
	,PETITION_INSERT_INVALID = 0xFFFFFFFF
};

enum ePETITIONSYSTEM_CATEGORY_1
{
	 PETITION_CATEGORY_1_ACCUSE = 0 // 불량 이용자 신고
	,PETITION_CATEGORY_1_BUG // 버그 관련
	,PETITION_CATEGORY_1_GAMEINFO // 게임 정보
    ,PETITION_CATEGORY_1_STUCK // 캐릭터 이동 불가

	,PETITION_CATEGORY_1_COUNT
	,PETITION_CATEGORY_1_INVALID = 0xFFFFFFFF
};

enum ePETITIONSYSTEM_CATEGORY_2
{
	 PETITION_CATEGORY_2_ACCUSE_AUTOPROGRAM = 0 // 불법프로그램
	,PETITION_CATEGORY_2_ACCUSE_FRAUD // 사기
	,PETITION_CATEGORY_2_ACCUSE_PAPERING // 도배
	,PETITION_CATEGORY_2_BUG_SYSTEM // 시스템
	,PETITION_CATEGORY_2_BUG_GRAPHIC // 그래픽
	,PETITION_CATEGORY_2_BUG_ETC // 기타
	,PETITION_CATEGORY_2_GAMEINFO // 게임 정보
	,PETITION_CATEGORY_2_STUCK // 캐릭터 이동 불가
	, PETITION_CATEGORY_2_CANCEL //

	,PETITION_CATEGORY_2_COUNT
	,PETITION_CATEGORY_2_INVALID = 0xFFFFFFFF
};

const int g_nPetitionCategory1to2[PETITION_CATEGORY_1_COUNT] = { 3, 3, 1, 1 };

enum ePETITIONSYSTEM_DATASTATE
{
	 PETITIONSTATE_NONE = 0
	,PETITIONSTATE_WAIT
	,PETITIONSTATE_START
	,PETITIONSTATE_FINISH

	,PETITIONSTATE_COUNT
	,PETITIONSTATE_INVALID = 0xFFFFFFFF

};

enum ePETITIONSYSTEM_PROGRESSSTATE
{
	 PETITIONPROGRESSSTATE_NONE = 0				//
	,PETITIONPROGRESSSTATE_REQUEST				// Petition 생성
	,PETITIONPROGRESSSTATE_CANCEL				// 취소
	,PETITIONPROGRESSSTATE_START				// 시작
	,PETITIONPROGRESSSTATE_INIT					// 초기화
	,PETITIONPROGRESSSTATE_WRONG				// 상담 불가
	,PETITIONPROGRESSSTATE_RESERVE				// 보류
	,PETITIONPROGRESSSTATE_FINISH				// 완료
	,PETITIONPROGRESSSTATE_FINISHMAILSEND		// 메일 답변
	,PETITIONPROGRESSSTATE_CHANGECATEGORY		// 전달
	,PETITIONPROGRESSSTATE_CHATTING_START		// 챗팅 시작
	,PETITIONPROGRESSSTATE_CHATTING_GMSAY		// GM이 말함
	,PETITIONPROGRESSSTATE_CHATTING_USERSAY		// User가 말함
	,PETITIONPROGRESSSTATE_CHATTING_END			// 챗팅 끝
	,PETITIONPROGRESSSTATE_MAIL					// 메일
	,PETITIONPROGRESSSTATE_CUSTOMERSATISFACTION	// 고객만족도
	,PETITIONPROGRESSSTATE_NOTE					// 비고

	,PETITIONPROGRESSSTATE_COUNT
	,PETITIONPROGRESSSTATE_INVALID = 0xFFFFFFFF
};

struct sPETITION_PROGRESSSTATE_DATA
{
	ePETITIONSYSTEM_PROGRESSSTATE	objType;
	PETITIONID						petitionID;
	GMACCOUNTID						gmAccountId;
	WCHAR							awchGMUserId[NTL_MAX_SIZE_USERID  + 1];
	WCHAR							awchPetitionCharName[NTL_MAX_SIZE_CHAR_NAME + 1];

	union uDATA
	{
		struct sSTART
		{
		} sStart;

		struct sRESERVE
		{
			WCHAR		awchNote[NTL_MAX_SIZE_NOTE + 1];//비고
		} sReserve;

		struct sCHANGECATEGORY
		{
			ePETITIONSYSTEM_CATEGORY_1	eNewCategory1;
			ePETITIONSYSTEM_CATEGORY_2	eNewCategory2;
		} sChangeCategory;

		struct sFINISH
		{
			ePETITIONFINISHTYPE			eFinishType;
			ePETITIONSYSTEM_CATEGORY_1	eNewCategory1;
			ePETITIONSYSTEM_CATEGORY_2	eNewCategory2;
			WCHAR						awchPetitionResultText[NTL_MAX_SIZE_PETITION_RESULTTEXT + 1];
		} sFinish;

		struct sMAIL
		{
			ACCOUNTID					targetAccountID;
			CHARACTERID 				targetCharID;
			WCHAR						wszTargetName[NTL_MAX_SIZE_CHAR_NAME + 1];	// 받을 캐릭 이름
			eMAIL_TYPE 					eMailType;// eMAIL_TYPE
			eMAIL_SENDER_TYPE			eSenderType;//eMAIL_SENDER_TYPE
			sITEM_DATA					sItemData;//아이템정보
			DWORD						dwZenny;// Req or Give Zenny
			BYTE						byDay;// 유효만료날짜 최대 10일
			BYTE  						byTextSize;
			WCHAR						wszText[NTL_MAX_LENGTH_OF_MAIL_MESSAGE + 1];// 메일내용
		} sMail;

		struct sINIT
		{
		} sInit;

		struct sSAY // Start, End, GM Say, User Say
		{
			WORD				wResultCode;
			GMACCOUNTID			userAccountId;
			WCHAR				awchUserId[NTL_MAX_SIZE_USERID  + 1];
			WCHAR				awchUserCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
			SERVERFARMID		serverFarmID;
			WORD				wMessageLengthInUnicode;
			WCHAR				wszPetitionSay[NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];
		} sSay;

	}uData;

public:
	sPETITION_PROGRESSSTATE_DATA()
		: objType ( PETITIONPROGRESSSTATE_INVALID )
		, petitionID ( INVALID_PETITIONID )
		, gmAccountId ( INVALID_GMACCOUNTID )
	{
		memset( awchGMUserId, 0x00, sizeof( awchGMUserId ) );
		memset( awchPetitionCharName, 0x00, sizeof( awchPetitionCharName ) );
	}

	sPETITION_PROGRESSSTATE_DATA( ePETITIONSYSTEM_PROGRESSSTATE _objType
		, PETITIONID _petitionID
		, GMACCOUNTID _gmAccountId
		, WCHAR* _pawchGMUserId
		, WCHAR* _pawchPetitionCharName )
		: objType ( _objType )
		, petitionID ( _petitionID )
		, gmAccountId ( _gmAccountId )
	{
		CopyMemory( awchGMUserId, _pawchGMUserId, sizeof( awchGMUserId ) );
		CopyMemory( awchPetitionCharName, _pawchPetitionCharName, sizeof( awchPetitionCharName ) );
	}

};


enum PETITION_CONSULTATIONDATA_WRITEBY
{
	 PETITION_CONSULTATIONDATA_WRITEBY_TYPE_USER = 0
	,PETITION_CONSULTATIONDATA_WRITEBY_TYPE_GM

	,PETITION_CONSULTATIONDATA_WRITEBY_TYPE_COUNT
	,PETITION_CONSULTATIONDATA_WRITEBY_INVALID = 0xFFFFFFFF
};

enum ePETITIONDATALOADTYPE
{
	 PETITION_DATA_SUMMARY
	,PETITION_DATA_DETAILS

	,PETITION_DATA_COUNT
	,PETITION_DATA_INVALID = -1
};

enum eCUSTOMERSATISFACTION
{
	 CUSTOMERSATISFACTION_NULL
	,CUSTOMERSATISFACTION_VERYGOOD
	,CUSTOMERSATISFACTION_GOOD
	,CUSTOMERSATISFACTION_NORMAL
	,CUSTOMERSATISFACTION_BAD
	,CUSTOMERSATISFACTION_VERYBAD

	,CUSTOMERSATISFACTION_COUNT
	,CUSTOMERSATISFACTION_INVALID = -1
};

//-----------------------------------------------------------------------------------
struct sPETITIONLISTDATA
{
	PETITIONID								petitionId;

	SERVERFARMID							serverfarmID;
	ACCOUNTID								userAccountID_PetitionCreate;
	CHARACTERID								userCharacterID_PetitionCreate;
	WCHAR									awchUserCharacterName_PetitionCreate[NTL_MAX_SIZE_CHAR_NAME + 1];
	WCHAR									awchGMCharacterName_PetitionCreate[NTL_MAX_SIZE_CHAR_NAME + 1]; // 없애자

	ePETITIONSYSTEM_CATEGORY_1				eCategory1;
	ePETITIONSYSTEM_CATEGORY_2				eCategory2;

	PETITIONTIME							tmCreateTime;

	ePETITIONSYSTEM_PETITIONINSERTTYPE		eIsCreateWho; // 없애자

	ePETITIONSYSTEM_DATASTATE				ePetitionDataState;
	ePETITIONSYSTEM_PROGRESSSTATE			ePetitionProgressState;

public:
	sPETITIONLISTDATA()
		: petitionId ( INVALID_PETITIONID )
		, serverfarmID ( INVALID_SERVERFARMID )
		, userAccountID_PetitionCreate ( INVALID_ACCOUNTID )
		, userCharacterID_PetitionCreate ( INVALID_CHARACTERID )
		, eCategory1 ( PETITION_CATEGORY_1_INVALID )
		, eCategory2 ( PETITION_CATEGORY_2_INVALID )
		, tmCreateTime ()
		, ePetitionDataState ( PETITIONSTATE_INVALID )
		, ePetitionProgressState ( PETITIONPROGRESSSTATE_INVALID )
	{
		memset( awchUserCharacterName_PetitionCreate, 0, sizeof( awchUserCharacterName_PetitionCreate ) );
	}

};//end of struct sPETITIONLISTDATA

//-----------------------------------------------------------------------------------
struct sPETITIONDATA
{
	PETITIONID								petitionId; // 접수번호
	SERVERFARMID							serverfarmID; // 서버

	ACCOUNTID								userAccountID_PetitionCreate; // Petition 생성당시 유져의 Account ID ( 시리얼 : 무조건 채워진다 )
	CHARACTERID								userCharacterID_PetitionCreate;// Petition 생성당시 유져의 Char ID ( 시리얼 : 무조건 채워진다 )
	GMACCOUNTID								gmAccountID_PetitionCreate; // Petition을 생성한 GM의 Account ID ( 시리얼 : 유져 생성시 - INVALID )
	CHARACTERID								gmCharacterID_PetitionCreate;// Petition을 생성한 GM의 Account ID ( 시리얼 : 유져 생성시 - INVALID )

	WCHAR									awchUserAccount_PetitionCreate[NTL_MAX_SIZE_USERID + 1];// Petition 생성당시 유져의 Account ID ( 무조건 채워진다 )
	WCHAR									awchUserCharacterName_PetitionCreate[NTL_MAX_SIZE_CHAR_NAME + 1];// Petition 생성당시 유져의 Char ID ( 무조건 채워진다 )
	WCHAR									awchGMAccount_PetitionCreate[NTL_MAX_SIZE_USERID + 1];// Petition을 생성한 GM의 Account ID ( 유져 생성시 "" )
	WCHAR									awchGMCharacterName_PetitionCreate[NTL_MAX_SIZE_CHAR_NAME + 1];// Petition을 생성한 GM의 Account ID ( 유져 생성시 "" )

	WCHAR									awchQuestionContent[NTL_MAX_SIZE_QUESTION_CONTENT + 1]; // 문의 내용
	eCUSTOMERSATISFACTION					eCustomerSatisfaction; // 고객 만족도
	WCHAR									awchPetitionResultText[NTL_MAX_SIZE_PETITION_RESULTTEXT + 1]; // 상담 결과
	WCHAR									awchNote[NTL_MAX_SIZE_NOTE + 1]; // 비고 ( 보류할때 쓰는 비고 )

	ePETITIONSYSTEM_CATEGORY_1				eCategory_Create1;
	ePETITIONSYSTEM_CATEGORY_2				eCategory_Create2;

	ePETITIONSYSTEM_CATEGORY_1				eCategory_Now1;
	ePETITIONSYSTEM_CATEGORY_2				eCategory_Now2;

	PETITIONTIME							tmCreateTime;
	PETITIONTIME							tmFinishTime;
	PETITIONTIME							tmStartTime;

	bool									bIsMailSend;
	bool									bIsFinish;
	ePETITIONSYSTEM_PETITIONINSERTTYPE		eIsCreateWho; // Petition을 GM이 생성했냐 유져가 생성했는지를 표시

	ePETITIONSYSTEM_DATASTATE				ePetitionDataState;
	ePETITIONSYSTEM_PROGRESSSTATE			ePetitionProgressState;

	WCHAR									awchBlackList[ NTL_MAX_LENGTH_OF_CHAT_MESSAGE + 1];

	//bool									bIsConnectUser;
	//SERVERFARMID							serverfarmID_ConnectUser;
	//WCHAR									awchUserCharacterName_ConnectUser[NTL_MAX_SIZE_CHAR_NAME + 1];
	//CHARACTERID							characterID_ConnectUser;

	GMACCOUNTID								gmAccountID_Consult; // 대응 정보의 GM Account ID( 시리얼 )
	WCHAR									awchGMAccount_Consult[NTL_MAX_SIZE_USERID + 1]; // 대응 상황의 GM Account ( Char )
	WCHAR									awchGMCharName_Consult[NTL_MAX_SIZE_CHAR_NAME + 1];// 대응 상황의 GM 이름

	WCHAR									awchReserveGMID[NTL_MAX_SIZE_RESERVEGMID_UNICODE + 1]; //대응 상황의 보류 GM ID
	WCHAR									awchStartGMID[NTL_MAX_SIZE_STARTGMID + 1]; // 대응 상황의 대응 GM ID
	WCHAR									awchChangeCategoryGMID[NTL_MAX_SIZE_CHANGECATEGORYGMID + 1]; // 대응상황의 카테고리 변경 GM ID

public:
	sPETITIONDATA()
		: petitionId ( INVALID_PETITIONID )
		, serverfarmID ( INVALID_SERVERFARMID )
		, userAccountID_PetitionCreate ( INVALID_ACCOUNTID )
		, userCharacterID_PetitionCreate ( INVALID_CHARACTERID )
		, gmAccountID_PetitionCreate ( INVALID_GMACCOUNTID )
		, gmCharacterID_PetitionCreate ( INVALID_CHARACTERID )
		, eCustomerSatisfaction( CUSTOMERSATISFACTION_INVALID )
		, eCategory_Create1( PETITION_CATEGORY_1_INVALID )
		, eCategory_Create2( PETITION_CATEGORY_2_INVALID )
		, eCategory_Now1( PETITION_CATEGORY_1_INVALID )
		, eCategory_Now2( PETITION_CATEGORY_2_INVALID )
		, tmCreateTime ()
		, tmFinishTime ()
		, tmStartTime ()
		, bIsMailSend ( false )
		, bIsFinish ( false )
		, eIsCreateWho ( PETITION_INSERT_INVALID )
		, ePetitionDataState ( PETITIONSTATE_INVALID )
		, ePetitionProgressState ( PETITIONPROGRESSSTATE_INVALID )
		, gmAccountID_Consult( INVALID_GMACCOUNTID )
	{
		memset( awchUserAccount_PetitionCreate, 0, sizeof( awchUserAccount_PetitionCreate ) );
		memset( awchUserCharacterName_PetitionCreate, 0, sizeof( awchUserCharacterName_PetitionCreate ) );
		memset( awchGMAccount_PetitionCreate, 0, sizeof( awchGMAccount_PetitionCreate ) );
		memset( awchGMCharacterName_PetitionCreate, 0, sizeof( awchGMCharacterName_PetitionCreate ) );

		memset( awchQuestionContent, 0, sizeof( awchQuestionContent ) );
		memset( awchPetitionResultText, 0, sizeof( awchPetitionResultText ) );
		memset( awchNote, 0, sizeof( awchNote ) );

		memset( awchBlackList, 0, sizeof( awchBlackList ) );
		memset( awchGMAccount_Consult, 0, sizeof( awchGMAccount_Consult ) );

		memset( awchReserveGMID, 0, sizeof(awchReserveGMID) );
		memset( awchStartGMID, 0, sizeof(awchStartGMID) );
		memset( awchChangeCategoryGMID, 0, sizeof( awchChangeCategoryGMID ) );
		return;
	}

};//end of struct sPETITIONDATA

//-----------------------------------------------------------------------------------
struct sPETITION_COUNSELDATA
{
	PETITIONID								petitionId;
	ePETITIONSYSTEM_PROGRESSSTATE			ePetitionProgressState;
	PETITIONTIME							tmCreateTime;

	WCHAR									awchAccount[NTL_MAX_SIZE_PETITIONCOUNSEL_USERID + 1];
	WCHAR									awchText[NTL_MAX_SIZE_LOG_TEXT + 1];

public:
	sPETITION_COUNSELDATA()
		: petitionId ( INVALID_PETITIONID )
		, ePetitionProgressState ( PETITIONPROGRESSSTATE_INVALID )
		, tmCreateTime()
	{
		memset( awchAccount, 0, sizeof( awchAccount ) );
		memset( awchText, 0, sizeof( awchText ) );
	}
};//end of struct sPETITION_COUNSELDATA

//-- Petition Observer --------------------------------------------------------------
typedef unsigned __int64					OBSERVERID;
#define INVALID_OBSERVERID					0xFFFFFFFFFFFFFFFFui64

enum eGMSTATE
{
	 GMSTATE_LOGIN = 0
	,GMSTATE_LOGOUT
	,GMSTATE_COUNSELSTART
};//end of enum( eGMSTATE )
//-----------------------------------------------------------------------------------
