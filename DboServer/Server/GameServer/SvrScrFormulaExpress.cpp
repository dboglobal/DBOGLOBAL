#include "stdafx.h"
#include "SvrScrFormulaExpress.h"
#include "SvrScrFormulaToken.h"


CSvrScrFormulaExpress::CSvrScrFormulaExpress()
{

}


CSvrScrFormulaExpress::~CSvrScrFormulaExpress()
{
	
}


bool CSvrScrFormulaExpress::Create()
{
	m_mapStr2Arithmetic.insert(std::make_pair("+", SSD_FORMULA_ARITHMETIC_PLUS));
	m_mapStr2Arithmetic.insert(std::make_pair("-", SSD_FORMULA_ARITHMETIC_MINUS));
	m_mapStr2Arithmetic.insert(std::make_pair("*", SSD_FORMULA_ARITHMETIC_MULTIPLE));
	m_mapStr2Arithmetic.insert(std::make_pair("/", SSD_FORMULA_ARITHMETIC_DIVIDE));
	m_mapStr2Arithmetic.insert(std::make_pair("(", SSD_FORMULA_ARITHMETIC_BRACKET_OPEN));
	m_mapStr2Arithmetic.insert(std::make_pair(")", SSD_FORMULA_ARITHMETIC_BRACKET_CLOSE));
	m_mapStr2Arithmetic.insert(std::make_pair("=", SSD_FORMULA_ARITHMETIC_EQUAL));
	m_mapStr2Arithmetic.insert(std::make_pair(",", SSD_FORMULA_ARITHMETIC_COMMA));

	m_mapPriority.insert(std::make_pair(SSD_FORMULA_ARITHMETIC_PLUS, 0));
	m_mapPriority.insert(std::make_pair(SSD_FORMULA_ARITHMETIC_MINUS, 0));
	m_mapPriority.insert(std::make_pair(SSD_FORMULA_ARITHMETIC_MULTIPLE, 1));
	m_mapPriority.insert(std::make_pair(SSD_FORMULA_ARITHMETIC_DIVIDE, 1));
	m_mapPriority.insert(std::make_pair(SSD_FORMULA_ARITHMETIC_BRACKET_OPEN, 2));
	m_mapPriority.insert(std::make_pair(SSD_FORMULA_ARITHMETIC_BRACKET_CLOSE, 2));
	m_mapPriority.insert(std::make_pair(SSD_FORMULA_ARITHMETIC_EQUAL, 3));
	m_mapPriority.insert(std::make_pair(SSD_FORMULA_ARITHMETIC_COMMA, 3));


	sFUNC_INFO funcVector;
	funcVector.eFuncType = SSD_FORMULA_FUNCTION_VECTOR;
	funcVector.vecParameter.push_back(SSD_VARIABLE_VECTOR);
	funcVector.vecParameter.push_back(SSD_VARIABLE_VECTOR);
	funcVector.vecParameter.push_back(SSD_VARIABLE_VECTOR);
	m_mapStr2Func.insert(std::make_pair("Vector", funcVector));
	m_mapFuncInfo.insert(std::make_pair(SSD_FORMULA_FUNCTION_VECTOR, funcVector));

	sFUNC_INFO funcRel;
	funcRel.eFuncType = SSD_FORMULA_FUNCTION_RELATIVECOORD;
	funcRel.vecParameter.push_back(SSD_VARIABLE_VECTOR);
	funcRel.vecParameter.push_back(SSD_VARIABLE_VECTOR);
	funcRel.vecParameter.push_back(SSD_VARIABLE_INTEGER);
	funcRel.vecParameter.push_back(SSD_VARIABLE_FLOAT);
	m_mapStr2Func.insert(std::make_pair("RelativeCoord", funcRel));
	m_mapFuncInfo.insert(std::make_pair(SSD_FORMULA_FUNCTION_RELATIVECOORD, funcRel));

	sFUNC_INFO funcRand;
	funcRand.eFuncType = SSD_FORMULA_FUNCTION_RANDOMRANGE;
	funcRand.vecParameter.push_back(SSD_VARIABLE_INTEGER);
	funcRand.vecParameter.push_back(SSD_VARIABLE_INTEGER);
	m_mapStr2Func.insert(std::make_pair("RandomRange", funcRand));
	m_mapFuncInfo.insert(std::make_pair(SSD_FORMULA_FUNCTION_RANDOMRANGE, funcRand));

	memset(m_szMsg, NULL, sizeof(m_szMsg));

	return true;
}

bool CSvrScrFormulaExpress::Analyze(sSSD_FORMULA& rFormula, CSvrScrFormulaToken& rToken)
{
	eSTATE_TYPE eState = STATE_START;

	for (CSvrScrFormulaToken::TOKEN_IT it = rToken.Begin(); it != rToken.End(); it++)
	{
		std::string& rSrc = *it;

		sSSD_FORMULA_ENTITY sEntity;

		if (!GetEntity(sEntity, rSrc))
			return false;

		switch (eState)
		{
			case STATE_START:
			{
				if (sEntity.eEntityType != SSD_FORMULA_ENTITY_VARNAME)
				{
					sprintf_s(m_szMsg, sizeof(m_szMsg), "'%s' is not variable name.", rSrc.c_str());
					return false;
				}

				rFormula.sResult = sEntity.sVarName.str();

				eState = STATE_VAR_NAME;
			}
			break;

			case STATE_VAR_NAME:
			{
				if (sEntity.eEntityType != SSD_FORMULA_ENTITY_ARITHMETIC || sEntity.eArithmetic != SSD_FORMULA_ARITHMETIC_EQUAL)
				{
					sprintf_s(m_szMsg, sizeof(m_szMsg), "before '%s' '=' must come.", rSrc.c_str());
					return false;
				}

				eState = STATE_EQUAL;
			}
			break;

			case STATE_EQUAL:
			{
				if (AnalyzeExpress(rFormula, rToken, it))
					return true;
				printf("AnalyzeExpress fail \n");
				return false;
			}
			break;

			default:
			{
				sprintf_s(m_szMsg, sizeof(m_szMsg), "invalid state type. state[%d]", eState);
				return false;
			}
		}
	}

	return true;
}


bool CSvrScrFormulaExpress::AnalyzeExpress(sSSD_FORMULA& rFormula, CSvrScrFormulaToken& rToken, std::list<std::string>::iterator& itRef)
{
	std::vector<sSSD_FORMULA_ENTITY> vecStack;
	bool bArithmeticState = false;

	while(true)
	{
		if (itRef == rToken.End())
		{
			while (!vecStack.empty())
			{
				rFormula.vecEntities.push_back(vecStack.back());
				vecStack.pop_back();
			}

			return true;
		}


		std::string& sMinus = *itRef;
		sSSD_FORMULA_ENTITY rEntity;

		if (!GetEntity(rEntity, sMinus))
		{
			return false;
		}

		if (bArithmeticState == true
			&& rEntity.eEntityType == SSD_FORMULA_ENTITY_ARITHMETIC
			&& (rEntity.eArithmetic == SSD_FORMULA_ARITHMETIC_PLUS || rEntity.eArithmetic == SSD_FORMULA_ARITHMETIC_MINUS))
		{
			eSSD_FORMULA_ARITHMETIC_TYPE eArithmeticType = rEntity.eArithmetic;

			itRef++;

			if (itRef == rToken.End())
			{
				sprintf_s(m_szMsg, sizeof(m_szMsg), "This formula did not end normally.");
				return false;
			}

			if (!GetEntity(rEntity, *itRef))
			{
				return false;
			}

			if (rEntity.eEntityType)
			{
				sprintf_s(m_szMsg, sizeof(m_szMsg), "It must be followed by a constant.");
				return false;
			}

			if (eArithmeticType == SSD_FORMULA_ARITHMETIC_MINUS)
			{
				sSSD_VARIABLE rVar(-1);
				rEntity.sConstant = rEntity.sConstant * rVar;
			}
		}

		bArithmeticState = rEntity.eEntityType == SSD_FORMULA_ENTITY_ARITHMETIC
			&& (rEntity.eArithmetic == SSD_FORMULA_ARITHMETIC_PLUS
			|| rEntity.eArithmetic == SSD_FORMULA_ARITHMETIC_MINUS
			|| rEntity.eArithmetic == SSD_FORMULA_ARITHMETIC_MULTIPLE
			|| rEntity.eArithmetic == SSD_FORMULA_ARITHMETIC_DIVIDE
			|| rEntity.eArithmetic == SSD_FORMULA_ARITHMETIC_BRACKET_OPEN);

		if (rEntity.eEntityType < SSD_FORMULA_ENTITY_CONSTANT)
		{
			return false;
		}

		if (rEntity.eEntityType <= SSD_FORMULA_ENTITY_VARNAME)
		{
			rFormula.vecEntities.push_back(rEntity);

			itRef++;
			continue;
		}

		if (rEntity.eEntityType == SSD_FORMULA_ENTITY_ARITHMETIC)
		{
			if (rEntity.eArithmetic < SSD_FORMULA_ARITHMETIC_PLUS || rEntity.eArithmetic > SSD_FORMULA_ARITHMETIC_BRACKET_OPEN)
			{
				if (rEntity.eArithmetic != SSD_FORMULA_ARITHMETIC_BRACKET_CLOSE)
				{
					sprintf_s(m_szMsg, sizeof(m_szMsg), "'%s' [", sMinus.c_str());

					for (CSvrScrFormulaToken::TOKEN_IT itOut = itRef; itOut != rToken.End(); itOut++)
					{
						sprintf_s(m_szMsg, sizeof(m_szMsg), "%s", m_szMsg);
						sprintf_s(m_szMsg, sizeof(m_szMsg), "%s", *itOut->c_str());
						//sprintf_s(m_szMsg, sizeof(m_szMsg), "%s %s", m_szMsg, *itOut->c_str());
					}

					sprintf_s(m_szMsg, sizeof(m_szMsg), "'%s']", m_szMsg);

					return false;
				}

				bool bFinedBracketOpen = false;
				while (!vecStack.empty())
				{
					if (vecStack.back().eEntityType == SSD_FORMULA_ENTITY_ARITHMETIC && vecStack.back().eArithmetic == SSD_FORMULA_ARITHMETIC_BRACKET_OPEN)
					{
						vecStack.pop_back();
						bFinedBracketOpen = true;
						break;
					}

					rFormula.vecEntities.push_back(vecStack.back());
					vecStack.pop_back();
				}

				if (!bFinedBracketOpen)
				{
					sprintf_s(m_szMsg, sizeof(m_szMsg), "can not find '('.");
					return false;
				}

				itRef++;
				continue;
			}

			if (vecStack.size() == 0)
			{
				vecStack.push_back(rEntity);

				itRef++;
				continue;
			}

			if (vecStack.back().eEntityType == SSD_FORMULA_ENTITY_ARITHMETIC)
			{
				if (vecStack.back().eArithmetic != SSD_FORMULA_ARITHMETIC_BRACKET_OPEN && GetPriority(rEntity.eArithmetic) <= GetPriority(vecStack.back().eArithmetic))
				{
					while (!vecStack.empty())
					{
						if (vecStack.back().eArithmetic == SSD_FORMULA_ARITHMETIC_BRACKET_OPEN)
							break;

						if (GetPriority(rEntity.eArithmetic) > GetPriority(vecStack.back().eArithmetic))
							break;

						rFormula.vecEntities.push_back(vecStack.back());
						vecStack.pop_back();
					}
				}
				else
				{
					vecStack.push_back(rEntity);
				}
			}
			else
			{
				sprintf_s(m_szMsg, sizeof(m_szMsg), "stack is not arithmetic type. type[%d]", vecStack.back().eArithmetic);
				return false;
			}
		}

		if (rEntity.eEntityType != SSD_FORMULA_ENTITY_FUNCTION)
		{
			return false;
		}

		std::map<eSSD_FORMULA_FUNCTION_TYPE, sFUNC_INFO>::iterator itFuncInfo = m_mapFuncInfo.find(rEntity.sFunction.eFuncType);
		if (itFuncInfo == m_mapFuncInfo.end())
		{
			sprintf_s(m_szMsg, sizeof(m_szMsg), "can not find function info. function[%s].", sMinus.c_str());
			return false;
		}

		++itRef;

		if (!AnalyzeFunction(rEntity.sFunction, itFuncInfo->second, rToken, itRef))
		{
			return false;
		}
		
		rFormula.vecEntities.push_back(rEntity);

		++itRef;
	}

	printf("omg rFormula.vecEntities size %I64u\n", rFormula.vecEntities.size());
	return false;
}


bool CSvrScrFormulaExpress::AnalyzeFunction(sSSD_FORMULA_FUNCTION& rFunc, sFUNC_INFO& rFuncInfo, CSvrScrFormulaToken& rToken, std::list<std::string>::iterator& itRef)
{
	enum eFUNC_STATE
	{
		FUNC_STATE_START = 0x0,
		FUNC_STATE_BRACKET_OPEN = 0x1,
		FUNC_STATE_PARAM = 0x2,
		FUNC_STATE_COMMA = 0x3,
		FUNC_STATE_BRACKET_CLOSE = 0x4,
	};

	eFUNC_STATE eFuncState = FUNC_STATE_START;
	int nCurParamCount = 0;
	eSSD_FORMULA_ARITHMETIC_TYPE eArithmeticType = INVALID_SSD_FORMULA_ARITHMETIC_TYPE;


	if (rToken.End() == itRef)
	{
		sprintf_s(m_szMsg, sizeof(m_szMsg), "invalid formula.");
		return false;
	}

	for (; itRef != rToken.End(); itRef++)
	{
		std::string& sMinus = *itRef;

		sSSD_FORMULA_ENTITY sFuncEntitiy;

		if (!GetEntity(sFuncEntitiy, sMinus))
		{
			return false;
		}

		switch (eFuncState)
		{
			case FUNC_STATE_START:
			{
				if (sFuncEntitiy.eEntityType != SSD_FORMULA_ENTITY_ARITHMETIC || sFuncEntitiy.eArithmetic != SSD_FORMULA_ARITHMETIC_BRACKET_OPEN)
				{
					sprintf_s(m_szMsg, sizeof(m_szMsg), "need '(', [%s].", sMinus.c_str());
					return false;
				}

				eFuncState = FUNC_STATE_BRACKET_OPEN;
				continue;
			}
			break;

			case FUNC_STATE_BRACKET_OPEN:
			{
				if (sFuncEntitiy.eEntityType == SSD_FORMULA_ENTITY_ARITHMETIC && sFuncEntitiy.eArithmetic == SSD_FORMULA_ARITHMETIC_MINUS)
				{
					eArithmeticType = SSD_FORMULA_ARITHMETIC_MINUS;
					continue;
				}

				if (sFuncEntitiy.eEntityType)
				{
					if (sFuncEntitiy.eEntityType != SSD_FORMULA_ENTITY_VARNAME)
					{
						sprintf_s(m_szMsg, sizeof(m_szMsg), "invalid type. [%s].", sMinus.c_str());
						return false;
					}

					if (eArithmeticType != INVALID_SSD_FORMULA_ARITHMETIC_TYPE)
					{
						sprintf_s(m_szMsg, sizeof(m_szMsg), "eArithmeticType != INVALID_SSD_FORMULA_ARITHMETIC_TYPE. [%s].", sMinus.c_str());
						return false;
					}
				}
				else if (eArithmeticType == SSD_FORMULA_ARITHMETIC_MINUS)
				{
					sSSD_VARIABLE rVar(-1);
					sFuncEntitiy.sConstant = sFuncEntitiy.sConstant * rVar;
					eArithmeticType = INVALID_SSD_FORMULA_ARITHMETIC_TYPE;
				}

				if (nCurParamCount >= rFuncInfo.vecParameter.size())
				{
					sprintf_s(m_szMsg, sizeof(m_szMsg), "Param Count is over. cur[%d] > max param[%I64u].", nCurParamCount, rFuncInfo.vecParameter.size());
					return false;
				}

				rFunc.vecParam.push_back(sFuncEntitiy);
				++nCurParamCount;
				eFuncState = FUNC_STATE_PARAM;
				continue;
			}
			break;

			case FUNC_STATE_PARAM:
			{
				if (nCurParamCount < rFuncInfo.vecParameter.size())
				{
					if (sFuncEntitiy.eEntityType != SSD_FORMULA_ENTITY_ARITHMETIC || sFuncEntitiy.eArithmetic != SSD_FORMULA_ARITHMETIC_COMMA)
					{
						sprintf_s(m_szMsg, sizeof(m_szMsg), "need ','. [%s]", sMinus.c_str());
						return false;
					}

					eFuncState = FUNC_STATE_BRACKET_OPEN;
					continue;
				}

				if (sFuncEntitiy.eEntityType == SSD_FORMULA_ENTITY_ARITHMETIC && sFuncEntitiy.eArithmetic == SSD_FORMULA_ARITHMETIC_BRACKET_CLOSE)
				{
					return true;
				}
				else
				{
					sprintf_s(m_szMsg, sizeof(m_szMsg), "need ')'. [%s]", sMinus.c_str());
					return false;
				}
			}
			break;

			default:
			{
				sprintf_s(m_szMsg, sizeof(m_szMsg), "invalid function state. func state[%d].", eFuncState);
				return false;
			}
			break;
		}
	}
	printf("axaxsad \n");
	return false;
}


bool CSvrScrFormulaExpress::GetEntity(sSSD_FORMULA_ENTITY& rEntity, std::string& rSrc)
{
	if (rSrc.size() > 32)
	{
		sprintf_s(m_szMsg, sizeof(m_szMsg), "string length is over 32");
		return false;
	}

	std::map<std::string, eSSD_FORMULA_ARITHMETIC_TYPE>::iterator itArith = m_mapStr2Arithmetic.find(rSrc);
	if (itArith != m_mapStr2Arithmetic.end())
	{
		rEntity.eEntityType = SSD_FORMULA_ENTITY_ARITHMETIC;
		rEntity.eArithmetic = itArith->second;
		return true;
	}

	if (*rSrc.c_str() < 48 || *rSrc.c_str() > 57)
	{
		std::map<std::string, sFUNC_INFO>::iterator it2 = m_mapStr2Func.find(rSrc);
		if (it2 != m_mapStr2Func.end())
		{
			rEntity.eEntityType = SSD_FORMULA_ENTITY_FUNCTION;
			rEntity.sFunction.eFuncType = it2->second.eFuncType;
		}
		else
		{
			rEntity.eEntityType = SSD_FORMULA_ENTITY_VARNAME;
			rEntity.sVarName = rSrc.c_str();
		}
	}
	else
	{
		int nPointCount = 0;
		for (int j = 1; ; j++)
		{
			if (j >= rSrc.size())
				break;
			if (rSrc.c_str()[j] == 46)
				++nPointCount;
			else
			{
				if (rSrc.c_str()[j] < 48 || rSrc.c_str()[j] > 57)
				{
					sprintf_s(m_szMsg, sizeof(m_szMsg), "'%s' is invalid format.", rSrc.c_str());
					return false;
				}
			}
		}

		if (nPointCount)
		{
			if (nPointCount != 1)
			{
				sprintf_s(m_szMsg, sizeof(m_szMsg), "'%s' invalid variable type.", rSrc.c_str());
				return false;
			}

			if (rSrc.c_str()[rSrc.size() - 1] == 46)
			{
				sprintf_s(m_szMsg, sizeof(m_szMsg), "'%s': invalid float type.", rSrc.c_str());
				return false;
			}

			rEntity.eEntityType = SSD_FORMULA_ENTITY_CONSTANT;
			rEntity.sConstant.operator=((float)std::atof(rSrc.c_str()));
		}
		else
		{
			rEntity.eEntityType = SSD_FORMULA_ENTITY_CONSTANT;
			rEntity.sConstant.operator=(std::atoi(rSrc.c_str()));
		}
	}

	return true;
}


int CSvrScrFormulaExpress::GetPriority(eSSD_FORMULA_ARITHMETIC_TYPE eType)
{
	std::map<eSSD_FORMULA_ARITHMETIC_TYPE, int>::iterator it = m_mapPriority.find(eType);
	if (it == m_mapPriority.end())
		return -1;

	return it->second;
}



const char* CSvrScrFormulaExpress::GetMessageA()
{
	return m_szMsg;
}
