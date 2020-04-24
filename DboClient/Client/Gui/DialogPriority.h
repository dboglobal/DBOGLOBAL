/******************************************************************************
* File			: DialogPriority.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 2. 12
* Abstract		: 
*****************************************************************************
* Desc			: 다이얼로그의 랜더링 우선순위를 정의한다
*				  큰 순자의 것이 위에 온다
*				  범위 : 0 ~ 65534
*
*				  우선순위의 간격을 넓직히 벌려두어 차후 우순순위간 추가를
*				  용이하도록 한다
*****************************************************************************/

#pragma once


#define dDIALOGPRIORITY_FLASH_PROLOG									58000
#define dDIALOGPRIORITY_POPUP_MSGBOX									56000
#define dDIALOGPRIORITY_MSGBOX											55000
#define dDIALOGPRIORITY_CHANNEL_CHANGE									54000
#define dDIALOGPRIORITY_ESCMENU											53000
#define dDIALOGPRIORITY_NOTIFY											50000

#define dDIALOGPRIORITY_BROADCAST_EMERGENCY								49500

#define dDIALOGPRIORITY_HLSHOPBUY										49101
#define dDIALOGPRIORITY_HLSHOP											49100

#define dDIALOGPRIORITY_CINEMATIC										49000

#define dDIALOGPRIORITY_BROADCAST_TS									48500

#define dDIALOGPRIORITY_RANKBOARD										48000
#define dDIALOGPRIORITY_WORLDMAP_QUEST_SEARCH							47001
#define dDIALOGPRIORITY_WORLDMAP										47000

#define dDIALOGPRIORITY_TB_TOURNEMENT									46600
#define dDIALOGPRIORITY_TB_REQUEST										46500
#define dDIALOGPRIORITY_TB_NEWS											46400

#define dDIALOGPRIORITY_RESULT_NARRATION								46300
#define dDIALOGPRIORITY_RESULT											46200

#define dDIALOGPRIORITY_CALC_POPUP										46000	// 계산기

#define dDIALOGPRIORITY_PRIVATE_BUSINESS_PRICE							45100	// 흥정 가격 조정 창

#define dDIALOGPRIORITY_EFDIALOG										45000	// Entire Focusing Dialog
#define dDIALOGPRIORITY_EFDIALOG_BACKGROUND								44999	// Entire Focusing Dialog backgound

#define dDIALOGPRIORITY_TBGAMBLERESULT									40000	// 천하제일 무도회 겜블 결과창

#define dDIALOGPRIORITY_PRIVATE_BUSINESS								35000	// 흥정 창

#define dDIALOGPRIORITY_RPBONUS											34000
#define dDIALOGPRIORITY_SKILLABILITY_SET								33500	///< 스킬 어빌리티 지정 창

#define dDIALOGPRIORITY_MANIA_TIME_SET									33250	///< 게임 매니아 타임 재설정 창

#define dDIALOGPRIORITY_CHATTING_OPTION									33155

#define dDIALOGPRIORITY_TUTORIAL_FOCUS									33100

#define dDIALOGPRIORITY_MAINMENU										33000

#define dDIALOGPRIORITY_INVENTORY										32995

#define dDIALOGPRIORITY_PORTAL											32990

#define dDIALOGPRIORITY_DEFAULT											32767

#define dDIALOGPRIORITY_QUICKSLOT										32567

#define dDIALOGPRIORITY_PARTY_MENU										32400

#define dDIALOGPRIORITY_SUBGAUGE										32100
#define dDIALOGPRIORITY_CHATTING										32000

#define dDIALOGPRIORITY_SIDEICON										31000

#define dDIALOGPRIORITY_SIDE_DIALOG										400

#define dDIALOGPRIORITY_BROADCAST_MININARRATION							350

#define dDIALOGPRIORITY_CHAT_BALLOON									300