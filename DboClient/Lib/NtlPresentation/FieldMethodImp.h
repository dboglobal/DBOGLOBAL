#pragma once


#include "ISectorMethodContainer.h"


class CFieldMethodImp : public ISectorMethodContainer
{
public:
	CFieldMethodImp(DWORD _MethodStyle);
	virtual ~CFieldMethodImp(void);

public:
	virtual RwBool Analyze();
	virtual RwBool Execute();
	virtual RwBool Report();
};
