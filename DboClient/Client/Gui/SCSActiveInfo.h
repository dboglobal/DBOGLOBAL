#pragma once

#include "GuiLineTree.h"

class SCSActiveInfo : public CNtlPLGui, public RWS::CEventHandler
{
protected:

public:
	SCSActiveInfo();
	virtual ~SCSActiveInfo();

	RwBool	Create( );
	void	Destroy( );

	// event
	void	HandleEvents( RWS::CMsg& msg );
	RwInt32	SwitchDialog(bool bOpen);
};