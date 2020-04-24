#ifndef __GUI_COMPONENT_GENERIC_H__
#define __GUI_COMPONENT_GENERIC_H__

#include "rectangle.h"
#include "position.h"
#include "inputdevice.h"
#include "key.h"
#include "gui_define.h"
#include "gui_componentoption.h"
#include "gui_signals.h"

START_GUI

class CComponent;
class CGUIManager;
class CSurfaceManager;

class CComponent_Generic
{
public:
	CComponent_Generic(	CComponent *pOwner,	CComponent *pParent, CSurfaceManager *pSurfaceManager );
	~CComponent_Generic();

	VOID SetOptions(const CComponentOptions &options);
	VOID FindPreferredSize(VOID);
	VOID PaintChildren(VOID);
	VOID CalcClippingRect(VOID);

	std::string m_strPage;
	std::string m_strID;
	std::string m_strDescription;
	std::wstring m_wstrToolTip;
	CComponent *m_pOwner;
	CComponent *m_pParent;

	CRectangle m_rtPosition;

	CSurfaceManager *m_pSurfaceMgr;
	static CGUIManager *m_pGuiMgr;

	bool m_bCaptured;
	bool m_bVisible;
	bool m_bEnabled;
	bool m_bPopup;
	bool m_bRemoveflag;
	bool m_bRenderTop;	// top level rendering 할것인가?
	
	int  m_nTabID;
	WORD m_wPriority;	// 우선순위. 큰 순서대로 화면 앞에 찍히게 된다. `
						// 작은 녀석이 큰녀석 앞으로 갈 수 없음. 기본값 0x7fff ( 32767 ). 

	CRectangle	m_rtClipping;	// 부모 영역 밖에 있는 영역을 잘라낸 스크린좌표. 나중에 Surface에 RwIm2DVertex를 포함하게 되면, 상대좌표로 바꾸고 RwIm2DVertex를 직접 바꾼다.
	bool		m_bClipping;	

	BYTE		m_ucAlpha;
	DWORD		m_uiColor;

	std::list<CComponent*> m_stlChildren;
	std::list<CComponent*> m_stlDeleteChildren;
	
	CSignal_v1<const CComponentOptions &> m_SigSetOptions;
	CSignal_v0 m_SigPaint;
	CSignal_v0 m_SigChildPaint;
	CSignal_v3<CComponent*, CInputDevice *, const CKey &> m_SigKeyDown;
	CSignal_v3<CComponent*, CInputDevice *, const CKey &> m_SigKeyUp;
	CSignal_v3<int, int, int> m_SigMouseMove;
	CSignal_v3<int, short, CPos&> m_SigMouseWheel;
	CSignal_v1<CComponent*> m_SigMouseEnter;
	CSignal_v1<CComponent*> m_SigMouseLeave;
	CSignal_v1<const CKey &> m_SigMouseDown;
	CSignal_v1<const CKey &> m_SigMouseUp;
	CSignal_v2<int, int> m_SigTracking;
	CSignal_v2<const CKey &,int& > m_SigDragDropQuery;
	CSignal_v4<CComponent *,const CKey &,int,bool&> m_SigDragDropRevote;
	CSignal_v1<int> m_SigDragDropNotProc;
	CSignal_v0 m_SigGotFocus;
	CSignal_v0 m_SigLostFocus;
	CSignal_v2<int, int> m_SigResize;
	CSignal_v2<int, int> m_SigMove;
	CSignal_v2<int &, int &> m_SigTransformCoords;
	CSignal_v1<CComponent*> m_SigBeforeRemove;
	CSignal_v1<CComponent*> m_SigChildAdd;
	CSignal_v1<CComponent*> m_SigChildRemove;
	CSignal_v1<CComponent*> m_SigChildBeforeAdd;
	CSignal_v1<CComponent*> m_SigChildBeforeRemove;
	CSignal_v1<CPos &> m_SigGetMinimumSize;
	CSignal_v1<CPos &> m_SigGetMaxiumSize;
	CSignal_v1<CPos &> m_SigGetPreferredSize;
	CSignal_v3<int,int,int>	m_SigSendMsg; 
	CSignal_v1<unsigned char> m_SigSetAlpha;
	CSignal_v3<unsigned char, unsigned char, unsigned char> m_SigSetColor;

	CSlot m_SlotSetOptions;
	CSlot m_SlotPaintChildren;
	CSlot m_SlotMouseEnter;
	CSlot m_SlotMouseLeave;	

	CTimerHandle m_hTimer;

	VOID SetToolTip( const std::wstring& wstrToolTip );
	VOID ShowToolTip(VOID);
	VOID HideToolTip(VOID);
	VOID CheckToolTipHide(VOID);

private:
	VOID OnTimer(VOID);
	VOID OnMouseEnter( CComponent* pComponent );
	VOID OnMouseLeave( CComponent* pComponent );		
};

END_GUI

#endif
