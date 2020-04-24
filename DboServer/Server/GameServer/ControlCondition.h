#ifndef __SERVER_CONTROL_CONDITION__
#define __SERVER_CONTROL_CONDITION__


#include "ControlStateComposite.h"


class CControlCondition : public CControlStateComposite
{

public:

	CControlCondition(int controlStateID, const char* lpszControlStateName);
	virtual ~CControlCondition();




};


#endif