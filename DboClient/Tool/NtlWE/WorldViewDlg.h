#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#define dWORLD_VIEW_TOP_MARGIN (25)

class CGamePropVisibleDlg;

enum EBLOCK_MODE
{
	EBLOCK_MODE_NONE	= 0,
	EBLOCK_MODE_BLOCK	= 1,
	EBLOCK_MODE_SECTOR	= 2,
	EBLOCK_MODE_FIELD	= 3,
};

class CWorldViewDlg : public CDialog
{
	DECLARE_DYNAMIC(CWorldViewDlg)

public:
	CWorldViewDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CWorldViewDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_WORLD_VIEW };

protected:
	virtual void	DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL	PreTranslateMessage(MSG* pMsg);
	virtual LRESULT	WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL	OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	void	InitializeWorldData();	

	void	CreateWorldViewResource();
	void	DestoryWorldViewResource();

	void	Render();
	void	RenderMainView();
	void	UpdateMainView();

	RwBool	IsSelectEmpty();

	RwInt32	GetBlockMode();
	RwInt32	GetBlockSize();

	RwV3d	GetSelectBlockMidPos(); 
	RwBool	GetSelectBlockPos(RwV3d* pvSup, RwV3d* pvInf);
	RwBool	GetSelectBlockNum(RwInt32* piNumX, RwInt32* piNumY);

	void	ResetSelectBlock();

	void	SetGuideBlock(RwBool bGuide)	{ m_BtnBlock.SetCheck(bGuide); UpdateData(FALSE); }
	RwBool	GetGuideBlock()					{ UpdateData(TRUE); return m_BtnBlock.GetCheck(); }
	void	SetGuideSector(RwBool bGuide)	{ m_BtnSector.SetCheck(bGuide); UpdateData(FALSE); }
	RwBool	GetGuideSector()				{ UpdateData(TRUE); return m_BtnSector.GetCheck(); }
	void	SetGuideField(RwBool bGuide)	{ m_BtnField.SetCheck(bGuide); UpdateData(FALSE); }
	RwBool	GetGuideField()					{ UpdateData(TRUE); return m_BtnField.GetCheck(); }

	void	ApplyFieldProp(sNTL_FIELD_PROP* pNtlFieldProp, eNTL_FIELD_PROPID eNtlFieldPropID);
	void	ApplyBlockProp(sNTL_BLOCK_PROP* pNtlBlockProp, eNTL_BLOCK_PROPID eNtlBlockPropID);

protected:
	RwBool	IsMouseOut();

	void	SetRwIm3DVertex(RwIm3DVertex* pRwIm3DVertex, RwReal fX, RwReal fY, RwReal fZ, RwReal fU, RwReal fV, RwUInt8 a, RwUInt8 r, RwUInt8 g, RwUInt8 b);

	void	RenderMainViewBBox(RwReal fRenderSize, RwCamera* pCamera, RwBool bRender, RwBool bRenderSelected, void (CWorldViewDlg::*CB_RENDER)(RwBool, RwBBox, RwUInt32));

	void	RenderBlockBBox(RwBool bSelected, RwBBox bboxRender, RwUInt32 uiFlags);
	void	RenderSectorBBox(RwBool bSelected, RwBBox bboxRender, RwUInt32 uiFlags);
	void	RenderFieldBBox(RwBool bSelected, RwBBox bboxRender, RwUInt32 uiFlags);

	void	RenderBlockTightBBox(RwReal fRenderSize, RwCamera* pCamera,
								RwBool bRender,
								RwUInt8 a, RwUInt8 r, RwUInt8 g, RwUInt8 b, RwBool bWire,
								RwBool bSelectedRender,
								RwUInt8 aS, RwUInt8 rS, RwUInt8 gS, RwUInt8 bS, RwBool bWireS);

	void	RenderVisibleProp(RwUInt32 uiVisibleProp, RwBBox* pBBox, RwReal fSize);

	void	RenderRwBBox(RwTexture* pRwTexture, RwBBox* pRwBBoxBlock, RwReal fWrapU, RwReal fWrapV, RwUInt8 a, RwUInt8 r, RwUInt8 g, RwUInt8 b, RwReal fHeight = 0.0f);

	void	UpdateSelectBlock(RwV3d* pvP1, RwV3d* pvP2);

	void	CalcSelectBlock(RwBBox* pRwBoxOut, RwBBox* pRwBoxIn, RwUInt32 iBlockSize);

	RwBool	CursorIntersectionWorld(RwBool bClipView, RwV3d* pvCollPos = NULL, RwReal* pfDist = NULL);
	RwBool	CursorIntersectionPlane(RwBool bClipView, RwV3d* pvCollPos = NULL, RwReal* pfDist = NULL);
	RwBool	PixelRayIntersectionWorld(RwV2d* pvPixelPos, RwV3d* pvCollPos = NULL, RwReal* pfDist = NULL);
	RwBool	PixelRayIntersectionPlane(RwV2d* pvPixelPos, RwV3d* pvCollPos = NULL, RwReal* pfDist = NULL);

protected:
	RpWorld*		m_pRpWorld;
	RwCamera*		m_pRwCamera;
	RpLight*		m_pRpLight;

	RwTexture*		m_pRwTexCamera;
	RwTexture*		m_pRwTexWorldMap;
	RwTexture*		m_pRwTexWorldTile;
	RwTexture*		m_pRwTexWorldTileSelect;
	RwTexture*		m_pRwTexWorldLoading;

	RwIm3DVertex	m_aRwIm3DVertex[4];

	RwV3d			m_vCameraPos;
	RwReal			m_fCameraScale;

	RwV3d			m_vMouseMoveStart;
	RwV3d			m_vMouseMoveEnd;

	RwV3d			m_vMouseSelectStart;
	RwV3d			m_vMouseSelectEnd;

	RwBBox			m_bboxWorld;

	RwBBox			m_bboxSelectBlock;
	RwBBox			m_bboxSelectBlockBlock;
	RwBBox			m_bboxSelectBlockSector;
	RwBBox			m_bboxSelectBlockField;

	CButton			m_BtnField;
	CButton			m_BtnSector;
	CButton			m_BtnBlock;
	CButton			m_BtnActive;
	CButton			m_BtnPropVisible;

	CComboBox		m_CBBoxSelect;

	CGamePropVisibleDlg* m_pGamePropVisibleDlg;

public:
	afx_msg void OnBnClickedBtnLoadWorldMap();
	afx_msg void OnBnClickedBtnSaberCamera();

	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);	

	afx_msg void OnCbnSelchangeComboSelect();
	afx_msg void OnBnClickedCheckProp();

	void		SetGamePropVisible(RwBool bVisible);
};
