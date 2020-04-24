/*****************************************************************************
*
* File			: NtlSLGuild.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2007. 4. 11	
* Abstract		: Simulation guild info class
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_GUILD_INFO_H__
#define __NTL_GUILD_INFO_H__

// core
#include "ceventhandler.h"

// Share
#include "NtlGuild.h"

// simulation
#include "NtlSLCommunityGroup.h"

struct sGuildMember : public sCommunityMember
{
	CHARACTERID		charID;			///< DB character id	
	RwUInt32		uiWorldNameTable;	///< 지역 이름 테이블 인덱스
	RwUInt8			byRace;			///< 종족
	RwUInt8			byLevel;		///< 레벨
	RwUInt8			byClass;		///< 클래스	
	RwBool			bOnline;		///< 게임 접속 여부
	RwUInt32        uiSerialID;     ///< 시리얼ID (핸들)
	RwUInt32		uiReputation;	///< 평판 점수
	std::wstring	wstrArea;		///< 현재 위치 이름
};

struct sGuildNotice
{
	WCHAR awcCharName[NTL_MAX_SIZE_CHAR_NAME + 1];
	WCHAR awcNotice[NTL_MAX_LENGTH_OF_GUILD_NOTICE + 1];
};

enum eGuildDataType
{
	COMMUNITY_GUILD_LEVEL	= COMMUNITY_DATA_NEXT_INDEX,
	COMMUNITY_GUILD_AREA,
};

enum eDOJO_TEAM_STATE
{
	DOJO_TEAM_NONE,
	DOJO_TEAM_SCRAMBLE_RESERV_OFFENCE,
	DOJO_TEAM_SCRAMBLE_RESERV_DEFENCE,
};

class CNtlGuild : public CNtlSLCommunityGroup, public RWS::CEventHandler
{
public:
	CNtlGuild();
	virtual ~CNtlGuild();	

	virtual	RwBool	Create(void);
	VOID			Destroy(void);
	VOID			Leave();

	virtual	void	HandleEvents(RWS::CMsg &pMsg);

	const WCHAR*	GetGuildName();							///< 길드 이름을 반환한다

	GUILDID			GetGuildID();
	CHARACTERID		GetMasterID();							///< 길드 마스터 ID을 반환한다
	CHARACTERID		GetSecondMasterID(RwInt32 iIndex);		///< 길드 부마스터 ID을 반환한다

	RwInt32			GetCountSecondMaster();					///< 길드 부마스터 인원수를 반환한다
	RwUInt32		GetReputation();						///< 길드 명성 포인트를 반환한다
	RwUInt32		GetReputationEver();					///< 길드 누적 평판 포인트를 반환한다
	time_t			GetTimeToDisband();						///< 길드가 해산되기까지 남은 시간을 반환한다	
	sGuildNotice*	GetNotice();							///< 갈드 공지내용 반환
	RwInt32			GetOnlineMemberCount();
	RwUInt8			GetGuildDogiColorIndex();
	RwUInt8			GetDojoDogiColorIndex();

	// Sort를 실행하고 나서는 이전에 GetMemberbyIndex()로 받은 맴버 구조체와 다를 수 있다
	virtual VOID	Sort(RwUInt8 byDataType, RwBool bAscent);

	virtual RwBool	IsHaveGroup();
	RwBool			IsBeingDisband();						///< 길드가 해산되기인지 여부
	RwBool			IsGuildMaster(CHARACTERID charID);		///< 길드장인지 여부
	RwBool			IsSecondGuildMaster(CHARACTERID charID);///< 부길드장인지 여부
	RwBool			IsHaveEmblem();							///< 길드 문장이 있는지 여부
	RwBool			IsChangedGuildDogiColor();
	RwBool			IsChangedDojoDogiColor();
	RwBool			IsExistFunction(eDBO_GUILD_FUNCTION eFunctionIndex);///< 길드 기능이 존재하는지 여부

	RwBool			CanGetGuildFunction(eDBO_GUILD_FUNCTION eFunctionIndex);	///< 길드 기능을 얻을 수 있는지 여부
	RwBool			CanGetDojoFunction(eDBO_GUILD_FUNCTION eFunctionIndex);		///< 도장 기능을 얻을 수 있는지 여부

	RwBool			EnableUIGuildFunction(eDBO_GUILD_FUNCTION eFunctionIndex);	///< 길드 기능 메뉴를 열 수 있는지 여부
	RwBool			EnableUIDojoFunction(eDBO_GUILD_FUNCTION eFunctionIndex);	///< 도장 기능 메뉴를 열 수 있는지 여부

protected:
	VOID			AppointSecondMaster(CHARACTERID charID);	///< 부길드장으로 임명한다
	VOID			DismissSecondMaster(CHARACTERID charID);	///< 부길드장의 직위를 해제한다

	VOID			ChangeAllMemberDogiColor(RwUInt8 byGuildDogiColor, RwUInt8 byDojoDogiColor);
	VOID			ChangeMemberDogiColor(CHARACTERID charID, RwUInt8 byGuildDogiColor, RwUInt8 byDojoDogiColor);

	static RwBool	CompareAscentLevel(sCommunityMember* pMember, sCommunityMember* pMember2)
	{
		if( !pMember || !pMember2 )
			return FALSE;

		return reinterpret_cast<sGuildMember*>( pMember )->byLevel < reinterpret_cast<sGuildMember*>( pMember2 )->byLevel;
	}
	static RwBool	CompareDecentLevel(sCommunityMember* pMember, sCommunityMember* pMember2)
	{
		if( !pMember || !pMember2 )
			return FALSE;

		return reinterpret_cast<sGuildMember*>( pMember )->byLevel > reinterpret_cast<sGuildMember*>( pMember2 )->byLevel;
	}
	static RwBool	CompareAscentArea(sCommunityMember* pMember, sCommunityMember* pMember2)
	{
		if( !pMember || !pMember2 )
			return FALSE;

		return (wcscmp(reinterpret_cast<sGuildMember*>( pMember )->wstrArea.c_str(),
					   reinterpret_cast<sGuildMember*>( pMember2 )->wstrArea.c_str()) < 0 );
	}
	static RwBool	CompareDecentArea(sCommunityMember* pMember, sCommunityMember* pMember2)
	{
		if( !pMember || !pMember2 )
			return FALSE;

		return (wcscmp(reinterpret_cast<sGuildMember*>( pMember )->wstrArea.c_str(),
					   reinterpret_cast<sGuildMember*>( pMember2 )->wstrArea.c_str()) > 0 );
	}

protected:
	WCHAR					m_wszGuildName[NTL_MAX_SIZE_GUILD_NAME+1];	///< 길드 이름

	GUILDID					m_guildID;
	
	CHARACTERID				m_masterID;					///< 마스터 ID
	CHARACTERID				m_secondMasterID[DBO_MAX_SECOND_MASTER_IN_GUILD];///< 부마스터 ID

	RwUInt32				m_uiReputation;				///< 길드 명성
	RwUInt32				m_uiMaxReputationEver;		///< 길드 최대 누적 명성 포인트	
	time_t					m_timeToDisband;			///< 길드가 해산되기까지 남은 시간

	RwInt32					m_iOnlineMember;

	RwUInt64				m_ui64Function;				///< 길드 기능
	sGuildNotice			m_GuildNotice;				///< 길드 공지

	RwUInt8					m_byGuildDogiColorIndex;	///< 유파 도복 색상
	RwUInt8					m_byDojoDogiColorIndex;		///< 도장 도복 색상
};

#endif