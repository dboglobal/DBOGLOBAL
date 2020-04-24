////////////////////////////////////////////////////////////////////////////////
// File: SkillCommonGui.h
// Desc: Skill Master, Skill Gui간의 공통된 클래스 선언.
//
// 2007.9.3 Peessi@hitel.net  
// To Be Desired:
////////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_COMMON_H__
#define __SKILL_COMMON_H__

struct sTBLDAT;
struct sSKILL_TBLDAT;			/// woosungs_test 20090730
class  CNtlSobIcon;
class  CNtlSobNpc;

class CSkillGuiItem
{
public:
	//! Enum 
	enum TYPE { SKILL, HTB, ACTION };

	//! Constructors and Destructor
	CSkillGuiItem(VOID) : m_pIcon( NULL ), m_pData( NULL ) {}
	CSkillGuiItem( CNtlSobIcon* pIcon, sTBLDAT* pData, TYPE eType ) : m_pIcon( pIcon ), m_pData( pData ), m_eType( eType) {}
	~CSkillGuiItem(VOID) {}

	//! Attributes
	RwBool IsUseable(VOID) { return m_pIcon ? TRUE : FALSE; }

	CNtlSobIcon*	GetIcon(VOID) { return m_pIcon; }
	sTBLDAT*		GetData(VOID) { return m_pData; }
	TYPE			GetType(VOID) { return m_eType; }

	VOID	SetIcon( CNtlSobIcon* pIcon ) { m_pIcon = pIcon; }
	VOID	SetData( sTBLDAT* pData ) { m_pData = pData; }
	VOID	SetType( TYPE eType ) { m_eType = eType; }

	//! Variables
protected:
	CNtlSobIcon*	m_pIcon;	
	sTBLDAT*		m_pData;
	TYPE			m_eType;
};

enum eSKILLRQSTATE
{ 
	SRQ_FLAG_INITIAL					= 0x0000,	// 					
	SRQ_FLAG_NOT_LEARN_SKILL			= 0x0001,	// 미리 배워야 하는 스킬때문에 배울 수 없다.
	SRQ_FLAG_NOT_LEARN_LEVEL			= 0x0002,	// 레벨이 낮아 스킬을 배울 수 없다.
	SRQ_FLAG_CAN_LEARN_BY_SKILLMASTER	= 0x0004,	// 스킬마스터에게 스킬을 배울 수 있다.
	SRQ_FLAG_CAN_LEARN_BY_SELF			= 0x0008,	// 자신이 직접 스킬을 배울 수 있다.
	SRQ_FLAG_NOT_UPGRADE_SKILLEXP		= 0x0010,	// 스킬 경험치가 낮아 업그레이드를 할 수 없다.
	SRQ_FLAG_NOT_UPGRADE_LEVEL			= 0x0020,	// 레벨이 낮아 업그레이드를 할 수 없다.
	SRQ_FLAG_CAN_UPGRADE				= 0x0040,	// 업그레이드를 할 수 있다.
	SRQ_FLAG_MASTER						= 0x0080,	// 스킬을 마스터 했다.
	SRQ_FLAG_LEARNED					= 0x0100,	// 이미 배운 스킬이다. 	
	SRQ_FLAG_NOT_LEARN_CLASS			= 0x0200,	// 클래스가 안맞아 배울수 없다. 
	SRQ_FLAG_NOT_LEARN_CHANGECLASSRIGHT	= 0x0400,	// 마스터리는 전직권한을 얻어야 배울 수 있다.
};

//! examin												/// woosungs_test 20090730
namespace SkillCommonLogic
{
	RwBool IsLearnable( CSkillGuiItem::TYPE eType, sTBLDAT* pItemTblData );
	RwUInt32 ExmineRQBySkill( sTBLDAT* pItemTblData, RwBool bExceptSelfLearn = TRUE );
	RwUInt32 ExmineRQByHTB( sTBLDAT* pItemTblData );
}

#define SKILLWND_ITEM_WIDTH			278	
#define SKILLWND_ITEM_HEIGHT		66	

#define SKILLWND_ITEM_ICON_X		9
#define SKILLWND_ITEM_ICON_Y		11
#define SKILLWND_ITEM_ICON_WIDTH	32
#define SKILLWND_ITEM_ICON_HEIGHT	32

#define SKILLWND_ITEM_BAR_X			86
#define	SKILLWND_ITEM_BAR_Y			46
#define SKILLWND_ITEM_BAR_WIDTH		100
#define SKILLWND_ITEM_BAR_HEIGHT	10

#define SKILLWND_ITEM_BUTTON_X		4	
#define SKILLWND_ITEM_BUTTON_Y		46
#define SKILLWND_ITEM_BUTTON_WIDTH	42
#define SKILLWND_ITEM_BUTTON_HEIGHT 16

#define SKILLWND_ITEM_RQ_X			240	
#define SKILLWND_ITEM_RQ_Y			27
#define SKILLWND_ITEM_RQ_WIDTH		33
#define SKILLWND_ITEM_RQ_HEIGHT		33

#define SKILLWND_ITEM_RPTYPE_X		31
#define SKILLWND_ITEM_RPTYPE_Y		46
#define SKILLWND_ITEM_RPTYPE_WIDTH	12
#define SKILLWND_ITEM_RPTYPE_HEIGHT	12

#define SKILLWND_TEXT_NAME_X		56
#define SKILLWND_TEXT_NAME_Y		3
#define SKILLWND_TEXT_NAME_WIDTH	214
#define SKILLWND_TEXT_NAME_HEIGHT	20

#define SKILLWND_TEXT_GRADE_X		0
#define SKILLWND_TEXT_GRADE_Y		1
#define SKILLWND_TEXT_GRADE_WIDTH	52
#define SKILLWND_TEXT_GRADE_HEIGHT	15

#define SKILLWND_ITEM_TEXT_X		0
#define SKILLWND_ITEM_TEXT_Y		0
#define SKILLWND_ITEM_TEXT_WIDTH	278
#define SKILLWND_ITEM_TEXT_HEIGHT	64

#define SKILLWND_ITEM_TEXT_EXP_X			189
#define SKILLWND_ITEM_TEXT_EXP_Y			46
#define SKILLWND_ITEM_TEXT_EXP_WIDTH		37
#define SKILLWND_ITEM_TEXT_EXP_HEIGHT		12

#define SKILLWND_ITEM_RPMODE_BUTTON_X		232
#define SKILLWND_ITEM_RPMODE_BUTTON_Y		1
#define SKILLWND_ITEM_RPMODE_BUTTON_WIDTH	37
#define SKILLWND_ITEM_RPMODE_BUTTON_HEIGHT	12

//#define SKILLWND_TEXT_CONSUME_TEXT_X		50
//#define SKILLWND_TEXT_CONSUME_TEXT_Y		28//25
//#define SKILLWND_TEXT_CONSUME_TEXT_WIDTH	43
//#define SKILLWND_TEXT_CONSUME_TEXT_HEIGHT	17
//
//#define SKILLWND_TEXT_REQUIRE1_TEXT_X		104	
//#define SKILLWND_TEXT_REQUIRE1_VALUE_X		124
//#define SKILLWND_TEXT_REQUIRE2_TEXT_X		154	
//#define SKILLWND_TEXT_REQUIRE2_VALUE_X		174
//#define SKILLWND_TEXT_REQUIRE_Y				30//27

#define SKILLWND_TEXT_EFFECT_X				57
#define SKILLWND_TEXT_EFFECT_Y				29
#define SKILLWND_TEXT_EFFECT_WIDTH			210
#define SKILLWND_TEXT_EFFECT_HEIGHT			14

#define SKILLWND_TEXT_HELP_X				57
#define SKILLWND_TEXT_HELP_Y				44//41
#define SKILLWND_TEXT_HELP_WIDTH			210
#define SKILLWND_TEXT_HELP_HEIGHT			14

#define SKILLWND_ITEM_ZENNYMARK_X		57
#define SKILLWND_ITEM_ZENNYMARK_Y		44
#define SKILLWND_ITEM_ZENNYMARK_WIDTH	13
#define SKILLWND_ITEM_ZENNYMARK_HEIGHT	14

#define SKILLWND_TEXT_ZENNY_OFFSET		5
#define SKILLWND_TEXT_SP_OFFSET			10

#define SKILLWND_TEXT_COST_X			83
#define SKILLWND_TEXT_COST_Y			47//44		

#define SKILLWND_TEXT_ACTIONCOMMAND_X	60	

#define SKILLWND_COLOR_GRADEMASTER		RGB(255,255,64)
#define SKILLWND_COLOR_CONSUME_TEXT		RGB(0xB7,0xB7,0xB7)
#define SKILLWND_COLOR_LP				RGB(235,54,72)
#define SKILLWND_COLOR_EP				RGB(59,212,80)
#define SKILLWND_COLOR_RP				RGB(255,162,0)
#define SKILLWND_COLOR_CANLEARN			RGB(0xFF,0xBD,0x1D)
#define SKILLWND_COLOR_NOTLEARN			RGB(0x8C,0x8C,0x8C)
#define SKILLWND_COLOR_CANUPGRADE		RGB(0xFF,0xBD,0x1D)
#define SKILLWND_COLOR_MASTERED			INFOCOLOR_0
#define SKILLWND_COLOR_COST				RGB(0xFF,0xBD,0x1D)
#define SKILLWND_COLOR_SP				RGB(0xFF,0xBD,0x1D)
#define SKILLWND_COLOR_ACTION_COMMANDS	INFOCOLOR_0
#define SKILLWND_COLOR_ACTION_TEXT		INFOCOLOR_0

#define SKILLWND_ITEM_LEARN_COLOR_RED		255
#define SKILLWND_ITEM_LEARN_COLOR_GREEN		255
#define SKILLWND_ITEM_LEARN_COLOR_BLUE		255
#define SKILLWND_ITEM_NOT_LEARN_COLOR_RED	120
#define SKILLWND_ITEM_NOT_LEARN_COLOR_GREEN	120
#define SKILLWND_ITEM_NOT_LEARN_COLOR_BLUE	120
#define SKILLWND_ITEM_LEARN_ALPHA			255
#define SKILLWND_ITEM_NOT_LEARN_ALPHA		255

#define SKILLWND_EXP_FONT_SIZE				80

#define SKILLWND_ITEM_BAR_TITLE			"srfEXPTitle"
#define SKILLWND_ITEM_BAR_SURFACE		"srfSkillExpGauge"
#define SKILLWND_ITEM_BARBACK_SURFACE	"srfSkillExpGaugeBack"
#define SKILLWND_UNKNOWN_TEXNAME		"skill_not_learn.png"

#endif//__SKILL_COMMON_H__ 