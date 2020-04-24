// ConvertingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool2.h"
#include "ConvertingDlg.h"
#include "ModelToolApplication.h"
#include "NtlPlDef.h"

CConvertingDlg::EConvertType CConvertingDlg::m_eType = CONVERT_BIN;

// CConvertingDlg dialog

IMPLEMENT_DYNAMIC(CConvertingDlg, CDialog)

CConvertingDlg::CConvertingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConvertingDlg::IDD, pParent)
{
    
}

CConvertingDlg::~CConvertingDlg()
{
}

void CConvertingDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);	
    DDX_Control(pDX, IDC_MSG, m_msgEdit);
    DDX_Control(pDX, IDC_PROG, m_progress);
    DDX_Control(pDX, IDOK, m_okButton);
    DDX_Control(pDX, IDC_RE_LOG, m_reLog);
}

void CConvertingDlg::SetState( EConverState eState ) 
{
	m_progress.SetRange(0, 50);

	switch(eState)
	{
	case LOAD_CHARACTER:
		m_msgEdit.SetWindowText("Loading Character XML Data...");
		m_progress.SetPos(10);
		break;
	case LOAD_ITEM:
		m_msgEdit.SetWindowText("Loading Item XML Data...");
		m_progress.SetPos(20);
		break;
	case LOAD_OBJECT:
		m_msgEdit.SetWindowText("Loading Object XML Data...");
		m_progress.SetPos(30);
		break;
	case SAVE_DATA:
		m_msgEdit.SetWindowText("XML->Binary Data Converting...");
		m_progress.SetPos(40);
		break;	
	case SAVE_COMPLETE:
		m_msgEdit.SetWindowText("XML->Binary Data Convert Complete!!");
		m_progress.SetPos(50);
		break;
	case SAVE_FAIL:
		m_msgEdit.SetWindowText("XML->Binary Data Convert Fail!! (It may be Read Only File)");
		m_progress.SetPos(0);
		break;
    case UPDATE_CHAR:
        m_msgEdit.SetWindowText("Update Character Data Complete");
        m_progress.SetPos(40);
        break;
    case UPDATE_OBJ:
        m_msgEdit.SetWindowText("Update Object Data Complete");
        m_progress.SetPos(45);
        break;
    case UPDATE_ITEM:
        m_msgEdit.SetWindowText("Update Item Data Complete");
        m_progress.SetPos(50);
        break;
	}
}

BEGIN_MESSAGE_MAP(CConvertingDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()


// CConvertingDlg message handlers

void CConvertingDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		DWORD dwDummy = 0;
        switch(m_eType)
        {
        case CONVERT_BIN:       
        case CONVERT_ENC_BIN:
            CreateThread(NULL, 0, ConvertBinary, this, 0, &dwDummy);  
            break;        
        case CONVERT_UPDATE_PROP:
            CreateThread(NULL, 0, PropDataResave, this, 0, &dwDummy);
            break;
        }
	}
}

RwBool CConvertingDlg::LoadPropertyAll() 
{
    SetState(LOAD_CHARACTER);
    if(!CModelToolApplication::GetInstance()->GetPropertyContainer()->LoadCharacter(".\\devdata\\character\\characterpropertyList.xml"))
    {
        m_msgEdit.SetWindowText("PC Character XML Data Load Fail!");
        return FALSE;
    }	

    if(!CModelToolApplication::GetInstance()->GetPropertyContainer()->LoadCharacter(".\\devdata\\character\\mobpropertyList.xml"))
    {
        m_msgEdit.SetWindowText("Mob Character XML Data Load Fail!");
        return FALSE;
    }

    if(!CModelToolApplication::GetInstance()->GetPropertyContainer()->LoadCharacter(".\\devdata\\character\\npcpropertyList.xml"))
    {
        m_msgEdit.SetWindowText("NPC Character XML Data Load Fail!");
        return FALSE;
    }

    if(!CModelToolApplication::GetInstance()->GetPropertyContainer()->LoadCharacter(".\\devdata\\character\\VehiclePropertyList.xml"))
    {
        m_msgEdit.SetWindowText("NPC Character XML Data Load Fail!");
        return 0;
    }

    SetState(LOAD_ITEM);
    if(!CModelToolApplication::GetInstance()->GetPropertyContainer()->LoadItem(".\\devdata\\item\\itempropertylist.xml"))
    {
        m_msgEdit.SetWindowText("Item XML Data Load Fail!");
        return FALSE;
    }

    SetState(LOAD_OBJECT);
    if(!CModelToolApplication::GetInstance()->GetPropertyContainer()->LoadObject(".\\devdata\\object\\ObjectPropertyList.XML"))
    {
        m_msgEdit.SetWindowText("Object XML Data Load Fail!");
        return FALSE;
    }

    return TRUE;
}

DWORD WINAPI CConvertingDlg::ConvertBinary(LPVOID pParam)
{
	CConvertingDlg* pDlg = (CConvertingDlg*)pParam;

    if(!pDlg->LoadPropertyAll())
        return 0;

	pDlg->SetState(SAVE_DATA);

	BOOL bResult = FALSE;
	if(pDlg->m_eType == CONVERT_ENC_BIN)
	{
		bResult = CModelToolApplication::GetInstance()->GetPropertyContainer()->SaveSerialize(TRUE, ENC_DATA_PASSWORD);
	}
	else
	{
		bResult = CModelToolApplication::GetInstance()->GetPropertyContainer()->SaveSerialize();		
	}
	
	if(bResult)
	{
		pDlg->SetState(SAVE_COMPLETE);
	}
	else
	{
		pDlg->SetState(SAVE_FAIL);
	}	

	return 0;
}

#define GetMapProp(type) CModelToolApplication::GetInstance()->GetPropertyContainer()->Gets(type)


DWORD WINAPI CConvertingDlg::PropDataResave( LPVOID pParam ) 
{
    CConvertingDlg* pDlg = (CConvertingDlg*)pParam;

    if(!pDlg->LoadPropertyAll())
        return 0;

    char szFileName[1024] = {0,};
    CMTPropertyContainer::MapProp::iterator it;        
    BOOL bSuccess;

    // Character                
    CNtlPLCharacterProperty* pProp = NULL;
    for(it = GetMapProp(PLENTITY_CHARACTER).begin(); it != GetMapProp(PLENTITY_CHARACTER).end(); ++it)
    {
        bSuccess = FALSE;        

        pProp = (CNtlPLCharacterProperty*)it->second;        
        if(pDlg->UpdateAnimData(pProp->GetName(), pProp->GetAnimTable()))
        {
            sprintf_s(szFileName, "%s%s.XML", pProp->m_strCharacterDataPath.c_str(), pProp->GetName());
            bSuccess = pProp->SaveScript(szFileName);
        }        

        CString msg;
        if(bSuccess)
        {
            msg.Format("Update : [%S]", it->first.c_str());
            pDlg->m_msgEdit.SetWindowText(msg);            
        }
        else 
        {
            msg.Format("FAIL : [%S] Character XML Data Load or Save Fail!", it->first.c_str());
            pDlg->m_msgEdit.SetWindowText(msg);            
        }    
    }

    pDlg->SetState(UPDATE_CHAR);

    // Object
    CNtlPLObjectProperty* pPropObj = NULL;
    for(it = GetMapProp(PLENTITY_OBJECT).begin(); it != GetMapProp(PLENTITY_OBJECT).end(); ++it)
    {
        bSuccess = FALSE;        

        pPropObj = (CNtlPLObjectProperty*)it->second;        
        if(pDlg->UpdateAnimData(pPropObj->GetName(), pPropObj->GetAnimTable()))
        {
            sprintf_s(szFileName, "%s%s.XML", pPropObj->m_strObjectDataPath.c_str(), pPropObj->GetName());
            bSuccess = pPropObj->SaveScript(szFileName);
        }        

        CString msg;
        if(bSuccess)
        {
            msg.Format("Update : [%S]", it->first.c_str());
            pDlg->m_msgEdit.SetWindowText(msg);            
        }
        else 
        {
            msg.Format("FAIL : [%S] Character XML Data Load or Save Fail!", it->first.c_str());
            pDlg->m_msgEdit.SetWindowText(msg);            
        }    
    }

    pDlg->SetState(UPDATE_OBJ);

    // Item
    CNtlPLItemProperty* pPropItem = NULL;
    for(it = GetMapProp(PLENTITY_ITEM).begin(); it != GetMapProp(PLENTITY_ITEM).end(); ++it)
    {
        bSuccess = FALSE;        

        pPropItem = (CNtlPLItemProperty*)it->second;        
        if(pDlg->UpdateAnimData(pPropItem->GetName(), pPropItem->GetAnimTable()))
        {
            sprintf_s(szFileName, "%s%s.XML", pPropItem->m_strItemDataPath.c_str(), pPropItem->GetName());
            bSuccess = pPropItem->SaveScript(szFileName);
        }        

        CString msg;
        if(bSuccess)
        {
            msg.Format("Update : [%S]", it->first.c_str());
            pDlg->m_msgEdit.SetWindowText(msg);            
        }
        else 
        {
            msg.Format("FAIL : [%S] Character XML Data Load or Save Fail!", it->first.c_str());
            pDlg->m_msgEdit.SetWindowText(msg);            
        }    
    }

    pDlg->SetState(UPDATE_ITEM);

    return 0;
}

RwBool CConvertingDlg::UpdateAnimData(const char* szName, CNtlTypeAnimTable* pAnimTable)
{
    USES_CONVERSION;

    if(!pAnimTable)
        return FALSE;
    
    CNtlPLResource* pResource = NULL;        
    char szAnimFile[1024] = {0,};

    TYPE_ANIM_MAP_ITER it;
    for(it = pAnimTable->GetTypeAnimMap()->begin(); it != pAnimTable->GetTypeAnimMap()->end(); ++it)
    {
        sprintf_s(szAnimFile, "%s%s", pAnimTable->GetAnimPath().c_str(), pAnimTable->GetFileName(it->first)->c_str());
        pResource = CNtlPLResourceManager::GetInstance()->LoadAnimation(szAnimFile);
        it->second->fPlayTime = pResource->GetAnimation()->duration;

        // 시간을 넘어가는 Event가 있는지 체크
        CheckAnimEventTime(szName, it->second);

        CNtlPLResourceManager::GetInstance()->UnLoad(pResource);
    }            
    
    return TRUE;
}

void CConvertingDlg::CheckAnimEventTime(const char* szName, STypeAnimData* pAnimData)
{
    for each(SEventAnim* pAnimEvent in pAnimData->vecAnimEvent)
    {
        if(pAnimEvent->fTime > pAnimData->fPlayTime)
        {
            CHAR szMsg[1024] = {0,};
            sprintf_s(szMsg, "[%S] Event(ID:%d) Time Over Animation Play Time [(%d)%S]\n",szName, pAnimEvent->eEventID, pAnimData->uiAnimKey, pAnimData->strAnimName.c_str());
            
            int nLength = m_reLog.GetWindowTextLength();
            m_reLog.SetSel(nLength, nLength);
            m_reLog.ReplaceSel(szMsg);
        }
    }
}