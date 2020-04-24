#ifndef _2DPARTICLE_APP_H_
#define _2DPARTICLE_APP_H_

// renderware
#include "rtcharse.h"

// framework
#include "NtlApplication.h"
#include "NtlCamera.h"

// gui
#include "gui_precomp.h"
#include "guicore_precomp.h"
#include "gui_particle_doc.h"
#include "gui_line_renderer.h"

// APP
#include "PathPoint.h"
#include "PathManager.h"

extern	gui::CGUIManager*			g_pGuiMgr;			///< Gui Manager

class C2DParticleApplication : public CNtlApplication
{
public:
	C2DParticleApplication(void);
	~C2DParticleApplication(void);

	virtual void Destroy();
	LRESULT	PreTranslateMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	void OnIdle(void);
	void OnResize(RwUInt32 uiWidth, RwUInt32 uiHeight, RwBool zBuffer);

	CNtlCamera *GetCamera() { NTL_ASSERT( m_pCamera != NULL, "m_pCamera == NULL"); return m_pCamera; }

	void Load( const char* groupname );
	void Play( BOOL bRepeat );
	
	// ParticleBox Offset
	void SetParticleBoxOffsetXY( int nOffsetX, int nOffsetY );
	void SetParticleBoxMoveXY( int nMovedX, int nMovedY );

	int	 GetPBOffsetX();
	int	 GetPBOffsetY();
	
	static C2DParticleApplication* m_pInstance;
	static C2DParticleApplication* GetInstance()	{ return m_pInstance; }

protected:

	RwBool	CreateSubSystem(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer);
	
	RwBool	Update(RwReal fTime, RwReal fElapsedTime);

	void	PreRender();
	void	AfterRender();

	RwBool	Resize(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer, RwBool bRestore_from_Minimize);

	RwBool	AttachPlugin();

	void	DisplayInfo(void);

protected:
	RtCharset*					m_pCharset;			///< Renderware chracter set
	RwRGBA						m_BackgroundColor;	///< Background color(배경색)
	RwRGBA						m_ForegroundColor;	///< Foreground color(전경색)

	CNtlCamera*					m_pCamera;			///< Main Camera

	gui::CResourceManager*		m_pResMgr;			///< Resource Manager
	gui::CSurfaceManager*		m_pSurMgr;			///< Surface Manager

	gui::CParticleBox*			m_pPbComp;
	
	RwBool						m_bRMouseDown;
	RwInt32						m_nOldX;
	RwInt32						m_nOldY;

	RwBlendFunction				m_eSrcBlend;
	RwBlendFunction				m_eDestBlend;
};

static C2DParticleApplication* GetParticleApp()
{
	return C2DParticleApplication::GetInstance();
}

#endif