#include "stdafx.h"
#include "ControlCondition.h"



CControlCondition::CControlCondition(int controlStateID, const char* lpszControlStateName)
: CControlStateComposite(controlStateID, lpszControlStateName)
{
	if (lpszControlStateName)
		printf("CControlCondition %s \n", lpszControlStateName);
}

CControlCondition::~CControlCondition()
{

}