#include "stdafx.h"
#include "ControlScriptNodeParam_Number.h"


CControlScriptNodeParam_Number::CControlScriptNodeParam_Number(const char* lpszName, long double number)
: CControlScriptNodeParam(lpszName)
{
	m_value = number;
}

CControlScriptNodeParam_Number::~CControlScriptNodeParam_Number()
{

}
