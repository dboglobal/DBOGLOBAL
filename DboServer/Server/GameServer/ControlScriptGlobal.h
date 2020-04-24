#ifndef _CONTROLSCRIPTGLOBALDEFINE_H_
#define _CONTROLSCRIPTGLOBALDEFINE_H_

#include "NtlVector.h"
#include <vector>

#include "AISDefine.h"
#include "WPSDefine.h"
#include "SPSDefine.h"
#include "TQSDefine.h"



enum eSSD_VARIABLE_TYPE
{
	SSD_VARIABLE_INTEGER,
	SSD_VARIABLE_FLOAT,
	SSD_VARIABLE_VECTOR,
	SSD_VARIABLE_NIL = 0xFF,
};

enum eSSD_VARIABLE_RESULT
{
	SSD_VARIABLE_RESULT_SUCCESS,
	SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH,
	SSD_VARIABLE_RESULT_CAN_NOT_DIVIDE_ZERO,
	SSD_VARIABLE_RESULT_DECLARATION_IS_NOT_VALID,
	SSD_VARIABLE_RESULT_VARIABLE_IS_NOT_VECTOR,
	SSD_VARIABLE_RESULT_VARIABLE_IS_ALREADY_EXIST,
	SSD_VARIABLE_RESULT_VARIABLE_NOT_EXIST,
	SSD_VARIABLE_RESULT_TYPE_COUNT,
};

inline const char* GetSSD_VARIABLE_RESULT_STRING(eSSD_VARIABLE_RESULT eResult)
{
	switch (eResult)
	{
		case SSD_VARIABLE_RESULT_SUCCESS: return "success"; break;
		case SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH: return "The resulttype mismatch"; break;
		case SSD_VARIABLE_RESULT_CAN_NOT_DIVIDE_ZERO: return "You can not divide by zero"; break;
		case SSD_VARIABLE_RESULT_DECLARATION_IS_NOT_VALID: return "The declaration is not valid"; break;
		case SSD_VARIABLE_RESULT_VARIABLE_IS_NOT_VECTOR: return "The variable is not vector"; break;
		case SSD_VARIABLE_RESULT_VARIABLE_IS_ALREADY_EXIST: return "The variable already exist"; break;
		case SSD_VARIABLE_RESULT_VARIABLE_NOT_EXIST: return "Variable does not exist"; break;
		default: return "NULL"; break;
	}

	return "NULL";
}


enum eSSD_COMPARE_RESULT
{
	SSD_COMPARE_TRUE,
	SSD_COMPARE_FALSE,
	SSD_COMPARE_CANNOT_COMPARE,
};

enum eSSD_VARIABLE_COMPARE_TYPE
{
	SSD_VARIABLE_COMPARE_OPERATOR_BIG,
	SSD_VARIABLE_COMPARE_OPERATOR_EQUAL,
	SSD_VARIABLE_COMPARE_OPERATOR_SMALL,
	SSD_VARIABLE_COMPARE_OPERATOR_BIG_EQUAL,
	SSD_VARIABLE_COMPARE_OPERATOR_SMALL_EQUAL,
	SSD_VARIABLE_COMPARE_OPERATOR_NOT_EQUAL,

	MAX_SSD_VARIABLE_COMPARE_OPERATOR_TYPE,
	INVALID_SSD_VARIABLE_COMPARE_OPERATOR_TYPE = 0xFF,
};

enum eSSD_FORMULA_FUNCTION_TYPE
{
	SSD_FORMULA_FUNCTION_VECTOR,
	SSD_FORMULA_FUNCTION_RANDOMRANGE,
	SSD_FORMULA_FUNCTION_RELATIVECOORD,

	MAX_SSD_FORMULA_FUNCTION,
	INVALID_SSD_FORMULA_FUNCTION = 0xFF,
};

enum eSSD_FORMULA_ENTITY_TYPE
{
	SSD_FORMULA_ENTITY_CONSTANT,
	SSD_FORMULA_ENTITY_VARNAME,
	SSD_FORMULA_ENTITY_ARITHMETIC,
	SSD_FORMULA_ENTITY_FUNCTION,

	MAX_SSD_FORMULA_ENTITY_TYPE,
	INVALID_SSD_FORMULA_ENTITY_TYPE = 0xFF,
};

enum eSSD_FORMULA_ARITHMETIC_TYPE
{
	SSD_FORMULA_ARITHMETIC_PLUS,
	SSD_FORMULA_ARITHMETIC_MINUS,
	SSD_FORMULA_ARITHMETIC_MULTIPLE,
	SSD_FORMULA_ARITHMETIC_DIVIDE,
	SSD_FORMULA_ARITHMETIC_BRACKET_OPEN,
	SSD_FORMULA_ARITHMETIC_BRACKET_CLOSE,
	SSD_FORMULA_ARITHMETIC_EQUAL,
	SSD_FORMULA_ARITHMETIC_COMMA,

	MAX_SSD_FORMULA_ARITHMETIC_TYPE,
	INVALID_SSD_FORMULA_ARITHMETIC_TYPE = 0xFF,
};

enum eSSD_SCRIPT_TYPE
{
	SSD_SCRIPT_TYPE_TS,
	SSD_SCRIPT_TYPE_SPS,
	SSD_SCRIPT_TYPE_TQS,
	SSD_SCRIPT_TYPE_WPS,
	SSD_SCRIPT_TYPE_AIS,
	SSD_SCRIPT_TYPE_NPCSYSTEM,
	INVALID_SSD_SCRIPT_TYPE = 0xFF,
};

enum eSSD_EVENT_TYPE
{
	SSD_EVENT_MOB_FIGHT_START = 0x0,
	SSD_EVENT_MOB_FIGHT_END = 0x0,
	SSD_EVENT_NPC_FIGHT_START = 0x0,
	SSD_EVENT_NPC_FIGHT_END = 0x0,
	INVALID_SSD_EVENT_TYPE = 0xFF,
};

enum eSSD_MOB_PAIR_TYPE
{
	SSD_MOB_PAIR_TBLIDX,
	SSD_MOB_PAIR_LIST_INDEX,
};

enum eSSD_TARGET_TYPE
{
	SSD_TARGET_SELF,
	SSD_TARGET_NPC,
	SSD_TARGET_MOB,
	SSD_TARGET_TRIGGER_OBJECT,

	INVALID_SSD_TARGET_TYPE = 0xFF,
};

enum eSSD_TSEVENT_APPLY_TYPE
{
	SSD_TSEVENT_APPLY_TYPE_REGISTER_ALL,
	SSD_TSEVENT_APPLY_TYPE_REGISTER_POINT_AREA,
	SSD_TSEVENT_APPLY_TYPE_REGISTER_OBJECT_AREA,
	SSD_TSEVENT_APPLY_TYPE_ALL_POINT_AREA,
	SSD_TSEVENT_APPLY_TYPE_ALL_OBJECT_AREA,

	INVALID_SSD_TSEVENT_APPLY_TYPE = 0xFF,
};


struct sSSD_VARIABLE
{
	sSSD_VARIABLE(sVECTOR3& sVec)
	{
		eType = SSD_VARIABLE_VECTOR;
		eResult = SSD_VARIABLE_RESULT_SUCCESS;
		sVector.x = sVec.x;
		sVector.y = sVec.y;
		sVector.z = sVec.z;
		nInteger = 0;
		fFloat = 0.f;
	}
	sSSD_VARIABLE(float x, float y, float z)
	{
		eType = SSD_VARIABLE_VECTOR;
		eResult = SSD_VARIABLE_RESULT_SUCCESS;
		sVector.x = x;
		sVector.y = y;
		sVector.z = z;
		fFloat = 0.f;
		nInteger = 0;
	}
	sSSD_VARIABLE(float fValue)
	{
		eType = SSD_VARIABLE_FLOAT;
		eResult = SSD_VARIABLE_RESULT_SUCCESS;
		fFloat = fValue;
		nInteger = 0;
		sVector.x = 0.f;
		sVector.y = 0.f;
		sVector.z = 0.f;
	}
	sSSD_VARIABLE(int nValue)
	{
		eType = SSD_VARIABLE_INTEGER;
		eResult = SSD_VARIABLE_RESULT_SUCCESS;
		nInteger = nValue;
		fFloat = 0.0f;
		sVector.x = 0.f;
		sVector.y = 0.f;
		sVector.z = 0.f;
	}
	sSSD_VARIABLE(void)
	{
		eType = SSD_VARIABLE_NIL;
		eResult = SSD_VARIABLE_RESULT_DECLARATION_IS_NOT_VALID;
		nInteger = 0;
		fFloat = 0.0f;
		sVector.x = 0.f;
		sVector.y = 0.f;
		sVector.z = 0.f;
	}

	sSSD_VARIABLE& operator = (sSSD_VARIABLE& variable)
	{
		switch (eType)
		{
			case SSD_VARIABLE_INTEGER:
			{
				if (variable.eType == SSD_VARIABLE_INTEGER)
				{
					nInteger = variable.nInteger;
				}
				else if (variable.eType == SSD_VARIABLE_FLOAT)
				{
					nInteger = (int)floor(variable.fFloat);
				}
				else 
					eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH;
			}
			break;
			case SSD_VARIABLE_FLOAT:
			{
				if (variable.eType == SSD_VARIABLE_INTEGER)
				{
					fFloat = (float)variable.nInteger;
				}
				else if (variable.eType == SSD_VARIABLE_FLOAT)
				{
					fFloat = variable.fFloat;
				}
				else
					eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH;
			}
			break;
			case SSD_VARIABLE_VECTOR:
			{
				if (variable.eType == SSD_VARIABLE_VECTOR)
					sVector = variable.sVector;
				else
					eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH;
			}
			break;
			case SSD_VARIABLE_NIL:
			{
				eType = variable.eType;

				if (eType == SSD_VARIABLE_INTEGER)
					nInteger = variable.nInteger;
				else if (eType == SSD_VARIABLE_FLOAT)
					fFloat = variable.fFloat;
				else if (eType == SSD_VARIABLE_VECTOR)
					sVector = variable.sVector;
				else
					eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH;
			}
				break;

			default: 
				eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH; 
			break;
		}

		return *this;
	}

	bool operator= (CNtlVector& rValue)
	{
		eType = SSD_VARIABLE_VECTOR;
		eResult = SSD_VARIABLE_RESULT_SUCCESS;
		sVector.x = rValue.x;
		sVector.y = rValue.y;
		sVector.z = rValue.z;
		return true;
	}
	bool operator= (sVECTOR3& sVec)
	{
		eType = SSD_VARIABLE_VECTOR;
		eResult = SSD_VARIABLE_RESULT_SUCCESS;
		sVector.x = sVec.x;
		sVector.y = sVec.y;
		sVector.z = sVec.z;
		return true;
	}
	bool operator= (float fValue)
	{
		eType = SSD_VARIABLE_FLOAT;
		eResult = SSD_VARIABLE_RESULT_SUCCESS;
		fFloat = fValue;
		return true;
	}
	bool operator= (int nValue)
	{
		eType = SSD_VARIABLE_INTEGER;
		eResult = SSD_VARIABLE_RESULT_SUCCESS;
		nInteger = nValue;
		return true;
	}

	void Declare(sSSD_VARIABLE& sVariable)
	{
		memcpy(this, &sVariable, sizeof(sSSD_VARIABLE));
	}

	void Declare(eSSD_VARIABLE_TYPE type)
	{
		eType = type;
		eResult = SSD_VARIABLE_RESULT_SUCCESS;

		if (eType == SSD_VARIABLE_INTEGER)
			nInteger = 0;
		else if (eType == SSD_VARIABLE_FLOAT)
			fFloat = 0.0f;
		else if (eType == SSD_VARIABLE_VECTOR)
		{
			sVector.x = 0.0f;
			sVector.y = 0.0f;
			sVector.z = 0.0f;
		}
		else
		{
			eType = SSD_VARIABLE_NIL;
			eResult = SSD_VARIABLE_RESULT_DECLARATION_IS_NOT_VALID;
		}
	}

	eSSD_VARIABLE_RESULT Vector(sSSD_VARIABLE& rX, sSSD_VARIABLE& rY, sSSD_VARIABLE& rZ)
	{
		if (eType != SSD_VARIABLE_VECTOR)
			return SSD_VARIABLE_RESULT_VARIABLE_IS_NOT_VECTOR;

		if (rX.eType > SSD_VARIABLE_FLOAT)
			return SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH;
		if (rY.eType > SSD_VARIABLE_FLOAT)
			return SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH;
		if (rZ.eType > SSD_VARIABLE_FLOAT)
			return SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH;

		sVector.x = (rX.eType == SSD_VARIABLE_INTEGER) ? (float)rX.nInteger : rX.fFloat;
		sVector.y = (rY.eType == SSD_VARIABLE_INTEGER) ? (float)rY.nInteger : rY.fFloat;
		sVector.z = (rZ.eType == SSD_VARIABLE_INTEGER) ? (float)rZ.nInteger : rZ.fFloat;

		return SSD_VARIABLE_RESULT_SUCCESS;
	}

	static eSSD_COMPARE_RESULT Compare(sSSD_VARIABLE* left, eSSD_VARIABLE_COMPARE_TYPE eCompareType, sSSD_VARIABLE* right)
	{
		float fLeft = 0.0f;
		bool bResult = false;
		float fRight = 0.0f;

		//left
		if (left->eType == SSD_VARIABLE_INTEGER)
			fLeft = (float)left->nInteger;
		else if (left->eType == SSD_VARIABLE_FLOAT)
		{
			fLeft = left->fFloat;
		}
		else
		{
			return SSD_COMPARE_CANNOT_COMPARE;
		}
		//right
		if (right->eType == SSD_VARIABLE_INTEGER)
			fRight = (float)right->nInteger;
		else if (right->eType == SSD_VARIABLE_FLOAT)
		{
			fRight = right->fFloat;
		}
		else
		{
			return SSD_COMPARE_CANNOT_COMPARE;
		}

		//compare left and right fFloat
		switch (eCompareType)
		{
			case SSD_VARIABLE_COMPARE_OPERATOR_BIG:
			{
				bResult = fLeft > fRight;
				return (bResult == true) ? SSD_COMPARE_TRUE : SSD_COMPARE_FALSE;
			}
			break;
			case SSD_VARIABLE_COMPARE_OPERATOR_EQUAL:
			{
				bResult = fLeft == fRight;
				return (bResult == true) ? SSD_COMPARE_TRUE : SSD_COMPARE_FALSE;
			}
			break;
			case SSD_VARIABLE_COMPARE_OPERATOR_SMALL:
			{
				bResult = fLeft < fRight;
				return (bResult == true) ? SSD_COMPARE_TRUE : SSD_COMPARE_FALSE;
			}
			break;
			case SSD_VARIABLE_COMPARE_OPERATOR_BIG_EQUAL:
			{
				bResult = fLeft >= fRight;
				return (bResult == true) ? SSD_COMPARE_TRUE : SSD_COMPARE_FALSE;
			}
			break;
			case SSD_VARIABLE_COMPARE_OPERATOR_SMALL_EQUAL:
			{
				bResult = fLeft <= fRight;
				return (bResult == true) ? SSD_COMPARE_TRUE : SSD_COMPARE_FALSE;
			}
			break;
			case SSD_VARIABLE_COMPARE_OPERATOR_NOT_EQUAL:
			{
				bResult = fLeft != fRight;
				return (bResult == true) ? SSD_COMPARE_TRUE : SSD_COMPARE_FALSE;
			}
			break;

			default: break;
		}

		return SSD_COMPARE_CANNOT_COMPARE;
	}

	sSSD_VARIABLE operator+ (sSSD_VARIABLE& rVar)
	{
		sSSD_VARIABLE sTemp;
		sTemp.eType = SSD_VARIABLE_NIL;
		sTemp.eResult = SSD_VARIABLE_RESULT_SUCCESS;

		switch (eType)
		{
			case SSD_VARIABLE_INTEGER:
			{
				switch (rVar.eType)
				{
					case SSD_VARIABLE_INTEGER:
					{
						sTemp.eType = SSD_VARIABLE_INTEGER;
						sTemp.nInteger = nInteger + rVar.nInteger;
					}
					break;
					case SSD_VARIABLE_FLOAT:
					{
						sTemp.eType = SSD_VARIABLE_FLOAT;
						sTemp.fFloat = (float)nInteger + rVar.fFloat;
					}
					break;
					case SSD_VARIABLE_VECTOR:
					{
						sTemp.eType = SSD_VARIABLE_NIL;
						sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH;
					}
					break;
				}
			}
			break;

			case SSD_VARIABLE_FLOAT:
			{
				switch (rVar.eType)
				{
					case SSD_VARIABLE_INTEGER:
					{
						sTemp.eType = SSD_VARIABLE_FLOAT;
						sTemp.fFloat = fFloat + (float)rVar.nInteger;
					}
					break;
					case SSD_VARIABLE_FLOAT:
					{
						sTemp.eType = SSD_VARIABLE_FLOAT;
						sTemp.fFloat = fFloat + rVar.fFloat;
					}
					break;
					case SSD_VARIABLE_VECTOR:
					{
						sTemp.eType = SSD_VARIABLE_NIL;
						sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH;
					}
					break;
				}
			}
			break;

			case SSD_VARIABLE_VECTOR:
			{
				switch (rVar.eType)
				{
					case SSD_VARIABLE_VECTOR:
					{
						sTemp.eType = SSD_VARIABLE_VECTOR;
						sTemp.sVector.x = sVector.x + rVar.sVector.x;
						sTemp.sVector.y = sVector.y + rVar.sVector.y;
						sTemp.sVector.z = sVector.z + rVar.sVector.z;
					}
					break;

					default: sTemp.eType = SSD_VARIABLE_NIL; sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH; break;
				}
			}
			break;

			default: sTemp.eType = SSD_VARIABLE_NIL; sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH; break;
		}

		return sTemp;
	}

	sSSD_VARIABLE operator- (sSSD_VARIABLE& rVar)
	{
		sSSD_VARIABLE sTemp;
		sTemp.eType = SSD_VARIABLE_NIL;
		sTemp.eResult = SSD_VARIABLE_RESULT_SUCCESS;

		switch (eType)
		{
			case SSD_VARIABLE_INTEGER:
			{
				switch (rVar.eType)
				{
					case SSD_VARIABLE_INTEGER:
					{
						sTemp.eType = SSD_VARIABLE_INTEGER;
						sTemp.nInteger = nInteger - rVar.nInteger;
					}
					break;
					case SSD_VARIABLE_FLOAT:
					{
						sTemp.eType = SSD_VARIABLE_FLOAT;
						sTemp.fFloat = (float)nInteger - rVar.fFloat;
					}
					break;
					case SSD_VARIABLE_VECTOR:
					{
						sTemp.eType = SSD_VARIABLE_NIL;
						sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH;
					}
					break;

					default: sTemp.eType = SSD_VARIABLE_NIL; sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH; break;
				}
			}
			break;

			case SSD_VARIABLE_FLOAT:
			{
				switch (rVar.eType)
				{
					case SSD_VARIABLE_INTEGER:
					{
						sTemp.eType = SSD_VARIABLE_FLOAT;
						sTemp.fFloat = fFloat - (float)rVar.nInteger;
					}
					break;
					case SSD_VARIABLE_FLOAT:
					{
						sTemp.eType = SSD_VARIABLE_FLOAT;
						sTemp.fFloat = fFloat - rVar.fFloat;
					}
					break;
					case SSD_VARIABLE_VECTOR:
					{
						sTemp.eType = SSD_VARIABLE_NIL;
						sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH;
					}
					break;

					default: sTemp.eType = SSD_VARIABLE_NIL; sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH; break;
				}
			}
			break;

			case SSD_VARIABLE_VECTOR:
			{
				switch (rVar.eType)
				{
					case SSD_VARIABLE_VECTOR:
					{
						sTemp.eType = SSD_VARIABLE_VECTOR;
						sTemp.sVector.x = sVector.x - rVar.sVector.x;
						sTemp.sVector.y = sVector.y - rVar.sVector.y;
						sTemp.sVector.z = sVector.z - rVar.sVector.z;
					}
					break;

					default: sTemp.eType = SSD_VARIABLE_NIL; sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH; break;
				}
			}
			break;

			default: sTemp.eType = SSD_VARIABLE_NIL; sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH; break;
		}

		return sTemp;
	}

	sSSD_VARIABLE operator* (sSSD_VARIABLE& rVar)
	{
		sSSD_VARIABLE sTemp;
		sTemp.eType = SSD_VARIABLE_NIL;
		sTemp.eResult = SSD_VARIABLE_RESULT_SUCCESS;

		switch (eType)
		{
			case SSD_VARIABLE_INTEGER:
			{
				switch (rVar.eType)
				{
					case SSD_VARIABLE_INTEGER:
					{
						sTemp.eType = SSD_VARIABLE_INTEGER;
						sTemp.nInteger = nInteger * rVar.nInteger;
					}
					break;
					case SSD_VARIABLE_FLOAT:
					{
						sTemp.eType = SSD_VARIABLE_FLOAT;
						sTemp.fFloat = (float)nInteger * rVar.fFloat;
					}
					break;
					case SSD_VARIABLE_VECTOR:
					{
						sTemp.eType = SSD_VARIABLE_VECTOR;
						sTemp.sVector.x = (float)nInteger * rVar.sVector.x;
						sTemp.sVector.y = (float)nInteger * rVar.sVector.y;
						sTemp.sVector.z = (float)nInteger * rVar.sVector.z;
					}
					break;

					default: sTemp.eType = SSD_VARIABLE_NIL; sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH; break;
				}
			}
			break;

			case SSD_VARIABLE_FLOAT:
			{
				switch (rVar.eType)
				{
					case SSD_VARIABLE_INTEGER:
					{
						sTemp.eType = SSD_VARIABLE_FLOAT;
						sTemp.fFloat = fFloat * (float)rVar.nInteger;
					}
					break;
					case SSD_VARIABLE_FLOAT:
					{
						sTemp.eType = SSD_VARIABLE_FLOAT;
						sTemp.fFloat = fFloat * rVar.fFloat;
					}
					break;
					case SSD_VARIABLE_VECTOR:
					{
						sTemp.eType = SSD_VARIABLE_VECTOR;
						sTemp.sVector.x = fFloat * rVar.sVector.x;
						sTemp.sVector.y = fFloat * rVar.sVector.y;
						sTemp.sVector.z = fFloat * rVar.sVector.z;
					}
					break;

					default: sTemp.eType = SSD_VARIABLE_NIL; sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH; break;
				}
			}
			break;

			case SSD_VARIABLE_VECTOR:
			{
				switch (rVar.eType)
				{
					case SSD_VARIABLE_FLOAT:
					{
						sTemp.eType = SSD_VARIABLE_VECTOR;
						sTemp.sVector.x = sVector.x * rVar.fFloat;
						sTemp.sVector.y = sVector.y * rVar.fFloat;
						sTemp.sVector.z = sVector.z * rVar.fFloat;
					}
					break;
					case SSD_VARIABLE_VECTOR:
					{
						sTemp.eType = SSD_VARIABLE_VECTOR;
						sTemp.sVector.x = sVector.x * rVar.sVector.x;
						sTemp.sVector.y = sVector.y * rVar.sVector.y;
						sTemp.sVector.z = sVector.z * rVar.sVector.z;
					}
					break;

					default: sTemp.eType = SSD_VARIABLE_NIL; sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH; break;
				}
			}
			break;

			default: sTemp.eType = SSD_VARIABLE_NIL; sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH; break;
		}

		return sTemp;
	}

	sSSD_VARIABLE operator/ (sSSD_VARIABLE& rVar)
	{
		sSSD_VARIABLE sTemp;
		sTemp.eType = SSD_VARIABLE_NIL;
		sTemp.eResult = SSD_VARIABLE_RESULT_SUCCESS;

		switch (eType)
		{
			case SSD_VARIABLE_INTEGER:
			{
				switch (rVar.eType)
				{
					case SSD_VARIABLE_INTEGER:
					{
						if (rVar.nInteger)
						{
							sTemp.eType = SSD_VARIABLE_FLOAT;
							sTemp.fFloat = (float)nInteger / (float)rVar.nInteger;
						}
						else
						{
							sTemp.eType = SSD_VARIABLE_NIL; 
							sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH;
						}
					}
					break;
					case SSD_VARIABLE_FLOAT:
					{
						if (rVar.fFloat == 0.0f)
						{
							sTemp.eType = SSD_VARIABLE_NIL;
							sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH;
						}
						else
						{
							sTemp.eType = SSD_VARIABLE_FLOAT;
							sTemp.fFloat = (float)nInteger / rVar.fFloat;
						}
					}
					break;
					case SSD_VARIABLE_VECTOR:
					{
						if (rVar.sVector.x != 0.0f && rVar.sVector.y != 0.0f && rVar.sVector.z != 0.0f)
						{
							sTemp.eType = SSD_VARIABLE_VECTOR;
							sTemp.sVector.x = (float)nInteger / rVar.sVector.x;
							sTemp.sVector.y = (float)nInteger / rVar.sVector.y;
							sTemp.sVector.z = (float)nInteger / rVar.sVector.z;
						}
						else
						{
							sTemp.eType = SSD_VARIABLE_NIL;
							sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH;
						}
					}
					break;

					default: sTemp.eType = SSD_VARIABLE_NIL; sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH; break;
				}
			}
			break;

			case SSD_VARIABLE_FLOAT:
			{
				switch (rVar.eType)
				{
					case SSD_VARIABLE_INTEGER:
					{
						if (rVar.nInteger)
						{
							sTemp.eType = SSD_VARIABLE_FLOAT;
							sTemp.fFloat = fFloat / (float)rVar.nInteger;
						}
						else
						{
							sTemp.eType = SSD_VARIABLE_NIL;
							sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH;
						}
					}
					break;
					case SSD_VARIABLE_FLOAT:
					{
						if (rVar.fFloat == 0.0f)
						{
							sTemp.eType = SSD_VARIABLE_NIL;
							sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH;
						}
						else
						{
							sTemp.eType = SSD_VARIABLE_FLOAT;
							sTemp.fFloat = fFloat / rVar.fFloat;
						}
					}
					break;
					case SSD_VARIABLE_VECTOR:
					{
						if (rVar.sVector.x != 0.0f && rVar.sVector.y != 0.0f && rVar.sVector.z != 0.0f)
						{
							sTemp.eType = SSD_VARIABLE_VECTOR;
							sTemp.sVector.x = fFloat / rVar.sVector.x;
							sTemp.sVector.y = fFloat / rVar.sVector.y;
							sTemp.sVector.z = fFloat / rVar.sVector.z;
						}
						else
						{
							sTemp.eType = SSD_VARIABLE_NIL;
							sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH;
						}
					}
					break;

					default: sTemp.eType = SSD_VARIABLE_NIL; sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH; break;
				}
			}
			break;

			case SSD_VARIABLE_VECTOR:
			{
				switch (rVar.eType)
				{
					case SSD_VARIABLE_INTEGER:
					{
						if (rVar.nInteger)
						{
							sTemp.eType = SSD_VARIABLE_VECTOR;
							sTemp.sVector.x = sVector.x / (float)rVar.nInteger;
							sTemp.sVector.y = sVector.y / (float)rVar.nInteger;
							sTemp.sVector.z = sVector.z / (float)rVar.nInteger;
						}
						else
						{
							sTemp.eType = SSD_VARIABLE_NIL;
							sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH;
						}
					}
					break;
					case SSD_VARIABLE_FLOAT:
					{
						if (rVar.fFloat == 0.0f)
						{
							sTemp.eType = SSD_VARIABLE_NIL;
							sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH;
						}
						else
						{
							sTemp.eType = SSD_VARIABLE_VECTOR;
							sTemp.sVector.x = sVector.x / rVar.fFloat;
							sTemp.sVector.y = sVector.y / rVar.fFloat;
							sTemp.sVector.z = sVector.z / rVar.fFloat;
						}
					}
					break;
					case SSD_VARIABLE_VECTOR:
					{
						sTemp.eType = SSD_VARIABLE_NIL;
						sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH;
					}
					break;

					default: sTemp.eType = SSD_VARIABLE_NIL; sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH; break;
				}
			}
			break;

			default: sTemp.eType = SSD_VARIABLE_NIL; sTemp.eResult = SSD_VARIABLE_RESULT_TYPE_IS_MISMATCH; break;
		}

		return sTemp;
	}

	eSSD_VARIABLE_TYPE eType;
	eSSD_VARIABLE_RESULT eResult;

	union
	{
		int nInteger;
		float fFloat;
		sVECTOR3 sVector;
	};
};

struct sSSD_MESSAGE
{
	enum eDEFINE
	{
		MAX_MESSAGE_LENGTH = 0x400,
	};

	int maxlength(void)
	{
		return MAX_MESSAGE_LENGTH;
	}

	int buffersize(void)
	{
		return 0;
	}

	char szMsg[MAX_MESSAGE_LENGTH + 1];
};

struct sSSD_VARNAME
{
	sSSD_VARNAME::sSSD_VARNAME(void)
	{
		memset(this, NULL, sizeof(sSSD_VARNAME));
	}

	sSSD_VARNAME* operator=(const char *szSrc)
	{
		memset(szVarName, NULL, 32 + 1);
		strncpy_s(szVarName, 32 + 1, szSrc, 32);
		return this;
	}

	char const* str(void)
	{
		return szVarName;
	}
	char szVarName[32 + 1];
};

struct sSSD_FORMULA_DECLARE
{
	sSSD_VARNAME sName;
	sSSD_VARIABLE sVariable;
};

struct sSSD_FORMULA_ENTITY;

struct sSSD_FORMULA_FUNCTION
{
	eSSD_FORMULA_FUNCTION_TYPE eFuncType;
	std::vector<sSSD_FORMULA_ENTITY> vecParam;
};

struct sSSD_FORMULA_ENTITY
{
	eSSD_FORMULA_ENTITY_TYPE eEntityType;
	sSSD_VARIABLE sConstant;
	sSSD_VARNAME sVarName;
	eSSD_FORMULA_ARITHMETIC_TYPE eArithmetic;
	sSSD_FORMULA_FUNCTION sFunction;
};

struct sSSD_FORMULA
{
	sSSD_FORMULA& operator=(sSSD_FORMULA& __that)
	{
		sResult = __that.sResult;
		vecEntities = __that.vecEntities;
		return (*this);
	}

	sSSD_VARNAME sResult;
	std::vector<sSSD_FORMULA_ENTITY> vecEntities;
};


struct sSSD_MOB_PAIR
{
	TBLIDX				tblidx;
	eSSD_MOB_PAIR_TYPE	ePairType;

	union
	{
		TBLIDX	pairTblidx;
		BYTE	pairIndex;
	};
};

inline void GetSSD_VARIABLEString(sSSD_VARIABLE& rVariable, int nBufferSize, char *szBuffer)
{
	switch (rVariable.eType)
	{
		case SSD_VARIABLE_INTEGER: sprintf_s(szBuffer, nBufferSize, "%d", rVariable.nInteger); break;
		case SSD_VARIABLE_FLOAT: sprintf_s(szBuffer, nBufferSize, "%f", rVariable.fFloat); break;
		case SSD_VARIABLE_VECTOR: sprintf_s(szBuffer, nBufferSize, "(%f,%f,%f)", rVariable.sVector.x, rVariable.sVector.y, rVariable.sVector.z); break;

		default: sprintf_s(szBuffer, nBufferSize, "error state : %d", rVariable.eType); break;
	}
}

inline void GetString_SSD_FORMULA_ENTITY(sSSD_FORMULA_ENTITY& sEntity, int nBufferSize, char *szBuffer)
{
	switch (sEntity.eEntityType)
	{
		case 0: GetSSD_VARIABLEString(sEntity.sConstant, nBufferSize, szBuffer); break;
		case 1:
		{
			sprintf_s(szBuffer, nBufferSize, "%s", sEntity.sVarName.szVarName);
		}
		break;
		case 2: sprintf_s(szBuffer, nBufferSize, "Arithmetic[%d]", sEntity.eArithmetic); break;
		case 3: sprintf_s(szBuffer, nBufferSize, "Function[%d]", sEntity.sFunction.eFuncType); break;
		default: sprintf_s(szBuffer, nBufferSize, "error state : %d", sEntity.eEntityType); break;
	}
}

//bool GetSvrScrEntity(sSSD_FORMULA_ENTITY& rEntity, std::string& rSrc, sSSD_MESSAGE& errmsg)
//{
//	return false;
//}


enum eSSL_TYPE
{
	SSL_PRINT,
	SSL_DBGINFO,
	SSL_WARNING,
	SSL_ERROR,
	SSL_SYSTEMERROR,

	SSL_TYPE_COUNT,
	INVALID_SSL_TYPE = 0xFF,
};


enum eBOTCONTROLID
{
	BOTCONTROL_ENTER = 0x0,
	BOTCONTROL_SPAWN = 0x1,
	BOTCONTROL_IDLE = 0x2,
	BOTCONTROL_FIGHT = 0x3,
	BOTCONTROL_BATTLESCENE = 0x4,
	BOTCONTROL_FAINT = 0x5,
	BOTCONTROL_TELEPORTING = 0x6,
	BOTCONTROL_DESPAWN = 0x7,
	BOTCONTROL_LEAVE = 0x8,
	BOTCONTROL_RETURN = 0x9,
	BOTCONTROL_FIRSTPROC_LIST = 0xA,
	BOTCONTROL_ACTION_LEAVE = 0xB,
	BOTCONTROL_ACTION_WAIT = 0xC,
	BOTCONTROL_ACTION_SITDOWN = 0xD,
	BOTCONTROL_ACTION_DESTMOVE = 0xE,
	BOTCONTROL_ACTION_NAVMOVE = 0xF,
	BOTCONTROL_ACTION_WANDERMOVE = 0x10,
	BOTCONTROL_ACTION_ROTATE = 0x11,
	BOTCONTROL_ACTION_EMOTION = 0x12,
	BOTCONTROL_ACTION_ATTACK = 0x13,
	BOTCONTROL_ACTION_CHASE = 0x14,
	BOTCONTROL_ACTION_DIALOG = 0x15,
	BOTCONTROL_ACTION_DIRECT_PLAY = 0x16,
	BOTCONTROL_ACTION_DIRECT_TURNING = 0x17,
	BOTCONTROL_ACTION_SET_CHAR_CONDITION = 0x18,
	BOTCONTROL_ACTION_SEND_SPS_EVENT = 0x19,
	BOTCONTROL_ACTION_ESCORT = 0x1A,
	BOTCONTROL_ACTION_SKILL_USE = 0x1B,
	BOTCONTROL_RANGED_ATTACK_MOVE = 0x1C,
	BOTCONTROL_ACTION_RETREAT = 0x1D,
	BOTCONTROL_ACTION_LOOK = 0x1E,
	BOTCONTROL_ACTION_TIMID = 0x1F,
	BOTCONTROL_ACTION_FLEE = 0x20,
	BOTCONTROL_ACTION_TERROR = 0x21,
	BOTCONTROL_ACTION_ADDMOB = 0x22,
	BOTCONTROL_ACTION_SPSEND = 0x23,
	BOTCONTROL_ACTION_TARGETFIXATTACK = 0x24,
	BOTCONTROL_ACTION_CHANGEAIBITFLAG = 0x25,
	BOTCONTROL_ACTION_CHANGE_TIMID_ATTRIBUTE = 0x26,
	BOTCONTROL_ACTION_CHANGE_CHAR_ATTRIBUTE = 0x27,
	BOTCONTROL_ACTION_CHANGE_NPC_ATTRIBUTE = 0x28,
	BOTCONTROL_ACTION_WANDER = 0x29,
	BOTCONTROL_ACTION_FIXLOC = 0x2A,
	BOTCONTROL_ACTION_FOLLOW = 0x2B,
	BOTCONTROL_ACTION_ESCORT_FOLLOW = 0x2C,
	BOTCONTROL_ACTION_SCENE_PLAY = 0x2D,
	BOTCONTROL_ACTION_PATROL_WAIT = 0x2E,
	BOTCONTROL_ACTION_PET_FOLLOW = 0x2F,
	BOTCONTROL_ACTION_PATROL = 0x30,
	BOTCONTROL_ACTION_RETURNMOVE = 0x31,
	BOTCONTROL_ACTION_FOLLOW_MOVE = 0x32,
	BOTCONTROL_ACTION_FUNCTION = 0x33,
	BOTCONTROL_ACTION_SEND_ESCORT_EVENT_TO_TS = 0x34,
	BOTCONTROL_ACTION_SEND_EVENT_TO_TQS = 0x35,
	BOTCONTROL_ACTION_SPS_SEND_EVENT_TO_WPS = 0x36,
	BOTCONTROL_ACTION_AIS_SEND_EVENT_TO_WPS = 0x37,
	BOTCONTROL_ACTION_AIS_SEND_EVENT_TO_TQS = 0x38,
	BOTCONTROL_ACTION_SPS_SEND_EVENT_TO_TS = 0x39,
	BOTCONTROL_ACTION_SPS_SEND_EVENT = 0x3A,
	BOTCONTROL_ACTION_CLEAR_SPS_EVENT = 0x3B,
	BOTCONTROL_ACTION_CALCULATE = 0x3C,
	BOTCONTROL_CONDITION_WAIT = 0x3D,
	BOTCONTROL_CONDITION_ATTACK_TARGET = 0x3E,
	BOTCONTROL_CONDITION_SCAN_TARGET = 0x3F,
	BOTCONTROL_CONDITION_FLEE = 0x40,
	BOTCONTROL_CONDITION_FACING = 0x41,
	BOTCONTROL_CONDITION_SPEECH = 0x42,
	BOTCONTROL_CONDITION_COMEBACK = 0x43,
	BOTCONTROL_CONDITION_RANGED_ATTACK = 0x44,
	BOTCONTROL_CONDITION_SKILL_USE = 0x45,
	BOTCONTROL_CONDITION_ALLIANCE_HELPME = 0x46,
	BOTCONTROL_CONDITION_RETREAT = 0x47,
	BOTCONTROL_CONDITION_TIMID = 0x48,
	BOTCONTROL_CONDITION_TERROR = 0x49,
	BOTCONTROL_CONDITION_CONFUSION = 0x4A,
	BOTCONTROL_CONDITION_FOLLOW_TELEPORT = 0x4B,
	BOTCONTROL_CONDITION_PET_FOLLOW = 0x4C,
	BOTCONTROL_CONDITION_PET_FOLLOW_TELEPORT = 0x4D,
	BOTCONTROL_CONDITION_DEFEND = 0x4E,
	BOTCONTROL_CONDITION_HIMSELFCHECK = 0x4F,
	BOTCONTROL_CONDITION_NONBATTLEACTIONPATTERNSET = 0x50,
	BOTCONTROL_CONDITION_SCAN = 0x51,
	BOTCONTROL_CONDITION_CHILD = 0x52,
	BOTCONTROL_CONDITION_SPS_RECV_EVENT_FROM_TS = 0x53,
	BOTCONTROL_CONDITION_SPS_RECV_EVENT_FROM_WPS = 0x54,
	BOTCONTROL_CONDITION_SPS_RECV_EVENT_FROM_ITEM = 0x55,
	BOTCONTROL_CONDITION_AIS_RECV_EVENT_FROM_WPS = 0x56,
	BOTCONTROL_CONDITION_AIS_RECV_EVENT_FROM_TQS = 0x57,
	BOTCONTROL_CONDITION_OUT_OF_NEST = 0x58,
	BOTCONTROL_CONDITION_REGISTER_BATTLE_SCENE = 0x59,
	BOTCONTROL_CONDITION_BS_CONDITION = 0x5A,
	BOTCONTROL_CONDITION_BS_CONDITION_GROUP = 0x5B,
	BOTCONTROL_CONDITION_BS_FLOW = 0x5C,
	BOTCONTROL_CONDITION_BS_CHECK_LP = 0x5D,
	BOTCONTROL_CONDITION_BS_CHECK_MOBGROUP = 0x5E,
	BOTCONTROL_CONDITION_BS_CHECK_NPC = 0x5F,

	MAX_BOTCONTROLID = 0x60,
	INVALID_BOTCONTROLID = 0x60,
};



enum eBOT_ACTIONSTATUS
{
	BOT_ACTIONSTATUS_PLAYSCENE = 0x0,
	BOT_ACTIONSTATUS_WANDER = 0x1,
	BOT_ACTIONSTATUS_FIXLOC = 0x2,
	BOT_ACTIONSTATUS_PATROL = 0x3,
	BOT_ACTIONSTATUS_FOLLOW = 0x4,
	BOT_ACTIONSTATUS_FACING = 0x5,
	BOT_ACTIONSTATUS_TERROR = 0x6,
	BOT_ACTIONSTATUS_CONFUSED = 0x7,
	BOT_ACTIONSTATUS_TIMID_MOVE = 0x8,
	BOT_ACTIONSTATUS_OUT_OF_NEST = 0x9,
	BOT_ACTIONSTATUS_PET_FOLLOW = 0xA,
	BOT_ACTIONSTATUS_DOING_BATTLE = 0xB,
	BOT_ACTIONSTATUS_DOING_RETURN = 0xC,
};






#endif