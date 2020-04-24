#include "precomp_ntlpresentation.h"
#include "NtlPLEmblemStuffList.h"

// core
#include "NtlDebug.h"

// gui
#include "gui_precomp.h"

// presentation
#include "NtlPLEmblemMaker.h"

#define dEMBLEM_STUFF_FILE_NAME		".\\script\\emblemstuff.dat"


CallbackLoadEmblemStuff g_fnCallback_LoadEmblemStuff = NULL;

void LinkLoadEmblemStuff(CallbackLoadEmblemStuff fn)
{
	g_fnCallback_LoadEmblemStuff = fn;
}

void UnLinkLoadEmblemStuff()
{
	g_fnCallback_LoadEmblemStuff = NULL;
}


CNtlPLEmblemStuffList::CNtlPLEmblemStuffList()
{
}

CNtlPLEmblemStuffList::~CNtlPLEmblemStuffList()
{
}

RwBool CNtlPLEmblemStuffList::Create()
{
	NTL_FUNCTION( "CNtlPLEmblemStuffList::Create" );

	LoadScript();

	NTL_RETURN(TRUE);
}

VOID CNtlPLEmblemStuffList::Destroy()
{
	NTL_FUNCTION("CGuildEmblemMakerGui::Destroy");

	for( RwUInt8 i = 0 ; i < NUM_EMBLEMTYPE ; ++i )
		m_mapStuff[i].clear();

	NTL_RETURNVOID();
}

VOID CNtlPLEmblemStuffList::LoadScript()
{
#define dEMBLEM_SCRIPT_COMMENT			"//"
#define dEMBLEM_SCRIPT_TYPE_A_ITEM		"<EmblemTypeA>"
#define dEMBLEM_SCRIPT_TYPE_B_ITEM		"<EmblemTypeB>"
#define dEMBLEM_SCRIPT_TYPE_C_ITEM		"<EmblemTypeC>"
#define dEMBLEM_SCRIPT_TYPE_END			"<End>"

	RwChar* pBuffer = NULL;

	if( g_fnCallback_LoadEmblemStuff )
	{
		RwInt32 iSize;
		RwChar* pcData = NULL;

		(*g_fnCallback_LoadEmblemStuff)(dEMBLEM_STUFF_FILE_NAME, (void**)&pcData, &iSize);

		if( NULL == pcData )
		{
			DBO_FAIL( "Can not load scprit file of name : " << dEMBLEM_STUFF_FILE_NAME );
			return;
		}

		// 버퍼 + 1 생성
		pBuffer = NTL_NEW RwChar[iSize + 1];
        ZeroMemory(pBuffer, sizeof(RwChar) * (iSize + 1));
		memcpy(pBuffer, pcData, sizeof(char) * iSize);
		pBuffer[iSize] = '\0';
	}
	else
	{
		FILE* fp;
		fopen_s(&fp, dEMBLEM_STUFF_FILE_NAME, "r");

		if(!fp)
			return;

		fseek(fp, 0, SEEK_END);
		RwInt32 iSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		pBuffer = NTL_NEW RwChar[iSize+1];
		fread(pBuffer, iSize, 1, fp);
		pBuffer[iSize] = '\0';

		fclose(fp);
	}

	gui::RemoveSpaceForSelfBuffer(pBuffer);

	int iEmblemType = EMBLEMTYPE_A;
	int iNum = 0;
	RwChar acBuffer[128] = "";

	char* pSeek = strstr(pBuffer, ";");
	if( !pSeek )
	{
		NTL_ARRAY_DELETE(pBuffer);
		return;
	}

	while( true )
	{
		++pSeek;

		if( strncmp(pSeek, dEMBLEM_SCRIPT_TYPE_B_ITEM, strlen(dEMBLEM_SCRIPT_TYPE_B_ITEM)) == 0 )
		{
			iEmblemType = EMBLEMTYPE_B;
		}
		else if( strncmp(pSeek, dEMBLEM_SCRIPT_TYPE_C_ITEM, strlen(dEMBLEM_SCRIPT_TYPE_C_ITEM)) == 0 )
		{
			iEmblemType = EMBLEMTYPE_C;
		}
		else if( strncmp(pSeek, dEMBLEM_SCRIPT_TYPE_END, strlen(dEMBLEM_SCRIPT_TYPE_END)) == 0 )
		{
			break;
		}
		else
		{
			char* pFindNext = strstr(pSeek, ";");
			strncpy_s(acBuffer, 128, pSeek, pFindNext - pSeek);
			iNum = atoi(acBuffer);

			switch(iEmblemType)
			{
				case EMBLEMTYPE_A:
				{
					if (m_mapStuff[EMBLEMTYPE_A].find(iNum) == m_mapStuff[EMBLEMTYPE_A].end())
					{
						m_mapStuff[EMBLEMTYPE_A][iNum] = iNum;
					}
					else
						NTL_ASSERT(false, "CNtlPLEmblemStuffList::LoadScript, 'script\\EmblemStuff.dat' Stuff type A have duplication index : " << iNum);
				}
				break;
				case EMBLEMTYPE_B:
				{
					if (m_mapStuff[EMBLEMTYPE_B].find(iNum) == m_mapStuff[EMBLEMTYPE_B].end())
					{
						m_mapStuff[EMBLEMTYPE_B][iNum] = iNum;
					}
					else
						NTL_ASSERT(false, "CNtlPLEmblemStuffList::LoadScript, 'script\\EmblemStuff.dat' Stuff type B have duplication index : " << iNum);
				}
				break;
				case EMBLEMTYPE_C:
				{
					if (m_mapStuff[EMBLEMTYPE_C].find(iNum) == m_mapStuff[EMBLEMTYPE_C].end())
					{
						m_mapStuff[EMBLEMTYPE_C][iNum] = iNum;
					}
					else
						NTL_ASSERT(false, "CNtlPLEmblemStuffList::LoadScript, 'script\\EmblemStuff.dat' Stuff type C have duplication index : " << iNum);
				}
				break;
			}
		}

		pSeek = strstr(pSeek, ";");
	}

	NTL_ARRAY_DELETE(pBuffer);
}

const MAP_STUFF* CNtlPLEmblemStuffList::GetStuffList(eEmblemType eType)
{
	return &(m_mapStuff[eType]);
}