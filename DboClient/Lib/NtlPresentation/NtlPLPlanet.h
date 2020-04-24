#pragma once


#include "ntlplsun.h"


#define dNTLPL_PLANET_MAX_CNT	(3)
#define dNTLPL_PLANET_DEF_SCL	(100.0f)
#define dNTLPL_PLANET_INT_SEC	(3.0f)


struct sNTL_PLANET;


class CNtlPLPlanet : public CNtlPLSun
{
public:
	void* operator new(size_t size);
	void operator delete(void *pObj);

	CNtlPLPlanet(void);
	virtual ~CNtlPLPlanet(void);

public:
	VOID	AddPosition(RwV3d& _Pos);
	VOID	GetWorldPos(RwV3d& _MidPos, RwReal& _Radius);
	VOID	GetRelativePos(RwV3d& _CurPos);
	VOID	SetRelativePos(RwV3d& _NewWorldPos);
	RwBool	Pick();	
	VOID	Save(FILE* _pFile);
	VOID	Load(FILE* _pFile);
	RwBool	Compare(sNTL_PLANET& _NtlPlanet);
	RwChar*	GetTexName() { if(m_pTexture) return m_pTexture->name; else return NULL; }
	RwBool	WasFadeOutDone();
};
