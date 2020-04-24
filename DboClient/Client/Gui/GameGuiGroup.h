/*****************************************************************************
 *
 * File			: GameGuiGroup.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 9. 20	
 * Abstract		: DBO Game stage gui group.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __GAME_GUI_GROUP_H__
#define __GAME_GUI_GROUP_H__

#include "ceventhandler.h"
#include "gui_precomp.h"

class CHpGui;
class CChatGui;
class CInventoryGui;
class CQuickSlotGui;
class CExpGui;
class CMainMenuGroup;
class CESCMenuGui;
class CMinimapGui;
class CTargetHpGui;
class CMsgBoxGui;
class CStatusWindowGui;
class CBagSlotGui;
class CSkillCustomizeGui;
class CPopupMenuGui;
class CCalcPopupGui;
class CTradeCart;
class CNPCShop;
class CScouterMeasureDlgGui;
class CScouterPCInfoViewerGui;
class CSubGaugeGui;
class CCommunityGui;
class CQuestGui;
class CItemChangeBattleAttributeGui;
class CWarehouseBarGui;
class CWarehouseGui;
class CWarehouseCommonGui;
class CCautionNotifyGui;
class CTerritoryNotifyGui;
class CPublicNotifyGui;
class CActionNotifyGui;
class CGroupNotifyGui;
class CWorldMapGui;
class COptionWindowGui;
class CHelpWindowGui;
class CDropItemInfoGui;
class CSideIconGui;
class CPrivateShopGui;
class CPrivateShopCartGui;
class CRBBoardGui;
//class CRBChallengeGui;
class CTMQBoardGui;
class CUltimateDungeonGui;
class CMailSystemGui;
class CMailSystemReadGui;
class CCinematicGui;
class CBackboardGui;
class CPortalGui;
class CObjectClickGui;
class CFlashNotifyGui;
class CFriendListGui;
class CInterfaceShakeGui;
class CRankBoardGui;
class CCommuTargetGui;
class CTimeNotifyGui;
class CSideDialogControlGui;
class CTutorialDialogFocus;
class CTutorialExampleGui;
class CTutorialMessageGui;
class CTutorialConfirmGui;
class CTBGambleResultGui;
class CChannelChangeGui;
class CGroupChatManager;
class CGMChatGui;
class CPetitionGui;
class CPetitionSatisfaction;
class CBroadCastGui;
class CPartyMenu;
class CNameNotifyGui;
class CSkillAbilitySetGui;
class CSkillAbilityUseGui;
class CSkillHTBRpResult;
class CSkillHtbRpUse;
class CVehicleGui;
class CDeveloperPreviewGui;
class CDojoInfoGui;
class CDogiGui;
class CDojoUpradeGui;
class CScrambleStatusGui;
class CScrambleNotifyGui;
class CNetMarblePCBangGui;
class CNetPyShopGui;
class CNetPyShopCartGui;
class CDeliberationRankContainer;
class CCommercialExtendGui;
class CItemBindSealingGui;
class CHLShopGui;
class CYardratWarehouseGui;
class CHLShopBuyGui;
class CHLShopBuyConfirmGui;
class CHLShopGiftGui;
class CHLShopGiftConfirmGui;
class CItemSwapGui;
class CItemSwapCartGui;
class CAuctionHouseGui;
class CItemUpgradeGui;
class CItemOptionRestructureGui;
class CQuickTeleportGui;
class CPlayerTitleGui;
class CCCBDBoardGui;
class CCCBDNotifyGui;
class CCCBDRewardGui;
class CHoiPoiMixRecipeGui;
class CHoiPoiMixCraftGui;

class CGMGuiGroup;

class CGameGuiGroup : public RWS::CEventHandler
{
private:

	CHpGui*						m_pHp;
	CChatGui*					m_pChat;
	CQuickSlotGui*				m_pQuickSlot;
	CExpGui*					m_pExp;
	CMainMenuGroup*				m_pMainMenu;
	CESCMenuGui*				m_pESCMenu;
	CMinimapGui*				m_pMinimap;
	CTargetHpGui*				m_pTargetHp;
	CStatusWindowGui*			m_pStatusWindow;
	CBagSlotGui*				m_pBagSlot;
	CSkillCustomizeGui*			m_pSkillWindow;
	CPopupMenuGui*				m_pPopupMenu;
	CCalcPopupGui*				m_pCalcPopup;
	CTradeCart*					m_pTradeCart;
	CNPCShop*					m_pNPCShop;
	CScouterMeasureDlgGui*		m_pScouterMeasureDlg;
	CScouterPCInfoViewerGui*	m_pScouterPCInfoViewer;
	CSubGaugeGui*				m_pSubGauge;
	CCommunityGui*				m_pCommunity;
	CPlayerTitleGui*			m_pPlayerTitle;
	CQuestGui*					m_pQuest;
	CItemChangeBattleAttributeGui*			m_pItemChangeBattleAttribute;
	CItemUpgradeGui*			m_pItemUpgrade;
	CWarehouseBarGui*			m_pWarehouseBar;
	CWarehouseGui*				m_pWarehouse_1;
	CWarehouseGui*				m_pWarehouse_2;
	CWarehouseGui*				m_pWarehouse_3;
	CWarehouseCommonGui*		m_pWarehouseCommon;
	CCautionNotifyGui*			m_pCautionNotify;
	CTerritoryNotifyGui*		m_pTerritoryNotify;
	CPublicNotifyGui*			m_pPublicNotify;
	CActionNotifyGui*			m_pActionNotify;
	CGroupNotifyGui*			m_pGroupNotify;
	CWorldMapGui*				m_pWorldMap;
	COptionWindowGui*			m_pOptionWindowGui;
	CHelpWindowGui*				m_pHelpWindowGui;
	CDropItemInfoGui*			m_pDropItemInfoGui;
	CSideIconGui*				m_pSideIconGui;
	CPrivateShopGui*			m_pPrivateShopGui;
	CPrivateShopCartGui*		m_pPrivateShopCartGui;
	CRBBoardGui*				m_pRBBoardGui;
	/*CRBChallengeGui*			m_pRBChallengeGui;*/
	CTMQBoardGui*				m_pTMQBoard;
	CUltimateDungeonGui*		m_pUDBoard;
	CMailSystemGui*				m_pMailSystemGui;
	CMailSystemReadGui*			m_pMailSystemReadGui;
	CCinematicGui*				m_pCinematicGui;
	CBackboardGui*				m_pBackboardGui;
    CPortalGui*                 m_pPortalGui;
	CObjectClickGui*			m_pObjectClickGui;
	CFlashNotifyGui*			m_pFlashNotify;
    CFriendListGui*             m_pFriendListGui;
	CInterfaceShakeGui*			m_pInterfaceShakeGui;
	CRankBoardGui*				m_pRankBoardGui;
    CCommuTargetGui*            m_pCommuTargetGui;
	CTimeNotifyGui*				m_pTimeNotifyGui;
	CSideDialogControlGui*		m_pSideDialogControl;
	CTutorialDialogFocus*		m_pTutorialDialogFocus;
	CTutorialExampleGui*		m_pTutorialExample;
	CTutorialMessageGui*		m_pTutorialMessage;
	CTutorialConfirmGui*		m_pToturialConfirm;
	CTBGambleResultGui*			m_pTBGambleResult;
	CChannelChangeGui*			m_pChannelChange;
	CGroupChatManager*			m_pGroupChatManager;
	CGMChatGui*					m_pGMChatting;
	CPetitionGui*				m_pPetition;
	CPetitionSatisfaction*		m_pPetitionSatisfaction;
	CBroadCastGui*				m_pBroadCastGui;
	CPartyMenu*					m_pPartyMenu;	
	CNameNotifyGui*				m_pNameNotify;
	CSkillAbilitySetGui*		m_pSkillAbilitySetGui;
	CSkillAbilityUseGui*		m_pSkillAbilityUseGui;
	CSkillHTBRpResult*			m_pHTBRpResult;
	CSkillHtbRpUse*				m_pHTBRpUse;
	CVehicleGui*				m_pVehicle;
	CDeveloperPreviewGui*		m_pDeveloperPreviewGui;
	CDojoInfoGui*				m_pDojoInfoGui;
	CDogiGui*					m_pDogiGui;
	CDojoUpradeGui*				m_pDojoUpgrade;
	CScrambleStatusGui*			m_pScrambleStatusGui;
	CScrambleNotifyGui*			m_pScrambleNotify;
    CNetMarblePCBangGui*        m_pNetMablePCBangGui;
	CNetPyShopGui*				m_pNetPyShopGui;
	CNetPyShopCartGui*			m_pNetPyShopCartGui;
	CDeliberationRankContainer*		m_pDeliverationRankContainer;
	CCommercialExtendGui*		m_pCommercialExtendGui;

	CGMGuiGroup*				m_pGMGuiGroup;

	CItemBindSealingGui*		m_pItemBindSealingGui;
	CHoiPoiMixRecipeGui*		m_pHoiPoiMixRecipeGui;
	CHoiPoiMixCraftGui*			m_pHoiPoiMixCraftGui;
	CHLShopGui*					m_pHLShopGui;
	CYardratWarehouseGui*		m_pYardratWarehouseGui;
	CHLShopBuyGui*				m_pHLShopBuyGui;
	CHLShopBuyConfirmGui*		m_pHLShopBuyConfirmGui;
	CHLShopGiftGui*				m_pHLShopGiftGui;
	CHLShopGiftConfirmGui*			m_pHLShopGiftConfirmGui;
	CItemSwapGui*				m_pItemSwapGui;
	CItemSwapCartGui*			m_pItemSwapCartGui;

	CAuctionHouseGui*			m_pAuctionHouseGui;
	CItemOptionRestructureGui*	m_pItemOptionRestructureGui;
	CQuickTeleportGui*			m_pQuickTeleportGui;

	CCCBDBoardGui*				m_pCCBDBoardGui;
	CCCBDNotifyGui*				m_pCCBDNotifyGui;
	CCCBDRewardGui*				m_pCCBDRewardGui;

	gui::CSlot m_slotPaint;
		
public:

	CGameGuiGroup();
	~CGameGuiGroup();

	RwBool		Create(void);
	void		Destroy(void);

	void		OnPaint(void);

	virtual void HandleEvents(RWS::CMsg &pMsg);

protected:
	void		LoadCommonScript();
	void		UnloadCommonScript();
};


#endif