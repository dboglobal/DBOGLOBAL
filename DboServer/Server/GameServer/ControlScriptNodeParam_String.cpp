#include "stdafx.h"
#include "ControlScriptNodeParam_String.h"


CControlScriptNodeParam_String::CControlScriptNodeParam_String(const char* lpszName, const char* lpszValue)
: CControlScriptNodeParam(lpszName)
{
	m_strValue.operator=(lpszValue);
}

CControlScriptNodeParam_String::~CControlScriptNodeParam_String()
{

}