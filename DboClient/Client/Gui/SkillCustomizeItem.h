////////////////////////////////////////////////////////////////////////////////
// File: SkillCustomizeItem.h
// Desc: SkillCustomize 
//
// 2008.11.5 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_CUSTOMIZE_ITEM_H__
#define __SKILL_CUSTOMIZE_ITEM_H__

#include "SurfaceGui.h"
#include "NtlSharedType.h"

class CNtlSobIcon;
class CNtlSobSkillAttr;
class CSkillCustomizeSkillItem;
class CSkillCustomizeParser;
struct sTBLDAT;

struct stSkillUpgradeInfo
{
	stSkillUpgradeInfo(VOID){ Init(); }

	VOID Init(VOID)
	{
		bLearn = FALSE; bMaster = FALSE; bSelfLearnEnable = FALSE;
		byCurrentLevel = 0; uiNeedSP = 0; bSPOK = FALSE; 
		RequireSkillIdx1 = INVALID_TBLIDX; bSkill1OK = FALSE;
		RequireSkillIdx2 = INVALID_TBLIDX; bSkill2OK = FALSE;
		byRequreAvatarLevel = 0; bAvatarLevelOK = FALSE; 
		bMasteryOK = TRUE; bClassOK = TRUE;
	}

	RwBool IsEnableLearn(VOID)
	{
		return ( bSPOK && bSkill1OK && bSkill2OK && bAvatarLevelOK && bMasteryOK && bClassOK );
	}

	RwBool		bLearn;
	RwBool		bMaster;
	RwBool		bSelfLearnEnable;
	RwUInt8		byCurrentLevel;	
			
	RwUInt32	uiNeedSP;
	RwBool		bSPOK;
	TBLIDX		RequireSkillIdx1;
	RwBool		bSkill1OK;
	TBLIDX		RequireSkillIdx2;
	RwBool		bSkill2OK;		
	RwUInt8		byRequreAvatarLevel;
	RwBool		bAvatarLevelOK;	
	RwBool		bMasteryOK;
	RwBool		bClassOK;
};

class CSkillCustomizeItem
{
public:
	//! Enumration
	enum	ICONSTATE { STATE_NONE, STATE_CLICK, STATE_CLICK_BUT_OUTSIDE };
	//! Constructor & Destructor
	CSkillCustomizeItem( CSkillCustomizeSkillItem* pParsedItem );
	~CSkillCustomizeItem(VOID);

	//! Operation
	VOID	Update( RwReal fElapsed );		

	VOID	SetInitSkill(VOID);
	VOID	SetSobIcon( CNtlSobIcon* pSobIcon, sTBLDAT* pData );
	VOID	SetRPType( CNtlSobSkillAttr* pSobSkillAttr );
	VOID	CheckSkillUpgrade(VOID);

	VOID	Show( bool bShow );
	VOID	SetAlpha( RwUInt8 byAlpha );
	VOID	SetColor( RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue );
	VOID	SetActive( RwBool bActive );

	VOID	CoolTimeEffect( RwBool bStart );
	void	SelectEffect(bool bStart);
	VOID	ClickEffect( RwBool bPush, RwBool bStillPush = TRUE );
	VOID	ShowPickedUp( RwBool bShow );

	RwInt32	GetBottomPos(VOID);

	/// woosungs_test 20090730
	RwBool	IsUpgradeble();
	RwBool	IsTrainableSkill();
	RwBool	IsSelfTrainableSkill();

	void	SetLearnAbleSkillBack(bool bActive);

	bool	CanSelect();

private:
	//! Implementation
	VOID	ShowSkillInfownd( RwBool bShow );
	VOID	ShowSPInfownd( RwBool bShow );
	
	//! Callback
	VOID	OnIconMouseUp( const CKey& key );
	VOID	OnIconMouseDown( const CKey& key );	
	VOID	OnIconMouseEnter( gui::CComponent* pComponent );
	VOID	OnIconMouseLeave( gui::CComponent* pComponent );
	VOID	OnIconPaint(VOID);
	VOID	OnIconMove( RwInt32 nOldX, RwInt32 nOldY );

	VOID	OnSPButtonEnter( gui::CComponent* pComponent );
	VOID	OnSPButtonLeave( gui::CComponent* pComponent );
	VOID	OnSPButtonClick( gui::CComponent* pComponent );
	VOID	OnRPButtonClick( gui::CComponent* pComponent );
	
	gui::CSlot			m_slotIconMouseUp;
	gui::CSlot			m_slotIconMouseDown;
	gui::CSlot			m_slotIconMouseEnter;
	gui::CSlot			m_slotIconMouseLeave;
	gui::CSlot			m_slotIconPaint;
	gui::CSlot			m_slotIconMove;

	gui::CSlot			m_slotSPButtonEnter;
	gui::CSlot			m_slotSPButtonLeave;
	gui::CSlot			m_slotSPButtonClick;
	gui::CSlot			m_slotRPButtonClick;	

	//! Variables
	ICONSTATE			m_eLClickIcon;	
	ICONSTATE			m_eRClickIcon;	
	RwBool				m_bPushDownIcon;

	RwReal				m_fRightClickTime;	// 오른쪽 버튼 눌렀을때의 시간. RPBonus적용

	gui::CRadarEffect	m_CoolTimeEffect;
	CSurfaceGui			m_surRPType;
	CSurfaceGui			m_surFocusEffect;
	CSurfaceGui			m_surPickedUp;
	gui::CFlash*		m_pflaSelectEffect;

	CSurfaceGui			m_surLearnAbleEffect;

	RwBool				m_bUpgradable;

	CNtlSobIcon*				m_pSobIcon;
	CSkillCustomizeSkillItem*	m_pItem;

	stSkillUpgradeInfo	m_stSkillUpgradeInfo;	// 업그레이드 정보. (Infownd용 데이터)
};

class CSkillCustomizeItemGroup
{
public:
	//! Type
	typedef std::map<RwUInt32,CSkillCustomizeItem*>				MAP_SKILL_ITEM;
	typedef std::map<RwUInt32,CSkillCustomizeItem*>::iterator	MAP_SKILL_ITEM_ITER;

	//! Constructor & Destructor
	CSkillCustomizeItemGroup( gui::CComponent* pParent, gui::CScrollBar* pScrollBar );
	~CSkillCustomizeItemGroup(VOID);

	VOID GenerateSkillItem(  const RwChar* szFileName, RwUInt32 flagClass );
	VOID ClearSkillItem(VOID);

	VOID Update( RwReal fElapsed );

	VOID Show( bool bShow );
	VOID SetInitSkill(VOID);
	VOID SetSkillItem( RwUInt32 uiBaseSkillIndex, CNtlSobIcon* pSobIcon, sTBLDAT* pData );
	VOID SetRPType( RwUInt32 uiBaseSkillIndex, CNtlSobSkillAttr* pSobSkillAttr );
	VOID CheckUpgradebleSkill(VOID);
	VOID CheckSkillGroupShowAndSize(VOID);	// 현재 스킬이 보여지는 만큼 라인 Show, 사이즈 조절.

	VOID SetOffset( RwInt32 nYOffset );						
	VOID CalcScrollBar(VOID);

	VOID HidePickedUp(VOID);
	VOID CoolingEffectProc( RwUInt32 uiBasetblidx, RwBool bStart );
	void SelectEffectProc(RwUInt32 uiBasetblidx, bool bStart);

	RwUInt32 GetClassFlag(VOID) { return m_flagClass; }
	
	/// woosungs_test 20090730	
	RwBool IsExistUpgradebleSkill();
	RwBool IsExistTrainableSkill();
	RwBool IsExistSelfTrainableSkill();

private:
	gui::CDialog*			m_pDialog;
	gui::CScrollBar*		m_pScrollBar;
	CSkillCustomizeParser*	m_pParser;

	CRectangle				m_rtOriginal;
	RwUInt32				m_flagClass;

	MAP_SKILL_ITEM			m_mapSkillItem;
};

#endif