// Precompile
#include "stdafx.h"
#include "2DParticleApp.h"

// Core
#include "NtlDebug.h"
#include "NtlCharsetPrint.h"
#include "NtlText.h"
#include "NtlCoreUtil.h"

// Gui
#include "gui_surface.h"
#include "gui_renderer.h"

// Editor
#include "2DParticleGlobal.h"
#include "2DParticleEditorDoc.h"
#include "2DParticleEditorView.h"
#include "MainFrm.h"
#include "InfomationPane.h"

// Application
#include "GuideManager.h"
#include "BackManager.h"

gui::CGUIManager*			g_pGuiMgr;			///< Gui Manager


C2DParticleApplication* C2DParticleApplication::m_pInstance = NULL;

/**
* \brief NTL Application을 상속받아 Particle Editor의 알맞은 Application을 구성한다.
*/
C2DParticleApplication::C2DParticleApplication() 
: m_pCharset(NULL)
{
	m_pInstance = this;

	// 배경 색깔
	m_BackgroundColor.red   = 90;
	m_BackgroundColor.green = 90;
	m_BackgroundColor.blue  = 90;
	m_BackgroundColor.alpha = 0;

	m_ForegroundColor.red   = 255;
	m_ForegroundColor.green = 255;
	m_ForegroundColor.blue  = 255;
	m_ForegroundColor.alpha = 255;
	
	m_pCamera	= NULL;

	m_pResMgr	= NULL;
	m_pSurMgr	= NULL;
	g_pGuiMgr	= NULL;

	m_pPbComp	= NULL;

	m_bRMouseDown = FALSE;
}

C2DParticleApplication::~C2DParticleApplication()
{
	m_pInstance = NULL;
}

/**
* \brief 하위 시스템 생성
*/
RwBool C2DParticleApplication::CreateSubSystem(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer)
{
	NTL_FUNCTION("C2DParticleApplication::CreateSubSystem" );
	
	CNtlApplication::CreateSubSystem(iWidth, iHeight, zBuffer);

	// Camera
	m_pCamera = NTL_NEW CNtlCamera;
	if(!m_pCamera->Create(iWidth, iHeight, zBuffer, 1.f, 500.f))
	{
		NTL_RETURN(FALSE);
	}

	// Character set
	m_pCharset = RtCharsetCreate(&m_ForegroundColor, &m_BackgroundColor);
    if( m_pCharset == NULL )
	{
		NTL_RETURN(FALSE);
	}	

	m_pResMgr = NTL_NEW gui::CResourceManager();
	m_pSurMgr = NTL_NEW gui::CSurfaceManager( m_pResMgr );
	g_pGuiMgr = NTL_NEW gui::CGUIManager(CRectangle(0, 0, iWidth, iHeight));
	
	// Gui setup
	gui::CSetupGUI::Init(GetHWnd(), 
		m_iScreenWidth, m_iScreenHeight, 
		g_pGuiMgr,
		".\\gui\\Font\\GuiFont.def", 
		".\\script\\metatag.htm", 
		".\\gui\\Font\\FlashFont.def"
		);

	std::string strResFile	= "gui\\"; strResFile += PARTICLE_RESOURCE_FILE;
	std::string strSurFile	= "gui\\particle\\"; strSurFile += PARTICLE_SURFACE_FILE;
	m_pResMgr->AddPage( strResFile );
	m_pSurMgr->AddPage( strSurFile );

	// Particle Box 생성
	m_pPbComp = NTL_NEW gui::CParticleBox( CRectangle( 0, 0, 0, 0 ),
		g_pGuiMgr, m_pSurMgr );

	// 초기화 및 Infomation에 정보 날려주기
	GetInfomationPane()->SetResolution( m_iScreenWidth, m_iWindowHeight );
	GetInfomationPane()->UpdateData( FALSE );

	// Guide Line 중앙에 그리기
	GetEditorView()->SetViewCenterPbOffset();

	NTL_RETURN(TRUE);
}	

RwBool C2DParticleApplication::AttachPlugin()
{
	NTL_FUNCTION( "C2DParticleApplication::AttachPlugin" );

	NTL_RETURN(TRUE);
}

RwBool C2DParticleApplication::Resize(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer, RwBool bRestore_from_Minimize)
{
	NTL_FUNCTION("C2DParticleApplication::Resize" );

	CNtlApplication::Resize(iWidth, iHeight, zBuffer, bRestore_from_Minimize);

	// 초기화 및 Infomation에 정보 날려주기
	GetInfomationPane()->SetResolution( iWidth, iHeight );

	if(m_pCamera != NULL)
	{
		if (m_pCamera->GetCamera())
		{
			if (iWidth == 0 || iHeight == 0)
			{
			}
			else
			{
				RwRect rt;
				rt.x = 0; rt.y = 0; rt.w = iWidth; rt.h = iHeight;
				m_pCamera->SetCameraSize(&rt); 

				gui::CSetupGUI::Resize(iWidth, iHeight);
			}
			NTL_RETURN(TRUE);
		}
		else
		{
			NTL_RETURN(FALSE);
		}
	}

	NTL_RETURN(TRUE);
}

LRESULT C2DParticleApplication::PreTranslateMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	if( window != ((CMainFrame*)AfxGetMainWnd())->GetActiveView()->GetSafeHwnd() )
		return 0;

	if(g_pGuiMgr)
	{
		switch( message )
		{
		case WM_RBUTTONDOWN:
			{
				m_bRMouseDown = TRUE;
				m_nOldX = LOWORD(lParam);
				m_nOldY = HIWORD(lParam);
			}
			break;
		case WM_RBUTTONUP:
			{
				m_bRMouseDown = FALSE;
			}
			break;
		case WM_MOUSEMOVE:
			{
				INT nX = LOWORD(lParam);
				INT nY = HIWORD(lParam);

				// 새 좌표 - 이전 좌표
				INT nMovedX = nX - m_nOldX;
				INT nMovedY = nY - m_nOldY;

				// 좌표 갱신
				m_nOldX = nX;
				m_nOldY = nY;

				if( m_bRMouseDown )
				{
					SetParticleBoxMoveXY( nMovedX, nMovedY );
				}
				
				// 마우스 좌표 업데이트
				GetInfomationPane()->SetClientMouse( nX, nY );
				GetInfomationPane()->SetParticleMouse( nX - GetPBOffsetX(), nY - GetPBOffsetY() );
			}
			break;
		default:
			break;
		}

		return g_pGuiMgr->Win32EventProc(window, message, wParam, lParam);
	}
	else
		return 0;
}

void C2DParticleApplication::OnIdle(void)
{
	CNtlApplication::Update();
}

void C2DParticleApplication::OnResize(RwUInt32 uiWidth, RwUInt32 uiHeight, RwBool zBuffer)
{
	Resize(uiWidth, uiHeight, zBuffer, FALSE);
}

void C2DParticleApplication::Load( const char* groupname )
{
	if( m_pPbComp )
		m_pPbComp->Load( groupname );
}

void C2DParticleApplication::Play( BOOL bRepeat )
{
	if( m_pPbComp )
		m_pPbComp->Play( bRepeat );
}

RwBool C2DParticleApplication::Update(RwReal fTime, RwReal fElapsedTime)
{
	NTL_FUNCTION("C2DParticleApplication::Update" );

	CEventTimer::KeepAlive();

	// 파티클 박스 업데이트
	m_pPbComp->Update( fElapsedTime );

	// Render
	if (RwCameraClear(m_pCamera->GetCamera(), &m_BackgroundColor, rwCAMERACLEARZ|rwCAMERACLEARIMAGE))
	{
		//Ready to render scene.
		if( RwCameraBeginUpdate(m_pCamera->GetCamera()) )
		{
			// 렌더링 전에 호출해줘야 한다.
			PreRender();

			// Gui
			g_GuiRenderer.ResetRender();

			// 배경 렌더링
			GetBackManager()->Render();	

			// Guide Line
			GetGuideManager()->Render();
			GetPathManager()->Render();

			// Buffer안에 있는 것들을 렌더링 한다.
			g_GuiRenderer.Render();

			if( g_pGuiMgr )
				g_pGuiMgr->Show();

			// 렌더링 후에 호출
			AfterRender();

			DisplayInfo();

			RwCameraEndUpdate(RwCameraGetCurrentCamera());
		}
		RwCameraShowRaster(m_pCamera->GetCamera(), GetHWnd(), 0);
	}
	
	NTL_RETURN(TRUE);
}


void C2DParticleApplication::Destroy()
{
	NTL_FUNCTION("CDboApplication::Destroy" );

	// 패스 매니저 리소스 제거
	CPathManager::GetInstance()->Destroy();

	// 파티클 박스 해제
	NTL_DELETE(m_pPbComp);

	// Gui Managers destroy
	m_pResMgr->RemovePage( PARTICLE_RESOURCE_FILE );
	m_pSurMgr->RemovePage( PARTICLE_SURFACE_FILE );

	NTL_DELETE(m_pResMgr);
	NTL_DELETE(m_pSurMgr);
	NTL_DELETE(g_pGuiMgr);

	gui::CSetupGUI::Deinit();

	// Character set destroy
	if(m_pCharset)
    {
		RtCharsetClose();
		RwRasterDestroy(m_pCharset);
		m_pCharset = NULL;
    }

	// Camera destroy
	if(m_pCamera)
	{
		m_pCamera->Destroy();
		NTL_DELETE(m_pCamera);
		m_pCamera = NULL;
	}

	CEventListener_Win32::RemoveAll();

	// Application Destroy
	CNtlApplication::Destroy();
	
	NTL_RETURNVOID();
}

void C2DParticleApplication::DisplayInfo(void)
{
	RwChar caption[256];
	RwInt32 iIdx = 0;

	//fps
	RsSprintf(caption, RWSTRING("FPS: %4.2f"), GetFps());
	RsCharsetPrint(m_pCharset, caption, 0, iIdx++, rsPRINTPOSTOPRIGHT);
}

void C2DParticleApplication::SetParticleBoxOffsetXY( int nOffsetX, int nOffsetY )
{
	if( m_pPbComp )
	{
		m_pPbComp->SetPosition( nOffsetX, nOffsetY );

		// GuideLine과 PathManager의 Offset 또한 수정한다.
		GetGuideManager()->SetCenter( (float)nOffsetX, (float)nOffsetY );
		GetPathManager()->RefreshOffsetData();
	}
}

void C2DParticleApplication::SetParticleBoxMoveXY( int nMovedX, int nMovedY )
{
	if( m_pPbComp )
	{
		CRectangle rtScreen = m_pPbComp->GetScreenRect();
		RwInt32 nNewX = rtScreen.left + nMovedX;
		RwInt32 nNewY = rtScreen.top + nMovedY;
		m_pPbComp->SetPosition( nNewX, nNewY );
		
		GetGuideManager()->SetCenter( (float)nNewX, (float)nNewY );
		GetPathManager()->RefreshOffsetData();
	}
}

int C2DParticleApplication::GetPBOffsetX()
{
	CRectangle rect = m_pPbComp->GetPosition();

	return rect.left;
}

int C2DParticleApplication::GetPBOffsetY()
{
	CRectangle rect = m_pPbComp->GetPosition();

	return rect.top;
}

/**
* \brief Rendering 전에 호출
*/
void C2DParticleApplication::PreRender()
{
	RwD3D9SetTexture(NULL, 0);
	RwD3D9SetTexture(NULL, 1);

	RwD3D9SetTextureStageState (0, D3DTSS_COLOROP, D3DTOP_MODULATE); 
	RwD3D9SetTextureStageState (0, D3DTSS_COLORARG1, D3DTA_TEXTURE); 
	RwD3D9SetTextureStageState (0, D3DTSS_COLORARG2, D3DTA_DIFFUSE); 

	// stage 0 alpha : nada 
	RwD3D9SetTextureStageState (0, D3DTSS_ALPHAOP,  D3DTOP_MODULATE); 
	RwD3D9SetTextureStageState (0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE); 
	RwD3D9SetTextureStageState (0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE); 

	RwRenderStateSet(rwRENDERSTATESHADEMODE, (void *)rwSHADEMODEGOURAUD);
	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) FALSE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) FALSE);
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *) NULL);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *) TRUE);
	RwRenderStateGet(rwRENDERSTATESRCBLEND, (void *)&m_eSrcBlend);
	RwRenderStateGet(rwRENDERSTATEDESTBLEND, (void *)&m_eDestBlend);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)rwBLENDSRCALPHA);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)rwBLENDINVSRCALPHA);
}

/**
* \brief Rendering 후에 호출
*/
void C2DParticleApplication::AfterRender()
{
	RwD3D9SetTexture(NULL, 0);

	RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *) TRUE);
	RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *) TRUE);
	RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *) NULL);
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *) FALSE);
	RwRenderStateSet(rwRENDERSTATESRCBLEND, (void *)m_eSrcBlend);
	RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void *)m_eDestBlend);
}