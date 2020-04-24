#pragma once


#include "NtlWorldConceptController.h"

/**
 * \ingroup NtlSimulation
 * \brief ø¨√‚ world concept
 * \date 2007-07-18
 * \author jhssugi
 */

class CNtlWorldConceptDirect : public CNtlWorldConceptController
{
public:
	CNtlWorldConceptDirect(void);
	~CNtlWorldConceptDirect(void);

	virtual RwBool	IsEnableAction(EAvatarAction eAction);
	virtual void	ChangeState(RwInt32 iState);
};
