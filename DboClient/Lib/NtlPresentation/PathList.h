#pragma once


class CPathPoint;
class C2DAABB;
class CNtlWorldBrush;


class CPathList
{
public:
	CPathList(RwInt32 _UID, RwReal _fRadius, RwChar* _pMemo);
	virtual ~CPathList(void);

	typedef std::list<CPathPoint*>				tLIST_PP;
	typedef std::list<CPathPoint*>::iterator	tLIST_PP_ITER;

protected:
	tLIST_PP		m_lstPathPoint;
	RwInt32			m_UID;
	RwReal			m_fRadius;
	RwChar			m_Memo[255];
	C2DAABB*		m_pAABB;
	RwBool			m_Visibility;
	D3DCOLOR		m_LineColor;
	CNtlWorldBrush*	m_pSelectionBrush;

public:
	tLIST_PP_ITER	GetBEGIN() { return m_lstPathPoint.begin(); }
	tLIST_PP_ITER	GetEND() { return m_lstPathPoint.end(); }
	const RwInt32	GetUID() { return m_UID; }
	const RwReal	GetRadius() { return m_fRadius; }
	const RwChar*	GetMemo() { return m_Memo; }
	RwInt32			GetPathPointSize() { return m_lstPathPoint.size(); }
	C2DAABB*		GetAABB() { return m_pAABB; }
	VOID			RefreshAABB();
	RwBool			GetVisibility() { return m_Visibility; }
	VOID			SetLineColor(D3DCOLOR _LineColor) { m_LineColor = _LineColor; }
	VOID			SetRadius(RwReal fRadius) { m_fRadius = fRadius; }
	VOID			SetMemo(const RwChar* pMemo) { if (pMemo) { strcpy_s(m_Memo, 255, pMemo); } }

	VOID		DeleteDummyEntities();
	RwBool		CompareUID(RwInt32 _UID);
	VOID		Insert(CPathPoint* _pPathPoint, RwBool _PushBack = TRUE);
	VOID		InsertN(CPathPoint* _pPathPoint, CPathPoint* _pBasePathPoint, RwBool _PushBack);
	VOID		Delete(CPathPoint* _pPathPoint);
	VOID		Move(CPathPoint* _pPathPoint);
	VOID		Cull(C2DAABB* _pCurFieldAABB);
	CPathPoint*	Pick(RwInt32 ScreenPosX, RwInt32 ScreenPosY);
	VOID		Save(FILE* _pFile, RwBool OnlyForTool);
	VOID		Load(FILE* _pFile, BYTE _ByPointCount);
	VOID		Export(FILE* _pFile);

	VOID Update();
	VOID Render();
};

class CMergePathList : public CPathList
{
public:
	CMergePathList(RwInt32 _UID, RwReal _fRadius, RwChar* _pMemo);
	virtual ~CMergePathList(void);
};