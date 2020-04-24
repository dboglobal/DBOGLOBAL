#include "stdafx.h"
#include "DatabaseTaskRun.h"
#include "QueryServer.h"

static CNtlMutex* taskRunMutex;


CDatabaseTaskRun::CDatabaseTaskRun()
{
	if (taskRunMutex == NULL)
		taskRunMutex = new CNtlMutex;
}

CDatabaseTaskRun::~CDatabaseTaskRun()
{
}

int CDatabaseTaskRun::Create()
{
	return NTL_SUCCESS;
}

void CDatabaseTaskRun::Destroy()
{
}

void CDatabaseTaskRun::Run()
{
	while (IsRunnable())
	{
		taskRunMutex->Lock();

		GetCharDB.QueryTaskRun();
		GetAccDB.QueryTaskRun();

		taskRunMutex->Unlock();

		Wait(1);

	} // end of while( m_bRunning )
}

void CDatabaseTaskRun::Close()
{
	CNtlRunObject::Close();
}
