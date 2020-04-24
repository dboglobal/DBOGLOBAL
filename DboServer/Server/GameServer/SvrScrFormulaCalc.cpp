#include "stdafx.h"
#include "SvrScrFormulaCalc.h"
#include "NtlRandom.h"
#include "NtlMovement.h"


CSvrScrFormulaCalc::CSvrScrFormulaCalc(CControlState* pControlSate)
: CSvrScrVariableContainer(pControlSate)
{
	Init();
}
CSvrScrFormulaCalc::CSvrScrFormulaCalc(CControlScriptNode* pScriptNode)
: CSvrScrVariableContainer(pScriptNode)
{
	Init();
}

CSvrScrFormulaCalc::~CSvrScrFormulaCalc()
{

}


void CSvrScrFormulaCalc::Init()
{
	memset(m_szMsg, 0, sizeof(m_szMsg));
}

bool CSvrScrFormulaCalc::Calculate(sSSD_FORMULA& rFormula)
{
	std::vector<sSSD_FORMULA_ENTITY> vecStack;
	sSSD_VARIABLE sVariable;

	std::vector<sSSD_FORMULA_ENTITY>::iterator it = rFormula.vecEntities.begin();
//	printf("rFormula.vecEntities xsdsfd size %u \n", rFormula.vecEntities.size());
	while ( true )
	{
		if (it == rFormula.vecEntities.end())
		{
			if (vecStack.size() != 1 || vecStack[0].eEntityType)
			{
				sprintf_s(m_szMsg, sizeof(m_szMsg), "idk what error. vecStack.size() %I64u, vecStack[0].eEntityType %u", vecStack.size(), vecStack[0].eEntityType);
				return false;
			}
			else
			{
				eSSD_VARIABLE_RESULT eResult = UpdateVariable(rFormula.sResult.str(), vecStack[0].sConstant);
				if (eResult == SSD_VARIABLE_RESULT_SUCCESS)
				{
					return true;
				}
				else
				{
					sprintf_s(m_szMsg, sizeof(m_szMsg), "'%s' eResult: %u %s", rFormula.sResult.str(), eResult, GetSSD_VARIABLE_RESULT_STRING(eResult));
					return false;
				}
			}
		}

		sSSD_FORMULA_ENTITY& sEntity = *it;

	//	printf("adsadsadad sVarName '%s' sEntity.eEntityType %u \n", sEntity.sVarName.str(), sEntity.eEntityType);

		switch (sEntity.eEntityType)
		{
			case SSD_FORMULA_ENTITY_CONSTANT:
			{
				vecStack.push_back(sEntity);
			}
			break;

			case SSD_FORMULA_ENTITY_VARNAME:
			{
				sSSD_VARIABLE* pVar = GetVariable(sEntity.sVarName.str());
				if (!pVar)
				{
					sprintf_s(m_szMsg, sizeof(m_szMsg), "GetVariable is null. VarName '%s'", sEntity.sVarName.str());
					return false;
				}

				sSSD_FORMULA_ENTITY sVarEntity;
				sVarEntity.eEntityType = SSD_FORMULA_ENTITY_CONSTANT;
				sVarEntity.sConstant.Declare(pVar->eType);
				sVarEntity.sConstant = *pVar;

				vecStack.push_back(sVarEntity);
			}
			break;

			case SSD_FORMULA_ENTITY_ARITHMETIC:
			{
				if (vecStack.size() < 2)
				{
					sprintf_s(m_szMsg, sizeof(m_szMsg), "invalid formula");
					return false;
				}
				switch (sEntity.eArithmetic)
				{
					case SSD_FORMULA_ARITHMETIC_PLUS: sVariable.Declare(vecStack[vecStack.size() - 2].sConstant + vecStack[vecStack.size() - 1].sConstant); break;

					case SSD_FORMULA_ARITHMETIC_MINUS: sVariable.Declare(vecStack[vecStack.size() - 2].sConstant - vecStack[vecStack.size() - 1].sConstant); break;

					case SSD_FORMULA_ARITHMETIC_MULTIPLE: sVariable.Declare(vecStack[vecStack.size() - 2].sConstant * vecStack[vecStack.size() - 1].sConstant); break;

					case SSD_FORMULA_ARITHMETIC_DIVIDE: sVariable.Declare(vecStack[vecStack.size() - 2].sConstant / vecStack[vecStack.size() - 1].sConstant); break;

					default:
					{
						sprintf_s(m_szMsg, sizeof(m_szMsg), "invalid formula(2)");
						return false;
					}
					break;
				}

				if (sVariable.eResult)
				{
					sprintf_s(m_szMsg, sizeof(m_szMsg), "yy %s", GetSSD_VARIABLE_RESULT_STRING(sVariable.eResult));
					return false;
				}

				vecStack.pop_back();
				vecStack.pop_back();

				sSSD_FORMULA_ENTITY sVarEntity;
				sVarEntity.eEntityType = SSD_FORMULA_ENTITY_CONSTANT;
				sVarEntity.sConstant.Declare(sVariable);
	
				vecStack.push_back(sVarEntity);
			}
			break;

			case SSD_FORMULA_ENTITY_FUNCTION:
			{
				switch (sEntity.sFunction.eFuncType)
				{
					case SSD_FORMULA_FUNCTION_VECTOR:
					{
						if (sEntity.sFunction.vecParam.size() != 3)
						{
							sprintf_s(m_szMsg, sizeof(m_szMsg), "vector parameter is invalid. param count[%I64u]", sEntity.sFunction.vecParam.size());
							return false;
						}

						sSSD_VARIABLE rX;
						sSSD_VARIABLE rY;
						sSSD_VARIABLE rZ;

						if (!ConvertVariable(rX, sEntity.sFunction.vecParam[0]))
						{
							return false;
						}
						if (!ConvertVariable(rY, sEntity.sFunction.vecParam[1]))
						{
							return false;
						}
						if (!ConvertVariable(rZ, sEntity.sFunction.vecParam[2]))
						{
							return false;
						}

						sSSD_FORMULA_ENTITY sVarEntity;
						sVarEntity.eEntityType = SSD_FORMULA_ENTITY_CONSTANT;
						sVarEntity.sConstant.Declare(SSD_VARIABLE_VECTOR);

						eSSD_VARIABLE_RESULT eResult = sVarEntity.sConstant.Vector(rX, rY, rZ);
						if (eResult)
						{
							sprintf_s(m_szMsg, sizeof(m_szMsg), "vector make is fail.(%s)", GetSSD_VARIABLE_RESULT_STRING(eResult));
							return false;
						}

						vecStack.push_back(sVarEntity);
					}
					break;

					case SSD_FORMULA_FUNCTION_RANDOMRANGE:
					{
						if (sEntity.sFunction.vecParam.size() != 2)
						{
							sprintf_s(m_szMsg, sizeof(m_szMsg), "vector parameter is invalid. param count[%I64u]", sEntity.sFunction.vecParam.size());
							return false;
						}

						sSSD_VARIABLE sRelaiveCoord[2];

						for (int j = 0; j < 2; j++)
						{
							if (!ConvertVariable(sRelaiveCoord[j], sEntity.sFunction.vecParam[j]))
							{
								return false;
							}
						}

						sSSD_FORMULA_ENTITY sVarEntity;
						sVarEntity.eEntityType = SSD_FORMULA_ENTITY_CONSTANT;
						sVarEntity.sConstant.Declare(SSD_VARIABLE_INTEGER);

						for (int j = 0; j < 2; j++)
						{
							if (&sRelaiveCoord[j] == NULL)
							{
								char buff[129];
								GetString_SSD_FORMULA_ENTITY(sEntity.sFunction.vecParam[j], 129, buff);

								sprintf_s(m_szMsg, sizeof(m_szMsg), "o %s", buff);
								return false;
							}
						}

						sVarEntity.sConstant = RandomRange(sRelaiveCoord[0].nInteger, sRelaiveCoord[1].nInteger);
						vecStack.push_back(sVarEntity);
					//	printf("lelele push back random range %i (%i %i)\n", sVarEntity.sConstant.nInteger, sRelaiveCoord[0].nInteger, sRelaiveCoord[1].nInteger);
					}
					break;

					case SSD_FORMULA_FUNCTION_RELATIVECOORD:
					{
						if (sEntity.sFunction.vecParam.size() != 4)
						{
							sprintf_s(m_szMsg, sizeof(m_szMsg), "vector parameter is invalid. param count[%I64u]", sEntity.sFunction.vecParam.size());
							return false;
						}

						sSSD_VARIABLE sRelaiveCoord[4];

						for (int j = 0; j < 4; j++)
						{
							if (!ConvertVariable(sRelaiveCoord[j], sEntity.sFunction.vecParam[j]))
							{
								return false;
							}
						}

						if (sRelaiveCoord[0].eType != SSD_VARIABLE_VECTOR)
						{
							sprintf_s(m_szMsg, sizeof(m_szMsg), "sRelaiveCoord[0].eType %u != SSD_VARIABLE_VECTOR", sRelaiveCoord[0].eType);
							return false;
						}
						if (sRelaiveCoord[1].eType != SSD_VARIABLE_VECTOR)
						{
							sprintf_s(m_szMsg, sizeof(m_szMsg), "sRelaiveCoord[1].eType %u != SSD_VARIABLE_VECTOR", sRelaiveCoord[1].eType);
							return false;
						}
						if (sRelaiveCoord[2].eType != SSD_VARIABLE_INTEGER )
						{
							sprintf_s(m_szMsg, sizeof(m_szMsg), "sRelaiveCoord[2].eType %u != SSD_VARIABLE_INTEGER", sRelaiveCoord[2].eType);
							return false;
						}
						if (sRelaiveCoord[3].eType != SSD_VARIABLE_INTEGER && sRelaiveCoord[3].eType != SSD_VARIABLE_FLOAT)
						{
							sprintf_s(m_szMsg, sizeof(m_szMsg), "sRelaiveCoord[3].eType %u != SSD_VARIABLE_INTEGER || SSD_VARIABLE_FLOAT", sRelaiveCoord[3].eType);
							return false;
						}


						sSSD_FORMULA_ENTITY sVarEntity;
						sVarEntity.eEntityType = SSD_FORMULA_ENTITY_CONSTANT;
						sVarEntity.sConstant.Declare(SSD_VARIABLE_VECTOR);

						// 0 = vector(loc), 1 = vector(dir), 2 = int(angle), 3 = float
						CNtlVector vLoc(sRelaiveCoord[0].sVector), vDir(sRelaiveCoord[1].sVector), vNewDir;

						if (!vDir.SafeNormalize())
						{
							vDir.x = 1.0f;
							vDir.z = 0;
						}

						sSSD_VARIABLE sVarInt(0);
						sVarInt = sRelaiveCoord[2].nInteger;
						sSSD_VARIABLE sVarFloat(0.0f);
						sVarFloat = sRelaiveCoord[3].fFloat + (float)sRelaiveCoord[3].nInteger;

						RotateVector(vNewDir, &vDir, (float)sVarInt.nInteger);
						vLoc += vNewDir * sVarFloat.fFloat;

					//	printf("vLoc %f %f %f, vDir %f %f %f, vNewDir %f %f %f \n", vLoc.x, vLoc.y, vLoc.z, vDir.x, vDir.y, vDir.z, vNewDir.x, vNewDir.y, vNewDir.z);

						sVarEntity.sConstant = vLoc;
						vecStack.push_back(sVarEntity);
					}
					break;

					default:
					{
						sprintf_s(m_szMsg, sizeof(m_szMsg), "not registered function. function type[%d]", sEntity.sFunction.eFuncType);
						return false;
					}
					break;
				}
			}
			break;

			default:
			{
				sprintf_s(m_szMsg, sizeof(m_szMsg), "invalid entitytype. %u", sEntity.eEntityType);
				return false;
			}
			break;
		}

		++it;
	}

	return false;
}

const char* CSvrScrFormulaCalc::GetMessageA()
{
	return m_szMsg;
}


bool CSvrScrFormulaCalc::ConvertVariable(sSSD_VARIABLE& sVariable, sSSD_FORMULA_ENTITY& rFormula)
{
	if (rFormula.eEntityType)
	{
		if (rFormula.eEntityType != SSD_FORMULA_ENTITY_VARNAME)
		{
			sprintf_s(m_szMsg, sizeof(m_szMsg), "invalid parameter type. type [%d]", rFormula.eEntityType);
			return false;
		}

		sSSD_VARIABLE* rVar = GetVariable(rFormula.sVarName.str());
		if (!rVar)
		{
			sprintf_s(m_szMsg, sizeof(m_szMsg), "Could not find %s", rFormula.sVarName.str());
			return false;
		}

		sVariable.Declare(*rVar);
	}
	else
	{
		sVariable.Declare(rFormula.sConstant);
	}

	return true;
}



