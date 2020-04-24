// ActionListCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "VenusFrame.h"
#include "ActionListCtrl.h"
#include ".\actionlistctrl.h"

#include "EffectEditLayer.h"

#include "NtlDebug.h"

#include "NtlResourceParticleSystem.h"
#include "NtlResourceMeshSystem.h"

#include ".\venuspropertycontainer.h"
#include ".\venusvisualmanager.h"

#include "StringManager.h"

// CActionListCtrl
const RwChar* EFFECT_ACTION_STRING[] = {"Color",		\
										"TexCoords",	\
										"2DRotate",		\
										"Size",			\
										"Matrix",		\
										"Hurricane",	\
										"Orbit Point",	\
										"FadeOut",		\
										"UV Animation", \

										"Emitter",			\
										"Multi Color",		\
										"Multi Size",		\
										"Multi RotateSize",	\
										"Multi TexCoords",	\

										"Multi Textures",	\

										"Spline Path",	\

										"PointList",		\
										"Circle",			\
										"Sphere",           \
                                        "Finish",           \
                                        "Trail"};

//IMPLEMENT_DYNAMIC(CActionListCtrl, CVenusGridCtrl)
CActionListCtrl::CActionListCtrl()
{
	m_bListUpdate	= TRUE;
}

CActionListCtrl::~CActionListCtrl()
{
}


BEGIN_MESSAGE_MAP(CActionListCtrl, CVenusGridCtrl)

	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(POPUP_INSERT_EMITTER_COLOR, OnInsertEmitterColor)
	ON_COMMAND(POPUP_INSERT_EMITTER_SIZE, OnInsertEmitterSize)
	ON_COMMAND(POPUP_INSERT_EMITTER_TEXCOORDS, OnInsertEmitterTexcoords)
	ON_COMMAND(POPUP_INSERT_EMITTER_2DROTATE, OnInsertEmitter2drotate)
	ON_COMMAND(POPUP_INSERT_EMITTER_MATRIX, OnInsertEmitterMatrix)
	ON_COMMAND(POPUP_INSERT_ATTACH_SYSTEM, OnInsertAttachSystem)
	ON_COMMAND(POPUP_INSERT_EMITTER_MULTI_TEXCOORDS, OnInsertEmitterMultiTexcoords)
	ON_COMMAND(POPUP_INSERT_EMITTER_MULTI_COLOR, OnInsertEmitterMultiColor)
	ON_COMMAND(POPUP_INSERT_EMITTER_MULTI_SIZE, OnInsertEmitterMultiSize)
	ON_COMMAND(POPUP_INSERT_EMITTER_MULTI_ROTATE, OnInsertEmitterMultiRotate)
	ON_COMMAND(POPUP_INSERT_POINT_EMITTER, OnInsertPointEmitter)
	ON_COMMAND(POPUP_INSERT_CIRCLE_EMITTER, OnInsertCircleEmitter)
	ON_COMMAND(POPUP_INSERT_SPHERE_EMITTER, OnInsertSphereEmitter)
	ON_COMMAND(POPUP_DELETE_ACTION, OnDeleteAction)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnLvnItemchanged)
	ON_COMMAND(POPUP_INSERT_EMITTER_SPLINE_PATH, OnInsertEmitterSplinePath)
	ON_COMMAND(POPUP_INSERT_EMITTER_MULTI_TEXTURES, OnInsertEmitterMultiTextures)
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(POPUP_INSERT_EMITTER_HURRICANE, OnInsertEmitterHurricane)
	ON_COMMAND(POPUP_INSERT_EMITTER_ORBITPOINT, OnInsertEmitterOrbitPoint)
	ON_COMMAND(POPUP_INSERT_EMITTER_FADE_OUT, OnInsertEmitterFadeOut)
    ON_COMMAND(POPUP_INSERT_EMITTER_FINISH, &CActionListCtrl::OnInsertEmitterFinish)
    ON_COMMAND(POPUP_INSERT_EMITTER_TRAIL, &CActionListCtrl::OnInsertEmitterTrail)
	ON_COMMAND(POPUP_INSERT_EMITTER_UVANIM, &CActionListCtrl::OnInsertEmitterUvanim)
END_MESSAGE_MAP()



// CActionListCtrl 메시지 처리기입니다.


int CActionListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CVenusGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	if(!m_image.Create(IDB_EFFECT_LIST_IMAGE,16,1,RGB(0, 255, 255)))
		return -1;

	SetImageList(&m_image, LVSIL_SMALL);
	CImageList *pImageList = GetImageList(LVSIL_SMALL);
	if(pImageList)
		ImageList_GetIconSize(pImageList->m_hImageList, &m_cxImage, &m_cyImage);
	else
		return -1;

	return 0;
}


void CActionListCtrl::InitializeGrid()
{
	/*
	ExtendedStyle support:
	LVS_EX_TRACKSELECT
	LVS_EX_GRIDLINES
	LVS_EX_FLATSB
	LVS_EX_CHECKBOXES
	LVS_EX_FLATSB
	all other ExStyles are not supported...buhhh and you call your self a windows-developer :(
	*/
	SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);

	LPTSTR lpszCols[] = {_T("Name"),0};

	LV_COLUMN   lvColumn;
	//initialize the columns
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 250;
	lvColumn.pszText = lpszCols[0];
	InsertColumn(0, &lvColumn);

//	lvColumn.cx = 100;
//	lvColumn.pszText = lpszCols[1];
//	InsertColumn(1, &lvColumn);
}

void CActionListCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (CVenusVisualManager::GetInstance().m_pResourceSystem == NULL) return;

	CMenu menu;
	menu.CreatePopupMenu();

	switch(CVenusVisualManager::GetInstance().m_pResourceSystem->GetSystemType())
	{
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_PARTICLE:
		{
			CNtlResourceParticleSystem* pParticleSystem = (CNtlResourceParticleSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem;
			NTL_ASSERTE(pParticleSystem != NULL);

			if (!pParticleSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_COLOR, "&Insert Emitter Color");
			if (!pParticleSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTTEXCOORDS))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_TEXCOORDS, "&Insert Emitter TexCoords");
			if (!pParticleSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_2DROTATE, "&Insert Emitter 2DRotate");
			if (!pParticleSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_SIZE, "&Insert Emitter Size");
			if (!pParticleSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTMATRIX))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MATRIX, "&Insert Emitter Matrix");
			if (!pParticleSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTHURRICANE))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_HURRICANE, "&Insert Emitter Hurricane");
			if (!pParticleSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTORBITPOINT))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_ORBITPOINT, "&Insert Emitter Orbit Point");
            menu.AppendMenu(MF_SEPARATOR, 0, "");
            if(!pParticleSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGTRAIL))
                menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_TRAIL, "&Insert Emitter Trail");

			menu.AppendMenu(MF_SEPARATOR, 0, "");
			if (pParticleSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR) &&
				!pParticleSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MULTI_COLOR, "&Insert Emitter Multi Color");
			if (pParticleSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTTEXCOORDS) &&
				!pParticleSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXCOORDS))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MULTI_TEXCOORDS, "&Insert Emitter Multi TexCoords");
			if (pParticleSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE) &&
				!pParticleSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MULTI_SIZE, "&Insert Emitter Multi Size");
			if (!pParticleSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXTURES))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MULTI_TEXTURES, "&Insert Emitter Multi Textures");

			if (!pParticleSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGPRTEMITTER))
			{
				menu.AppendMenu(MF_SEPARATOR, 0, "");
				menu.AppendMenu(MF_STRING, POPUP_INSERT_ATTACH_SYSTEM, "&Insert Sub Emitter");
			}

			if (pParticleSystem->m_nPathEmitterDataFlag == 0)
			{
				menu.AppendMenu(MF_SEPARATOR, 0, "");
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_SPLINE_PATH, "&Insert Spline Path");
			}

			if (pParticleSystem->m_nPointEmitterDataFlag == 0)
			{
				menu.AppendMenu(MF_SEPARATOR, 0, "");
				menu.AppendMenu(MF_STRING, POPUP_INSERT_POINT_EMITTER, "&Insert Point Emitter");
				menu.AppendMenu(MF_STRING, POPUP_INSERT_CIRCLE_EMITTER, "&Insert Circle Emitter");
				menu.AppendMenu(MF_STRING, POPUP_INSERT_SPHERE_EMITTER, "&Insert Sphere Emitter");
			}
		}
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_MESH:
		{
			CNtlResourceMeshSystem* pMeshSystem = (CNtlResourceMeshSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem;
			NTL_ASSERTE(pMeshSystem != NULL);

			if (!pMeshSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_COLOR, "&Insert Emitter Color");
			if (!pMeshSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_2DROTATE, "&Insert Emitter Rotate");
			if (!pMeshSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_SIZE, "&Insert Emitter Size");
            menu.AppendMenu(MF_SEPARATOR, 0, "");
            if(!pMeshSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGTRAIL))
                menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_TRAIL, "&Insert Emitter Trail");

			menu.AppendMenu(MF_SEPARATOR, 0, "");
			if (pMeshSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR) &&
				!pMeshSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MULTI_COLOR, "&Insert Emitter Multi Color");
			if (pMeshSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE) &&
				!pMeshSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MULTI_SIZE, "&Insert Emitter Multi Size");
			if (pMeshSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE) &&
				!pMeshSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTIROTATE))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MULTI_ROTATE, "&Insert Emitter Multi Rotate");

            menu.AppendMenu(MF_SEPARATOR, 0, "");
            if(!pMeshSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTFINISH))
                menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_FINISH, "&Insert Emitter Finish");

			if (pMeshSystem->m_nPathEmitterDataFlag == 0)
			{
				menu.AppendMenu(MF_SEPARATOR, 0, "");
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_SPLINE_PATH, "&Insert Spline Path");
			}
		}
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_BEAM:
		{
			CNtlResourceBeamSystem* pBeamSystem = (CNtlResourceBeamSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem;
			NTL_ASSERTE(pBeamSystem != NULL);

			if (!pBeamSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_COLOR, "&Insert Emitter Color");
			if (!pBeamSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_SIZE, "&Insert Emitter Size");
			if (!pBeamSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTFADEOUT))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_FADE_OUT, "&Insert Emitter FadeOut");

			menu.AppendMenu(MF_SEPARATOR, 0, "");
			if (pBeamSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR) &&
				!pBeamSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MULTI_COLOR, "&Insert Emitter Multi Color");
			if (pBeamSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE) &&
				!pBeamSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MULTI_SIZE, "&Insert Emitter Multi Size");
			if (!pBeamSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXTURES))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MULTI_TEXTURES, "&Insert Emitter Multi Textures");

            menu.AppendMenu(MF_SEPARATOR, 0, "");
            if(!pBeamSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTFINISH))
                menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_FINISH, "&Insert Emitter Finish");
		}
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_HURRICANE:
		{
			CNtlResourceHurricaneSystem* pHurricaneSystem = (CNtlResourceHurricaneSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem;
			NTL_ASSERTE(pHurricaneSystem != NULL);

			if (!pHurricaneSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_COLOR, "&Insert Emitter Color");
			if (!pHurricaneSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_SIZE, "&Insert Emitter Size");

			menu.AppendMenu(MF_SEPARATOR, 0, "");
			if (pHurricaneSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR) &&
				!pHurricaneSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MULTI_COLOR, "&Insert Emitter Multi Color");
			if (pHurricaneSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE) &&
				!pHurricaneSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MULTI_SIZE, "&Insert Emitter Multi Size");
			if (!pHurricaneSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXTURES))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MULTI_TEXTURES, "&Insert Emitter Multi Textures");

            menu.AppendMenu(MF_SEPARATOR, 0, "");
            if(!pHurricaneSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTFINISH))
                menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_FINISH, "&Insert Emitter Finish");
            
		}
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LIGHTNING:
		{
			CNtlResourceLightningSystem* pLightningSystem = (CNtlResourceLightningSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem;
			NTL_ASSERTE(pLightningSystem != NULL);

			if (!pLightningSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_COLOR, "&Insert Emitter Color");
			if (!pLightningSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_SIZE, "&Insert Emitter Size");

			menu.AppendMenu(MF_SEPARATOR, 0, "");
			if (pLightningSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR) &&
				!pLightningSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MULTI_COLOR, "&Insert Emitter Multi Color");
			if (pLightningSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE) &&
				!pLightningSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MULTI_SIZE, "&Insert Emitter Multi Size");
			if (!pLightningSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXTURES))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MULTI_TEXTURES, "&Insert Emitter Multi Textures");

            menu.AppendMenu(MF_SEPARATOR, 0, "");
            if(!pLightningSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTFINISH))
                menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_FINISH, "&Insert Emitter Finish");
		}
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_DECAL:
		{
			CNtlResourceDecalSystem* pDecalSystem = (CNtlResourceDecalSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem;
			NTL_ASSERTE(pDecalSystem != NULL);

			
			if (!pDecalSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_COLOR, "&Insert Emitter Color");
			if (!pDecalSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_2DROTATE, "&Insert Emitter Rotate");
			if (!pDecalSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_SIZE, "&Insert Emitter Size");
			if(!pDecalSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGUVANIM))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_UVANIM, "&Insert UV Animation");

			menu.AppendMenu(MF_SEPARATOR, 0, "");
			if (pDecalSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR) &&
				!pDecalSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MULTI_COLOR, "&Insert Emitter Multi Color");
			if (pDecalSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE) &&
				!pDecalSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MULTI_SIZE, "&Insert Emitter Multi Size");
			if (!pDecalSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXTURES))
				menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MULTI_TEXTURES, "&Insert Emitter Multi Textures");
		}
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_POST_EFFECT:
		{
			CNtlResourcePostEffectSystem* pPostEfefctSystem = (CNtlResourcePostEffectSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem;
			NTL_ASSERTE(pPostEfefctSystem != NULL);

            if(!pPostEfefctSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
            {
                menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_COLOR, "&Insert Emitter Color");
            }
            else if(!pPostEfefctSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
            {
                menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MULTI_COLOR, "&Insert Emitter Multi Color");
            }

            if(!pPostEfefctSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXTURES))
            {
                menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MULTI_TEXTURES, "&Insert Emitter Multi Textures");
            }                
		}
        break;    
    case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LINE:
        {
            CNtlResourceLineSystem* pLineSystem = (CNtlResourceLineSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem;
            NTL_ASSERTE(pLineSystem != NULL);

            if (!pLineSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
                menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_COLOR, "&Insert Emitter Color");
            if (!pLineSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
                menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_SIZE, "&Insert Emitter Size");            
            if (!pLineSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))
                menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_2DROTATE, "&Insert Emitter Rotate");

            menu.AppendMenu(MF_SEPARATOR, 0, "");
            if (pLineSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR) &&
                !pLineSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
                menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MULTI_COLOR, "&Insert Emitter Multi Color");
            if (pLineSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE) &&
                !pLineSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))
                menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MULTI_SIZE, "&Insert Emitter Multi Size");
            if (!pLineSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXTURES))
                menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MULTI_TEXTURES, "&Insert Emitter Multi Textures");
            if (pLineSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE) &&
                !pLineSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTIROTATE))
                menu.AppendMenu(MF_STRING, POPUP_INSERT_EMITTER_MULTI_ROTATE, "&Insert Emitter Multi Rotate");

            if (!pLineSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGSPHERE))
            {
                menu.AppendMenu(MF_SEPARATOR, 0, "");                
                menu.AppendMenu(MF_STRING, POPUP_INSERT_SPHERE_EMITTER, "&Insert Sphere Emitter");
            }
        }
        break;
	default:
		NTL_ASSERTE(!"default");
		break;
	}

	if(GetSelectedItem() != -1)
	{
		menu.AppendMenu(MF_SEPARATOR, 0, "");
		menu.AppendMenu(MF_STRING, ID_EDIT_CUT,		"Cut\tCtrl+X(&T)");
		menu.AppendMenu(MF_STRING, ID_EDIT_COPY,	"Copy\tCtrl+C(&C)");
	}
	if (CVenusPropertyContainer::GetInstance().m_nCopySystemDataFlag != 0)
	{
		menu.AppendMenu(MF_SEPARATOR, 0, "");
		menu.AppendMenu(MF_STRING, ID_EDIT_PASTE,	"Paste\tCtrl+V(&P)");
	}

	if(GetSelectedItem() != -1)
	{
		menu.AppendMenu(MF_SEPARATOR, 0, "");
		menu.AppendMenu(MF_STRING, POPUP_DELETE_ACTION, "&Delete Action");
	}

	menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}

RwUInt32 CActionListCtrl::GetSelectResourceData()
{
	NTL_ASSERTE(CVenusVisualManager::GetInstance().m_pResourceEffect != NULL);
	NTL_ASSERTE(CVenusVisualManager::GetInstance().m_pResourceSystem != NULL);

	RwUInt32 nDataFlag = 0;
	int nItem = GetSelectedItem();
	if(nItem == -1)
	{
		return nDataFlag;
	}

	CTreeItem* pSelectItem = GetTreeItem(GetSelectedItem());
	CItemInfo* pInfo = GetData(pSelectItem);
	switch(pInfo->GetImage())
	{
		case IMAGE_INDEX_EMITTER_COLOR:				nDataFlag = rpPRTSTDEMITTERDATAFLAGPRTCOLOR;		break;
		case IMAGE_INDEX_EMITTER_TEXCOORDS:			nDataFlag = rpPRTSTDEMITTERDATAFLAGPRTTEXCOORDS;	break;
		case IMAGE_INDEX_EMITTER_2DROTATE:			nDataFlag = rpPRTSTDEMITTERDATAFLAGPRT2DROTATE;		break;
		case IMAGE_INDEX_EMITTER_SIZE:				nDataFlag = rpPRTSTDEMITTERDATAFLAGPRTSIZE;			break;
		case IMAGE_INDEX_EMITTER_MATRIX:			nDataFlag = rpPRTSTDEMITTERDATAFLAGPRTMATRIX;		break;
		case IMAGE_INDEX_EMITTER_HURRICANE:			nDataFlag = rpPRTSTDEMITTERDATAFLAGPRTHURRICANE;	break;
		case IMAGE_INDEX_EMITTER_ORBITPOINT:		nDataFlag = rpPRTSTDEMITTERDATAFLAGPRTORBITPOINT;	break;
		case IMAGE_INDEX_EMITTER_FADE_OUT:			nDataFlag = rpPRTSTDEMITTERDATAFLAGPRTFADEOUT;		break;
        case IMAGE_INDEX_EMITTER_TRAIL:             nDataFlag = rpPRTSTDEMITTERDATAFLAGTRAIL;           break;
        case IMAGE_INDEX_EMITTER_FINISH:            nDataFlag = rpPRTSTDEMITTERDATAFLAGPRTFINISH;       break;
		case IMAGE_INDEX_EMITTER_UVANIM:			nDataFlag = rpPRTSTDEMITTERDATAFLAGUVANIM;			break;

		case IMAGE_INDEX_EMITTER_EMITTER:			nDataFlag = rpPRTADVEMITTERDATAFLAGPRTEMITTER;		break;
		case IMAGE_INDEX_EMITTER_MULTI_COLOR:		nDataFlag = rpPRTADVEMITTERDATAFLAGMULTICOLOR;		break;
		case IMAGE_INDEX_EMITTER_MULTI_TEXCOORDS:	nDataFlag = rpPRTADVEMITTERDATAFLAGMULTITEXCOORDS;	break;
		case IMAGE_INDEX_EMITTER_MULTI_SIZE:		nDataFlag = rpPRTADVEMITTERDATAFLAGMULTISIZE;		break;
		case IMAGE_INDEX_EMITTER_MULTI_ROTATE:		nDataFlag = rpPRTADVEMITTERDATAFLAGMULTIROTATE;		break;

		case IMAGE_INDEX_EMITTER_MULTI_TEXTURES:	nDataFlag = rpPRTADVEMITTERDATAFLAGMULTITEXTURES;	break;

		case IMAGE_INDEX_EMITTER_SPLINE_PATH:		nDataFlag = rpPRTADVEMITTERDATAFLAGSPLINEPATH;		break;

		case IMAGE_INDEX_POINT_EMITTER:				nDataFlag = rpPRTADVEMITTERDATAFLAGPOINTLIST;		break;
		case IMAGE_INDEX_CIRCLE_EMITTER:			nDataFlag = rpPRTADVEMITTERDATAFLAGCIRCLE;			break;
		case IMAGE_INDEX_SPHERE_EMITTER:			nDataFlag = rpPRTADVEMITTERDATAFLAGSPHERE;			break;
            

		default:									NTL_ASSERTE(!"default");							break;
	}
	return nDataFlag;
}

RwInt32 CActionListCtrl::GetImageIndexFromDataFlag(RwUInt32 nDataFlag)
{
	switch(nDataFlag)
	{
		case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:		return IMAGE_INDEX_EMITTER_COLOR;			break;
		case rpPRTSTDEMITTERDATAFLAGPRTTEXCOORDS:	return IMAGE_INDEX_EMITTER_TEXCOORDS;		break;
		case rpPRTSTDEMITTERDATAFLAGPRT2DROTATE:	return IMAGE_INDEX_EMITTER_2DROTATE;		break;
		case rpPRTSTDEMITTERDATAFLAGPRTSIZE:		return IMAGE_INDEX_EMITTER_SIZE;			break;
		case rpPRTSTDEMITTERDATAFLAGPRTMATRIX:		return IMAGE_INDEX_EMITTER_MATRIX;			break;
		case rpPRTSTDEMITTERDATAFLAGPRTHURRICANE:	return IMAGE_INDEX_EMITTER_HURRICANE;		break;
		case rpPRTSTDEMITTERDATAFLAGPRTORBITPOINT:	return IMAGE_INDEX_EMITTER_ORBITPOINT;		break;
		case rpPRTSTDEMITTERDATAFLAGPRTFADEOUT:		return IMAGE_INDEX_EMITTER_FADE_OUT;		break;
        case rpPRTSTDEMITTERDATAFLAGPRTFINISH:      return IMAGE_INDEX_EMITTER_FINISH;          break;
        case rpPRTSTDEMITTERDATAFLAGTRAIL:          return IMAGE_INDEX_EMITTER_TRAIL;           break;
		case rpPRTSTDEMITTERDATAFLAGUVANIM:			return IMAGE_INDEX_EMITTER_UVANIM;			break;

		case rpPRTADVEMITTERDATAFLAGPRTEMITTER:		return IMAGE_INDEX_EMITTER_EMITTER;			break;
		case rpPRTADVEMITTERDATAFLAGMULTICOLOR:		return IMAGE_INDEX_EMITTER_MULTI_COLOR;		break;
		case rpPRTADVEMITTERDATAFLAGMULTITEXCOORDS:	return IMAGE_INDEX_EMITTER_MULTI_TEXCOORDS;	break;
		case rpPRTADVEMITTERDATAFLAGMULTISIZE:		return IMAGE_INDEX_EMITTER_MULTI_SIZE;		break;

		case rpPRTADVEMITTERDATAFLAGMULTITEXTURES:	return IMAGE_INDEX_EMITTER_MULTI_TEXTURES;	break;

		case rpPRTADVEMITTERDATAFLAGSPLINEPATH:		return IMAGE_INDEX_EMITTER_SPLINE_PATH;		break;

		case rpPRTADVEMITTERDATAFLAGPOINTLIST:		return IMAGE_INDEX_POINT_EMITTER;			break;
		case rpPRTADVEMITTERDATAFLAGCIRCLE:			return IMAGE_INDEX_CIRCLE_EMITTER;			break;
		case rpPRTADVEMITTERDATAFLAGSPHERE:			return IMAGE_INDEX_SPHERE_EMITTER;			break;
        

		default:									NTL_ASSERTE(!"default");					break;
	}
	return -1;
}

void CActionListCtrl::ResetResourceSystem()
{
	SetRedraw(0);
	m_bListUpdate = FALSE;


	DeleteAllItems();
	DeleteAll();

	if (CVenusVisualManager::GetInstance().m_pResourceSystem == NULL)
	{
		m_bListUpdate = TRUE;
		SetRedraw(1);
		return;
	}

	switch(CVenusVisualManager::GetInstance().m_pResourceSystem->GetSystemType())
	{
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_PARTICLE:
		SetResourceSystem((CNtlResourceParticleSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem);
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_MESH:
		SetResourceSystem((CNtlResourceMeshSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem);
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_BEAM:
		SetResourceSystem((CNtlResourceBeamSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem);
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_HURRICANE:
		SetResourceSystem((CNtlResourceHurricaneSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem);
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LIGHTNING:
		SetResourceSystem((CNtlResourceLightningSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem);
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_DECAL:
		SetResourceSystem((CNtlResourceDecalSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem);
		break;
	case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_POST_EFFECT:
		SetResourceSystem((CNtlResourcePostEffectSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem);
		break;
    case CNtlResourceComponentSystem::RESOURCE_SYSTEMTYPE_LINE:
        SetResourceSystem((CNtlResourceLineSystem*)CVenusVisualManager::GetInstance().m_pResourceSystem);
        break;
	default:
		NTL_ASSERTE(!"default");
		break;
	}
	m_bListUpdate = TRUE;
	SetRedraw(1);
}

void CActionListCtrl::SetResourceSystem(CNtlResourceParticleSystem* pResourceSystem)
{
	// Color
	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_COLOR], IMAGE_INDEX_EMITTER_COLOR, (DWORD_PTR)&pResourceSystem->m_EmitterPrtCol, NULL);
	}
	// TexCoords
	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTTEXCOORDS))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_TEXCOORDS], IMAGE_INDEX_EMITTER_TEXCOORDS, (DWORD_PTR)&pResourceSystem->m_EmitterPrtTexCoords, NULL);
	}
	// 2DRotate
	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_2DROTATE], IMAGE_INDEX_EMITTER_2DROTATE, (DWORD_PTR)&pResourceSystem->m_EmitterPrtRotate, NULL);
	}
	// Size
	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_SIZE], IMAGE_INDEX_EMITTER_SIZE, (DWORD_PTR)&pResourceSystem->m_EmitterPrtSize, NULL);
	}
	// Matrix
	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTMATRIX))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_MATRIX], IMAGE_INDEX_EMITTER_MATRIX, (DWORD_PTR)&pResourceSystem->m_EmitterPrtMatrix, NULL);
	}
	// Matrix
	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTHURRICANE))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_HURRICANE], IMAGE_INDEX_EMITTER_HURRICANE, (DWORD_PTR)&pResourceSystem->m_EmitterPrtHurricane, NULL);
	}
	// Matrix
	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTORBITPOINT))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_ORBITPOINT], IMAGE_INDEX_EMITTER_ORBITPOINT, (DWORD_PTR)&pResourceSystem->m_EmitterPrtOrbitPoint, NULL);
	}

	// Emitter Emitter
	if (pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGPRTEMITTER))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_EMITTER], IMAGE_INDEX_EMITTER_EMITTER, (DWORD_PTR)&pResourceSystem->m_PrtEmitterEmitter, NULL);
	}
	// Multi Color
	if (pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_MULTI_COLOR], IMAGE_INDEX_EMITTER_MULTI_COLOR, (DWORD_PTR)&pResourceSystem->m_EmitterPrtMultiCol, NULL);
	}
	// Multi TexCoords
	if (pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXCOORDS))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_MULTI_TEXCOORDS], IMAGE_INDEX_EMITTER_MULTI_TEXCOORDS, (DWORD_PTR)&pResourceSystem->m_EmitterPrtMultiTexCoords, NULL);
	}
	// Multi Size
	if (pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_MULTI_SIZE], IMAGE_INDEX_EMITTER_MULTI_SIZE, (DWORD_PTR)&pResourceSystem->m_EmitterPrtMultiSize, NULL);
	}
	// Multi Textures
	if (pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXTURES))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_MULTI_TEXTURES], IMAGE_INDEX_EMITTER_MULTI_TEXTURES, (DWORD_PTR)&pResourceSystem->m_EmitterMultiTextures, NULL);
	}
    // Trail
    if(pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGTRAIL))
    {
        InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_TRAIL], IMAGE_INDEX_EMITTER_TRAIL, (DWORD_PTR)&pResourceSystem->m_EmitterTrail, NULL);
    }

	switch(pResourceSystem->m_nPathEmitterDataFlag)
	{
	case rpPRTADVEMITTERDATAFLAGSPLINEPATH:
		{
			InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_SPLINE_PATH], IMAGE_INDEX_EMITTER_SPLINE_PATH, (DWORD_PTR)&pResourceSystem->m_EmitterPrtSplinePath, NULL);
		}
		break;
	}

	switch(pResourceSystem->m_nPointEmitterDataFlag)
	{
	case rpPRTADVEMITTERDATAFLAGPOINTLIST:
		{
			InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_POINT_EMITTER], IMAGE_INDEX_POINT_EMITTER, (DWORD_PTR)&pResourceSystem->m_EmitterPtList, NULL);
		}
		break;
	case rpPRTADVEMITTERDATAFLAGCIRCLE:
		{
			InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_CIRCLE_EMITTER], IMAGE_INDEX_CIRCLE_EMITTER, (DWORD_PTR)&pResourceSystem->m_EmitterPtrCircle, NULL);
		}
		break;
	case rpPRTADVEMITTERDATAFLAGSPHERE:
		{
			InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_SPHERE_EMITTER], IMAGE_INDEX_SPHERE_EMITTER, (DWORD_PTR)&pResourceSystem->m_EmitterPtrSphere, NULL);
		}
		break;
	}
}

void CActionListCtrl::SetResourceSystem(CNtlResourceMeshSystem* pResourceSystem)
{
	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_COLOR], IMAGE_INDEX_EMITTER_COLOR, (DWORD_PTR)&pResourceSystem->m_EmitterPrtCol, NULL);
	}
	// 2DRotate
	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_2DROTATE], IMAGE_INDEX_EMITTER_2DROTATE, (DWORD_PTR)&pResourceSystem->m_EmitterPrtRotate, NULL);
	}
	// Size
	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_SIZE], IMAGE_INDEX_EMITTER_SIZE, (DWORD_PTR)&pResourceSystem->m_EmitterPrtSize, NULL);
	}
	if (pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_MULTI_COLOR], IMAGE_INDEX_EMITTER_MULTI_COLOR, (DWORD_PTR)&pResourceSystem->m_EmitterPrtMultiCol, NULL);
	}
	if (pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_MULTI_SIZE], IMAGE_INDEX_EMITTER_MULTI_SIZE, (DWORD_PTR)&pResourceSystem->m_EmitterPrtMultiSize, NULL);
	}
	if (pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTIROTATE))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_MULTI_ROTATE], IMAGE_INDEX_EMITTER_MULTI_ROTATE, (DWORD_PTR)&pResourceSystem->m_EmitterPrtMultiRotate, NULL);
	}
    // Trail
    if(pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGTRAIL))
    {
        InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_TRAIL], IMAGE_INDEX_EMITTER_TRAIL, (DWORD_PTR)&pResourceSystem->m_EmitterTrail, NULL);
    }

	switch(pResourceSystem->m_nPathEmitterDataFlag)
	{
	case rpPRTADVEMITTERDATAFLAGSPLINEPATH:
		{
			InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_SPLINE_PATH], IMAGE_INDEX_EMITTER_SPLINE_PATH, (DWORD_PTR)&pResourceSystem->m_EmitterPrtSplinePath, NULL);
		}
		break;
	}

    if(pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTFINISH))
    {
        InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_FINISH], IMAGE_INDEX_EMITTER_FINISH, (DWORD_PTR)&pResourceSystem->m_EmitterPrtFinish, NULL);
    }
}

void CActionListCtrl::SetResourceSystem(CNtlResourceImVertexSystem* pResourceSystem)
{
	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_COLOR], IMAGE_INDEX_EMITTER_COLOR, (DWORD_PTR)&pResourceSystem->m_EmitterPrtCol, NULL);
	}
	// Size
	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTSIZE))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_SIZE], IMAGE_INDEX_EMITTER_SIZE, (DWORD_PTR)&pResourceSystem->m_EmitterPrtSize, NULL);
	}
	// fade Out
	if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTFADEOUT))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_FADE_OUT], IMAGE_INDEX_EMITTER_FADE_OUT, (DWORD_PTR)&pResourceSystem->m_EmitterPrtFadeOut, NULL);
	}
	if (pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_MULTI_COLOR], IMAGE_INDEX_EMITTER_MULTI_COLOR, (DWORD_PTR)&pResourceSystem->m_EmitterPrtMultiCol, NULL);
	}
	if (pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTISIZE))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_MULTI_SIZE], IMAGE_INDEX_EMITTER_MULTI_SIZE, (DWORD_PTR)&pResourceSystem->m_EmitterPrtMultiSize, NULL);
	}
	// Multi Textures
	if (pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXTURES))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_MULTI_TEXTURES], IMAGE_INDEX_EMITTER_MULTI_TEXTURES, (DWORD_PTR)&pResourceSystem->m_EmitterMultiTextures, NULL);
	}

    if(pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTFINISH))
    {
        InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_FINISH], IMAGE_INDEX_EMITTER_FINISH, (DWORD_PTR)&pResourceSystem->m_EmitterPrtFinish, NULL);
    }
}

void CActionListCtrl::SetResourceSystem(CNtlResourceBeamSystem* pResourceSystem)
{
	SetResourceSystem((CNtlResourceImVertexSystem*)pResourceSystem);
}

void CActionListCtrl::SetResourceSystem(CNtlResourceHurricaneSystem* pResourceSystem)
{
	SetResourceSystem((CNtlResourceImVertexSystem*)pResourceSystem);
}

void CActionListCtrl::SetResourceSystem(CNtlResourceLightningSystem* pResourceSystem)
{
	SetResourceSystem((CNtlResourceImVertexSystem*)pResourceSystem);
}

void CActionListCtrl::SetResourceSystem(CNtlResourceDecalSystem* pResourceSystem)
{
	SetResourceSystem((CNtlResourceImVertexSystem*)pResourceSystem);

	// Rotate 
	if(pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_2DROTATE], IMAGE_INDEX_EMITTER_2DROTATE, (DWORD_PTR)&pResourceSystem->m_EmitterPrtRotate, NULL);
	}

	// UV Animation
	if(pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGUVANIM))
	{
		InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_UVANIM], IMAGE_INDEX_EMITTER_UVANIM, (DWORD_PTR)&pResourceSystem->m_EmitterUVAnim, NULL);
	}
}

void CActionListCtrl::SetResourceSystem(CNtlResourcePostEffectSystem* pResourceSystem)
{
    // Color
    if (pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTCOLOR))
    {
        InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_COLOR], IMAGE_INDEX_EMITTER_COLOR, (DWORD_PTR)&pResourceSystem->m_EmitterPrtCol, NULL);
    }

    // Multi Color
    if (pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTICOLOR))
    {
        InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_MULTI_COLOR], IMAGE_INDEX_EMITTER_MULTI_COLOR, (DWORD_PTR)&pResourceSystem->m_EmitterPrtMultiCol, NULL);
    }

    // Multi Textures
    if (pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTITEXTURES))
    {
        InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_MULTI_TEXTURES], IMAGE_INDEX_EMITTER_MULTI_TEXTURES, (DWORD_PTR)&pResourceSystem->m_EmitterMultiTextures, NULL);
    }
}

void CActionListCtrl::SetResourceSystem( CNtlResourceLineSystem* pResourceSystem )
{
    SetResourceSystem((CNtlResourceImVertexSystem*)pResourceSystem);

    // Sphere Emitter
    if (pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGSPHERE))
    {
        InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_SPHERE_EMITTER], IMAGE_INDEX_SPHERE_EMITTER, (DWORD_PTR)&pResourceSystem->m_EmitterSphere, NULL);
    }

    // Rotate
    if(pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))
    {
        InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_2DROTATE], IMAGE_INDEX_EMITTER_2DROTATE, (DWORD_PTR)&pResourceSystem->m_EmitterPrtRotate, NULL);
    }

    // Multi Rotate
    if(pResourceSystem->IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTIROTATE))
    {
        InsertTreeItem(EFFECT_ACTION_STRING[IMAGE_INDEX_EMITTER_MULTI_ROTATE], IMAGE_INDEX_EMITTER_MULTI_ROTATE, (DWORD_PTR)&pResourceSystem->m_EmitterPrtMultiRotate, NULL);
    }
}

//------------------------------------------------------------------
//	FuncName	: CopyAction
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CActionListCtrl::CopyAction(RwInt32 nImage, RwUInt32 nEmitterDataFlag, DWORD_PTR dataPtr)
{
	NTL_ASSERTE(CVenusPropertyContainer::GetInstance().m_pCopyResourceEffect != NULL);
	NTL_ASSERTE(CVenusPropertyContainer::GetInstance().m_pCopyResourceSystem != NULL);

	NTL_ASSERTE(CVenusVisualManager::GetInstance().m_pResourceEffect != NULL);
	NTL_ASSERTE(CVenusVisualManager::GetInstance().m_pResourceSystem != NULL);

	CVenusPropertyContainer::GetInstance().SetEmitterDataFlag(CVenusVisualManager::GetInstance().m_pResourceSystem, nEmitterDataFlag, CVenusPropertyContainer::GetInstance().m_pCopyResourceSystem);
	CVenusVisualManager::GetInstance().ResetInstanceEffect(CVenusVisualManager::GetInstance().m_pResourceEffect->GetName());

	InsertTreeItem(EFFECT_ACTION_STRING[nImage], nImage, dataPtr, NULL, LVIS_SELECTED | LVIS_FOCUSED);
	InvalidateRect(NULL, TRUE);
}

//------------------------------------------------------------------
//	FuncName	: CreateAction
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CActionListCtrl::CreateAction(RwInt32 nImage, RwUInt32 nEmitterDataFlag, DWORD_PTR dataPtr)
{
	NTL_ASSERTE(CVenusVisualManager::GetInstance().m_pResourceEffect != NULL);
	NTL_ASSERTE(CVenusVisualManager::GetInstance().m_pResourceSystem != NULL);

	CVenusPropertyContainer::GetInstance().SetEmitterDataFlag(CVenusVisualManager::GetInstance().m_pResourceSystem, nEmitterDataFlag);

	CVenusVisualManager::GetInstance().ResetInstanceEffect(CVenusVisualManager::GetInstance().m_pResourceEffect->GetName());

	InsertTreeItem(EFFECT_ACTION_STRING[nImage], nImage, dataPtr, NULL, LVIS_SELECTED | LVIS_FOCUSED);
	InvalidateRect(NULL, TRUE);
}


void CActionListCtrl::OnInsertEmitterColor()
{
	CreateAction(IMAGE_INDEX_EMITTER_COLOR, rpPRTSTDEMITTERDATAFLAGPRTCOLOR, NULL);
}
void CActionListCtrl::OnInsertEmitterTexcoords()
{
	CreateAction(IMAGE_INDEX_EMITTER_TEXCOORDS, rpPRTSTDEMITTERDATAFLAGPRTTEXCOORDS, NULL);
}

void CActionListCtrl::OnInsertEmitter2drotate()
{
	CreateAction(IMAGE_INDEX_EMITTER_2DROTATE, rpPRTSTDEMITTERDATAFLAGPRT2DROTATE, NULL);
}

void CActionListCtrl::OnInsertEmitterSize()
{
	CreateAction(IMAGE_INDEX_EMITTER_SIZE, rpPRTSTDEMITTERDATAFLAGPRTSIZE, NULL);
}
void CActionListCtrl::OnInsertEmitterMatrix()
{
	CreateAction(IMAGE_INDEX_EMITTER_MATRIX, rpPRTSTDEMITTERDATAFLAGPRTMATRIX, NULL);
}

void CActionListCtrl::OnInsertEmitterHurricane()
{
	CreateAction(IMAGE_INDEX_EMITTER_HURRICANE, rpPRTSTDEMITTERDATAFLAGPRTHURRICANE, NULL);
}
void CActionListCtrl::OnInsertEmitterOrbitPoint()
{
	CreateAction(IMAGE_INDEX_EMITTER_ORBITPOINT, rpPRTSTDEMITTERDATAFLAGPRTORBITPOINT, NULL);
}

void CActionListCtrl::OnInsertEmitterFadeOut()
{
	CreateAction(IMAGE_INDEX_EMITTER_FADE_OUT, rpPRTSTDEMITTERDATAFLAGPRTFADEOUT, NULL);
}

void CActionListCtrl::OnInsertEmitterFinish()
{
    CreateAction(IMAGE_INDEX_EMITTER_FINISH, rpPRTSTDEMITTERDATAFLAGPRTFINISH, NULL);    
}


void CActionListCtrl::OnInsertAttachSystem()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (CVenusVisualManager::GetInstance().m_pResourceSystem->IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRTMATRIX))
	{
//		RwUInt32 result = AfxMessageBox("Matrix Action 과 Sub System 을 같이 쓰면 에러가 난다.\n에러가 나는 이유는 현재 알수가 없으므로 Matrix 속성을 지우고 해라", MB_OK);
		return;
	}
	CreateAction(IMAGE_INDEX_EMITTER_EMITTER, rpPRTADVEMITTERDATAFLAGPRTEMITTER, NULL);
}

void CActionListCtrl::OnInsertEmitterMultiColor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CreateAction(IMAGE_INDEX_EMITTER_MULTI_COLOR, rpPRTADVEMITTERDATAFLAGMULTICOLOR, NULL);
}

void CActionListCtrl::OnInsertEmitterMultiSize()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CreateAction(IMAGE_INDEX_EMITTER_MULTI_SIZE, rpPRTADVEMITTERDATAFLAGMULTISIZE, NULL);
}
void CActionListCtrl::OnInsertEmitterMultiRotate()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CreateAction(IMAGE_INDEX_EMITTER_MULTI_ROTATE, rpPRTADVEMITTERDATAFLAGMULTIROTATE, NULL);
}

void CActionListCtrl::OnInsertEmitterMultiTexcoords()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CreateAction(IMAGE_INDEX_EMITTER_MULTI_TEXCOORDS, rpPRTADVEMITTERDATAFLAGMULTITEXCOORDS, NULL);
}

void CActionListCtrl::OnInsertEmitterMultiTextures()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CreateAction(IMAGE_INDEX_EMITTER_MULTI_TEXTURES, rpPRTADVEMITTERDATAFLAGMULTITEXTURES, NULL);
}

void CActionListCtrl::OnInsertPointEmitter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CreateAction(IMAGE_INDEX_POINT_EMITTER, rpPRTADVEMITTERDATAFLAGPOINTLIST, NULL);
}

void CActionListCtrl::OnInsertCircleEmitter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CreateAction(IMAGE_INDEX_CIRCLE_EMITTER, rpPRTADVEMITTERDATAFLAGCIRCLE, NULL);
}

void CActionListCtrl::OnInsertSphereEmitter()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CreateAction(IMAGE_INDEX_SPHERE_EMITTER, rpPRTADVEMITTERDATAFLAGSPHERE, NULL);
}


void CActionListCtrl::OnInsertEmitterSplinePath()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CreateAction(IMAGE_INDEX_EMITTER_SPLINE_PATH, rpPRTADVEMITTERDATAFLAGSPLINEPATH, NULL);
}

void CActionListCtrl::OnInsertEmitterTrail()
{
    CreateAction(IMAGE_INDEX_EMITTER_TRAIL, rpPRTSTDEMITTERDATAFLAGTRAIL, NULL);
}

void CActionListCtrl::OnInsertEmitterUvanim()
{
	CreateAction(IMAGE_INDEX_EMITTER_UVANIM, rpPRTSTDEMITTERDATAFLAGUVANIM, NULL);
}

void CActionListCtrl::OnDeleteAction()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (AfxMessageBox("really ?", MB_OKCANCEL) != IDOK) return;
	RwInt32 nDataFlag = GetSelectResourceData();
	RwInt32 nMultiDataFlag = 0;

	switch(nDataFlag)
	{
		case rpPRTSTDEMITTERDATAFLAGPRTCOLOR:		nMultiDataFlag = rpPRTADVEMITTERDATAFLAGMULTICOLOR;		break;
		case rpPRTSTDEMITTERDATAFLAGPRTTEXCOORDS:	nMultiDataFlag = rpPRTADVEMITTERDATAFLAGMULTITEXCOORDS;	break;
		case rpPRTSTDEMITTERDATAFLAGPRTSIZE:		nMultiDataFlag = rpPRTADVEMITTERDATAFLAGMULTISIZE;		break;
	}

	if (CVenusVisualManager::GetInstance().m_pResourceSystem != NULL)
	{
		CVenusPropertyContainer::GetInstance().DeleteEmitterDataFlag(CVenusVisualManager::GetInstance().m_pResourceSystem, nDataFlag);

		if (nMultiDataFlag != 0 && CVenusVisualManager::GetInstance().m_pResourceSystem->IsEmitterDataFlag(nMultiDataFlag))
		{
			CVenusPropertyContainer::GetInstance().DeleteEmitterDataFlag(CVenusVisualManager::GetInstance().m_pResourceSystem, nMultiDataFlag);
			CVenusVisualManager::GetInstance().m_pResourceEffect->ResetSubSystem();
			CVenusVisualManager::GetInstance().ResetInstanceEffect(CVenusVisualManager::GetInstance().m_pResourceEffect->GetName());
			ResetResourceSystem();
		}
		else
		{
			CVenusVisualManager::GetInstance().m_pResourceEffect->ResetSubSystem();
			CVenusVisualManager::GetInstance().ResetInstanceEffect(CVenusVisualManager::GetInstance().m_pResourceEffect->GetName());
			DeleteItemEx(GetTreeItem(GetSelectedItem()), GetSelectedItem());
		}
	}
	InvalidateRect(NULL, TRUE);
}


void CActionListCtrl::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_bListUpdate)
	{
		RwInt32 nDataFlag = GetSelectResourceData();

		CVenusVisualManager::GetInstance().SetSelectResourceData(CVenusVisualManager::GetInstance().m_pResourceEffect, CVenusVisualManager::GetInstance().m_pResourceSystem, nDataFlag);

		CEffectEditLayer* pView = (CEffectEditLayer*)gGetEditLayer(EDIT_LAYER_EFFECT_EDIT);
		pView->ResetPropertiesOnly();
	}

	*pResult = 0;
}

void CActionListCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	RwInt32 nDataFlag = GetSelectResourceData();

	CVenusVisualManager::GetInstance().SetSelectResourceData(CVenusVisualManager::GetInstance().m_pResourceEffect, CVenusVisualManager::GetInstance().m_pResourceSystem, nDataFlag);

	CEffectEditLayer* pView = (CEffectEditLayer*)gGetEditLayer(EDIT_LAYER_EFFECT_EDIT);
	pView->ResetPropertiesOnly();

	*pResult = 0;
}

void CActionListCtrl::OnEditCopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	RwInt32 nDataFlag = GetSelectResourceData();
	CVenusPropertyContainer::GetInstance().SetCopyResourceData(TRUE, CVenusVisualManager::GetInstance().m_pResourceEffect, CVenusVisualManager::GetInstance().m_pResourceSystem, nDataFlag);
}

void CActionListCtrl::OnEditCut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	RwInt32 nDataFlag = GetSelectResourceData();
	CVenusPropertyContainer::GetInstance().SetCopyResourceData(FALSE, CVenusVisualManager::GetInstance().m_pResourceEffect, CVenusVisualManager::GetInstance().m_pResourceSystem, nDataFlag);
}

void CActionListCtrl::OnEditPaste()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	NTL_ASSERTE(CVenusPropertyContainer::GetInstance().m_pCopyResourceEffect != NULL);
	NTL_ASSERTE(CVenusPropertyContainer::GetInstance().m_pCopyResourceSystem != NULL);
	NTL_ASSERTE(CVenusPropertyContainer::GetInstance().m_nCopySystemDataFlag != 0);

	NTL_ASSERTE(CVenusVisualManager::GetInstance().m_pResourceEffect != NULL);
	NTL_ASSERTE(CVenusVisualManager::GetInstance().m_pResourceSystem != NULL);

	// 같은 flag 가 세팅되어 있는지 확인해서 세팅되어 있지 않을때만 copy 한다
	if (CVenusVisualManager::GetInstance().m_pResourceSystem->IsEmitterDataFlag(CVenusPropertyContainer::GetInstance().m_nCopySystemDataFlag))
	{
		AfxMessageBox(CStringManager::GetInstance().GetString(STRING_ALREADY_SETTING_FLAG), MB_OK);
		return;
	}
	// 같은 system type 일 경우만 복사한다
	if (CVenusPropertyContainer::GetInstance().m_pCopyResourceSystem->GetSystemType() != CVenusVisualManager::GetInstance().m_pResourceSystem->GetSystemType())
	{
		AfxMessageBox(CStringManager::GetInstance().GetString(STRING_DIFFERENT_SYSTEM_TYPE), MB_OK);
		return;
	}

	CopyAction(GetImageIndexFromDataFlag(CVenusPropertyContainer::GetInstance().m_nCopySystemDataFlag), CVenusPropertyContainer::GetInstance().m_nCopySystemDataFlag, NULL);

	if (!CVenusPropertyContainer::GetInstance().m_bCopyResourceData)
	{
		CVenusPropertyContainer::GetInstance().DeleteEmitterDataFlag(CVenusPropertyContainer::GetInstance().m_pCopyResourceSystem, CVenusPropertyContainer::GetInstance().m_nCopySystemDataFlag);
		CVenusPropertyContainer::GetInstance().m_pCopyResourceEffect->ResetSubSystem();

		CVenusVisualManager::GetInstance().ResetInstanceEffect(CVenusPropertyContainer::GetInstance().m_pCopyResourceEffect->GetName());

		ResetResourceSystem();

		CVenusPropertyContainer::GetInstance().SetCopyResourceData(TRUE, NULL, NULL, 0);
	}
}

void CActionListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nChar)
	{
	case VK_CONTROL:
		{
		}
		break;
	case 'C':
		{
			if (nFlags & MK_CONTROL)
				OnEditCopy();
		}
		break;
	case 'T':
		{
			if (nFlags & MK_CONTROL)
				OnEditCut();
		}
		break;
	case 'V':
		{
			if (nFlags & MK_CONTROL)
			{
				if (CVenusVisualManager::GetInstance().m_pResourceEffect != NULL && 
					CVenusVisualManager::GetInstance().m_pResourceSystem != NULL && 
					CVenusPropertyContainer::GetInstance().m_nCopySystemDataFlag != 0)
				{
					OnEditPaste();
				}
			}
		}
		break;
	case VK_DELETE:
		{
			if(GetSelectedItem() != -1)
			{
				OnDeleteAction();
			}
		}
		break;
	}

	CVenusGridCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CActionListCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nChar)
	{
		case VK_CONTROL:
		{
		}
		break;
	}

	CVenusGridCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
}




