#pragma once


class CPathList;
class C2DAABB;
class CPathPoint;


class CPathHandler
{
public:
	CPathHandler(void);
	~CPathHandler(void);

	typedef std::vector<CPathList*>				tVECTOR_PL;
	typedef std::vector<CPathList*>::iterator	tVECTOR_PL_ITER;

private:

	// m_RenderOnly flag is available only for spawn palette mode
	RwBool m_RenderOnly;

	CPathList*		m_pCurPathList;
	CPathList*		m_pSelPathList;
	CPathPoint*		m_pCurPathPoint;
	CPathPoint*		m_pSnaPathPoint;
	tVECTOR_PL		m_vecPathList;
	CNtlPLEntity*	m_pDummyLineEntity;
	RwV3d			m_Intersection;
	RwBool			m_EndPPFlow;

private:
	RwBool		OnSnapVertex();
	RwBool		OnCreateNewPathList();
	RwBool		OnAppendNewPathPoint(RwV3d& _IntersectionPt, RwBool _EndPP);
	VOID		OnCreateNewPathPoint(RwV3d& _IntersectionPt);
	CPathPoint*	OnPickPathPoint();
	RwBool		VerifyPathListUID();

public:
	VOID	OnDestroy();
	VOID	OnClearCurVertex();
	VOID	OnDeletePathPoints(std::vector<RwInt32>& _vecIndices);
	VOID	OnDeletePathPoint();
	VOID	OnInsertPathPoint(RwBool bBack);

	VOID	OnDeletePathList();
	VOID	OnDeleteDummyEntities();
	VOID	OnCancelCurPathList();
	VOID	OnPortalTo(RwInt32 _Index);
	VOID	OnChangedRadius();
	VOID	OnChangedMemo();
	VOID	OnSave();
	VOID	OnLoad();
	VOID	OnExport();
	VOID	CullPathList();

	tVECTOR_PL_ITER	GetBEGIN() { return m_vecPathList.begin(); }
	tVECTOR_PL_ITER	GetEND() { return m_vecPathList.end(); }
	CPathList*		GetCurPathList() const { return m_pCurPathList; }
	VOID			SetRenderOnlyMode(RwBool _RenderOnly = FALSE);
	VOID			SetCurSelPathList(RwInt32 _SelIdx);

	VOID	Update();
	VOID	Render();
	VOID	RenderText(RtCharset* _pCharset, RwInt32 LenWindowWidth, RwInt32 LenWindowHeight);
};
