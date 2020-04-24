#pragma once

#include "NtlWorldConceptController.h"

/**
 * \ingroup NtlSimulation
 * \brief NPC 와 대화할 때 이용
 * \date 2007-07-18
 * \author avooo
 */

class CNtlWorldConceptNPCCOMMU : public CNtlWorldConceptController
{
public:
	CNtlWorldConceptNPCCOMMU(void);
	~CNtlWorldConceptNPCCOMMU(void);

	virtual RwBool	IsEnableAction(EAvatarAction eAction);
	virtual void	ChangeState(RwInt32 iState);
};
