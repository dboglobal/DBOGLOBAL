#pragma once


#include "NtlPLCharMeshRenderPipe.h"


class CNtlPLCharMeshRenderPipe01 : public CNtlPLCharMeshRenderPipe
{
public:
	CNtlPLCharMeshRenderPipe01(void);
	virtual ~CNtlPLCharMeshRenderPipe01(void);

public:
	static CNtlPLCharMeshRenderPipe* GetInstance();

public:
	VOID	Execute(void* _pParam);
	VOID	RenderPetrify();
	VOID	RenderMajinWithVest();
	VOID	RenderCHAR();
	VOID	RenderCHARWithDogi();
	VOID	RenderCHARWithDogiEmblem();	
	VOID	RenderCHARPlayers();		// Render players with their armor upgraded or not
	VOID	RenderCHARNMVWithSEM();		// Render NPC, MOB, Vehicles with specular enviroment map rendered
};