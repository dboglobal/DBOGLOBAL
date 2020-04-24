#pragma once

enum eSideIconType
{
	SIDEICON_HINT,						///< 힌트 아이콘
	SIDEICON_DBC,						///< 드래곤볼 수집 아이콘
	SIDEICON_RB,						///< 랭크배틀 아이콘
	SIDEICON_DOREPAIR,					///< 수리요구 아이콘
	SIDEICON_TUTORIAL,					///< 튜토리얼 아이콘
	SIDEICON_CAUTION,					///< 경고 아이콘
	SIDEICON_BUDOKAI,					///< 천하제일무도회 아이콘
	SIDEICON_PETITION,					///< Petition 아이콘
	SIDEICON_GM,						///< GM 아이콘
	SIDEICON_DOJO,						///< 도장 사이드 아이콘
    SIDEICON_NETPY,                     ///< NetPy 사이드 아이콘
	SIDEICON_HLS,

	SIDEICON_NUMS,
	INVALID_SIDEICON
};

enum eSideViewType
{
	SIDEVIEW_HINT_HELP,					///< 헬프 힌트 뷰
	SIDEVIEW_HINT_GUIDE,				///< 가이드 힌트 뷰
	SIDEVIEW_HINT_STRING,				///< 문자열 알림 뷰
	SIDEVIEW_DBC,						///< 드래곤볼 수집 알림 뷰
	SIDEVIEW_RB,						///< 랭크배틀 뷰
	SIDEVIEW_DOREPAIR,					///< 수리요구 뷰
	SIDEVIEW_TUTORIAL,					///< 튜토리얼 뷰
	SIDEVIEW_CAUTION,					///< 경고 뷰
	SIDEVIEW_BUDOKAI,					///< 천하제일무도회 뷰
	SIDEVIEW_PETITION,					///< Petition 뷰
	SIDEVIEW_GM,						///< GM 뷰
	SIDEVIEW_DOJO,						///< 도장 뷰
    SIDEVIEW_NETPY,                     ///< NetPy 뷰
	SIDEVIEW_HLS,

	SIDEVIEW_NUMS,
	INVALID_SIDEVIEW
};

// 드래곤볼 컬렉션의 사이드 아이콘 상태
enum eDBCSideIconType
{
	E_ICON_NO_SEASON,					///< 드래곤볼 수집 기간이 아닐때
	E_ICON_SEASON,						///< 드래곤볼 수집 기간일때
	E_ICON_COLLECT,						///< 드래곤볼 수집을 완료 했을때
};

enum eNetPySideViewType
{
    E_NETPY_INFO,                       ///< 현재 NetPy 상태 표시
    E_NETPY_NOTIFY,                     ///< NetPy 획득 알림 표시
};


// 사이드 아이콘 우선순위 ////////////////////////////////////////////////////////////////////
// 우선 순위가 낮을수록 오른쪽에 출력된다.
#define dSIDEICON_PRIORITY_DEFAULT		50
#define dSIDEICON_PRIORITY_CAUTION		25
#define dSIDEICON_PRIORITY_TUTORIAL		0