#ifndef __SERVER_SRV_SCR_FORMULA_EXPRESS__
#define __SERVER_SRV_SCR_FORMULA_EXPRESS__


#include "ControlScriptGlobal.h"
#include <list>
#include <map>

class CSvrScrFormulaToken;

class CSvrScrFormulaExpress
{
	typedef sSSD_MESSAGE::eDEFINE CSvrScrFormulaExpress::eDEFINE;

	enum eSTATE_TYPE
	{
		STATE_START,
		STATE_VAR_NAME,
		STATE_EQUAL,
		STATE_EXPRESS,
		INVALID_STATE_TYPE = 0xFF,
	};

	enum eEXPRESS_STATE_TYPE
	{
		EXPRESS_STATE_START,
		EXPRESS_STATE_VALUE,
		EXPRESS_STATE_ARITHMETIC,
		EXPRESS_STATE_FUNCTION,
	};

	struct sFUNC_INFO
	{
		eSSD_FORMULA_FUNCTION_TYPE eFuncType;
		std::vector<eSSD_VARIABLE_TYPE> vecParameter;

		sFUNC_INFO::sFUNC_INFO(){}
		sFUNC_INFO::sFUNC_INFO(sFUNC_INFO* __that)
		{
			eFuncType = __that->eFuncType;
			vecParameter = __that->vecParameter;
		}
		sFUNC_INFO::~sFUNC_INFO(){}
	};

public:
	CSvrScrFormulaExpress();
	virtual ~CSvrScrFormulaExpress();


public:

	bool					Create();

	bool					Analyze(sSSD_FORMULA& rFormula, CSvrScrFormulaToken& rToken);

	bool					AnalyzeExpress(sSSD_FORMULA& rFormula, CSvrScrFormulaToken& rToken, std::list<std::string>::iterator& itRef);

	bool					AnalyzeFunction(sSSD_FORMULA_FUNCTION& rFunc, sFUNC_INFO& rFuncInfo, CSvrScrFormulaToken& rToken, std::list<std::string>::iterator& itRef);

	bool					GetEntity(sSSD_FORMULA_ENTITY& rEntity, std::string& rSrc);

	int						GetPriority(eSSD_FORMULA_ARITHMETIC_TYPE eType);

	const char*				GetMessageA();

protected:

	char					m_szMsg[eDEFINE::MAX_MESSAGE_LENGTH + 1];

	std::map<std::string, eSSD_FORMULA_ARITHMETIC_TYPE> m_mapStr2Arithmetic;
	std::map<std::string, sFUNC_INFO> m_mapStr2Func;
	std::map<eSSD_FORMULA_FUNCTION_TYPE, sFUNC_INFO> m_mapFuncInfo;
	std::map<eSSD_FORMULA_ARITHMETIC_TYPE, int> m_mapPriority;
};


#endif