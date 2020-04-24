#pragma once


#include "rwplcore.h"


#define dPOINT_NUM4CIRCLE (20)


class CNtlWorldBrush3DCircle
{
public:
	CNtlWorldBrush3DCircle();
	~CNtlWorldBrush3DCircle();

public:
	static VOID Render(RwSphere* _pSphere);
};