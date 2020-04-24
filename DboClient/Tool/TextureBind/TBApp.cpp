#include "stdafx.h"
#include "TBApp.h"

//////////////////////////////////////////////////////////////////////////

RwBool
psInstallFileSystem(void)
{
	RwChar      curDirBuffer[_MAX_PATH];
	RwUInt32    retValue;
	RtFileSystem *wfs, *unc;

	RwUInt32 drivesMask;
	RwInt32 drive;
	RwChar  fsName[2];

	/* get the current directory */
	retValue = GetCurrentDirectory(_MAX_PATH, curDirBuffer);
	if (0 == retValue || retValue > _MAX_PATH)
	{
		return FALSE;
	}

	/* Register a unc file system */

	/* tag a \ to the end of the current directory */
	/* only fails if the buffer size is exhausted */
	rwstrcat( curDirBuffer, "\\" );

	/** Setup the file system manager */
	if ((unc = RtWinFSystemInit(MAX_NB_FILES_PER_FS, curDirBuffer, "unc")) != NULL)
	{
		/* Now actually register the file system */
		if (RtFSManagerRegister(unc) == FALSE)
		{
			return (FALSE);
		}
	}
	else
	{
		return (FALSE);
	}

	/* Now register local file systems */

	CharUpper(curDirBuffer);

	/* 
	* loop through all logical drives and register a file system for each
	* valid one.
	* Start at 2: this removes a:
	*/
	drivesMask = GetLogicalDrives();

	for( drive = 2, drivesMask >>= 1; drivesMask != 0; drive++, drivesMask >>= 1)
	{
		if (drivesMask & 0x01)
		{
			RwInt32 ret;
			RwChar  deviceName[4];

			deviceName[0] = drive + 'A' - 1;
			deviceName[1] = ':';
			deviceName[2] = '\\';   /* Needed for Win98 */
			deviceName[3] = '\0';

			ret = GetDriveType(deviceName);

			if ( ret != DRIVE_UNKNOWN && ret != DRIVE_NO_ROOT_DIR )
			{
				/* Fix device name */
				deviceName[2] = '\0';

				fsName[0] = deviceName[0];
				fsName[1] = '\0';

				/** Setup the file system manager */
				if ((wfs = RtWinFSystemInit(MAX_NB_FILES_PER_FS, deviceName, fsName)) != NULL)
				{
					/* Now actually register the file system */
					if (RtFSManagerRegister(wfs) == FALSE)
					{
						return (FALSE);
					}
					else
					{
						/* Set the unc file system as default if we have a unc path */
						if (curDirBuffer[1] != ':')
						{
							RtFSManagerSetDefaultFileSystem(unc);
						}
						else if (deviceName[0] == curDirBuffer[0])
						{
							RtFSManagerSetDefaultFileSystem(wfs);
						}
					}
				}
				else
				{
					return (FALSE);
				}
			}
		}
	}

	return (TRUE);
}

void
psDebugMessageHandler(RwDebugType type __RWUNUSED__, const RwChar *str)
{
	OutputDebugString(str);
	OutputDebugString(RWSTRING("\n"));

	return;
}

//////////////////////////////////////////////////////////////////////////

CTBApp::CTBApp(void)
{
	m_pRpWorld = NULL;
	m_pRpLight = NULL;
	m_pRwCamera = NULL;
	m_pRtCharset = NULL;

	m_BackGroundColor.red	= 98;
	m_BackGroundColor.green = 94;
	m_BackGroundColor.blue	= 89;
	m_BackGroundColor.alpha = 255;

	m_ForeGroundColor.red	= 0;
	m_ForeGroundColor.green	= 0;
	m_ForeGroundColor.blue	= 0;
	m_ForeGroundColor.alpha	= 255;

	m_bRMouseDown = FALSE;
	m_nOldX = 0;
	m_nOldY = 0;
}

CTBApp::~CTBApp( void )
{

}

/**
* \brief Application 의 Create 및 RenderWare 엔진을 초기화한다.
* \param hWnd		장치가 초기화 될 창의 핸들
* \param nWidth		넓이
* \param nHeight	높이
* \param bZBuffer	Z 버퍼 유무
*/
RwBool CTBApp::Create( HWND hWnd, RwInt32 nWidth, RwInt32 nHeight, RwBool bZBuffer /* = TRUE */ )
{
	// 변수 저장
	m_hWnd = hWnd;
	m_nWidth = nWidth;
	m_nHeight = nHeight;

	// Renderware Engine init
	if( !RwEngineInit( NULL, rwENGINEINITNOFREELISTS, rsRESOURCESDEFAULTARENASIZE))
	{
		return FALSE;
	}

	RwDebugSetHandler(psDebugMessageHandler);
	RwDebugSendMessage(rwDEBUGMESSAGE, "Texture Binding",
		RWSTRING("Debugging Initialized"));

	// Attach Plugin
	AttachPlugin();

	RwEngineOpenParams openParams;
	openParams.displayID = hWnd;

	// Engine Open
	if( !RwEngineOpen(&openParams) )
	{
		RwEngineTerm();
		return FALSE;
	}

	// Engine Start
	if( !RwEngineStart() )
	{
		return FALSE;
	}

	if (!RwImageRegisterImageFormat(RWSTRING("bmp"), RtBMPImageRead, RtBMPImageWrite))
	{
		return FALSE;
	}

	if (!RwImageRegisterImageFormat(RWSTRING("png"), RtPNGImageRead, RtPNGImageWrite))
	{
		return FALSE;
	}

	RwRGBA colorTextForeGround =    {255, 255, 255, 255};
	RwRGBA colorTextBackGround =    { 64,  64,  64,   0};

	// Char set
	m_pRtCharset = RtCharsetCreate( &colorTextForeGround, &colorTextBackGround );
	if( !m_pRtCharset )
		return FALSE;

	CTBTimer::StartTimer();

	RwBBox BBox;
	BBox.inf.x = BBox.inf.y = BBox.inf.z = -1000.f;
	BBox.sup.x = BBox.sup.y = BBox.sup.z = 1000.f;

	// World & Camera & Light Create
	if (m_pRpWorld = RpWorldCreate(&BBox))
	{
		if (m_pRwCamera	= CameraCreate( nWidth, nHeight, TRUE ) )
		{
			RwCameraSetProjection(m_pRwCamera, rwPERSPECTIVE);

			RwCameraSetFarClipPlane(m_pRwCamera, 100000.f);
			RwCameraSetNearClipPlane(m_pRwCamera, 0.001f);
			RwCameraSetFogDistance(m_pRwCamera, 100000.f);

			RwReal fViewWindow = RwTan(TB_DEG2RAD(60.f * 0.5f));
			RwV2d vw;
			vw.x = fViewWindow;
			vw.y = ((float)nHeight * fViewWindow) / (float)nWidth;

			RwCameraSetViewWindow(m_pRwCamera, &vw);

			RwD3D9CameraAttachWindow(m_pRwCamera, m_hWnd);

			RpWorldAddCamera(m_pRpWorld, m_pRwCamera);
		}

		m_pRpLight = RpLightCreate(rpLIGHTAMBIENT);
		RpWorldAddLight(m_pRpWorld, m_pRpLight);

		m_pRpLight = RpLightCreate(rpLIGHTDIRECTIONAL);
		if( m_pRpLight )
		{
			RwFrame *frame;

			frame = RwFrameCreate();
			if( frame )
			{
				RpLightSetFrame(m_pRpLight, frame);

				RpWorldAddLight(m_pRpWorld, m_pRpLight);
			}
		}
	}

	if (RtFSManagerOpen(RTFSMAN_UNLIMITED_NUM_FS) != FALSE)
	{
		if (!psInstallFileSystem())
		{
			return (FALSE);
		}
	}
	else
	{
		return (FALSE);
	}

	m_pBindingTexManager.Create();

	return TRUE;
}

void CTBApp::Destroy()
{
	RtFSManagerClose();

	m_pBindingTexManager.Destroy();

	RpWorldForAllLights(m_pRpWorld, &CTBApp::DestroyLight, m_pRpWorld);

	if( m_pRwCamera && m_pRpWorld )
	{
		RpWorldRemoveCamera(m_pRpWorld, m_pRwCamera);

		CameraDestroy( m_pRwCamera );
	}

	// World Destroy
	if( m_pRpWorld )
	{
		RpWorldDestroy(m_pRpWorld);
	}

	// Character set Destroy
	if(m_pRtCharset)
	{
		RtCharsetClose();
		RwRasterDestroy(m_pRtCharset);        
		m_pRtCharset = NULL;        
	}

	RwEngineStop();
	RwEngineClose();
	RwEngineTerm();
}

LRESULT CTBApp::PreTranslateMessage( HWND window, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
	case WM_MOUSEWHEEL:
		{
			RwReal zDelta = GET_WHEEL_DELTA_WPARAM(wParam) * 0.5f;

			RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->z += zDelta;
			RwMatrixUpdate(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)));
			RwFrameUpdateObjects(RwCameraGetFrame(m_pRwCamera));
		}
		break;
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
			if( !m_bRMouseDown )
				break;

			INT nX = LOWORD(lParam);
			INT nY = HIWORD(lParam);

			// 새 좌표 - 이전 좌표
			INT nMovedX = nX - m_nOldX;
			INT nMovedY = nY - m_nOldY;

			// 좌표 갱신
			m_nOldX = nX;
			m_nOldY = nY;

			RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->x += nMovedX;
			RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->y += nMovedY;
			RwMatrixUpdate(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)));
			RwFrameUpdateObjects(RwCameraGetFrame(m_pRwCamera));
		}
		break;
	default:
		break;
	}

	return TRUE;
}

RwBool CTBApp::OnIdle()
{
	CTBTimer::UpdateTimer();

	m_fTime			= CTBTimer::GetAppTime();
	m_fElapsedTime	= CTBTimer::GetElapsedTime();
	m_fFps			= CTBTimer::GetFps();

	if( Update( m_fTime, m_fElapsedTime ) )
		return FALSE;

	return TRUE;
}

RwBool CTBApp::Resize( RwInt32 nWidth, RwInt32 nHeight, RwBool bZBuffer )
{
	return TRUE;
}

RwBool CTBApp::Update( RwReal fTime, RwReal fElapsedTime )
{
	if (RwCameraClear(m_pRwCamera, &m_BackGroundColor, rwCAMERACLEARZ | rwCAMERACLEARIMAGE))
	{
		if(RwCameraBeginUpdate(m_pRwCamera))
		{	
			m_pBindingTexManager.RenderPerspective( m_pRwCamera );

			// 화면에 정보 표시        
			DisplayInfo();

			RwCameraEndUpdate(m_pRwCamera);
		}

		RwCameraShowRaster( m_pRwCamera, m_hWnd, 0 );
	}

	return TRUE;
}

RwBool CTBApp::LoadTexFromPath( CBindingTexManager::eType type, CBindingTexManager::eSize size, const CString strPath )
{
	// Path 지정
	RwBool bLoad = m_pBindingTexManager.LoadTexFromPath( type, size, strPath );

	if( bLoad )
	{
		RwMatrixGetRight(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->x = 1.0f;
		RwMatrixGetRight(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->y = 0.0f;
		RwMatrixGetRight(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->z = 0.0f;
		RwMatrixGetUp(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->x = 0.0f;
		RwMatrixGetUp(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->y = 1.0f;
		RwMatrixGetUp(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->z = 0.0f;
		RwMatrixGetAt(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->x = 0.0f;
		RwMatrixGetAt(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->y = 0.0f;
		RwMatrixGetAt(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->z = 1.0f;
		RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->x = (RwReal)m_pBindingTexManager.GetWidth() / 2.f;
		RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->y = (RwReal)m_pBindingTexManager.GetHeight() / 2.f;
		RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)))->z = -1024.0f;
		RwMatrixUpdate(RwFrameGetMatrix(RwCameraGetFrame(m_pRwCamera)));
		RwFrameUpdateObjects(RwCameraGetFrame(m_pRwCamera));
	}

	return bLoad;
}

RwBool CTBApp::SaveTexCurrentData( CBindingTexManager::eSize size, const CString strPath )
{
	RwBool bSave = m_pBindingTexManager.SaveTexCurrentData( size, m_pRwCamera, strPath );

	return bSave;
}

RwBool CTBApp::AttachPlugin()
{
	if( !RpWorldPluginAttach() )
		return FALSE;

	return TRUE;
}

void CTBApp::DisplayInfo()
{
	RwChar caption[256] = {0,};

	sprintf_s( caption, 256, "FPS : %.2f", CTBTimer::GetFps());
	RsCharsetPrint(m_pRtCharset, caption, 0, 0, rsPRINTPOSTOPRIGHT);

	RwV3d camPos;

	RwFrame* cameraFrame = RwCameraGetFrame(m_pRwCamera);
	RwMatrix* cameraMatrix = RwFrameGetMatrix(cameraFrame);

	camPos = *RwMatrixGetPos(cameraMatrix);
	sprintf_s( caption, 256, "CAMERA POS : %.2f, %.2f, %.2f", camPos.x, camPos.y, camPos.z );

	RsCharsetPrint(m_pRtCharset, caption, 0, 0, rsPRINTPOSBOTTOMLEFT );
}

RwCamera* CTBApp::CameraCreate( RwInt32 nWidth, RwInt32 nHeight, RwBool bZBuffer )
{
	RwCamera *camera;

	camera = RwCameraCreate();
	if( camera )
	{
		RwCameraSetFrame(camera, RwFrameCreate());

		RwCameraSetRaster(camera, 
			RwRasterCreate(nWidth, nHeight, 0, rwRASTERTYPECAMERA));

		if( bZBuffer )
		{
			RwCameraSetZRaster(camera,
				RwRasterCreate(nWidth, nHeight, 0, rwRASTERTYPEZBUFFER));
		}

		/*
		* Check everything is valid...
		*/
		if( RwCameraGetFrame(camera) && RwCameraGetRaster(camera) &&
			(!bZBuffer || RwCameraGetZRaster(camera)) )
		{
			/*
			* Everything OK...
			*/
			return camera;
		}
	}

	/*
	* Error - so clean up...
	*/
	CameraDestroy(camera);

	return NULL;
}

VOID CTBApp::CameraDestroy( RwCamera* pRwCamera )
{
	if( pRwCamera )
	{
		RwRaster *raster;
		RwFrame *frame;

		frame = RwCameraGetFrame(pRwCamera);
		if( frame )
		{
			RwCameraSetFrame(pRwCamera, NULL);

			RwFrameDestroy(frame);
		}

		raster = RwCameraGetRaster(pRwCamera);
		if( raster )
		{
			RwRasterDestroy(raster);

			RwCameraSetRaster(pRwCamera, NULL);
		}

		raster = RwCameraGetZRaster(pRwCamera);
		if( raster )
		{
			RwRasterDestroy(raster);

			RwCameraSetZRaster(pRwCamera, NULL);
		}

		RwCameraDestroy(pRwCamera);
	}

	return;
}

RpLight* CTBApp::DestroyLight( RpLight* light, void* data )
{
	RwFrame *frame;

	RpWorldRemoveLight((RpWorld *)data, light);

	frame = RpLightGetFrame(light);
	if( frame )
	{
		RpLightSetFrame(light, NULL);

		RwFrameDestroy(frame);
	}

	RpLightDestroy(light);

	return light;

}



