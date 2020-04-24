// PalettePlantDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "PalettePlantDlg.h"

#include "NtlWorldPaletteDefine.h"

#include "NtlWorldFieldManager.h"
#include "NtlPLPropertyContainer.h"
#include "NtlPLPlantProperty.h"
#include "NtlPLWorldEntity.h"
#include "NtlPLPlant.h"

#define dPLANT_MAX_IN_SECTOR 100

// CPalettePlantDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPalettePlantDlg, CDialog)

CPalettePlantDlg::CPalettePlantDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPalettePlantDlg::IDD, pParent)
	, m_strNumMin(_T(""))
	, m_strNumMax(_T(""))
	, m_strScaleMin(_T(""))
	, m_strScaleMax(_T(""))
	, m_strRotationTerrain(_T(""))
	, m_strBrushSize(_T(""))
	, m_strClipDistance(_T(""))
{

}

CPalettePlantDlg::~CPalettePlantDlg()
{
}

void CPalettePlantDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLANT_SET_LISTBOX, m_PlantSetList);
	DDX_Text(pDX, IDC_NUM_MIN, m_strNumMin);
	DDX_Text(pDX, IDC_NUM_MAX, m_strNumMax);
	DDX_Text(pDX, IDC_SCALE_MIN, m_strScaleMin);
	DDX_Text(pDX, IDC_SCALE_MAX, m_strScaleMax);
	DDX_Text(pDX, IDC_ROTATION_TERRAIN, m_strRotationTerrain);
	DDX_Control(pDX, IDC_PLANT_SET_RES_LISTBOX, m_PlantSetResList);
	DDX_Control(pDX, IDC_BRUSH_SIZE_SLIDER, m_BrushSizeSlider);
	DDX_Text(pDX, IDC_BRUSH_SIZE, m_strBrushSize);
	DDX_Control(pDX, IDC_CLIP_DISTANCE_SLIDER, m_ClipDistanceSlider);
	DDX_Text(pDX, IDC_CLIP_DISTANCE, m_strClipDistance);
}


BEGIN_MESSAGE_MAP(CPalettePlantDlg, CDialog)
	ON_LBN_SELCHANGE(IDC_PLANT_SET_LISTBOX, &CPalettePlantDlg::OnLbnSelchangePlantSetListbox)
	ON_BN_CLICKED(IDC_REFRESH, &CPalettePlantDlg::OnBnClickedRefresh)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CPalettePlantDlg 메시지 처리기입니다.

LRESULT CPalettePlantDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		{
			dGET_CUR_PALETTE() = ePM_PLANT;
			dGETHDL()->DeleteAllPaletteDummies();
			break;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CPalettePlantDlg::PreTranslateMessage(MSG* pMsg)
{
	// windows messages
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
			{
				return true;
				break;
			}
		case VK_RETURN:
			{
				if (pMsg->hwnd == GetDlgItem(IDC_BRUSH_SIZE)->m_hWnd)
				{
					UpdateData(TRUE);

					SetBrushSize(static_cast<RwReal>(::atof(m_strBrushSize.GetBuffer(0))));
				}
				else if (pMsg->hwnd == GetDlgItem(IDC_CLIP_DISTANCE)->m_hWnd)
				{
					UpdateData(TRUE);

					SetClipDistance(static_cast<RwReal>(::atof(m_strClipDistance.GetBuffer(0))));
				}
				return true;
				break;
			}
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CPalettePlantDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_BrushSizeSlider.SetRange(1, 100);
	m_ClipDistanceSlider.SetRange(0, 512);
	
	return TRUE;
}

void CPalettePlantDlg::InitializeWorldData()
{
	UpdateData(TRUE);
	m_PlantSetList.ResetContent();


	CNtlWorldFieldManager*				pMgr		= dGETMGR();
	CNtlPLPlantProperty*				pProperty	= static_cast<CNtlPLPlantProperty*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_PLANT, NTL_PLEN_PLANT));

	if (!pProperty)
	{
		return;
	}

	CNtlPLPlantProperty::PROPERTY_MAP&	mapProperty	= pProperty->m_mapProperty;
	for (CNtlPLPlantProperty::PROPERTY_MAP_IT it = mapProperty.begin(); it != mapProperty.end(); ++it)
	{
		RwInt32								iIdx	= it->first; 
		CNtlPLPlantProperty::sPLANT_PROP&	sProp	= it->second;
		
		m_PlantSetList.SetItemData(m_PlantSetList.AddString(sProp.strName.c_str()), iIdx);
	}
	
	SetBrushSize(dGET_WORLD_PARAM()->NumBrushRadiusInTile);
	SetClipDistance(GetSceneManager()->GetPlant()->GetClipDistance());
	SelectPlantSet(m_PlantSetList.GetItemData(0));

	UpdateData(FALSE);
}

void CPalettePlantDlg::OnLbnSelchangePlantSetListbox()
{
	SelectPlantSet(m_PlantSetList.GetItemData(m_PlantSetList.GetCurSel()));
}

void CPalettePlantDlg::SelectPlantSet(RwInt32 iIdx)
{
	if (iIdx == -1)
	{
		return;
	}

	CNtlPLPlantProperty* pProperty = static_cast<CNtlPLPlantProperty*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_PLANT, NTL_PLEN_PLANT));
	if (pProperty)
	{
		CNtlPLPlantProperty::PROPERTY_MAP&	mapProperty	= pProperty->m_mapProperty;
		CNtlPLPlantProperty::PROPERTY_MAP_IT it = mapProperty.find(iIdx);
		if (it != mapProperty.end())
		{
			CNtlPLPlantProperty::sPLANT_PROP& sProp = it->second;

			m_strNumMin.Format("%d", sProp.iNumMin);
			m_strNumMax.Format("%d", sProp.iNumMax);
			m_strScaleMin.Format("%.1f", sProp.fScaleMin);
 			m_strScaleMax.Format("%.1f", sProp.fScaleMax);

			if (sProp.bIsRotationTerrain)
			{
				m_strRotationTerrain = "TRUE";
			}
			else
			{
				m_strRotationTerrain = "FALSE";
			}

			m_PlantSetResList.ResetContent();
			for (int i = 0; i < (int)sProp.vecstrResourceName.size(); ++i)
			{
				m_PlantSetResList.AddString(sProp.vecstrResourceName.at(i).c_str());
			}
			m_PlantSetList.SetCurSel(iIdx);
		}
	}
	UpdateData(FALSE);
}

void CPalettePlantDlg::OnBnClickedRefresh()
{
	CNtlXMLDoc doc;
	doc.Create();
	if(doc.Load(".\\script\\PlantProperty.xml") == false)
	{
		::AfxMessageBox("plant property load failed");
		return;
	}

	char				chBuffer[NTL_MAX_DIR_PATH]	= {0,};
	IXMLDOMNodeList*	pList						= doc.SelectNodeList((char*)"/plant_property/set_element");

	long lNum; 
	pList->get_length(&lNum);

	if (pList)
	{
		pList->Release();
		pList = NULL;
	}

	doc.Destroy();

	CNtlPLPlantProperty* pPlantProperty = static_cast<CNtlPLPlantProperty*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_PLANT, NTL_PLEN_PLANT));

	if (!pPlantProperty)
	{
		::AfxMessageBox("plant property invalid");
		return;
	}
	if (lNum != pPlantProperty->m_mapProperty.size())
	{
		::AfxMessageBox("plant property, set size is not same");
		return;
	}

	for (RwInt32 i = 0; i < 36; ++i)
	{
		if(dGETMGR()->IsFieldLoaded(dGETMGR()->m_Fields6x6[1][i]))
		{
			dGETMGR()->DeleteFieldInMemory(dGETMGR()->m_Fields6x6[1][i], FALSE);
		}
	}

	CNtlPLPropertyContainer::GetInstance()->ReloadPlant();
	GetSceneManager()->GetPlant()->SetProperty(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_PLANT, NTL_PLEN_PLANT));

	for (RwInt32 i = 0; i < 36; ++i)
	{
		dGETMGR()->CreateFields(dGETMGR()->m_Fields6x6[1][i]);
	}
	InitializeWorldData();
}

void CPalettePlantDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateData(TRUE);

	SetBrushSize(static_cast<RwReal>(m_BrushSizeSlider.GetPos()));
	SetClipDistance(static_cast<RwReal>(m_ClipDistanceSlider.GetPos()));
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CPalettePlantDlg::SetBrushSize(RwReal fSize)
{
	CLAMP(fSize, 1.0f, 100.0f);
	dGET_WORLD_PARAM()->NumBrushRadiusInTile = fSize;
	m_BrushSizeSlider.SetPos(static_cast<int>(fSize));
	m_strBrushSize.Format("%.1f", fSize);

	UpdateData(FALSE);
}

void CPalettePlantDlg::SetClipDistance(RwReal fClipDistance)
{
	CLAMP(fClipDistance, 0.0f, 512.0f);
	m_ClipDistanceSlider.SetPos(static_cast<int>(fClipDistance));
	m_strClipDistance.Format("%.1f", fClipDistance);
	GetSceneManager()->GetPlant()->SetClipDistance(fClipDistance);
	UpdateData(FALSE);
}

void CPalettePlantDlg::OnWork(RwV3d vCenter, RwReal fRadius, RwBool bIsCreate)
{
	CNtlWorldFieldManager*		pMgr	= dGETMGR();
	CNtlWorldField*				pFields	= const_cast<CNtlWorldField*>(pMgr->GetFields());
	std::vector<RwInt32>		vecNeighborFields;

	pMgr->GetNeighborFields(pMgr->GetFieldIdx(vCenter), vecNeighborFields);
	vecNeighborFields.push_back(pMgr->GetFieldIdx(vCenter));

	RwReal		fWorldSectorTileSizeHalf = (RwReal)(dGET_WORLD_PARAM()->WorldSectorTileSize / 2);
	RwSphere	sphere;	sphere.radius = fWorldSectorTileSizeHalf; sphere.center.y = 0.0f;

	for (int i = 0; i < (int)g_vecVisibleSectors.size(); ++i)
	{
		RwInt32				iTileIdx = 0;
		RwV3d				vSPos, vEPos;
		CNtlWorldSector*	pSector = &pMgr->m_pSectors[g_vecVisibleSectors.at(i)];

// 		if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
// 		{
// 			vSPos.x = pSector->m_pWorldSector->boundingBox.inf.x;
// 			vSPos.z = pSector->m_pWorldSector->boundingBox.inf.z;
// 			vEPos.x = pSector->m_pWorldSector->boundingBox.sup.x;
// 			vEPos.z = pSector->m_pWorldSector->boundingBox.sup.z;
// 		}
// 		else
// 		{
			RwReal fSectorHalfSize = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize / 2);
			
			vSPos.x = pSector->DatumPoint.x - fSectorHalfSize;
			vSPos.z = pSector->DatumPoint.z - fSectorHalfSize;
			vEPos.x = pSector->DatumPoint.x + fSectorHalfSize;
			vEPos.z = pSector->DatumPoint.z + fSectorHalfSize;
//		}

		for (int z = (RwInt32)(vSPos.z + fWorldSectorTileSizeHalf); z < (RwInt32)vEPos.z; z += dGET_WORLD_PARAM()->WorldSectorTileSize)
		{
			sphere.center.z = (RwReal)z;
			for (int x = (RwInt32)(vSPos.x + fWorldSectorTileSizeHalf); x < (RwInt32)vEPos.x; x += dGET_WORLD_PARAM()->WorldSectorTileSize)
			{
				sphere.center.x = (RwReal)x;

				if ((sphere.center.x - vCenter.x) * (sphere.center.x - vCenter.x) +	(sphere.center.z - vCenter.z) * (sphere.center.z - vCenter.z) <=
					(dGET_WORLD_PARAM()->NumBrushRadiusInTile + sphere.radius) * (dGET_WORLD_PARAM()->NumBrushRadiusInTile + sphere.radius))
				{
					if (bIsCreate)
					{
						GetSceneManager()->GetPlant()->OnCreate(pSector, iTileIdx, m_PlantSetList.GetItemData(m_PlantSetList.GetCurSel()));
					}
					else
					{
						GetSceneManager()->GetPlant()->OnDelete(pSector, iTileIdx);						
					}
				}

				++iTileIdx;
			}
		}
	}
	// Plant는 Update Tick을 가지고 있기 때문에 지워진 객체에 대한 정보 갱신을 위하여 여기서 수동적으로 Update를 해준다.
	if (bIsCreate)
	{
		for (int i = 0; i < (int)g_vecVisibleSectors.size(); ++i)
		{
			GetSceneManager()->GetPlant()->OnObjectCount(&pMgr->m_pSectors[g_vecVisibleSectors.at(i)], dPLANT_MAX_IN_SECTOR);
		}		
	}
	GetSceneManager()->GetPlant()->Update(999.0f);
}