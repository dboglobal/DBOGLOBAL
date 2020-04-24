#pragma once


#include "ntlworldpropbasetemplate.h"


class CNtlWorldPropSMapController :	public CNtlWorldPropBaseTemplate
{
public:
	CNtlWorldPropSMapController(void);
	virtual ~CNtlWorldPropSMapController(void);

protected:
	VOID LoadPropImage(RwChar* pTexName, RwInt32 IdxProp);
	VOID AppendPropDat(RwV3d& PosTile, DWORD _PropDat);
	VOID DeletePropDat(RwV3d& PosTile, DWORD _PropDat);
	VOID SavePropImage(RwInt32 Width, RwInt32 Height, RwInt32 Depth, RwInt32 IdxProp, RwChar* pIdxName);
};

// 하위 4바이트로 표현할수 있는 모든 수치값들을 스크립트 인덱스로 표현하자고 얘기했다. 이때 테이블은 따로 작성