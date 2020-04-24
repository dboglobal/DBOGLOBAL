#include "stdafx.h"
#include "ModelTool2.h"
#include "TreeVehiclePane.h"


CTreeVehiclePane* CTreeVehiclePane::m_pInstance = NULL;

// CTreeVehiclePane

IMPLEMENT_DYNCREATE(CTreeVehiclePane, CTreePCPane)

CTreeVehiclePane::CTreeVehiclePane()
{
    m_pInstance = this;
    m_pImageList = NULL;
    m_pCharacter = NULL;

    m_strRootName = "VEHICLE";
}

CTreeVehiclePane::~CTreeVehiclePane()
{
}

BEGIN_MESSAGE_MAP(CTreeVehiclePane, CTreePCPane)
    ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CTreeVehiclePane 진단입니다.

#ifdef _DEBUG
void CTreeVehiclePane::AssertValid() const
{
	CTreePCPane::AssertValid();
}

#ifndef _WIN32_WCE
void CTreeVehiclePane::Dump(CDumpContext& dc) const
{
	CTreePCPane::Dump(dc);
}
#endif
#endif //_DEBUG

void CTreeVehiclePane::InitClass() 
{
    USES_CONVERSION;

    CString sWorkFolder = (CModelToolApplication::GetInstance()->GetWorkDir());
    m_sSaveFolderName = sWorkFolder;
    m_sSaveFolderName += MT_PATH_CHARACTER;

    m_strTreeFileName = sWorkFolder;
    m_strTreeFileName += "\\Tool\\ModelTool\\TreeVehicle.xml";

    m_strPropertyListFileName = m_sSaveFolderName;
    m_strPropertyListFileName += "VehiclePropertyList.xml";

}


// CTreeVehiclePane 메시지 처리기입니다.

void CTreeVehiclePane::OnSetFocus(CWnd* pOldWnd)
{
    CTreePCPane::OnSetFocus(pOldWnd);

    CModelToolApplication::GetInstance()->SetAppMode(MT_MODE_VEHICLE);    
}

