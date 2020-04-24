#pragma once


#include "ntlworldpropbasetemplate.h"


class CNtlWorldPropNMapController :	public CNtlWorldPropBaseTemplate
{
public:
	CNtlWorldPropNMapController(void);
	virtual ~CNtlWorldPropNMapController(void);

public:
	VOID LoadPropImage(RwChar* pTexName, RwInt32 IdxProp);
	VOID AppendPropDat(RwV3d& PosTile, DWORD _PropDat);
	VOID DeletePropDat(RwV3d& PosTile, DWORD _PropDat);
	VOID SavePropImage(RwInt32 Width, RwInt32 Height, RwInt32 Depth, RwInt32 IdxProp, RwChar* pIdxName);
};
