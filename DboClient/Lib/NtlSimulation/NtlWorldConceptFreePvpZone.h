#pragma once

#include "NtlWorldConceptController.h"

class CNtlWorldConceptFreePvpZone : public CNtlWorldConceptController
{

public:

	CNtlWorldConceptFreePvpZone();
	~CNtlWorldConceptFreePvpZone();

	virtual void	Update(RwReal fElapsedTime);
	virtual void	ChangeState(RwInt32 iState);

public:

	CNtlSobActor*	GetNearAttackableEnemy(CNtlSobActor* pActor);

	void			UpdateNames();
};