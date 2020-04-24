#pragma once

#include "GuiLineTree.h"

class SCSActiveMSG : public CNtlPLGui, public RWS::CEventHandler
{
protected:

public:
	SCSActiveMSG();
	virtual ~SCSActiveMSG();

	RwBool	Create( );
	void	Destroy( );

	// event
	void	HandleEvents( RWS::CMsg& msg );
	RwInt32	SwitchDialog(bool bOpen);
};