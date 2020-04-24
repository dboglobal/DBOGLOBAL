////////////////////////////////////////////////////////////////////////////////
// File: BuffDispObject
// Desc: Buff Gui Object. 
//		 - 중복 생성이 가능하도록 구현.
//
//		 - 1. Show(), Update(), Move()등은 포함하는 객체에서 연결해 주어야 함. 
//
// 2007.05.07 Peessi@hitel.net   
////////////////////////////////////////////////////////////////////////////////

#ifndef __BUFF_DISP_OBJECT_H__
#define __BUFF_DISP_OBJECT_H__

#include "gui_precomp.h"
#include "guicore_precomp.h"
#include "SurfaceGui.h"
#include "DialogDefine.h"

class CNtlBuffContainer;
class CNtlFakeBuffContainer;
class CNtlSobBuff;
class CNtlSobBuffAttr;
struct sNtlFakeBuff;
enum eBUFF_TYPE;

class CBuff
{
public:
	//! Enumeration
	enum TYPE { TYPE_NONE, TYPE_SKILL, TYPE_ITEM };

	//! Constructor & Destructor
	CBuff(VOID) { m_eType = TYPE_NONE; }
	virtual ~CBuff(VOID) {}

	virtual VOID SetBuff( void* pBuff, TYPE eType ) {}
	virtual VOID ShowInfoWnd( RwInt32 nParentX, RwInt32 nParentY, RwInt32 nDlgType ) {}
	virtual VOID DropBuff(VOID) {}

	VOID SetMove( RwInt32 nScreenX, RwInt32 nScreenY );
	VOID SetRect( int nX, int nY, RwInt32 nSize, gui::CSurface& KeepTimeSurface, gui::CSurface& DisableSurface);

	virtual VOID Update( RwReal fElapsedTime ) {}
	VOID Render(VOID);

	//! Variable
	CRectangle			m_Rect;
	CSurfaceGui			m_Surface;
	TYPE				m_eType;

	CSurfaceGui			m_surDisableSlot;

	gui::CRadarEffect	m_KeepTimeEffect;
};

class CSobBuff : public CBuff
{
public:
	CSobBuff(VOID) { m_pBuff = NULL; }
	virtual ~CSobBuff(VOID) { m_pBuff = NULL; }

	VOID	SetBuff( void* pBuff, TYPE eType );
	VOID	ShowInfoWnd( RwInt32 nParentX, RwInt32 nParentY, RwInt32 nDlgType );
	VOID	DropBuff(VOID);

	virtual VOID	Update( RwReal fElapsedTime );

	CNtlSobBuff*	m_pBuff;
};

class CFakeBuff : public CBuff
{
public:
	CFakeBuff(VOID) { m_pBuff = NULL; }
	virtual ~CFakeBuff(VOID) {}

	VOID	SetBuff( void* pBuff, TYPE eType );
	VOID	ShowInfoWnd( RwInt32 nParentX, RwInt32 nParentY, RwInt32 nDlgType );

	virtual VOID	Update( RwReal fElapsedTime );

	sNtlFakeBuff*	m_pBuff;
};

class CBuffDispBar
{
public:
	//! Constructur & Destructor
	CBuffDispBar(eBUFF_TYPE eBuffType);
	~CBuffDispBar(VOID);

	//! Operation
	VOID	Create( RwInt32 nBuffCount, eDialogType eDlgType, RwInt32 nIconSize, RwInt32 eBuffOrigin );
	VOID	Destroy(VOID);

	VOID	Update( RwReal fElapsedTime );	

	VOID	SetBuff( VOID* pBuff, CBuff::TYPE eType, RwInt32 nSlot );
	VOID	SetMove( RwInt32 nScreenX, RwInt32 nScreenY );
	VOID	SetSize( RwInt32 nWidth, RwInt32 nHeight );
	VOID	Show( bool bShow );

	VOID	RefreshInfoWnd(VOID);

	VOID	SetBuffRect(int nIconSize, int nBuffPerRowCnt);

	int		GetColumn();
	int		GetBuffPerRowCnt() { return m_nBuffPerRowCnt; }

	gui::CDialog*	GetDialog() { return m_pDialog; }

protected:
	//! Implementation

	//! CallBack
	VOID	OnMouseDown( const CKey& key );
	VOID	OnMouseMove( RwInt32 nKey, RwInt32 nXPos, RwInt32 nYPos );
	VOID	OnMouseLeave( gui::CComponent* pComponent );
	VOID	OnMove( RwInt32 nX, RwInt32 nY );
	VOID	OnPaint(VOID);

	gui::CSlot		m_slotMouseDown;
	gui::CSlot		m_slotMouseMove;
	gui::CSlot		m_slotMouseLeave;
	gui::CSlot		m_slotMove;
	gui::CSlot		m_slotPaint;

	//! Variable
	gui::CDialog*	m_pDialog;
	eDialogType		m_eDlgType;

	CBuff**			m_ppArrBuff;
	int				m_nBuffCnt;
	int				m_nBuffPerRowCnt;

	RwInt32			m_nMouseOnIndex;

	eBUFF_TYPE		m_eBuffType;
};

class CBuffDispGui
{
public: 
	//! Enumuration
	enum ICONSIZE	{ SMALL_ICONSIZE = 16, NORMAL_ICONSIZE = 20 };
	enum ORIGIN		{ SOBBUFF, FAKEBUFF };
		
	//! Contructor & Destructor
	CBuffDispGui(VOID);
	~CBuffDispGui(VOID);

	//! Operation
	VOID	Create(BYTE byShowBuffFlag, ICONSIZE eIconSize, RwInt32 nOffsetX, RwInt32 nOffsetY, eDialogType eDlgType, ORIGIN eOrigin = SOBBUFF );
	VOID	Destroy(VOID);

	VOID	Update( RwReal fElapsedTime );

	VOID	SetBuffAll( RwUInt32 hSerial );
	VOID	SetBuffAll( CNtlFakeBuffContainer* pFakeBuffContainer );
	VOID	ClearBuff(VOID);
	
	VOID	SetMove( CRectangle& rtScreen );	// 속해 있는 GUI의 스크린 좌표를 넘겨주는 것으로 OK.
	VOID	Show( bool bShow );					// 속해 있는 GUI의 Show시 같이 지정을 해 주도록.

	RwBool	IsShow() { return m_bShow; }

	void	UpdateBarLocation();
	
protected:
	//! Implementation
	VOID	BuffRegisterProc( CNtlBuffContainer* pBuffContainer );
	VOID	BuffRegisterProc( CNtlFakeBuffContainer* pFakeBuffContainer	);
	VOID	BuffUnregisterProc(VOID);
	VOID	RegisterBuffToSlot( VOID* pBuff, eBUFF_TYPE eBuffAttrType, CBuff::TYPE eBuffOriginType, RwInt32 nIndex );
	VOID	FitBuffBarRect( RwInt32 nBlessBuffCnt, RwInt32 nCurseBuffCnt, int nSubBuffCnt );

	CBuffDispBar*	GetBuffDispBar(BYTE byBuffBar);

	//! Variable

	CBuffDispBar*	m_pBuffDisplayBless;
	CBuffDispBar*	m_pBuffDisplayCurse;
	CBuffDispBar*	m_pBuffDisplaySub;

	BYTE			m_byShowBuffFlag;

	ICONSIZE		m_eIconSize;
	RwInt32			m_nOffsetX;					// The coordinate difference with Gui containing this object.
	RwInt32			m_nOffsetY;	
	eDialogType		m_eDlgType;
	ORIGIN			m_eOrigin;
	unsigned int	m_hOwnerSerial;
	CNtlBuffContainer*	m_pBuffContainer;

	RwReal			m_fUpdateInfoTime;	

	RwBool			m_bShow;
	RwBool			m_bEnable; // is this required?
};

#endif//__BUFF_DISP_OBJECT_H__