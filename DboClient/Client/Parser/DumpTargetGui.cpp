#include "precomp_dboclient.h"
#include "DumpTargetGui.h"
#include "NtlDebug.h"
#include "NtlPLGuiManager.h"
#include "DumpGui.h"

CDumpTargetGui::CDumpTargetGui()
{
	m_pDumpGui = NULL;
}

CDumpTargetGui::~CDumpTargetGui()
{
	if(m_pDumpGui)
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui(m_pDumpGui);
		m_pDumpGui->Destroy();
		NTL_DELETE(m_pDumpGui);
	}
}

void CDumpTargetGui::Dump(void)
{
	if(m_pDumpGui)
	{
		gui::COutputBox *poutDisplay = m_pDumpGui->GetOutputBox();
		poutDisplay->AddText(GetStream());
	}

	CNtlDumpTarget::Dump(); 
}

void CDumpTargetGui::Active(RwBool bActive)
{
	if(bActive)
	{
		if(m_pDumpGui == NULL)
		{
			m_pDumpGui = NTL_NEW CDumpGui("DumpGui");
			if(!m_pDumpGui->Create())
			{
				m_pDumpGui->Destroy(); 
				NTL_DELETE(m_pDumpGui);
			}
		}
	}
	else
	{
		if(m_pDumpGui)
		{
			CNtlPLGuiManager::GetInstance()->RemoveGui(m_pDumpGui);
			m_pDumpGui->Destroy();
			NTL_DELETE(m_pDumpGui);
		}
	}

	CNtlDumpTarget::Active(bActive); 
}


void CDumpTargetGui::Color(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	if(m_pDumpGui)
	{
		gui::COutputBox *poutDisplay = m_pDumpGui->GetOutputBox();
		poutDisplay->SetTextColor(RGB(byRed, byGreen, byBlue));
	}
}