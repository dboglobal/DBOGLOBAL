#include "precomp_dboclient.h"
#include "WorldMapScript.h"

// core
#include "NtlDebug.h"

// util
#include "NtlTokenizer.h"


CWorldMapScript::CWorldMapScript()
{

}

CWorldMapScript::~CWorldMapScript()
{

}

sMAP_NODE* CWorldMapScript::LoadScript()
{
	std::string strFileName = "script/WorldmapNode.txt";
	CNtlTokenizer lexer(strFileName);
	if( !lexer.IsSuccess() )
		return NULL;

	RwInt32 iLine;	

	while( TRUE )
	{
		const std::string& strToken = lexer.PeekNextToken(NULL, &iLine);
		if( strToken.empty() )
			break;

		if( strToken == "Node" )
		{
			return LoadChildNode(&lexer);
		}	
	}	

	return NULL;
}

sMAP_NODE* CWorldMapScript::LoadChildNode(CNtlTokenizer* pLexer)
{	
	RwInt32 iLine;
	const std::string& strToken1 = pLexer->PeekNextToken(NULL, &iLine);
	RwInt32 iNodeIndex = atoi( strToken1.c_str() );

	if( iNodeIndex == 0 )
		return NULL;

	RwBool bSuccess		= TRUE;
	sMAP_NODE* pNode	= NTL_NEW sMAP_NODE;

	pNode->uiNodeID		= iNodeIndex;
	

	while( TRUE )
	{
		const std::string& strToken = pLexer->PeekNextToken(NULL, &iLine);
		if( strToken == "}" )
		{
			break;
		}
		else if( strToken == "Node" )
		{
			sMAP_NODE* pChildNode = LoadChildNode(pLexer);
			if( !pChildNode )
			{
				UnloadScript(pNode);
				return NULL;
			}

			pNode->mapChildNode[pChildNode->uiNodeID] = pChildNode;
		}
		else if( strToken == "MainTexture" )
		{
			const std::string& strToken1 = pLexer->PeekNextToken(NULL, &iLine);
			if( strToken1 != "=" )
			{
				UnloadScript(pNode);
				return NULL;
			}

			pNode->strMainTexture = pLexer->PeekNextToken(NULL, &iLine);
		}
		else if( strToken == "ChildFocusResourceFile" )
		{
			const std::string& strToken1 = pLexer->PeekNextToken(NULL, &iLine);
			if( strToken1 != "=" )
			{
				UnloadScript(pNode);
				return NULL;
			}

			pNode->strChildResourceFile = pLexer->PeekNextToken(NULL, &iLine);
		}
		else if( strToken == "ChildFocusSurfaceFile" )
		{
			const std::string& strToken1 = pLexer->PeekNextToken(NULL, &iLine);
			if( strToken1 != "=" )
			{
				UnloadScript(pNode);
				return NULL;
			}

			pNode->strChildSurfaceFile = pLexer->PeekNextToken(NULL, &iLine);
		}
		else if( strToken == "FocusPosition_X" )
		{
			const std::string& strToken1 = pLexer->PeekNextToken(NULL, &iLine);
			if( strToken1 != "=" )
			{
				UnloadScript(pNode);
				return NULL;
			}

			const std::string& strToken2 = pLexer->PeekNextToken(NULL, &iLine);
			pNode->iFocusPosX = atoi( strToken2.c_str() );
		}
		else if( strToken == "FocusPosition_Y" )
		{
			const std::string& strToken1 = pLexer->PeekNextToken(NULL, &iLine);
			if( strToken1 != "=" )
			{
				UnloadScript(pNode);
				return NULL;
			}

			const std::string& strToken2 = pLexer->PeekNextToken(NULL, &iLine);
			pNode->iFocusPosY = atoi( strToken2.c_str() );
		}
		else if( strToken == "FocusSurfaceName" )
		{
			const std::string& strToken1 = pLexer->PeekNextToken(NULL, &iLine);
			if( strToken1 != "=" )
			{
				UnloadScript(pNode);
				return NULL;
			}

			pNode->strFocusSurfaceName = pLexer->PeekNextToken(NULL, &iLine);
		}
	}

	return pNode;
}

VOID CWorldMapScript::UnloadScript(sMAP_NODE* pNode)
{
	if( !pNode )
		return;

	MAP_MAPNODE::iterator it = pNode->mapChildNode.begin();
	for( ; it != pNode->mapChildNode.end() ; ++it )
	{
		UnloadScript( it->second );
	}
	pNode->mapChildNode.clear();

	NTL_DELETE( pNode );
}