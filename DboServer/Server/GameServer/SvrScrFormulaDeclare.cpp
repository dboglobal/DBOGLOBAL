#include "stdafx.h"
#include "SvrScrFormulaDeclare.h"
#include "SvrScrFormulaToken.h"


CSvrScrFormulaDeclare::CSvrScrFormulaDeclare()
{

}

CSvrScrFormulaDeclare::~CSvrScrFormulaDeclare()
{

}



bool CSvrScrFormulaDeclare::Create()
{
	memset(m_szMsg, NULL, sizeof(m_szMsg));

	m_mapStr2Var.insert(std::make_pair("int", SSD_VARIABLE_INTEGER));
	m_mapStr2Var.insert(std::make_pair("float", SSD_VARIABLE_FLOAT));
	m_mapStr2Var.insert(std::make_pair("vector", SSD_VARIABLE_VECTOR));

	m_mapStr2Arithmetic.insert(std::make_pair("+", SSD_FORMULA_ARITHMETIC_PLUS));
	m_mapStr2Arithmetic.insert(std::make_pair("-", SSD_FORMULA_ARITHMETIC_MINUS));
	m_mapStr2Arithmetic.insert(std::make_pair("*", SSD_FORMULA_ARITHMETIC_MULTIPLE));
	m_mapStr2Arithmetic.insert(std::make_pair("/", SSD_FORMULA_ARITHMETIC_DIVIDE));
	m_mapStr2Arithmetic.insert(std::make_pair("(", SSD_FORMULA_ARITHMETIC_BRACKET_OPEN));
	m_mapStr2Arithmetic.insert(std::make_pair(")", SSD_FORMULA_ARITHMETIC_BRACKET_CLOSE));
	m_mapStr2Arithmetic.insert(std::make_pair("=", SSD_FORMULA_ARITHMETIC_EQUAL));
	m_mapStr2Arithmetic.insert(std::make_pair(",", SSD_FORMULA_ARITHMETIC_COMMA));

	return true;
}


bool CSvrScrFormulaDeclare::Analyze(std::vector<sSSD_FORMULA_DECLARE> & rDeclearList, CSvrScrFormulaToken & rToken)
{
	eDECLARE_STATE_TYPE eState = DECLARE_STATE_START;

	sSSD_FORMULA_DECLARE sOneDeclare;
	sOneDeclare.sVariable.Declare(SSD_VARIABLE_NIL);

	eSSD_FORMULA_ARITHMETIC_TYPE arithmeticType = INVALID_SSD_FORMULA_ARITHMETIC_TYPE;

	sPARAMETER sParam;
	sParam.Init(0);

	CSvrScrFormulaToken::TOKEN_IT itToken = rToken.Begin();

	while(true)
	{
		if (itToken != rToken.End())
		{
			CSvrScrFormulaDeclare::sDECLARE_ENTITY sEntity;
			std::string& sMinus = *itToken;

			if (!GetEntity(sEntity, sMinus))
				return false;

			switch (eState)
			{
				case DECLARE_STATE_START:
				{
					if (sEntity.eEntityType == DECLARE_ENTITY_DECLARE_TYPE)
					{
						sOneDeclare.sVariable.Declare(sEntity.eDeclareType);
						eState = DECLARE_STATE_DECLARE_TYPE;

						itToken++;
						continue;
					}

					sprintf_s(m_szMsg, sizeof(m_szMsg), "'%s' is not declare type.", sMinus.c_str());
					return false;
				}
				break;
				case DECLARE_STATE_DECLARE_TYPE:
				{
					if (sEntity.eEntityType == DECLARE_ENTITY_VAR_NAME)
					{
						sOneDeclare.sName.operator=(sEntity.sVarName.str());
						eState = DECLARE_STATE_VAR_NAME;

						itToken++;
						continue;
					}

					sprintf_s(m_szMsg, sizeof(m_szMsg), "'%s' is not variable name.", sMinus.c_str());
					return false;
				}
				break;
				case DECLARE_STATE_VAR_NAME:
				{
					if (sEntity.eEntityType == DECLARE_ENTITY_ARITHMETIC && sEntity.eArithmeticType == SSD_FORMULA_ARITHMETIC_EQUAL)
					{
						eState = DECLARE_STATE_EQUAL;

						itToken++;
						continue;
					}

					sprintf_s(m_szMsg, sizeof(m_szMsg), "before '%s' must come '=' ", sMinus.c_str());
					return false;
				}
				break;
				case DECLARE_STATE_EQUAL:
				{
					if (sEntity.eEntityType == DECLARE_ENTITY_ARITHMETIC)
					{
						if (sEntity.eArithmeticType == SSD_FORMULA_ARITHMETIC_PLUS || sEntity.eArithmeticType == SSD_FORMULA_ARITHMETIC_MINUS)
						{
							arithmeticType = sEntity.eArithmeticType;
							eState = DECLARE_STATE_PLUS_MINUS;

							itToken++;
							continue;
						}

						sprintf_s(m_szMsg, sizeof(m_szMsg), "'%s' is a unknown type", sMinus.c_str());
						return false;
					}
					else if (sEntity.eEntityType == DECLARE_ENTITY_INT_FLOAT)
					{
						if (sOneDeclare.sVariable.eType || sEntity.sVariable.eType != SSD_VARIABLE_FLOAT)
						{
							sOneDeclare.sVariable.operator=(sEntity.sVariable);
							eState = DECLARE_STATE_END_ONEUNIT;

							itToken++;
							continue;
						}

						sprintf_s(m_szMsg, sizeof(m_szMsg), "The declaration can not put a real-type '%f' in the integer variable '%s'.", sEntity.sVariable.fFloat, sOneDeclare.sName.str());
						return false;
					}
					else
					{
						if (sEntity.eEntityType == DECLARE_ENTITY_VECTOR)
						{
							eState = DECLARE_STATE_VECTOR;
							itToken++;
							continue;
						}
						sprintf_s(m_szMsg, sizeof(m_szMsg), "'%s' is a type unknown", sMinus.c_str());
						return false;
					}
				}
				break;
				case DECLARE_STATE_PLUS_MINUS:
				{
					if (sEntity.eEntityType == DECLARE_ENTITY_INT_FLOAT)
					{
						if (sOneDeclare.sVariable.eType || sEntity.sVariable.eType != SSD_VARIABLE_FLOAT)
						{
							if (arithmeticType == SSD_FORMULA_ARITHMETIC_MINUS)
							{
								sSSD_VARIABLE rVar(-1);
								sOneDeclare.sVariable.operator=(sEntity.sVariable.operator*(rVar));
							}

							arithmeticType = INVALID_SSD_FORMULA_ARITHMETIC_TYPE;
							eState = DECLARE_STATE_END_ONEUNIT;

							itToken++;
							continue;
						}

						sprintf_s(m_szMsg, sizeof(m_szMsg), "The declaration can not put a real-type '%i' in the integer variable '%s'.", sEntity.sVariable.nInteger, sOneDeclare.sName.str());
						return false;
					}
					else
					{
						sprintf_s(m_szMsg, sizeof(m_szMsg), "'%s' is a type unknown.", sMinus.c_str());
						return false;
					}
				}
				break;
				case DECLARE_STATE_END_ONEUNIT:
				{
					if (sEntity.eEntityType == DECLARE_ENTITY_ARITHMETIC && sEntity.eArithmeticType == SSD_FORMULA_ARITHMETIC_COMMA)
					{
						rDeclearList.push_back(sOneDeclare);

						sOneDeclare.sName.operator=("\0");
						sOneDeclare.sVariable.Declare(SSD_VARIABLE_NIL);
						eState = DECLARE_STATE_START;

						itToken++;
						continue;
					}

					sprintf_s(m_szMsg, sizeof(m_szMsg), "before '%s' must come a ','", sMinus.c_str());
					return false;
				}
				break;
				case DECLARE_STATE_VECTOR:
				{
					if (sEntity.eEntityType == DECLARE_ENTITY_ARITHMETIC && sEntity.eArithmeticType == SSD_FORMULA_ARITHMETIC_BRACKET_OPEN)
					{
						sParam.Init(3);
						eState = DECLARE_STATE_VECTOR_PARAM;

						itToken++;
						continue;
					}
					sprintf_s(m_szMsg, sizeof(m_szMsg), "'%s' is not '('", sMinus.c_str());
					return false;
				}
				break;
				case DECLARE_STATE_VECTOR_PARAM:
				{
					if (sEntity.eEntityType == DECLARE_ENTITY_ARITHMETIC)
					{
						if (sEntity.eArithmeticType == SSD_FORMULA_ARITHMETIC_PLUS || sEntity.eArithmeticType == SSD_FORMULA_ARITHMETIC_MINUS)
						{
							arithmeticType = sEntity.eArithmeticType;
							eState = DECLARE_STATE_VECTOR_PLUS_MINUS;

							itToken++;
							continue;
						}
						sprintf_s(m_szMsg, sizeof(m_szMsg), "'%s' is a type unknown", sMinus.c_str());
						return false;
					}
					else if (sEntity.eEntityType == DECLARE_ENTITY_INT_FLOAT)
					{
						if (sParam.vecParameter.size() < sParam.nMaxCount)
						{
							sParam.vecParameter.push_back(sEntity.sVariable);
							eState = DECLARE_STATE_VECTOR_END;

							itToken++;
							continue;
						}
						sprintf_s(m_szMsg, sizeof(m_szMsg), "Too many parameters in the vector");
						return false;
					}
					else
					{
						sprintf_s(m_szMsg, sizeof(m_szMsg), "int or float type must come before '%s'", sMinus.c_str());
						return false;
					}
				}
				break;
				case DECLARE_STATE_VECTOR_PLUS_MINUS:
				{
					if (sEntity.eEntityType == DECLARE_ENTITY_INT_FLOAT)
					{
						if (sParam.vecParameter.size() < sParam.nMaxCount)
						{
							if (arithmeticType == SSD_FORMULA_ARITHMETIC_MINUS)
							{
								sSSD_VARIABLE rVar(-1);
								sEntity.sVariable.operator=(sEntity.sVariable.operator*(rVar));
							}

							sParam.vecParameter.push_back(sEntity.sVariable);
							arithmeticType = INVALID_SSD_FORMULA_ARITHMETIC_TYPE;
							eState = DECLARE_STATE_VECTOR_END;

							itToken++;
							continue;
						}
						sprintf_s(m_szMsg, sizeof(m_szMsg), "Too many parameters in the vector");
						return false;
					}
					else
					{
						sprintf_s(m_szMsg, sizeof(m_szMsg), "'%s' is a type unknown", sMinus.c_str());
						return false;
					}
				}
				break;
				case DECLARE_STATE_VECTOR_END:
				{
					if (sEntity.eEntityType == DECLARE_ENTITY_ARITHMETIC)
					{
						if (sEntity.eArithmeticType == SSD_FORMULA_ARITHMETIC_BRACKET_CLOSE)
						{
							if (sParam.vecParameter.size() >= sParam.nMaxCount)
							{
								sSSD_VARIABLE& rZ = sParam.vecParameter[2];
								sSSD_VARIABLE& rY = sParam.vecParameter[1];
								sSSD_VARIABLE& rX = sParam.vecParameter[0];

								eSSD_VARIABLE_RESULT eVarResult = sOneDeclare.sVariable.Vector(rX, rY, rZ);
								if (eVarResult == SSD_VARIABLE_RESULT_SUCCESS)
								{
									sParam.Init(0);
									eState = DECLARE_STATE_END_ONEUNIT;

									itToken++;
									continue;
								}

								sprintf_s(m_szMsg, sizeof(m_szMsg), "'%s': vector set failed (%s)", sMinus.c_str(), GetSSD_VARIABLE_RESULT_STRING(eVarResult));
								return false;
							}
							else
							{
								sprintf_s(m_szMsg, sizeof(m_szMsg), "'%s': the number of parameters is less than %d", sMinus.c_str(), sParam.nMaxCount);
								return false;
							}
						}
						else
						{
							if (sEntity.eArithmeticType != SSD_FORMULA_ARITHMETIC_COMMA)
							{
								itToken++;
								continue;
							}

							if (sParam.vecParameter.size() < sParam.nMaxCount)
							{
								eState = DECLARE_STATE_VECTOR_PARAM;
								itToken++;
								continue;
							}

							sprintf_s(m_szMsg, sizeof(m_szMsg), "'%s': Before it already exceeds the %d parameter", sMinus.c_str(), sParam.nMaxCount);
							return false;
						}
					}
					else
					{
						sprintf_s(m_szMsg, sizeof(m_szMsg), "'%s' before ',' or ')' it must be followed by", sMinus.c_str());
						return false;
					}
				}
				break;
				case DECLARE_STATE_END:
				{
					itToken++;
					continue;
				}
				break;

				default:
				{
					sprintf_s(m_szMsg, sizeof(m_szMsg), "invalid state type. state[%d]", eState);
					return false;
				}
				break;
			}
		}
		else
		{
			if (sOneDeclare.sName.str())
				rDeclearList.push_back(sOneDeclare);

			if (eState == DECLARE_STATE_END_ONEUNIT)
			{
				//for (std::vector<sSSD_FORMULA_DECLARE>::iterator it = rDeclearList.begin(); it != rDeclearList.end(); it++)
				//{
				//	sSSD_FORMULA_DECLARE& token = *it;
				//	printf("<ANALYZE>declare [%s] Value %u\n", token.sName.str(), token.sVariable.nInteger);
				//}

				return true;
			}
			else
			{
				sprintf_s(m_szMsg, sizeof(m_szMsg), "This formula has not been properly terminated. The last condition [%d]", eState);
				return false;
			}
		}
	}

	return false;
}


bool CSvrScrFormulaDeclare::GetEntity(sDECLARE_ENTITY& rEntity, std::string& rSrc)
{
	if (rSrc.size() > 32)
	{
		sprintf_s(m_szMsg, sizeof(m_szMsg), "The length of name exceeds the size 32");
		return false;
	}

	std::map<std::string, eSSD_FORMULA_ARITHMETIC_TYPE>::iterator itArith = m_mapStr2Arithmetic.find(rSrc);
	if (itArith != m_mapStr2Arithmetic.end())
	{
		rEntity.eEntityType = DECLARE_ENTITY_ARITHMETIC;
		rEntity.eArithmeticType = itArith->second;
		return true;
	}

	if (*rSrc.c_str() < 48 || *rSrc.c_str() > 57)
	{
		if (rSrc == "Vector")
		{
			rEntity.eEntityType = DECLARE_ENTITY_VECTOR;
		}
		else
		{
			std::map<std::string, eSSD_VARIABLE_TYPE>::iterator it2 = m_mapStr2Var.find(rSrc);
			if (it2 != m_mapStr2Var.end())
			{
				rEntity.eEntityType = DECLARE_ENTITY_DECLARE_TYPE;
				rEntity.eDeclareType = it2->second;
			}
			else
			{
				rEntity.eEntityType = DECLARE_ENTITY_VAR_NAME;
				rEntity.sVarName = rSrc.c_str();
			}
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
					sprintf_s(m_szMsg, sizeof(m_szMsg), "'%s' It is the wrong format..", rSrc.c_str());
					return false;
				}
			}
		}

		if (nPointCount)
		{
			if (nPointCount != 1)
			{
				sprintf_s(m_szMsg, sizeof(m_szMsg), "'%s' It is the wrong format..", rSrc.c_str());
				return false;
			}

			if (rSrc.c_str()[rSrc.size() - 1] == 46)
			{
				sprintf_s(m_szMsg, sizeof(m_szMsg), "'%s': does not confirm the type float.", rSrc.c_str());
				return false;
			}

			rEntity.eEntityType = DECLARE_ENTITY_INT_FLOAT;
			rEntity.sVariable = (float)std::atof(rSrc.c_str());
		}
		else
		{
			rEntity.eEntityType = DECLARE_ENTITY_INT_FLOAT;
			rEntity.sVariable = std::atoi(rSrc.c_str());
		}
	}

	return true;
}


const char* CSvrScrFormulaDeclare::GetMessageA()
{
	return m_szMsg;
}
