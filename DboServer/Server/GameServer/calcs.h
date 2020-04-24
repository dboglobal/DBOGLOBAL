#ifndef __INC_DBOG_GAME_CALC_H__
#define __INC_DBOG_GAME_CALC_H__

#include "SystemEffectTable.h"
#include "NtlAvatar.h"
#include "NtlBattle.h"
class CCharacterAtt;


//-----------------------------------------------------------------------------------
//		Purpose	: UPDATE 'WORD' ATTRIBUTE 
//		Return	: 
//-----------------------------------------------------------------------------------
inline void	CalcAttributeW(WORD& wAttribute, float fValue, BYTE byApplyType, bool bIsPlus, WORD wCap = INVALID_WORD)
{
	if(bIsPlus == true)
	{
		if(byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			wAttribute = UnsignedSafeIncrease<WORD>(wAttribute, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			float percentvalue = NtlRound((float)wAttribute * fValue / 100.0f);
			wAttribute = UnsignedSafeIncrease<WORD>(wAttribute, (WORD)percentvalue);
		}
		
		if (wCap != INVALID_WORD && wAttribute > wCap)
			wAttribute = wCap;
	}
	else
	{
		if(byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			wAttribute = UnsignedSafeDecrease<WORD>(wAttribute, (WORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			float percentvalue = NtlRound((float)wAttribute * fValue / 100.0f);
			wAttribute = UnsignedSafeDecrease<WORD>(wAttribute, (WORD)percentvalue);
		}

		if (wCap != INVALID_WORD && wAttribute < wCap)
			wAttribute = wCap;
	}

}
//-----------------------------------------------------------------------------------
//		Purpose	: UPDATE 'INT' ATTRIBUTE
//		Return	: 
//-----------------------------------------------------------------------------------
inline void	CalcAttributeN(int& nAttribute, float fValue, BYTE byApplyType, bool bIsPlus)
{
	if(bIsPlus == true)
	{
		if(byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			nAttribute += (int)fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			float percentvalue = NtlRound((float)nAttribute * fValue / 100.0f);
			nAttribute += (int)percentvalue;
		}
		else
		{
			NTL_PRINT(PRINT_APP,"!ERROR! CalcAttributeN failed. byApplyType %d not set !ERROR!", byApplyType);
		}
	}
	else
	{
		if(byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			nAttribute -= (int)fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			float percentvalue = NtlRound((float)nAttribute * fValue / 100.0f);
			nAttribute -= (int)percentvalue;
		}
		else
		{
			NTL_PRINT(PRINT_APP,"!ERROR! CalcAttributeN failed. byApplyType %d not set !ERROR!", byApplyType);
		}
	}
}
//-----------------------------------------------------------------------------------
//		Purpose	: UPDATE 'float' ATTRIBUTE
//		Return	: 
//-----------------------------------------------------------------------------------
inline void	CalcAttributeF(float& fAttribute, float fValue, BYTE byApplyType, bool bIsPlus, float fCap = INVALID_FLOAT)
{
	if(bIsPlus == true)
	{
		if(byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			fAttribute += fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			fAttribute += fAttribute * fValue / 100.0f;
		}
		else
		{
			NTL_PRINT(PRINT_APP,"!ERROR! CalcAttributeF failed. byApplyType %d not set !ERROR!", byApplyType);
		}

		if (fCap != INVALID_FLOAT && fCap > 0.f && fAttribute > fCap)
			fAttribute = fCap;
	}
	else
	{
		if(byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			fAttribute -= fValue;
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			float percentvalue = fAttribute * fValue / 100.0f;
			fAttribute -= percentvalue;
		}
		else
		{
			NTL_PRINT(PRINT_APP,"!ERROR! CalcAttributeF failed. byApplyType %d not set !ERROR!", byApplyType);
		}

		if (fCap != INVALID_FLOAT && fAttribute < fCap)
			fAttribute = fCap;
	}
}
//-----------------------------------------------------------------------------------
//		Purpose	: UPDATE 'DWOOD' ATTRIBUTE
//		Return	: 
//-----------------------------------------------------------------------------------
inline void	CalcAttributeDW(DWORD& dwAttribute, float fValue, BYTE byApplyType, bool bIsPlus)
{
	if(bIsPlus == true)
	{
		if(byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			dwAttribute = UnsignedSafeIncrease<DWORD>(dwAttribute, (DWORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			float percentvalue = NtlRound((float)dwAttribute * fValue / 100.0f);
			dwAttribute = UnsignedSafeIncrease<DWORD>(dwAttribute, (DWORD)percentvalue);
		}
		else
		{
			NTL_PRINT(PRINT_APP,"!ERROR! CalcAttributeDW failed. byApplyType %d not set !ERROR!", byApplyType);
		}
	}
	else
	{
		if(byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			dwAttribute = UnsignedSafeDecrease<DWORD>(dwAttribute, (DWORD)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			float percentvalue = NtlRound((float)dwAttribute * fValue / 100.0f);
			dwAttribute = UnsignedSafeDecrease<DWORD>(dwAttribute, (DWORD)percentvalue);
		}
		else
		{
			NTL_PRINT(PRINT_APP,"!ERROR! CalcAttributeDW failed. byApplyType %d not set !ERROR!", byApplyType);
		}
	}
}
//-----------------------------------------------------------------------------------
//		Purpose	: UPDATE 'BYTE' ATTRIBUTE
//		Return	: 
//-----------------------------------------------------------------------------------
inline void	CalcAttributeBYTE(BYTE& byAttribute, float fValue, BYTE byApplyType, bool bIsPlus)
{
	if (bIsPlus == true)
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			byAttribute = UnsignedSafeIncrease<BYTE>(byAttribute, (BYTE)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			float percentvalue = NtlRound((float)byAttribute * fValue / 100.0f);
			byAttribute = UnsignedSafeIncrease<BYTE>(byAttribute, (BYTE)percentvalue);
		}
		else
		{
			NTL_PRINT(PRINT_APP, "!ERROR! CalcAttributeBYTE failed. byApplyType %d not set !ERROR!", byApplyType);
		}
	}
	else
	{
		if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_VALUE)
		{
			byAttribute = UnsignedSafeDecrease<BYTE>(byAttribute, (BYTE)fValue);
		}
		else if (byApplyType == SYSTEM_EFFECT_APPLY_TYPE_PERCENT)
		{
			float percentvalue = NtlRound((float)byAttribute * fValue / 100.0f);
			byAttribute = UnsignedSafeDecrease<BYTE>(byAttribute, (BYTE)percentvalue);
		}
		else
		{
			NTL_PRINT(PRINT_APP, "!ERROR! CalcAttributeBYTE failed. byApplyType %d not set !ERROR!", byApplyType);
		}
	}
}


inline int MIN(int a, int b)
{
    return a < b ? a : b;
}

inline int MAX(int a, int b)
{
    return a > b ? a : b;
}


//-----------------------------------------------------------------------------------
//		Purpose	: 
//		Return	: 
//-----------------------------------------------------------------------------------
void Dbo_SetAvatarAttributeValue(CCharacterAtt* pCharAtt, eSYSTEM_EFFECT_CODE effectcode, float effectvalue, BYTE byApplyType);


//-----------------------------------------------------------------------------------
//		Purpose	: 
//		Return	: 
//-----------------------------------------------------------------------------------
void Dbo_SetRandomOptionValues(CCharacterAtt* pCharAtt, eSYSTEM_EFFECT_CODE effectcode, float effectvalue);



#endif