#include "stdafx.h"
#include "SvrScrFormulaToken.h"

const char *g_szSSD_FORMULA_REMOVER[5] = { " ", "\t", "\n", "\r", ";" };
const char *g_szSSD_FORMULA_SEPARATOR[14] = { "<=", ">=", "!=", "==", "<", ">", "+", "-", "*", "/", "=", "(", ")", "," };

CSvrScrFormulaToken::CSvrScrFormulaToken()
{
}


CSvrScrFormulaToken::~CSvrScrFormulaToken()
{
}



bool CSvrScrFormulaToken::Create()
{
	for (int j = 0; j < 5; j++)
	{
		std::string remover = g_szSSD_FORMULA_REMOVER[j];
		AddRemover(remover);
	}

	for (int k = 0; k < 14; k++)
	{
		std::string seperator = g_szSSD_FORMULA_SEPARATOR[k];
		AddSeparator(seperator);
	}

	return true;
}