#pragma once

#include "NtlApplication.h"

class CNtlPLEventHandler;
class CNtlPLVisualManager;
class CNtlPLPropertyContainer;

class CNtlPLWorldEntity;

class CNtlPostEffectCamera;

class CNtlWPPApplication : public CNtlApplication
{
public:
	CNtlWPPApplication();
	virtual ~CNtlWPPApplication();

	// CNtlApplication
	virtual RwBool	CreateSubSystem(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer);
	virtual void	Destroy();

	virtual RwBool	Update(RwReal fTime, RwReal fElapsedTime);

	virtual RwBool	AttachPlugin();

	// CNtlWPPApplication	
	virtual RwBool	TestWorldProject(const char* pcWorldFullPath, sNtlWorldParam* pNtlWorldParam = NULL);
	virtual RwBool	CreateWorldProject(const char* pcWorldFullPath, char* pcLoadVersion, char* pcSaveVersion);
	virtual void	DestroyWorldProject();

	virtual RwInt32	GetWorldProjectFieldNum(const char* pcWorldFullPath);

	virtual void	InitPatchLog();
	virtual void	PrintPatchLog(RwChar *format, ...);
	
protected:
	CNtlPLEventHandler*			m_pNtlPLEventHandler;
	CNtlPLVisualManager*		m_pNtlPLVisualManager;
	CNtlPLPropertyContainer*	m_pNtlPLPropertyContainer;

	CNtlPLWorldEntity*			m_pNtlPLWorldEntity;
	CNtlPostEffectCamera*		m_pNtlPostEffectCamera;

	std::string					m_strLogFilePath;
	std::string					m_strTraceFilePath;
	std::string					m_strPatchFilePath;
};
