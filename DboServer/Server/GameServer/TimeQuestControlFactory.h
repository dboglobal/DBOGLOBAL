#ifndef _TIMEQUESTCONTROLFACTORY_H_
#define _TIMEQUESTCONTROLFACTORY_H_

#include "ControlStateFactory.h"

class CControlState;
class CTqsAlgoObject;

class CTimeQuestControlFactory : public CControlStateFactory
{

public:
	CTimeQuestControlFactory(CTqsAlgoObject* pOwner);
	virtual ~CTimeQuestControlFactory();


public:

	virtual CControlState*	CreateScriptControlState(int controlStateID);

	CTqsAlgoObject*		GetOwner() { return m_pOwner; }


private:

	CTqsAlgoObject*		m_pOwner;

};


#endif