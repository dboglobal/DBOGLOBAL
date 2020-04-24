#include "precomp_dboclient.h"
#include "DboGlobal.h"

// shared
#include "NtlClientNet.h"
#include "NtlXMLDoc.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlNetSender.h"
#include "NtlSobAvatar.h"
#include "NtlSobManager.h"
#include "NtlSobStatusAnimSyncManager.h"
#include "NtlNetSender.h"

// dbo
#include "DboDef.h"
#include "DboVirtualServer.h"
#include "DumpCommand.h"
#include "PacketProc.h"

CDboGlobal* CDboGlobal::m_pInstance = 0;

#define dCONTRACT_FILE			"script\\Contract.htm"


CallbackLoadContract g_fnCallback_LoadContract = NULL;

void LinkLoadContract(CallbackLoadContract fn)
{
	g_fnCallback_LoadContract = fn;
}

void UnLinkLoadContract()
{
	g_fnCallback_LoadContract = NULL;
}


void SConnectData::Init(void)
{
	bChangingGameServer = FALSE;

	sAuthCon.Init();
	sCharCon.Init();
	sGameCon.Init();
	sChatCon.Init();

	listCharServer.clear();	
}

void SConnectData::Destroy(void)
{
	ResetCharacterServer();	
}

void SConnectData::ResetCharacterServer(void)
{
	SCharServerInfo *pInfo;
	ListCharServer::iterator it;
	
	for(it = listCharServer.begin(); it != listCharServer.end(); it++)
	{
		pInfo = (*it);
		NTL_DELETE(pInfo);
	}

	listCharServer.clear();
}

void SConnectData::AddCharacterServer(const RwChar *pServerIP, RwUInt16 wServerPort, RwUInt32 uiLoad)
{
	SCharServerInfo *pAddInfo = NTL_NEW SCharServerInfo;

	strcpy_s(pAddInfo->chServerIP, pServerIP);
	pAddInfo->wServerPort	= wServerPort;
	pAddInfo->uiLoad		= uiLoad;
	
	listCharServer.push_back(pAddInfo);
}

RwBool SConnectData::PopCharacterServer(SCharServerInfo *pInfo)
{
	if(listCharServer.size() == 0)
		return FALSE;

	SCharServerInfo *pBestInfo = NULL;

	RwUInt32 uiLoad = 1000000000;
	ListCharServer::iterator it;
	ListCharServer::iterator itbest;
	for(it = listCharServer.begin(); it != listCharServer.end(); it++)
	{
		if((*it)->uiLoad < uiLoad)
		{
			itbest = it;
			pBestInfo = (*it);
		}
	}

	listCharServer.erase(itbest);
	memcpy(pInfo, pBestInfo, sizeof(SCharServerInfo));

	NTL_DELETE(pBestInfo);

	return TRUE;
}

CDboGlobal::CDboGlobal()
:m_bySelectedServerHandle(INVALID_SERVERFARMID)
,m_byPostSelectedServerHandle(INVALID_SERVERFARMID)
,m_pNetwork(NULL)
,m_pLoginNetSender(NULL)
,m_pLobbyNetSender(NULL)
,m_pGameNetSender(NULL)
,m_pChatNetSender(NULL)
,m_pLoginPacketGenerator(NULL)
,m_pLobbyPacketGenerator(NULL)
,m_pGamePacketGenerator(NULL)
,m_pChatPacketGenerator(NULL)
,m_bySelectedCharIndex(INVALID_BYTE)
,m_bEnterTutorial(FALSE)
,m_bDirectionFirstConnect(FALSE)
,m_pDBOUIConfig(NULL)
,m_pContractInfo(NULL)
{
	NTL_PRE(m_pInstance == 0);
	m_pInstance = this;	

	m_sUser.Init();
	memset(&m_sConfig, 0, sizeof(SConfigData));
	ZeroMemory(&m_sDBCSheduleInfo, sizeof(SNtlEventDBCSchedule_Info));
	m_sConnect.Init();
	m_sGame.Init();		
}

CDboGlobal::~CDboGlobal()
{
	if( m_pContractInfo )
	{
		NTL_ARRAY_DELETE(m_pContractInfo->pcContract);
		NTL_ARRAY_DELETE(m_pContractInfo->pwcContract);
		NTL_DELETE(m_pContractInfo);
	}

	m_sConnect.Destroy();
}	

CDboGlobal* CDboGlobal::GetInstance(void)
{
	return m_pInstance;
}

RwBool CDboGlobal::Create(HWND hWnd)
{
	NTL_FUNCTION("CDboGlobal::Create");

	m_pNetwork = NTL_NEW CNtlClientNet;
	if(!m_pNetwork->Create(NULL, NetworkEventHander, NetworkLogCallback))
	{
		DBO_FAIL("Fail create client network");
		NTL_RETURN(FALSE);
	}

	if(!LoadConfigOptions("ConfigOptions.xml"))
	{
		DBO_FAIL("Fail load ConfigOptions.xml");
		NTL_RETURN(FALSE);
	}

#ifdef NET_SERVER_SYNC
	
	m_pLoginNetSender = NTL_NEW CNtlNetSender;					
	m_pLobbyNetSender = NTL_NEW CNtlNetSender;					
	m_pGameNetSender = NTL_NEW CNtlNetSender;					
	m_pChatNetSender = NTL_NEW CNtlNetSender;					

#else

	m_pLoginNetSender = NTL_NEW CNtlVirtualNetSender;					
	m_pLobbyNetSender = NTL_NEW CNtlVirtualNetSender;					
	m_pGameNetSender = NTL_NEW CNtlVirtualNetSender;					
	m_pChatNetSender = NTL_NEW CNtlVirtualNetSender;	

	CNtlVirtualNetSender::SetVirtualNetSenderFunction(DboVirtualNetSenderFunc); 

#endif

	// net sender callback  등록.
	CNtlNetSender::LinkNotifySendPacketFunc(CallbackNetSendPacket);

	// packet generator 생성.
	m_pLoginPacketGenerator = NTL_NEW CLoginPacketGenerator(m_pLoginNetSender);	
	m_pLobbyPacketGenerator = NTL_NEW CLobbyPacketGenerator(m_pLobbyNetSender);	
	m_pGamePacketGenerator = NTL_NEW CGamePacketGenerator(m_pGameNetSender);	
	m_pChatPacketGenerator = NTL_NEW CChatPacketGenerator(m_pChatNetSender);	

	// packet handler register
	RegisterPacketHandler();

    // DBO UI Config
    m_pDBOUIConfig = NTL_NEW CDBOUIConfig();

	NTL_RETURN(TRUE);
}

void CDboGlobal::Destroy(void)
{
	NTL_FUNCTION("CDboGlobal::Create");

	NTL_DELETE(m_pDBOUIConfig);
	
	if(m_pNetwork)
	{
		NTL_DELETE(m_pNetwork);
	}
	
	// network destroy
	UnRegisterPacketHandler();	
	CNtlNetSender::UnLinkNotifySendPacket();

	if(m_pLoginNetSender)
	{
		NTL_DELETE(m_pLoginNetSender);
	}

	if(m_pLobbyNetSender)
	{
		NTL_DELETE(m_pLobbyNetSender);
	}

	if(m_pGameNetSender)
	{
		NTL_DELETE(m_pGameNetSender);
	}

	if(m_pChatNetSender)
	{
		NTL_DELETE(m_pChatNetSender);
	}

	// packet handler destroy

	if(m_pLoginPacketGenerator)
	{
		NTL_DELETE(m_pLoginPacketGenerator);
	}

	if(m_pLobbyPacketGenerator)
	{
		NTL_DELETE(m_pLobbyPacketGenerator);
	}

	if(m_pGamePacketGenerator)
	{
		NTL_DELETE(m_pGamePacketGenerator);
	}

	if(m_pChatPacketGenerator)
	{
		NTL_DELETE(m_pChatPacketGenerator);
	}

	NTL_RETURNVOID();
}

void CDboGlobal::Reset(void)
{
	m_sGame.Init();
}


RwBool CDboGlobal::LoadConfigOptions(const RwChar *pFileName)
{
	NTL_FUNCTION("CDboGlobal::LoadConfigOptions");

	CNtlXMLDoc doc;
	doc.Create();

	// xml doc load
	if(doc.Load( (char*)pFileName ) == FALSE)
	{
		NTL_RETURN(FALSE);
	}

    char chBuffer[1024] = {0,};
    
	// config operation을 얻어온다.
	IXMLDOMNode* pNode = doc.SelectSingleNode((char*)"/config_options/op");
	
	if(!doc.GetTextWithAttributeName(pNode, "ver", chBuffer, 1024))
	{
		NTL_RETURN(FALSE);
	}

	if(!doc.GetTextWithAttributeName(pNode, "ip", chBuffer, 1024))
	{
		NTL_RETURN(FALSE);
	}

    m_sConfig.strAddr.reserve(256);
	m_sConfig.strAddr = chBuffer;

	if(!doc.GetTextWithAttributeName(pNode, "port", chBuffer, 1024))
	{
		NTL_RETURN(FALSE);
	}

	m_sConfig.dwPort = (DWORD)atoi(chBuffer);

    pNode->Release();

    // Bug Trap
    pNode = doc.SelectSingleNode("/config_options/BUGTRAP");
    if(doc.GetTextWithAttributeName(pNode, "ip", chBuffer, 1024))
    {
        m_sConfig.strBugTrapServerIP.reserve(256);        
        m_sConfig.strBugTrapServerIP = chBuffer;
    }

    if(doc.GetTextWithAttributeName(pNode, "port", chBuffer, 1024))
    {
        m_sConfig.dwBugTrapServerPort = atoi(chBuffer);        
    }

    if(doc.GetTextWithAttributeName(pNode, "folder", chBuffer, 1024))
    {
        m_sConfig.strBugTrapFolder.reserve(256);
        m_sConfig.strBugTrapFolder = chBuffer;
    }

	pNode->Release();

	// localize

	pNode = doc.SelectSingleNode((char*)"/config_options/localize");

	if (!doc.GetTextWithAttributeName(pNode, "folder", chBuffer, 1024))
	{
		m_sConfig.strLocalize = "english"; // default english
	}

	m_sConfig.strLocalize = chBuffer;

	pNode->Release();
	
	NTL_RETURN(TRUE);
}


RwBool CDboGlobal::HaveFunction(eDBO_ALLOWED_FUNC_FOR_DEV eAllowFunc)
{
	return Logic_IsExistFlag(m_sConnect.m_acAllowFuncForDev, eAllowFunc, DBO_ALLOWED_FUNC_FOR_DEV_COUNT);
}

void CDboGlobal::LoadingContractInfo()
{
	if( m_pContractInfo )
		return;

	RwInt32 iSize = 0;

	m_pContractInfo = NTL_NEW sContractInfo;
	ZeroMemory(m_pContractInfo, sizeof(m_pContractInfo));

	// The file must be saved as ANSI
	if( g_fnCallback_LoadContract )
	{
		RwChar* pcData = NULL;

		(*g_fnCallback_LoadContract)(dCONTRACT_FILE, (void**)&pcData, &iSize);

		if( !pcData )
		{
			DBO_FAIL("Can not contract file of name : " << dCONTRACT_FILE);
			return;
		}

		m_pContractInfo->pcContract = NTL_NEW RwChar[iSize + 1];
		memcpy(m_pContractInfo->pcContract, pcData, sizeof(RwChar)*iSize);
		m_pContractInfo->pcContract[iSize] = '\0';

		NTL_ARRAY_DELETE(pcData);
	}
	else
	{
		FILE* fp;
		fopen_s(&fp, dCONTRACT_FILE, "rb");
		if(!fp)
		{
			DBO_FAIL("Can not contract file of name : " << dCONTRACT_FILE);
			return;
		}


		fseek(fp, 0, SEEK_END);
		iSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);	

		m_pContractInfo->pcContract = NTL_NEW char[iSize + 1];
		ZeroMemory(m_pContractInfo->pcContract, iSize + 1);
		fread(m_pContractInfo->pcContract, iSize, 1, fp );

		fclose(fp);
	}

	m_pContractInfo->pwcContract = NTL_NEW wchar_t[iSize + 1];
	ZeroMemory(m_pContractInfo->pwcContract, sizeof(wchar_t) * (iSize + 1));
	::MultiByteToWideChar(GetACP(), 0, m_pContractInfo->pcContract, -1, m_pContractInfo->pwcContract, iSize);

	// The version information is a 4-digit number
	WCHAR awcVersionBuffer[4];
	ZeroMemory(awcVersionBuffer, sizeof(awcVersionBuffer));
	memcpy(awcVersionBuffer, m_pContractInfo->pwcContract, 4 * sizeof(wchar_t));

	m_pContractInfo->uiContractVersion	= _wtoi(awcVersionBuffer);
	m_pContractInfo->pwcRealContract	= m_pContractInfo->pwcContract + 4;
}

void CDboGlobal::SetGameContract(RwBool bAccept)
{
	if(m_pContractInfo)
		m_pContractInfo->bAcceptGameContract = bAccept;
}

RwBool CDboGlobal::IsAcceptGameContract()
{
	if( !m_pContractInfo )
		return FALSE;

	return m_pContractInfo->bAcceptGameContract;
}

RwUInt32 CDboGlobal::GetContractVerstion()
{
	if( !m_pContractInfo )
		return 0;

	return m_pContractInfo->uiContractVersion;
}

WCHAR* CDboGlobal::GetContract()
{
	if( !m_pContractInfo )
		return NULL;

	return m_pContractInfo->pwcRealContract;
}