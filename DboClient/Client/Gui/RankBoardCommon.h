/*****************************************************************************
*
* File			: RankBoardCommon.h
* Author		: Haesung, Cho
* Copyright		: (주)NTL
* Date			: 2007. 12. 14	
* Abstract		: DBO Rank Board GUI 공통
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __RANKBOARDCOMMON_H__
#define __RANKBOARDCOMMON_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"
#include "NtlRankBattle.h"

// 보드의 타입들
enum eBoardType
{ 
	eBOARD_RANKBATTLE = 0,						// RankBattle Board
	//eBOARD_TMQ,									// TMQ Board
	eBOARD_NUMS, 
	eBOARD_INVALID = eBOARD_NUMS 
};

// RankBoard Common Define
#define dRANKBOARD_COMMON_ENABLE_IDLE			((RwReal)DBO_RANKBATTLE_RANK_REQUEST_TERM*0.001f)// 한번의 패킷 전송 후에 입력이 될 수 있는 시간

// RankBoard Size
#define dRANKBOARD_DIALOG_WIDTH					631				// Dialog Width
#define dRANKBOARD_DIALOG_HEIGHT				358				// Dialog Height


class CRankBoardGui;

/**
* \ingroup client
* \brief Base class (Board)
*/
class CBoard
{
public:
	CBoard() : m_eBoardType(eBOARD_INVALID) { }
	virtual ~CBoard() {};

	eBoardType GetType() { return m_eBoardType; }

	// abstract
	virtual void Create( eBoardType eType , CRankBoardGui* pParent ) = 0;
	virtual void Destroy() = 0;

	virtual void Show( RwBool bShow = TRUE ) = 0;
	virtual void Enable( RwBool bEnable = TRUE ) = 0;

	// Remove
	virtual void CurrentPage() = 0;
	virtual void MessageProc( RWS::CMsg& msg ) = 0;

protected:
	eBoardType	m_eBoardType;
};

#endif//__RANKBOARDCOMMON_H_