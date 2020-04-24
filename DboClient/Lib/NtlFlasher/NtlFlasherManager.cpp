#include "precomp_ntlflasher.h"
#include "NtlFlasherManager.h"
#include "NtlFlasher.h"

#include "GFxLog.h"


CallFlashResourcePack g_fnCallFlashPack = NULL;

void LinkFlashResourceLoad(CallFlashResourcePack fn)
{
	g_fnCallFlashPack = fn;
}

void UnLinkFlashResourceLoad(void)
{
	g_fnCallFlashPack = NULL;
}

GFile* FlasherFileOpener::OpenFile(const char* pFilename, int flags /*= GFileConstants::Open_Read | GFileConstants::Open_Buffered*/,
	int mode /*= GFileConstants::Mode_ReadWrite*/)
{
	if (g_fnCallFlashPack)
	{
		BYTE* pData = NULL;
		INT nSize = 0;

		(*g_fnCallFlashPack)(pFilename, (void**)&pData, &nSize);

		return NTL_NEW FlasherMemoryFile(pFilename, pData, nSize);
	}
	else
	{
		DBO_WARNING_MESSAGE("FlasherFileOpener::OpenFile failed. FileName: " << pFilename);

		return NULL;
	}
	//return NTL_NEW GSysFile( pFilename ); 
}


VOID FlasherCommandHandler::Callback(GFxMovieView* pMovie, const char* command, const char* args)
{
	if (pMovie)
	{
		const int nID = reinterpret_cast<const int>(pMovie->GetUserData());

		std::map<int, CNtlCallbackVPParam2*>::iterator it = m_callbackMap.find(nID);
		if (it != m_callbackMap.end())
		{
			it->second->Call((VOID*)command, (VOID*)args);
		}
	}
}


CNtlFlasherManager::CNtlFlasherManager(VOID)
{
	memset( &m_d3dpp, 0, sizeof( D3DPRESENT_PARAMETERS ) );
	m_pStateBlock = NULL;
	m_hWnd = 0;

	m_nScreenWidth = 0;
	m_nScreenHeight = 0;

	m_nUniqueID = 1;
}

CNtlFlasherManager::~CNtlFlasherManager(VOID)
{
}

BOOL CNtlFlasherManager::CreateInstance( HWND hWnd )
{
	m_gfxSystem = NTL_NEW GFxSystem(&m_sysAlloc);

	m_gfxLoader = NTL_NEW GFxLoader;

	m_gfxLoader->SetLog(GPtr<GFxLog>(*new GFxLog()));

	m_pFileOpener = *NTL_NEW FlasherFileOpener;
	m_gfxLoader->SetFileOpener(m_pFileOpener);

	// FS Callback
	m_pCommandHandler = *NTL_NEW FlasherCommandHandler;
	m_gfxLoader->SetFSCommandHandler(m_pCommandHandler);

	if( !( m_pRenderer = *GRendererD3D9::CreateRenderer() ) )
		return FALSE;

	m_hWnd = hWnd;

	m_pFontLib = *NTL_NEW GFxFontLib;
	m_pFontMap = *NTL_NEW GFxFontMap;
	
	SetVideoMode();

	m_hDeviceRestore = CNtlDeviceRestoreHandler::GetInstance()->LinkDeviceRestore( this, &CNtlFlasherManager::DeviceRestoreCallback );
	m_hDeviceRelease = CNtlDeviceRestoreHandler::GetInstance()->LinkDeviceRelease( this, &CNtlFlasherManager::DeviceReleaseCallback );

	return TRUE;
}

VOID CNtlFlasherManager::DeleteInstance(VOID)
{
	m_pFontLib = 0;
	m_pFontMap = 0;

	CNtlDeviceRestoreHandler::GetInstance()->UnLinkDeviceRestore( m_hDeviceRestore );
	CNtlDeviceRestoreHandler::GetInstance()->UnLinkDeviceRelease( m_hDeviceRelease );

	m_pRenderer = NULL;

	m_pCommandHandler = NULL;
}

VOID CNtlFlasherManager::ResetRenderer(VOID)
{
	if( m_pRenderer && m_bRendererEnable )
	{
		m_pRenderer->ResetVideoMode();	
		
		if( m_pStateBlock )
		{
			m_pStateBlock->Release();
			m_pStateBlock = NULL;
		}

		m_bRendererEnable = FALSE;
	}
}

VOID CNtlFlasherManager::SetVideoMode(VOID)
{
	RwVideoMode videomodeinfo;
	RwEngineGetVideoModeInfo( &videomodeinfo, RwEngineGetCurrentVideoMode() );

	m_d3dpp.Windowed			= ( videomodeinfo.flags & rwVIDEOMODEEXCLUSIVE ) ? FALSE : TRUE;
	m_d3dpp.SwapEffect			= D3DSWAPEFFECT_DISCARD;
	m_d3dpp.BackBufferFormat	= D3DFMT_R8G8B8;
	m_d3dpp.BackBufferWidth		= videomodeinfo.width;
	m_d3dpp.BackBufferHeight	= videomodeinfo.height;

	LPDIRECT3DDEVICE9 pDevice = (LPDIRECT3DDEVICE9)RwD3D9GetCurrentD3DDevice();

	m_pRenderer->SetDependentVideoMode( pDevice, &m_d3dpp, 0, m_hWnd );
	pDevice->CreateStateBlock( D3DSBT_ALL, &m_pStateBlock );

	m_bRendererEnable = TRUE;
}

INT CNtlFlasherManager::DeviceRestoreCallback(VOID)
{
	SetVideoMode();	

	//while( TRUE )
	//{
	//	GRendererD3D9::DisplayStatus eStatus = m_pRenderer->CheckDisplayStatus();

	//	if( eStatus != GRendererD3D9::DisplayStatus_Ok )
	//	{
	//		::Sleep( 10 );			
	//	}
	//	else
	//	{
	//		break;
	//	}
	//}	
	
	return 1;
}

INT CNtlFlasherManager::DeviceReleaseCallback(VOID)
{
	ResetRenderer();
	return 1;
}

int CNtlFlasherManager::GenerateUniqueID()
{
	m_nUniqueID += 1;

	if (m_nUniqueID > 100000000)
		m_nUniqueID = 1;

	return m_nUniqueID;
}

VOID CNtlFlasherManager::SetFontLibData( std::string& strLibFileName )
{
	GPtr<GFxMovieDef> movieDef = *m_gfxLoader->CreateMovie( strLibFileName.c_str() );
	if (!movieDef)
	{
//		DBO_WARNING_MESSAGE("CNtlFlasherManager::SetFontLibData fail FileName: " << strLibFileName.c_str());
	}
//	else
//		DBO_WARNING_MESSAGE("CNtlFlasherManager::SetFontLibData success FileName: " << strLibFileName.c_str());

	m_pFontLib->AddFontsFrom( movieDef );
}

VOID CNtlFlasherManager::SetFontMapData( std::string& strMapName, std::string& strFontname, std::string& strStyle )
{
	GFxFontMap::MapFontFlags FontFlags = GFxFontMap::MFF_Normal;

	if( strStyle == "bold" )
		FontFlags = GFxFontMap::MFF_Bold;
	else if( strStyle == "italic" )
		FontFlags = GFxFontMap::MFF_Italic;
	else if( strStyle == "bolditalic" )
		FontFlags = GFxFontMap::MFF_BoldItalic;
	else if (strStyle == "Original")
		FontFlags = GFxFontMap::MFF_FauxItalic;

	//m_pFontMap->MapFont( L"$NormalFont", L"¼Ò¸ÁM", FontFlags );	

	INT nLen = (INT)strlen( strMapName.c_str() );
	WCHAR pBuffer[1024];
	::MultiByteToWideChar(GetACP(), 0, strMapName.c_str(), -1, pBuffer, nLen+1);
	std::wstring wstrMapName = pBuffer;

	nLen = (INT)strlen( strFontname.c_str() );
	::MultiByteToWideChar(GetACP(), 0, strFontname.c_str(), -1, pBuffer, nLen+1);
	std::wstring wstrFontName = pBuffer;

	m_pFontMap->MapFont( wstrMapName.c_str(), wstrFontName.c_str(), FontFlags );	
}

CNtlFlasherManager* CNtlFlasherManager::GetInstance(VOID)
{
	static CNtlFlasherManager intance;
	return &intance;
}
