#include "precomp_ntlpresentation.h"
#include "ntlworldbrush.h"
#include "ntlworldbrushcontroller.h"
#include "NtlDebug.h"


CNtlWorldBrush::CNtlWorldBrush()
{
	m_pController = NULL;
}

CNtlWorldBrush::~CNtlWorldBrush()
{
	NTL_DELETE(m_pController);
}
	
void CNtlWorldBrush::SetController(CNtlWorldBrushController *pController)
{
	NTL_FUNCTION("CNtlWorldBrush::SetController");

	NTL_DELETE(m_pController);

	m_pController = pController;
	
	NTL_RETURNVOID();
}

void CNtlWorldBrush::Render()
{
	if(m_pController)
	{	
		m_pController->Render();
	}
}