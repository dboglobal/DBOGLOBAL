/******************************************************************************
* File			: SideIconGui.h
* Author		: Kim JinSung -> Cho HaeSung
* Copyright		: (주)NTL
* Date			: 2007. 6. 11
* Abstract		: 
*****************************************************************************
* Desc			: DBO 사이드 아이콘 GUI
*****************************************************************************/

#pragma once

#include "SideIconDefine.h"

// SideIcon들의 간격
#define dSIDEICON_MARGIN_WIDTH	5
#define dSIDEICON_MARGIN_HEIGHT	5

// Class 전방 선언
class CSideIconGui;
class CSideIconBase;
class CSideViewBase;

class CHintSideIconGui;
class CHelpHintSideViewGui;
class CGuideHintSideViewGui;
class CStringHintSideViewGui;

class CDBCSideViewGui;
class CDBCSideIconGui;

class CRBSideViewGui;
class CRBSideIconGui;

class CDoRepairSideViewGui;
class CDoRepairSideIconGui;

class CTutorialSideIconGui;
class CTutorialSideViewGui;

class CCautionSideIconGui;
class CCautionSideViewGui;

class CBudokaiSideIconGui;
class CBudokaiSideViewGui;

class CPetitionSideIconGui;
class CPetitionSideViewGui;

class CGMSideIconGui;
class CGMSideViewGui;

class CDojoSideIconGui;
class CDojoSideViewGui;

class CSCSSideIconGui;
class CSCSSideViewGui;

class CHLSSideIconGui;
class CHLSSideViewGui;


/**
* \ingroup client
* \brief DBO상의 사이드 아이콘들을 생성 및 관리한다. (싱글톤)
*/
class CSideIconGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	typedef std::vector<CSideIconBase*>						SIDEICONVEC;
	typedef std::map<const eSideIconType, CSideIconBase*>	SIDEICONMAP;
	typedef std::map<const eSideViewType, CSideViewBase*>	SIDEVIEWMAP;

public:
	CSideIconGui(const RwChar* pName);
	virtual ~CSideIconGui();

	RwBool			Create();
	void			Destroy();

	void			HandleEvents(RWS::CMsg &msg);
	RwInt32			SwitchDialog(bool bOpen);

	void			OnMove(RwInt32 iOldX, RwInt32 iOldY);

	CSideIconBase*	GetSideIcon(const eSideIconType& eSideIcon);
	CSideViewBase*	GetSideView(const eSideViewType& eSideView);

	eSideViewType	GetPresentView(void) { return m_nPresentViewType; }

	void			SortSideIcon();
	static bool		SortSideIconCompare(CSideIconBase* pLeft, CSideIconBase* pRight);

	void			RocateSideIcon();

	void			OnPressESC();
	void			OnSideIconShow(CSideIconBase* pSideIcon, bool bShow);
	void			OnSideViewShow(CSideViewBase* pSideView, bool bShow);

	void			OpenSideView(CSideIconBase* pSideIcon, const eSideViewType& eSideView, void* pData);
    void            CloseSideView(const eSideViewType eSideView);

	void			OnSideIconSend(const eSideIconType& eSideIcon, void* pData);
	void			OnSideViewSend(const eSideViewType& eSideView, void* pData);

protected:
	void			RegSideIcon(const eSideIconType& eSideIcon, CSideIconBase* pSideIcon);
	void			UnRegSideIcon(const eSideIconType& eSideIcon);

	void			RegSideView(const eSideViewType& eSideView, CSideViewBase* pSideView);
	void			UnRegSideView(const eSideViewType& eSideView);

protected:
	static CSideIconGui* s_pSideIconGui;

public:
	static CSideIconGui* GetInstance() { return s_pSideIconGui; }
	
protected:
	SIDEICONVEC			m_vecSideIcon;		// Number를 통해 정렬 한다.
	SIDEICONMAP			m_mapSideIcon;
	SIDEVIEWMAP			m_mapSideView;

	CSideIconBase*		m_pPresentIcon;
	CSideViewBase*		m_pPresentView;

	eSideViewType		m_nPresentViewType;	// 현재 보여지고 있는 뷰의 Enum

	gui::CSlot			m_slotMove;

	// DBO Side Icon & Side View
	CHintSideIconGui*			m_pHintSideIconGui;
	CHelpHintSideViewGui*		m_pHelpHintSideViewGui;
	CGuideHintSideViewGui*		m_pGuideHintSideViewGui;
	CStringHintSideViewGui*		m_pStringHintSideViewGui;

	CDBCSideIconGui*			m_pDBCSideIconGui;
	CDBCSideViewGui*			m_pDBCSideViewGui;

	CRBSideIconGui*				m_pRBSideIconGui;
	CRBSideViewGui*				m_pRBSideViewGui;

	CDoRepairSideIconGui*		m_pDoRepairSideIconGui;
	CDoRepairSideViewGui*		m_pDoRepairSideViewGui;

	CTutorialSideIconGui*		m_pTutorialSideIconGui;
	CTutorialSideViewGui*		m_pTutorialSideViewGui;

	CCautionSideIconGui*		m_pCautionSideIconGui;
	CCautionSideViewGui*		m_pCautionSideViewGui;

	CBudokaiSideIconGui*		m_pBudokaiSideIconGui;
	CBudokaiSideViewGui*		m_pBudokaiSideViewGui;

	CPetitionSideIconGui*		m_pPetitionSideIconGui;
	CPetitionSideViewGui*		m_pPetitionSideViewGui;

	CGMSideIconGui*				m_pGMSideIconGui;
	CGMSideViewGui*				m_pGMSideViewGui;

	CDojoSideIconGui*			m_pDojoSideIconGui;
	CDojoSideViewGui*			m_pDojoSideViewGui;

	CHLSSideIconGui*			m_pHLSSideIconGui;
	CHLSSideViewGui*			m_pHLSSideViewGui;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class : CSideIconBase
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CSideIconBase : public CNtlPLGui
{
private:
	bool			m_bDisplay;
	RwUInt32		m_uiNumber;	

public:
	CSideIconBase(const RwChar* pName);
	virtual ~CSideIconBase();

	virtual void	Show(bool bShow);
	void			SetNumber(const RwUInt32& uiNumber); // uiNumber가 작을 수록 사이드 오른쪽에 출력
	RwUInt32		GetNumber();

	bool			IsDisplay()	{ return m_bDisplay; }

	virtual void	OnSideIconReceive(void* pData);
	virtual void	OnSideViewClosed() = 0;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class : CSideViewBase
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CSideViewBase : public CNtlPLGui
{
public:
	CSideViewBase(const RwChar* pName);
	virtual ~CSideViewBase();

	virtual void	Show(bool bShow);

	virtual void	OnPressESC() = 0;
	virtual void	OnSideViewOpen(const void* pData) = 0;
	virtual void	OnSideViewReceive(void* pData);
	virtual void	OnSideViewClose() = 0;
	virtual void	OnSideViewLocate(const CRectangle& rectSideIcon) = 0;
};