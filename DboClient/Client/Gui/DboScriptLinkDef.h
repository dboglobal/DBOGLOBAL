/****************************************************************************
* File			: DboScriptLinkDef.h
* Author		: HaeSung, Cho
* Copyright		: (주)NTL
* Date			: 2008. 5. 26	
* Abstract		: DBO 의 스크립트 링크 테이블의 정의된 ID들
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __DBO_SCRIPT_LINK_DEF_H___
#define __DBO_SCRIPT_LINK_DEF_H___

// 연결된 컨텐츠의 타입
enum eScriptLinkType
{
	eLINKTYPE_TENKAICHIBUDOKAI = 0,
	eLINKTYPE_TIMEMACHINEQUEST,
	eLINKTYPE_RANKBATTLE,

	eLINKTYPE_NUMS,
	eLINKTYPE_INVALID = 0xFF
};

// 천하제일무도회의 액션 넘버
enum eTenKaichiBudokaiAction
{
	eBUDOKAI_NEWSMAIN = 0,					///< 소식지 NEWS 메인을 연다.
	eBUDOKAI_REQUEST,						///< 신청서 UI를 본다.
	eBUDOKAI_TOURNAMENT,					///< 토너먼트 대진표 보기
	eBUDOKAI_HTML,							///< 공용 HTML 열기
	eBUDOKAI_CLOSE,							///< 인터페이스 닫기

	eBUDOKAI_NUMS,
	eBUDOKAI_INVALID = 0xFF
};

#endif
