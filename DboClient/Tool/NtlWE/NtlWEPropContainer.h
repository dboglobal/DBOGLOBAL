#pragma once
#include "ntlplpropertycontainer.h"

class CNtlWEPropContainer :	public CNtlPLPropertyContainer
{
public:
	CNtlWEPropContainer(void);
public:
	virtual ~CNtlWEPropContainer(void);

public:
	virtual RwBool Load(const RwChar *pFileName);
};
