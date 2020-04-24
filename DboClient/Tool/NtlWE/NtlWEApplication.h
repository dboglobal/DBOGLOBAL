#ifndef _NTLWEAPP_H_
#define _NTLWEAPP_H_

#include <rwcore.h>
#include <rpworld.h>

#include "NtlDeviceRestoreHandler.h"

#include "NtlWorldDI.h"
#include "NtlWorldPlugin.h"

#include "NtlApplication.h"

#include "NtlPLVisualManager.h"

#include "NtlWorldCommon.h"
#include "NtlWorldPropUtil.h"

#include "NtlCamera.h"
#include "NtlGameCamera.h"
#include "NtlCameraTop.h"
#include "NtlPostEffectCamera.h"

#include "NtlWEPropContainer.h"

#ifdef dNTL_WORLD_TOOL_MODE
#include "UpdateEvent.h"
#endif

#include <string>
using std::string;

enum eAPP_ENTITY
{
	eAO_FOG = 0,
	eAO_BLOCK_GUIDE_LINE,
	eAO_SECTOR_GUIDE_LINE,
	eAO_FIELD_GUIDE_LINE,
};

class CNtlPLTerrain;
class CNtlPLWorldEntity;
class CNtlPLSky;
class CNtlPLFog;
class CNtlPLEventHandler;

class CWorldEditorEventHander;


class CNtlWEApplication : public CNtlApplication
{
protected:
	CNtlGameCamera*			m_pCamera;
	CNtlCameraTop*			m_pCameraTop;
	CNtlGameCamera*			m_pCameraTex;

	CNtlPLWorldEntity*		m_pWorldEntity;

	RtCharset*				m_pCharset;

	CNtlWEPropContainer		m_PropContainter;
	CNtlPLEventHandler*		m_pPLEventHandler;

public:
	RwRGBA					m_BackgroundColor;
	RwRGBA					m_ForegroundColor;

	CNtlWorldPropToolData	m_WorldPropToolData;
protected:
	CString					m_LogFilePath;
	CString					m_TraFilePath;

private:
	DEVICE_RESTORE_HANDLE	m_handleRestore;

#ifdef dNTL_WORLD_TOOL_MODE

public:
	CNtlPLVisualManager*			m_pVisualManager;
	CWorldEditorEventHander*		m_pEventHandler;

#endif

protected:
	void				SetErrorReport();

protected:
	RwBool				CreateSubSystem(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer);
	void				DisplayInfo();
	RwBool				AttachPlugin();

private:
	void				UpdateMsg();
	void				UpdateCamPos();
	VOID				CreateDefaultPVSFile();

public:
	static	CNtlWEApplication *GetInstance()	{ return (CNtlWEApplication *)m_pInstance; }

public:
	CNtlWEApplication(void);
	~CNtlWEApplication(void);

	virtual void		Destroy();

	virtual LRESULT		WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	RwBool				Update(RwReal fTime, RwReal fElapsedTime);
	RwBool				OnIdle();
	RwBool				OnSize(RwUInt32 iType, RwUInt32 iWidth, RwUInt32 iHeight);

	RwBool				Resize(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer, RwBool bRestore_from_Minimize);

	CNtlGameCamera*		GetNtlGameCamera()	{ return m_pCamera; }
	CNtlCameraTop*		GetNtlTopCamera()	{ return m_pCameraTop; }
	
public:
	VOID				PickSC();
	RwBool				RestoreHandler();
	VOID				CreateWorldDefaultParam();
	bool				CreateWorld(string& FullPathFileName);
	bool				CreateWorldFromFolder(string& FullPathFileName);

	CNtlPLWorldEntity*	GetNtlPLWorldEntity() { return m_pWorldEntity; }
	CNtlWeController*	GetNtlCamController(RwInt32 IdxCamMode);

	void				Render();
	void				RenderText(RwV3d vPos, const RwChar* pPrintString, RwUInt8 r, RwUInt8 g, RwUInt8 b, RwBool bBackCulling = FALSE);
	void				RenderText(RwV3d vPos, const RwChar* pPrintString, RwUInt32 uiWidth, RwUInt32 uiHeight, RwUInt8 r, RwUInt8 g, RwUInt8 b, RwBool bBackCulling = FALSE);
	bool				IsWorking();

	void				Switch(eAPP_ENTITY OptionID);
	RwBool				IsWorking(eAPP_ENTITY OptionID);

	RwBool				CreateSky();
	void				UpdateSky( RwV3d * pPosAvatar );
	void				DestroySky();

	RwBool				CreateFog();
	void				UpdateFog( RwV3d * pPosAvatar );
	void				DestroyFog();

protected:
	bool				CreateWorldEx(string& FullPathFileName);
	bool				CreateWorldFromFolderEx(string& FullPathFileName);
	void				DestroyWorldEx();
};

#endif //_NTLWEAPP_H_
