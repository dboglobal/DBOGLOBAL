#ifndef _DBO_CHARACTER_APPLICATION_
#define _DBO_CHARACTER_APPLICATION_

#include "NtlApplication.h"

#include "NtlPLVisualManager.h"
#include "NtlPLPropertyContainer.h"

#include "ToolCamera.h"

#include "NtlPLCharacter.h"
#include "NtlPLCharacterParser.h"

#include <rwcore.h>
#include <rpworld.h>
#include "NtlPLAnimSet.h"

#include "NtlAtomic.h"
#include "ntlworldplugin.h"
class CDboCharacterApplication : public CNtlApplication
{
protected:
	CNtlGameCamera	*m_pCamera;
	RtCharset		*m_pCharset;
	RwRGBA			m_BackgroundColor;
	RwRGBA			m_ForegroundColor;
	
	
	CNtlPLVisualManager			*m_pVisualManager;
	CNtlPLPropertyContainer		m_PropContainter;
	

	CNtlPLCharacter				*m_pPLCharacter;
	RpWorld						*m_pWorld;
	RpLight						*m_pLtAmbient;
	RpLight						*m_pLtDirectional;

	DWORD						m_dwPrevLMouseDownTime;
	DWORD						m_dwPrevRMouseDownTime;

protected:
	RwBool	CreateSubSystem(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer);
	void	DisplayInfo();
	RwBool	AttachPlugin();
	
public:

	CDboCharacterApplication(void);
	~CDboCharacterApplication(void);

	virtual void   Destroy();
	RwBool	Update(RwReal fTime, RwReal fElapsedTime);
	RwBool	OnIdle() { return CNtlApplication::Update(); }
	RwBool	Resize(RwUInt32 iWidth, RwUInt32 iHeight, RwBool zBuffer);

	static	CDboCharacterApplication *GetInstance()	{ return (CDboCharacterApplication *)m_pInstance; }
	virtual LRESULT WndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

public:
	RwBool	CreatePLCharacter(const char *chScriptFileName);
	
	CNtlPLActionName *GetConditionName()      { return CNtlPLCharacterParser::GetInstance().GetConditionName();}
	CNtlPLActionName *GetActionName()         { return CNtlPLCharacterParser::GetInstance().GetActionName(); }
	CNtlPLAnimationTable *GetAnimaitonTable() { return m_pPLCharacter->GetAnimationTable(); }
	
	RwBool	SavePLCharacter(const char *szSaveFileName);
	RwBool	LoadPLCharacter(const char *szLoadFileName);
	RwBool	SetAllChannelChangeAnimation(unsigned int uiAnimKey);
};

#endif
