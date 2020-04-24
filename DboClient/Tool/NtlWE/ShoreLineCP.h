#pragma once


#define dHEIGHT_GAP (0.2f)


class CNtlPLObject;
class CNtlWorldBrush;


class CShoreLineCP
{
public:
	CShoreLineCP(RwV3d& Pos);
	virtual ~CShoreLineCP(void);

public:
	CNtlPLObject*	m_pCPUIObj;
	RwBool			m_NmlFixed;
	RwV3d			m_Nml;

public:
	VOID	SetPosition(RwV3d& Pos);
	RwV3d	GetPosition();
};
