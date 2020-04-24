#include "StdAfx.h"
#include "ShoreLineCP.h"
#include "NtlPLEntity.h"
#include "NtlPLObject.h"
#include "NtlPLVisualManager.h"


CShoreLineCP::CShoreLineCP(RwV3d& Pos)
{
	m_pCPUIObj = NULL;

	SPLEntityCreateParam PLEntityCreateParam;
	PLEntityCreateParam.pPos = &Pos;

	m_pCPUIObj = static_cast<CNtlPLObject*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, "CONTROL_POINT_DUMMY", &PLEntityCreateParam));
	DBO_ASSERT(m_pCPUIObj, "Entity create failed.");

	m_pCPUIObj->SetFadeEnable(FALSE);

	m_NmlFixed = FALSE;
}

CShoreLineCP::~CShoreLineCP(void)
{
	GetSceneManager()->DeleteEntity(m_pCPUIObj);
	m_pCPUIObj = NULL;
}

RwV3d CShoreLineCP::GetPosition()
{
	return m_pCPUIObj->GetPosition();
}

VOID CShoreLineCP::SetPosition(RwV3d& Pos)
{
	m_pCPUIObj->SetPosition(&Pos);
}