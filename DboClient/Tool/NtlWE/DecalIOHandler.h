#pragma once


enum eDECAL_EDIT_MODE
{
	eDEM_IDLE_MODE = 0,
	eDEM_EDIT_MODE,
};


class CNtlPLEntity;
class CNtlWorldBrush;


#include "NtlPLWorldDecal.h"


class CDecalIOHandler
{
public:
	CDecalIOHandler(void);
	~CDecalIOHandler(void);

public:
	VOID Update();
	VOID Render();

private:
	// drag info.
	CNtlPLEntity*	m_pNtlPLEntity;
	CNtlWorldBrush*	m_pRectInDetailBrush;
	CNtlWorldBrush*	m_pRectInDetailBrushSelecter;

	eDECAL_EDIT_MODE m_Mode;

private:
	VOID On_DEM_IDLE();
	VOID On_DEM_EDIT();

public:
	VOID	Delete();
	VOID	ApplyDecalPropParam(sDECAL_PROP_PARAM& _DecalPropParam);
	VOID	RenderText(RtCharset* _pCharset, RwInt32 LenWindowWidth, RwInt32 LenWindowHeight);
};
