#ifndef _WORLDPLAYCONTROLFACTORY_H_
#define _WORLDPLAYCONTROLFACTORY_H_

#include "ControlStateFactory.h"

class CControlState;
class CWpsAlgoObject;

class CWorldPlayControlFactory : public CControlStateFactory
{

public:
	CWorldPlayControlFactory(CWpsAlgoObject* pOwner);
	virtual ~CWorldPlayControlFactory();


public:

	virtual CControlState*	CreateScriptControlState(int controlStateID);

	CWpsAlgoObject*		GetOwner() { return m_pOwner; }

	
private:

	CWpsAlgoObject*		m_pOwner;

};


#endif