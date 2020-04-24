/*****************************************************************************
*
* File			: NtlSLCommunityGroup.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 
* Abstract		: 
*****************************************************************************
* Desc          : 게임중의 연관성 있는 그룹의 데이터를 관리하기 위한 원시 클래스
*
*				  map과 list로 중복되어 맴버를 관리하는데 이는 규모가 큰 그룹은
*				  맴버의 가입, 탈퇴가 빈번하지 않고 규모가 작은 그룹은
*				  맴버의 가입, 탈퇴의 부하가 크지 않다고 판단되기 때문이다.
*****************************************************************************/

#ifndef __NTL_COMMUNITY_GROUP_H__
#define __NTL_COMMUNITY_GROUP_H__

#include <algorithm>

// shared
#include "NtlSharedDef.h"


struct sCommunityMember
{
	RwUInt32		uiKey;
	WCHAR			wszMemberName[NTL_MAX_SIZE_CHAR_NAME + 1];	///< 맴버 이름
};

typedef std::map<RwUInt32, sCommunityMember*>			COMMUNITY_MAP;
typedef std::map<RwUInt32, sCommunityMember*>::iterator	COMMUNITY_MAP_ITER;

typedef std::list<sCommunityMember*>					COMMUNITY_LIST;
typedef std::list<sCommunityMember*>::iterator			COMMUNITY_ITER;



// Sort함수용 매크로
#define dCOMMUNITY_GROUP_SORT(bAscentValue, function_Ascent, function_Decent) \
{	\
	if( bAscentValue )	\
	std::stable_sort(m_listMember.begin(), m_listMember.end(), function_Ascent);	\
	else	\
	std::stable_sort(m_listMember.begin(), m_listMember.end(), function_Decent);	\
	break;	\
}

// Sort를 할 데이터 인덱스는 eCommunityDataType을 이어서 정의한다.
// 각 커뮤니티별 Sort는 서로 영향을 끼치지 않기에 인덱스가 아래 정의된 것 이후로
// 중복되어도 무방하다.
enum eCommunityDataType
{
	COMMUNITY_DATA_NAME,

	COMMUNITY_DATA_NEXT_INDEX,

    // 친구 리스트 Sort시 사용
    COMMUNITY_DATA_LEVEL,
    COMMUNITY_DATA_LOC,
    COMMUNITY_DATA_CHANNEL,
};



class CNtlSLCommunityGroup
{
public:
	CNtlSLCommunityGroup() {}
	virtual ~CNtlSLCommunityGroup() {}

	virtual	RwBool	Create() = 0;
	virtual VOID	Destroy() = 0;

	// 커뮤니티에 따라 동적으로 가지고 있는 데이터가 있을 수 있다.
	// 그것들은 함수를 오버라이딩하자.
	virtual VOID	ClearMember();
	virtual VOID	Leave()	= 0;

	sCommunityMember* GetMemberbyIndex(RwInt32 iIndex);
	sCommunityMember* GetMemberbyKey(RwUInt32 uiKey);
	sCommunityMember* GetMemberbyName(WCHAR* pwcName);
	RwInt32			GetMemberCount();

	COMMUNITY_ITER	Begin();
	COMMUNITY_ITER	End();

	// Sort를 실행하고 나서는 이전에 GetMemberbyIndex()로 받은 맴버 구조체와 다를 수 있다
	virtual VOID	Sort(RwUInt8 byDataType, RwBool bAscent);

	// member list를 모두 받고서 한꺼번 GUI단을 업데이트 하기 위해
	// 자신이 그룹에 속해 있는지를 조사하는 것은 member가 있는지 없는지를 검사하지 말고
	// 별도의 로직으로 관리하라. 예) 길드의 이름이 존재하는가?
	virtual RwBool	IsHaveGroup() = 0;
	RwBool			IsMember(RwUInt32 uiKey);

protected:
	RwBool			AddMember(RwUInt32 uiKey, sCommunityMember* pNewMember);
	VOID			DelMember(RwUInt32 uiKey);

	static RwBool	CompareAscentName(sCommunityMember* pMember, sCommunityMember* pMember2)
	{
		if( !pMember || !pMember2 )
			return FALSE;

		return (wcscmp(pMember->wszMemberName, pMember2->wszMemberName) < 0 );
	}
	static RwBool	CompareDecentName(sCommunityMember* pMember, sCommunityMember* pMember2)
	{
		if( !pMember || !pMember2 )
			return FALSE;

		return (wcscmp(pMember->wszMemberName, pMember2->wszMemberName) > 0 );
	}

protected:
	COMMUNITY_MAP			m_mapMember;			///< Container for search
	COMMUNITY_LIST			m_listMember;			///< Aligned container for GUI
};

#endif