#ifndef __TBAPP_H__
#define __TBAPP_H__

#pragma once

#include "stdafx.h"

#define MAX_NB_FILES_PER_FS (20)   
#define UNC_PATH_SIZE       (256)   /* should be sufficient */


class CTBApp
{
public:
	CTBApp(void);
	virtual ~CTBApp(void);

	RwBool		Create( HWND hWnd, RwInt32 nWidth, RwInt32 nHeight, RwBool bZBuffer = TRUE );
	void		Destroy();

	LRESULT		PreTranslateMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
	
	RwBool		OnIdle();
	RwBool		Resize( RwInt32 nWidth, RwInt32 nHeight, RwBool bZBuffer );
	RwBool		Update( RwReal fTime, RwReal fElapsedTime );

	RwBool		LoadTexFromPath( CBindingTexManager::eType type, CBindingTexManager::eSize size,
		const CString strPath);
	RwBool		SaveTexCurrentData( CBindingTexManager::eSize size, const CString strPath );

	// ΩÃ±€≈Ê
	static CTBApp* GetInstance()
	{
		static CTBApp instance;
		return &instance;
	}
	
protected:
	RwBool		AttachPlugin();
	void		DisplayInfo();

	RwCamera*	CameraCreate( RwInt32 nWidth, RwInt32 nHeight, RwBool bZBuffer );
	VOID		CameraDestroy( RwCamera* pRwCamera );
	static RpLight*	DestroyLight( RpLight* light, void* data );


protected:
	RpWorld*	m_pRpWorld;
	RpLight*	m_pRpLight;
	RwCamera*	m_pRwCamera;
	RtCharset*	m_pRtCharset;

	RwRGBA		m_BackGroundColor;
	RwRGBA		m_ForeGroundColor;

	RwReal		m_fTime;
	RwReal		m_fElapsedTime;
	RwReal		m_fFps;

	RwInt32		m_nWidth;
	RwInt32		m_nHeight;

	HWND		m_hWnd;

	RwBool		m_bRMouseDown;
	RwInt32		m_nOldX;
	RwInt32		m_nOldY;

	CBindingTexManager	m_pBindingTexManager;
};

static CTBApp* GetTBApp()
{
	return CTBApp::GetInstance();
}

#endif