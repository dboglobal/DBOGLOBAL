#include "stdafx.h"
#include "PlayScriptManager.h"
#include "TableContainerManager.h"

CPlayScriptManager::CPlayScriptManager()
{
}

CPlayScriptManager::~CPlayScriptManager()
{
}


CTableContainer* CPlayScriptManager::GetTableContainer()
{
	return g_pTableContainer;
}
