#include "stdafx.h"
#include "WPSNodeAction_DirectPlay.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"
#include "NtlDirection.h"
#include "DirectionLinkTable.h"
#include "TableContainerManager.h"


CWPSNodeAction_DirectPlay::CWPSNodeAction_DirectPlay(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_ADD_DIRECT_PLAY, lpszName)
{
	m_eDirectionType = DIRECTION_FLASH;
	m_bSynchronize = false;
	m_dwPlayTime = INVALID_DWORD;
	m_dwWaitTime = 0;
}

CWPSNodeAction_DirectPlay::~CWPSNodeAction_DirectPlay()
{

}

CWPSNodeAction_DirectPlay::sPARAMETER_INFO* CWPSNodeAction_DirectPlay::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_DirectPlay::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "id") == 0)
	{
		m_directTblidx = (TBLIDX)pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_ChangeCharAttribute: [%s] not exist \n", name);
	return false;
}

bool CWPSNodeAction_DirectPlay::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "sync") == 0)
	{
		if (_stricmp(pNode->GetValue(), "true") == 0)
		{
			m_bSynchronize = true;
			return CControlScriptNode::AddParam(pNode);
		}
		printf("value '%s' dont exist \n", pNode->GetValue());
	}

	if (_stricmp(name, "type") == 0)
	{
		if (_stricmp(pNode->GetValue(), "cinematic") == 0)
		{
			m_eDirectionType = DIRECTION_CINEMATIC;
			return CControlScriptNode::AddParam(pNode);
		}
		printf("value '%s' dont exist \n", pNode->GetValue());
	}

	printf("CWPSNodeAction_ChangeCharAttribute: [%s] not exist \n", name);
	return false;
}


bool CWPSNodeAction_DirectPlay::CheckParameter()
{
	sDIRECTION_LINK_TBLDAT* pTbldat = (sDIRECTION_LINK_TBLDAT*)g_pTableContainer->GetDirectionLinkTable()->FindData(m_directTblidx);
	if (pTbldat)
	{

	}

	return true;
}