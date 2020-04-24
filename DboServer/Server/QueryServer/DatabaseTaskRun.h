#pragma once


#include "NtlThread.h"


class CDatabaseTaskRun : public CNtlRunObject
{
public:

	CDatabaseTaskRun();

	~CDatabaseTaskRun();


public:

	int							Create();

	void						Destroy();

public:

	virtual void				Run();

	virtual void				Close();

};
//-----------------------------------------------------------------------------------