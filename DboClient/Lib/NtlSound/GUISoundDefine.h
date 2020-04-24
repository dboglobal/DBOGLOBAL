/******************************************************************************
* File			: GUISoundDefine.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2007. 2. 21
* Abstract		: 
*****************************************************************************
* Desc			: System에서 쓰이는 사운드 파일의 이름을 정의
*****************************************************************************/

#pragma once

#include "NtlSoundManager.h"


#define GSD_SYSTEM_ICON_SET					"System\\SYSTEM_ICON_SET.wav"
///< Skill Icon, etc. to put in the Quick Slot. "Patch"

#define GSD_SYSTEM_ICON_SELECT				"System\\SYSTEM_ICON_SELECT.wav"
///< The sound when I picked Icon.

#define GSD_SYSTEM_ICON_WASTE				"System\\SYSTEM_ICON_WASTE.wav"	
///< Icon를 Quick Slot로부터 버렸을 때의 소리.「카삭」.(종이를 쓰레기통에 넣었을 때의 이미지로.)

#define GSD_SYSTEM_BUTTON_CLICK				"System\\SYSTEM_BUTTON_CLICK.wav"
///< Button를 눌렀을 때의 Click음.「카치」.

#define GSD_SYSTEM_BUTTON_CANCEL			"System\\SYSTEM_BUTTON_CANCEL.wav"
///< Cancel를 눌렀을 때의 소리.「트」.

#define GSD_SYSTEM_BUTTON_FOCUS				"System\\SYSTEM_Button_Focus.wav"
///< 버튼 Focus 되었을 때의 소리.

#define GSD_SYSTEM_BUTTON_DISABLE			"System\\SYSTEM_Button_Disable_Alarm.wav"
///< Disable 버튼 Click 되었을 때의 소리.

#define GSD_SYSTEM_WINDOW_OPEN				"System\\SYSTEM_WINDOW_OPEN.wav"
///< Window가 열릴 때의 소리.「슨」.

#define GSD_SYSTEM_WINDOW_CLOSE				"System\\SYSTEM_WINDOW_CLOSE.wav"
///< Window가 닫을 때의 소리.「삭」.

#define GSD_SYSTEM_ITEM_REPAIR				"System\\SYSTEM_ITEM_REPAIR.wav"
///< Item 수리 때의 소리.「캔」.(대장간의 이미지로.)

#define GSD_SYSTEM_WINDOW_ALERT				"System\\SYSTEM_WINDOW_ALART.wav"
///< 경고 Window가 나올 때의 소리.「곤」.

#define GSD_SYSTEM_NOTICE					"System\\SYSTEM_NOTICE.wav"
///< 화면에 좋은 것을 알리는 Message가 나왔을 때의 소리.「핀!」.

#define GSD_SYSTEM_ITEM_SPEND				"System\\SYSTEM_ITEM_SPEND.wav"
///< 소비 Item를 사용을 결정했을 때의 소리.「포원」.

#define GSD_SYSTEM_ITEM_EQUIP				"System\\SYSTEM_ITEM_EQUIP.wav"
///< 장비 Item를 장비했을 때의 소리.「사삭」.(옷에 소매를 통하는 이미지.)

#define GSD_SYSTEM_ITEM_TAKEOFF				"System\\SYSTEM_ITEM_TAKEOFF.wav"
///< 장비 Item를 해제했을 때의 소리.「삭」.(옷에 소매를 통하는 이미지.)

#define GSD_SYSTEM_TAB_CLICK				"System\\SYSTEM_TAB_CLICK.wav"
///< Tab를 선택했을 때의 Sound.「삭」.(종이를 갈아넣은 이미지.)

//******************************************대화가 필요한.....?
#define GSD_SYSTEM_POPUPWINDOW_OPEN			"System\\SYSTEM_POPWIN_OPEN.wav"
///< PopWindow 열릴 때의 소리

#define GSD_SYSTEM_POPUPWINDOW_CLOSE		"System\\SYSTEM_POPWIN_CLOSE.wav"
///< PopWindow 닫을 때의 소리

#define GSD_SYSTEM_TAB_FOCUS				"System\\SYSTEM_TAB_FOCUS.wav"
//< Tab 마우스가 올라갔을때 소리

#define GSD_SYSTEM_TREE_OPEN				"System\\SYSTEM_TREE_OPEN.wav"
//< 트리가 펼쳐질때

#define GSD_SYSTEM_TREE_CLOSE				"System\\SYSTEM_TREE_CLOSE.wav"
//< 트리가 닫혀질때

#define GSD_SYSTEM_TUTORIAL_MSGBOX			"System\\TUTORIAL_MSGBOX.wav"
//< tutorial msgbox 

#define GSD_SYSTEM_CHECKBOX					"System\\GENERAL_CHKBOX.wav"
//< 체크박스 체크될때

//******************************************완료

#define GSD_SYSTEM_MESSAGEBOX				"System\\SYSTEM_MSGBOX.wav"
//#define GSD_SYSTEM_MESSAGEBOX				""
/////< MES Box 나올때의 소리.
//
#define GSD_SYSTEM_ENEMY_SELECT				"System\\SYSTEM_ENEMY_FOCUS.wav"	
////<적을선택했을때
//
#define GSD_SYSTEM_ENEMY_FOUSE				"System\\SYSTEM_ENEMY_SELECT.wav"	
////<적에 마우스를 올려 놓았을때
//
#define GSD_SYSTEM_CHARACTER_ROTATE			"System\\SYSTEM_CHAR_ROTATE.wav"
/////< Character  회전
//
#define GSD_SYSTEM_PARTS_EQUIP				"System\\SYSTEM_EQUIP.wav"	
////< 스카우트 파츠 장착할때
//
#define GSD_SYSTEM_ABILITY_UI_OPEN			"System\\SYSTEM_SKILL_OPEN.wav"
////< 스킬 어빌리티 UI나타날때
//
#define GSD_SYSTEM_GUARD_GAGE				"System\\SYSTEM_GUARD_GAGEDOWN.wav"
////< 가드 게이지가 줄어들때
//
#define GSD_SYSTEM_GUARD_GAGE_END			"System\\SYSTEM_GUARD_GAGEDOWN_END.wav"
////< 가드 게이지가 줄어들때
//

#define GSD_SYSTEM_PUBLIC_NOTIFY			"System\\SYSTEM_NOTICE.wav"	
///< 공지가 뜰때 알람음

#define GSD_SYSTEM_ACTION_NOTIFY			"System\\SYSTEM_NOTICE_ACTION.wav"
/////< 어떤 행동의 결과에 따라 나오는 알림음
//
#define GSD_SYSTEM_WARNING_NOTIFY			"System\\SYSTEM_NOTICE_WARNING.wav"
/////< 해서는 안되는 행동을 했을 때의 알림음				ps. Class 는 CCautionNotifyGui 임
//
#define GSD_SYSTEM_PARTY_NOTIFY				"System\\SYSTEM_NOTICE_PARTY.wav"
/////< 파티또는 길드에서 유저를 부를 때의 알림음
//
#define GSD_SYSTEM_TERRAIN_NOTIFY			"System\\SYSTEM_NOTICE_TERRAIN.wav"	

/////< 새로운 지형에 갔을때 지형이름을 알려주는 알림음
//
#define GSD_SYSTEM_ITEM_LIGHT_ON			"System\\SYSTEM_ITEM_UPGRADE.wav"

////< 속성에 맞는 색으로 변할때 ( 업그레이드 가능한 공간으로 이동시 )

#define GSD_SYSTEM_KEY_DOWN					"System\\GENERAL_TEXTINPUT.wav"
//< 타자 치는 소리

//
#define GSD_SYSTEM_DRAGOBALL_UI_OPEN		"System\\SYSTEM_DB_OPEN.wav"	

////< 드래곤볼 제단 열릴때
//
#define GSD_SYSTEM_DRAGOBALL_UI_CLOSE		"System\\SYSTEM_DB_CLOSE.wav"	
	
////< 드래곤볼 제단 닫힐때
//
#define GSD_SYSTEM_POTAL_ICON_MOUSE_OVER	"System\\SYSTEM_PORTAL_MOUSE_OVER.wav"

#define GSD_SYSTEM_MOUSE_CLICK_HV			"System\\SYSTEM_MOUSE_CLICK_HV.wav"

////< 포탈 탈수 있는 위치의 아이콘에 마우스를 올려 놓았을 때
//

#define GSD_SYSTEM_DRAGOBALL_REGISTRATION	"System\\SYSTEM_DB_REG.wav"	
//< 드래곤볼이 돌에 박힐때

#define GSD_SYSTEM_DRAGOBALL_UI_LIGHT		"System\\SYSTEM_DB_LIGHT.wav"	

////< 드래곤볼 제단에 불이 켜질때
//
//
#define GSD_SYSTEM_NEW_MAIL_ALRAM			"System\\SYSTEM_NEW_MAIL.wav"	

////< 새로운 편지가 도착했을때
//
#define GSD_SYSTEM_MAIL_SEND				"System\\SYSTEM_SEND_MAIL.wav"	

////< 메일을 보낼때
//
#define GSD_SYSTEM_SCOUTER_LOOP				"System\\GENERAL_Scouter_Loop.wav"
//< 스카우트 측정증

#define GSD_SYSTEM_SCOUTER_SCAN				"System\\GENERAL_Scouter_Scan.wav"
//< 스카우트 타겟 설정시
//******************************************완료

#define GSD_SYSTEM_USE_MONEY				"System\\SYSTEM_USE_MONEY.wav"
///< Money를 사용했을 때의 Sound.기분 좋은 소리로, 「차 인」.

#define GSD_SYSTEM_DROP_NORMAL				"System\\SYSTEM_DROP_NORMAL.wav"
///< Item Drop - Mob가 사라져 Capsule와 Zenny가 떨어지는 Sound.별로 귀에 거슬리는 소리가 되지 않는 소리로, 「본」.

#define GSD_SYSTEM_DROP_LUCKY				"System\\SYSTEM_DROP_LUCKY.wav"
///< Item Drop - Lucky!! 운이 좋아서 평소보다 좋은 Capsule와 많은 Zenny가 떨어지는 Sound.별로 귀에 거슬리는 소리가 되지 않는 소리로, 「보욘」.

#define GSD_SYSTEM_ITEM_GET					"System\\SYSTEM_ITEM_GET.wav"
///< Item Drop - Drop가 된 Item를 얻었을 때의 Sound.「슨」.

#define GSD_SYSTEM_SKILL_LEARN				"System\\SYSTEM_SKILL_LEARN.wav"
///< Skill를 배웠을 때의 Sound.교회의 종과 같은 소리로, 「곤」.

#define GSD_SYSTEM_SKILL_DISABLE			"System\\SYSTEM_SKILL_DISABLE.wav"
///< Skill나 Item를 사용할 수 없을 때의 Sound.귀에 거슬리는 소리가 되지 않는 소프트인 낮은 소리로, 「곤」.

#define GSD_SYSTEM_QUEST_GIVE_UP			"System\\SYSTEM_QUEST_GIVE_UP.wav"
///< Quest를 Give up 할 때의 Sound.유감스러운 소리로, 「워···」.

#define GSD_SYSTEM_TRIGGER_OBJECT_CLICK		"System\\GENERAL_Click_01.wav"
///< Trigger object를 클릭 했을 때 나는 소리

#define GSD_JINGLE_QUEST_START				"BGM\\JINGLE_QUEST_START.ogg"
///< Quest를 시작할 때 의지를 담은 느낌을 주는 용감한 느낌.

#define GSD_JINGLE_QUEST_FINISH				"BGM\\JINGLE_QUEST_FINISH.ogg"
///< Quest를 끝냈을때 칭송하는것 같은 축복의 음악

#define GSD_JINGLE_LEVEL_UP					"BGM\\JINGLE_LEVEL_UP.ogg"
///< LevelUp

#define GSD_JINGLE_PVP_START				"BGM\\JINGLE_PVP_START.ogg"
///< PvP Start

#define GSD_JINGLE_PVP_END					"BGM\\JINGLE_PVP_END.ogg"
///< PvP End

#define GSD_GET_EXP							"System\\SYSTEM_Exp_Get.wav"
///< Exp Get

#define GSD_QUEST_ITEM_GET					"System\\SYSTEM_Quest_item_Get.wav"
///< Quest Item Get

#define GSD_RP_FULL							"System\\GENERAL_RP_Full.wav"
///< RP Full



#define GSD_HEART_BEAT						"Syatem\\GENERAL_Radar.wav"	// 임시
///< HeartBeat HP저하시 반복

/// 용신 등장시의 배경음악
#define GSD_JINGLE_DRAGON                   "BGM\\JINGLE_SHENRON.ogg"

///< 주의가 필요하여 사용자에게 알림성 효과음
#define GSD_SYSTEM_WINDOW_ALREAT					"System\\SYSTEM_WINDOW_ALART.wav"

///< 짤깍짤깍
#define GSD_GENERAL_CLOCK_SECOND					"System\\GENERAL_Clock_Second.wav"

///< Music when you catch a HERO BOSS in a dungeon
#define GSD_FINISH_HERO_MOB							"BGM\\JINGLE_BOSS_DOWN.ogg"



//////////////////////////////////////////////////////////////////////////
//						Background Mugic								//
//////////////////////////////////////////////////////////////////////////

#define GSD_BGM_PVP							"BGM\\BGM_PVP.ogg"

#define GSD_BGM_PRIVATE_RANKBATTLE			"BGM\\BGM_RANK_BATTLE_2m.ogg"
#define GSD_BGM_PARTY_RANKBATTLE			"BGM\\BGM_RANK_BATTLE_3m.ogg"

#define GSD_BGM_MAIN_THEME					"BGM\\BGM_MAIN_THEME.ogg"
#define GSD_BGM_PROLOG						"BGM\\BGM_PROLOGE.ogg"


//////////////////////////////////////////////////////////////////////////
//						천하제일 무도회									//
//////////////////////////////////////////////////////////////////////////

#define GSD_BGM_TB_STANDBY					"BGM\\BGM_TB_STANDBY.ogg"
#define GSD_BGM_TB_PRIVATE_BATTLE			"BGM\\BGM_TB_MAIN_m3.ogg"
#define GSD_BGM_TB_PARTY_BATTLE				"BGM\\BGM_TB_MAIN_m4.ogg"

#define GSD_TB_TOURNAMENT_START				"BGM\\JINGLE_TB_START.ogg"
#define GSD_TB_ROUND_END					"BGM\\JINGLE_PVP_END.ogg"
#define GSD_TB_MATCH_RESULT					"BGM\\JINGLE_TB_RESULT.ogg"


//////////////////////////////////////////////////////////////////////////
//						도장 쟁탈전										//
//////////////////////////////////////////////////////////////////////////

#define GSD_BGM_DOJO_SCRAMBLE_READY			"BGM\\BGM_PROLOGE_B.ogg"
#define GSD_BGM_DOJO_SCRAMBLE_START			"BGM\\BGM_DOJO_BATTLE.ogg"