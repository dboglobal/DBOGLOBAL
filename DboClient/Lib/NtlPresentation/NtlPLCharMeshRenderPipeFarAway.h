#pragma once


#include "NtlPLCharMeshRenderPipe.h"
#include "NtlPLCharMeshRenderPipeFarAway.h"


class CNtlPLCharMeshRenderPipeFarAway : public CNtlPLCharMeshRenderPipe
{
public:
	CNtlPLCharMeshRenderPipeFarAway(void);
	virtual ~CNtlPLCharMeshRenderPipeFarAway(void);

public:
	static CNtlPLCharMeshRenderPipe* GetInstance();

public:
	void Execute(void* _pParam);
};
