#ifndef __SERVER_SRV_SCR_FORMULA_CALC__
#define __SERVER_SRV_SCR_FORMULA_CALC__


#include "ControlScriptGlobal.h"
#include "SvrScrVariableContainer.h"

class CControlState;
class CControlScriptNode;

class CSvrScrFormulaCalc : public CSvrScrVariableContainer
{
	typedef sSSD_MESSAGE::eDEFINE CSvrScrFormulaCalc::eDEFINE;

public:
	CSvrScrFormulaCalc(CControlState* pControlSate);
	CSvrScrFormulaCalc(CControlScriptNode* pScriptNode);
	virtual ~CSvrScrFormulaCalc();


public:

	void					Init();

	bool					Calculate(sSSD_FORMULA& rFormula);

	const char*				GetMessageA();

protected:

	bool					ConvertVariable(sSSD_VARIABLE& sVariable, sSSD_FORMULA_ENTITY& rFormula);


	char					m_szMsg[eDEFINE::MAX_MESSAGE_LENGTH + 1];

};


#endif