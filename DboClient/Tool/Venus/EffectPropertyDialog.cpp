// EffectPropertyDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "VenusFrame.h"
#include "EffectPropertyDialog.h"

#include "EffectEditLayer.h"

#include "PropertyControlID.h"

#include "NtlResourceFileManager.h"
#include "NtlEffectSystemFreeList.h"

#include ".\venusvisualmanager.h"
#include "VenusPropertyContainer.h"

#include ".\effectpropertydialog.h"

#include "StringManager.h"
#include "VenusAPI.h"


// CEffectPropertyDialog 대화 상자입니다.

const RwChar* g_BlendString[] = {	"rwBLENDZERO",			\
									"rwBLENDONE",			\
									"rwBLENDSRCCOLOR",		\
									"rwBLENDINVSRCCOLOR",	\

									"rwBLENDSRCALPHA",		\
									"rwBLENDINVSRCALPHA",	\
									"rwBLENDDESTALPHA",		\
									"rwBLENDINVDESTALPHA",	\

									"rwBLENDDESTCOLOR",		\
									"rwBLENDINVDESTCOLOR",	\
									"rwBLENDDESTCOLOR",		\
									"rwBLENDSRCALPHASAT" };
const RwInt32 g_nBlendMaxCount = 12;

static RwBlendFunction BlendFunctions[g_nBlendMaxCount] =
{
    rwBLENDZERO,
    rwBLENDONE,
    rwBLENDSRCCOLOR,
    rwBLENDINVSRCCOLOR,

    rwBLENDSRCALPHA,
    rwBLENDINVSRCALPHA,
    rwBLENDDESTALPHA,
    rwBLENDINVDESTALPHA,

    rwBLENDDESTCOLOR,
    rwBLENDINVDESTCOLOR,
    rwBLENDDESTCOLOR,
    rwBLENDSRCALPHASAT
};

inline RwInt32 GetBlendIndex(RwBlendFunction blend)
{
	for (RwInt32 i = 0; i < g_nBlendMaxCount; ++ i)
	{
		if (BlendFunctions[i] == blend) return i;
	}

	return -1;
}


IMPLEMENT_DYNCREATE(CEffectPropertyDialog, CDialog)
CEffectPropertyDialog::CEffectPropertyDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEffectPropertyDialog::IDD, pParent)
{
	m_Initialized = FALSE;

}

CEffectPropertyDialog::~CEffectPropertyDialog()
{
	m_PropertyTree.DeleteAllItems();
}

void CEffectPropertyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEffectPropertyDialog, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(PTN_ITEMCHANGED, IDC_PROPERTY_STATIC, OnItemChanged)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CEffectPropertyDialog 메시지 처리기입니다.

BOOL CEffectPropertyDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	DWORD dwStyle;
	CRect rc;

	// PTS_NOTIFY - CPropTree will send notification messages to the parent window
	dwStyle = WS_CHILD|WS_VISIBLE|PTS_NOTIFY;

	// Init the control's size to cover the entire client area
	CWnd* pPropertyStaticWindow = GetDlgItem(IDC_PROPERTY_STATIC);
	if( pPropertyStaticWindow )
	{
		pPropertyStaticWindow->GetWindowRect(rc);
		this->ScreenToClient( &rc );
	}

	// Create CPropTree control
	m_PropertyTree.Create(dwStyle, rc, this, IDC_PROPERTY_STATIC);

	CPropTreeItem* pRoot;
	pRoot = m_PropertyTree.InsertItem(new CPropTreeItem());
	pRoot->SetLabelText(_T("No selected"));

	m_Initialized = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control	
}

void CEffectPropertyDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(m_Initialized)
	{
		CWnd* pPropertyStaticWindow = GetDlgItem(IDC_PROPERTY_STATIC);
		if(pPropertyStaticWindow)
		{			
			CRect rc;	
			pPropertyStaticWindow->GetWindowRect(rc);
			this->ScreenToClient( &rc );
			rc.right = rc.left + cx - 10;			
			pPropertyStaticWindow->MoveWindow( rc.left, rc.top, cx-2,rc.bottom-rc.top );

			if (::IsWindow(m_PropertyTree.GetSafeHwnd()))
				m_PropertyTree.MoveWindow( &rc );		
		}
	}
}

BOOL CEffectPropertyDialog::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN) 
	{
		if(pMsg->wParam == VK_RETURN) return FALSE;	
		if(pMsg->wParam == VK_ESCAPE) return FALSE;	
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CEffectPropertyDialog::OnTimer(UINT nIDEvent)
{
	KillTimer(nIDEvent);

	switch(nIDEvent)
	{
	case TIME_TYPE_RESET_PROPERTIES:
		{
			ResetProperties(m_nCurrentPoint);
		}
		break;
	case TIME_TYPE_CHANGE_SYSTEMNAME:
		{
			if (CVenusVisualManager::GetInstance().m_pResourceSystem != NULL)
			{
				CEffectEditLayer* pView = (CEffectEditLayer*)gGetEditLayer(EDIT_LAYER_EFFECT_EDIT);
				pView->ResetTreeItemName(CVenusVisualManager::GetInstance().m_pResourceSystem->GetSystemName());
			}
		}
		break;
	case TIME_TYPE_CHANGE_EFFECTNAME:
		{
			if (CVenusVisualManager::GetInstance().m_pResourceEffect != NULL)
			{
				CEffectEditLayer* pView = (CEffectEditLayer*)gGetEditLayer(EDIT_LAYER_EFFECT_EDIT);
				pView->ResetTreeItemName(CVenusVisualManager::GetInstance().m_pResourceEffect->GetName());
			}
		}
		break;
	}

	CDialog::OnTimer(nIDEvent);
}


void CEffectPropertyDialog::OnItemChanged(NMHDR* pNotifyStruct, LRESULT* plResult)
{
	LPNMPROPTREE pNMPropTree = (LPNMPROPTREE)pNotifyStruct;
	
	if (pNMPropTree->pItem)
	{
		ApplyProperties(pNMPropTree->pItem);
	}
	*plResult = 0;
}

//------------------------------------------------------------------
//	FuncName	: ResetProperties
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::ResetProperties(RwUInt32 nCurrentPoint)
{
	CNtlResourceComponentSystem* pResourceSystem = CVenusVisualManager::GetInstance().m_pResourceSystem;
	CNtlResourceEffect* pResourceEffect = CVenusVisualManager::GetInstance().m_pResourceEffect;

	m_nCurrentPoint = nCurrentPoint;

	m_PropertyTree.DeleteAllItems();
	if (pResourceEffect == NULL) return;

	if (pResourceSystem == NULL)
	{
		CString strPropertiesTitle;
		CPropTreeItem* pRoot = m_PropertyTree.InsertItem(new CPropTreeItem());
		pRoot->Expand();
		strPropertiesTitle.Format( _T("[%s] Effect properties"), pResourceEffect->GetName());
		pRoot->SetLabelText(strPropertiesTitle);

		SetProperties_ResourceEffect(pResourceEffect, pRoot);
		return;
	}

	CString strPropertiesTitle;
	CPropTreeItem* pRoot = m_PropertyTree.InsertItem(new CPropTreeItem());
	pRoot->Expand();
	strPropertiesTitle.Format( _T("[%s] System properties"), pResourceSystem->GetSystemName());
	pRoot->SetLabelText(strPropertiesTitle);

	switch(pResourceSystem->GetSystemType())
	{
		case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_PARTICLE:
		{
			switch(CVenusVisualManager::GetInstance().m_nSystemDataFlag)
			{
			case 0:
				SetProperties_ResourceParticleSystem((CNtlResourceParticleSystem*)pResourceSystem, pRoot);
				break;

			case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:
				SetProperties_EmitterPrtColor(&((CNtlResourceParticleSystem*)pResourceSystem)->m_EmitterPrtCol, pRoot);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTTEXCOORDS:
				SetProperties_EmitterPrtTexCoords(&((CNtlResourceParticleSystem*)pResourceSystem)->m_EmitterPrtTexCoords, pRoot);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRT2DROTATE:
				SetProperties_EmitterPrt2DRotate(&((CNtlResourceParticleSystem*)pResourceSystem)->m_EmitterPrtRotate, pRoot);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTSIZE:
				SetProperties_EmitterPrtSize(&((CNtlResourceParticleSystem*)pResourceSystem)->m_EmitterPrtSize, pRoot);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTMATRIX:
				SetProperties_EmitterPrtMatrix(&((CNtlResourceParticleSystem*)pResourceSystem)->m_EmitterPrtMatrix, pRoot, pResourceSystem->m_nEmitterDataFlag);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTHURRICANE:
				SetProperties_EmitterPrtHurricane(&((CNtlResourceParticleSystem*)pResourceSystem)->m_EmitterPrtHurricane, pRoot);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTORBITPOINT:
				SetProperties_EmitterPrtOrbitPoint(&((CNtlResourceParticleSystem*)pResourceSystem)->m_EmitterPrtOrbitPoint, pRoot);
				break;
			case rpPRTADVEMITTERDATAFLAGPRTEMITTER:
				SetProperties_PrtEmitterEmitter(&((CNtlResourceParticleSystem*)pResourceSystem)->m_PrtEmitterEmitter, pRoot);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTICOLOR:
				SetProperties_MultiColorEmitter(&((CNtlResourceParticleSystem*)pResourceSystem)->m_EmitterPrtMultiCol, pRoot);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTITEXCOORDS:
				SetProperties_MultiTexCoordsEmitter(&((CNtlResourceParticleSystem*)pResourceSystem)->m_EmitterPrtMultiTexCoords, pRoot);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTISIZE:
				SetProperties_MultiSizeEmitter(&((CNtlResourceParticleSystem*)pResourceSystem)->m_EmitterPrtMultiSize, pRoot);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTITEXTURES:
				SetProperties_MultiTexturesEmitter(&((CNtlResourceParticleSystem*)pResourceSystem)->m_EmitterMultiTextures, pRoot);
				break;
			case rpPRTADVEMITTERDATAFLAGSPLINEPATH:
				SetProperties_SplinePathEmitter(&((CNtlResourceParticleSystem*)pResourceSystem)->m_EmitterPrtSplinePath, pRoot);
				break;

			case rpPRTADVEMITTERDATAFLAGPOINTLIST:
				SetProperties_EmitterPointList(&((CNtlResourceParticleSystem*)pResourceSystem)->m_EmitterPtList, pRoot);
				break;
			case rpPRTADVEMITTERDATAFLAGCIRCLE:
				SetProperties_CircleEmitter(&((CNtlResourceParticleSystem*)pResourceSystem)->m_EmitterPtrCircle, pRoot);
				break;
			case rpPRTADVEMITTERDATAFLAGSPHERE:
				SetProperties_SphereEmitter(&((CNtlResourceParticleSystem*)pResourceSystem)->m_EmitterPtrSphere, pRoot);
				break;
            case rpPRTSTDEMITTERDATAFLAGTRAIL:
                SetProperties_EmitterTrail(&((CNtlResourceParticleSystem*)pResourceSystem)->m_EmitterTrail, pRoot);
                break;
			}
		}
		break;
		case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_MESH:
		{
			switch(CVenusVisualManager::GetInstance().m_nSystemDataFlag)
			{
			case 0:
				SetProperties_ResourceMeshSystem((CNtlResourceMeshSystem*)pResourceSystem, pRoot);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:
				SetProperties_EmitterPrtColor(&((CNtlResourceMeshSystem*)pResourceSystem)->m_EmitterPrtCol, pRoot);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRT2DROTATE:
				SetProperties_EmitterPrt2DRotate(&((CNtlResourceMeshSystem*)pResourceSystem)->m_EmitterPrtRotate, pRoot);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTSIZE:
				SetProperties_EmitterPrtSize(&((CNtlResourceMeshSystem*)pResourceSystem)->m_EmitterPrtSize, pRoot);
				break;
            case rpPRTSTDEMITTERDATAFLAGPRTFINISH:
                SetProperties_EmitterPrtFinish(&((CNtlResourceMeshSystem*)pResourceSystem)->m_EmitterPrtFinish, pRoot);
                break;

			case rpPRTADVEMITTERDATAFLAGMULTICOLOR:
				SetProperties_MultiColorEmitter(&((CNtlResourceMeshSystem*)pResourceSystem)->m_EmitterPrtMultiCol, pRoot);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTISIZE:
				SetProperties_MultiSizeEmitter(&((CNtlResourceMeshSystem*)pResourceSystem)->m_EmitterPrtMultiSize, pRoot);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTIROTATE:
				SetProperties_MultiRotateEmitter(&((CNtlResourceMeshSystem*)pResourceSystem)->m_EmitterPrtMultiRotate, pRoot);
				break;

			case rpPRTADVEMITTERDATAFLAGSPLINEPATH:
				SetProperties_SplinePathEmitter(&((CNtlResourceMeshSystem*)pResourceSystem)->m_EmitterPrtSplinePath, pRoot);
				break;
            case rpPRTSTDEMITTERDATAFLAGTRAIL:
                SetProperties_EmitterTrail(&((CNtlResourceMeshSystem*)pResourceSystem)->m_EmitterTrail, pRoot);
                break;
                
			}
		}
		break;
		case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_BEAM:
		{
			switch(CVenusVisualManager::GetInstance().m_nSystemDataFlag)
			{
			case 0:
				SetProperties_ResourceBeamSystem((CNtlResourceBeamSystem*)pResourceSystem, pRoot);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:
				SetProperties_EmitterPrtColor(&((CNtlResourceBeamSystem*)pResourceSystem)->m_EmitterPrtCol, pRoot);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTSIZE:
				SetProperties_EmitterPrtSize(&((CNtlResourceBeamSystem*)pResourceSystem)->m_EmitterPrtSize, pRoot);
				break;
            case rpPRTSTDEMITTERDATAFLAGPRTFINISH:
                SetProperties_EmitterPrtFinish(&((CNtlResourceBeamSystem*)pResourceSystem)->m_EmitterPrtFinish, pRoot);
                break;
			case rpPRTADVEMITTERDATAFLAGMULTICOLOR:
				SetProperties_MultiColorEmitter(&((CNtlResourceBeamSystem*)pResourceSystem)->m_EmitterPrtMultiCol, pRoot);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTISIZE:
				SetProperties_MultiSizeEmitter(&((CNtlResourceBeamSystem*)pResourceSystem)->m_EmitterPrtMultiSize, pRoot);
				break;

			case rpPRTSTDEMITTERDATAFLAGPRTFADEOUT:
				SetProperties_EmitterPrtFadeOut(&((CNtlResourceBeamSystem*)pResourceSystem)->m_EmitterPrtFadeOut, pRoot);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTITEXTURES:
				SetProperties_MultiTexturesEmitter(&((CNtlResourceBeamSystem*)pResourceSystem)->m_EmitterMultiTextures, pRoot);
				break;
			}
		}
		break;
		case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_HURRICANE:
		{
			switch(CVenusVisualManager::GetInstance().m_nSystemDataFlag)
			{
			case 0:
				SetProperties_ResourceHurricaneSystem((CNtlResourceHurricaneSystem*)pResourceSystem, pRoot);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:
				SetProperties_EmitterPrtColor(&((CNtlResourceHurricaneSystem*)pResourceSystem)->m_EmitterPrtCol, pRoot);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTSIZE:
				SetProperties_EmitterPrtSize(&((CNtlResourceHurricaneSystem*)pResourceSystem)->m_EmitterPrtSize, pRoot);
				break;
            case rpPRTSTDEMITTERDATAFLAGPRTFINISH:
                SetProperties_EmitterPrtFinish(&((CNtlResourceHurricaneSystem*)pResourceSystem)->m_EmitterPrtFinish, pRoot);
                break;
                
			case rpPRTADVEMITTERDATAFLAGMULTICOLOR:
				SetProperties_MultiColorEmitter(&((CNtlResourceHurricaneSystem*)pResourceSystem)->m_EmitterPrtMultiCol, pRoot);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTISIZE:
				SetProperties_MultiSizeEmitter(&((CNtlResourceHurricaneSystem*)pResourceSystem)->m_EmitterPrtMultiSize, pRoot);
				break;

			case rpPRTADVEMITTERDATAFLAGMULTITEXTURES:
				SetProperties_MultiTexturesEmitter(&((CNtlResourceHurricaneSystem*)pResourceSystem)->m_EmitterMultiTextures, pRoot);
				break;
			}
		}
		break;
		case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LIGHTNING:
		{
			switch(CVenusVisualManager::GetInstance().m_nSystemDataFlag)
			{
			case 0:
				SetProperties_ResourceLightningSystem((CNtlResourceLightningSystem*)pResourceSystem, pRoot);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:
				SetProperties_EmitterPrtColor(&((CNtlResourceLightningSystem*)pResourceSystem)->m_EmitterPrtCol, pRoot);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTSIZE:
				SetProperties_EmitterPrtSize(&((CNtlResourceLightningSystem*)pResourceSystem)->m_EmitterPrtSize, pRoot);
				break;
            case rpPRTSTDEMITTERDATAFLAGPRTFINISH:
                SetProperties_EmitterPrtFinish(&((CNtlResourceLightningSystem*)pResourceSystem)->m_EmitterPrtFinish, pRoot);
                break;
			case rpPRTADVEMITTERDATAFLAGMULTICOLOR:
				SetProperties_MultiColorEmitter(&((CNtlResourceLightningSystem*)pResourceSystem)->m_EmitterPrtMultiCol, pRoot);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTISIZE:
				SetProperties_MultiSizeEmitter(&((CNtlResourceLightningSystem*)pResourceSystem)->m_EmitterPrtMultiSize, pRoot);
				break;

			case rpPRTADVEMITTERDATAFLAGMULTITEXTURES:
				SetProperties_MultiTexturesEmitter(&((CNtlResourceLightningSystem*)pResourceSystem)->m_EmitterMultiTextures, pRoot);
				break;
			}
		}
		break;        
        case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LINE:
        {
            switch(CVenusVisualManager::GetInstance().m_nSystemDataFlag)
            {
            case 0:
                SetProperties_ResourceLineSystem((CNtlResourceLineSystem*)pResourceSystem, pRoot);
                break;
            case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:
                SetProperties_EmitterPrtColor(&((CNtlResourceLineSystem*)pResourceSystem)->m_EmitterPrtCol, pRoot);
                break;
            case rpPRTSTDEMITTERDATAFLAGPRTSIZE:
                SetProperties_EmitterPrtSize(&((CNtlResourceLineSystem*)pResourceSystem)->m_EmitterPrtSize, pRoot);
                break;
            case rpPRTADVEMITTERDATAFLAGMULTICOLOR:
                SetProperties_MultiColorEmitter(&((CNtlResourceLineSystem*)pResourceSystem)->m_EmitterPrtMultiCol, pRoot);
                break;
            case rpPRTADVEMITTERDATAFLAGMULTISIZE:
                SetProperties_MultiSizeEmitter(&((CNtlResourceLineSystem*)pResourceSystem)->m_EmitterPrtMultiSize, pRoot);
                break;
            case rpPRTADVEMITTERDATAFLAGMULTITEXTURES:
                SetProperties_MultiTexturesEmitter(&((CNtlResourceLineSystem*)pResourceSystem)->m_EmitterMultiTextures, pRoot);
                break;      
            case rpPRTADVEMITTERDATAFLAGSPHERE:
                SetProperties_SphereEmitter(&((CNtlResourceLineSystem*)pResourceSystem)->m_EmitterSphere, pRoot);
                break;
            case rpPRTSTDEMITTERDATAFLAGPRT2DROTATE:
                SetProperties_EmitterPrt2DRotate(&((CNtlResourceLineSystem*)pResourceSystem)->m_EmitterPrtRotate, pRoot);
                break;
            case rpPRTADVEMITTERDATAFLAGMULTIROTATE:
                SetProperties_MultiRotateEmitter(&((CNtlResourceLineSystem*)pResourceSystem)->m_EmitterPrtMultiRotate, pRoot);
                break;
            }
        }
        break;
		case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_DECAL:
		{
			switch(CVenusVisualManager::GetInstance().m_nSystemDataFlag)
			{
			case 0:
				SetProperties_ResourceDecalSystem((CNtlResourceDecalSystem*)pResourceSystem, pRoot);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:
				SetProperties_EmitterPrtColor(&((CNtlResourceDecalSystem*)pResourceSystem)->m_EmitterPrtCol, pRoot);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRT2DROTATE:
				SetProperties_EmitterPrt2DRotate(&((CNtlResourceDecalSystem*)pResourceSystem)->m_EmitterPrtRotate, pRoot);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTSIZE:
				SetProperties_EmitterPrtSize(&((CNtlResourceDecalSystem*)pResourceSystem)->m_EmitterPrtSize, pRoot);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTICOLOR:
				SetProperties_MultiColorEmitter(&((CNtlResourceDecalSystem*)pResourceSystem)->m_EmitterPrtMultiCol, pRoot);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTISIZE:
				SetProperties_MultiSizeEmitter(&((CNtlResourceDecalSystem*)pResourceSystem)->m_EmitterPrtMultiSize, pRoot);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTITEXTURES:
				SetProperties_MultiTexturesEmitter(&((CNtlResourceDecalSystem*)pResourceSystem)->m_EmitterMultiTextures, pRoot);
				break;
			case rpPRTSTDEMITTERDATAFLAGUVANIM:
				SetProperties_EmitterUVAnim(&((CNtlResourceDecalSystem*)pResourceSystem)->m_EmitterUVAnim, pRoot);
				break;
			}
		}
		break;
		case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_POST_EFFECT:
		{
			switch(CVenusVisualManager::GetInstance().m_nSystemDataFlag)
			{
			case 0:
				SetProperties_ResourcePostEffectSystem((CNtlResourcePostEffectSystem*)pResourceSystem, pRoot);
                break;
            case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:
                SetProperties_EmitterPrtColor(&((CNtlResourcePostEffectSystem*)pResourceSystem)->m_EmitterPrtCol, pRoot);
                break;
            case rpPRTADVEMITTERDATAFLAGMULTICOLOR:
                SetProperties_MultiColorEmitter(&((CNtlResourcePostEffectSystem*)pResourceSystem)->m_EmitterPrtMultiCol, pRoot);
                break;
            case rpPRTADVEMITTERDATAFLAGMULTITEXTURES:
                SetProperties_MultiTexturesEmitter(&((CNtlResourcePostEffectSystem*)pResourceSystem)->m_EmitterMultiTextures, pRoot);
                break;
			}
		}
		break;
		default:
			NTL_ASSERTE(!"default");
			break;
	}
}

void CEffectPropertyDialog::SetProperties_ResourceEffect(CNtlResourceEffect* pResourceEffect, CPropTreeItem* pRoot)
{
	CString strPropertiesTitle;

	// ============================================
	// System Name
	// ============================================
	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("Name"));
	pEdit->SetInfoText(_T("Resource Efffct Name"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatText);
	pEdit->SetItemValue((LPARAM)pResourceEffect->GetName());
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EFFECT_NAME);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("Resource Efffct Scale"));
	pEdit->SetInfoText(_T("Resource Efffct Scale Test"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceEffect->m_fScaleForTool);	
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EFFECT_SCALE);

    // Sound 관련
    CPropTreeItem* pSoundRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);
    pSoundRoot->Expand(TRUE);
    pSoundRoot->SetLabelText(_T("Effect Sound"));

    std::string strSoundFileName = pResourceEffect->m_EffectSound.chSoundName;
    CPropTreeItemCombo* pCombo = (CPropTreeItemCombo*)SetProperties_Sound(strSoundFileName, pSoundRoot);    
    pCombo->SetCtrlID(_PROPERTY_CONTROL_EFFECT_SOUND_FILE_NAME1);

    strSoundFileName = pResourceEffect->m_EffectSound.chSoundName2;
    pCombo = (CPropTreeItemCombo*)SetProperties_Sound(strSoundFileName, pSoundRoot);    
    pCombo->SetCtrlID(_PROPERTY_CONTROL_EFFECT_SOUND_FILE_NAME2);

    strSoundFileName = pResourceEffect->m_EffectSound.chSoundName3;
    pCombo = (CPropTreeItemCombo*)SetProperties_Sound(strSoundFileName, pSoundRoot);    
    pCombo->SetCtrlID(_PROPERTY_CONTROL_EFFECT_SOUND_FILE_NAME3);

    strSoundFileName = pResourceEffect->m_EffectSound.chSoundName4;
    pCombo = (CPropTreeItemCombo*)SetProperties_Sound(strSoundFileName, pSoundRoot);    
    pCombo->SetCtrlID(_PROPERTY_CONTROL_EFFECT_SOUND_FILE_NAME4);

    pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pSoundRoot);
    pCombo->CreateComboBoxBool();
    pCombo->SetLabelText(_T("Sound Loop"));
    pCombo->SetItemValue((LPARAM)pResourceEffect->m_EffectSound.bLoop);
    pCombo->SetCtrlID(_PROPERTY_CONTROL_EFFECT_SOUND_LOOP);

    pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pSoundRoot);
    pEdit->SetLabelText(_T("Sound Volume"));
    pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pEdit->SetItemValue((LPARAM)&pResourceEffect->m_EffectSound.fSoundVolume);
    pEdit->SetCtrlID(_PROPERTY_CONTROL_EFFECT_SOUND_VOLUME);

    pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pSoundRoot);
    pEdit->SetLabelText(_T("Sound Distance"));
    pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pEdit->SetItemValue((LPARAM)&pResourceEffect->m_EffectSound.fSoundDist);
    pEdit->SetCtrlID(_PROPERTY_CONTROL_EFFECT_SOUND_DIST);

    pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pSoundRoot);
    pEdit->SetLabelText(_T("Sound Decay Dist"));
    pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pEdit->SetItemValue((LPARAM)&pResourceEffect->m_EffectSound.fSoundDecayDist);
    pEdit->SetCtrlID(_PROPERTY_CONTROL_EFFECT_SOUND_DECAY_DIST);

    pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pSoundRoot);
    pEdit->SetLabelText(_T("Sound Pitch Min"));
    pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pEdit->SetItemValue((LPARAM)&pResourceEffect->m_EffectSound.fSoundPitchMin);
    pEdit->SetCtrlID(_PROPERTY_CONTROL_EFFECT_SOUND_PITCH_MIN);

    pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pSoundRoot);
    pEdit->SetLabelText(_T("Sound Pitch Max"));
    pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pEdit->SetItemValue((LPARAM)&pResourceEffect->m_EffectSound.fSoundPitchMax);
    pEdit->SetCtrlID(_PROPERTY_CONTROL_EFFECT_SOUND_PITCH_MAX);

    // 바운딩 박스 설정 관련
    CPropTreeItem* pBoundingSphere = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);
    pBoundingSphere->SetLabelText(_T("Bounding Sphere"));
    pBoundingSphere->Expand(FALSE);
    
    pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pBoundingSphere);
    pCombo->CreateComboBoxBool();
    pCombo->SetLabelText(_T("Not AutoGenerate"));
    pCombo->SetItemValue((LPARAM)pResourceEffect->m_EffectBoundingSphere.bDisableAuto);
    pCombo->SetCtrlID(_PROPERTY_CONTROL_EFFECT_BOUNDING_NOTGENERATE);

    pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pBoundingSphere);
    pEdit->SetLabelText(_T("Radius"));
    pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pEdit->SetItemValue((LPARAM)&pResourceEffect->m_EffectBoundingSphere.fRadius);
    pEdit->SetCtrlID(_PROPERTY_CONTROL_EFFECT_BOUNDING_RADIUS);
    
}

void CEffectPropertyDialog::SetProperties_ResourceComponentSystem(CNtlResourceComponentSystem* pResourceComponentSystem, CPropTreeItem* pRoot)
{
	CString strPropertiesTitle;

	// ============================================
	// System Name
	// ============================================
	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("System Name"));
	pEdit->SetInfoText(_T("Resource Particle System Name"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatText);
	pEdit->SetItemValue((LPARAM)pResourceComponentSystem->GetSystemName());
	pEdit->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_NAME);

	// ============================================
	// Eternity Time
	// ============================================
	CPropTreeItemCombo* pCombo;
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);	
	strPropertiesTitle.Format(_T("Eternity"));
	pCombo->SetLabelText(strPropertiesTitle);
	pCombo->SetInfoText(strPropertiesTitle);
	pCombo->CreateComboBoxBool();
	pCombo->SetItemValue(pResourceComponentSystem->m_bEternity);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_ETERNITY);

	// ============================================
	// System Start Time
	// ============================================
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("Start Time"));
	pEdit->SetInfoText(_T("Start Time"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceComponentSystem->m_fStartTime);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_START_TIME);


	// ============================================
	// System Life Time
	// ============================================
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("Life Time"));
	pEdit->SetInfoText(_T("Life Time"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceComponentSystem->m_fLifeTime);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_LIFE_TIME);

	// ============================================
	// World Position
	// ============================================
	CPropTreeItem* pPositionRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pPositionRoot->Expand();
	pPositionRoot->SetLabelText("Position");

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pPositionRoot);	
	pEdit->SetLabelText(_T("X"));
	pEdit->SetInfoText(_T("World Position X"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceComponentSystem->m_vPosition.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_MATRIX_POSITION_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pPositionRoot);	
	pEdit->SetLabelText(_T("Y"));
	pEdit->SetInfoText(_T("World Position Y"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceComponentSystem->m_vPosition.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_MATRIX_POSITION_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pPositionRoot);	
	pEdit->SetLabelText(_T("Z"));
	pEdit->SetInfoText(_T("World Position Y"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceComponentSystem->m_vPosition.z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_MATRIX_POSITION_Z);

    // Lightning에는 Rotation 정보를 표시하지 않는다. (Rotation 값을 EndPoint 값으로 사용한다.
    if(pResourceComponentSystem->GetSystemType() != CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LIGHTNING)    
    {
        CPropTreeItem* pRotateRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
        pRotateRoot->Expand();
        pRotateRoot->SetLabelText("Rotate");

        CPropTreeItemSlider* pSlider;
        pSlider = (CPropTreeItemSlider*)m_PropertyTree.InsertItem(new CPropTreeItemSlider(), pRotateRoot);
        pSlider->SetLabelText(_T("X"));
        pSlider->SetInfoText(_T("World Rotate X"));
        pSlider->SetRange(0, 360);
        pSlider->SetColors(RGB(255, 255, 255), RGB(255, 0, 0));
        pSlider->SetItemValue((LPARAM) ((RwInt32)(pResourceComponentSystem->m_vRotate.x)) );
        pSlider->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_MATRIX_ROTATE_X);

        pSlider = (CPropTreeItemSlider*)m_PropertyTree.InsertItem(new CPropTreeItemSlider(), pRotateRoot);
        pSlider->SetLabelText(_T("Y"));
        pSlider->SetInfoText(_T("World Rotate Y"));
        pSlider->SetRange(0, 360);
        pSlider->SetColors(RGB(255, 255, 255), RGB(0, 255, 0));
        pSlider->SetItemValue((LPARAM) ((RwInt32)(pResourceComponentSystem->m_vRotate.y)) );
        pSlider->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_MATRIX_ROTATE_Y);

        pSlider = (CPropTreeItemSlider*)m_PropertyTree.InsertItem(new CPropTreeItemSlider(), pRotateRoot);
        pSlider->SetLabelText(_T("Z"));
        pSlider->SetInfoText(_T("World Rotate Z"));
        pSlider->SetRange(0, 360);
        pSlider->SetColors(RGB(255, 255, 255), RGB(0, 0, 255));
        pSlider->SetItemValue((LPARAM) ((RwInt32)(pResourceComponentSystem->m_vRotate.z)) );
        pSlider->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_MATRIX_ROTATE_Z);
    }

    // Ground : 이펙트가 지면에 붙여서 표시될지의 설정값
    CPropTreeItem* pGroundRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
    pGroundRoot->Expand(TRUE);
    pGroundRoot->SetLabelText("Ground Attach");

    pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pGroundRoot);
    pCombo->CreateComboBoxBool();
    pCombo->SetLabelText(_T("Ground Attach"));
    pCombo->SetItemValue(pResourceComponentSystem->m_sGroundFlag.bGround);
    pCombo->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_GROUND_ATTACH);

    pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pGroundRoot);
    pEdit->SetLabelText(_T("Ground Offset"));
    pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pEdit->SetItemValue((LPARAM)&pResourceComponentSystem->m_sGroundFlag.fOffsetY);
    pEdit->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_GROUND_OFFSET);
}

void CEffectPropertyDialog::SetProperties_ResourceParticleSystem(CNtlResourceParticleSystem* pResourceParticleSystem, CPropTreeItem* pRoot)
{
	CString strPropertiesTitle;

	SetProperties_ResourceComponentSystem(pResourceParticleSystem, pRoot);

	CPropTreeItem* pAlphaRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pAlphaRoot->Expand();
	pAlphaRoot->SetLabelText("System Alpha");

	// Alpha Blend
	CPropTreeItemCombo* pCombo;
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pAlphaRoot);	
	strPropertiesTitle.Format(_T("Alpha Blend"));
	pCombo->SetLabelText(strPropertiesTitle);
	pCombo->SetInfoText(strPropertiesTitle);
	pCombo->CreateComboBoxBool();
	pCombo->SetItemValue(pResourceParticleSystem->m_bAlphaBlend);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_ALPHA_ENALBE);

	// Src Blend
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pAlphaRoot);	
	strPropertiesTitle.Format(_T("Src Blend"));
	pCombo->SetLabelText(strPropertiesTitle);
	pCombo->SetInfoText(strPropertiesTitle);
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
	pCombo->SetDropDownHeight(300);
	for (RwInt32 i = 0; i < g_nBlendMaxCount; ++ i)
	{
		pCombo->AddString(_T(g_BlendString[i]));
		pCombo->SetItemData(i, i);
	}
	pCombo->SetItemValue(GetBlendIndex(pResourceParticleSystem->m_nSrcBlend));
	pCombo->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_ALPHA_SRC);

	// Dest Blend
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pAlphaRoot);	
	strPropertiesTitle.Format(_T("Dest Blend"));
	pCombo->SetLabelText(strPropertiesTitle);
	pCombo->SetInfoText(strPropertiesTitle);
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
	pCombo->SetDropDownHeight(300);
	for (RwInt32 i = 0; i < g_nBlendMaxCount; ++ i)
	{
		pCombo->AddString(_T(g_BlendString[i]));
		pCombo->SetItemData(i, i);
	}
	pCombo->SetItemValue(GetBlendIndex(pResourceParticleSystem->m_nDestBlend));
	pCombo->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_ALPHA_DEST);

    // Follow Particle 유무
    pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);
    pCombo->SetLabelText("Follow Emitter");
    pCombo->SetInfoText("Set up Follow to Emitter");
    pCombo->CreateComboBoxBool();
    if(pResourceParticleSystem->IsEmitterDataFlag(NTLrpPRTADVEMITTERDATAFLAGFOLLOW))
    {
        pCombo->SetItemValue(TRUE);
    }
    else
    {
        pCombo->SetItemValue(FALSE);
    }

    pCombo->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_FOLLOW);

	SetProperties_EmitterStandard(pResourceParticleSystem, pRoot);
}

//------------------------------------------------------------------
//	FuncName	: SetProperties_EmitterStandard
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_EmitterStandard(CNtlResourceParticleSystem* pResourceParticleSystem, CPropTreeItem* pRoot)
{
	CPropTreeItemEdit* pEdit;

	// ============================================
	// Max Particle Count
	// ============================================
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("Max Particle Count"));
	pEdit->SetInfoText(_T("Max Particle Count"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);
	pEdit->SetItemValue((LPARAM)pResourceParticleSystem->m_EmitterStandard.maxPrt);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_MAX_PARTICLE_COUNT);


	// ============================================
	// Texture
	// ============================================
	CPropTreeItem* pTextureItem = SetProperties_Textures(pResourceParticleSystem->m_strTextureName, pRoot);
	pTextureItem->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_TEXTURE);


	// ============================================
	// Rotate
	// ============================================
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("2D Rotate"));
	pEdit->SetInfoText(_T("2D Rotate"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceParticleSystem->m_EmitterStandard.prtDelta2DRotate);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_ROTATE);

	// ============================================
	// Particle Size
	// ============================================
	CPropTreeItem* pPtrRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pPtrRoot->Expand();
	pPtrRoot->SetLabelText(_T("Particle Size"));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pPtrRoot);	
	pEdit->SetLabelText(_T("X"));
	pEdit->SetInfoText(_T("Particle Size X"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceParticleSystem->m_EmitterStandard.prtSize.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_PTR_SIZE_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pPtrRoot);	
	pEdit->SetLabelText(_T("Y"));
	pEdit->SetInfoText(_T("Particle Size Y"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceParticleSystem->m_EmitterStandard.prtSize.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_PTR_SIZE_Y);

	// ============================================
	// Particle Color
	// ============================================
	CPropTreeItem* pPtrColorRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pPtrColorRoot->Expand();
	pPtrColorRoot->SetLabelText(_T("Particle Color"));

	CPropTreeItemColor* pColor;
	pColor = (CPropTreeItemColor*)m_PropertyTree.InsertItem(new CPropTreeItemColor(), pPtrColorRoot);
	pColor->SetLabelText(_T("Color"));
	pColor->SetInfoText(_T("Particle Color"));
	pColor->SetItemValue((LPARAM)RGB(pResourceParticleSystem->m_EmitterStandard.prtColor.red, pResourceParticleSystem->m_EmitterStandard.prtColor.green, pResourceParticleSystem->m_EmitterStandard.prtColor.blue)); // default as color yellow
	pColor->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_PTR_COLOR);

	CPropTreeItemSlider* pSlider;
	pSlider = (CPropTreeItemSlider*)m_PropertyTree.InsertItem(new CPropTreeItemSlider(), pPtrColorRoot);
	pSlider->SetLabelText(_T("Alpha"));
	pSlider->SetInfoText(_T("Particle Color Alpha"));
	pSlider->SetRange( 0, 255 );
	pSlider->SetItemValue((LPARAM) ((RwInt32)(pResourceParticleSystem->m_EmitterStandard.prtColor.alpha)) );
	pSlider->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_PTR_COLOR_A);


	// ============================================
	// Emitter Size
	// ============================================
	CPropTreeItem* pEmitterRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pEmitterRoot->Expand();
	pEmitterRoot->SetLabelText(_T("Emitter Size"));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEmitterRoot);	
	pEdit->SetLabelText(_T("X"));
	pEdit->SetInfoText(_T("Emitter Size X"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceParticleSystem->m_EmitterStandard.emtSize.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_EMITTED_SIZE_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEmitterRoot);	
	pEdit->SetLabelText(_T("Y"));
	pEdit->SetInfoText(_T("Emitter Size Y"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceParticleSystem->m_EmitterStandard.emtSize.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_EMITTED_SIZE_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEmitterRoot);	
	pEdit->SetLabelText(_T("Z"));
	pEdit->SetInfoText(_T("Emitter Size Z"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceParticleSystem->m_EmitterStandard.emtSize.z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_EMITTED_SIZE_Z);

	// ============================================
	// Emitted Particle Count
	// ============================================
	CPropTreeItem* pEmittedCountRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pEmittedCountRoot->Expand();
	pEmittedCountRoot->SetLabelText(_T("Emittter Particles"));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEmittedCountRoot);	
	pEdit->SetLabelText(_T("Count"));
	pEdit->SetInfoText(_T("Count"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);
	pEdit->SetItemValue((LPARAM)pResourceParticleSystem->m_EmitterStandard.emtPrtEmit);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_EMITTED_COUNT);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEmittedCountRoot);	
	pEdit->SetLabelText(_T("Count Bias"));
	pEdit->SetInfoText(_T("Count Bias"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);
	pEdit->SetItemValue((LPARAM)pResourceParticleSystem->m_EmitterStandard.emtPrtEmitBias);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_EMITTED_COUNT_BIAS);

	// ============================================
	// Emitted Gap
	// ============================================
	CPropTreeItem* pEmittedGapRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pEmittedGapRoot->Expand();
	pEmittedGapRoot->SetLabelText(_T("Emittter Gap"));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEmittedGapRoot);	
	pEdit->SetLabelText(_T("Gap"));
	pEdit->SetInfoText(_T("Gap"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceParticleSystem->m_EmitterStandard.emtEmitGap);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_EMITTED_GAP);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEmittedGapRoot);	
	pEdit->SetLabelText(_T("Gap Bias"));
	pEdit->SetInfoText(_T("Gap Bias"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceParticleSystem->m_EmitterStandard.emtEmitGapBias);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_EMITTED_GAP_BIAS);

	// ============================================
	// Particles Life
	// ============================================
	CPropTreeItem* pParticlesLifeRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pParticlesLifeRoot->Expand();
	pParticlesLifeRoot->SetLabelText(_T("Particles Life"));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pParticlesLifeRoot);	
	pEdit->SetLabelText(_T("Life"));
	pEdit->SetInfoText(_T("Life"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceParticleSystem->m_EmitterStandard.prtLife);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_LIFE);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pParticlesLifeRoot);	
	pEdit->SetLabelText(_T("Life Bias"));
	pEdit->SetInfoText(_T("Life Bias"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceParticleSystem->m_EmitterStandard.prtLifeBias);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_LIFE_BIAS);

	// ============================================
	// Particles Init Velocity
	// ============================================
	CPropTreeItem* pParticlesInitVelRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pParticlesInitVelRoot->Expand();
	pParticlesInitVelRoot->SetLabelText(_T("Init Velocity"));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pParticlesInitVelRoot);	
	pEdit->SetLabelText(_T("Velocity"));
	pEdit->SetInfoText(_T("Velocity"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceParticleSystem->m_EmitterStandard.prtInitVel);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_VELOCITY);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pParticlesInitVelRoot);	
	pEdit->SetLabelText(_T("Velocity Bias"));
	pEdit->SetInfoText(_T("Velocity Bias"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceParticleSystem->m_EmitterStandard.prtInitVelBias);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_VELOCITY_BIAS);

	// ============================================
	// Particles Init Direction
	// ============================================
	CPropTreeItem* pParticlesInitDirRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pParticlesInitDirRoot->Expand();
	pParticlesInitDirRoot->SetLabelText(_T("Init Direction"));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pParticlesInitDirRoot);	
	pEdit->SetLabelText(_T("Dir X"));
	pEdit->SetInfoText(_T("Dir X"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceParticleSystem->m_EmitterStandard.prtInitDir.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_DIRECTION_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pParticlesInitDirRoot);	
	pEdit->SetLabelText(_T("Dir Y"));
	pEdit->SetInfoText(_T("Dir Y"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceParticleSystem->m_EmitterStandard.prtInitDir.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_DIRECTION_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pParticlesInitDirRoot);	
	pEdit->SetLabelText(_T("Dir Z"));
	pEdit->SetInfoText(_T("Dir Z"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceParticleSystem->m_EmitterStandard.prtInitDir.z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_DIRECTION_Z);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pParticlesInitDirRoot);	
	pEdit->SetLabelText(_T("Dir Bias X"));
	pEdit->SetInfoText(_T("Dir Bias X"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceParticleSystem->m_EmitterStandard.prtInitDirBias.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_DIRECTION_BIAS_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pParticlesInitDirRoot);	
	pEdit->SetLabelText(_T("Dir Bias Y"));
	pEdit->SetInfoText(_T("Dir Bias Y"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceParticleSystem->m_EmitterStandard.prtInitDirBias.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_DIRECTION_BIAS_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pParticlesInitDirRoot);	
	pEdit->SetLabelText(_T("Dir Bias Z"));
	pEdit->SetInfoText(_T("Dir Bias Z"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceParticleSystem->m_EmitterStandard.prtInitDirBias.z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_DIRECTION_BIAS_Z);


	// ============================================
	// Force
	// ============================================
	CPropTreeItem* pForceRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pForceRoot->Expand();
	pForceRoot->SetLabelText(_T("Force"));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pForceRoot);	
	pEdit->SetLabelText(_T("X"));
	pEdit->SetInfoText(_T("X"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceParticleSystem->m_EmitterStandard.force.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_FORCE_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pForceRoot);	
	pEdit->SetLabelText(_T("Y"));
	pEdit->SetInfoText(_T("Y"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceParticleSystem->m_EmitterStandard.force.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_FORCE_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pForceRoot);	
	pEdit->SetLabelText(_T("Z"));
	pEdit->SetInfoText(_T("Z"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceParticleSystem->m_EmitterStandard.force.z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_FORCE_Z);

	// ============================================
	// Z Write
	// ============================================
	CPropTreeItemCombo* pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);
	pCombo->CreateComboBoxBool();
	pCombo->SetLabelText("Z Write");
	pCombo->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_ZWRITE);

	if(pResourceParticleSystem->IsEmitterDataFlag(NTLrpPRTSTDEMITTERDATAFLAGPRTZWRITE))
	{
		pCombo->SetItemValue(TRUE);
	}
	else
	{
		pCombo->SetItemValue(FALSE);
	}

	// Depth Bias
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);
	pEdit->SetLabelText("Depth Bias");
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceParticleSystem->m_fDepthBias);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_DEPTHBIAS);
}

//------------------------------------------------------------------
//	FuncName	: SetProperties_PrtEmitterEmitter
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_PrtEmitterEmitter(RpPrtAdvPrtEmitterEmitter* pPrtEmitterEmitter, CPropTreeItem* pRoot)
{
	CPropTreeItem* pEmitterRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pEmitterRoot->Expand();
	pEmitterRoot->SetLabelText(_T("Prt Emitter Emitter"));

	// ============================================
	// Attach System
	// ============================================
	CPropTreeItemCombo* pCombo;
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pEmitterRoot);	
	pCombo->SetLabelText(_T("Attach System"));
	pCombo->SetInfoText(_T("Attach System"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);

	RwInt32 index = 0;
	index = pCombo->AddString(_T("NONE"));
	pCombo->SetItemData(index, NULL);
	pCombo->SetItemValue(0);

	CNtlResourceEffect* pResourceEffect = CVenusVisualManager::GetInstance().m_pResourceEffect;
	NTL_ASSERTE(pResourceEffect);

	NTL_ASSERTE(CVenusVisualManager::GetInstance().m_pResourceSystem->GetSystemType() == CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_PARTICLE);
	CNtlResourceParticleSystem* pResourceParticleSystem = (CNtlResourceParticleSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem;

	CString strPropertiesTitle;
	CNtlResourceEffect::svdef_ResourceComponentSystem::iterator it;
	for (it = pResourceEffect->m_svResourceSystem.begin(); it != pResourceEffect->m_svResourceSystem.end(); ++ it)
	{
		CNtlResourceComponentSystem* pResourceSystem = (*it);
		// 자기 자신은 제외한다.
		if (_stricmp(pResourceSystem->GetSystemName(), pResourceParticleSystem->GetSystemName()) != 0)
		{
			if (pResourceSystem->GetSystemType() == CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_PARTICLE)
			{
				strPropertiesTitle.Format(_T(pResourceSystem->GetSystemName()));
				index = pCombo->AddString(_T(strPropertiesTitle));
				pCombo->SetItemData(index, (DWORD_PTR)pResourceSystem);
				if (!pResourceParticleSystem->m_strSubSystem.empty() && 
					_stricmp(pResourceParticleSystem->m_strSubSystem.c_str(), pResourceSystem->GetSystemName()) == 0)
				{
					pCombo->SetItemValue(index);
				}
			}
		}
	}
	pCombo->SetCtrlID(_PROPERTY_CONTROL_ACTION_PRT_EMITTER_EMITTER_SYSTEM);

	// ============================================
	// Time
	// ============================================
	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEmitterRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("time"));
	pEdit->SetInfoText(_T("time"));
	pEdit->SetItemValue((LPARAM)&pPrtEmitterEmitter->time);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_PRT_EMITTER_EMITTER_TIME);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEmitterRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("time Bias"));
	pEdit->SetInfoText(_T("time"));
	pEdit->SetItemValue((LPARAM)&pPrtEmitterEmitter->timeBias);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_PRT_EMITTER_EMITTER_TIME_BIAS);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEmitterRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("timeGap"));
	pEdit->SetInfoText(_T("timeGap"));
	pEdit->SetItemValue((LPARAM)&pPrtEmitterEmitter->timeGap);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_PRT_EMITTER_EMITTER_TIMEGAP);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEmitterRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("timeGap Bias"));
	pEdit->SetInfoText(_T("timeGap Bias"));
	pEdit->SetItemValue((LPARAM)&pPrtEmitterEmitter->timeGapBias);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_PRT_EMITTER_EMITTER_TIMEGAP_BIAS);
}

//------------------------------------------------------------------
//	FuncName	: SetProperties_EmitterPointList
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_EmitterPointList(RpPrtAdvEmtPointList* pEmitterPointList, CPropTreeItem* pRoot)
{
	CPropTreeItem* pPointListRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pPointListRoot->Expand();
	pPointListRoot->SetLabelText(_T("Emitter Point List"));

	CString strPropertiesTitle;

	CPropTreeItemCombo* pCombo;
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pPointListRoot);	
	pCombo->SetLabelText(_T("use Direction"));
	pCombo->SetInfoText(_T("use Direction"));
	pCombo->CreateComboBoxBool();
	pCombo->SetItemValue(pEmitterPointList->useDirection);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_POINTLIST_USE_DIRECTION);

	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pPointListRoot);	
	pCombo->SetLabelText(_T("random"));
	pCombo->SetInfoText(_T("random"));
	pCombo->CreateComboBoxBool();
	pCombo->SetItemValue(pEmitterPointList->random);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_POINTLIST_RANDOM);

	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pPointListRoot);	
	pEdit->SetLabelText(_T("Num Point"));
	pEdit->SetInfoText(_T("Num Point"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);
	pEdit->SetItemValue((LPARAM)pEmitterPointList->numPoint);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_POINTLIST_NUM_POINT);

	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pPointListRoot);	
	pCombo->SetLabelText(_T("Current Index"));
	pCombo->SetInfoText(_T("Current Index"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
	for (RwUInt32 i = 0; i < pEmitterPointList->numPoint; ++i)
	{
		strPropertiesTitle.Format(_T("List %02d"), i);
		pCombo->AddString(_T(strPropertiesTitle));
		pCombo->SetItemData(i, i);
	}
	pCombo->SetItemValue(m_nCurrentPoint);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_POINTLIST_CURRENT_INDEX);
	NTL_ASSERTE(m_nCurrentPoint < pEmitterPointList->numPoint);

	// ============================================
	// Point List
	// ============================================
	CPropTreeItem* pPointListListRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pPointListRoot);		
	pPointListListRoot->Expand();
	strPropertiesTitle.Format(_T("Point List : %02d"), m_nCurrentPoint);
	pPointListListRoot->SetLabelText(_T(strPropertiesTitle));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pPointListListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("X"));
	pEdit->SetInfoText(_T("X"));
	pEdit->SetItemValue((LPARAM)&pEmitterPointList->pointList[m_nCurrentPoint].x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_POINTLIST_POINT_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pPointListListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Y"));
	pEdit->SetInfoText(_T("Y"));
	pEdit->SetItemValue((LPARAM)&pEmitterPointList->pointList[m_nCurrentPoint].y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_POINTLIST_POINT_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pPointListListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Z"));
	pEdit->SetInfoText(_T("Z"));
	pEdit->SetItemValue((LPARAM)&pEmitterPointList->pointList[m_nCurrentPoint].z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_POINTLIST_POINT_Z);

	// ============================================
	// Direction List
	// ============================================
	CPropTreeItem* pDirectionListListRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pPointListRoot);		
	pDirectionListListRoot->Expand();
	strPropertiesTitle.Format(_T("Direction List : %02d"), m_nCurrentPoint);
	pDirectionListListRoot->SetLabelText(_T(strPropertiesTitle));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pDirectionListListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("X"));
	pEdit->SetInfoText(_T("X"));
	pEdit->SetItemValue((LPARAM)&pEmitterPointList->directionList[m_nCurrentPoint].x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_POINTLIST_DIRECTION_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pDirectionListListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Y"));
	pEdit->SetInfoText(_T("Y"));
	pEdit->SetItemValue((LPARAM)&pEmitterPointList->directionList[m_nCurrentPoint].y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_POINTLIST_DIRECTION_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pDirectionListListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Z"));
	pEdit->SetInfoText(_T("Z"));
	pEdit->SetItemValue((LPARAM)&pEmitterPointList->directionList[m_nCurrentPoint].z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_POINTLIST_DIRECTION_Z);
}

//------------------------------------------------------------------
//	FuncName	: SetProperties_CircleEmitter
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_CircleEmitter(RpPrtAdvCircleEmitter* pCircleEmitter, CPropTreeItem* pRoot)
{
	CPropTreeItem* pCircleRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pCircleRoot->Expand();
	pCircleRoot->SetLabelText(_T("Circle Emitter"));

	CPropTreeItemCombo* pCombo;
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pCircleRoot);	
	pCombo->SetLabelText(_T("useCircleEmission"));
	pCombo->SetInfoText(_T("useCircleEmission"));
	pCombo->CreateComboBoxBool();
	pCombo->SetItemValue(pCircleEmitter->useCircleEmission);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_ACTION_CIRCLE_EMITTER_USE_EMISSION);

	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pCircleRoot);	
	pEdit->SetLabelText(_T("Radius"));
	pEdit->SetInfoText(_T("Radius"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pCircleEmitter->radius);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_CIRCLE_EMITTER_RAIDUS);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pCircleRoot);	
	pEdit->SetLabelText(_T("Radius Gap"));
	pEdit->SetInfoText(_T("Radius Gap"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pCircleEmitter->radiusGap);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_CIRCLE_EMITTER_RAIDUS_GAP);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pCircleRoot);	
	pEdit->SetLabelText(_T("Height"));
	pEdit->SetInfoText(_T("Height"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pCircleEmitter->height);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_CIRCLE_EMITTER_HEIGHT);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pCircleRoot);	
	pEdit->SetLabelText(_T("dir Ratation"));
	pEdit->SetInfoText(_T("dir Ratation"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pCircleEmitter->dirRotation);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_CIRCLE_EMITTER_DIR_ROTATION);
}

//------------------------------------------------------------------
//	FuncName	: SetProperties_SphereEmitter
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_SphereEmitter(RpPrtAdvSphereEmitter* pSphereEmitter, CPropTreeItem* pRoot)
{
	CPropTreeItem* pSphereRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pSphereRoot->Expand();
	pSphereRoot->SetLabelText(_T("Sphere Emitter"));

	CPropTreeItemCombo* pCombo;
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pSphereRoot);	
	pCombo->SetLabelText(_T("useSphereEmission"));
	pCombo->SetInfoText(_T("useSphereEmission"));
	pCombo->CreateComboBoxBool();
	pCombo->SetItemValue(pSphereEmitter->useSphereEmission);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_ACTION_SPHERE_EMITTER_USE_EMISSION);

	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pSphereRoot);	
	pEdit->SetLabelText(_T("Radius"));
	pEdit->SetInfoText(_T("Radius"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pSphereEmitter->radius);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_SPHERE_EMITTER_RAIDUS);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pSphereRoot);	
	pEdit->SetLabelText(_T("Radius Gap"));
	pEdit->SetInfoText(_T("Radius Gap"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pSphereEmitter->radiusGap);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_SPHERE_EMITTER_RAIDUS_GAP);
}


/**
 * Line System 전용 Sphere Emitter
 * \param pSphereEmitter Sphere Emitter 정보를 담고있는 구조체의 포인터
 * \param pRoot 트리 컨트롤의 루트 아이템 
 */
void CEffectPropertyDialog::SetProperties_SphereEmitter( SNtlPrtStdEmitterPrtSphere* pSphereEmitter, CPropTreeItem* pRoot ) 
{
    CPropTreeItem* pSphereRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);
    pSphereRoot->Expand();
    pSphereRoot->SetLabelText(_T("Sphere Emitter"));

    CPropTreeItemEdit* pSphereX;
    pSphereX = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pSphereRoot);
    pSphereX->SetLabelText(_T("X Size"));
    pSphereX->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pSphereX->SetItemValue((LPARAM)&pSphereEmitter->m_vEmitterSize.x);
    pSphereX->SetCtrlID(_PROPERTY_CONTROL_ACTION_SPHERE_EMITTER_RAIDUS_X);

    CPropTreeItemEdit* pSphereY;
    pSphereY = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pSphereRoot);
    pSphereY->SetLabelText(_T("Y Size"));
    pSphereY->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pSphereY->SetItemValue((LPARAM)&pSphereEmitter->m_vEmitterSize.y);
    pSphereY->SetCtrlID(_PROPERTY_CONTROL_ACTION_SPHERE_EMITTER_RAIDUS_Y);

    CPropTreeItemEdit* pSphereZ;
    pSphereZ = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pSphereRoot);
    pSphereZ->SetLabelText(_T("Z Size"));
    pSphereZ->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pSphereZ->SetItemValue((LPARAM)&pSphereEmitter->m_vEmitterSize.z);
    pSphereZ->SetCtrlID(_PROPERTY_CONTROL_ACTION_SPHERE_EMITTER_RAIDUS_Z);

}

//------------------------------------------------------------------
//	FuncName	: SetProperties_EmitterPrtColor
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_EmitterPrtColor(RpPrtStdEmitterPrtColor* pEmitterPrtColor, CPropTreeItem* pRoot)
{
	CPropTreeItem* pStartColorRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pStartColorRoot->Expand();
	pStartColorRoot->SetLabelText(_T("Start Emitter Color"));

	CPropTreeItemColor* pColor;
	pColor = (CPropTreeItemColor*)m_PropertyTree.InsertItem(new CPropTreeItemColor(), pStartColorRoot);
	pColor->SetLabelText(_T("Start Color"));
	pColor->SetItemValue((LPARAM)RGB(pEmitterPrtColor->prtStartCol.red, pEmitterPrtColor->prtStartCol.green, pEmitterPrtColor->prtStartCol.blue)); // default as color yellow
	pColor->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_COLOR_START_COLOR);

	CPropTreeItemSlider* pSlider;
	pSlider = (CPropTreeItemSlider*)m_PropertyTree.InsertItem(new CPropTreeItemSlider(), pStartColorRoot);
	pSlider->SetLabelText(_T("Color Alpha"));
	pSlider->SetInfoText(_T("Color Alpha"));
	pSlider->SetRange( 0, 255 );
	pSlider->SetItemValue((LPARAM) ((RwInt32)(pEmitterPrtColor->prtStartCol.alpha)) );
	pSlider->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_COLOR_START_COLOR_A);

	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStartColorRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Color Bias R"));
	pEdit->SetInfoText(_T("Color Bias R"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtColor->prtStartColBias.red);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_COLOR_START_COLOR_BIAS_R);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStartColorRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Color Bias G"));
	pEdit->SetInfoText(_T("Color Bias G"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtColor->prtStartColBias.green);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_COLOR_START_COLOR_BIAS_G);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStartColorRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Color Bias B"));
	pEdit->SetInfoText(_T("Color Bias B"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtColor->prtStartColBias.blue);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_COLOR_START_COLOR_BIAS_B);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStartColorRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Color Bias A"));
	pEdit->SetInfoText(_T("Color Bias A"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtColor->prtStartColBias.alpha);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_COLOR_START_COLOR_BIAS_A);


	CPropTreeItem* pEndColorRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pEndColorRoot->Expand();
	pEndColorRoot->SetLabelText(_T("End Emitter Color"));

	pColor = (CPropTreeItemColor*)m_PropertyTree.InsertItem(new CPropTreeItemColor(), pEndColorRoot);
	pColor->SetLabelText(_T("Color"));
	pColor->SetItemValue((LPARAM)RGB(pEmitterPrtColor->prtEndCol.red, pEmitterPrtColor->prtEndCol.green, pEmitterPrtColor->prtEndCol.blue));
	pColor->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_COLOR_END_COLOR);

	pSlider = (CPropTreeItemSlider*)m_PropertyTree.InsertItem(new CPropTreeItemSlider(), pEndColorRoot);
	pSlider->SetLabelText(_T("Color Alpha"));
	pSlider->SetInfoText(_T("Color Alpha"));
	pSlider->SetRange( 0, 255 );
	pSlider->SetItemValue((LPARAM) ((RwInt32)(pEmitterPrtColor->prtEndCol.alpha)) );
	pSlider->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_COLOR_END_COLOR_A);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEndColorRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Color Bias R"));
	pEdit->SetInfoText(_T("Color Bias R"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtColor->prtEndColBias.red);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_COLOR_END_COLOR_BIAS_R);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEndColorRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Color Bias G"));
	pEdit->SetInfoText(_T("Color Bias G"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtColor->prtEndColBias.green);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_COLOR_END_COLOR_BIAS_R);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEndColorRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Color Bias B"));
	pEdit->SetInfoText(_T("Color Bias B"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtColor->prtEndColBias.blue);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_COLOR_END_COLOR_BIAS_R);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEndColorRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Color Bias A"));
	pEdit->SetInfoText(_T("Color Bias A"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtColor->prtEndColBias.alpha);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_COLOR_END_COLOR_BIAS_R);
}

//------------------------------------------------------------------
//	FuncName	: SetProperties_EmitterPrtTexCoords
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_EmitterPrtTexCoords(RpPrtStdEmitterPrtTexCoords* pEmitterPrtTexCoords, CPropTreeItem* pRoot)
{
	CPropTreeItem* pStartTexCoords = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pStartTexCoords->Expand();
	pStartTexCoords->SetLabelText(_T("Start TexCoords"));

	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStartTexCoords);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Start0 U"));
	pEdit->SetInfoText(_T("Start0 U"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtTexCoords->prtStartUV0.u);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV0_START_U);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStartTexCoords);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Start0 V"));
	pEdit->SetInfoText(_T("Start0 V"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtTexCoords->prtStartUV0.v);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV0_START_V);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStartTexCoords);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Start0 Bias U"));
	pEdit->SetInfoText(_T("Start0 Bias U"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtTexCoords->prtStartUV0Bias.u);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV0_START_BIAS_U);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStartTexCoords);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Start0 Bias V"));
	pEdit->SetInfoText(_T("Start0 Bias V"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtTexCoords->prtStartUV0Bias.v);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV0_START_BIAS_V);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStartTexCoords);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Start1 U"));
	pEdit->SetInfoText(_T("Start1 U"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtTexCoords->prtStartUV1.u);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV1_START_U);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStartTexCoords);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Start1 V"));
	pEdit->SetInfoText(_T("Start1 V"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtTexCoords->prtStartUV1.v);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV1_START_V);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStartTexCoords);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Start1 Bias U"));
	pEdit->SetInfoText(_T("Start1 Bias U"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtTexCoords->prtStartUV1Bias.u);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV1_START_BIAS_U);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStartTexCoords);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Start1 Bias V"));
	pEdit->SetInfoText(_T("Start1 Bias V"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtTexCoords->prtStartUV1Bias.v);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV1_START_BIAS_V);


	CPropTreeItem* pEndTexCoords = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pEndTexCoords->Expand();
	pEndTexCoords->SetLabelText(_T("End TexCoords"));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEndTexCoords);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("End0 U"));
	pEdit->SetInfoText(_T("End0 U"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtTexCoords->prtEndUV0.u);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV0_END_U);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEndTexCoords);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("End0 V"));
	pEdit->SetInfoText(_T("End0 V"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtTexCoords->prtEndUV0.v);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV0_END_V);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEndTexCoords);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("End0 Bias U"));
	pEdit->SetInfoText(_T("End0 Bias U"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtTexCoords->prtEndUV0Bias.u);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV0_END_BIAS_U);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEndTexCoords);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("End0 Bias V"));
	pEdit->SetInfoText(_T("End0 Bias V"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtTexCoords->prtEndUV0Bias.v);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV0_END_BIAS_V);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEndTexCoords);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("End1 U"));
	pEdit->SetInfoText(_T("End1 U"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtTexCoords->prtEndUV1.u);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV1_END_U);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEndTexCoords);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("End1 V"));
	pEdit->SetInfoText(_T("End1 V"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtTexCoords->prtEndUV1.v);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV1_END_V);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEndTexCoords);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("End1 Bias U"));
	pEdit->SetInfoText(_T("End1 Bias U"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtTexCoords->prtEndUV1Bias.u);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV1_END_BIAS_U);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEndTexCoords);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("End1 Bias V"));
	pEdit->SetInfoText(_T("End1 Bias V"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtTexCoords->prtEndUV1Bias.v);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV1_END_BIAS_V);
}

//------------------------------------------------------------------
//	FuncName	: SetProperties_EmitterPrt2DRotate
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_EmitterPrt2DRotate(RpPrtStdEmitterPrt2DRotate* pEmitterPrt2DRotate, CPropTreeItem* pRoot)
{
	CPropTreeItem* pStart2DRotateRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pStart2DRotateRoot->Expand();
	pStart2DRotateRoot->SetLabelText(_T("Emitter Start 2DRotate"));

	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStart2DRotateRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Rotate"));
	pEdit->SetInfoText(_T("Rotate"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrt2DRotate->prtStart2DRotate);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_START);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStart2DRotateRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Rotate Bias"));
	pEdit->SetInfoText(_T("Rotate Bias"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrt2DRotate->prtStart2DRotateBias);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_START_BIAS);

	CPropTreeItem* pEnd2DRotateRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pEnd2DRotateRoot->Expand();
	pEnd2DRotateRoot->SetLabelText(_T("Emitter End 2DRotate"));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEnd2DRotateRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Rotate"));
	pEdit->SetInfoText(_T("Rotate"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrt2DRotate->prtEnd2DRotate);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_END);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEnd2DRotateRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Rotate Bias"));
	pEdit->SetInfoText(_T("Rotate Bias"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrt2DRotate->prtEnd2DRotateBias);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_END_BIAS);
}

//------------------------------------------------------------------
//	FuncName	: SetProperties_EmitterPrt2DRotate
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_EmitterPrt2DRotate(SNtlPrtStdEmitterPrtRotate* pEmitterPrt2DRotate, CPropTreeItem* pRoot)
{
	CPropTreeItem* pStart2DRotateRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pStart2DRotateRoot->Expand();
	pStart2DRotateRoot->SetLabelText(_T("Mesh Start Rotate"));

	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStart2DRotateRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("X"));
	pEdit->SetInfoText(_T("Mesh Start Rotate X"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrt2DRotate->prtStartRotate.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_START_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStart2DRotateRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Y"));
	pEdit->SetInfoText(_T("Mesh Start Rotate Y"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrt2DRotate->prtStartRotate.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_START_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStart2DRotateRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Z"));
	pEdit->SetInfoText(_T("Mesh Start Rotate Z"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrt2DRotate->prtStartRotate.z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_START_Z);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStart2DRotateRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Bias X"));
	pEdit->SetInfoText(_T("Mesh Start Rotate Bias X"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrt2DRotate->prtStartRotateBias.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_START_BIAS_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStart2DRotateRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Bias Y"));
	pEdit->SetInfoText(_T("Mesh Start Rotate Bias Y"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrt2DRotate->prtStartRotateBias.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_START_BIAS_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStart2DRotateRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Bias Z"));
	pEdit->SetInfoText(_T("Mesh Start Rotate Bias Z"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrt2DRotate->prtStartRotateBias.z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_START_BIAS_Z);


	// ============================================
	// End Rotate
	// ============================================
	CPropTreeItem* pEnd2DRotateRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pEnd2DRotateRoot->Expand();
	pEnd2DRotateRoot->SetLabelText(_T("Mesh End Rotate"));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEnd2DRotateRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("X"));
	pEdit->SetInfoText(_T("Mesh End Rotate X"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrt2DRotate->prtEndRotate.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_END_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEnd2DRotateRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Y"));
	pEdit->SetInfoText(_T("Mesh End Rotate Y"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrt2DRotate->prtEndRotate.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_END_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEnd2DRotateRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Z"));
	pEdit->SetInfoText(_T("Mesh End Rotate Z"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrt2DRotate->prtEndRotate.z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_END_Z);

	// ============================================
	// End Rotate Bias
	// ============================================
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEnd2DRotateRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Bias X"));
	pEdit->SetInfoText(_T("Mesh End Rotate Bias X"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrt2DRotate->prtEndRotateBias.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_END_BIAS_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEnd2DRotateRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Bias Y"));
	pEdit->SetInfoText(_T("Mesh End Rotate Bias Y"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrt2DRotate->prtEndRotateBias.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_END_BIAS_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEnd2DRotateRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Bias Z"));
	pEdit->SetInfoText(_T("Mesh End Rotate Bias Z"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrt2DRotate->prtEndRotateBias.z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_END_BIAS_Z);
}

//------------------------------------------------------------------
//	FuncName	: SetProperties_EmitterPrtSize
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_EmitterPrtSize(RpPrtStdEmitterPrtSize* pEmitterPrtSize, CPropTreeItem* pRoot)
{
	CPropTreeItem* pStartSizeRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pStartSizeRoot->Expand();
	pStartSizeRoot->SetLabelText(_T("Start Emitter Size"));

	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStartSizeRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size X"));
	pEdit->SetInfoText(_T("Size X"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtSize->prtStartSize.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_START_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStartSizeRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size Y"));
	pEdit->SetInfoText(_T("Size Y"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtSize->prtStartSize.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_START_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStartSizeRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size Bias X"));
	pEdit->SetInfoText(_T("Size Bias X"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtSize->prtStartSizeBias.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_START_BIAS_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStartSizeRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size Bias Y"));
	pEdit->SetInfoText(_T("Size Bias Y"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtSize->prtStartSizeBias.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_START_BIAS_Y);


	CPropTreeItem* pEndSizeRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pEndSizeRoot->Expand();
	pEndSizeRoot->SetLabelText(_T("End Emitter Size"));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEndSizeRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size X"));
	pEdit->SetInfoText(_T("Size X"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtSize->prtEndSize.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_END_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEndSizeRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size Y"));
	pEdit->SetInfoText(_T("Size Y"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtSize->prtEndSize.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_END_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEndSizeRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size Bias X"));
	pEdit->SetInfoText(_T("Size Bias X"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtSize->prtEndSizeBias.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_END_BIAS_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEndSizeRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size Bias Y"));
	pEdit->SetInfoText(_T("Size Bias Y"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtSize->prtEndSizeBias.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_END_BIAS_Y);
}

//------------------------------------------------------------------
//	FuncName	: SetProperties_EmitterPrtSize
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_EmitterPrtSize(SNtlPrtStdEmitterPrtSize* pEmitterPrtSize, CPropTreeItem* pRoot)
{
	CPropTreeItem* pStartSizeRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pStartSizeRoot->Expand();
	pStartSizeRoot->SetLabelText(_T("Start Emitter Size"));

	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStartSizeRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size X"));
	pEdit->SetInfoText(_T("Size X"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtSize->prtStartSize.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_START_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStartSizeRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size Y"));
	pEdit->SetInfoText(_T("Size Y"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtSize->prtStartSize.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_START_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStartSizeRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size Z"));
	pEdit->SetInfoText(_T("Size Z"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtSize->prtStartSize.z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_START_Z);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStartSizeRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size Bias X"));
	pEdit->SetInfoText(_T("Size Bias X"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtSize->prtStartSizeBias.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_START_BIAS_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStartSizeRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size Bias Y"));
	pEdit->SetInfoText(_T("Size Bias Y"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtSize->prtStartSizeBias.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_START_BIAS_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pStartSizeRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size Bias Z"));
	pEdit->SetInfoText(_T("Size Bias Z"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtSize->prtStartSizeBias.z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_START_BIAS_Z);


	CPropTreeItem* pEndSizeRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pEndSizeRoot->Expand();
	pEndSizeRoot->SetLabelText(_T("End Emitter Size"));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEndSizeRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size X"));
	pEdit->SetInfoText(_T("Size X"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtSize->prtEndSize.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_END_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEndSizeRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size Y"));
	pEdit->SetInfoText(_T("Size Y"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtSize->prtEndSize.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_END_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEndSizeRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size Z"));
	pEdit->SetInfoText(_T("Size Z"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtSize->prtEndSize.z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_END_Z);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEndSizeRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size Bias X"));
	pEdit->SetInfoText(_T("Size Bias X"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtSize->prtEndSizeBias.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_END_BIAS_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEndSizeRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size Bias Y"));
	pEdit->SetInfoText(_T("Size Bias Y"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtSize->prtEndSizeBias.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_END_BIAS_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pEndSizeRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size Bias Z"));
	pEdit->SetInfoText(_T("Size Bias Z"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtSize->prtEndSizeBias.z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_END_BIAS_Z);
}


//------------------------------------------------------------------
//	FuncName	: SetProperties_EmitterPrtMatrix
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_EmitterPrtMatrix(RpPrtStdEmitterPrtMatrix* pEmitterPrtMatrix, CPropTreeItem* pRoot, RwUInt32 nEmitterFlag)
{
    //NOTE : LookAt, LookUp Vector를 가지고 Rotation 으로 변경한다.
    // Rotation 값
    //RwV3d vRotAngle = API_GetAngle(pEmitterPrtMatrix->prtPosMtxUp, pEmitterPrtMatrix->prtPosMtxAt);
    RwV3d vRotAngle = pEmitterPrtMatrix->prtCnsMtx.pos;
    CPropTreeItem* pRotateRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
    pRotateRoot->Expand();
    pRotateRoot->SetLabelText("Rotate");

    CPropTreeItemSlider* pSlider;
    pSlider = (CPropTreeItemSlider*)m_PropertyTree.InsertItem(new CPropTreeItemSlider(), pRotateRoot);
    pSlider->SetLabelText(_T("X"));
    pSlider->SetInfoText(_T("World Rotate X"));
    pSlider->SetRange(0, 360);
    pSlider->SetColors(RGB(255, 255, 255), RGB(255, 0, 0));    
    pSlider->SetItemValue((LPARAM)(RwInt32)(vRotAngle.x));
    pSlider->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_MATRIX_ROTATION_X);

    pSlider = (CPropTreeItemSlider*)m_PropertyTree.InsertItem(new CPropTreeItemSlider(), pRotateRoot);
    pSlider->SetLabelText(_T("Y"));
    pSlider->SetInfoText(_T("World Rotate Y"));
    pSlider->SetRange(0, 360);
    pSlider->SetColors(RGB(255, 255, 255), RGB(0, 255, 0));
    pSlider->SetItemValue((LPARAM)(RwInt32)(vRotAngle.y));
    pSlider->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_MATRIX_ROTATION_Y);

    pSlider = (CPropTreeItemSlider*)m_PropertyTree.InsertItem(new CPropTreeItemSlider(), pRotateRoot);
    pSlider->SetLabelText(_T("Z"));
    pSlider->SetInfoText(_T("World Rotate Z"));
    pSlider->SetRange(0, 360);
    pSlider->SetColors(RGB(255, 255, 255), RGB(0, 0, 255));
    pSlider->SetItemValue((LPARAM)(RwInt32)(vRotAngle.z));
    pSlider->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_MATRIX_ROTATION_Z);

    // BillBoard 설정   
    CPropTreeItemCombo* pCombo;
    pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRotateRoot);
    pCombo->SetLabelText("BillBoard Setting");
    pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
    pCombo->AddString("None");
    pCombo->AddString("Y BillBoard");
    pCombo->AddString("X BillBoard");
    pCombo->AddString("Z BillBoard");
    for(int i = 0; i < 4; ++i)
    {
        pCombo->SetItemData(i, i);
    }
    
    if(nEmitterFlag & NTLrpPRTSTDEMITTERDATAFLAGPRTYBILLBOARD)
    {
        pCombo->SetItemValue(BILLBOARD_Y);

    }
    else if(nEmitterFlag & NTLrpPRTSTDEMITTERDATAFLAGPRTXBILLBOARD)
    {
        pCombo->SetItemValue(BILLBOARD_X);

    }    
    else if(nEmitterFlag & NTLrpPRTSTDEMITTERDATAFLAGPRTZBILLBOARD)
    {
        pCombo->SetItemValue(BILLBOARD_Z);
    }
    else 
    {
        pCombo->SetItemValue(BILLBOARD_NONE);
    }
    pCombo->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_MATRIX_BILLBOARD);

}

//------------------------------------------------------------------
//	FuncName	: SetProperties_EmitterPrtHurricane
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_EmitterPrtHurricane(SNtlPrtStdEmitterPrtHurricane* pEmitterPrtHurricane, CPropTreeItem* pRoot)
{
	CPropTreeItem* pHurricaneRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pHurricaneRoot->Expand();
	pHurricaneRoot->SetLabelText(_T("Hurricane"));

	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pHurricaneRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Speed"));
	pEdit->SetInfoText(_T("Hurricane Speed"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtHurricane->fSpeed);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_HURRICANE_SPEED);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pHurricaneRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("radius"));
	pEdit->SetInfoText(_T("Hurricane radius"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtHurricane->radius);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_HURRICANE_RADIUS);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pHurricaneRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("revolutionAngle"));
	pEdit->SetInfoText(_T("Hurricane revolutionAngle"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtHurricane->revolutionAngle);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_HURRICANE_REVOLUTION_ANGLE);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pHurricaneRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("revolutionCount"));
	pEdit->SetInfoText(_T("Hurricane revolutionCount"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtHurricane->revolutionCount);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_HURRICANE_REVOLUTION_COUNT);
}

//------------------------------------------------------------------
//	FuncName	: SetProperties_EmitterPrtHurricane
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_EmitterPrtOrbitPoint(SNtlPrtStdEmitterPrtOrbitPoint* pEmitterPrtOrbitPoint, CPropTreeItem* pRoot)
{
	CPropTreeItem* pOrbitPointRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pOrbitPointRoot->Expand();
	pOrbitPointRoot->SetLabelText(_T("Orbit Point"));

	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pOrbitPointRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("X"));
	pEdit->SetInfoText(_T("Orbit Point X"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtOrbitPoint->point.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_ORBITPOINT_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pOrbitPointRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Y"));
	pEdit->SetInfoText(_T("Orbit Point y"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtOrbitPoint->point.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_ORBITPOINT_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pOrbitPointRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Z"));
	pEdit->SetInfoText(_T("Orbit Point z"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtOrbitPoint->point.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_ORBITPOINT_Z);
}

void CEffectPropertyDialog::SetProperties_EmitterPrtFadeOut(SNtlPrtStdEmitterPrtFadeOut* pEmitterPrtFadeOut, CPropTreeItem* pRoot)
{
	CPropTreeItem* pFadeOutRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pFadeOutRoot->Expand();
	pFadeOutRoot->SetLabelText(_T("FadeOut"));

	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pFadeOutRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("time"));
	pEdit->SetInfoText(_T("Fade Out time"));
	pEdit->SetItemValue((LPARAM)&pEmitterPrtFadeOut->time);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_FADE_OUT_TIME);

	CPropTreeItemCombo* pCombo;
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pFadeOutRoot);	
	pCombo->SetLabelText(_T("follow"));
	pCombo->SetInfoText(_T("Fade Out follow"));
	pCombo->CreateComboBoxBool();
	pCombo->SetItemValue(pEmitterPrtFadeOut->follow);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_FADE_OUT_FOLLOW);

	// 
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pFadeOutRoot);	
	pCombo->SetLabelText(_T("System"));
	pCombo->SetInfoText(_T("follow System"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);

	RwInt32 index = 0;
	index = pCombo->AddString(_T("NONE"));
	pCombo->SetItemData(index, NULL);
	pCombo->SetItemValue(0);

	CNtlResourceEffect* pResourceEffect = CVenusVisualManager::GetInstance().m_pResourceEffect;
	NTL_ASSERTE(pResourceEffect);

	CNtlResourceComponentSystem* pResourceComponentSystem = (CNtlResourceComponentSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem;
	NTL_ASSERTE(pResourceComponentSystem);

	CString strPropertiesTitle;
	CNtlResourceEffect::svdef_ResourceComponentSystem::iterator it;
	for (it = pResourceEffect->m_svResourceSystem.begin(); it != pResourceEffect->m_svResourceSystem.end(); ++ it)
	{
		CNtlResourceComponentSystem* pResourceSystem = (*it);
		// 자기 자신은 제외한다.
		if (_stricmp(pResourceSystem->GetSystemName(), pResourceComponentSystem->GetSystemName()) != 0)
		{
			strPropertiesTitle.Format(_T(pResourceSystem->GetSystemName()));
			index = pCombo->AddString(_T(strPropertiesTitle));
			pCombo->SetItemData(index, (DWORD_PTR)pResourceSystem);
			if (_stricmp(pResourceComponentSystem->m_EmitterPrtFadeOut.strFollowSystem, pResourceSystem->GetSystemName()) == 0)
				pCombo->SetItemValue(index);
		}
	}
	pCombo->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_FADE_OUT_FOLLOW_SYSTEM);
}

void CEffectPropertyDialog::SetProperties_EmitterPrtFinish( SNtlPrtStdEmitterPrtFinish* pEmitterPrtFinish, CPropTreeItem* pRoot ) 
{
    CPropTreeItem* pFinishRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
    pFinishRoot->Expand();
    pFinishRoot->SetLabelText(_T("FadeOut"));

    CPropTreeItemEdit* pEdit;
    CPropTreeItemCombo* pCombo;

    pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pFinishRoot);
    pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pEdit->SetLabelText(_T("Finish Time"));
    pEdit->SetItemValue((LPARAM)&pEmitterPrtFinish->m_fFinishTime);
    pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_FINISH_TIME);    

    pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pFinishRoot);
    pCombo->SetLabelText(_T("Alpha Finish"));
    pCombo->CreateComboBoxBool();
    pCombo->SetItemValue(pEmitterPrtFinish->m_nFinishTypeFlag & FINISH_TYPE_ALPHA ? 1 : 0);
    pCombo->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_FINISH_ALPHA);

    pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pFinishRoot);
    pCombo->SetLabelText(_T("Size Finish"));
    pCombo->CreateComboBoxBool();
    pCombo->SetItemValue((RwBool)pEmitterPrtFinish->m_nFinishTypeFlag & FINISH_TYPE_SIZE ? 1 : 0);
    pCombo->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_FINISH_SIZE);

    pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pFinishRoot);
    pCombo->SetLabelText(_T("Follow Finish"));
    pCombo->CreateComboBoxBool();
    pCombo->SetItemValue((RwBool)pEmitterPrtFinish->m_nFinishTypeFlag & FINISH_TYPE_FOLLOW ? 1 : 0);
    pCombo->SetCtrlID(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_FINISH_FOLLOW);
}


void CEffectPropertyDialog::SetProperties_EmitterUVAnim( SNtlPrtStdEmitterPrtUVAnim* pEmitterUVAnim, CPropTreeItem* pRoot ) 
{
	CPropTreeItem* pUVAnimRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);
	pUVAnimRoot->Expand();
	pUVAnimRoot->SetLabelText(_T("UV Animation"));

	CPropTreeItemEdit* pEdit;
	CPropTreeItemSlider* pSlider;

	// Dir
	RwV3d vAt;
	vAt.x = pEmitterUVAnim->vDir.x;
	vAt.y = 0.0f;
	vAt.z = pEmitterUVAnim->vDir.z;
	RwV3dNormalize(&vAt, &vAt);
	RwReal fDegree = CNtlMath::LineToAngleY(&vAt);
	if(fDegree < 0.0f)
	{
		fDegree += 360.0f;
	}

	pSlider = (CPropTreeItemSlider*)m_PropertyTree.InsertItem(new CPropTreeItemSlider(), pUVAnimRoot);
	pSlider->SetLabelText(_T("Direction"));
	pSlider->SetRange(0, 360);
	pSlider->SetItemValue((LPARAM)(RwInt32)fDegree);
	pSlider->SetCtrlID(_PROPERTY_CONTROL_ACTION_UVANIM_DIR);

	// Velocity
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pUVAnimRoot);
	pEdit->SetLabelText(_T("Velocity"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pEmitterUVAnim->fVelocity);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_UVANIM_VELOCITY);

	// Max Offset
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pUVAnimRoot);
	pEdit->SetLabelText(_T("Max Offset"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pEmitterUVAnim->fMaxOffset);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_UVANIM_OFFSET);
}

void CEffectPropertyDialog::SetProperties_EmitterTrail( SNtlPrtStdEmitterTrail* pEmitterTrail, CPropTreeItem* pRoot ) 
{
    CPropTreeItem* pTrailRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);
    pTrailRoot->Expand();
    pTrailRoot->SetLabelText("Trail Action");

    CPropTreeItemEdit* pEdit;
    CPropTreeItemCombo* pCombo;

    // Edge Life Time
    pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pTrailRoot);
    pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pEdit->SetLabelText(_T("Edge Life Time"));
    pEdit->SetItemValue((LPARAM)&pEmitterTrail->fEdgeLifeTime);
    pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_TRAIL_EDGE_LIFE_TIME);

    // Max Length
    pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pTrailRoot);
    pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pEdit->SetLabelText(_T("Max Length"));
    pEdit->SetItemValue((LPARAM)&pEmitterTrail->fMaxLength);
    pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_TRAIL_MAX_LENGTH);

    // Width
    pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pTrailRoot);
    pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pEdit->SetLabelText(_T("Width"));
    pEdit->SetItemValue((LPARAM)&pEmitterTrail->fWidth);
    pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_TRAIL_WIDTH);

    // Texture
    std::string strTexture = pEmitterTrail->strTexture;
    CPropTreeItem* pTextureItem = SetProperties_Textures(strTexture, pTrailRoot);
    pTextureItem->SetCtrlID(_PROPERTY_CONTROL_EMITTER_TRAIL_TEXTURE);

    // Edge Gap
    pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pTrailRoot);
    pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pEdit->SetLabelText(_T("Edge Gap"));
    pEdit->SetItemValue((LPARAM)&pEmitterTrail->fEdgeGap);
    pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_TRAIL_EDGE_GAP);

    // Spline Point Count
    pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pTrailRoot);
    pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);
    pEdit->SetLabelText(_T("Spline Point Count"));
    pEdit->SetItemValue((LPARAM)pEmitterTrail->nSplinePointCount);
    pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_TRAIL_SPLINE_COUNT);

    // Max Edge Count
    pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pTrailRoot);
    pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);
    pEdit->SetLabelText(_T("Max Edge Count"));
    pEdit->SetItemValue((LPARAM)pEmitterTrail->nMaxEdgeCount);
    pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_TRAIL_MAX_EDGE_COUNT);    

    // Src Blend
    pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pTrailRoot);	    
    pCombo->SetLabelText(_T("Src Blend"));    
    pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
    pCombo->SetDropDownHeight(300);
    for (RwInt32 i = 0; i < g_nBlendMaxCount; ++ i)
    {
        pCombo->AddString(_T(g_BlendString[i]));
        pCombo->SetItemData(i, i);
    }
    pCombo->SetItemValue(GetBlendIndex(pEmitterTrail->eSrcBlend));
    pCombo->SetCtrlID(_PROPERTY_CONTROL_EMITTER_TRAIL_SRC_BLEND);

    // Dest Blend
    pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pTrailRoot);	    
    pCombo->SetLabelText(_T("Dest Blend"));    
    pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
    pCombo->SetDropDownHeight(300);
    for (RwInt32 i = 0; i < g_nBlendMaxCount; ++ i)
    {
        pCombo->AddString(_T(g_BlendString[i]));
        pCombo->SetItemData(i, i);
    }
    pCombo->SetItemValue(GetBlendIndex(pEmitterTrail->eDestBlend));
    pCombo->SetCtrlID(_PROPERTY_CONTROL_EMITTER_TRAIL_DEST_BLEND);

    // Start Color
    CPropTreeItemColor* pColor;
    pColor = (CPropTreeItemColor*)m_PropertyTree.InsertItem(new CPropTreeItemColor(), pTrailRoot);
    pColor->SetLabelText(_T("Start Color"));
    pColor->SetItemValue((LPARAM)RGB(pEmitterTrail->colStartColor.red, pEmitterTrail->colStartColor.green, pEmitterTrail->colStartColor.blue));
    pColor->SetCtrlID(_PROPERTY_CONTROL_EMITTER_TRAIL_START_COL);

    // Start Alpha
    CPropTreeItemSlider* pSlider;
    pSlider = (CPropTreeItemSlider*)m_PropertyTree.InsertItem(new CPropTreeItemSlider(), pTrailRoot);
    pSlider->SetLabelText(_T("Start Alpha"));    
    pSlider->SetRange( 0, 255 );
    pSlider->SetItemValue((LPARAM) ((RwInt32)(pEmitterTrail->colStartColor.alpha)) );
    pSlider->SetCtrlID(_PROPERTY_CONTROL_EMITTER_TRAIL_START_ALPHA);

    // End Color
    pColor = (CPropTreeItemColor*)m_PropertyTree.InsertItem(new CPropTreeItemColor(), pTrailRoot);
    pColor->SetLabelText(_T("End Color"));
    pColor->SetItemValue((LPARAM)RGB(pEmitterTrail->colEndColor.red, pEmitterTrail->colEndColor.green, pEmitterTrail->colEndColor.blue));
    pColor->SetCtrlID(_PROPERTY_CONTROL_EMITTER_TRAIL_END_COL);

    // End Alpha
    pSlider = (CPropTreeItemSlider*)m_PropertyTree.InsertItem(new CPropTreeItemSlider(), pTrailRoot);
    pSlider->SetLabelText(_T("End Alpha"));    
    pSlider->SetRange( 0, 255 );
    pSlider->SetItemValue((LPARAM) ((RwInt32)(pEmitterTrail->colEndColor.alpha)) );
    pSlider->SetCtrlID(_PROPERTY_CONTROL_EMITTER_TRAIL_END_ALPHA);

    // Offset
    CPropTreeItem* pOffset = m_PropertyTree.InsertItem(new CPropTreeItem(), pTrailRoot);
    pOffset->SetLabelText(_T("Off Set"));

    // Offset X
    pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pOffset);
    pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pEdit->SetLabelText(_T("Offset_X"));
    pEdit->SetItemValue((LPARAM)&pEmitterTrail->vOffset.x);
    pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_TRAIL_OFFSET_X);

    pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pOffset);
    pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pEdit->SetLabelText(_T("Offset_Y"));
    pEdit->SetItemValue((LPARAM)&pEmitterTrail->vOffset.y);
    pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_TRAIL_OFFSET_Y);

    pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pOffset);
    pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pEdit->SetLabelText(_T("Offset_Z"));
    pEdit->SetItemValue((LPARAM)&pEmitterTrail->vOffset.z);
    pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_TRAIL_OFFSET_Z);
}   


void CEffectPropertyDialog::SetProperties_MultiColorEmitter(RpPrtAdvPrtMultiColorEmitter* pMultiColorEmitteer, CPropTreeItem* pRoot)
{
	CString strPropertiesTitle;
	CPropTreeItem* pMultiColorRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pMultiColorRoot->Expand();
	pMultiColorRoot->SetLabelText(_T("Multi Color Emitter"));

	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiColorRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);
	pEdit->SetLabelText(_T("Num Color"));
	pEdit->SetInfoText(_T("Num Color"));
	pEdit->SetItemValue((LPARAM)pMultiColorEmitteer->numCol);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_COLOR_EMITTER_NUM_COLOR);

	CPropTreeItemCombo* pCombo;
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pMultiColorRoot);	
	pCombo->SetLabelText(_T("Current Index"));
	pCombo->SetInfoText(_T("Current Index"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
	for (RwUInt32 i = 0; i < pMultiColorEmitteer->numCol; ++i)
	{
		strPropertiesTitle.Format(_T("List %02d"), i);
		pCombo->AddString(_T(strPropertiesTitle));
		pCombo->SetItemData(i, i);
	}
	pCombo->SetItemValue(m_nCurrentPoint);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_COLOR_EMITTER_CURRENT_INDEX);
	NTL_ASSERTE(m_nCurrentPoint < pMultiColorEmitteer->numCol);


	CPropTreeItem* pMultiColorListRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pMultiColorRoot);		
	pMultiColorListRoot->Expand();
	strPropertiesTitle.Format(_T("Color List : %02d"), m_nCurrentPoint);
	pMultiColorListRoot->SetLabelText(_T(strPropertiesTitle));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiColorListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("time"));
	pEdit->SetInfoText(_T("time"));
	pEdit->SetItemValue((LPARAM)&pMultiColorEmitteer->list[m_nCurrentPoint].time);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_COLOR_EMITTER_TIME);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiColorListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("time Bias"));
	pEdit->SetInfoText(_T("time Bias"));
	pEdit->SetItemValue((LPARAM)&pMultiColorEmitteer->list[m_nCurrentPoint].timeBias);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_COLOR_EMITTER_TIME_BIAS);


	CPropTreeItemColor* pColor;
	pColor = (CPropTreeItemColor*)m_PropertyTree.InsertItem(new CPropTreeItemColor(), pMultiColorListRoot);
	pColor->SetLabelText(_T("Color"));
	pColor->SetItemValue((LPARAM)RGB(pMultiColorEmitteer->list[m_nCurrentPoint].midCol.red, pMultiColorEmitteer->list[m_nCurrentPoint].midCol.green, pMultiColorEmitteer->list[m_nCurrentPoint].midCol.blue)); // default as color yellow
	pColor->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_COLOR_EMITTER_COLOR);

	CPropTreeItemSlider* pSlider;
	pSlider = (CPropTreeItemSlider*)m_PropertyTree.InsertItem(new CPropTreeItemSlider(), pMultiColorListRoot);
	pSlider->SetLabelText(_T("Alpha"));
	pSlider->SetInfoText(_T("Alpha"));
	pSlider->SetRange( 0, 255 );
	pSlider->SetItemValue((LPARAM) ((RwInt32)(pMultiColorEmitteer->list[m_nCurrentPoint].midCol.alpha)) );
	pSlider->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_COLOR_EMITTER_COLOR_A);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiColorListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Bias R"));
	pEdit->SetInfoText(_T("Bias R"));
	pEdit->SetItemValue((LPARAM)&pMultiColorEmitteer->list[m_nCurrentPoint].midColBias.red);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_COLOR_EMITTER_COLOR_BIAS_R);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiColorListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Bias G"));
	pEdit->SetInfoText(_T("Bias G"));
	pEdit->SetItemValue((LPARAM)&pMultiColorEmitteer->list[m_nCurrentPoint].midColBias.green);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_COLOR_EMITTER_COLOR_BIAS_G);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiColorListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Bias B"));
	pEdit->SetInfoText(_T("Bias B"));
	pEdit->SetItemValue((LPARAM)&pMultiColorEmitteer->list[m_nCurrentPoint].midColBias.blue);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_COLOR_EMITTER_COLOR_BIAS_B);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiColorListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Bias A"));
	pEdit->SetInfoText(_T("Bias A"));
	pEdit->SetItemValue((LPARAM)&pMultiColorEmitteer->list[m_nCurrentPoint].midColBias.alpha);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_COLOR_EMITTER_COLOR_BIAS_A);
}

//------------------------------------------------------------------
//	FuncName	: SetProperties_MultiSizeEmitter
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_MultiSizeEmitter(RpPrtAdvPrtMultiSizeEmitter* pMultiSizeEmitter, CPropTreeItem* pRoot)
{
	CString strPropertiesTitle;

	CPropTreeItem* pMultiSizeRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pMultiSizeRoot->Expand();
	pMultiSizeRoot->SetLabelText(_T("Multi Size Emitter"));

	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiSizeRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);
	pEdit->SetLabelText(_T("Num Size"));
	pEdit->SetInfoText(_T("Num Size"));
	pEdit->SetItemValue((LPARAM)pMultiSizeEmitter->numSize);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_NUM_SIZE);

	CPropTreeItemCombo* pCombo;
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pMultiSizeRoot);	
	pCombo->SetLabelText(_T("Current Index"));
	pCombo->SetInfoText(_T("Current Index"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
	for (RwUInt32 i = 0; i < pMultiSizeEmitter->numSize; ++i)
	{
		strPropertiesTitle.Format(_T("List %02d"), i);
		pCombo->AddString(_T(strPropertiesTitle));
		pCombo->SetItemData(i, i);
	}
	pCombo->SetItemValue(m_nCurrentPoint);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_CURRENT_INDEX);
	NTL_ASSERTE(m_nCurrentPoint < pMultiSizeEmitter->numSize);

	CPropTreeItem* pMultiSizeListRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pMultiSizeRoot);		
	pMultiSizeListRoot->Expand();
	strPropertiesTitle.Format(_T("Size List : %02d"), m_nCurrentPoint);
	pMultiSizeListRoot->SetLabelText(_T(strPropertiesTitle));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiSizeListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("time"));
	pEdit->SetInfoText(_T("time"));
	pEdit->SetItemValue((LPARAM)&pMultiSizeEmitter->list[m_nCurrentPoint].time);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_TIME);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiSizeListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("time Bias"));
	pEdit->SetInfoText(_T("time Bias"));
	pEdit->SetItemValue((LPARAM)&pMultiSizeEmitter->list[m_nCurrentPoint].timeBias);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_TIME_BIAS);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiSizeListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size X"));
	pEdit->SetInfoText(_T("Size X"));
	pEdit->SetItemValue((LPARAM)&pMultiSizeEmitter->list[m_nCurrentPoint].midSize.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_SIZE_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiSizeListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size Y"));
	pEdit->SetInfoText(_T("Size Y"));
	pEdit->SetItemValue((LPARAM)&pMultiSizeEmitter->list[m_nCurrentPoint].midSize.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_SIZE_Y);


	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiSizeListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size Bias X"));
	pEdit->SetInfoText(_T("Size Bias X"));
	pEdit->SetItemValue((LPARAM)&pMultiSizeEmitter->list[m_nCurrentPoint].midSizeBias.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_SIZE_BIAS_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiSizeListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size Bias Y"));
	pEdit->SetInfoText(_T("Size Bias Y"));
	pEdit->SetItemValue((LPARAM)&pMultiSizeEmitter->list[m_nCurrentPoint].midSizeBias.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_SIZE_BIAS_Y);
}

//------------------------------------------------------------------
//	FuncName	: SetProperties_MultiSizeEmitter
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_MultiSizeEmitter(SNtlAdvMultiSizeEmitter* pMultiSizeEmitter, CPropTreeItem* pRoot)
{
	CString strPropertiesTitle;

	CPropTreeItem* pMultiSizeRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pMultiSizeRoot->Expand();
	pMultiSizeRoot->SetLabelText(_T("Multi Size Emitter"));

	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiSizeRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);
	pEdit->SetLabelText(_T("Num Size"));
	pEdit->SetInfoText(_T("Num Size"));
	pEdit->SetItemValue((LPARAM)pMultiSizeEmitter->numSize);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_NUM_SIZE);

	CPropTreeItemCombo* pCombo;
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pMultiSizeRoot);	
	pCombo->SetLabelText(_T("Current Index"));
	pCombo->SetInfoText(_T("Current Index"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
	for (RwUInt32 i = 0; i < pMultiSizeEmitter->numSize; ++i)
	{
		strPropertiesTitle.Format(_T("List %02d"), i);
		pCombo->AddString(_T(strPropertiesTitle));
		pCombo->SetItemData(i, i);
	}
	pCombo->SetItemValue(m_nCurrentPoint);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_CURRENT_INDEX);
	NTL_ASSERTE(m_nCurrentPoint < pMultiSizeEmitter->numSize);

	CPropTreeItem* pMultiSizeListRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pMultiSizeRoot);		
	pMultiSizeListRoot->Expand();
	strPropertiesTitle.Format(_T("Size List : %02d"), m_nCurrentPoint);
	pMultiSizeListRoot->SetLabelText(_T(strPropertiesTitle));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiSizeListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("time"));
	pEdit->SetInfoText(_T("time"));
	pEdit->SetItemValue((LPARAM)&pMultiSizeEmitter->list[m_nCurrentPoint].time);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_TIME);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiSizeListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("time Bias"));
	pEdit->SetInfoText(_T("time Bias"));
	pEdit->SetItemValue((LPARAM)&pMultiSizeEmitter->list[m_nCurrentPoint].timeBias);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_TIME_BIAS);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiSizeListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size X"));
	pEdit->SetInfoText(_T("Size X"));
	pEdit->SetItemValue((LPARAM)&pMultiSizeEmitter->list[m_nCurrentPoint].midSize.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_SIZE_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiSizeListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size Y"));
	pEdit->SetInfoText(_T("Size Y"));
	pEdit->SetItemValue((LPARAM)&pMultiSizeEmitter->list[m_nCurrentPoint].midSize.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_SIZE_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiSizeListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size Z"));
	pEdit->SetInfoText(_T("Size Z"));
	pEdit->SetItemValue((LPARAM)&pMultiSizeEmitter->list[m_nCurrentPoint].midSize.z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_SIZE_Z);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiSizeListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size Bias X"));
	pEdit->SetInfoText(_T("Size Bias X"));
	pEdit->SetItemValue((LPARAM)&pMultiSizeEmitter->list[m_nCurrentPoint].midSizeBias.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_SIZE_BIAS_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiSizeListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size Bias Y"));
	pEdit->SetInfoText(_T("Size Bias Y"));
	pEdit->SetItemValue((LPARAM)&pMultiSizeEmitter->list[m_nCurrentPoint].midSizeBias.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_SIZE_BIAS_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiSizeListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Size Bias Z"));
	pEdit->SetInfoText(_T("Size Bias Z"));
	pEdit->SetItemValue((LPARAM)&pMultiSizeEmitter->list[m_nCurrentPoint].midSizeBias.z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_SIZE_BIAS_Z);
}

//------------------------------------------------------------------
//	FuncName	: SetProperties_MultiSizeEmitter
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_MultiRotateEmitter(SNtlAdvMultiRotateEmitter* pMultiRotateEmitter, CPropTreeItem* pRoot)
{
	CString strPropertiesTitle;

	CPropTreeItem* pMultiRotateRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pMultiRotateRoot->Expand();
	pMultiRotateRoot->SetLabelText(_T("Multi Rotate Emitter"));

	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiRotateRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);
	pEdit->SetLabelText(_T("Num Rotate"));
	pEdit->SetInfoText(_T("Num Rotate"));
	pEdit->SetItemValue((LPARAM)pMultiRotateEmitter->numRotate);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_ROTATE_EMITTER_NUM_ROTATE);

	CPropTreeItemCombo* pCombo;
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pMultiRotateRoot);	
	pCombo->SetLabelText(_T("Current Index"));
	pCombo->SetInfoText(_T("Current Index"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
	for (RwUInt32 i = 0; i < pMultiRotateEmitter->numRotate; ++i)
	{
		strPropertiesTitle.Format(_T("List %02d"), i);
		pCombo->AddString(_T(strPropertiesTitle));
		pCombo->SetItemData(i, i);
	}
	pCombo->SetItemValue(m_nCurrentPoint);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_ROTATE_EMITTER_CURRENT_INDEX);
	NTL_ASSERTE(m_nCurrentPoint < pMultiRotateEmitter->numRotate);

	CPropTreeItem* pMultiRotateListRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pMultiRotateRoot);		
	pMultiRotateListRoot->Expand();
	strPropertiesTitle.Format(_T("Rotate List : %02d"), m_nCurrentPoint);
	pMultiRotateListRoot->SetLabelText(_T(strPropertiesTitle));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiRotateListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("time"));
	pEdit->SetInfoText(_T("time"));
	pEdit->SetItemValue((LPARAM)&pMultiRotateEmitter->list[m_nCurrentPoint].time);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_ROTATE_EMITTER_TIME);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiRotateListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("time Bias"));
	pEdit->SetInfoText(_T("time Bias"));
	pEdit->SetItemValue((LPARAM)&pMultiRotateEmitter->list[m_nCurrentPoint].timeBias);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_ROTATE_EMITTER_TIME_BIAS);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiRotateListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Rotate X"));
	pEdit->SetInfoText(_T("Rotate X"));
	pEdit->SetItemValue((LPARAM)&pMultiRotateEmitter->list[m_nCurrentPoint].midRotate.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_ROTATE_EMITTER_ROTATE_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiRotateListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Rotate Y"));
	pEdit->SetInfoText(_T("Rotate Y"));
	pEdit->SetItemValue((LPARAM)&pMultiRotateEmitter->list[m_nCurrentPoint].midRotate.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_ROTATE_EMITTER_ROTATE_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiRotateListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Rotate Z"));
	pEdit->SetInfoText(_T("Rotate Z"));
	pEdit->SetItemValue((LPARAM)&pMultiRotateEmitter->list[m_nCurrentPoint].midRotate.z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_ROTATE_EMITTER_ROTATE_Z);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiRotateListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Rotate Bias X"));
	pEdit->SetInfoText(_T("Rotate Bias X"));
	pEdit->SetItemValue((LPARAM)&pMultiRotateEmitter->list[m_nCurrentPoint].midRotateBias.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_ROTATE_EMITTER_ROTATE_BIAS_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiRotateListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Rotate Bias Y"));
	pEdit->SetInfoText(_T("Rotate Bias Y"));
	pEdit->SetItemValue((LPARAM)&pMultiRotateEmitter->list[m_nCurrentPoint].midRotateBias.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_ROTATE_EMITTER_ROTATE_BIAS_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiRotateListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("Rotate Bias Z"));
	pEdit->SetInfoText(_T("Rotate Bias Z"));
	pEdit->SetItemValue((LPARAM)&pMultiRotateEmitter->list[m_nCurrentPoint].midRotateBias.z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_ROTATE_EMITTER_ROTATE_BIAS_Z);
}

//------------------------------------------------------------------
//	FuncName	: SetProperties_MultiTesturesEmitter
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_MultiTexturesEmitter(RpPrtAdvEmtPrtMultiTextures* pMultiTexturesEmitter, CPropTreeItem* pRoot)
{
	CString strPropertiesTitle;

	CPropTreeItem* pMultiTexturesRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pMultiTexturesRoot->Expand();
	pMultiTexturesRoot->SetLabelText(_T("Multi Textures Emitter"));

	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiTexturesRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("time"));
	pEdit->SetInfoText(_T("start Time"));
	pEdit->SetItemValue((LPARAM)&pMultiTexturesEmitter->time);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_TEXTURES_EMITTER_START_TIME);


	CPropTreeItemCombo* pCombo;
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pMultiTexturesRoot);	
	pCombo->SetLabelText(_T("loop"));
	pCombo->SetInfoText(_T("Textures loop"));
	pCombo->CreateComboBoxBool();
	pCombo->SetItemValue(pMultiTexturesEmitter->bLoop);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_TEXTURES_EMITTER_LOOP);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiTexturesRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);
	pEdit->SetLabelText(_T("Num Textures"));
	pEdit->SetInfoText(_T("Multi Num Textures"));
	pEdit->SetItemValue((LPARAM)pMultiTexturesEmitter->numTextures);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_TEXTURES_EMITTER_NUM_TEXTURES);

	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pMultiTexturesRoot);	
	pCombo->SetLabelText(_T("Current Index"));
	pCombo->SetInfoText(_T("Current Index"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
	for (RwUInt32 i = 0; i < pMultiTexturesEmitter->numTextures; ++i)
	{
		strPropertiesTitle.Format(_T("List %02d"), i);
		pCombo->AddString(_T(strPropertiesTitle));
		pCombo->SetItemData(i, i);
	}
	pCombo->SetItemValue(m_nCurrentPoint);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_TEXTURES_EMITTER_CURRENT_INDEX);
	NTL_ASSERTE(m_nCurrentPoint < pMultiTexturesEmitter->numTextures);


	CPropTreeItem* pMultiTexturesListRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pMultiTexturesRoot);		
	pMultiTexturesListRoot->Expand();
	strPropertiesTitle.Format(_T("Textures List : %02d"), m_nCurrentPoint);
	pMultiTexturesListRoot->SetLabelText(_T(strPropertiesTitle));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiTexturesListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("time"));
	pEdit->SetInfoText(_T("time"));
	pEdit->SetItemValue((LPARAM)&pMultiTexturesEmitter->list[m_nCurrentPoint].time);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_TEXTURES_EMITTER_TIME);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiTexturesListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("time Bias"));
	pEdit->SetInfoText(_T("time Bias"));
	pEdit->SetItemValue((LPARAM)&pMultiTexturesEmitter->list[m_nCurrentPoint].timeBias);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_TEXTURES_EMITTER_TIME_BIAS);


	// ============================================
	// Texture
	// ============================================
	CPropTreeItem* pTextureItem = SetProperties_Textures(pMultiTexturesEmitter->list[m_nCurrentPoint].strTexture, pMultiTexturesListRoot);
	pTextureItem->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_TEXTURES_EMITTER_TEXTURE_NAME);

}


//------------------------------------------------------------------
//	FuncName	: SetProperties_MultiTexCoordsEmitter
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_MultiTexCoordsEmitter(RpPrtAdvEmtPrtMultiTexCoords* pMultiTexCoordsEmitter, CPropTreeItem* pRoot)
{
	CString strPropertiesTitle;

	CPropTreeItem* pMultiTexCoordsRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pMultiTexCoordsRoot->Expand();
	pMultiTexCoordsRoot->SetLabelText(_T("Multi TexCoords Emitter"));

	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiTexCoordsRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);
	pEdit->SetLabelText(_T("Num Tex"));
	pEdit->SetInfoText(_T("Num Tex"));
	pEdit->SetItemValue((LPARAM)pMultiTexCoordsEmitter->numTex);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_TEXCOORDS_EMITTER_NUM_TEX);

	CPropTreeItemCombo* pCombo;
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pMultiTexCoordsRoot);	
	pCombo->SetLabelText(_T("Current Index"));
	pCombo->SetInfoText(_T("Current Index"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
	for (RwUInt32 i = 0; i < pMultiTexCoordsEmitter->numTex; ++i)
	{
		strPropertiesTitle.Format(_T("List %02d"), i);
		pCombo->AddString(_T(strPropertiesTitle));
		pCombo->SetItemData(i, i);
	}
	pCombo->SetItemValue(m_nCurrentPoint);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_TEXCOORDS_EMITTER_CURRENT_INDEX);
	NTL_ASSERTE(m_nCurrentPoint < pMultiTexCoordsEmitter->numTex);

	CPropTreeItem* pMultiTexCoordsListRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pMultiTexCoordsRoot);		
	pMultiTexCoordsListRoot->Expand();
	strPropertiesTitle.Format(_T("Tex List : %02d"), m_nCurrentPoint);
	pMultiTexCoordsListRoot->SetLabelText(_T(strPropertiesTitle));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiTexCoordsListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("time"));
	pEdit->SetInfoText(_T("time"));
	pEdit->SetItemValue((LPARAM)&pMultiTexCoordsEmitter->list[m_nCurrentPoint].time);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_TEXCOORDS_EMITTER_TIME);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiTexCoordsListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("time Bias"));
	pEdit->SetInfoText(_T("time Bias"));
	pEdit->SetItemValue((LPARAM)&pMultiTexCoordsEmitter->list[m_nCurrentPoint].timeBias);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_TEXCOORDS_EMITTER_TIME_BIAS);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiTexCoordsListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("mid0 U"));
	pEdit->SetInfoText(_T("mid0 U"));
	pEdit->SetItemValue((LPARAM)&pMultiTexCoordsEmitter->list[m_nCurrentPoint].midUV0.u);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_TEXCOORDS_EMITTER_MIDUV0_U);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiTexCoordsListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("mid0 V"));
	pEdit->SetInfoText(_T("mid0 V"));
	pEdit->SetItemValue((LPARAM)&pMultiTexCoordsEmitter->list[m_nCurrentPoint].midUV0.v);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_TEXCOORDS_EMITTER_MIDUV0_V);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiTexCoordsListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("mid0 Bias U"));
	pEdit->SetInfoText(_T("mid0 Bias U"));
	pEdit->SetItemValue((LPARAM)&pMultiTexCoordsEmitter->list[m_nCurrentPoint].midUV0Bias.u);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_TEXCOORDS_EMITTER_MIDUV0_BIAS_U);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiTexCoordsListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("mid0 Bias V"));
	pEdit->SetInfoText(_T("mid0 Bias V"));
	pEdit->SetItemValue((LPARAM)&pMultiTexCoordsEmitter->list[m_nCurrentPoint].midUV0Bias.v);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_TEXCOORDS_EMITTER_MIDUV0_BIAS_V);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiTexCoordsListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("mid1 U"));
	pEdit->SetInfoText(_T("mid1 U"));
	pEdit->SetItemValue((LPARAM)&pMultiTexCoordsEmitter->list[m_nCurrentPoint].midUV1.u);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_TEXCOORDS_EMITTER_MIDUV1_U);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiTexCoordsListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("mid1 V"));
	pEdit->SetInfoText(_T("mid1 V"));
	pEdit->SetItemValue((LPARAM)&pMultiTexCoordsEmitter->list[m_nCurrentPoint].midUV1.v);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_TEXCOORDS_EMITTER_MIDUV1_V);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiTexCoordsListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("mid1 Bias U"));
	pEdit->SetInfoText(_T("mid1 Bias U"));
	pEdit->SetItemValue((LPARAM)&pMultiTexCoordsEmitter->list[m_nCurrentPoint].midUV1Bias.u);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_TEXCOORDS_EMITTER_MIDUV1_BIAS_U);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pMultiTexCoordsListRoot);	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetLabelText(_T("mid1 Bias V"));
	pEdit->SetInfoText(_T("mid1 Bias V"));
	pEdit->SetItemValue((LPARAM)&pMultiTexCoordsEmitter->list[m_nCurrentPoint].midUV1Bias.v);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_MULTI_TEXCOORDS_EMITTER_MIDUV1_BIAS_V);
}

//------------------------------------------------------------------
//	FuncName	: SetProperties_SplinePathEmitter
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_SplinePathEmitter(SNtlPrtEmitterSplinePath* pSplinePathEmitter, CPropTreeItem* pRoot)
{
	CString strPropertiesTitle;

	// Alpha Blend
	CPropTreeItemCombo* pCombo;
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);	
	pCombo->SetLabelText(_T("path"));
	pCombo->SetInfoText(_T("Spline path"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);

	RwInt32 index = 0;
	index = pCombo->AddString(_T("NONE"));
	pCombo->SetItemData(index, NULL);
	pCombo->SetItemValue(0);

	CVenusPropertyContainer::svdef_Property::iterator it;
	for (it = CVenusPropertyContainer::GetInstance().m_svSplinePathProperty.begin(); it != CVenusPropertyContainer::GetInstance().m_svSplinePathProperty.end(); ++ it)
	{
		index = pCombo->AddString(_T((*it)->GetName()));
		pCombo->SetItemData(index, (DWORD_PTR)(*it));
		if (_stricmp(pSplinePathEmitter->strName, (*it)->GetName()) == 0)
		{
			pCombo->SetItemValue(index);
		}
	}
	pCombo->SetCtrlID(_PROPERTY_CONTROL_ACTION_SPLINE_PATH_NAME);



	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);	
	pCombo->SetLabelText(_T("loop"));
	pCombo->SetInfoText(_T("path loop"));
	pCombo->CreateComboBoxBool();
	pCombo->SetItemValue(pSplinePathEmitter->loop);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_ACTION_SPLINE_PATH_LOOP);

	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);	
	pCombo->SetLabelText(_T("use Rotate"));
	pCombo->SetInfoText(_T("use Path Rotate"));
	pCombo->CreateComboBoxBool();
	pCombo->SetItemValue(pSplinePathEmitter->useRotate);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_ACTION_SPLINE_PATH_USE_ROTATE);

	// ============================================
	// World Position
	// ============================================
	CPropTreeItem* pPositionRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pPositionRoot->Expand();
	pPositionRoot->SetLabelText("Position");

	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pPositionRoot);	
	pEdit->SetLabelText(_T("X"));
	pEdit->SetInfoText(_T("World Position X"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pSplinePathEmitter->vPosition.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_SPLINE_MATRIX_POSITION_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pPositionRoot);	
	pEdit->SetLabelText(_T("Y"));
	pEdit->SetInfoText(_T("World Position Y"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pSplinePathEmitter->vPosition.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_SPLINE_MATRIX_POSITION_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pPositionRoot);	
	pEdit->SetLabelText(_T("Z"));
	pEdit->SetInfoText(_T("World Position Y"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pSplinePathEmitter->vPosition.z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_ACTION_SPLINE_MATRIX_POSITION_Z);

	CPropTreeItem* pRotateRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pRotateRoot->Expand();
	pRotateRoot->SetLabelText("Rotate");

	CPropTreeItemSlider* pSlider;
	pSlider = (CPropTreeItemSlider*)m_PropertyTree.InsertItem(new CPropTreeItemSlider(), pRotateRoot);
	pSlider->SetLabelText(_T("X"));
	pSlider->SetInfoText(_T("World Rotate X"));
	pSlider->SetRange(0, 360);
	pSlider->SetColors(RGB(255, 255, 255), RGB(255, 0, 0));
	pSlider->SetItemValue((LPARAM) ((RwInt32)(pSplinePathEmitter->vRotate.x)) );
	pSlider->SetCtrlID(_PROPERTY_CONTROL_ACTION_SPLINE_MATRIX_ROTATE_X);

	pSlider = (CPropTreeItemSlider*)m_PropertyTree.InsertItem(new CPropTreeItemSlider(), pRotateRoot);
	pSlider->SetLabelText(_T("Y"));
	pSlider->SetInfoText(_T("World Rotate Y"));
	pSlider->SetRange(0, 360);
	pSlider->SetColors(RGB(255, 255, 255), RGB(0, 255, 0));
	pSlider->SetItemValue((LPARAM) ((RwInt32)(pSplinePathEmitter->vRotate.y)) );
	pSlider->SetCtrlID(_PROPERTY_CONTROL_ACTION_SPLINE_MATRIX_ROTATE_Y);

	pSlider = (CPropTreeItemSlider*)m_PropertyTree.InsertItem(new CPropTreeItemSlider(), pRotateRoot);
	pSlider->SetLabelText(_T("Z"));
	pSlider->SetInfoText(_T("World Rotate Z"));
	pSlider->SetRange(0, 360);
	pSlider->SetColors(RGB(255, 255, 255), RGB(0, 0, 255));
	pSlider->SetItemValue((LPARAM) ((RwInt32)(pSplinePathEmitter->vRotate.z)) );
	pSlider->SetCtrlID(_PROPERTY_CONTROL_ACTION_SPLINE_MATRIX_ROTATE_Z);
}

void CEffectPropertyDialog::SetProperties_ResourceMeshSystem(CNtlResourceMeshSystem* pResourceMeshSystem, CPropTreeItem* pRoot)
{
	CString strPropertiesTitle;

	SetProperties_ResourceComponentSystem(pResourceMeshSystem, pRoot);

	// ============================================
	// Clump Name
	// ============================================
	CPropTreeItemCombo* pCombo;
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);	
	pCombo->SetLabelText(_T("Clump Name"));
	pCombo->SetInfoText(_T("Clump Name"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
	pCombo->SetDropDownHeight(300);

	RwInt32 index = 0;
	index = pCombo->AddString(_T("NONE"));
	pCombo->SetItemData(index, NULL);
	pCombo->SetItemValue(0);

	CNtlResourceFileManager::smdef_Entity::iterator it;
	for (it = CNtlEffectSystemFreeList::m_ClumpFileManager.m_smEntity.begin(); it != CNtlEffectSystemFreeList::m_ClumpFileManager.m_smEntity.end(); ++ it)
	{
		std::string strFileName = (*it).first.c_str();
		index = pCombo->AddString(_T(strFileName.c_str()));
		pCombo->SetItemData(index, (DWORD_PTR)(*it).first.c_str());
		if (_stricmp(strFileName.c_str(), pResourceMeshSystem->m_strMeshFileName.c_str()) == 0)
		{
			pCombo->SetItemValue(index);
		}
	}
	pCombo->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_MESH_FILE_NAME);

	// ============================================
	// Animation Name
	// ============================================
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);	
	pCombo->SetLabelText(_T("Ani Name"));
	pCombo->SetInfoText(_T("Ani Name"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
	pCombo->SetDropDownHeight(300);

	index = pCombo->AddString(_T("NONE"));
	pCombo->SetItemData(index, NULL);
	pCombo->SetItemValue(0);

	RwChar strClumpName[MAX_PATH];
	memset(strClumpName, 0, MAX_PATH);
	if (!pResourceMeshSystem->m_strMeshFileName.empty())
	{
		strcpy(strClumpName, pResourceMeshSystem->m_strMeshFileName.c_str());
		strClumpName[pResourceMeshSystem->m_strMeshFileName.size() - 4] = '\0';

		for (it = CNtlEffectSystemFreeList::m_AnimationFileManager.m_smEntity.begin(); it != CNtlEffectSystemFreeList::m_AnimationFileManager.m_smEntity.end(); ++ it)
		{
			std::string strFileName = (*it).first.c_str();
			if (strstr(strFileName.c_str(), strClumpName) != NULL)
			{
				index = pCombo->AddString(_T(strFileName.c_str()));
				pCombo->SetItemData(index, (DWORD_PTR)(*it).first.c_str());
				if (_stricmp(strFileName.c_str(), pResourceMeshSystem->m_strAniFileName.c_str()) == 0)
				{
					pCombo->SetItemValue(index);
				}
			}
		}
	}
	pCombo->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_ANIMATION_FILE_NAME);

	// ============================================
	// Uv Animation Name
	// ============================================
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);	
	pCombo->SetLabelText(_T("Uv Name"));
	pCombo->SetInfoText(_T("Uv Name"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
	pCombo->SetDropDownHeight(300);

	index = pCombo->AddString(_T("NONE"));
	pCombo->SetItemData(index, NULL);
	pCombo->SetItemValue(0);

	for (it = CNtlEffectSystemFreeList::m_UvAnimationFileManager.m_smEntity.begin(); it != CNtlEffectSystemFreeList::m_UvAnimationFileManager.m_smEntity.end(); ++ it)
	{
		std::string strFileName = (*it).first.c_str();
		index = pCombo->AddString(_T(strFileName.c_str()));
		pCombo->SetItemData(index, (DWORD_PTR)(*it).first.c_str());
		if (_stricmp(strFileName.c_str(), pResourceMeshSystem->m_strUvFileName.c_str()) == 0)
		{
			pCombo->SetItemValue(index);
		}
	}
	pCombo->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_UVANIMATION_FILE_NAME);


	CPropTreeItem* pAlphaRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pAlphaRoot->Expand();
	pAlphaRoot->SetLabelText("System Alpha");

	// Alpha Blend
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pAlphaRoot);	
	strPropertiesTitle.Format(_T("Alpha Blend"));
	pCombo->SetLabelText(strPropertiesTitle);
	pCombo->SetInfoText(strPropertiesTitle);
	pCombo->CreateComboBoxBool();
	pCombo->SetItemValue(pResourceMeshSystem->m_bAlphaBlend);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_ALPHA_ENALBE);

	// Src Blend
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pAlphaRoot);	
	strPropertiesTitle.Format(_T("Src Blend"));
	pCombo->SetLabelText(strPropertiesTitle);
	pCombo->SetInfoText(strPropertiesTitle);
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
	pCombo->SetDropDownHeight(300);
	for (RwInt32 i = 0; i < g_nBlendMaxCount; ++ i)
	{
		pCombo->AddString(_T(g_BlendString[i]));
		pCombo->SetItemData(i, i);
	}
	pCombo->SetItemValue(GetBlendIndex(pResourceMeshSystem->m_nSrcBlend));
	pCombo->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_ALPHA_SRC);

	// Dest Blend
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pAlphaRoot);	
	strPropertiesTitle.Format(_T("Dest Blend"));
	pCombo->SetLabelText(strPropertiesTitle);
	pCombo->SetInfoText(strPropertiesTitle);
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
	pCombo->SetDropDownHeight(300);
	for (RwInt32 i = 0; i < g_nBlendMaxCount; ++ i)
	{
		pCombo->AddString(_T(g_BlendString[i]));
		pCombo->SetItemData(i, i);
	}
	pCombo->SetItemValue(GetBlendIndex(pResourceMeshSystem->m_nDestBlend));
	pCombo->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_ALPHA_DEST);

	SetProperties_EmitterStandard(pResourceMeshSystem, pRoot);

}

void CEffectPropertyDialog::SetProperties_EmitterStandard(CNtlResourceMeshSystem* pResourceMeshSystem, CPropTreeItem* pRoot)
{
	CPropTreeItemEdit* pEdit;

	// ============================================
	// ZWRITE ENABLE
	// ============================================
	CPropTreeItemCombo* pCombo;
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);	
	pCombo->SetLabelText(_T("ZWrite"));
	pCombo->SetInfoText(_T("ZWrite Enable"));
	pCombo->CreateComboBoxBool();
	pCombo->SetItemValue(pResourceMeshSystem->m_EmitterStandard.bWriteEnable);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_MESH_ZWRITE_ENABLE);

    // ============================================
    // Projectile Shot
    // ============================================    
    pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);	
    pCombo->SetLabelText(_T("Projectile Shot"));
    pCombo->SetInfoText(_T("Projectile Shot Mesh On/Off"));
    pCombo->CreateComboBoxBool();
    pCombo->SetItemValue(pResourceMeshSystem->IsEmitterDataFlag(NTLrpPRTADVEMITTERDATAFLAGFOLLOW));
    pCombo->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_MESH_PROJECTILE);

	// ============================================
	// Color
	// ============================================
	CPropTreeItem* pColorRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pColorRoot->Expand();
	pColorRoot->SetLabelText(_T("Mesh Color"));

	CPropTreeItemColor* pColor;
	pColor = (CPropTreeItemColor*)m_PropertyTree.InsertItem(new CPropTreeItemColor(), pColorRoot);
	pColor->SetLabelText(_T("Color"));
	pColor->SetInfoText(_T("Mesh Color"));
	pColor->SetItemValue((LPARAM)RGB(pResourceMeshSystem->m_EmitterStandard.prtColor.red, pResourceMeshSystem->m_EmitterStandard.prtColor.green, pResourceMeshSystem->m_EmitterStandard.prtColor.blue)); // default as color yellow
	pColor->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_MESH_COLOR);

	CPropTreeItemSlider* pSlider;
	pSlider = (CPropTreeItemSlider*)m_PropertyTree.InsertItem(new CPropTreeItemSlider(), pColorRoot);
	pSlider->SetLabelText(_T("Alpha"));
	pSlider->SetInfoText(_T("Mesh Color Alpha"));
	pSlider->SetRange( 0, 255 );
	pSlider->SetItemValue((LPARAM) ((RwInt32)(pResourceMeshSystem->m_EmitterStandard.prtColor.alpha)) );
	pSlider->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_MESH_COLOR_A);

	// ============================================
	// Size
	// ============================================
	CPropTreeItem* pSizeRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pSizeRoot->Expand();
	pSizeRoot->SetLabelText(_T("Mesh Size"));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pSizeRoot);	
	pEdit->SetLabelText(_T("X"));
	pEdit->SetInfoText(_T("Mesh Size X"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceMeshSystem->m_EmitterStandard.prtSize.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_MESH_SIZE_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pSizeRoot);	
	pEdit->SetLabelText(_T("Y"));
	pEdit->SetInfoText(_T("Mesh Size Y"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceMeshSystem->m_EmitterStandard.prtSize.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_MESH_SIZE_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pSizeRoot);	
	pEdit->SetLabelText(_T("Z"));
	pEdit->SetInfoText(_T("Mesh Size Z"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceMeshSystem->m_EmitterStandard.prtSize.z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_MESH_SIZE_Z);

	// ============================================
	// Rotate
	// ============================================
	CPropTreeItem* pRotateRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pRotateRoot->Expand();
	pRotateRoot->SetLabelText(_T("Mesh Rotate"));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRotateRoot);	
	pEdit->SetLabelText(_T("X"));
	pEdit->SetInfoText(_T("Mesh Rotate X"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceMeshSystem->m_EmitterStandard.prtRotate.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_MESH_ROTATE_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRotateRoot);	
	pEdit->SetLabelText(_T("Y"));
	pEdit->SetInfoText(_T("Mesh Rotate Y"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceMeshSystem->m_EmitterStandard.prtRotate.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_MESH_ROTATE_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRotateRoot);	
	pEdit->SetLabelText(_T("Z"));
	pEdit->SetInfoText(_T("Mesh Rotate Z"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceMeshSystem->m_EmitterStandard.prtRotate.z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_MESH_ROTATE_Z);
}

//------------------------------------------------------------------
//	FuncName	: SetProperties_ResourceImVertexSystem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_ResourceImVertexSystem(CNtlResourceImVertexSystem* pResourceImVertexSystem, CPropTreeItem* pRoot)
{
	SetProperties_ResourceComponentSystem(pResourceImVertexSystem, pRoot);

	// ============================================
	// Alpha
	// ============================================
	CPropTreeItem* pAlphaRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pAlphaRoot->Expand();
	pAlphaRoot->SetLabelText("System Alpha");

	CString strPropertiesTitle;
	// Alpha Blend
	CPropTreeItemCombo* pCombo;
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pAlphaRoot);	
	strPropertiesTitle.Format(_T("Alpha Blend"));
	pCombo->SetLabelText(strPropertiesTitle);
	pCombo->SetInfoText(strPropertiesTitle);
	pCombo->CreateComboBoxBool();
	pCombo->SetItemValue(pResourceImVertexSystem->m_bAlphaBlend);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_ALPHA_ENALBE);

	// Src Blend
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pAlphaRoot);	
	strPropertiesTitle.Format(_T("Src Blend"));
	pCombo->SetLabelText(strPropertiesTitle);
	pCombo->SetInfoText(strPropertiesTitle);
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
	pCombo->SetDropDownHeight(300);
	for (RwInt32 i = 0; i < g_nBlendMaxCount; ++ i)
	{
		pCombo->AddString(_T(g_BlendString[i]));
		pCombo->SetItemData(i, i);
	}
	pCombo->SetItemValue(GetBlendIndex(pResourceImVertexSystem->m_nSrcBlend));
	pCombo->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_ALPHA_SRC);

	// Dest Blend
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pAlphaRoot);	
	strPropertiesTitle.Format(_T("Dest Blend"));
	pCombo->SetLabelText(strPropertiesTitle);
	pCombo->SetInfoText(strPropertiesTitle);
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
	pCombo->SetDropDownHeight(300);
	for (RwInt32 i = 0; i < g_nBlendMaxCount; ++ i)
	{
		pCombo->AddString(_T(g_BlendString[i]));
		pCombo->SetItemData(i, i);
	}
	pCombo->SetItemValue(GetBlendIndex(pResourceImVertexSystem->m_nDestBlend));
	pCombo->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_ALPHA_DEST);


	// ============================================
	// Texture
	// ============================================
	CPropTreeItem* pTextureItem = SetProperties_Textures(pResourceImVertexSystem->m_strTextureName, pRoot);
	pTextureItem->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_TEXTURE);

}

//------------------------------------------------------------------
//	FuncName	: SetProperties_ResourceBeamSystem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_ResourceBeamSystem(CNtlResourceBeamSystem* pResourceBeamSystem, CPropTreeItem* pRoot)
{
	SetProperties_ResourceImVertexSystem(pResourceBeamSystem, pRoot);

	SetProperties_EmitterStandard(pResourceBeamSystem, pRoot);
}

void CEffectPropertyDialog::SetProperties_EmitterStandard(CNtlResourceBeamSystem* pResourceBeamSystem, CPropTreeItem* pRoot)
{
	CPropTreeItemEdit* pEdit;

	// ============================================
	// Speed
	// ============================================
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("Speed"));
	pEdit->SetInfoText(_T("Beam Speed"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceBeamSystem->m_EmitterStandard.fSpeed);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_SPEED);

	// ============================================
	// Color
	// ============================================
	CPropTreeItem* pColorRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pColorRoot->Expand();
	pColorRoot->SetLabelText(_T("Beam Color"));

	CPropTreeItemColor* pColor;
	pColor = (CPropTreeItemColor*)m_PropertyTree.InsertItem(new CPropTreeItemColor(), pColorRoot);
	pColor->SetLabelText(_T("Color"));
	pColor->SetInfoText(_T("Beam Color"));
	pColor->SetItemValue((LPARAM)RGB(pResourceBeamSystem->m_EmitterStandard.prtColor.red, pResourceBeamSystem->m_EmitterStandard.prtColor.green, pResourceBeamSystem->m_EmitterStandard.prtColor.blue)); // default as color yellow
	pColor->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_COLOR);

	CPropTreeItemSlider* pSlider;
	pSlider = (CPropTreeItemSlider*)m_PropertyTree.InsertItem(new CPropTreeItemSlider(), pColorRoot);
	pSlider->SetLabelText(_T("Alpha"));
	pSlider->SetInfoText(_T("Beam Color Alpha"));
	pSlider->SetRange( 0, 255 );
	pSlider->SetItemValue((LPARAM) ((RwInt32)(pResourceBeamSystem->m_EmitterStandard.prtColor.alpha)) );
	pSlider->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_COLOR_A);

	// ============================================
	// Size
	// ============================================
	CPropTreeItem* pSizeRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pSizeRoot->Expand();
	pSizeRoot->SetLabelText(_T("Beam Size"));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pSizeRoot);	
	pEdit->SetLabelText(_T("Near"));
	pEdit->SetInfoText(_T("Beam Near Size"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceBeamSystem->m_EmitterStandard.prtSize.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_SIZE_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pSizeRoot);	
	pEdit->SetLabelText(_T("Center"));
	pEdit->SetInfoText(_T("Beam Center Size"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceBeamSystem->m_EmitterStandard.prtSize.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_SIZE_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pSizeRoot);	
	pEdit->SetLabelText(_T("Far"));
	pEdit->SetInfoText(_T("Beam Far Size"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceBeamSystem->m_EmitterStandard.prtSize.z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_SIZE_Z);

	// Head system
	CPropTreeItem* pHeadRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pHeadRoot->Expand();
	pHeadRoot->SetLabelText(_T("Head System"));

	CPropTreeItemCombo* pCombo;
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pHeadRoot);	
	pCombo->SetLabelText(_T("System"));
	pCombo->SetInfoText(_T("Head System"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);

	RwInt32 index = 0;
	index = pCombo->AddString(_T("NONE"));
	pCombo->SetItemData(index, NULL);
	pCombo->SetItemValue(0);

	CNtlResourceEffect* pResourceEffect = CVenusVisualManager::GetInstance().m_pResourceEffect;
	NTL_ASSERTE(pResourceEffect);

	CString strPropertiesTitle;
	CNtlResourceEffect::svdef_ResourceComponentSystem::iterator it;
	for (it = pResourceEffect->m_svResourceSystem.begin(); it != pResourceEffect->m_svResourceSystem.end(); ++ it)
	{
		CNtlResourceComponentSystem* pResourceSystem = (*it);
		// 자기 자신은 제외한다.
		if (_stricmp(pResourceSystem->GetSystemName(), pResourceBeamSystem->GetSystemName()) != 0)
		{
			strPropertiesTitle.Format(_T(pResourceSystem->GetSystemName()));
			index = pCombo->AddString(_T(strPropertiesTitle));
			pCombo->SetItemData(index, (DWORD_PTR)pResourceSystem);
			if (_stricmp(pResourceBeamSystem->m_EmitterStandard.strHeadSystem, pResourceSystem->GetSystemName()) == 0)
				pCombo->SetItemValue(index);
		}
	}
	pCombo->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_BEAM_HEAD_SYSTEM);
}

//------------------------------------------------------------------
//	FuncName	: SetProperties_ResourceBeamSystem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_ResourceHurricaneSystem(CNtlResourceHurricaneSystem* pResourceHurricaneSystem, CPropTreeItem* pRoot)
{
	SetProperties_ResourceImVertexSystem(pResourceHurricaneSystem, pRoot);

	SetProperties_EmitterStandard(pResourceHurricaneSystem, pRoot);
}

void CEffectPropertyDialog::SetProperties_EmitterStandard(CNtlResourceHurricaneSystem* pResourceHurricaneSystem, CPropTreeItem* pRoot)
{
	CPropTreeItemEdit* pEdit;

	// ============================================
	// m_bEternity
	// ============================================
	CPropTreeItemCombo* pCombo;
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);	
	pCombo->SetLabelText(_T("Eternity"));
	pCombo->SetInfoText(_T("Hurricane Eternity"));
	pCombo->CreateComboBoxBool();
	pCombo->SetItemValue(pResourceHurricaneSystem->m_EmitterStandard.bEternity);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_HURRICANE_ETERNITY);

	// ============================================
	// Speed
	// ============================================
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("Speed"));
	pEdit->SetInfoText(_T("Hurricane Speed"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceHurricaneSystem->m_EmitterStandard.fSpeed);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_SPEED);

	// ============================================
	// Color
	// ============================================
	CPropTreeItem* pColorRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pColorRoot->Expand();
	pColorRoot->SetLabelText(_T("Hurricane Color"));

	CPropTreeItemColor* pColor;
	pColor = (CPropTreeItemColor*)m_PropertyTree.InsertItem(new CPropTreeItemColor(), pColorRoot);
	pColor->SetLabelText(_T("Color"));
	pColor->SetInfoText(_T("Hurricane Color"));
	pColor->SetItemValue((LPARAM)RGB(pResourceHurricaneSystem->m_EmitterStandard.prtColor.red, pResourceHurricaneSystem->m_EmitterStandard.prtColor.green, pResourceHurricaneSystem->m_EmitterStandard.prtColor.blue)); // default as color yellow
	pColor->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_COLOR);

	CPropTreeItemSlider* pSlider;
	pSlider = (CPropTreeItemSlider*)m_PropertyTree.InsertItem(new CPropTreeItemSlider(), pColorRoot);
	pSlider->SetLabelText(_T("Alpha"));
	pSlider->SetInfoText(_T("Hurricane Color Alpha"));
	pSlider->SetRange( 0, 255 );
	pSlider->SetItemValue((LPARAM) ((RwInt32)(pResourceHurricaneSystem->m_EmitterStandard.prtColor.alpha)) );
	pSlider->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_COLOR_A);

	// ============================================
	// Size
	// ============================================
	CPropTreeItem* pSizeRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pSizeRoot->Expand();
	pSizeRoot->SetLabelText(_T("Hurricane Size"));

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pSizeRoot);	
	pEdit->SetLabelText(_T("Size"));
	pEdit->SetInfoText(_T("Hurricane Near Size"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceHurricaneSystem->m_EmitterStandard.prtSize.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_SIZE_X);


	// ============================================
	// Max Polygon Count
	// ============================================
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("Poly Count"));
	pEdit->SetInfoText(_T("Hurricane Max Polygon Count"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);
	pEdit->SetItemValue((LPARAM)pResourceHurricaneSystem->m_EmitterStandard.maxPolygonCount);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_HURRICANE_MAX_POLYGON_COUNT);

	// ============================================
	// Life Time
	// ============================================
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("Life Time"));
	pEdit->SetInfoText(_T("Hurricane Life Time"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceHurricaneSystem->m_EmitterStandard.lifeTime);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_HURRICANE_LIFE_TIME);

	// ============================================
	// radius
	// ============================================
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("radius"));
	pEdit->SetInfoText(_T("Hurricane radius"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceHurricaneSystem->m_EmitterStandard.radius);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_HURRICANE_RADIUS);

	// ============================================
	// radiusGap
	// ============================================
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("radiusGap"));
	pEdit->SetInfoText(_T("Hurricane radiusGap"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceHurricaneSystem->m_EmitterStandard.radiusGap);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_HURRICANE_RADIUSGAP);

	// ============================================
	// Max Polygon Count
	// ============================================
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("revolutionCount"));
	pEdit->SetInfoText(_T("Hurricane revolutionCount"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);
	pEdit->SetItemValue((LPARAM)pResourceHurricaneSystem->m_EmitterStandard.revolutionCount);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_HURRICANE_REVOLUTION_COUNT);

	// ============================================
	// radiusGap
	// ============================================
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("revolutionAngle"));
	pEdit->SetInfoText(_T("Hurricane revolutionAngle"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceHurricaneSystem->m_EmitterStandard.revolutionAngle);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_HURRICANE_REVOLUTION_ANGLE);
}

//------------------------------------------------------------------
//	FuncName	: SetProperties_ResourceBeamSystem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_ResourceLightningSystem(CNtlResourceLightningSystem* pResourceLightningSystem, CPropTreeItem* pRoot)
{
	SetProperties_ResourceImVertexSystem(pResourceLightningSystem, pRoot);

    // ============================================
    // End Position
    // ============================================
    CPropTreeItem* pPositionRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
    pPositionRoot->Expand();
    pPositionRoot->SetLabelText("End Point");

    CPropTreeItemEdit* pEdit;
    pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pPositionRoot);	
    pEdit->SetLabelText(_T("End Point X"));
    pEdit->SetInfoText(_T("Lightning End Point X"));
    pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pEdit->SetItemValue((LPARAM)&(pResourceLightningSystem->m_vRotate.x));
    pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_LIHGTNING_ENDPOINT_X);

    pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pPositionRoot);	
    pEdit->SetLabelText(_T("End Point Y"));
    pEdit->SetInfoText(_T("Lightning End Point Y"));
    pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pEdit->SetItemValue((LPARAM)&(pResourceLightningSystem->m_vRotate.y));
    pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_LIHGTNING_ENDPOINT_Y);

    pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pPositionRoot);	
    pEdit->SetLabelText(_T("End Point Z"));
    pEdit->SetInfoText(_T("Lighting End Point Z"));
    pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pEdit->SetItemValue((LPARAM)&(pResourceLightningSystem->m_vRotate.z));
    pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_LIHGTNING_ENDPOINT_Z);
    

	SetProperties_EmitterStandard(pResourceLightningSystem, pRoot);
}

void CEffectPropertyDialog::SetProperties_EmitterStandard(CNtlResourceLightningSystem* pResourceLightningSystem, CPropTreeItem* pRoot)
{
	// ============================================
	// Color
	// ============================================
	CPropTreeItem* pColorRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pColorRoot->Expand();
	pColorRoot->SetLabelText(_T("Lightning Color"));

	CPropTreeItemColor* pColor;
	pColor = (CPropTreeItemColor*)m_PropertyTree.InsertItem(new CPropTreeItemColor(), pColorRoot);
	pColor->SetLabelText(_T("Color"));
	pColor->SetInfoText(_T("Lightning Color"));
	pColor->SetItemValue((LPARAM)RGB(pResourceLightningSystem->m_EmitterStandard.prtColor.red, pResourceLightningSystem->m_EmitterStandard.prtColor.green, pResourceLightningSystem->m_EmitterStandard.prtColor.blue)); // default as color yellow
	pColor->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_COLOR);

	CPropTreeItemSlider* pSlider;
	pSlider = (CPropTreeItemSlider*)m_PropertyTree.InsertItem(new CPropTreeItemSlider(), pColorRoot);
	pSlider->SetLabelText(_T("Alpha"));
	pSlider->SetInfoText(_T("Lightning Color Alpha"));
	pSlider->SetRange( 0, 255 );
	pSlider->SetItemValue((LPARAM) ((RwInt32)(pResourceLightningSystem->m_EmitterStandard.prtColor.alpha)) );
	pSlider->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_COLOR_A);

	// ============================================
	// Size
	// ============================================
	CPropTreeItem* pSizeRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pSizeRoot->Expand();
	pSizeRoot->SetLabelText(_T("Lightning Width"));

	CPropTreeItemEdit* pEdit;
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pSizeRoot);	
	pEdit->SetLabelText(_T("Start"));
	pEdit->SetInfoText(_T("Lightning Start Size"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceLightningSystem->m_EmitterStandard.prtSize.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_SIZE_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pSizeRoot);	
	pEdit->SetLabelText(_T("End"));
	pEdit->SetInfoText(_T("Lightning End Size"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceLightningSystem->m_EmitterStandard.prtSize.y);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_SIZE_Y);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pSizeRoot);	
	pEdit->SetLabelText(_T("Center"));
	pEdit->SetInfoText(_T("Lightning Center Size"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceLightningSystem->m_EmitterStandard.prtSize.z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_SIZE_Z);

	// ============================================
	// Update Time
	// ============================================
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("UpdateTime"));
	pEdit->SetInfoText(_T("Lightning UpdateTime"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceLightningSystem->m_EmitterStandard.fUpdateTime);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_LIHGTNING_UPDATE_TIME);

	// ============================================
	// Amp
	// ============================================
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("Amp"));
	pEdit->SetInfoText(_T("Lightning Amp"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceLightningSystem->m_EmitterStandard.fAmp);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_LIHGTNING_AMP);

	// ============================================
	// Rough
	// ============================================
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("Rough"));
	pEdit->SetInfoText(_T("Lightning Rough"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceLightningSystem->m_EmitterStandard.fRough);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_LIHGTNING_ROUGH);

}

void CEffectPropertyDialog::SetProperties_ResourceLineSystem( CNtlResourceLineSystem* pResourceLineSystem, CPropTreeItem* pRoot )
{
    SetProperties_ResourceImVertexSystem(pResourceLineSystem, pRoot);

    SetProperties_EmitterStandard(pResourceLineSystem, pRoot);
}

void CEffectPropertyDialog::SetProperties_EmitterStandard( CNtlResourceLineSystem* pResourceLineSystem, CPropTreeItem* pRoot )
{
    // Line Life Time
    CPropTreeItemEdit* pLifeTime = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);
    pLifeTime->SetLabelText(_T("Line Life Time"));
    pLifeTime->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pLifeTime->SetItemValue((LPARAM)&pResourceLineSystem->m_EmitterStandard.m_fLineLifeTime);
    pLifeTime->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_LINE_LIFETIME);

    // Max Count
    CPropTreeItemEdit* pMaxCount = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);
    pMaxCount->SetLabelText(_T("Max Line Count"));
    pMaxCount->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);
    pMaxCount->SetItemValue(pResourceLineSystem->m_EmitterStandard.m_nMaxCount);
    pMaxCount->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_LINE_MAX_COUNT);

    // Emitter Count
    CPropTreeItemEdit* pEmitterCount = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);
    pEmitterCount->SetLabelText(_T("Emitter Count"));
    pEmitterCount->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);
    pEmitterCount->SetItemValue(pResourceLineSystem->m_EmitterStandard.m_nEmitterCount);
    pEmitterCount->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_LINE_EMITTER_COUNT);

    // Emitter Gap
    CPropTreeItemEdit* pEmitterGap = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);
    pEmitterGap->SetLabelText(_T("Emitter Gap"));
    pEmitterGap->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pEmitterGap->SetItemValue((LPARAM)&pResourceLineSystem->m_EmitterStandard.m_fEmitterGap);
    pEmitterGap->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_LINE_EMITTER_GAP);

    // Emitter Radius
    CPropTreeItemEdit* pEmitterRadius = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);
    pEmitterRadius->SetLabelText(_T("Emitter Radius"));
    pEmitterRadius->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pEmitterRadius->SetItemValue((LPARAM)&pResourceLineSystem->m_EmitterStandard.m_fEmitterRaduis);
    pEmitterRadius->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_LINE_EMITTER_RADIUS);

    // Emitter Bias
    CPropTreeItemEdit* pEmitterBias = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);
    pEmitterBias->SetLabelText(_T("Emitter Bias"));
    pEmitterBias->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pEmitterBias->SetItemValue((LPARAM)&pResourceLineSystem->m_EmitterStandard.m_fEmitterBias);
    pEmitterBias->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_LINE_EMITTER_BIAS);

    // Target Radius
    CPropTreeItemEdit* pTargetRadius = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);
    pTargetRadius->SetLabelText(_T("Target Radius"));
    pTargetRadius->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pTargetRadius->SetItemValue((LPARAM)&pResourceLineSystem->m_EmitterStandard.m_fTargetRadius);
    pTargetRadius->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_LINE_TARGET_RADIUS);

    // Size Line
    CPropTreeItem* pSizeRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);
    pSizeRoot->Expand();
    pSizeRoot->SetLabelText(_T("Line Size"));

    CPropTreeItemEdit* pSizeLineX = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pSizeRoot);
    pSizeLineX->SetLabelText(_T("Width"));
    pSizeLineX->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pSizeLineX->SetItemValue((LPARAM)&pResourceLineSystem->m_EmitterStandard.m_sizeLine.x);
    pSizeLineX->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_LINE_SIZE_X);

    CPropTreeItemEdit* pSizeLineY = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pSizeRoot);
    pSizeLineY->SetLabelText(_T("Length"));
    pSizeLineY->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pSizeLineY->SetItemValue((LPARAM)&pResourceLineSystem->m_EmitterStandard.m_sizeLine.y);
    pSizeLineY->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_LINE_SIZE_Y);

    // Velocity
    CPropTreeItemEdit* pVelocity = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);
    pVelocity->SetLabelText(_T("Velocity"));
    pVelocity->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pVelocity->SetItemValue((LPARAM)&pResourceLineSystem->m_EmitterStandard.m_fVelocity);
    pVelocity->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_LINE_VELOCITY);

    // Shake Enable
    CPropTreeItem* pShakeRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);
    pShakeRoot->Expand();
    pShakeRoot->SetLabelText(_T("Line Shake"));

    CPropTreeItemCombo* pShake = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pShakeRoot);
    pShake->SetLabelText(_T("Shake Enable"));
    pShake->CreateComboBoxBool();
    pShake->SetItemValue(pResourceLineSystem->m_EmitterStandard.m_bShake);
    pShake->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_LINE_SHAKE);

    CPropTreeItemEdit* pShakeBias = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pShakeRoot);
    pShakeBias->SetLabelText(_T("Shake Bias"));
    pShakeBias->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pShakeBias->SetItemValue((LPARAM)&pResourceLineSystem->m_EmitterStandard.m_fShakeBias);
    pShakeBias->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_LINE_SHAKE_BIAS);

    // Move Line
    CPropTreeItemCombo* pMoveLine = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);
    pMoveLine->SetLabelText(_T("Move Line"));
    pMoveLine->CreateComboBoxBool();
    pMoveLine->SetItemValue(pResourceLineSystem->m_EmitterStandard.m_bMoveLine);
    pMoveLine->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_LINE_MOVE_LINE);

    // Z-Buffer Enable
    CPropTreeItemCombo* pZBufferEnable = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);
    pZBufferEnable->SetLabelText(_T("Z-Buffer Enable"));
    pZBufferEnable->CreateComboBoxBool();
    pZBufferEnable->SetItemValue(pResourceLineSystem->m_EmitterStandard.m_bZBufferEnable);
    pZBufferEnable->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_LINE_ZBUFFER);

    // Z-BillBoard Enable
    CPropTreeItemCombo* pZBillBoardEnable = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);
    pZBillBoardEnable->SetLabelText(_T("Z-BillBoard Enable"));
    pZBillBoardEnable->CreateComboBoxBool();
    pZBillBoardEnable->SetItemValue(pResourceLineSystem->m_EmitterStandard.m_bZBiilBoard);
    pZBillBoardEnable->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_LINE_ZBILLBOARD);    
}

//------------------------------------------------------------------
//	FuncName	: SetProperties_ResourceBeamSystem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_ResourceDecalSystem(CNtlResourceDecalSystem* pResourceDecalSystem, CPropTreeItem* pRoot)
{
	SetProperties_ResourceImVertexSystem(pResourceDecalSystem, pRoot);

	SetProperties_EmitterStandard(pResourceDecalSystem, pRoot);
}

void CEffectPropertyDialog::SetProperties_EmitterStandard(CNtlResourceDecalSystem* pResourceDecalSystem, CPropTreeItem* pRoot)
{
	// ============================================
	// Color
	// ============================================
	CPropTreeItem* pColorRoot = m_PropertyTree.InsertItem(new CPropTreeItem(), pRoot);		
	pColorRoot->Expand();
	pColorRoot->SetLabelText(_T("Decal Color"));

	CPropTreeItemColor* pColor;
	pColor = (CPropTreeItemColor*)m_PropertyTree.InsertItem(new CPropTreeItemColor(), pColorRoot);
	pColor->SetLabelText(_T("Color"));
	pColor->SetInfoText(_T("Decal Color"));	
	pColor->SetItemValue((LPARAM)RGB(pResourceDecalSystem->m_EmitterStandard.color.red, pResourceDecalSystem->m_EmitterStandard.color.green, pResourceDecalSystem->m_EmitterStandard.color.blue));
	pColor->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_COLOR);

	CPropTreeItemSlider* pSlider;
	pSlider = (CPropTreeItemSlider*)m_PropertyTree.InsertItem(new CPropTreeItemSlider(), pColorRoot);
	pSlider->SetLabelText(_T("Alpha"));
	pSlider->SetInfoText(_T("Decal Color Alpha"));
	pSlider->SetRange( 0, 255 );
	pSlider->SetItemValue((LPARAM) ((RwInt32)(pResourceDecalSystem->m_EmitterStandard.color.alpha)));
	pSlider->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_COLOR_A);

    // Size
	CPropTreeItemCombo* pCombo;	
	CPropTreeItemEdit* pEdit;

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("X Size"));	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceDecalSystem->m_EmitterStandard.vSize.x);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_DECAL_SIZE_X);

	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("Z Size"));	
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceDecalSystem->m_EmitterStandard.vSize.z);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_DECAL_SIZE_Z);

    // Scale
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("Scale"));
	pEdit->SetInfoText(_T("Scale of Decal Effect"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceDecalSystem->m_EmitterStandard.fScale);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_DECAL_SIZE_SCALE);

    // Wrap
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);
	pCombo->SetLabelText(_T("Texture AddressMode"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);	
	pCombo->AddString("WRAP"); pCombo->SetItemData(0, 0);
	pCombo->AddString("MIRROR"); pCombo->SetItemData(1, 1);
	pCombo->AddString("CLAMP"); pCombo->SetItemData(2, 2);
	pCombo->AddString("BORDER"); pCombo->SetItemData(3, 3);
	pCombo->SetItemValue(pResourceDecalSystem->m_EmitterStandard.eWrapType - 1);	/// 넣을때는 Index 값으로 넣어줘야 된다
	pCombo->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_DECAL_WRAP_TYPE);
	
    // Visible Distance
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);
	pEdit->SetLabelText(_T("Visible Distance"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
	pEdit->SetItemValue((LPARAM)&pResourceDecalSystem->m_EmitterStandard.fVisibleDistance);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_DECAL_VISIBLE_DISTANCE);

    // YOffset
    pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);
    pEdit->SetLabelText(_T("Y Offset"));
    pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    pEdit->SetItemValue((LPARAM)&pResourceDecalSystem->m_EmitterStandard.fYOffset);
    pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_DECAL_YOFFSET);
}   

//------------------------------------------------------------------
//	FuncName	: SetProperties_ResourceBeamSystem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::SetProperties_ResourcePostEffectSystem(CNtlResourcePostEffectSystem* pResourcePostEffectSystem, CPropTreeItem* pRoot)
{
	SetProperties_ResourceComponentSystem(pResourcePostEffectSystem, pRoot);

	SetProperties_EmitterStandard(pResourcePostEffectSystem, pRoot);
}

void CEffectPropertyDialog::SetProperties_EmitterStandard(CNtlResourcePostEffectSystem* pResourcePostEffectSystem, CPropTreeItem* pRoot)
{
	// ============================================
	// Texture
	// ============================================
	CPropTreeItem* pTextureItem = SetProperties_Textures(pResourcePostEffectSystem->m_strTextureName, pRoot);
	pTextureItem->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_TEXTURE);

    // Src Blend
    CPropTreeItemCombo* pCombo;
    pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);    
    pCombo->SetLabelText(_T("Src Blend"));    
    pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
    pCombo->SetDropDownHeight(300);
    for (RwInt32 i = 0; i < g_nBlendMaxCount; ++ i)
    {
        pCombo->AddString(_T(g_BlendString[i]));
        pCombo->SetItemData(i, i);
    }
    pCombo->SetItemValue(GetBlendIndex(pResourcePostEffectSystem->m_nSrcBlend));
    pCombo->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_ALPHA_SRC);

    // Dest Blend
    pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);	    
    pCombo->SetLabelText(_T("Dest Blend"));    
    pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
    pCombo->SetDropDownHeight(300);
    for (RwInt32 i = 0; i < g_nBlendMaxCount; ++ i)
    {        pCombo->AddString(_T(g_BlendString[i]));
        pCombo->SetItemData(i, i);
    }
    pCombo->SetItemValue(GetBlendIndex(pResourcePostEffectSystem->m_nDestBlend));
    pCombo->SetCtrlID(_PROPERTY_CONTROL_SYSTEM_ALPHA_DEST);

    // Vertex Count
    CPropTreeItemEdit* pEdit;
    pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);
    pEdit->SetLabelText(_T("Vertex Count"));
    pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);
    pEdit->SetItemValue(pResourcePostEffectSystem->m_EmitterStandard.nVertexCount);
    pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_POST_EFFECT_VERTEX_COUNT);

    // Radius
    pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);
    pEdit->SetLabelText(_T("Radius"));
    pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);
    pEdit->SetItemValue((RwInt32)pResourcePostEffectSystem->m_EmitterStandard.fRadius);
    pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_POST_EFFECT_RADIUS);

	// Draw FullScreen
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);	
	pCombo->SetLabelText(_T("Draw Full Screen"));
	pCombo->SetInfoText(_T("Draw Full Screen"));
	pCombo->CreateComboBoxBool();
	pCombo->SetItemValue(pResourcePostEffectSystem->m_EmitterStandard.bDrawFullScreen);
	pCombo->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_POST_EFFECT_DRAW_FULL_SCREEN);

    // Loop Count	
	pEdit = (CPropTreeItemEdit*)m_PropertyTree.InsertItem(new CPropTreeItemEdit(), pRoot);	
	pEdit->SetLabelText(_T("Loop Count"));
	pEdit->SetInfoText(_T("Post Effect Loop Count"));
	pEdit->SetValueFormat(CPropTreeItemEdit::ValueFormatNumber);
	pEdit->SetItemValue((LPARAM)pResourcePostEffectSystem->m_EmitterStandard.nLoopCount);
	pEdit->SetCtrlID(_PROPERTY_CONTROL_EMITTER_STANDARD_POST_EFFECT_LOOP_COUNT);

    // Center Fix
    pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);
    pCombo->SetLabelText(_T("Center Fix"));
    pCombo->CreateComboBoxBool();
    if(pResourcePostEffectSystem->m_nEmitterDataFlag & NTLrpPRTSTDEMITTERDATAFLAGPRTCENTERFIX)
    {
        pCombo->SetItemValue(TRUE);
    }
    else
    {
        pCombo->SetItemValue(FALSE);
    }

    pCombo->SetCtrlID(_PROPRETY_CONTROL_EMITTER_STANDARD_POST_EFFECT_CENTER_FIX);
}


//------------------------------------------------------------------
//	FuncName	: ApplyProperties
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectPropertyDialog::ApplyProperties(CPropTreeItem* pItem)
{
	if (CVenusVisualManager::GetInstance().m_pResourceEffect == NULL)	{ return; }

	if (CVenusVisualManager::GetInstance().m_pResourceSystem == NULL)
	{
		ApplyProperties_ResourceEffect(pItem, CVenusVisualManager::GetInstance().m_pResourceEffect);

		CVenusPropertyContainer::GetInstance().m_bSave = TRUE;

		NTL_ASSERTE(CVenusVisualManager::GetInstance().m_pResourceEffect != NULL);
		CVenusVisualManager::GetInstance().ResetInstanceEffect(CVenusVisualManager::GetInstance().m_pResourceEffect->GetName());

		return;
	}

	switch(CVenusVisualManager::GetInstance().m_pResourceSystem->GetSystemType())
	{
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_PARTICLE:
		{
			CNtlResourceParticleSystem* pResourceParticleSystem = (CNtlResourceParticleSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem;
			switch(CVenusVisualManager::GetInstance().m_nSystemDataFlag)
			{
			case 0:
				ApplyProperties_ResourceParticleSystem(pItem, pResourceParticleSystem);
				break;

			case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:
				ApplyProperties_EmitterPrtColor(pItem, (RpPrtStdEmitterPrtColor*)&pResourceParticleSystem->m_EmitterPrtCol);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTTEXCOORDS:
				ApplyProperties_EmitterPrtTexCoords(pItem, (RpPrtStdEmitterPrtTexCoords*)&pResourceParticleSystem->m_EmitterPrtTexCoords);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRT2DROTATE:
				ApplyProperties_EmitterPrt2DRotate(pItem, (RpPrtStdEmitterPrt2DRotate*)&pResourceParticleSystem->m_EmitterPrtRotate);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTSIZE:
				ApplyProperties_EmitterPrtSize(pItem, (RpPrtStdEmitterPrtSize*)&pResourceParticleSystem->m_EmitterPrtSize);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTMATRIX:
				ApplyProperties_EmitterPrtMatrix(pItem, (RpPrtStdEmitterPrtMatrix*)&pResourceParticleSystem->m_EmitterPrtMatrix, pResourceParticleSystem);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTHURRICANE:
				ApplyProperties_EmitterPrtHurricane(pItem, (SNtlPrtStdEmitterPrtHurricane*)&pResourceParticleSystem->m_EmitterPrtHurricane);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTORBITPOINT:
				ApplyProperties_EmitterPrtOrbitPoint(pItem, (SNtlPrtStdEmitterPrtOrbitPoint*)&pResourceParticleSystem->m_EmitterPrtOrbitPoint);
				break;

			case rpPRTADVEMITTERDATAFLAGPRTEMITTER:
				ApplyProperties_PrtEmitterEmitter(pItem, (RpPrtAdvPrtEmitterEmitter*)&pResourceParticleSystem->m_PrtEmitterEmitter);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTICOLOR:
				ApplyProperties_MultiColorEmitter(pItem, (RpPrtAdvPrtMultiColorEmitter*)&pResourceParticleSystem->m_EmitterPrtMultiCol);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTISIZE:
				ApplyProperties_MultiSizeEmitter(pItem, (RpPrtAdvPrtMultiSizeEmitter*)&pResourceParticleSystem->m_EmitterPrtMultiSize);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTITEXCOORDS:
				ApplyProperties_MultiTexCoordsEmitter(pItem, (RpPrtAdvEmtPrtMultiTexCoords*)&pResourceParticleSystem->m_EmitterPrtMultiTexCoords);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTITEXTURES:
				ApplyProperties_MultiTexturesEmitter(pItem, (RpPrtAdvEmtPrtMultiTextures*)&pResourceParticleSystem->m_EmitterMultiTextures);
				break;

			case rpPRTADVEMITTERDATAFLAGSPLINEPATH:
				ApplyProperties_SplinePathEmitter(pItem, (SNtlPrtEmitterSplinePath*)&pResourceParticleSystem->m_EmitterPrtSplinePath);
				break;

			case rpPRTADVEMITTERDATAFLAGPOINTLIST:
				ApplyProperties_EmitterPointList(pItem, (RpPrtAdvPointListEmitter*)&pResourceParticleSystem->m_EmitterPtList);
				break;
			case rpPRTADVEMITTERDATAFLAGCIRCLE:
				ApplyProperties_CircleEmitter(pItem, (RpPrtAdvCircleEmitter*)&pResourceParticleSystem->m_EmitterPtrCircle);
				break;
			case rpPRTADVEMITTERDATAFLAGSPHERE:
				ApplyProperties_SphereEmitter(pItem, (RpPrtAdvSphereEmitter*)&pResourceParticleSystem->m_EmitterPtrSphere);
				break;
            case rpPRTSTDEMITTERDATAFLAGTRAIL:
                ApplyProperties_EmitterTrail(pItem, (SNtlPrtStdEmitterTrail*)&pResourceParticleSystem->m_EmitterTrail);
                break;
			}
		}
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_MESH:
		{
			CNtlResourceMeshSystem* pResourceMeshSystem = (CNtlResourceMeshSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem;
			switch(CVenusVisualManager::GetInstance().m_nSystemDataFlag)
			{
			case 0:
				ApplyProperties_ResourceMeshSystem(pItem, pResourceMeshSystem);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:
				ApplyProperties_EmitterPrtColor(pItem, (RpPrtStdEmitterPrtColor*)&pResourceMeshSystem->m_EmitterPrtCol);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRT2DROTATE:
				ApplyProperties_EmitterPrt2DRotate(pItem, (SNtlPrtStdEmitterPrtRotate*)&pResourceMeshSystem->m_EmitterPrtRotate);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTSIZE:
				ApplyProperties_EmitterPrtSize(pItem, (SNtlPrtStdEmitterPrtSize*)&pResourceMeshSystem->m_EmitterPrtSize);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTICOLOR:
				ApplyProperties_MultiColorEmitter(pItem, (RpPrtAdvPrtMultiColorEmitter*)&pResourceMeshSystem->m_EmitterPrtMultiCol);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTISIZE:
				ApplyProperties_MultiSizeEmitter(pItem, (SNtlAdvMultiSizeEmitter*)&pResourceMeshSystem->m_EmitterPrtMultiSize);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTIROTATE:
				ApplyProperties_MultiRotateEmitter(pItem, (SNtlAdvMultiRotateEmitter*)&pResourceMeshSystem->m_EmitterPrtMultiRotate);
				break;

			case rpPRTADVEMITTERDATAFLAGSPLINEPATH:
				ApplyProperties_SplinePathEmitter(pItem, (SNtlPrtEmitterSplinePath*)&pResourceMeshSystem->m_EmitterPrtSplinePath);
				break;
            case rpPRTSTDEMITTERDATAFLAGPRTFINISH:
                ApplyProperties_EmitterPrtFinish(pItem, (SNtlPrtStdEmitterPrtFinish*)&pResourceMeshSystem->m_EmitterPrtFinish);
                break;
            case rpPRTSTDEMITTERDATAFLAGTRAIL:
                ApplyProperties_EmitterTrail(pItem, (SNtlPrtStdEmitterTrail*)&pResourceMeshSystem->m_EmitterTrail);
                break;
			}
		}
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_BEAM:
		{
			CNtlResourceBeamSystem* pResourceBeamSystem = (CNtlResourceBeamSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem;
			switch(CVenusVisualManager::GetInstance().m_nSystemDataFlag)
			{
			case 0:
				ApplyProperties_ResourceBeamSystem(pItem, pResourceBeamSystem);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:
				ApplyProperties_EmitterPrtColor(pItem, (RpPrtStdEmitterPrtColor*)&pResourceBeamSystem->m_EmitterPrtCol);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTSIZE:
				ApplyProperties_EmitterPrtSize(pItem, (SNtlPrtStdEmitterPrtSize*)&pResourceBeamSystem->m_EmitterPrtSize);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTFADEOUT:
				ApplyProperties_EmitterPrtFadeOut(pItem, (SNtlPrtStdEmitterPrtFadeOut*)&pResourceBeamSystem->m_EmitterPrtFadeOut);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTICOLOR:
				ApplyProperties_MultiColorEmitter(pItem, (RpPrtAdvPrtMultiColorEmitter*)&pResourceBeamSystem->m_EmitterPrtMultiCol);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTISIZE:
				ApplyProperties_MultiSizeEmitter(pItem, (SNtlAdvMultiSizeEmitter*)&pResourceBeamSystem->m_EmitterPrtMultiSize);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTITEXTURES:
				ApplyProperties_MultiTexturesEmitter(pItem, (RpPrtAdvEmtPrtMultiTextures*)&pResourceBeamSystem->m_EmitterMultiTextures);
				break;
            case rpPRTSTDEMITTERDATAFLAGPRTFINISH:
                ApplyProperties_EmitterPrtFinish(pItem, (SNtlPrtStdEmitterPrtFinish*)&pResourceBeamSystem->m_EmitterPrtFinish);
                break;
			}
		}
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_HURRICANE:
		{
			CNtlResourceHurricaneSystem* pResourceHurricaneSystem = (CNtlResourceHurricaneSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem;
			switch(CVenusVisualManager::GetInstance().m_nSystemDataFlag)
			{
			case 0:
				ApplyProperties_ResourceHurricaneSystem(pItem, pResourceHurricaneSystem);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:
				ApplyProperties_EmitterPrtColor(pItem, (RpPrtStdEmitterPrtColor*)&pResourceHurricaneSystem->m_EmitterPrtCol);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTSIZE:
				ApplyProperties_EmitterPrtSize(pItem, (SNtlPrtStdEmitterPrtSize*)&pResourceHurricaneSystem->m_EmitterPrtSize);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTICOLOR:
				ApplyProperties_MultiColorEmitter(pItem, (RpPrtAdvPrtMultiColorEmitter*)&pResourceHurricaneSystem->m_EmitterPrtMultiCol);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTISIZE:
				ApplyProperties_MultiSizeEmitter(pItem, (SNtlAdvMultiSizeEmitter*)&pResourceHurricaneSystem->m_EmitterPrtMultiSize);
				break;

			case rpPRTADVEMITTERDATAFLAGMULTITEXTURES:
				ApplyProperties_MultiTexturesEmitter(pItem, (RpPrtAdvEmtPrtMultiTextures*)&pResourceHurricaneSystem->m_EmitterMultiTextures);
				break;
            case rpPRTSTDEMITTERDATAFLAGPRTFINISH:
                ApplyProperties_EmitterPrtFinish(pItem, (SNtlPrtStdEmitterPrtFinish*)&pResourceHurricaneSystem->m_EmitterPrtFinish);
                break;
			}
		}
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LIGHTNING:
		{
			CNtlResourceLightningSystem* pResourceLIghtningSystem = (CNtlResourceLightningSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem;
			switch(CVenusVisualManager::GetInstance().m_nSystemDataFlag)
			{
			case 0:
				ApplyProperties_ResourceLightningSystem(pItem, pResourceLIghtningSystem);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:
				ApplyProperties_EmitterPrtColor(pItem, (RpPrtStdEmitterPrtColor*)&pResourceLIghtningSystem->m_EmitterPrtCol);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTSIZE:
				ApplyProperties_EmitterPrtSize(pItem, (SNtlPrtStdEmitterPrtSize*)&pResourceLIghtningSystem->m_EmitterPrtSize);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTICOLOR:
				ApplyProperties_MultiColorEmitter(pItem, (RpPrtAdvPrtMultiColorEmitter*)&pResourceLIghtningSystem->m_EmitterPrtMultiCol);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTISIZE:
				ApplyProperties_MultiSizeEmitter(pItem, (SNtlAdvMultiSizeEmitter*)&pResourceLIghtningSystem->m_EmitterPrtMultiSize);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTITEXTURES:
				ApplyProperties_MultiTexturesEmitter(pItem, (RpPrtAdvEmtPrtMultiTextures*)&pResourceLIghtningSystem->m_EmitterMultiTextures);
				break;
            case rpPRTSTDEMITTERDATAFLAGPRTFINISH:
                ApplyProperties_EmitterPrtFinish(pItem, (SNtlPrtStdEmitterPrtFinish*)&pResourceLIghtningSystem->m_EmitterPrtFinish);
                break;
			}
		}
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_DECAL:
		{
			CNtlResourceDecalSystem* pResourceDecalSystem = (CNtlResourceDecalSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem;
			switch(CVenusVisualManager::GetInstance().m_nSystemDataFlag)
			{
			case 0:
				ApplyProperties_ResourceDecalSystem(pItem, pResourceDecalSystem);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:
				ApplyProperties_EmitterPrtColor(pItem, (RpPrtStdEmitterPrtColor*)&pResourceDecalSystem->m_EmitterPrtCol);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRTSIZE:
				ApplyProperties_EmitterPrtSize(pItem, (SNtlPrtStdEmitterPrtSize*)&pResourceDecalSystem->m_EmitterPrtSize);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTICOLOR:
				ApplyProperties_MultiColorEmitter(pItem, (RpPrtAdvPrtMultiColorEmitter*)&pResourceDecalSystem->m_EmitterPrtMultiCol);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTISIZE:
				ApplyProperties_MultiSizeEmitter(pItem, (SNtlAdvMultiSizeEmitter*)&pResourceDecalSystem->m_EmitterPrtMultiSize);
				break;
			case rpPRTADVEMITTERDATAFLAGMULTITEXTURES:
				ApplyProperties_MultiTexturesEmitter(pItem, (RpPrtAdvEmtPrtMultiTextures*)&pResourceDecalSystem->m_EmitterMultiTextures);
				break;
			case rpPRTSTDEMITTERDATAFLAGPRT2DROTATE:
				ApplyProperties_EmitterPrt2DRotate(pItem, (RpPrtStdEmitterPrt2DRotate*)&pResourceDecalSystem->m_EmitterPrtRotate);
				break;
			case rpPRTSTDEMITTERDATAFLAGUVANIM:
				ApplyProperties_EmitterPrtUVAnim(pItem, &pResourceDecalSystem->m_EmitterUVAnim);
				break;
			}
		}
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_POST_EFFECT:
		{
			CNtlResourcePostEffectSystem* pResourcePostEffectSystem = (CNtlResourcePostEffectSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem;
			switch(CVenusVisualManager::GetInstance().m_nSystemDataFlag)
			{
			case 0:
				ApplyProperties_ResourcePostEffectSystem(pItem, pResourcePostEffectSystem);
				break;
            case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:
                ApplyProperties_EmitterPrtColor(pItem, (RpPrtStdEmitterPrtColor*)&pResourcePostEffectSystem->m_EmitterPrtCol);
                break;
            case rpPRTADVEMITTERDATAFLAGMULTITEXTURES:
                ApplyProperties_MultiTexturesEmitter(pItem, (RpPrtAdvEmtPrtMultiTextures*)&pResourcePostEffectSystem->m_EmitterMultiTextures);
                break;
            case rpPRTADVEMITTERDATAFLAGMULTICOLOR:
                ApplyProperties_MultiColorEmitter(pItem, (RpPrtAdvPrtMultiColorEmitter*)&pResourcePostEffectSystem->m_EmitterPrtMultiCol);
                break;
			}
		}
		break;    
    case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LINE:
        {
            CNtlResourceLineSystem* pResourceLineSystem = (CNtlResourceLineSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem;

            switch(CVenusVisualManager::GetInstance().m_nSystemDataFlag)
            {
            case 0:
                ApplyProperties_ResourceLineSystem(pItem, pResourceLineSystem);
                break;
            case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:
                ApplyProperties_EmitterPrtColor(pItem, (RpPrtStdEmitterPrtColor*)&pResourceLineSystem->m_EmitterPrtCol);
                break;
            case rpPRTSTDEMITTERDATAFLAGPRTSIZE:
                ApplyProperties_EmitterPrtSize(pItem, (SNtlPrtStdEmitterPrtSize*)&pResourceLineSystem->m_EmitterPrtSize);
                break;            
            case rpPRTADVEMITTERDATAFLAGMULTICOLOR:
                ApplyProperties_MultiColorEmitter(pItem, (RpPrtAdvPrtMultiColorEmitter*)&pResourceLineSystem->m_EmitterPrtMultiCol);
                break;
            case rpPRTADVEMITTERDATAFLAGMULTISIZE:
                ApplyProperties_MultiSizeEmitter(pItem, (SNtlAdvMultiSizeEmitter*)&pResourceLineSystem->m_EmitterPrtMultiSize);
                break;
            case rpPRTADVEMITTERDATAFLAGMULTITEXTURES:
                ApplyProperties_MultiTexturesEmitter(pItem, (RpPrtAdvEmtPrtMultiTextures*)&pResourceLineSystem->m_EmitterMultiTextures);
                break;
            case rpPRTADVEMITTERDATAFLAGSPHERE:
                ApplyProperties_SphereEmitter(pItem, (SNtlPrtStdEmitterPrtSphere*)&pResourceLineSystem->m_EmitterSphere);
                break;
            case rpPRTSTDEMITTERDATAFLAGPRT2DROTATE:
                ApplyProperties_EmitterPrt2DRotate(pItem, (SNtlPrtStdEmitterPrtRotate*)&pResourceLineSystem->m_EmitterPrtRotate);
                break;
            case rpPRTADVEMITTERDATAFLAGMULTIROTATE:
                ApplyProperties_MultiRotateEmitter(pItem, (SNtlAdvMultiRotateEmitter*)&pResourceLineSystem->m_EmitterPrtMultiRotate);
                break;
            }
        }
        break;
	default:
		NTL_ASSERTE(!"default");
		break;
	}

	CVenusPropertyContainer::GetInstance().m_bSave = TRUE;

	NTL_ASSERTE(CVenusVisualManager::GetInstance().m_pResourceEffect != NULL);
	CVenusVisualManager::GetInstance().ResetInstanceEffect(CVenusVisualManager::GetInstance().m_pResourceEffect->GetName());
}

void CEffectPropertyDialog::ApplyProperties_ResourceEffect(CPropTreeItem* pItem, CNtlResourceEffect* pResourceEffect)
{
	if (pResourceEffect == NULL)	{ return; }
	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_EFFECT_NAME:
		{
			std::string strName = (RwChar*)pItem->GetItemValue();
			if (strName.empty())
			{
				AfxMessageBox(CStringManager::GetInstance().GetString(STRING_NAME_ERROR), MB_OK);
				SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
				return;
			}

			CNtlResourceEffect* pSrcResourceEffect = (CNtlResourceEffect*)CVenusPropertyContainer::GetInstance().GetProperty(PLENTITY_EFFECT, strName.c_str());
			if (pSrcResourceEffect != pResourceEffect && pSrcResourceEffect)
			{
				AfxMessageBox(CStringManager::GetInstance().GetString(STRING_NAME_ERROR), MB_OK);
				SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
				return;
			}

			CVenusPropertyContainer::GetInstance().ChangePropertyName(PLENTITY_EFFECT, pResourceEffect->GetName(), strName.c_str());

			CEffectEditLayer* pView = (CEffectEditLayer*)gGetEditLayer(EDIT_LAYER_EFFECT_EDIT);
			pView->ResetTreeItemName(CVenusVisualManager::GetInstance().m_pResourceEffect->GetName());

//			SetTimer(TIME_TYPE_CHANGE_EFFECTNAME, 100, NULL);
		}
		break;
	case _PROPERTY_CONTROL_EFFECT_SCALE:
		{
			RwReal fScale = *(FLOAT*)pItem->GetItemValue();
			CVenusPropertyContainer::GetInstance().SetResourceEffectSize(pResourceEffect, fScale);
		}
		break;	
    case _PROPERTY_CONTROL_EFFECT_SOUND_FILE_NAME1:
        {
             if(pItem->GetItemValue() != NULL)
             {
                 sprintf_s(pResourceEffect->m_EffectSound.chSoundName, "%s", (RwChar*)pItem->GetItemValue());                
             }             
        }
        break;
    case _PROPERTY_CONTROL_EFFECT_SOUND_FILE_NAME2:
        {
            if(pItem->GetItemValue() != NULL)
            {
                sprintf_s(pResourceEffect->m_EffectSound.chSoundName2, "%s", (RwChar*)pItem->GetItemValue());                
            }             
        }
        break;
    case _PROPERTY_CONTROL_EFFECT_SOUND_FILE_NAME3:
        {
            if(pItem->GetItemValue() != NULL)
            {
                sprintf_s(pResourceEffect->m_EffectSound.chSoundName3, "%s", (RwChar*)pItem->GetItemValue());                
            }             
        }
        break;
    case _PROPERTY_CONTROL_EFFECT_SOUND_FILE_NAME4:
        {
            if(pItem->GetItemValue() != NULL)
            {
                sprintf_s(pResourceEffect->m_EffectSound.chSoundName4, "%s", (RwChar*)pItem->GetItemValue());                
            }             
        }
        break;
    case _PROPERTY_CONTROL_EFFECT_SOUND_LOOP:
        pResourceEffect->m_EffectSound.bLoop = (RwBool)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_EFFECT_SOUND_VOLUME:        
        {
            RwReal fVolume = *(RwReal*)pItem->GetItemValue();
            pResourceEffect->m_EffectSound.fSoundVolume = fVolume;            
        }
        break;
    case _PROPERTY_CONTROL_EFFECT_SOUND_DIST:
        {
            RwReal fDist = *(RwReal*)pItem->GetItemValue();
            pResourceEffect->m_EffectSound.fSoundDist = fDist;
        }
        break;
    case _PROPERTY_CONTROL_EFFECT_SOUND_DECAY_DIST:
        {
            RwReal fDecayDist = *(RwReal*)pItem->GetItemValue();
            pResourceEffect->m_EffectSound.fSoundDecayDist = fDecayDist;
        }
        break;
    case _PROPERTY_CONTROL_EFFECT_SOUND_PITCH_MIN:
        {
            RwReal fPitch = *(RwReal*)pItem->GetItemValue();
            pResourceEffect->m_EffectSound.fSoundPitchMin = fPitch;
        }
        break;        
    case _PROPERTY_CONTROL_EFFECT_SOUND_PITCH_MAX:
        {
            RwReal fPitch = *(RwReal*)pItem->GetItemValue();
            pResourceEffect->m_EffectSound.fSoundPitchMax = fPitch;
        }
        break;
    case _PROPERTY_CONTROL_EFFECT_BOUNDING_NOTGENERATE:
        pResourceEffect->m_EffectBoundingSphere.bDisableAuto = (RwBool)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_EFFECT_BOUNDING_RADIUS:
        pResourceEffect->m_EffectBoundingSphere.fRadius = *(RwReal*)pItem->GetItemValue();
        break;            
	}
}

void CEffectPropertyDialog::ApplyProperties_ResourceComponentSystem(CPropTreeItem* pItem, CNtlResourceComponentSystem*	pResourceComponentSystem)
{
	if (pResourceComponentSystem == NULL)	{ return; }
	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_SYSTEM_NAME:
		{
			pResourceComponentSystem->SetSystemName((RwChar*)pItem->GetItemValue());

			CEffectEditLayer* pView = (CEffectEditLayer*)gGetEditLayer(EDIT_LAYER_EFFECT_EDIT);
			pView->ResetTreeItemName(CVenusVisualManager::GetInstance().m_pResourceSystem->GetSystemName());

//			SetTimer(TIME_TYPE_CHANGE_SYSTEMNAME, 100, NULL);
		}
		break;
	case _PROPERTY_CONTROL_SYSTEM_ETERNITY:
		pResourceComponentSystem->m_bEternity = (BOOL)pItem->GetItemValue();
		break;
	case _PROPERTY_CONTROL_SYSTEM_START_TIME:
		pResourceComponentSystem->m_fStartTime = *(FLOAT*)pItem->GetItemValue();
		break;
	case _PROPERTY_CONTROL_SYSTEM_LIFE_TIME:
		pResourceComponentSystem->m_fLifeTime = *(FLOAT*)pItem->GetItemValue();
		break;
	case _PROPERTY_CONTROL_SYSTEM_MATRIX_POSITION_X:
		pResourceComponentSystem->m_vPosition.x = *(FLOAT*)pItem->GetItemValue();
		pResourceComponentSystem->RebuildMatrix();
		break;
	case _PROPERTY_CONTROL_SYSTEM_MATRIX_POSITION_Y:
		pResourceComponentSystem->m_vPosition.y = *(FLOAT*)pItem->GetItemValue();
		pResourceComponentSystem->RebuildMatrix();
		break;
	case _PROPERTY_CONTROL_SYSTEM_MATRIX_POSITION_Z:
		pResourceComponentSystem->m_vPosition.z = *(FLOAT*)pItem->GetItemValue();
		pResourceComponentSystem->RebuildMatrix();
		break;
	case _PROPERTY_CONTROL_SYSTEM_MATRIX_ROTATE_X:
		pResourceComponentSystem->m_vRotate.x = (RwReal)pItem->GetItemValue();
		pResourceComponentSystem->RebuildMatrix();
		break;
	case _PROPERTY_CONTROL_SYSTEM_MATRIX_ROTATE_Y:
		pResourceComponentSystem->m_vRotate.y = (RwReal)pItem->GetItemValue();
		pResourceComponentSystem->RebuildMatrix();
		break;
	case _PROPERTY_CONTROL_SYSTEM_MATRIX_ROTATE_Z:
		pResourceComponentSystem->m_vRotate.z = (RwReal)pItem->GetItemValue();
		pResourceComponentSystem->RebuildMatrix();
		break;
    case _PROPERTY_CONTROL_SYSTEM_GROUND_ATTACH:
        pResourceComponentSystem->m_sGroundFlag.bGround = (BOOL)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_SYSTEM_GROUND_OFFSET:
        pResourceComponentSystem->m_sGroundFlag.fOffsetY = *(FLOAT*)pItem->GetItemValue();
        break;
	}
}

void CEffectPropertyDialog::ApplyProperties_ResourceParticleSystem(CPropTreeItem* pItem, CNtlResourceParticleSystem* pResourceParticleSystem)
{
	if (pResourceParticleSystem == NULL)	{ return; }

	ApplyProperties_ResourceComponentSystem(pItem, pResourceParticleSystem);

	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_SYSTEM_ALPHA_ENALBE:
		{
			pResourceParticleSystem->m_bAlphaBlend = (BOOL)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_SYSTEM_ALPHA_SRC:
		{
			pResourceParticleSystem->m_nSrcBlend = BlendFunctions[(RwInt32)pItem->GetItemValue()];
		}
		break;
	case _PROPERTY_CONTROL_SYSTEM_ALPHA_DEST:
		{
			pResourceParticleSystem->m_nDestBlend = BlendFunctions[(RwInt32)pItem->GetItemValue()];
		}
		break;
    case _PROPERTY_CONTROL_SYSTEM_FOLLOW:
        {
            pResourceParticleSystem->SetFollowFlag((BOOL)pItem->GetItemValue());
        }
        break;
	}

	ApplyProperties_EmitterStandard(pItem, pResourceParticleSystem);
}


void CEffectPropertyDialog::ApplyProperties_ResourceMeshSystem(CPropTreeItem* pItem, CNtlResourceMeshSystem* pResourceMeshSystem)
{
	if (pResourceMeshSystem == NULL)	{ return; }

	ApplyProperties_ResourceComponentSystem(pItem, pResourceMeshSystem);

	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_SYSTEM_MESH_FILE_NAME:
		{
			if (pItem->GetItemValue() != NULL)
			{
				std::string strFileName = (RwChar*)pItem->GetItemValue();
				NTL_ASSERTE(CNtlEffectSystemFreeList::m_ClumpFileManager.IsFile(strFileName.c_str()));
				pResourceMeshSystem->m_strMeshFileName = strFileName.c_str();
			}
			else
			{
				pResourceMeshSystem->m_strMeshFileName.clear();
			}

			// Mesh 가 바뀌면 연결했던 animation 파일과 uv animation 파일을 해제한다.
			if (!pResourceMeshSystem->m_strAniFileName.empty() || !pResourceMeshSystem->m_strUvFileName.empty())
			{
				pResourceMeshSystem->m_strAniFileName.clear();
				pResourceMeshSystem->m_strUvFileName.clear();
			}
			SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
		}
		break;
	case _PROPERTY_CONTROL_SYSTEM_ANIMATION_FILE_NAME:
		{
			if (pItem->GetItemValue() != NULL)
			{
				std::string strFileName = (RwChar*)pItem->GetItemValue();
				NTL_ASSERTE(CNtlEffectSystemFreeList::m_AnimationFileManager.IsFile(strFileName.c_str()));
				pResourceMeshSystem->m_strAniFileName = strFileName.c_str();
			}
			else
			{
				pResourceMeshSystem->m_strAniFileName.clear();
			}
		}
		break;
	case _PROPERTY_CONTROL_SYSTEM_UVANIMATION_FILE_NAME:
		{
			if (pItem->GetItemValue() != NULL)
			{
				std::string strFileName = (RwChar*)pItem->GetItemValue();
				pResourceMeshSystem->m_strUvFileName = strFileName.c_str();
			}
			else
			{
				pResourceMeshSystem->m_strUvFileName.clear();
			}
		}
		break;

	case _PROPERTY_CONTROL_SYSTEM_ALPHA_ENALBE:
		{
			pResourceMeshSystem->m_bAlphaBlend = (BOOL)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_SYSTEM_ALPHA_SRC:
		{
			pResourceMeshSystem->m_nSrcBlend = BlendFunctions[(RwInt32)pItem->GetItemValue()];
		}
		break;
	case _PROPERTY_CONTROL_SYSTEM_ALPHA_DEST:
		{
			pResourceMeshSystem->m_nDestBlend = BlendFunctions[(RwInt32)pItem->GetItemValue()];
		}
		break;

	}
	ApplyProperties_EmitterStandard(pItem, pResourceMeshSystem);
}

void CEffectPropertyDialog::ApplyProperties_ResourceImVertexSystem(CPropTreeItem* pItem, CNtlResourceImVertexSystem* pResourceImVertexSystem)
{
	if (pResourceImVertexSystem == NULL)	{ return; }

	ApplyProperties_ResourceComponentSystem(pItem, pResourceImVertexSystem);

	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_SYSTEM_ALPHA_ENALBE:
		{
			pResourceImVertexSystem->m_bAlphaBlend = (BOOL)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_SYSTEM_ALPHA_SRC:
		{
			pResourceImVertexSystem->m_nSrcBlend = BlendFunctions[(RwInt32)pItem->GetItemValue()];
		}
		break;
	case _PROPERTY_CONTROL_SYSTEM_ALPHA_DEST:
		{
			pResourceImVertexSystem->m_nDestBlend = BlendFunctions[(RwInt32)pItem->GetItemValue()];
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_TEXTURE:
		{
			if (pItem->GetItemValue() != NULL)
			{
				pResourceImVertexSystem->m_strTextureName = (RwChar*)pItem->GetItemValue();
			}
			else
			{
				pResourceImVertexSystem->m_strTextureName.clear();
			}
		}
		break;
	}
}

void CEffectPropertyDialog::ApplyProperties_ResourceBeamSystem(CPropTreeItem* pItem, CNtlResourceBeamSystem* pResourceBeamSystem)
{
	if (pResourceBeamSystem == NULL)	{ return; }

	ApplyProperties_ResourceImVertexSystem(pItem, pResourceBeamSystem);

	ApplyProperties_EmitterStandard(pItem, pResourceBeamSystem);
}

void CEffectPropertyDialog::ApplyProperties_ResourceHurricaneSystem(CPropTreeItem* pItem, CNtlResourceHurricaneSystem* pResourceHurricaneSystem)
{
	if (pResourceHurricaneSystem == NULL)	{ return; }

	ApplyProperties_ResourceImVertexSystem(pItem, pResourceHurricaneSystem);

	ApplyProperties_EmitterStandard(pItem, pResourceHurricaneSystem);
}

void CEffectPropertyDialog::ApplyProperties_ResourceLightningSystem(CPropTreeItem* pItem, CNtlResourceLightningSystem* pResourceLightningSystem)
{
	if (pResourceLightningSystem == NULL)	{ return; }

	ApplyProperties_ResourceImVertexSystem(pItem, pResourceLightningSystem);

	ApplyProperties_EmitterStandard(pItem, pResourceLightningSystem);

    switch(pItem->GetCtrlID())
    {
    case _PROPERTY_CONTROL_EMITTER_STANDARD_LIHGTNING_ENDPOINT_X:
        {
            //RwV3d vEndPoint = pResourceLightningSystem->GetEndPosition();
            //vEndPoint.x = *(FLOAT*)pItem->GetItemValue();
            //pResourceLightningSystem->SetEndPosition(vEndPoint);
            pResourceLightningSystem->m_vRotate.x = *(FLOAT*)pItem->GetItemValue();
        }
        break;
    case _PROPERTY_CONTROL_EMITTER_STANDARD_LIHGTNING_ENDPOINT_Y:
        {
            //RwV3d vEndPoint = pResourceLightningSystem->GetEndPosition();
            //vEndPoint.y = *(FLOAT*)pItem->GetItemValue();
            //pResourceLightningSystem->SetEndPosition(vEndPoint);

            pResourceLightningSystem->m_vRotate.y = *(FLOAT*)pItem->GetItemValue();
        }
        break;
    case _PROPERTY_CONTROL_EMITTER_STANDARD_LIHGTNING_ENDPOINT_Z:
        {
            //RwV3d vEndPoint = pResourceLightningSystem->GetEndPosition();
            //vEndPoint.z = *(FLOAT*)pItem->GetItemValue();
            //pResourceLightningSystem->SetEndPosition(vEndPoint);

            pResourceLightningSystem->m_vRotate.z = *(FLOAT*)pItem->GetItemValue();
        }
        break;
    }
}

void CEffectPropertyDialog::ApplyProperties_ResourceLineSystem( CPropTreeItem* pItem, CNtlResourceLineSystem* pResourceLineSystem )
{
    if(!pResourceLineSystem)
        return;

    ApplyProperties_ResourceImVertexSystem(pItem, pResourceLineSystem);
    ApplyProperties_EmitterStandard(pItem, pResourceLineSystem);

}

void CEffectPropertyDialog::ApplyProperties_ResourceDecalSystem(CPropTreeItem* pItem, CNtlResourceDecalSystem* pResourceDecalSystem)
{
	if (pResourceDecalSystem == NULL)	{ return; }

	ApplyProperties_ResourceImVertexSystem(pItem, pResourceDecalSystem);

	ApplyProperties_EmitterStandard(pItem, pResourceDecalSystem);
}

void CEffectPropertyDialog::ApplyProperties_ResourcePostEffectSystem(CPropTreeItem* pItem, CNtlResourcePostEffectSystem* pResourcePostEffectSystem)
{
	if (pResourcePostEffectSystem == NULL)	{ return; }

	ApplyProperties_ResourceComponentSystem(pItem, pResourcePostEffectSystem);

	ApplyProperties_EmitterStandard(pItem, pResourcePostEffectSystem);
}


void CEffectPropertyDialog::ApplyProperties_EmitterStandard(CPropTreeItem* pItem, CNtlResourceParticleSystem* pResourceParticleSystem)
{
	if (pResourceParticleSystem == NULL)	{ return; }

	RpPrtStdEmitterStandard* pEmitterStandard = &pResourceParticleSystem->m_EmitterStandard;

	switch(pItem->GetCtrlID()) 
	{
    case _PROPERTY_CONTROL_SYSTEM_ALPHA_SRC:
        {
            pResourceParticleSystem->m_nSrcBlend = BlendFunctions[(RwInt32)pItem->GetItemValue()];
        }
        break;
    case _PROPERTY_CONTROL_SYSTEM_ALPHA_DEST:
        {
            pResourceParticleSystem->m_nDestBlend = BlendFunctions[(RwInt32)pItem->GetItemValue()];
        }
        break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_MAX_PARTICLE_COUNT:
		{
			pEmitterStandard->maxPrt = (RwInt32)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_TEXTURE:
		{
			if (pItem->GetItemValue() != NULL)
			{
				pResourceParticleSystem->m_strTextureName = (RwChar*)pItem->GetItemValue();
			}
			else
			{
				pResourceParticleSystem->m_strTextureName.clear();
			}
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_ROTATE:
		{
			pEmitterStandard->prtDelta2DRotate = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_PTR_SIZE_X:
		{
			pEmitterStandard->prtSize.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_PTR_SIZE_Y:
		{
			pEmitterStandard->prtSize.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_PTR_COLOR:
		{
			COLORREF color = (COLORREF)pItem->GetItemValue();
			pEmitterStandard->prtColor.red = GetRValue(color);
			pEmitterStandard->prtColor.green = GetGValue(color);
			pEmitterStandard->prtColor.blue = GetBValue(color);
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_PTR_COLOR_A:
		{
			// slider 이기 때문에 (RwReal)pItem->GetItemValue(); 으로 처리해야 한다.
			pEmitterStandard->prtColor.alpha = (RwUInt8)pItem->GetItemValue();
		}
		break;


	case _PROPERTY_CONTROL_EMITTER_STANDARD_EMITTED_SIZE_X:
		{
			pEmitterStandard->emtSize.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_EMITTED_SIZE_Y:
		{
			pEmitterStandard->emtSize.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_EMITTED_SIZE_Z:
		{
			pEmitterStandard->emtSize.z = *(FLOAT*)pItem->GetItemValue();
		}
		break;

	case _PROPERTY_CONTROL_EMITTER_STANDARD_EMITTED_COUNT:
		{
			pEmitterStandard->emtPrtEmit = (RwInt32)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_EMITTED_COUNT_BIAS:
		{
			pEmitterStandard->emtPrtEmitBias = (RwInt32)pItem->GetItemValue();
		}
		break;

	case _PROPERTY_CONTROL_EMITTER_STANDARD_EMITTED_GAP:
		{
			pEmitterStandard->emtEmitGap = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_EMITTED_GAP_BIAS:
		{
			pEmitterStandard->emtEmitGapBias = *(FLOAT*)pItem->GetItemValue();
		}
		break;

	case _PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_LIFE:
		{
			pEmitterStandard->prtLife = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_LIFE_BIAS:
		{
			pEmitterStandard->prtLifeBias = *(FLOAT*)pItem->GetItemValue();
		}
		break;

	case _PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_VELOCITY:
		{
			pEmitterStandard->prtInitVel = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_VELOCITY_BIAS:
		{
			pEmitterStandard->prtInitVelBias = *(FLOAT*)pItem->GetItemValue();
		}
		break;

	case _PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_DIRECTION_X:
		{
			pEmitterStandard->prtInitDir.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_DIRECTION_Y:
		{
			pEmitterStandard->prtInitDir.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_DIRECTION_Z:
		{
			pEmitterStandard->prtInitDir.z = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_DIRECTION_BIAS_X:
		{
			pEmitterStandard->prtInitDirBias.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_DIRECTION_BIAS_Y:
		{
			pEmitterStandard->prtInitDirBias.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_DIRECTION_BIAS_Z:
		{
			pEmitterStandard->prtInitDirBias.z = *(FLOAT*)pItem->GetItemValue();
		}
		break;

	case _PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_FORCE_X:
		{
			pEmitterStandard->force.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_FORCE_Y:
		{
			pEmitterStandard->force.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_FORCE_Z:
		{
			pEmitterStandard->force.z = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_ZWRITE:
		{
			if(pItem->GetItemValue())
			{
				pResourceParticleSystem->m_nEmitterDataFlag |= NTLrpPRTSTDEMITTERDATAFLAGPRTZWRITE;
			}
			else
			{
				pResourceParticleSystem->m_nEmitterDataFlag &= ~NTLrpPRTSTDEMITTERDATAFLAGPRTZWRITE;
			}
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_PRTICLE_DEPTHBIAS:
		{
			pResourceParticleSystem->m_fDepthBias = *(float*)pItem->GetItemValue();
		}
		break;
	}
}

void CEffectPropertyDialog::ApplyProperties_EmitterStandard(CPropTreeItem* pItem, CNtlResourceMeshSystem* pResourceMeshSystem)
{
	if (pResourceMeshSystem == NULL)	{ return; }

	SNtlMeshEmitterStandard* pEmitterStandard = &pResourceMeshSystem->m_EmitterStandard;
	switch(pItem->GetCtrlID()) 
	{
    case _PROPERTY_CONTROL_SYSTEM_MESH_PROJECTILE:
        if((BOOL)pItem->GetItemValue())
        {
            pResourceMeshSystem->m_nEmitterDataFlag |= NTLrpPRTADVEMITTERDATAFLAGFOLLOW;
        }
        else
        {
            pResourceMeshSystem->m_nEmitterDataFlag ^= NTLrpPRTADVEMITTERDATAFLAGFOLLOW;
        }
        break;

	case _PROPERTY_CONTROL_SYSTEM_MESH_ZWRITE_ENABLE:
		{
			pEmitterStandard->bWriteEnable = (BOOL)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_MESH_COLOR:
		{
			COLORREF color = (COLORREF)pItem->GetItemValue();
			pEmitterStandard->prtColor.red = GetRValue(color);
			pEmitterStandard->prtColor.green = GetGValue(color);
			pEmitterStandard->prtColor.blue = GetBValue(color);
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_MESH_COLOR_A:
		{
			// slider 이기 때문에 (RwReal)pItem->GetItemValue(); 으로 처리해야 한다.
			pEmitterStandard->prtColor.alpha = (RwUInt8)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_MESH_SIZE_X:
		{
			pEmitterStandard->prtSize.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_MESH_SIZE_Y:
		{
			pEmitterStandard->prtSize.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_MESH_SIZE_Z:
		{
			pEmitterStandard->prtSize.z = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_MESH_ROTATE_X:
		{
			pEmitterStandard->prtRotate.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_MESH_ROTATE_Y:
		{
			pEmitterStandard->prtRotate.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_MESH_ROTATE_Z:
		{
			pEmitterStandard->prtRotate.z = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	}
}

void CEffectPropertyDialog::ApplyProperties_EmitterStandard(CPropTreeItem* pItem, CNtlResourceBeamSystem* pResourceBeamSystem)
{
	if (pResourceBeamSystem == NULL)	{ return; }

	SNtlBeamEmitterStandard* pEmitterStandard = &pResourceBeamSystem->m_EmitterStandard;
	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_SPEED:
		{
			pEmitterStandard->fSpeed = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_COLOR:
		{
			COLORREF color = (COLORREF)pItem->GetItemValue();
			pEmitterStandard->prtColor.red = GetRValue(color);
			pEmitterStandard->prtColor.green = GetGValue(color);
			pEmitterStandard->prtColor.blue = GetBValue(color);
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_COLOR_A:
		{
			// slider 이기 때문에 (RwReal)pItem->GetItemValue(); 으로 처리해야 한다.
			pEmitterStandard->prtColor.alpha = (RwUInt8)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_SIZE_X:
		{
			pEmitterStandard->prtSize.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_SIZE_Y:
		{
			pEmitterStandard->prtSize.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_SIZE_Z:
		{
			pEmitterStandard->prtSize.z = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_BEAM_HEAD_SYSTEM:
		{
			// 우선 임시로 여기서 캐스팅한다. 차후 수정해야만 한다
			NTL_ASSERTE(CVenusVisualManager::GetInstance().m_pResourceSystem != NULL);
			CNtlResourceBeamSystem* pResourceBeamSystem = (CNtlResourceBeamSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem;
			NTL_ASSERTE(pResourceBeamSystem);

			CNtlResourceComponentSystem* pResourceSystem = (CNtlResourceComponentSystem*)pItem->GetItemValue();
			if (pResourceSystem != NULL)
			{
				strcpy(pResourceBeamSystem->m_EmitterStandard.strHeadSystem, pResourceSystem->GetSystemName());
			}
			else
			{
				memset(pResourceBeamSystem->m_EmitterStandard.strHeadSystem, 0, MAX_PATH);
			}
		}
		break;
	}
}

void CEffectPropertyDialog::ApplyProperties_EmitterStandard(CPropTreeItem* pItem, CNtlResourceHurricaneSystem* pResourceHurricaneSystem)
{
	if (pResourceHurricaneSystem == NULL)	{ return; }

	SNtlHurricaneEmitterStandard* pEmitterStandard = &pResourceHurricaneSystem->m_EmitterStandard;
	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_EMITTER_STANDARD_HURRICANE_ETERNITY:
		{
			pEmitterStandard->bEternity = (BOOL)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_SPEED:
		{
			pEmitterStandard->fSpeed = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_COLOR:
		{
			COLORREF color = (COLORREF)pItem->GetItemValue();
			pEmitterStandard->prtColor.red = GetRValue(color);
			pEmitterStandard->prtColor.green = GetGValue(color);
			pEmitterStandard->prtColor.blue = GetBValue(color);
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_COLOR_A:
		{
			// slider 이기 때문에 (RwReal)pItem->GetItemValue(); 으로 처리해야 한다.
			pEmitterStandard->prtColor.alpha = (RwUInt8)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_SIZE_X:
		{
			pEmitterStandard->prtSize.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_HURRICANE_MAX_POLYGON_COUNT:
		{
			pEmitterStandard->maxPolygonCount = (RwUInt32)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_HURRICANE_LIFE_TIME:
		{
			pEmitterStandard->lifeTime = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_HURRICANE_RADIUS:
		{
			pEmitterStandard->radius = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_HURRICANE_RADIUSGAP:
		{
			pEmitterStandard->radiusGap = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_HURRICANE_REVOLUTION_COUNT:
		{
			pEmitterStandard->revolutionCount = (RwUInt32)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_HURRICANE_REVOLUTION_ANGLE:
		{
			pEmitterStandard->revolutionAngle = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	}
}

void CEffectPropertyDialog::ApplyProperties_EmitterStandard(CPropTreeItem* pItem, CNtlResourceLightningSystem* pResourceLightningSystem)
{
	if (pResourceLightningSystem == NULL)	{ return; }

	SNtlLightningEmitterStandard* pEmitterStandard = &pResourceLightningSystem->m_EmitterStandard;
	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_COLOR:
		{
			COLORREF color = (COLORREF)pItem->GetItemValue();
			pEmitterStandard->prtColor.red = GetRValue(color);
			pEmitterStandard->prtColor.green = GetGValue(color);
			pEmitterStandard->prtColor.blue = GetBValue(color);
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_COLOR_A:
		{
			// slider 이기 때문에 (RwReal)pItem->GetItemValue(); 으로 처리해야 한다.
			pEmitterStandard->prtColor.alpha = (RwUInt8)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_SIZE_X:
		{
			pEmitterStandard->prtSize.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_SIZE_Y:
		{
			pEmitterStandard->prtSize.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_SIZE_Z:
		{
			pEmitterStandard->prtSize.z = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_LIHGTNING_UPDATE_TIME:
		{
			pEmitterStandard->fUpdateTime = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_LIHGTNING_AMP:
		{
			pEmitterStandard->fAmp = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_LIHGTNING_ROUGH:
		{
			pEmitterStandard->fRough = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	}
}

void CEffectPropertyDialog::ApplyProperties_EmitterStandard(CPropTreeItem* pItem, CNtlResourceDecalSystem* pResourceDecalSystem)
{
	if (pResourceDecalSystem == NULL)	{ return; }

	SNtlDecalEmitterStandard* pEmitterStandard = &pResourceDecalSystem->m_EmitterStandard;
	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_COLOR:
		{
			COLORREF color = (COLORREF)pItem->GetItemValue();			
            pEmitterStandard->color.red = GetRValue(color);
            pEmitterStandard->color.green = GetGValue(color);
            pEmitterStandard->color.blue = GetBValue(color);
		}
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_IMVERTEX_COLOR_A:
		{
			// slider 이기 때문에 (RwReal)pItem->GetItemValue(); 으로 처리해야 한다.
			pEmitterStandard->color.alpha = (RwUInt8)pItem->GetItemValue();
		}
		break;	
	case _PROPERTY_CONTROL_EMITTER_STANDARD_DECAL_SIZE_X:
		pEmitterStandard->vSize.x = *(float*)pItem->GetItemValue();
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_DECAL_SIZE_Z:
		pEmitterStandard->vSize.z = *(float*)pItem->GetItemValue();
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_DECAL_SIZE_SCALE:
		pEmitterStandard->fScale = *(FLOAT*)pItem->GetItemValue();
		break;			
	case _PROPERTY_CONTROL_EMITTER_STANDARD_DECAL_WRAP_TYPE:
		pEmitterStandard->eWrapType = (RwTextureAddressMode)(pItem->GetItemValue() + 1);
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_DECAL_VISIBLE_DISTANCE:
		pEmitterStandard->fVisibleDistance = *(FLOAT*)pItem->GetItemValue();
		break;
    case _PROPERTY_CONTROL_EMITTER_STANDARD_DECAL_YOFFSET:
        pEmitterStandard->fYOffset = *(FLOAT*)pItem->GetItemValue();
        break;
	}
}

void CEffectPropertyDialog::ApplyProperties_EmitterStandard(CPropTreeItem* pItem, CNtlResourcePostEffectSystem* pResourcePostEffectSystem)
{
	if (pResourcePostEffectSystem == NULL)	{ return; }

	SNtlPostEffectEmitterStandard* pEmitterStandard = &pResourcePostEffectSystem->m_EmitterStandard;
	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_EMITTER_STANDARD_TEXTURE:
		if (pItem->GetItemValue() != NULL)
		{
			pResourcePostEffectSystem->m_strTextureName = (RwChar*)pItem->GetItemValue();
		}
		else
		{
			pResourcePostEffectSystem->m_strTextureName.clear();
		}
		break;
    case _PROPERTY_CONTROL_SYSTEM_ALPHA_SRC:
        pResourcePostEffectSystem->m_nSrcBlend = BlendFunctions[(RwInt32)pItem->GetItemValue()];
        break;
    case _PROPERTY_CONTROL_SYSTEM_ALPHA_DEST:
        pResourcePostEffectSystem->m_nDestBlend = BlendFunctions[(RwInt32)pItem->GetItemValue()];
        break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_POST_EFFECT_DRAW_FULL_SCREEN:
		pResourcePostEffectSystem->m_EmitterStandard.bDrawFullScreen = (RwBool)pItem->GetItemValue();
		break;
	case _PROPERTY_CONTROL_EMITTER_STANDARD_POST_EFFECT_LOOP_COUNT:
		pResourcePostEffectSystem->m_EmitterStandard.nLoopCount = (RwInt32)pItem->GetItemValue();
		break;
    case _PROPERTY_CONTROL_EMITTER_STANDARD_POST_EFFECT_VERTEX_COUNT:
        pResourcePostEffectSystem->m_EmitterStandard.nVertexCount = (RwInt32)pItem->GetItemValue();        
        break;
    case _PROPERTY_CONTROL_EMITTER_STANDARD_POST_EFFECT_RADIUS:
        pResourcePostEffectSystem->m_EmitterStandard.fRadius = (RwReal)(RwInt32)pItem->GetItemValue();
        break;
    case _PROPRETY_CONTROL_EMITTER_STANDARD_POST_EFFECT_CENTER_FIX:
        if(pItem->GetItemValue())
        {
            pResourcePostEffectSystem->m_nEmitterDataFlag |= NTLrpPRTSTDEMITTERDATAFLAGPRTCENTERFIX;
        }
        else
        {
            pResourcePostEffectSystem->m_nEmitterDataFlag &= ~NTLrpPRTSTDEMITTERDATAFLAGPRTCENTERFIX;
        }
        break;
	}
}

void CEffectPropertyDialog::ApplyProperties_EmitterStandard( CPropTreeItem* pItem, CNtlResourceLineSystem* pResourceLineSystem )
{
    if(!pResourceLineSystem)
        return;

    switch(pItem->GetCtrlID())
    {
    case _PROPERTY_CONTROL_EMITTER_STANDARD_LINE_LIFETIME:
        pResourceLineSystem->m_EmitterStandard.m_fLineLifeTime = *(RwReal*)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_EMITTER_STANDARD_LINE_MAX_COUNT:
        pResourceLineSystem->m_EmitterStandard.m_nMaxCount = (RwInt32)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_EMITTER_STANDARD_LINE_EMITTER_COUNT:
        pResourceLineSystem->m_EmitterStandard.m_nEmitterCount = (RwInt32)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_EMITTER_STANDARD_LINE_EMITTER_GAP:
        pResourceLineSystem->m_EmitterStandard.m_fEmitterGap = *(RwReal*)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_EMITTER_STANDARD_LINE_EMITTER_RADIUS:
        pResourceLineSystem->m_EmitterStandard.m_fEmitterRaduis = *(RwReal*)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_EMITTER_STANDARD_LINE_EMITTER_BIAS:
        pResourceLineSystem->m_EmitterStandard.m_fEmitterBias = *(RwReal*)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_EMITTER_STANDARD_LINE_TARGET_RADIUS:
        pResourceLineSystem->m_EmitterStandard.m_fTargetRadius = *(RwReal*)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_EMITTER_STANDARD_LINE_SIZE_X:
        pResourceLineSystem->m_EmitterStandard.m_sizeLine.x = *(RwReal*)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_EMITTER_STANDARD_LINE_SIZE_Y:
        pResourceLineSystem->m_EmitterStandard.m_sizeLine.y = *(RwReal*)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_EMITTER_STANDARD_LINE_VELOCITY:
        pResourceLineSystem->m_EmitterStandard.m_fVelocity = *(RwReal*)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_EMITTER_STANDARD_LINE_SHAKE:
        pResourceLineSystem->m_EmitterStandard.m_bShake = (RwBool)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_EMITTER_STANDARD_LINE_SHAKE_BIAS:
        pResourceLineSystem->m_EmitterStandard.m_fShakeBias = *(RwReal*)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_EMITTER_STANDARD_LINE_MOVE_LINE:
        pResourceLineSystem->m_EmitterStandard.m_bMoveLine = (RwBool)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_EMITTER_STANDARD_LINE_ZBUFFER:
        pResourceLineSystem->m_EmitterStandard.m_bZBufferEnable = (RwBool)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_EMITTER_STANDARD_LINE_ZBILLBOARD:
        pResourceLineSystem->m_EmitterStandard.m_bZBiilBoard = (RwBool)pItem->GetItemValue();
        break;
    }
}
void CEffectPropertyDialog::ApplyProperties_EmitterPrtColor(CPropTreeItem* pItem, RpPrtStdEmitterPrtColor* pEmitterPrtCol)
{
	if (pEmitterPrtCol == NULL)	{ return; }

	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_COLOR_START_COLOR:
		{
			COLORREF color = (COLORREF)pItem->GetItemValue();
			pEmitterPrtCol->prtStartCol.red = GetRValue(color);
			pEmitterPrtCol->prtStartCol.green = GetGValue(color);
			pEmitterPrtCol->prtStartCol.blue = GetBValue(color);
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_COLOR_START_COLOR_A:
		{
			// slider 이기 때문에 (RwReal)pItem->GetItemValue(); 으로 처리해야 한다.
			pEmitterPrtCol->prtStartCol.alpha = (RwReal)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_COLOR_START_COLOR_BIAS_R:
		{
			pEmitterPrtCol->prtStartColBias.red = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_COLOR_START_COLOR_BIAS_G:
		{
			pEmitterPrtCol->prtStartColBias.green = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_COLOR_START_COLOR_BIAS_B:
		{
			pEmitterPrtCol->prtStartColBias.blue = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_COLOR_START_COLOR_BIAS_A:
		{
			pEmitterPrtCol->prtStartColBias.alpha = *(FLOAT*)pItem->GetItemValue();
		}
		break;

	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_COLOR_END_COLOR:
		{
			COLORREF color = (COLORREF)pItem->GetItemValue();
			pEmitterPrtCol->prtEndCol.red = GetRValue(color);
			pEmitterPrtCol->prtEndCol.green = GetGValue(color);
			pEmitterPrtCol->prtEndCol.blue = GetBValue(color);
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_COLOR_END_COLOR_A:
		{
			pEmitterPrtCol->prtEndCol.alpha = (RwReal)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_COLOR_END_COLOR_BIAS_R:
		{
			pEmitterPrtCol->prtEndColBias.red = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_COLOR_END_COLOR_BIAS_G:
		{
			pEmitterPrtCol->prtEndColBias.green = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_COLOR_END_COLOR_BIAS_B:
		{
			pEmitterPrtCol->prtEndColBias.blue = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_COLOR_END_COLOR_BIAS_A:
		{
			pEmitterPrtCol->prtEndColBias.alpha = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	}
}

void CEffectPropertyDialog::ApplyProperties_EmitterPrtTexCoords(CPropTreeItem* pItem, RpPrtStdEmitterPrtTexCoords* pEmitterPrtTexCoords)
{
	if (pEmitterPrtTexCoords == NULL)	{ return; }

	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV0_START_U:
		{
			pEmitterPrtTexCoords->prtStartUV0.u = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV0_START_V:
		{
			pEmitterPrtTexCoords->prtStartUV0.v = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV0_START_BIAS_U:
		{
			pEmitterPrtTexCoords->prtStartUV0Bias.u = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV0_START_BIAS_V:
		{
			pEmitterPrtTexCoords->prtStartUV0Bias.v = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV1_START_U:
		{
			pEmitterPrtTexCoords->prtStartUV1.u = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV1_START_V:
		{
			pEmitterPrtTexCoords->prtStartUV1.v = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV1_START_BIAS_U:
		{
			pEmitterPrtTexCoords->prtStartUV1Bias.u = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV1_START_BIAS_V:
		{
			pEmitterPrtTexCoords->prtStartUV1Bias.v = *(FLOAT*)pItem->GetItemValue();
		}
		break;

	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV0_END_U:
		{
			pEmitterPrtTexCoords->prtEndUV0.u = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV0_END_V:
		{
			pEmitterPrtTexCoords->prtEndUV0.v = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV0_END_BIAS_U:
		{
			pEmitterPrtTexCoords->prtEndUV0Bias.u = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV0_END_BIAS_V:
		{
			pEmitterPrtTexCoords->prtEndUV0Bias.v = *(FLOAT*)pItem->GetItemValue();
		}
		break;

	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV1_END_U:
		{
			pEmitterPrtTexCoords->prtEndUV1.u = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV1_END_V:
		{
			pEmitterPrtTexCoords->prtEndUV1.v = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV1_END_BIAS_U:
		{
			pEmitterPrtTexCoords->prtEndUV1Bias.u = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_TEXCOOORS_UV1_END_BIAS_V:
		{
			pEmitterPrtTexCoords->prtEndUV1Bias.v = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	}
}

void CEffectPropertyDialog::ApplyProperties_EmitterPrt2DRotate(CPropTreeItem* pItem, RpPrtStdEmitterPrt2DRotate* pEmitterPrt2DRotate)
{
	if (pEmitterPrt2DRotate == NULL)	{ return; }

	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_START:
		{
			pEmitterPrt2DRotate->prtStart2DRotate = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_START_BIAS:
		{
			pEmitterPrt2DRotate->prtStart2DRotateBias = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_END:
		{
			pEmitterPrt2DRotate->prtEnd2DRotate = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_END_BIAS:
		{
			pEmitterPrt2DRotate->prtEnd2DRotateBias = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	}
}

void CEffectPropertyDialog::ApplyProperties_EmitterPrt2DRotate(CPropTreeItem* pItem, SNtlPrtStdEmitterPrtRotate* pEmitterPrt2DRotate)
{
	if (pEmitterPrt2DRotate == NULL)	{ return; }

	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_START_X:
		{
			pEmitterPrt2DRotate->prtStartRotate.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_START_Y:
		{
			pEmitterPrt2DRotate->prtStartRotate.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_START_Z:
		{
			pEmitterPrt2DRotate->prtStartRotate.z = *(FLOAT*)pItem->GetItemValue();
		}
		break;

	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_START_BIAS_X:
		{
			pEmitterPrt2DRotate->prtStartRotateBias.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_START_BIAS_Y:
		{
			pEmitterPrt2DRotate->prtStartRotateBias.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_START_BIAS_Z:
		{
			pEmitterPrt2DRotate->prtStartRotateBias.z = *(FLOAT*)pItem->GetItemValue();
		}
		break;

	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_END_X:
		{
			pEmitterPrt2DRotate->prtEndRotate.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_END_Y:
		{
			pEmitterPrt2DRotate->prtEndRotate.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_END_Z:
		{
			pEmitterPrt2DRotate->prtEndRotate.z = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_END_BIAS_X:
		{
			pEmitterPrt2DRotate->prtEndRotateBias.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_END_BIAS_Y:
		{
			pEmitterPrt2DRotate->prtEndRotateBias.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_2DROTATE_END_BIAS_Z:
		{
			pEmitterPrt2DRotate->prtEndRotateBias.z = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	}
}

void CEffectPropertyDialog::ApplyProperties_EmitterPrtSize(CPropTreeItem* pItem, RpPrtStdEmitterPrtSize* pEmitterPtrSize)
{
	if (pEmitterPtrSize == NULL)	{ return; }

	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_START_X:
		{
			pEmitterPtrSize->prtStartSize.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_START_Y:
		{
			pEmitterPtrSize->prtStartSize.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_START_BIAS_X:
		{
			pEmitterPtrSize->prtStartSizeBias.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_START_BIAS_Y:
		{
			pEmitterPtrSize->prtStartSizeBias.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;

	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_END_X:
		{
			pEmitterPtrSize->prtEndSize.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_END_Y:
		{
			pEmitterPtrSize->prtEndSize.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_END_BIAS_X:
		{
			pEmitterPtrSize->prtEndSizeBias.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_END_BIAS_Y:
		{
			pEmitterPtrSize->prtEndSizeBias.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	}
}

void CEffectPropertyDialog::ApplyProperties_EmitterPrtSize(CPropTreeItem* pItem, SNtlPrtStdEmitterPrtSize* pEmitterPtrSize)
{
	if (pEmitterPtrSize == NULL)	{ return; }

	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_START_X:
		{
			pEmitterPtrSize->prtStartSize.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_START_Y:
		{
			pEmitterPtrSize->prtStartSize.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_START_Z:
		{
			pEmitterPtrSize->prtStartSize.z = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_START_BIAS_X:
		{
			pEmitterPtrSize->prtStartSizeBias.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_START_BIAS_Y:
		{
			pEmitterPtrSize->prtStartSizeBias.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_START_BIAS_Z:
		{
			pEmitterPtrSize->prtStartSizeBias.z = *(FLOAT*)pItem->GetItemValue();
		}
		break;

	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_END_X:
		{
			pEmitterPtrSize->prtEndSize.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_END_Y:
		{
			pEmitterPtrSize->prtEndSize.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_END_Z:
		{
			pEmitterPtrSize->prtEndSize.z = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_END_BIAS_X:
		{
			pEmitterPtrSize->prtEndSizeBias.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_END_BIAS_Y:
		{
			pEmitterPtrSize->prtEndSizeBias.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_SIZE_END_BIAS_Z:
		{
			pEmitterPtrSize->prtEndSizeBias.z = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	}
}

void CEffectPropertyDialog::ApplyProperties_EmitterPrtMatrix(CPropTreeItem* pItem, RpPrtStdEmitterPrtMatrix* pEmitterPrtMatrix, CNtlResourceParticleSystem* pResourceParticleSystem)
{
	if (pEmitterPrtMatrix == NULL)	{ return; }

	switch(pItem->GetCtrlID()) 
	{	    
    case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_MATRIX_ROTATION_X:
    case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_MATRIX_ROTATION_Y:
    case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_MATRIX_ROTATION_Z:
        {
            // Rotate -> LookAt, LookUp 으로 변환           
            CPropTreeItem* pItemXRot = m_PropertyTree.FindItem(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_MATRIX_ROTATION_X);
            CPropTreeItem* pItemYRot = m_PropertyTree.FindItem(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_MATRIX_ROTATION_Y);
            CPropTreeItem* pItemZRot = m_PropertyTree.FindItem(_PROPERTY_CONTROL_ACTION_EMITTER_PRT_MATRIX_ROTATION_Z);

            RwV3d vAngle;
            vAngle.x = (RwReal)pItemXRot->GetItemValue();
            vAngle.y = (RwReal)pItemYRot->GetItemValue();
            vAngle.z = (RwReal)pItemZRot->GetItemValue();

            API_GetAtUpVecApplyAngle(&pEmitterPrtMatrix->prtPosMtxAt, &pEmitterPrtMatrix->prtPosMtxUp, vAngle);

            // Cns Mtx값을 보관용으로 사용한다.
            //pEmitterPrtMatrix->prtPosMtxUpBias = vAngle;
            pEmitterPrtMatrix->prtCnsMtx.pos = vAngle;
            
        }
        break;
    case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_MATRIX_BILLBOARD:
        {
            // 기존에 설정되어 있는 Flag를 제거해 준다.
            pResourceParticleSystem->m_nEmitterDataFlag &= (~NTLrpPRTSTDEMITTERDATAFLAGPRTYBILLBOARD);
            pResourceParticleSystem->m_nEmitterDataFlag &= (~NTLrpPRTSTDEMITTERDATAFLAGPRTXBILLBOARD);
            pResourceParticleSystem->m_nEmitterDataFlag &= (~NTLrpPRTSTDEMITTERDATAFLAGPRTZBILLBOARD);           

            RwInt32 nFlag = (RwInt32)pItem->GetItemValue();            
            if(nFlag == BILLBOARD_Y)
            {
                pResourceParticleSystem->m_nEmitterDataFlag |= NTLrpPRTSTDEMITTERDATAFLAGPRTYBILLBOARD;
            }
            else if(nFlag == BILLBOARD_X)
            {
                pResourceParticleSystem->m_nEmitterDataFlag |= NTLrpPRTSTDEMITTERDATAFLAGPRTXBILLBOARD;
            }
            else if(nFlag == BILLBOARD_Z)
            {
                pResourceParticleSystem->m_nEmitterDataFlag |= NTLrpPRTSTDEMITTERDATAFLAGPRTZBILLBOARD;
            }            
        }
        break;
    }
}

void CEffectPropertyDialog::ApplyProperties_EmitterPrtHurricane(CPropTreeItem* pItem, SNtlPrtStdEmitterPrtHurricane* pEmitterPrtHurricane)
{
	if (pEmitterPrtHurricane == NULL)	{ return; }

	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_HURRICANE_SPEED:
		{
			pEmitterPrtHurricane->fSpeed = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_HURRICANE_RADIUS:
		{
			pEmitterPrtHurricane->radius = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_HURRICANE_REVOLUTION_ANGLE:
		{
			pEmitterPrtHurricane->revolutionAngle = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_HURRICANE_REVOLUTION_COUNT:
		{
			pEmitterPrtHurricane->revolutionCount = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	}
}

void CEffectPropertyDialog::ApplyProperties_EmitterPrtOrbitPoint(CPropTreeItem* pItem, SNtlPrtStdEmitterPrtOrbitPoint* pEmitterPrtOrbitPoint)
{
	if (pEmitterPrtOrbitPoint == NULL)	{ return; }

	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_ORBITPOINT_X:
		{
			pEmitterPrtOrbitPoint->point.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_ORBITPOINT_Y:
		{
			pEmitterPrtOrbitPoint->point.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_ORBITPOINT_Z:
		{
			pEmitterPrtOrbitPoint->point.z = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	}
}

void CEffectPropertyDialog::ApplyProperties_EmitterPrtFadeOut(CPropTreeItem* pItem, SNtlPrtStdEmitterPrtFadeOut* pEmitterPrtFadeOut)
{
	if (pEmitterPrtFadeOut == NULL)	{ return; }

	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_FADE_OUT_TIME:
		{
			pEmitterPrtFadeOut->time = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_FADE_OUT_FOLLOW:
		{
			pEmitterPrtFadeOut->follow = (RwBool)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_FADE_OUT_FOLLOW_SYSTEM:
		{
			// 우선 임시로 여기서 캐스팅한다. 차후 수정해야만 한다
			NTL_ASSERTE(CVenusVisualManager::GetInstance().m_pResourceSystem != NULL);
			CNtlResourceComponentSystem* pResourceComponentSystem = CVenusVisualManager::GetInstance().m_pResourceSystem;
			NTL_ASSERTE(pResourceComponentSystem);

			CNtlResourceComponentSystem* pResourceSystem = (CNtlResourceComponentSystem*)pItem->GetItemValue();
			if (pResourceSystem != NULL)
			{
				strcpy(pResourceComponentSystem->m_EmitterPrtFadeOut.strFollowSystem, pResourceSystem->GetSystemName());
			}
			else
			{
				memset(pResourceComponentSystem->m_EmitterPrtFadeOut.strFollowSystem, 0, MAX_PATH);
			}
		}
		break;
	}
}

void CEffectPropertyDialog::ApplyProperties_EmitterPrtFinish( CPropTreeItem* pItem, SNtlPrtStdEmitterPrtFinish* pEmitterPrtFinish ) 
{
    if(!pEmitterPrtFinish)
        return;

    switch(pItem->GetCtrlID())
    {
    case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_FINISH_TIME:
        pEmitterPrtFinish->m_fFinishTime = *(RwReal*)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_FINISH_ALPHA:
        if((RwBool)pItem->GetItemValue())
        {
            pEmitterPrtFinish->m_nFinishTypeFlag |= FINISH_TYPE_ALPHA;
        }
        else
        {
            pEmitterPrtFinish->m_nFinishTypeFlag ^= FINISH_TYPE_ALPHA;
        }
        break;
    case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_FINISH_SIZE:
        if((RwBool)pItem->GetItemValue())
        {
            pEmitterPrtFinish->m_nFinishTypeFlag |= FINISH_TYPE_SIZE;
        }
        else 
        {
            pEmitterPrtFinish->m_nFinishTypeFlag ^= FINISH_TYPE_SIZE;
        }
        break;
    case _PROPERTY_CONTROL_ACTION_EMITTER_PRT_FINISH_FOLLOW:
        if((RwBool)pItem->GetItemValue())
        {
            pEmitterPrtFinish->m_nFinishTypeFlag |= FINISH_TYPE_FOLLOW;
        }
        else
        {
            pEmitterPrtFinish->m_nFinishTypeFlag ^= FINISH_TYPE_FOLLOW;
        }
            break;
    }
}


void CEffectPropertyDialog::ApplyProperties_EmitterPrtUVAnim( CPropTreeItem* pItem, SNtlPrtStdEmitterPrtUVAnim* pEmitterUVAnim ) 
{
	if(!pEmitterUVAnim)
		return;

	switch(pItem->GetCtrlID())
	{
	case _PROPERTY_CONTROL_ACTION_UVANIM_DIR:
		{
			RwReal fDegree = (RwReal)pItem->GetItemValue();
			pEmitterUVAnim->vDir.x = sinf(NTL_DEG2RAD(fDegree));
			pEmitterUVAnim->vDir.y = 0.0f;
			pEmitterUVAnim->vDir.z = cosf(NTL_DEG2RAD(fDegree));
		}
		break;
	case _PROPERTY_CONTROL_ACTION_UVANIM_VELOCITY:
		pEmitterUVAnim->fVelocity = *(RwReal*)pItem->GetItemValue();
		break;
	case _PROPERTY_CONTROL_ACTION_UVANIM_OFFSET:
		pEmitterUVAnim->fMaxOffset = *(RwReal*)pItem->GetItemValue();
		break;
	default:
		NTL_ASSERTE(!"Ctrl ID Not Declare for Apply Properties");
		break;
	}
}	


void CEffectPropertyDialog::ApplyProperties_EmitterTrail( CPropTreeItem* pItem, SNtlPrtStdEmitterTrail* pEmitterTrail ) 
{
    if(!pEmitterTrail)
        return;

    switch(pItem->GetCtrlID())
    {
    case _PROPERTY_CONTROL_EMITTER_TRAIL_EDGE_LIFE_TIME:
        pEmitterTrail->fEdgeLifeTime = *(RwReal*)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_EMITTER_TRAIL_TEXTURE:
        if(pItem->GetItemValue() != NULL)
        {
            ZeroMemory(pEmitterTrail->strTexture, sizeof(pEmitterTrail->strTexture));
            strcpy(pEmitterTrail->strTexture, (RwChar*)pItem->GetItemValue());            
        }
        else
        {
            ZeroMemory(pEmitterTrail->strTexture, sizeof(pEmitterTrail->strTexture));            
        }
        break;
    case _PROPERTY_CONTROL_EMITTER_TRAIL_EDGE_GAP:
        pEmitterTrail->fEdgeGap = *(RwReal*)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_EMITTER_TRAIL_SPLINE_COUNT:
        pEmitterTrail->nSplinePointCount = (RwInt32)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_EMITTER_TRAIL_MAX_EDGE_COUNT:
        pEmitterTrail->nMaxEdgeCount = (RwInt32)pItem->GetItemValue();
        if(pEmitterTrail->nMaxEdgeCount > 1000)
        {
            AfxMessageBox("Max Edge Count must Under 1000!");
            pEmitterTrail->nMaxEdgeCount = 100;
            pItem->SetItemValue(100);
        }
        break;
    case _PROPERTY_CONTROL_EMITTER_TRAIL_MAX_LENGTH:
        pEmitterTrail->fMaxLength = *(RwReal*)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_EMITTER_TRAIL_WIDTH:
        pEmitterTrail->fWidth = *(RwReal*)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_EMITTER_TRAIL_SRC_BLEND:
        pEmitterTrail->eSrcBlend = BlendFunctions[(RwInt32)pItem->GetItemValue()];
        break;
    case _PROPERTY_CONTROL_EMITTER_TRAIL_DEST_BLEND:
        pEmitterTrail->eDestBlend = BlendFunctions[(RwInt32)pItem->GetItemValue()];
        break;
    case _PROPERTY_CONTROL_EMITTER_TRAIL_START_COL:
        {
            COLORREF color = (COLORREF)pItem->GetItemValue();
            pEmitterTrail->colStartColor.red = GetRValue(color);
            pEmitterTrail->colStartColor.green = GetGValue(color);
            pEmitterTrail->colStartColor.blue = GetBValue(color);
        }        
        break;
    case _PROPERTY_CONTROL_EMITTER_TRAIL_START_ALPHA:
        pEmitterTrail->colStartColor.alpha = (RwUInt32)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_EMITTER_TRAIL_END_COL:
        {
            COLORREF color = (COLORREF)pItem->GetItemValue();
            pEmitterTrail->colEndColor.red = GetRValue(color);
            pEmitterTrail->colEndColor.green = GetGValue(color);
            pEmitterTrail->colEndColor.blue = GetBValue(color);
        }        
        break;        
    case _PROPERTY_CONTROL_EMITTER_TRAIL_END_ALPHA:
        pEmitterTrail->colEndColor.alpha = (RwUInt32)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_EMITTER_TRAIL_OFFSET_X:
        pEmitterTrail->vOffset.x = *(RwReal*)pItem->GetItemValue();        
        break;
    case _PROPERTY_CONTROL_EMITTER_TRAIL_OFFSET_Y:
        pEmitterTrail->vOffset.y = *(RwReal*)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_EMITTER_TRAIL_OFFSET_Z:
        pEmitterTrail->vOffset.z = *(RwReal*)pItem->GetItemValue();
        break;
    }
}


void CEffectPropertyDialog::ApplyProperties_PrtEmitterEmitter(CPropTreeItem* pItem, RpPrtAdvPrtEmitterEmitter* pPrtEmitterEmitter)
{
	if (pPrtEmitterEmitter == NULL)	{ return; }

	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_ACTION_PRT_EMITTER_EMITTER_SYSTEM:
		{
			// 우선 임시로 여기서 캐스팅한다. 차후 수정해야만 한다
			NTL_ASSERTE(CVenusVisualManager::GetInstance().m_pResourceSystem != NULL);
			CNtlResourceParticleSystem* pResourceParticleSystem = (CNtlResourceParticleSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem;

			CNtlResourceComponentSystem* pResourceSystem = (CNtlResourceComponentSystem*)pItem->GetItemValue();
			if (pResourceSystem != NULL)
			{
				NTL_ASSERTE(pResourceSystem->GetSystemType() == CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_PARTICLE);
				pResourceParticleSystem->m_strSubSystem = pResourceSystem->GetSystemName();
			}
			else
			{
				pResourceParticleSystem->m_strSubSystem.clear();
			}

			NTL_ASSERTE(CVenusVisualManager::GetInstance().m_pResourceEffect != NULL);
			CVenusVisualManager::GetInstance().m_pResourceEffect->ResetSubSystem();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_PRT_EMITTER_EMITTER_TIME:
		{
			pPrtEmitterEmitter->time = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_PRT_EMITTER_EMITTER_TIME_BIAS:
		{
			pPrtEmitterEmitter->timeBias = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_PRT_EMITTER_EMITTER_TIMEGAP:
		{
			pPrtEmitterEmitter->timeGap = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_PRT_EMITTER_EMITTER_TIMEGAP_BIAS:
		{
			pPrtEmitterEmitter->timeGapBias = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	}
}

void CEffectPropertyDialog::ApplyProperties_MultiColorEmitter(CPropTreeItem* pItem, RpPrtAdvPrtMultiColorEmitter* pEmitterPrtMultiCol)
{
	if (pEmitterPrtMultiCol == NULL)	{ return; }

	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_ACTION_MULTI_COLOR_EMITTER_NUM_COLOR:
		{
			if ((RwUInt32)pItem->GetItemValue() <= 0)
			{
				SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
				break;
			}
			RwUInt32 nOldCount = pEmitterPrtMultiCol->numCol;
			RpPrtAdvEmtPrtColorItem* pSourceData = pEmitterPrtMultiCol->list;
			pEmitterPrtMultiCol->numCol = (RwUInt32)pItem->GetItemValue();
			pEmitterPrtMultiCol->list = NTL_NEW RpPrtAdvEmtPrtColorItem[pEmitterPrtMultiCol->numCol];
			memset(pEmitterPrtMultiCol->list, 0, sizeof(RpPrtAdvEmtPrtColorItem) * pEmitterPrtMultiCol->numCol);

			for (RwUInt32 i = 0;i < pEmitterPrtMultiCol->numCol; ++ i)
			{
				pEmitterPrtMultiCol->list[i].time = (RwReal) i + 1.f;
				pEmitterPrtMultiCol->list[i].timeBias = 0.f;
			}
			if (nOldCount < pEmitterPrtMultiCol->numCol)
				memcpy(pEmitterPrtMultiCol->list, pSourceData, sizeof(RpPrtAdvEmtPrtColorItem) * nOldCount);
			else
				memcpy(pEmitterPrtMultiCol->list, pSourceData, sizeof(RpPrtAdvEmtPrtColorItem) * pEmitterPrtMultiCol->numCol);
			NTL_ARRAY_DELETE(pSourceData);
			if (m_nCurrentPoint >= pEmitterPrtMultiCol->numCol)
				m_nCurrentPoint = 0;

			SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_COLOR_EMITTER_CURRENT_INDEX:
		{
			m_nCurrentPoint = (RwUInt32)pItem->GetItemValue();
			SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_COLOR_EMITTER_TIME:
		{
			pEmitterPrtMultiCol->list[m_nCurrentPoint].time = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_COLOR_EMITTER_TIME_BIAS:
		{
			pEmitterPrtMultiCol->list[m_nCurrentPoint].timeBias = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_COLOR_EMITTER_COLOR:
		{
			COLORREF color = (COLORREF)pItem->GetItemValue();
			pEmitterPrtMultiCol->list[m_nCurrentPoint].midCol.red = GetRValue(color);
			pEmitterPrtMultiCol->list[m_nCurrentPoint].midCol.green = GetGValue(color);
			pEmitterPrtMultiCol->list[m_nCurrentPoint].midCol.blue = GetBValue(color);
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_COLOR_EMITTER_COLOR_A:
		{
			// slider 이기 때문에 (RwReal)pItem->GetItemValue(); 으로 처리해야 한다.
			pEmitterPrtMultiCol->list[m_nCurrentPoint].midCol.alpha = (RwReal)pItem->GetItemValue();
		}
		break;

	case _PROPERTY_CONTROL_ACTION_MULTI_COLOR_EMITTER_COLOR_BIAS_R:
		{
			pEmitterPrtMultiCol->list[m_nCurrentPoint].midColBias.red = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_COLOR_EMITTER_COLOR_BIAS_G:
		{
			pEmitterPrtMultiCol->list[m_nCurrentPoint].midColBias.green = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_COLOR_EMITTER_COLOR_BIAS_B:
		{
			pEmitterPrtMultiCol->list[m_nCurrentPoint].midColBias.blue = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_COLOR_EMITTER_COLOR_BIAS_A:
		{
			pEmitterPrtMultiCol->list[m_nCurrentPoint].midColBias.alpha = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	}
}

void CEffectPropertyDialog::ApplyProperties_MultiSizeEmitter(CPropTreeItem* pItem, RpPrtAdvPrtMultiSizeEmitter* pEmitterPrtMultiSize)
{
	if (pEmitterPrtMultiSize == NULL)	{ return; }

	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_NUM_SIZE:
		{
			if ((RwUInt32)pItem->GetItemValue() <= 0)
			{
				SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
				break;
			}
			RwUInt32 nOldCount = pEmitterPrtMultiSize->numSize;
			RpPrtAdvEmtPrtSizeItem* pSourceData = pEmitterPrtMultiSize->list;
			pEmitterPrtMultiSize->numSize = (RwUInt32)pItem->GetItemValue();
			pEmitterPrtMultiSize->list = NTL_NEW RpPrtAdvEmtPrtSizeItem[pEmitterPrtMultiSize->numSize];
			memset(pEmitterPrtMultiSize->list, 0, sizeof(RpPrtAdvEmtPrtSizeItem) * pEmitterPrtMultiSize->numSize);

			for (RwUInt32 i = 0;i < pEmitterPrtMultiSize->numSize; ++ i)
			{
				pEmitterPrtMultiSize->list[i].time = (RwReal) i + 1.f;
				pEmitterPrtMultiSize->list[i].timeBias = 0.f;
			}

			if (nOldCount < pEmitterPrtMultiSize->numSize)
				memcpy(pEmitterPrtMultiSize->list, pSourceData, sizeof(RpPrtAdvEmtPrtSizeItem) * nOldCount);
			else
				memcpy(pEmitterPrtMultiSize->list, pSourceData, sizeof(RpPrtAdvEmtPrtSizeItem) * pEmitterPrtMultiSize->numSize);
			NTL_ARRAY_DELETE(pSourceData);
			if (m_nCurrentPoint >= pEmitterPrtMultiSize->numSize)
				m_nCurrentPoint = 0;

			SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_CURRENT_INDEX:
		{
			m_nCurrentPoint = (RwUInt32)pItem->GetItemValue();
			SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_TIME:
		{
			pEmitterPrtMultiSize->list[m_nCurrentPoint].time = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_TIME_BIAS:
		{
			pEmitterPrtMultiSize->list[m_nCurrentPoint].timeBias = *(FLOAT*)pItem->GetItemValue();
		}
		break;

	case _PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_SIZE_X:
		{
			pEmitterPrtMultiSize->list[m_nCurrentPoint].midSize.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_SIZE_Y:
		{
			pEmitterPrtMultiSize->list[m_nCurrentPoint].midSize.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_SIZE_BIAS_X:
		{
			pEmitterPrtMultiSize->list[m_nCurrentPoint].midSizeBias.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_SIZE_BIAS_Y:
		{
			pEmitterPrtMultiSize->list[m_nCurrentPoint].midSizeBias.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	}
}

void CEffectPropertyDialog::ApplyProperties_MultiSizeEmitter(CPropTreeItem* pItem, SNtlAdvMultiSizeEmitter* pEmitterPrtMultiSize)
{
	if (pEmitterPrtMultiSize == NULL)	{ return; }

	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_NUM_SIZE:
		{
			if ((RwUInt32)pItem->GetItemValue() <= 0)
			{
				SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
				break;
			}
			RwUInt32 nOldCount = pEmitterPrtMultiSize->numSize;
			SNtlAdvEmtPrtSizeItem* pSourceData = pEmitterPrtMultiSize->list;
			pEmitterPrtMultiSize->numSize = (RwUInt32)pItem->GetItemValue();
			pEmitterPrtMultiSize->list = NTL_NEW SNtlAdvEmtPrtSizeItem[pEmitterPrtMultiSize->numSize];
			memset(pEmitterPrtMultiSize->list, 0, sizeof(SNtlAdvEmtPrtSizeItem) * pEmitterPrtMultiSize->numSize);

			for (RwUInt32 i = 0;i < pEmitterPrtMultiSize->numSize; ++ i)
			{
				pEmitterPrtMultiSize->list[i].time = (RwReal) i + 1.f;
				pEmitterPrtMultiSize->list[i].timeBias = 0.f;
			}

			if (nOldCount < pEmitterPrtMultiSize->numSize)
				memcpy(pEmitterPrtMultiSize->list, pSourceData, sizeof(SNtlAdvEmtPrtSizeItem) * nOldCount);
			else
				memcpy(pEmitterPrtMultiSize->list, pSourceData, sizeof(SNtlAdvEmtPrtSizeItem) * pEmitterPrtMultiSize->numSize);
			NTL_ARRAY_DELETE(pSourceData);
			if (m_nCurrentPoint >= pEmitterPrtMultiSize->numSize)
				m_nCurrentPoint = 0;

			SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_CURRENT_INDEX:
		{
			m_nCurrentPoint = (RwUInt32)pItem->GetItemValue();
			SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_TIME:
		{
			pEmitterPrtMultiSize->list[m_nCurrentPoint].time = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_TIME_BIAS:
		{
			pEmitterPrtMultiSize->list[m_nCurrentPoint].timeBias = *(FLOAT*)pItem->GetItemValue();
		}
		break;

	case _PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_SIZE_X:
		{
			pEmitterPrtMultiSize->list[m_nCurrentPoint].midSize.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_SIZE_Y:
		{
			pEmitterPrtMultiSize->list[m_nCurrentPoint].midSize.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_SIZE_Z:
		{
			pEmitterPrtMultiSize->list[m_nCurrentPoint].midSize.z = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_SIZE_BIAS_X:
		{
			pEmitterPrtMultiSize->list[m_nCurrentPoint].midSizeBias.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_SIZE_BIAS_Y:
		{
			pEmitterPrtMultiSize->list[m_nCurrentPoint].midSizeBias.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_SIZE_EMITTER_SIZE_BIAS_Z:
		{
			pEmitterPrtMultiSize->list[m_nCurrentPoint].midSizeBias.z = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	}
}

void CEffectPropertyDialog::ApplyProperties_MultiRotateEmitter(CPropTreeItem* pItem, SNtlAdvMultiRotateEmitter* pEmitterPrtMultiRotate)
{
	if (pEmitterPrtMultiRotate == NULL)	{ return; }

	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_ACTION_MULTI_ROTATE_EMITTER_NUM_ROTATE:
		{
			if ((RwUInt32)pItem->GetItemValue() <= 0)
			{
				SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
				break;
			}
			RwUInt32 nOldCount = pEmitterPrtMultiRotate->numRotate;
			SNtlAdvEmtPrtRotateItem* pSourceData = pEmitterPrtMultiRotate->list;
			pEmitterPrtMultiRotate->numRotate = (RwUInt32)pItem->GetItemValue();
			pEmitterPrtMultiRotate->list = NTL_NEW SNtlAdvEmtPrtRotateItem[pEmitterPrtMultiRotate->numRotate];
			memset(pEmitterPrtMultiRotate->list, 0, sizeof(SNtlAdvEmtPrtRotateItem) * pEmitterPrtMultiRotate->numRotate);

			for (RwUInt32 i = 0;i < pEmitterPrtMultiRotate->numRotate; ++ i)
			{
				pEmitterPrtMultiRotate->list[i].time = (RwReal) i + 1.f;
				pEmitterPrtMultiRotate->list[i].timeBias = 0.f;
			}

			if (nOldCount < pEmitterPrtMultiRotate->numRotate)
				memcpy(pEmitterPrtMultiRotate->list, pSourceData, sizeof(SNtlAdvEmtPrtRotateItem) * nOldCount);
			else
				memcpy(pEmitterPrtMultiRotate->list, pSourceData, sizeof(SNtlAdvEmtPrtRotateItem) * pEmitterPrtMultiRotate->numRotate);
			NTL_ARRAY_DELETE(pSourceData);
			if (m_nCurrentPoint >= pEmitterPrtMultiRotate->numRotate)
				m_nCurrentPoint = 0;

			SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_ROTATE_EMITTER_CURRENT_INDEX:
		{
			m_nCurrentPoint = (RwUInt32)pItem->GetItemValue();
			SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_ROTATE_EMITTER_TIME:
		{
			pEmitterPrtMultiRotate->list[m_nCurrentPoint].time = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_ROTATE_EMITTER_TIME_BIAS:
		{
			pEmitterPrtMultiRotate->list[m_nCurrentPoint].timeBias = *(FLOAT*)pItem->GetItemValue();
		}
		break;

	case _PROPERTY_CONTROL_ACTION_MULTI_ROTATE_EMITTER_ROTATE_X:
		{
			pEmitterPrtMultiRotate->list[m_nCurrentPoint].midRotate.x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_ROTATE_EMITTER_ROTATE_Y:
		{
			pEmitterPrtMultiRotate->list[m_nCurrentPoint].midRotate.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_ROTATE_EMITTER_ROTATE_Z:
		{
			pEmitterPrtMultiRotate->list[m_nCurrentPoint].midRotate.z = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_ROTATE_EMITTER_ROTATE_BIAS_X:
		{
			pEmitterPrtMultiRotate->list[m_nCurrentPoint].midRotateBias.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_ROTATE_EMITTER_ROTATE_BIAS_Y:
		{
			pEmitterPrtMultiRotate->list[m_nCurrentPoint].midRotateBias.y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_ROTATE_EMITTER_ROTATE_BIAS_Z:
		{
			pEmitterPrtMultiRotate->list[m_nCurrentPoint].midRotateBias.z = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	}
}

void CEffectPropertyDialog::ApplyProperties_MultiTexCoordsEmitter(CPropTreeItem* pItem, RpPrtAdvEmtPrtMultiTexCoords *pEmitterPrtMultiTexCoords)
{
	if (pEmitterPrtMultiTexCoords == NULL)	{ return; }

	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_ACTION_MULTI_TEXCOORDS_EMITTER_NUM_TEX:
		{
			if ((RwUInt32)pItem->GetItemValue() <= 0)
			{
				SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
				break;
			}
			RwUInt32 nOldCount = pEmitterPrtMultiTexCoords->numTex;
			RpPrtAdvEmtPrtTexCoordsItem* pSourceData = pEmitterPrtMultiTexCoords->list;
			pEmitterPrtMultiTexCoords->numTex = (RwUInt32)pItem->GetItemValue();
			pEmitterPrtMultiTexCoords->list = NTL_NEW RpPrtAdvEmtPrtTexCoordsItem[pEmitterPrtMultiTexCoords->numTex];
			memset(pEmitterPrtMultiTexCoords->list, 0, sizeof(RpPrtAdvEmtPrtTexCoordsItem) * pEmitterPrtMultiTexCoords->numTex);

			for (RwUInt32 i = 0;i < pEmitterPrtMultiTexCoords->numTex; ++ i)
			{
				pEmitterPrtMultiTexCoords->list[i].time = (RwReal) i + 1.f;
				pEmitterPrtMultiTexCoords->list[i].timeBias = 0.f;
			}

			if (nOldCount < pEmitterPrtMultiTexCoords->numTex)
				memcpy(pEmitterPrtMultiTexCoords->list, pSourceData, sizeof(RpPrtAdvEmtPrtTexCoordsItem) * nOldCount);
			else
				memcpy(pEmitterPrtMultiTexCoords->list, pSourceData, sizeof(RpPrtAdvEmtPrtTexCoordsItem) * pEmitterPrtMultiTexCoords->numTex);
			NTL_ARRAY_DELETE(pSourceData);
			if (m_nCurrentPoint >= pEmitterPrtMultiTexCoords->numTex)
				m_nCurrentPoint = 0;

			SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_TEXCOORDS_EMITTER_CURRENT_INDEX:
		{
			m_nCurrentPoint = (RwUInt32)pItem->GetItemValue();
			SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_TEXCOORDS_EMITTER_TIME:
		{
			pEmitterPrtMultiTexCoords->list[m_nCurrentPoint].time = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_TEXCOORDS_EMITTER_TIME_BIAS:
		{
			pEmitterPrtMultiTexCoords->list[m_nCurrentPoint].timeBias = *(FLOAT*)pItem->GetItemValue();
		}
		break;

	case _PROPERTY_CONTROL_ACTION_MULTI_TEXCOORDS_EMITTER_MIDUV0_U:
		{
			pEmitterPrtMultiTexCoords->list[m_nCurrentPoint].midUV0.u = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_TEXCOORDS_EMITTER_MIDUV0_V:
		{
			pEmitterPrtMultiTexCoords->list[m_nCurrentPoint].midUV0.v = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_TEXCOORDS_EMITTER_MIDUV0_BIAS_U:
		{
			pEmitterPrtMultiTexCoords->list[m_nCurrentPoint].midUV0Bias.u = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_TEXCOORDS_EMITTER_MIDUV0_BIAS_V:
		{
			pEmitterPrtMultiTexCoords->list[m_nCurrentPoint].midUV0Bias.v = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_TEXCOORDS_EMITTER_MIDUV1_U:
		{
			pEmitterPrtMultiTexCoords->list[m_nCurrentPoint].midUV1.u = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_TEXCOORDS_EMITTER_MIDUV1_V:
		{
			pEmitterPrtMultiTexCoords->list[m_nCurrentPoint].midUV1.v = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_TEXCOORDS_EMITTER_MIDUV1_BIAS_U:
		{
			pEmitterPrtMultiTexCoords->list[m_nCurrentPoint].midUV1Bias.u = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_TEXCOORDS_EMITTER_MIDUV1_BIAS_V:
		{
			pEmitterPrtMultiTexCoords->list[m_nCurrentPoint].midUV1Bias.v = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	}
}

void CEffectPropertyDialog::ApplyProperties_MultiTexturesEmitter(CPropTreeItem* pItem, RpPrtAdvEmtPrtMultiTextures *pEmitterPrtMultiTextures)
{
	if (pEmitterPrtMultiTextures == NULL)	{ return; }

	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_ACTION_MULTI_TEXTURES_EMITTER_START_TIME:
		{
			pEmitterPrtMultiTextures->time = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_TEXTURES_EMITTER_LOOP:
		{
			pEmitterPrtMultiTextures->bLoop = (RwBool)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_TEXTURES_EMITTER_NUM_TEXTURES:
		{
			if ((RwUInt32)pItem->GetItemValue() <= 0)
			{
				SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
				break;
			}
			RwUInt32 nOldCount = pEmitterPrtMultiTextures->numTextures;
			RpPrtAdvEmtPrtTextureItem* pSourceData = pEmitterPrtMultiTextures->list;
			pEmitterPrtMultiTextures->numTextures = (RwUInt32)pItem->GetItemValue();
			pEmitterPrtMultiTextures->list = NTL_NEW RpPrtAdvEmtPrtTextureItem[pEmitterPrtMultiTextures->numTextures];

			for (RwUInt32 i = 0;i < pEmitterPrtMultiTextures->numTextures; ++ i)
			{
				pEmitterPrtMultiTextures->list[i].time = 1.f;
				pEmitterPrtMultiTextures->list[i].timeBias = 0.f;
			}

			if (nOldCount < pEmitterPrtMultiTextures->numTextures)
			{
				for (RwUInt32 i = 0;i < nOldCount; ++ i)
				{
					pEmitterPrtMultiTextures->list[i].time = pSourceData[i].time;
					pEmitterPrtMultiTextures->list[i].timeBias = pSourceData[i].timeBias;
					pEmitterPrtMultiTextures->list[i].strTexture = pSourceData[i].strTexture.c_str();
				}
			}
			else
			{
				for (RwUInt32 i = 0;i < pEmitterPrtMultiTextures->numTextures; ++ i)
				{
					pEmitterPrtMultiTextures->list[i].time = pSourceData[i].time;
					pEmitterPrtMultiTextures->list[i].timeBias = pSourceData[i].timeBias;
					pEmitterPrtMultiTextures->list[i].strTexture = pSourceData[i].strTexture.c_str();
				}
			}
			NTL_ARRAY_DELETE(pSourceData);
			if (m_nCurrentPoint >= pEmitterPrtMultiTextures->numTextures)
				m_nCurrentPoint = 0;

			SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_TEXTURES_EMITTER_CURRENT_INDEX:
		{
			m_nCurrentPoint = (RwUInt32)pItem->GetItemValue();
			SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_TEXTURES_EMITTER_TIME:
		{
			pEmitterPrtMultiTextures->list[m_nCurrentPoint].time = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_TEXTURES_EMITTER_TIME_BIAS:
		{
			pEmitterPrtMultiTextures->list[m_nCurrentPoint].timeBias = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_MULTI_TEXTURES_EMITTER_TEXTURE_NAME:
		{
			if (pItem->GetItemValue() != NULL)
			{
				pEmitterPrtMultiTextures->list[m_nCurrentPoint].strTexture = (RwChar*)pItem->GetItemValue();
			}
			else
			{
				pEmitterPrtMultiTextures->list[m_nCurrentPoint].strTexture.clear();
			}
		}
		break;
	}
}

void CEffectPropertyDialog::ApplyProperties_SplinePathEmitter(CPropTreeItem* pItem, SNtlPrtEmitterSplinePath* pEmitterSplinePath)
{
	if (pEmitterSplinePath == NULL)	{ return; }

	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_ACTION_SPLINE_PATH_NAME:
		{
			if (NULL == pItem->GetItemValue())
			{
				memset(pEmitterSplinePath->strName, 0, sizeof(RwChar) * MAX_PATH);
			}
			else
			{
				CNtlEffectSplinePath* pSplinePath = (CNtlEffectSplinePath*)pItem->GetItemValue();
				strcpy(pEmitterSplinePath->strName, pSplinePath->GetName());
			}
		}
		break;
	case _PROPERTY_CONTROL_ACTION_SPLINE_PATH_LOOP:
		{
			pEmitterSplinePath->loop = (BOOL)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_SPLINE_PATH_USE_ROTATE:
		{
			pEmitterSplinePath->useRotate = (BOOL)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_SPLINE_MATRIX_POSITION_X:
		{
			pEmitterSplinePath->vPosition.x = *(FLOAT*)pItem->GetItemValue();
			CVenusVisualManager::GetInstance().m_pResourceSystem->RebuildPathMatrix();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_SPLINE_MATRIX_POSITION_Y:
		{
			pEmitterSplinePath->vPosition.y = *(FLOAT*)pItem->GetItemValue();
			CVenusVisualManager::GetInstance().m_pResourceSystem->RebuildPathMatrix();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_SPLINE_MATRIX_POSITION_Z:
		{
			pEmitterSplinePath->vPosition.z = *(FLOAT*)pItem->GetItemValue();
			CVenusVisualManager::GetInstance().m_pResourceSystem->RebuildPathMatrix();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_SPLINE_MATRIX_ROTATE_X:
		{
			pEmitterSplinePath->vRotate.x = (RwReal)pItem->GetItemValue();
			CVenusVisualManager::GetInstance().m_pResourceSystem->RebuildPathMatrix();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_SPLINE_MATRIX_ROTATE_Y:
		{
			pEmitterSplinePath->vRotate.y = (RwReal)pItem->GetItemValue();
			CVenusVisualManager::GetInstance().m_pResourceSystem->RebuildPathMatrix();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_SPLINE_MATRIX_ROTATE_Z:
		{
			pEmitterSplinePath->vRotate.z = (RwReal)pItem->GetItemValue();
			CVenusVisualManager::GetInstance().m_pResourceSystem->RebuildPathMatrix();
		}
		break;
	}
}


void CEffectPropertyDialog::ApplyProperties_EmitterPointList(CPropTreeItem* pItem, RpPrtAdvPointListEmitter* pEmitterPtList)
{
	if (pEmitterPtList == NULL)	{ return; }

	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_ACTION_EMITTER_POINTLIST_USE_DIRECTION:
		{
			pEmitterPtList->useDirection = (BOOL)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_POINTLIST_RANDOM:
		{
			pEmitterPtList->random = (BOOL)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_POINTLIST_NUM_POINT:
		{
			if ((RwUInt32)pItem->GetItemValue() <= 0)
			{
				SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
				break;
			}
			RwUInt32 nOldCount = pEmitterPtList->numPoint;

			RwV3d* pSourceDataPoint = pEmitterPtList->pointList;
			RwV3d* pSourceDataDirection = pEmitterPtList->directionList;

			pEmitterPtList->numPoint = (RwUInt32)pItem->GetItemValue();
			pEmitterPtList->pointList = NTL_NEW RwV3d[pEmitterPtList->numPoint];
			pEmitterPtList->directionList = NTL_NEW RwV3d[pEmitterPtList->numPoint];
			memset(pEmitterPtList->pointList, 0, sizeof(RwV3d) * pEmitterPtList->numPoint);
			memset(pEmitterPtList->directionList, 0, sizeof(RwV3d) * pEmitterPtList->numPoint);

			if (nOldCount < pEmitterPtList->numPoint)
			{
				memcpy(pEmitterPtList->pointList, pSourceDataPoint, sizeof(RwV3d) * nOldCount);
				memcpy(pEmitterPtList->directionList, pSourceDataDirection, sizeof(RwV3d) * nOldCount);
			}
			else
			{
				memcpy(pEmitterPtList->pointList, pSourceDataPoint, sizeof(RwV3d) * pEmitterPtList->numPoint);
				memcpy(pEmitterPtList->directionList, pSourceDataDirection, sizeof(RwV3d) * pEmitterPtList->numPoint);
			}
			NTL_ARRAY_DELETE(pSourceDataPoint);
			NTL_ARRAY_DELETE(pSourceDataDirection);

			if (m_nCurrentPoint >= pEmitterPtList->numPoint)
			{
				m_nCurrentPoint = 0;
			}

			SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_POINTLIST_CURRENT_INDEX:
		{
			m_nCurrentPoint = (RwUInt32)pItem->GetItemValue();
			SetTimer(TIME_TYPE_RESET_PROPERTIES, 100, NULL);
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_POINTLIST_POINT_X:
		{
			pEmitterPtList->pointList[m_nCurrentPoint].x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_POINTLIST_POINT_Y:
		{
			pEmitterPtList->pointList[m_nCurrentPoint].y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_POINTLIST_POINT_Z:
		{
			pEmitterPtList->pointList[m_nCurrentPoint].z = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_POINTLIST_DIRECTION_X:
		{
			pEmitterPtList->directionList[m_nCurrentPoint].x = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_POINTLIST_DIRECTION_Y:
		{
			pEmitterPtList->directionList[m_nCurrentPoint].y = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_EMITTER_POINTLIST_DIRECTION_Z:
		{
			pEmitterPtList->directionList[m_nCurrentPoint].z = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	}
}

void CEffectPropertyDialog::ApplyProperties_CircleEmitter(CPropTreeItem* pItem, RpPrtAdvCircleEmitter* pEmitterPtrCircle)
{
	if (pEmitterPtrCircle == NULL)	{ return; }

	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_ACTION_CIRCLE_EMITTER_USE_EMISSION:
		{
			pEmitterPtrCircle->useCircleEmission = (BOOL)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_CIRCLE_EMITTER_RAIDUS:
		{
			pEmitterPtrCircle->radius = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_CIRCLE_EMITTER_RAIDUS_GAP:
		{
			pEmitterPtrCircle->radiusGap = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_CIRCLE_EMITTER_HEIGHT:
		{
			pEmitterPtrCircle->height = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_CIRCLE_EMITTER_DIR_ROTATION:
		{
			pEmitterPtrCircle->dirRotation = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	}
}

void CEffectPropertyDialog::ApplyProperties_SphereEmitter(CPropTreeItem* pItem, RpPrtAdvSphereEmitter* pEmitterPtrSphere)
{
	if (pEmitterPtrSphere == NULL)	{ return; }

	switch(pItem->GetCtrlID()) 
	{
	case _PROPERTY_CONTROL_ACTION_SPHERE_EMITTER_USE_EMISSION:
		{
			pEmitterPtrSphere->useSphereEmission = (BOOL)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_SPHERE_EMITTER_RAIDUS:
		{
			pEmitterPtrSphere->radius = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	case _PROPERTY_CONTROL_ACTION_SPHERE_EMITTER_RAIDUS_GAP:
		{
			pEmitterPtrSphere->radiusGap = *(FLOAT*)pItem->GetItemValue();
		}
		break;
	}
}

void CEffectPropertyDialog::ApplyProperties_SphereEmitter( CPropTreeItem* pItem, SNtlPrtStdEmitterPrtSphere* pEmitterPtrSphere ) 
{
    if(!pEmitterPtrSphere)
        return;

    switch(pItem->GetCtrlID())
    {
    case _PROPERTY_CONTROL_ACTION_SPHERE_EMITTER_RAIDUS_X:
        pEmitterPtrSphere->m_vEmitterSize.x = *(RwReal*)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_ACTION_SPHERE_EMITTER_RAIDUS_Y:
        pEmitterPtrSphere->m_vEmitterSize.y = *(RwReal*)pItem->GetItemValue();
        break;
    case _PROPERTY_CONTROL_ACTION_SPHERE_EMITTER_RAIDUS_Z:
        pEmitterPtrSphere->m_vEmitterSize.z = *(RwReal*)pItem->GetItemValue();
        break;    
    }
}

//------------------------------------------------------------------
//	FuncName	: SetProperties_Textures
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
CPropTreeItem* CEffectPropertyDialog::SetProperties_Textures(std::string& strCurrentName, CPropTreeItem* pRoot)
{
	CPropTreeItemCombo* pCombo;
	pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);	
	pCombo->SetLabelText(_T("Texture"));
	pCombo->SetInfoText(_T("Texture"));
	pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
	pCombo->SetDropDownHeight(300);

	RwInt32 index = 0;
	index = pCombo->AddString(_T("NONE"));
	pCombo->SetItemData(index, NULL);
	pCombo->SetItemValue(0);

	CNtlResourceFileManager::smdef_Entity::iterator it;
	for (it = CNtlEffectSystemFreeList::m_TextureFileManager.m_smEntity.begin(); it != CNtlEffectSystemFreeList::m_TextureFileManager.m_smEntity.end(); ++ it)
	{
		std::string strFileName = (*it).first.c_str();
		index = pCombo->AddString(_T(strFileName.c_str()));
		pCombo->SetItemData(index, (DWORD_PTR)(*it).first.c_str());
		if (_stricmp(strFileName.c_str(), strCurrentName.c_str()) == 0)
		{
			pCombo->SetItemValue(index);            
		}
	}
	return pCombo;
}

CPropTreeItem* CEffectPropertyDialog::SetProperties_Sound( std::string& strSoundName, CPropTreeItem* pRoot ) 
{
    CPropTreeItemCombo* pCombo;
    pCombo = (CPropTreeItemCombo*)m_PropertyTree.InsertItem(new CPropTreeItemCombo(), pRoot);	
    pCombo->SetLabelText(_T("SoundFile Name"));
    pCombo->SetInfoText(_T("SoundFile Name"));
    pCombo->CreateComboBox(WS_CHILD|WS_VSCROLL|CBS_DROPDOWNLIST);
    pCombo->SetDropDownHeight(300);

    RwInt32 index = 0;
    index = pCombo->AddString(_T("NONE"));
    pCombo->SetItemData(index, NULL);
    pCombo->SetItemValue(0);

    CNtlResourceFileManager::smdef_Entity::iterator it;
    for (it = CNtlEffectSystemFreeList::m_SoundFileManager.m_smEntity.begin(); it != CNtlEffectSystemFreeList::m_SoundFileManager.m_smEntity.end(); ++ it)
    {
        std::string strFileName = (*it).first.c_str();
        index = pCombo->AddString(_T(strFileName.c_str()));
        pCombo->SetItemData(index, (DWORD_PTR)(*it).first.c_str());
        if (_stricmp(strFileName.c_str(), strSoundName.c_str()) == 0)
        {
            pCombo->SetItemValue(index);            
        }
    }
    return pCombo;
}

