#include "precomp_dboclient.h"
#include "DisplayParser.h"
#include "NtlDebug.h"
#include "NtlTokenizer.h"
#include "NtlPLResourcePack.h"
#include "DboCompileOpt.h"


CDisplayParser::CDisplayParser(const std::string &strDataFileName)
{
	if( !ParseUnicodeData(strDataFileName) )
		return;
}

//RwBool CDisplayParser::ParseData(const std::string &strFileName)
//{
//	NTL_FUNCTION("CDisplayParser::ParseData");
//	
//	CallTokenPack fnTokenPack = NULL;
//
//	if( GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_LANGUAGE )
//		fnTokenPack = LanguagePackLoad;
//	
//	CNtlTokenizer lexer( strFileName, fnTokenPack );
//
//	if(!lexer.IsSuccess())
//	{
//		NTL_RETURN(FALSE);
//	}
//
//	enum EDataParseState
//	{
//		PARSE_DATA_VARIABLE_TYPE,   // display data variable type
//		PARSE_DATA_VARIABLE_STRING	// display data variable string
//	};
//
//	m_DataOption.Clear();
//
//	EDataParseState eState = PARSE_DATA_VARIABLE_TYPE;
//	RwInt32 iOldLine = 0;
//	RwInt32 iLine;
//	RwChar chType[1024];
//	
//
//	while(1)
//	{
//		std::string strToken = lexer.PeekNextToken(NULL, &iLine);
//
//		if(strToken == "") 
//			break;
//
//		if(iOldLine != iLine)
//		{
//			eState = PARSE_DATA_VARIABLE_TYPE;
//			iOldLine = iLine;
//		}
//
//		switch (eState)
//		{
//		case PARSE_DATA_VARIABLE_TYPE:
//			if(strToken == "=")
//				eState = PARSE_DATA_VARIABLE_STRING;
//			else
//			{
//#if defined(_MSC_VER) && (_MSC_VER >= 1400)
//				strcpy_s(chType, 1024, strToken.c_str());
//#else
//				strcpy(chType, strToken.c_str());
//#endif
//			}
//			break;
//		case PARSE_DATA_VARIABLE_STRING:
//			m_DataOption.AddOption(chType, strToken); 
//			break;
//		}
//
//		lexer.PopToPeek();
//	}
//
//	NTL_RETURN(TRUE);
//}
//
//RwBool CDisplayParser::ParseDef(const std::string &strFileName)
//{
//	NTL_FUNCTION("CDisplayParser::ParseDef");
//
//	CallTokenPack fnTokenPack = NULL;
//
//	if( GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_LANGUAGE )
//		fnTokenPack = LanguagePackLoad;
//
//	CNtlTokenizer lexer( strFileName, fnTokenPack );
//
//	if(!lexer.IsSuccess())
//	{
//		NTL_RETURN(FALSE);
//	}
//
//	enum EDefParseState
//	{
//		PARSE_DEF_GLOBAL,
//		PARSE_DEF_VARI_START,
//		PARSE_DEF_VARI_NAME, 
//		PARSE_DEF_VARI_VALUE, 
//		RARSE_DEF_VARI_VALUE1
//	};
//
//	m_mapDefOption.clear();
//
//	EDefParseState eState = PARSE_DEF_GLOBAL;
//	RwInt32 iIdx = 0;
//	RwBool bRunning = TRUE;
//	RwChar chType[1024];
//	
//	while(bRunning)
//	{
//		std::string strToken = lexer.PeekNextToken();
//
//		if(strToken == "") 
//			break;
//
//		switch (eState)
//		{
//		case PARSE_DEF_GLOBAL:
//			if(strToken == "enum")
//				eState = PARSE_DEF_VARI_START;
//			break;
//		case PARSE_DEF_VARI_START:
//			if(strToken == "{")
//				eState = PARSE_DEF_VARI_NAME;
//			break;
//		case PARSE_DEF_VARI_NAME:
//			{
//#if defined(_MSC_VER) && (_MSC_VER >= 1400)
//				strcpy_s(chType, 1024, strToken.c_str()); 
//				eState  = PARSE_DEF_VARI_VALUE;
//#else
//				strcpy(chType, strToken.c_str()); 
//				eState  = PARSE_DEF_VARI_VALUE;
//#endif
//			}
//			break;
//		case PARSE_DEF_VARI_VALUE:
//			if(strToken == ",")
//			{
//				m_mapDefOption[chType] = iIdx;
//				iIdx++;
//				eState = PARSE_DEF_VARI_NAME;
//			}
//			else if(strToken == "=")
//			{
//				eState = RARSE_DEF_VARI_VALUE1;
//			}
//			else if(strToken == "}")
//			{
//				m_mapDefOption[chType] = iIdx;
//				bRunning = FALSE;
//			}
//			
//			break;
//		case RARSE_DEF_VARI_VALUE1:
//			if(strToken == ",")
//			{
//				m_mapDefOption[chType] = iIdx;
//				iIdx++;
//				eState = PARSE_DEF_VARI_NAME;
//			}
//			else if(strToken == "}")
//			{
//				m_mapDefOption[chType] = iIdx;
//				bRunning = FALSE;
//			}
//			else
//			{
//				iIdx = atoi(strToken.c_str()); 
//			}
//			break;
//		}
//
//		lexer.PopToPeek();
//	}
//
//	NTL_RETURN(TRUE);
//}

RwBool CDisplayParser::ParseUnicodeData( const std::string &strFilename )
{
	NTL_FUNCTION("CDisplayParser::ParseUnicodeData");

	CallTokenPack fnTokenPack = NULL;

	if( GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_LANGUAGE )
		fnTokenPack = LanguagePackLoad;

	CNtlTokenizerW lexer( strFilename, fnTokenPack );

	if(!lexer.IsSuccess())
	{
		NTL_RETURN(FALSE);
	}

	enum EDataParseState
	{
		PARSE_DATA_VARIABLE_TYPE,   // display data variable type
		PARSE_DATA_VARIABLE_STRING	// display data variable string
	};

	m_DataOption.clear();

	EDataParseState eState = PARSE_DATA_VARIABLE_TYPE;
	RwInt32 iOldLine = 0;
	RwInt32 iLine;
	char chType[1024];

	while(1)
	{
		std::wstring wstrToken = lexer.PeekNextToken(NULL, &iLine);

		if(wstrToken == L"") 
			break;

		if(iOldLine != iLine)
		{
			eState = PARSE_DATA_VARIABLE_TYPE;
			iOldLine = iLine;
		}

		switch (eState)
		{
		case PARSE_DATA_VARIABLE_TYPE:
			if(wstrToken == L"=")
				eState = PARSE_DATA_VARIABLE_STRING;
			else
			{
				::WideCharToMultiByte(GetACP(), 0, wstrToken.c_str(), -1, chType, 1024, NULL, NULL);
			}
			break;
		case PARSE_DATA_VARIABLE_STRING:
			m_DataOption[chType] = wstrToken;
			break;
		}

		lexer.PopToPeek();
	}

	NTL_RETURN(TRUE);
}
//
//RwBool CDisplayParser::ParseUnicodeDef( const std::string &strFilename )
//{
//	NTL_FUNCTION("CDisplayParser::ParseUnicodeDef");
//
//	CallTokenPack fnTokenPack = NULL;
//
//	if( GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_LANGUAGE )
//		fnTokenPack = LanguagePackLoad;
//
//	CNtlTokenizerW lexer( strFilename, fnTokenPack );
//
//	if(!lexer.IsSuccess())
//	{
//		NTL_RETURN(FALSE);
//	}
//
//	enum EDefParseState
//	{
//		PARSE_DEF_GLOBAL,
//		PARSE_DEF_VARI_START,
//		PARSE_DEF_VARI_NAME, 
//		PARSE_DEF_VARI_VALUE, 
//		RARSE_DEF_VARI_VALUE1
//	};
//
//	m_mapDefOptionW.clear();
//
//	EDefParseState eState = PARSE_DEF_GLOBAL;
//	RwInt32 iIdx = 0;
//	RwBool bRunning = TRUE;
//	wchar_t chType[1024];
//
//	while(bRunning)
//	{
//		std::wstring wstrToken = lexer.PeekNextToken();
//
//		if(wstrToken == L"") 
//			break;
//
//		switch (eState)
//		{
//		case PARSE_DEF_GLOBAL:
//			if(wstrToken == L"enum")
//				eState = PARSE_DEF_VARI_START;
//			break;
//		case PARSE_DEF_VARI_START:
//			if(wstrToken == L"{")
//				eState = PARSE_DEF_VARI_NAME;
//			break;
//		case PARSE_DEF_VARI_NAME:
//			{
//#if defined(_MSC_VER) && (_MSC_VER >= 1400)
//				wcscpy_s(chType, 1024, wstrToken.c_str());
//				eState  = PARSE_DEF_VARI_VALUE;
//#else
//				wcscpy(chType, wstrToken.c_str());
//				eState  = PARSE_DEF_VARI_VALUE;
//#endif
//			}
//			break;
//		case PARSE_DEF_VARI_VALUE:
//			if(wstrToken == L",")
//			{
//				m_mapDefOptionW[chType] = iIdx;
//				iIdx++;
//				eState = PARSE_DEF_VARI_NAME;
//			}
//			else if(wstrToken == L"=")
//			{
//				eState = RARSE_DEF_VARI_VALUE1;
//			}
//			else if(wstrToken == L"}")
//			{
//				m_mapDefOptionW[chType] = iIdx;
//				bRunning = FALSE;
//			}
//
//			break;
//		case RARSE_DEF_VARI_VALUE1:
//			if(wstrToken == L",")
//			{
//				m_mapDefOptionW[chType] = iIdx;
//				iIdx++;
//				eState = PARSE_DEF_VARI_NAME;
//			}
//			else if(wstrToken == L"}")
//			{
//				m_mapDefOptionW[chType] = iIdx;
//				bRunning = FALSE;
//			}
//			else
//			{
//				iIdx = _wtoi(wstrToken.c_str()); 
//			}
//			break;
//		}
//
//		lexer.PopToPeek();
//	}
//
//	NTL_RETURN(TRUE);
//}
/*
std::string CGUIFileParser::EvaluateExpression(std::list<std::string> &expression)
{
	bool is_expression = true;
	std::string result;
	std::list<std::string>::iterator it = expression.begin();
	for (;it!=expression.end();it++)
	{
		std::string &s = *it;
		if (s == "+" || s == "-" || s == "*" || s == "/" || s == "(" || s == ")")
		{
			continue;
		}
		const char *str = s.c_str();
		for (;*str;str++)
		{
			if ((*str < '0' || *str > '9') && *str != '.')
			{
				break;
			}
		}
		if (*str)
		{
			is_expression = false;
			break;
		}
	}

	if (!is_expression)
	{
		return ConcatenateStringlist(expression);
	}

	
	int op = 0;
	float result_val = 0.0f;
	for (it=expression.begin();it!=expression.end();it++)
	{
		std::string &s = *it;
		if (s == "+") op = 0;
		else if (s == "-") op = 1;
		else if (s == "*") op = 2;
		else if (s == "/") op = 3;
		else if (s == "(" || s == ")")
		{
			continue;
		}
		else
		{
			if (op == -1) 
			{
				assert(0 && "Could not evaluate expresstion");
			}
			float val = (float)atof(s.c_str());
			switch (op)
			{
			case 0:
				result_val += val;
				break;
			case 1:
				result_val -= val;
				break;
			case 2:
				result_val *= val;
				break;
			case 3:
				result_val /= val;
				break;
			default:
				result_val = val;
				break;
			}
			op = -1;
		}
	}


	std::string str;
	char buf[20];
	int nRVal = (int)(result_val+0.5);
	sprintf(buf,"%d",nRVal);
	str = buf;
	return str;
	}

}

*/