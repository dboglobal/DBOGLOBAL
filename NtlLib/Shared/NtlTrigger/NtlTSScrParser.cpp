#include "precomp_trigger.h"
#include "NtlTSScrParser.h"
#include "NtlTSScrTokenizer.h"
#include "NtlTSScrFactoryType.h"
#include "NtlTSScrFactory.h"
#include "NtlTSScrObject.h"
#include "NtlTSLog.h"
#include "NtlTSCoreStatic.h"


/** 
	Script parser
*/


bool CNtlTSScrParser::Parsing( const std::string& strScriptName, std::vector<CNtlTSScrObject*>& vecCreatedRootObjList )
{
	CNtlTSScrTokenizer clLexer( strScriptName, 0, 0 );

	if ( !Parsing( &clLexer ) )
	{
		printf( "Parsing error. Info[%s]. [%s]\n", strScriptName.c_str(), TS_CODE_TRACE() );
		return false;
	}

	if ( !Building( vecCreatedRootObjList ) )
	{
		printf( "Building error. Info[%s]. [%s]\n", strScriptName.c_str(), TS_CODE_TRACE() );
		return false;
	}

	return true;
}

bool CNtlTSScrParser::Parsing( const std::string& strScriptName, const char* pBuff, int nSize, std::vector<CNtlTSScrObject*>& vecCreatedRootObjList )
{
	CNtlTSScrTokenizer clLexer( strScriptName, pBuff, nSize );

	if ( !Parsing( &clLexer ) )
	{
		printf( "Parsing error. Info[%s]. [%s]\n", strScriptName.c_str(), TS_CODE_TRACE() );
		return false;
	}

	if ( !Building( vecCreatedRootObjList ) )
	{
		printf( "Building error. Info[%s]. [%s]\n", strScriptName.c_str(), TS_CODE_TRACE() );
		return false;
	}

	return true;
}

bool CNtlTSScrParser::Parsing( CNtlTSScrTokenizer* pLexer )
{
	enum ePARSESTATE
	{
		ePARSESTATE_GLOBAL,			// Global
		ePARSESTATE_OBJTYPE,		// Object type
		ePARSESTATE_OBJNAME,		// Object name
		ePARSESTATE_OBJ,			// Object
		ePARSESTATE_VARIABLENAME,	// Variable name
		ePARSESTATE_VARIABLEVALUE	// Variable value
	};

	typedef std::pair<std::string, sOBJ_INFO> pairdef_NamedObjPair;

	std::stack<pairdef_NamedObjPair>	stackdefObjInfoStack;
	std::map<std::string, int>			mapdefObjEnumerator;

	ePARSESTATE							eState = ePARSESTATE_GLOBAL;
	ePARSESTATE							ePrevState = ePARSESTATE_GLOBAL;
	std::string							strObjType;
	std::string							strObjName;
	std::string							strVariableName;
	std::string							strVariableValue;
	std::string							strTopLevelStyle;

	bool bLoop = true;
	while ( bLoop )
	{
		const std::string& strToken = pLexer->PeekNextToken();
		if ( "" == strToken ) break;

		CNtlTSScrFactoryType* pObjType = 0;

		switch ( eState )
		{
		case ePARSESTATE_GLOBAL:
			{
				pObjType = m_pObjFactory->GetFactoryType( strToken );

				if ( pObjType )
				{
					eState = ePARSESTATE_OBJTYPE;
					strObjType = strToken;
					strObjName = "";

					sOBJ_INFO Info;
					Info.strType = strToken;
					Info.pObj = 0;
					Info.pObjType = pObjType;
					stackdefObjInfoStack.push( pairdef_NamedObjPair( "", Info ) );
				}
				else
				{
					printf( "Can not find the object type. Info[%s]. [%s] \n", strToken.c_str(), TS_CODE_TRACE() );
					return false;
				}
			}
			break;

		case ePARSESTATE_OBJTYPE:
			{
				if ( "{" == strToken )
				{
					int nNextID = mapdefObjEnumerator[strObjType]++;
					sprintf_s( g_NtlTSString, "%d", nNextID );
					if ( strlen( g_NtlTSString ) == 1 ) 
					{
						strObjName = strObjType + "00" + g_NtlTSString;
					}
					else if ( strlen( g_NtlTSString ) == 2 )
					{
						strObjName = strObjType + "0" + g_NtlTSString;
					}
					else
					{
						strObjName = strObjType + g_NtlTSString;
					}
					stackdefObjInfoStack.top().first = strObjName;

					eState = ePARSESTATE_OBJ;

				}
				else
				{
					strObjName = strToken;
					stackdefObjInfoStack.top().first = strObjName;

					eState = ePARSESTATE_OBJNAME;
				}
			}
			break;

		case ePARSESTATE_OBJNAME:
			{
				if ( "{" != strToken )
				{
					printf( "\'{\' does not exist. Info[%s]. [%s] \n", strToken.c_str(), TS_CODE_TRACE() );
					return false;
				}

				eState = ePARSESTATE_OBJ;
			}
			break;

		case ePARSESTATE_OBJ:
			{
				pObjType = m_pObjFactory->GetFactoryType( strToken );
				if ( 0 != pObjType )
				{
					if ( stackdefObjInfoStack.empty() )
					{
						printf( "The object stack is empty. Info[%s]. [%s] \n", strToken.c_str(), TS_CODE_TRACE() );
						return false;
					}

					sOBJ_INFO Info;
					Info.strType = strToken;
					Info.pObj = 0;
					Info.pObjType = pObjType;

					stackdefObjInfoStack.push( pairdef_NamedObjPair( "", Info ) );
					strObjType = strToken;
					strObjName = "";

					eState = ePARSESTATE_OBJTYPE;
				}
				else if ( "}" == strToken )
				{
					if ( stackdefObjInfoStack.empty() )
					{
						printf( "The object stack is empty. Info[%s]. [%s] \n", strToken.c_str(), TS_CODE_TRACE() );
						return false;
					}

					pairdef_NamedObjPair defObjPair = stackdefObjInfoStack.top();
					stackdefObjInfoStack.pop();

					defObjPair.second.strName = defObjPair.first;
					std::pair<std::map<std::string, sOBJ_INFO>::iterator, bool> it;
					it = m_defObjMap.insert( std::map<std::string, sOBJ_INFO>::value_type( defObjPair.first, defObjPair.second ) );
					if ( !it.second )
					{
						printf( "Inserting a object is failed. [%s] \n", TS_CODE_TRACE() );
						return false;
					}

					m_defObjSortedMap.insert( mmapdef_ObjSortMap::value_type( (int)stackdefObjInfoStack.size(), &(*it.first).second ) );

					if ( !stackdefObjInfoStack.empty() )
					{
						pairdef_NamedObjPair& defParentObj = stackdefObjInfoStack.top();
						defParentObj.second.listChildren.push_back( &(*it.first).second );
					}
					else
					{
						eState = ePARSESTATE_GLOBAL;
					}
				}
				else if ( 1 == strToken.length() && pLexer->IsOperator( strToken[0] ) )
				{
					printf( "Parsing is failed. ParamInfo[%s]. [%s] \n", strToken.c_str(), TS_CODE_TRACE() );
					return false;
				}
				else
				{
					strVariableName = strToken;
					strVariableValue.erase();

					ePrevState = ePARSESTATE_OBJ;
					eState = ePARSESTATE_VARIABLENAME;
				}
			}
			break;

		case ePARSESTATE_VARIABLENAME:
			{
				if ( "=" == strToken )
				{
					eState = ePARSESTATE_VARIABLEVALUE;
				}
				else if ( strToken == ";" )
				{
					if ( !stackdefObjInfoStack.top().second.pObjType->HasProperty( strVariableName ) )
					{
						printf( "Don't have the variable type. Info[%s]. [%s] \n", strVariableName.c_str(), TS_CODE_TRACE() );
						return false;
					}

					stackdefObjInfoStack.top().second.Property.m_defProperty.insert( std::pair<std::string, std::string>( strVariableName, "" ) );
					strVariableName = "";
					strVariableValue.erase();

					eState = ePrevState;
				}
				else
				{
					printf( "Must be \'=\' or \';\'. Info[%s]. [%s] \n", strToken.c_str(), TS_CODE_TRACE() );
					return false;
				}
			}
			break;

		case ePARSESTATE_VARIABLEVALUE:
			{
				if ( ";" != strToken )
				{
					strVariableValue += strToken;
				}
				else
				{
					if ( stackdefObjInfoStack.empty() )
					{
						m_defGlobalList.push_back( pairdef_VariablePair( strVariableName, strVariableValue ) );
					}
					else if ( !stackdefObjInfoStack.top().second.pObjType->HasProperty( strVariableName ) )
					{
						printf( "Don't have the variable type. Info[%s]. [%s] \n", strVariableName.c_str(), TS_CODE_TRACE() );
						return false;
					}
					else
					{
						stackdefObjInfoStack.top().second.Property.m_defProperty.insert( std::make_pair( strVariableName, strVariableValue ) );
					}

					strVariableName = "";
					strVariableValue.erase();

					eState = ePrevState;
				}
			}
			break;
		}

		pLexer->PopToPeek();
	}

	return true;
}

bool CNtlTSScrParser::Building( std::vector<CNtlTSScrObject*>& vecCreatedRootObjList )
{

	for (mmapdef_ObjSortMap::iterator cit = m_defObjSortedMap.begin(); cit != m_defObjSortedMap.end(); ++cit )
	{
		sOBJ_INFO* pInfo = (*cit).second;

		if ( 0 == pInfo->pObj )
		{
			pInfo->pObj = m_pObjFactory->CreateObj( pInfo->strType.c_str() );
			if ( 0 == pInfo->pObj )
			{
				printf( "Creating the object is failed. Info[%s]. [%s] \n", pInfo->strType.c_str(), TS_CODE_TRACE() );
				return false;
			}

			pInfo->pObj->ApplyScriptDataForScript( pInfo->Property );

			vecCreatedRootObjList.push_back( pInfo->pObj );
		}

		if ( !pInfo->listChildren.empty() )
		{
			for (std::list< sOBJ_INFO* >::iterator cit2 = pInfo->listChildren.begin(); cit2 != pInfo->listChildren.end(); ++cit2 )
			{
				sOBJ_INFO* pChildInfo = *cit2;
				if ( 0 == pChildInfo->pObj )
				{
					pChildInfo->pObj = m_pObjFactory->CreateObj( pChildInfo->strType.c_str() );
					if ( 0 == pChildInfo->pObj )
					{
						printf( "Creating the object is failed. Info[%s]. [%s]", pChildInfo->strType.c_str(), TS_CODE_TRACE() );
						return false;
					}

					pChildInfo->pObj->ApplyScriptDataForScript( pChildInfo->Property );

					pInfo->pObj->AttachChildForScript( pChildInfo->pObj );
					pChildInfo->pObj->SetParentForScript( pInfo->pObj );
				}
			}
			pInfo->listChildren.clear();
		}
	}

	return true;
}
