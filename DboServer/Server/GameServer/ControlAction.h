#ifndef __SERVER_CONTROL_ACTION__
#define __SERVER_CONTROL_ACTION__


#include "ControlStateComposite.h"


class CControlAction : public CControlStateComposite
{

public:

	CControlAction(int controlStateID, const char* lpszControlStateName);
	virtual ~CControlAction();




};


#endif