#include "stdafx.h"
#include "DynamicFieldSystem.h"
#include "QueryServer.h"


CDynamicFieldSystem::CDynamicFieldSystem()
{
	Init();
}

CDynamicFieldSystem::~CDynamicFieldSystem()
{
	Destroy();
}



void CDynamicFieldSystem::Init()
{
	smart_ptr<QueryResult> item = GetLogDB.Query("SELECT count FROM dynamic_field_count WHERE serverIndex=0");
	if (item)
	{
		Field* i = item->Fetch();
		if (i)
		{
			m_dwCurCount = i->GetDWORD();
		}
	}
}

void CDynamicFieldSystem::Destroy()
{
}



