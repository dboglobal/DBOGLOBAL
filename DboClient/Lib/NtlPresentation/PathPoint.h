#pragma once


#define dMAX_PATH_POINT (255)


class CNtlPLEntity;


class CPathPoint
{
public:
	CPathPoint(RwV3d& _Pos);
	virtual ~CPathPoint(void);

protected:
	RwV3d	m_Pos;
	RwReal	m_PrevDist;
	RwReal	m_Range;
	RwBool	m_Visibility;

	CNtlPLEntity* m_pDispEntity;

public:
	VOID			SetVisibility(RwBool _Visibility);
	VOID			SetPosition(RwV3d& _Pos);
	RwBool			GetVisibility() { return m_Visibility; }
	const RwV3d&	GetPosition() { return m_Pos; }
	VOID			SetColor(RwRGBA& _RGBA);

	VOID Save(FILE* _pFile, RwReal _PrevDist);
	VOID Load(FILE* _pFile);

	VOID Updata();
	VOID Render();
};
