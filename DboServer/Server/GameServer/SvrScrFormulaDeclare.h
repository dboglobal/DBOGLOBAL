#ifndef __SERVER_SRV_SCR_FORMULA_DECLARE__
#define __SERVER_SRV_SCR_FORMULA_DECLARE__


#include "ControlScriptGlobal.h"
#include <map>


class CSvrScrFormulaToken;


class CSvrScrFormulaDeclare
{
public:

	typedef sSSD_MESSAGE::eDEFINE CSvrScrFormulaDeclare::eDEFINE;

	enum eDECLARE_STATE_TYPE
	{
		DECLARE_STATE_START,
		DECLARE_STATE_DECLARE_TYPE,
		DECLARE_STATE_VAR_NAME,
		DECLARE_STATE_EQUAL,
		DECLARE_STATE_PLUS_MINUS,
		DECLARE_STATE_END_ONEUNIT,
		DECLARE_STATE_VECTOR,
		DECLARE_STATE_VECTOR_PARAM,
		DECLARE_STATE_VECTOR_PLUS_MINUS,
		DECLARE_STATE_VECTOR_END,
		DECLARE_STATE_END,
		INVALID_DECLARE_STATE_TYPE = 0xFF,
	};

	enum eDECLARE_ENTITY_TYPE
	{
		DECLARE_ENTITY_DECLARE_TYPE,
		DECLARE_ENTITY_VAR_NAME,
		DECLARE_ENTITY_ARITHMETIC,
		DECLARE_ENTITY_INT_FLOAT,
		DECLARE_ENTITY_VECTOR,
		INVALID_DECLARE_ENTITY_TYPE = 0xFF,
	};

	struct sDECLARE_ENTITY
	{
		eDECLARE_ENTITY_TYPE eEntityType;
		eSSD_VARIABLE_TYPE eDeclareType;
		sSSD_VARNAME sVarName;
		eSSD_FORMULA_ARITHMETIC_TYPE eArithmeticType;
		sSSD_VARIABLE sVariable;
	};

	struct sPARAMETER
	{
		void Init(int nCount)
		{
			nMaxCount = nCount;
			if (vecParameter.size())
				vecParameter.clear();
		}

		int nMaxCount;

		std::vector<sSSD_VARIABLE> vecParameter;
	};

public:
	CSvrScrFormulaDeclare();
	virtual ~CSvrScrFormulaDeclare();


public:

	bool					Create();

	bool					Analyze(std::vector<sSSD_FORMULA_DECLARE> & rDeclearList, CSvrScrFormulaToken & rToken);

	bool					GetEntity(sDECLARE_ENTITY& rEntity, std::string& rSrc);

	const char*				GetMessageA();

protected:

	char					m_szMsg[eDEFINE::MAX_MESSAGE_LENGTH + 1];

	std::map<std::string, eSSD_VARIABLE_TYPE> m_mapStr2Var;
	std::map<std::string, eSSD_FORMULA_ARITHMETIC_TYPE> m_mapStr2Arithmetic;

};


#endif