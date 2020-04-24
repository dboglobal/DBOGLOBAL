#pragma once

#include "ResourceLayer.h"
#include "SurfaceLayer.h"

class CLayerMisc
{
protected:
	CLayerMisc(void);
public:
	~CLayerMisc(void);

	enum LAYER { RESOURCE_LAYER = 0, SURFACE_LAYER, COMPONENT_LAYER, LAYER_MAXCOUNT };
	
	CView*	GetLayer( LAYER nIndex );
	void	SetLayer( LAYER nIndex );
	
	static CLayerMisc& GetInstance(); 

protected:

	LAYER	m_nCurrentLayer;
};
