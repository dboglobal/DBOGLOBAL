#ifndef __GUIDEV_APPLICATION__
#define __GUIDEV_APPLICATION__

#include "NtlApplication.h"
#include "NtlCamera.h"

#include "gui_precomp.h"
#include "guicore_precomp.h"

class CGuiDevApplication : public CNtlApplication
{
protected:
	CNtlCamera		*m_pCamera;
	RtCharset*		m_pCharset;
	RwRGBA			m_BackgroundColor;
	RwRGBA			m_ForegroundColor;

	// gui 
	gui::CResourceManager *m_pResMgr;
	gui::CSurfaceManager *m_pSurMgr;
	gui::CComponentManager *m_pCompMgr;
	gui::CGUIManager *m_pGuiMgr;
	
	gui::CDialog *m_pThis;

	std::string	m_strSurPage;
	std::string	m_strResPage;
	std::string	m_strCompPage;

	COMPPAGE_HANDLE m_hCompPage;

	gui::CSlot m_slotReturn;
	gui::CSlot m_slotTabChanged;

	gui::CSlot m_slotBtnClick;
	gui::CSlot m_slotBtnPressed;
	gui::CSlot m_slotBtnReleased;
	gui::CSlot m_slotBtnToggled;	

	gui::CHtmlBox*		m_pHtml;
	std::list<gui::CPanel*> m_listPoint;
	gui::CStaticBox*	m_pMovingText;
	gui::CFlash*		m_pFlash;

protected:
	
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

void Logic_CallbackHtmlLinkTag(gui::CComponent* pComponent, RwUInt8 byType, RwUInt32 uiIndex, std::wstring& strFilename);

#endif
