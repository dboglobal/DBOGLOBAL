#pragma once

#include "GuiLineTree.h"

class SCSGui : public CNtlPLGui, public RWS::CEventHandler
{
protected:

public:
	SCSGui();
	virtual ~SCSGui();

	RwBool	Create( );
	void	Destroy( );

	// event
	void	HandleEvents( RWS::CMsg& msg );
	RwInt32	SwitchDialog(bool bOpen);
};
