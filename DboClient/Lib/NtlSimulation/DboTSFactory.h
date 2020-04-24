#ifndef _DBO_TSFACTORY_H_
#define _DBO_TSFACTORY_H_


#include "DboTSCore.h"


/**
	Control Factory
*/


class CDboTSCCtrlFactory : public CDboTSCtrlFactory
{
// Methods
public:
	virtual	void						RegisterCtrlType( void );
};


/**
	UI Factory
*/


class CDboTSCUIFactory : public CDboTSUIFactory
{
// Methods
public:
	virtual	void						RegisterUIType( void );
};


#endif