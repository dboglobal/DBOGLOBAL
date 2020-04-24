#ifndef __GUIDEV_APPLICATION__
#define __GUIDEV_APPLICATION__

#include "NtlApplication.h"
#include "NtlCamera.h"

#include "gui_precomp.h"
#include "guicore_precomp.h"

class CTableContainer;
class CSkillCustomizeParser;

class CGuiDevApplication : public CNtlApplication
{
protected:
	CNtlCamera*		m_pCamera;
	RtCharset*		m_pCharset;
	RwRGBA			m_BackgroundColor;
	RwRGBA			m_ForegroundColor;

	// gui 
	gui::CResourceManager*	m_pResMgr;
	gui::CSurfaceManager*	m_pSurMgr;
	gui::CComponentManager*	m_pCompMgr;
	gui::CGUIManager*		m_pGuiMgr;
	
	gui::CDialog*			m_pThis;
	gui::CDialog*			m_pSkillCustomize;
	gui::CInputBox*			m_pInput;
	gui::CButton*			m_pbtnBuild;
	gui::CButton*			m_pbtnActive;
	gui::CScrollBar*		m_pscbScroll;
	gui::CStaticBox*		m_pstbError;

	std::string	m_strSurPage;
	std::string m_strSkillPage;
	std::string	m_strResPage;
	std::string m_strSkillResPage;
	std::string	m_strCompPage;

	COMPPAGE_HANDLE m_hCompPage;

	gui::CSlot m_slotReturn;
	gui::CSlot m_slotBtnClick;	
	gui::CSlot m_slotActiveBtnClick;
	gui::CSlot m_slotScrollChange;
	gui::CSlot m_slotSliderMove;
	gui::CSlot m_slotCaptureWheelMove;
	gui::CSlot m_slotResizeCustomize;

	RwBool	m_bActiveBtn;

	//
	CTableContainer* m_pTableContainer;
	CSkillCustomizeParser* m_pSkillCustomizeParser;

protected:
	VOID	OnBtnClick( gui::CComponent* pComponent );
	VOID	OnActiveBtnClick( gui::CComponent* pComponent );
	VOID	OnInputReturn(VOID);
	VOID	OnScrollChanged( RwInt32 nNewOffset );
	VOID	OnCaptureWheelMove( RwInt32 nFlag, RwInt16 sDelta, CPos& pos );
	VOID	OnResizeSkillCustomize( RwInt32 nOldWidth, RwInt32 nOldHeight );

	VOID	ParseSkillCustomizeFile( RwChar* szFileName );
	
	
	RwBool	CreateSubSystem(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer);
	LRESULT	PreTranslateMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
	RwBool	Update(RwReal fTime, RwReal fElapsedTime);
	RwBool	Resize(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer);

	void	DisplayInfo();
	RwBool	AttachPlugin();
	
public:

	CGuiDevApplication(void);
	~CGuiDevApplication(void);

	virtual void   Destroy();

	virtual LRESULT WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	static CGuiDevApplication * m_pInstance;
};

#endif
