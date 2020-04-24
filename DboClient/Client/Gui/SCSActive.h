#pragma once

#include "GuiLineTree.h"

class SCSActive : public CNtlPLGui, public RWS::CEventHandler
{
protected:

public:
	SCSActive();
	virtual ~SCSActive();

	RwBool	Create( );
	void	Destroy( );

	// event
	void	HandleEvents( RWS::CMsg& msg );
	RwInt32	SwitchDialog(bool bOpen);
};