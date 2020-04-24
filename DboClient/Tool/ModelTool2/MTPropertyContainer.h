#ifndef _MT_PROPERTY_CONTAINER_H_
#define _MT_PROPERTY_CONTAINER_H_

#include "NtlPLPropertyContainer.h"

class CMTPropertyContainer : public CNtlPLPropertyContainer
{
public:
	CMTPropertyContainer();
	virtual ~CMTPropertyContainer();

public:
	virtual RwBool Load();
};

#endif