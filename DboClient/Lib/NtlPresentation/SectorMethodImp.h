#pragma once


#include "ISectorMethodContainer.h"


class CSectorMethodImp : public ISectorMethodContainer
{
public:
	CSectorMethodImp(DWORD _MethodStyle);
	virtual ~CSectorMethodImp(void);

public:
	virtual RwBool Analyze();
	virtual RwBool Execute();
	virtual RwBool Report();
};
