#pragma once


class CShoreLineCP;
class CNtlWorldBrush;


class CShoreLinePGenerator
{
public:
	CShoreLinePGenerator(RwV3d& StartPos);
	CShoreLinePGenerator(RwV3d& _SSnapPos, RwV3d& _ESnapPos);
	virtual ~CShoreLinePGenerator(void);

private:
	CShoreLineCP*				m_pSLCPNext;
	std::list<CShoreLineCP*>	m_listShoreLineCP;
	RwIm3DVertex				m_VB[6];

public:
	std::list<CShoreLineCP*>::iterator	SLCP_BEGIN() { return m_listShoreLineCP.begin(); }		
	std::list<CShoreLineCP*>::iterator	SLCP_END() { return m_listShoreLineCP.end(); }

public:
	CNtlWorldBrush*	m_pNtlWorldBrushLine;
	RwBool			m_IsLeft;

private:
	VOID RenderLine(RwReal _Size);
	VOID RenderGeom(RwReal _Size, RwRGBA _RGBA);

public:	
	VOID	SetHeight(RwReal _Value, RwBool _AddFlag = TRUE);
	RwBool	GetSnapVert(RwV3d& _CurPos, RwV3d& SSnapVert, RwV3d& ESSnapVert);
	VOID	CreateShoreLineCP(RwV3d& Pos);
	VOID	CreateShoreLineCP(RwV3d& _SSnap, RwV3d& _ESnap);
	VOID	Update();
	VOID	Render();
};
