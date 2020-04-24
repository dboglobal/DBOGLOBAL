#include "precomp_ntlpresentation.h"
#include "NtlPLGuiGroup.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLSceneChangerGui.h"
#include "NtlPLCinematicObject.h"
#include "NtlPLProfilerGui.h"
#include "NtlPLChatBalloonManager.h"


CNtlPLGuiGroup::CNtlPLGuiGroup()
:m_pSceneChanger(NULL)
,m_pProfilerGui(NULL)
{	
}

CNtlPLGuiGroup::~CNtlPLGuiGroup()
{
}


RwBool CNtlPLGuiGroup::Create(void)
{
	NTL_FUNCTION("CNtlPLGuiGroup::Create");

	CNtlPLBalloonManager::CreateInstance();
	
	// SceneChanger
	m_pSceneChanger = NTL_NEW CNtlPLSceneChangerGui("SceneChange");
	if(!m_pSceneChanger->Create())
	{
		m_pSceneChanger->Destroy(); 
		NTL_DELETE(m_pSceneChanger);
		NTL_RETURN(FALSE);
	}
	CNtlPLGuiManager::GetInstance()->AddGui(m_pSceneChanger);

	m_pProfilerGui = NTL_NEW CNtlPLProfilerGui("ProfilerGui");
	if(!m_pProfilerGui->Create())
	{
		m_pProfilerGui->Destroy();
		NTL_DELETE(m_pProfilerGui);
		NTL_RETURN(FALSE);
	}
	CNtlPLGuiManager::GetInstance()->AddGui(m_pProfilerGui);

	// Cinematic Object
	CNtlPLCinematicObject::CreateInstance();	

	NTL_RETURN(TRUE);
}

void CNtlPLGuiGroup::Destroy(void)
{
	NTL_FUNCTION("CNtlPLGuiGroup::Destroy");

	if(m_pSceneChanger)
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui(m_pSceneChanger);
		m_pSceneChanger->Destroy();
		NTL_DELETE(m_pSceneChanger);
	}

	if(m_pProfilerGui)
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui(m_pProfilerGui);
		m_pProfilerGui->Destroy();
		NTL_DELETE(m_pProfilerGui);
	}

	CNtlPLCinematicObject::DeleteInstance();
	
	CNtlPLBalloonManager::DeleteInstance();	

	NTL_RETURNVOID();
}