// NtlWEDoc.cpp : CNtlWEDoc 클래스의 구현
//
#include "stdafx.h"
#include "NtlWE.h"
#include "NtlWEDoc.h"
#include "NtlWEFrm.h"
#include "NtlWorldFieldManager.h"
#include "FieldUIManager.h"
#include "MergeUIManager.h"
#include "NtlPLWorldEntity.h"
#include "NtlWorldBrushTile.h"
#include "NtlWorldBrush.h"
#include "NtlWorldPropNMapController.h"
#include "PaletteNMProp.h"
#include "ProgressWnd.h"
#include "PaletteFieldUI.h"
#include "NtlWorldMergeManager.h"
#include "ProgressWnd.h"
#include "VisibilityDistPropDlg.h"
#include "NtlWeControlUi.h"
#include "CameraAttrDlg.h"
#include "MiniMapPropDlg.h"
#include "FieldNamePropDlg.h"
#include "FieldsInformation.h"
#include "BloomPropDlg.h"
#include "NtlWorldSectorPVS.h"
#include "MaterialPropDlg.h"
#include "FieldSearchDlg.h"
#include "WorldViewDlg.h"
#include "SpawnMergeDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNtlWEDoc

IMPLEMENT_DYNCREATE(CNtlWEDoc, CDocument)

BEGIN_MESSAGE_MAP(CNtlWEDoc, CDocument)
	ON_COMMAND(ID_FILE_SAVE, &CNtlWEDoc::OnFileSave)
	ON_COMMAND(ID_PORTAL, &CNtlWEDoc::OnPortal)
	ON_UPDATE_COMMAND_UI(ID_PORTAL, &CNtlWEDoc::OnUpdatePortal)
	ON_COMMAND(ID_MERGE, &CNtlWEDoc::OnMerge)
	ON_UPDATE_COMMAND_UI(ID_MERGE, &CNtlWEDoc::OnUpdateMerge)
	ON_COMMAND(ID_FILE_VERIFICATION, &CNtlWEDoc::OnFileVerification)
	ON_COMMAND(ID_ATTR_FOG, &CNtlWEDoc::OnAttrFog)
	ON_UPDATE_COMMAND_UI(ID_ATTR_FOG, &CNtlWEDoc::OnUpdateAttrFog)
	ON_COMMAND(ID_ATTR_SKY, &CNtlWEDoc::OnAttrSky)
	ON_UPDATE_COMMAND_UI(ID_ATTR_SKY, &CNtlWEDoc::OnUpdateAttrSky)
	ON_COMMAND(ID_ATTR_LIGHT, &CNtlWEDoc::OnAttrLight)
	ON_UPDATE_COMMAND_UI(ID_ATTR_LIGHT, &CNtlWEDoc::OnUpdateAttrLight)
	ON_COMMAND(ID_MENU_SLOPELIGHTING, &CNtlWEDoc::OnMenuSlopelighting)
	ON_UPDATE_COMMAND_UI(ID_MENU_SLOPELIGHTING, &CNtlWEDoc::OnUpdateMenuSlopelighting)
	ON_COMMAND(ID_PROP_CREATE, &CNtlWEDoc::OnPropCreate)
	ON_COMMAND(ID_PROP_DELETE, &CNtlWEDoc::OnPropDelete)
//	ON_COMMAND(ID_FIELD_BGM, &CNtlWEDoc::OnFieldBgm)
ON_COMMAND(ID_FIELD_BGM, &CNtlWEDoc::OnFieldBgm)
ON_COMMAND(ID_HEIGHTFIELDUPDATE, &CNtlWEDoc::OnHeightfieldupdate)
ON_UPDATE_COMMAND_UI(ID_HEIGHTFIELDUPDATE, &CNtlWEDoc::OnUpdateHeightfieldupdate)
ON_COMMAND(ID_VISIBILITY_DISTANCE, &CNtlWEDoc::OnVisibilityDistance)
ON_COMMAND(ID_CREATEAMINIMAP, &CNtlWEDoc::OnCreateaminimap)
ON_COMMAND(ID_FIELD_FIELDNAME, &CNtlWEDoc::OnFieldFieldname)
ON_COMMAND(ID_INFORMATION, &CNtlWEDoc::OnInformation)
ON_COMMAND(ID_FIELD_MONOPOWER, &CNtlWEDoc::OnFieldMonopower)
ON_UPDATE_COMMAND_UI(ID_FIELD_MONOPOWER, &CNtlWEDoc::OnUpdateFieldMonopower)
ON_COMMAND(ID_CREATE_TRANSPARENT_TILES, &CNtlWEDoc::OnCreateTransparentTiles)
ON_COMMAND(ID_DELETE_TRANSPARENT_TILES, &CNtlWEDoc::OnDeleteTransparentTiles)
//ON_COMMAND(ID_CREATE_PVS, &CNtlWEDoc::OnCreatePvs)
//ON_COMMAND(ID_DESTROY_PVS, &CNtlWEDoc::OnDestroyPvs)
ON_COMMAND(ID_MATERIAL_PROP_UPDATE, &CNtlWEDoc::OnMaterialPropUpdate)
ON_COMMAND(ID_FIELDCOLOR, &CNtlWEDoc::OnFieldcolor)
ON_COMMAND(ID_FIELDSEARCH, &CNtlWEDoc::OnFieldsearch)
ON_COMMAND(ID_WEATHER, &CNtlWEDoc::OnWeather)
ON_COMMAND(ID_TEMP_FIELDHEATHAZE, &CNtlWEDoc::OnFieldheathaze)
ON_COMMAND(ID_BLOCK_PORTAL, &CNtlWEDoc::OnBlockPortal)
ON_COMMAND(ID_SECTOR_PORTAL, &CNtlWEDoc::OnSectorPortal)
ON_COMMAND(ID_FIELD_PORTAL, &CNtlWEDoc::OnFieldPortal)
ON_COMMAND(ID_BLOCK_GAMEPROPERTY, &CNtlWEDoc::OnBlockGameProperty)
ON_COMMAND(ID_BLOCK_SKYFLOATING, &CNtlWEDoc::OnBlockSkyfloating)
ON_COMMAND(ID_PROPERTY_CLIPBOARD_COPY, &CNtlWEDoc::OnPropertyClipboardCopy)
ON_COMMAND(ID_PROPERTY_CLIPBOARD_PASTE, &CNtlWEDoc::OnPropertyClipboardPaste)
ON_COMMAND(ID_TEMP_PROOFSINVALIDSECTORLINK, &CNtlWEDoc::OnTempProofsinvalidsectorlink)
END_MESSAGE_MAP()


// CNtlWEDoc 생성/소멸

CNtlWEDoc::CNtlWEDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.
	m_pNtlFieldProp				= NULL;
	m_pNtlBlockProp				= NULL;
	m_pNtlFieldPropClipboard	= NULL;
	m_uiFieldPropMode			= EFIELDPROP_MODE_NONE;
}

CNtlWEDoc::~CNtlWEDoc()
{
	NTL_DELETE(m_pNtlFieldPropClipboard);
	NTL_DELETE(m_pNtlFieldProp);
	NTL_DELETE(m_pNtlBlockProp);
}

BOOL CNtlWEDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 다시 초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}

// CNtlWEDoc serialization

void CNtlWEDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CNtlWEDoc 진단

#ifdef _DEBUG
void CNtlWEDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNtlWEDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CNtlWEDoc 명령

void CNtlWEDoc::OnFileSave()
{
	if(!((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.IsWorking())
	{
		::AfxMessageBox("World Data wasn't loaded.", MB_OK);
		return;
	}

#ifdef dNTL_WORLD_TOOL_MODE

	CNtlWorldFieldManager* pMgr = dGETMGR();
	pMgr->SaveCurFields();

#else

	::AfxMessageBox("Game Mode's not activated.", MB_OK);
	return;

#endif
}

void CNtlWEDoc::CreateWorldData()
{
	NTL_DELETE(m_pNtlFieldProp);
	NTL_DELETE(m_pNtlBlockProp);
	NTL_DELETE(m_pNtlFieldPropClipboard);

	m_pNtlFieldProp				= NTL_NEW sNTL_FIELD_PROP;
	m_pNtlBlockProp				= NTL_NEW sNTL_BLOCK_PROP;
	m_pNtlFieldPropClipboard	= NTL_NEW sNTL_FIELD_PROP;
}

void CNtlWEDoc::OnPortal()
{
	RwV3d CurPortalFieldPT = dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
	RwV3d CurPortalWorldPT = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurPortalFieldPT);

	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurPortalWorldPT);
	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlPLWorldEntity()->SetPortalPosition(CurPortalWorldPT);

	// force the camera to be free
	CNtlWeControlUi::GetInstance().SetActiveWeController(dGETAPP().GetNtlGameCamera());
	dGETFRM()->m_pCameraPortal->SetCurCamMode(0);
}

void CNtlWEDoc::OnUpdatePortal(CCmdUI *pCmdUI)
{
	if(dGETHDL()->m_pFieldUIManager->IsSingleSelected())
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);
}

void CNtlWEDoc::OnMerge()
{
	if(dGETFRM()->m_pFieldUI->GetWorldMergeStateNothing() && dGETFRM()->m_pFieldUI->GetSpawnMergeStateNothing())
	{
		AfxMessageBox("Please choose at least one of merge menu", MB_OK);
		return;
	}

	if (!dGETFRM()->m_pFieldUI->GetWorldMergeStateNothing() && !dGETFRM()->m_pFieldUI->m_MergeWorldPathName.Compare(_T("null")))
	{
		AfxMessageBox("Please link another world to be used for merging", MB_OK);
		return;
	}

	if (!dGETFRM()->m_pFieldUI->GetSpawnMergeStateNothing() && (!dGETFRM()->m_pFieldUI->m_MergeSpawnSrcName.Compare(_T("null")) || !dGETFRM()->m_pFieldUI->m_MergeSpawnDstName.Compare(_T("null"))))
	{
		AfxMessageBox("Please spawn folder link.", MB_OK);
		return;
	}

	if(::AfxMessageBox("Are you sure?", MB_YESNO) == IDYES)
	{
		RwBool					IsAField	= dGETHDL()->m_pMergeUIManager->IsSingleSelected();
		CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

		// progress bar
		RwInt32			NumCurFields2Merge			= dGETHDL()->m_pMergeUIManager->GetCurDragFieldsNum();
		RwInt32			CntFields					= 0;

		// spawn merge
		if (!dGETFRM()->m_pFieldUI->GetSpawnMergeStateNothing())
		{
			CSpawnMergeDlg SpawnMergeDlg;
			SpawnMergeDlg.DoModal();
		}

		if (!dGETFRM()->m_pFieldUI->GetWorldMergeStateNothing())
		{
			if(IsAField)
			{			
				// same counts of the fields
				if(!dGETHDL()->m_pFieldUIManager->IsSingleSelected())
				{
					AfxMessageBox("The number of each dragging fields are not same", MB_OK);
					return;
				}

				// current world
				RwV3d CurFieldMidPosInUI4CurWorld		= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
				RwV3d CurFieldMidPosInWorld4CurWorld	= dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI4CurWorld);
				((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld4CurWorld, FALSE);

				// merge world
				RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pMergeUIManager->GetFieldMidPos();
				RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pMergeUIManager->GetWorldPT(CurFieldMidPosInUI);
				((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);

				if(dGETHDL()->m_pMergeUIManager->UpdateMerge(CurFieldMidPosInWorld, CurFieldMidPosInWorld, CurFieldMidPosInWorld4CurWorld, CurFieldMidPosInWorld4CurWorld))
				{
					dGETHDL()->m_pMergeUIManager->InsertMergedArea(CurFieldMidPosInWorld);
				}
				else
				{
					AfxMessageBox("Something wrong with merge state, please call wooteck", MB_OK);
					return;
				}
			}
			else
			{
				// same counts of the fields
				if(dGETHDL()->m_pFieldUIManager->IsSingleSelected())
				{
					AfxMessageBox("The number of each dragging fields are not same", MB_OK);
					return;
				}

				RECT	RTInUI4CurWorld		= dGETHDL()->m_pFieldUIManager->GetFieldDragInfo();
				RECT	RTInUI				= dGETHDL()->m_pMergeUIManager->GetFieldDragInfo();
				RwInt32 GridSize			= dGETHDL()->m_pFieldUIManager->GetFieldGridSize();

				RwInt32 WidthGridNum			= abs(RTInUI.left - RTInUI.right) / GridSize;
				RwInt32 HeightColGridNum		= abs(RTInUI.top - RTInUI.bottom) / GridSize;
				RwInt32 WidthGridNum4World		= abs(RTInUI4CurWorld.left - RTInUI4CurWorld.right) / GridSize;
				RwInt32 HeightColGridNum4World	= abs(RTInUI4CurWorld.top - RTInUI4CurWorld.bottom) / GridSize;

				if(WidthGridNum != WidthGridNum4World || HeightColGridNum != HeightColGridNum4World)
				{
					AfxMessageBox("The number of each dragging fields are not same", MB_OK);
					return;
				}

				// current world
				RwV3d STmp4CurWorld, ETmp4CurWorld;
				STmp4CurWorld.x = static_cast<RwReal>(RTInUI4CurWorld.left);
				STmp4CurWorld.z = static_cast<RwReal>(RTInUI4CurWorld.top);
				ETmp4CurWorld.x = static_cast<RwReal>(RTInUI4CurWorld.right);
				ETmp4CurWorld.z = static_cast<RwReal>(RTInUI4CurWorld.bottom);
				RwV3d SPos4CurWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(STmp4CurWorld);
				RwV3d EPos4CurWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(ETmp4CurWorld);
				((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(SPos4CurWorld, FALSE);
				((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(EPos4CurWorld, FALSE);

				RwV3d vPosTmp4CurWorld[2];
				vPosTmp4CurWorld[0].x = EPos4CurWorld.x + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
				vPosTmp4CurWorld[0].z = EPos4CurWorld.z + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
				vPosTmp4CurWorld[1].x = SPos4CurWorld.x - dGET_WORLD_PARAM()->WorldFieldSize + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
				vPosTmp4CurWorld[1].z = SPos4CurWorld.z - dGET_WORLD_PARAM()->WorldFieldSize + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);

				// merge world
				RwV3d STmp, ETmp;
				STmp.x = static_cast<RwReal>(RTInUI.left - 512);
				STmp.z = static_cast<RwReal>(RTInUI.top);
				ETmp.x = static_cast<RwReal>(RTInUI.right - 512);
				ETmp.z = static_cast<RwReal>(RTInUI.bottom);
				RwV3d SPos = dGETHDL()->m_pMergeUIManager->GetWorldPT(STmp);
				RwV3d EPos = dGETHDL()->m_pMergeUIManager->GetWorldPT(ETmp);
				((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(SPos, FALSE);
				((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(EPos, FALSE);

				RwV3d vPosTmp[2];
				vPosTmp[0].x = EPos.x + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
				vPosTmp[0].z = EPos.z + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
				vPosTmp[1].x = SPos.x - dGET_WORLD_PARAM()->WorldFieldSize + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
				vPosTmp[1].z = SPos.z - dGET_WORLD_PARAM()->WorldFieldSize + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);

				if(dGETHDL()->m_pMergeUIManager->UpdateMerge(vPosTmp[0], vPosTmp[1], vPosTmp4CurWorld[0], vPosTmp4CurWorld[1], FALSE))
				{
					for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
					{
						for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
						{
							RwV3d CurPos;
							CurPos.x = j + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
							CurPos.z = i + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
							dGETHDL()->m_pMergeUIManager->InsertMergedArea(CurPos);
						}
					}
				}
				else
				{
					AfxMessageBox("Something wrong with merge state, please call wooteck", MB_OK);
					return;
				}
			}
		}
	}
}	

void CNtlWEDoc::OnUpdateMerge(CCmdUI *pCmdUI)
{
	if(dGETHDL()->m_pMergeUIManager->IsMergeEnable())
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);
}

void CNtlWEDoc::OnFileVerification()
{
	ITEMIDLIST *pidlBrowse;
	char pszPathname[NTL_MAX_DIR_PATH];

	// set dir. info
	BROWSEINFO BrInfo;
	BrInfo.hwndOwner = ((CNtlWEApp*)(AfxGetApp()))->m_pFrm->GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = "Select a project folder to verify data.";
	BrInfo.ulFlags = BIF_NONEWFOLDERBUTTON;
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	CString strResult = "";
	char CurDir[NTL_MAX_DIR_PATH] = {0,};
	if(pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);

		// check this folder'got already ntl files
		GetCurrentDirectory(NTL_MAX_DIR_PATH, CurDir);
		SetCurrentDirectory(pszPathname);
		if((_access("#######.gwp", 0)) == -1)
		{
			::AfxMessageBox("this folder hasn't got ntl world files, please choose another one.", MB_OK);
			return;
		}
		SetCurrentDirectory(CurDir);
	}
	else
		return;

	char CurPathBk[NTL_MAX_DIR_PATH] = {0,};
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, CurPathBk);
	::SetCurrentDirectory(pszPathname);

	// change dir.
	_chdir("fields");

	::GetCurrentDirectory(NTL_MAX_DIR_PATH, CurDir);

	CFileFind finder;
	BOOL bWorking = finder.FindFile("*.*");
	while(bWorking)
	{
		bWorking = finder.FindNextFile();

		if(finder.IsDots())
			continue;

		if(finder.IsDirectory())
		{
			char		szOldDir[NTL_MAX_DIR_PATH];
			CFileFind	ff;

			::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldDir);
			::_chdir(finder.GetFileName().GetBuffer(0));
			
			BOOL bff = ff.FindFile("*.*");

			int NoFileFlag = 0;
			while(bff)
			{
				bff = ff.FindNextFile();

				if(ff.IsDots())
				{
					++NoFileFlag;

					// 'cos of ".", ".."
					if(NoFileFlag == 2 && !bff)
					{
						CString Msg;
						Msg.Format("There's no %s file", finder.GetFileName().GetBuffer(0));
						::AfxMessageBox(Msg, MB_OK);
					}

					continue;
				}

				// can't compare bff is NULL 'cos the bff flag is always NULL; this is 'cos the file name has't got a extension
				HANDLE hFile = NULL;
				hFile = ::CreateFile(	ff.GetFilePath().GetBuffer(0),
										GENERIC_READ,
										FILE_SHARE_READ, 
										NULL,
										OPEN_EXISTING, 
										FILE_ATTRIBUTE_NORMAL, 
										NULL);

				if(hFile != INVALID_HANDLE_VALUE)
				{
					DWORD dwFileSize = ::GetFileSize(hFile, NULL);
					if(!dwFileSize)
					{
						CString Msg;
						Msg.Format("%s file file size is zero", ff.GetFileName());
						::AfxMessageBox(Msg, MB_OK);
					}
				}

				::CloseHandle(hFile);

				break;
			}

			::SetCurrentDirectory(szOldDir);
		}
	}

	::SetCurrentDirectory(CurPathBk);

	::AfxMessageBox("Done.", MB_OK);
}

void CNtlWEDoc::OnAttrFog()
{
	if(m_FogPropDlg.DoModal() == IDOK)
	{
		/*RwBool					IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
		CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

		if(IsAField)
		{
			RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
			RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);
			pFieldMgr->SetAFieldProp(CurFieldMidPosInWorld, *m_pNtlFieldProp, eNFP_FOG);
		}
		else
		{
			RECT RTInUI = dGETHDL()->m_pFieldUIManager->GetFieldDragInfo();
			RwV3d STmp, ETmp;
			STmp.x = static_cast<RwReal>(RTInUI.left);
			STmp.z = static_cast<RwReal>(RTInUI.top);
			ETmp.x = static_cast<RwReal>(RTInUI.right);
			ETmp.z = static_cast<RwReal>(RTInUI.bottom);
			RwV3d SPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(STmp);
			RwV3d EPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(ETmp);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(SPos, FALSE);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(EPos, FALSE);
			
			for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
			{
				for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
				{
					RwV3d CurPos;
					CurPos.x = j + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					CurPos.z = i + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					pFieldMgr->SetAFieldProp(CurPos, *m_pNtlFieldProp, eNFP_FOG);
				}
			}
		}*/
		
		dGETDOC()->ApplyFieldProp(((CNtlWEApp*)(AfxGetApp()))->m_pDoc->m_pNtlFieldProp, eNFP_FOG);
		// 인도어
// 		switch (GetSceneManager()->GetActiveWorldType())
// 		{
// 		case AW_HEGITHFIELD:
// 			dGETHDL()->m_pFieldUIManager->ApplyFieldProp(m_pNtlFieldProp, eNFP_FOG);
// 			break;
// 		case AW_RWWORLD:
// 			dGETFRM()->m_pWorldViewDlg->ApplyFieldProp(m_pNtlFieldProp, eNFP_FOG);
// 			break;
// 		}
	}
}

void CNtlWEDoc::OnUpdateAttrFog(CCmdUI *pCmdUI)
{
// 	if(dGETHDL()->m_pFieldUIManager->IsSelected())
// 		pCmdUI->Enable();
// 	else
// 		pCmdUI->Enable(FALSE);
}

void CNtlWEDoc::OnAttrSky()
{
	if(m_SkyPropDlg.DoModal() == IDOK)
	{
		/*RwBool					IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
		CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

		if(IsAField)
		{
			RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
			RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);
			pFieldMgr->SetAFieldProp(CurFieldMidPosInWorld, *m_pNtlFieldProp, eNFP_SKY);
		}
		else
		{
			RECT RTInUI = dGETHDL()->m_pFieldUIManager->GetFieldDragInfo();
			RwV3d STmp, ETmp;
			STmp.x = static_cast<RwReal>(RTInUI.left);
			STmp.z = static_cast<RwReal>(RTInUI.top);
			ETmp.x = static_cast<RwReal>(RTInUI.right);
			ETmp.z = static_cast<RwReal>(RTInUI.bottom);
			RwV3d SPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(STmp);
			RwV3d EPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(ETmp);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(SPos, FALSE);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(EPos, FALSE);

			for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
			{
				for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
				{
					RwV3d CurPos;
					CurPos.x = j + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					CurPos.z = i + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					pFieldMgr->SetAFieldProp(CurPos, *m_pNtlFieldProp, eNFP_SKY);
				}
			}
		}*/

		dGETDOC()->ApplyFieldProp(((CNtlWEApp*)(AfxGetApp()))->m_pDoc->m_pNtlFieldProp, eNFP_SKY);

		// 인도어
// 		switch (GetSceneManager()->GetActiveWorldType())
// 		{
// 		case AW_HEGITHFIELD:
// 			dGETHDL()->m_pFieldUIManager->ApplyFieldProp(m_pNtlFieldProp, eNFP_SKY);
// 			break;
// 		case AW_RWWORLD:
// 			dGETFRM()->m_pWorldViewDlg->ApplyFieldProp(m_pNtlFieldProp, eNFP_SKY);
// 			break;
// 		}		
	}
}

void CNtlWEDoc::OnUpdateAttrSky(CCmdUI *pCmdUI)
{
// 	if(dGETHDL()->m_pFieldUIManager->IsSelected())
// 		pCmdUI->Enable();
// 	else
// 		pCmdUI->Enable(FALSE);
}

void CNtlWEDoc::OnAttrLight()
{
	if(m_LightPropDlg.DoModal() == IDOK)
	{
		/*RwBool					IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
		CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

		if(IsAField)
		{
			RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
			RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);
			pFieldMgr->SetAFieldProp(CurFieldMidPosInWorld, *m_pNtlFieldProp, eNFP_LIGHT);
		}
		else
		{
			RECT RTInUI = dGETHDL()->m_pFieldUIManager->GetFieldDragInfo();
			RwV3d STmp, ETmp;
			STmp.x = static_cast<RwReal>(RTInUI.left);
			STmp.z = static_cast<RwReal>(RTInUI.top);
			ETmp.x = static_cast<RwReal>(RTInUI.right);
			ETmp.z = static_cast<RwReal>(RTInUI.bottom);
			RwV3d SPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(STmp);
			RwV3d EPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(ETmp);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(SPos, FALSE);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(EPos, FALSE);

			for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
			{
				for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
				{
					RwV3d CurPos;
					CurPos.x = j + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					CurPos.z = i + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					pFieldMgr->SetAFieldProp(CurPos, *m_pNtlFieldProp, eNFP_LIGHT);
				}
			}
		}*/

		dGETDOC()->ApplyFieldProp(((CNtlWEApp*)(AfxGetApp()))->m_pDoc->m_pNtlFieldProp, eNFP_LIGHT);

		// 인도어
// 		switch (GetSceneManager()->GetActiveWorldType())
// 		{
// 		case AW_HEGITHFIELD:
// 			dGETHDL()->m_pFieldUIManager->ApplyFieldProp(m_pNtlFieldProp, eNFP_LIGHT);
// 			break;
// 		case AW_RWWORLD:
// 			dGETFRM()->m_pWorldViewDlg->ApplyFieldProp(m_pNtlFieldProp, eNFP_LIGHT);
// 			break;
// 		}
	}
}

void CNtlWEDoc::OnUpdateAttrLight(CCmdUI *pCmdUI)
{
	if(dGETHDL()->m_pFieldUIManager->IsSelected() || !dGETFRM()->m_pWorldViewDlg->IsSelectEmpty() || dGETHDL()->GetCapturedDoodadsNum())
		pCmdUI->Enable();
	else
		pCmdUI->Enable(FALSE);
}

void CNtlWEDoc::SetSLData(RwV3d& Pos)
{
	CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	RwInt32					Idx			= pFieldMgr->GetFieldIdx(Pos);
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());

	if(Idx == -1)
	{
		DBO_TRACE(FALSE, "CNtlWEDoc::SetSLData, invalid field index.");
		return;
	}
	else
	{
		int l, m;
		int SectorIdx;
		RwV3d SPos = pFields[Idx].GetSPos();
		RwV3d SectorSPos;

		if(!pFieldMgr->IsFieldLoaded(Idx))
		{
			for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
				{
					SectorSPos.x = (RwReal)m;
					SectorSPos.z = (RwReal)l;
					SectorIdx = pFieldMgr->CNtlWorldSectorManager::GetSectorIdx(SectorSPos);
					
					_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
					_chdir("swap");
					_chdir("sl");

					FILE *pFile = NULL;
					::sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.sl", SectorIdx);
					if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "wb") != 0)
					{
						DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
						::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
						return;
					}
					else
					{
						::fwrite(&m_NtlWorldSL, sizeof(sNTL_WORLD_SL), 1, pFile);
						::fclose(pFile);

						pFieldMgr->OnSetSlopeLighting(SectorIdx);
					}
				}
			}
		}
		else
		{
			for(l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				for(m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
				{
					SectorSPos.x = (RwReal)m;
					SectorSPos.z = (RwReal)l;
					SectorIdx = pFieldMgr->CNtlWorldSectorManager::GetSectorIdx(SectorSPos);

					// there is no deep copy
					::CopyMemory(pFieldMgr->m_pSectors[SectorIdx].m_pNtlWorldSL, &m_NtlWorldSL, sizeof(sNTL_WORLD_SL));

					pFieldMgr->OnSetSlopeLighting(SectorIdx);
				}
			}
		}

		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	}
}

void CNtlWEDoc::OnMenuSlopelighting()
{
	if(m_SLPropDlg.DoModal() == IDOK)
	{
		RwBool					IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
		CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
		RwChar					Text[40];
		RwInt32					SLFieldNum	= 0;
		RwInt32					Counter		= 0;
		CProgressWnd			WndProgress((CNtlWEFrm*)AfxGetMainWnd(), "Slope lighting");

		if(IsAField)
		{
			RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
			RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);

			WndProgress.GoModal();
			WndProgress.SetRange(0, 1);

			::sprintf_s(Text, 40, "1 / 1 field is now calculating...");
			WndProgress.SetText(Text);
			WndProgress.StepIt();
			WndProgress.PeekAndPump();

			SetSLData(CurFieldMidPosInWorld);
			
			WndProgress.SetText("Done.");
			Sleep(500);
		}
		else
		{
			RECT RTInUI = dGETHDL()->m_pFieldUIManager->GetFieldDragInfo();
			RwV3d STmp, ETmp;
			STmp.x = static_cast<RwReal>(RTInUI.left);
			STmp.z = static_cast<RwReal>(RTInUI.top);
			ETmp.x = static_cast<RwReal>(RTInUI.right);
			ETmp.z = static_cast<RwReal>(RTInUI.bottom);
			RwV3d SPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(STmp);
			RwV3d EPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(ETmp);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(SPos, FALSE);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(EPos, FALSE);

			// count how many fields
			for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
			{
				for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
				{
					++SLFieldNum;
				}
			}

			WndProgress.GoModal();
			WndProgress.SetRange(0, SLFieldNum);

			for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
			{
				for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
				{
					::sprintf_s(Text, 40, "%d / %d field is now calculating...", Counter++, SLFieldNum);
					WndProgress.SetText(Text);

					RwV3d CurPos;
					CurPos.x = j + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					CurPos.z = i + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					SetSLData(CurPos);

					WndProgress.StepIt();
					WndProgress.PeekAndPump();
				}
			}

			WndProgress.SetText("Done.");
			Sleep(500);
		}
	}
}

void CNtlWEDoc::OnUpdateMenuSlopelighting(CCmdUI *pCmdUI)
{
// 	if(dGETHDL()->m_pFieldUIManager->IsSelected())
// 		pCmdUI->Enable();
// 	else
// 		pCmdUI->Enable(FALSE);
}

void CNtlWEDoc::OnPropCreate()
{
	CNtlWorldBrushTile<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWorldBrushTile<CNtlWorldFieldManager>*>(dGETHDL()->m_pBrushSelector->GetController());
	vector<RwInt32> vecTileContainer = pCurBrush->GetTileContainer();
	
	RwInt32 CntX;
	RwInt32 CntZ;
	RwInt32 NumWorldTile	= dGET_WORLD_PARAM()->WorldSize / dGET_WORLD_PARAM()->WorldSectorTileSize;
	RwInt32 LenTile			= dGET_WORLD_PARAM()->WorldSectorTileSize;
	RwV3d	PosTile;
	DWORD	CurProp;
	
	for(RwUInt32 i = 0; i < vecTileContainer.size(); ++i)
	{
		CntX		= vecTileContainer[i] - ((vecTileContainer[i] / NumWorldTile) * NumWorldTile);
		CntZ		= vecTileContainer[i] / NumWorldTile;
		PosTile.x	= static_cast<RwReal>(CntX * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf);
		PosTile.z	= static_cast<RwReal>(CntZ * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf);
	
		CurProp = dGETFRM()->m_pNMPropPalette->GetCurProp();
	
		dGETFRM()->m_pNMPropPalette->m_pController->AppendPropDat(PosTile, CurProp);
	}

	dGETHDL()->UpdateNMPropTile();
}

void CNtlWEDoc::OnPropDelete()
{
	if(AfxMessageBox("Are you sure?", MB_YESNO) == IDNO)
	{
		return;
	}

	CNtlWorldBrushTile<CNtlWorldFieldManager>* pCurBrush = reinterpret_cast<CNtlWorldBrushTile<CNtlWorldFieldManager>*>(dGETHDL()->m_pBrushSelector->GetController());
	vector<RwInt32> vecTileContainer = pCurBrush->GetTileContainer();

	RwInt32 CntX;
	RwInt32 CntZ;
	RwInt32 NumWorldTile	= dGET_WORLD_PARAM()->WorldSize / dGET_WORLD_PARAM()->WorldSectorTileSize;
	RwInt32 LenTile			= dGET_WORLD_PARAM()->WorldSectorTileSize;
	RwV3d	PosTile;
	DWORD	CurProp;

	for(RwUInt32 i = 0; i < vecTileContainer.size(); ++i)
	{
		CntX		= vecTileContainer[i] - ((vecTileContainer[i] / NumWorldTile) * NumWorldTile);
		CntZ		= vecTileContainer[i] / NumWorldTile;
		PosTile.x	= static_cast<RwReal>(CntX * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf);
		PosTile.z	= static_cast<RwReal>(CntZ * LenTile - dGET_WORLD_PARAM()->WorldSizeHalf);

		CurProp = dGETFRM()->m_pNMPropPalette->GetCurProp();

		dGETFRM()->m_pNMPropPalette->m_pController->DeletePropDat(PosTile, CurProp);
	}

	dGETHDL()->UpdateNMPropTile();
}

void CNtlWEDoc::OnFieldBgm()
{
	if(m_FieldSoundProp.DoModal() == IDOK)
	{
		/*RwBool					IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
		CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

		if(IsAField)
		{
			RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
			RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);
			pFieldMgr->SetAFieldProp(CurFieldMidPosInWorld, *m_pNtlFieldProp, eNFP_SOUND);
		}
		else
		{
			RECT RTInUI = dGETHDL()->m_pFieldUIManager->GetFieldDragInfo();
			RwV3d STmp, ETmp;
			STmp.x = static_cast<RwReal>(RTInUI.left);
			STmp.z = static_cast<RwReal>(RTInUI.top);
			ETmp.x = static_cast<RwReal>(RTInUI.right);
			ETmp.z = static_cast<RwReal>(RTInUI.bottom);
			RwV3d SPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(STmp);
			RwV3d EPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(ETmp);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(SPos, FALSE);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(EPos, FALSE);

			for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
			{
				for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
				{
					RwV3d CurPos;
					CurPos.x = j + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					CurPos.z = i + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					pFieldMgr->SetAFieldProp(CurPos, *m_pNtlFieldProp, eNFP_SOUND);
				}
			}
		}*/

		dGETDOC()->ApplyFieldProp(((CNtlWEApp*)(AfxGetApp()))->m_pDoc->m_pNtlFieldProp, eNFP_SOUND);

		// 인도어
// 		switch (GetSceneManager()->GetActiveWorldType())
// 		{
// 		case AW_HEGITHFIELD:
// 			dGETHDL()->m_pFieldUIManager->ApplyFieldProp(m_pNtlFieldProp, eNFP_SOUND);
// 			break;
// 		case AW_RWWORLD:
// 			dGETFRM()->m_pWorldViewDlg->ApplyFieldProp(m_pNtlFieldProp, eNFP_SOUND);
// 			break;
// 		}
	}
}

VOID CNtlWEDoc::SetSerialID(CNtlPLEntity* pNtlPLEntity)
{
	if(m_NtlDoodadsPropParam.EnableTrigger)
	{
		if(pNtlPLEntity->GetSID() == 0xffffffff)
		{
			pNtlPLEntity->SetSID(m_NtlDoodadsPropParam.SerialID);
			dGET_WORLD_PARAM()->WorldTriggerSerialID++;
			SaveCurWorldState();
		}
	}
	else
	{
		pNtlPLEntity->SetSID(0xffffffff);
	}
}

VOID CNtlWEDoc::SetObjNameIdx(CNtlPLEntity* pNtlPLEntity)
{
	if(m_NtlDoodadsPropParam.EnableTrigger)
	{
		if(m_NtlDoodadsPropParam.NameIdx != 0xffffffff && m_NtlDoodadsPropParam.NameIdx != 999)
		{
			pNtlPLEntity->SetObjNameIdx(m_NtlDoodadsPropParam.NameIdx);
		}
	}
	else
	{
		pNtlPLEntity->SetObjNameIdx(0xffffffff);
	}
}
void CNtlWEDoc::OnHeightfieldupdate()
{
	if(::AfxMessageBox("Are you sure?", MB_YESNO) == IDYES)
	{
		RwBool					IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
		CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
		RwInt32					IdxField;
		vector<RwInt32>			vecIdxField;

		if(IsAField)
		{
			// handle files
			RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
			RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);

			// setup
			IdxField = pFieldMgr->GetFieldIdx(CurFieldMidPosInWorld);

			// get new heightfield
			pFieldMgr->DeleteFieldThenCreateNew(IdxField);

			// update heightfield interpolation
			CNtlWorldMergeManager::GetInstance()->OnHeightfieldVariation(IdxField);

			// refresh terrain
			pFieldMgr->GetNeighborFields(IdxField, vecIdxField);

			for(RwUInt32 i = 0; i < vecIdxField.size(); ++i)
			{
				if(vecIdxField[i] != -1)
				{
					pFieldMgr->RefreshField(vecIdxField[i]);
				}
			}

			pFieldMgr->RefreshField(IdxField);

			// refresh slope lighting
			for(RwUInt32 i = 0; i < vecIdxField.size(); ++i)
			{
				if(vecIdxField[i] != -1)
				{
					pFieldMgr->OnSetSlopeLighting(static_cast<RwUInt32>(vecIdxField[i]));
				}
			}

			pFieldMgr->OnSetSlopeLighting(static_cast<RwUInt32>(IdxField));

			vecIdxField.clear();
		}
		else
		{
			// handle files
			RECT RTInUI = dGETHDL()->m_pFieldUIManager->GetFieldDragInfo();
			RwV3d STmp, ETmp;
			STmp.x = static_cast<RwReal>(RTInUI.left);
			STmp.z = static_cast<RwReal>(RTInUI.top);
			ETmp.x = static_cast<RwReal>(RTInUI.right);
			ETmp.z = static_cast<RwReal>(RTInUI.bottom);
			RwV3d SPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(STmp);
			RwV3d EPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(ETmp);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(SPos, FALSE);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(EPos, FALSE);

			for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
			{
				for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
				{
					RwV3d CurPos;
					CurPos.x = j + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					CurPos.z = i + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					pFieldMgr->DeleteFieldThenCreateNew(pFieldMgr->GetFieldIdx(CurPos));
				}
			}

			// update heightfield interpolation
			for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
			{
				for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
				{
					RwV3d CurPos;
					CurPos.x = j + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					CurPos.z = i + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					CNtlWorldMergeManager::GetInstance()->OnHeightfieldVariation(pFieldMgr->GetFieldIdx(CurPos));
				}
			}

			// refresh terrain
			for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
			{
				for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
				{
					RwV3d CurPos;
					CurPos.x = j + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					CurPos.z = i + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					IdxField = pFieldMgr->GetFieldIdx(CurPos);

					pFieldMgr->GetNeighborFields(IdxField, vecIdxField);

					for(RwUInt32 i = 0; i < vecIdxField.size(); ++i)
					{
						if(vecIdxField[i] != -1)
						{
							pFieldMgr->RefreshField(vecIdxField[i]);
						}
					}

					pFieldMgr->RefreshField(IdxField);

					vecIdxField.clear();
				}
			}

			// refresh slope lighting
			for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
			{
				for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
				{
					RwV3d CurPos;
					CurPos.x = j + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					CurPos.z = i + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					IdxField = pFieldMgr->GetFieldIdx(CurPos);

					pFieldMgr->GetNeighborFields(IdxField, vecIdxField);

					for(RwUInt32 i = 0; i < vecIdxField.size(); ++i)
					{
						if(vecIdxField[i] != -1)
						{
							pFieldMgr->OnSetSlopeLighting(static_cast<RwUInt32>(vecIdxField[i]));
						}
					}

					pFieldMgr->OnSetSlopeLighting(static_cast<RwUInt32>(IdxField));

					vecIdxField.clear();
				}
			}
		}
	}
}

void CNtlWEDoc::OnUpdateHeightfieldupdate(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
}

void CNtlWEDoc::OnUpdateFieldMonopower(CCmdUI *pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
}

void CNtlWEDoc::OnVisibilityDistance()
{
	CVisibilityDistPropDlg Dlg;
	Dlg.DoModal();
}

void CNtlWEDoc::OnCreateaminimap()
{
	CMiniMapPropDlg Dlg;
	Dlg.DoModal();
}

void CNtlWEDoc::OnFieldFieldname()
{
	CFieldNamePropDlg Dlg;
	if(Dlg.DoModal() == IDOK)
	{
		if(!stricmp(m_pNtlFieldProp->_Name, "null"))
		{
			return;
		}

		/*RwBool					IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
		CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

		if(IsAField)
		{
			RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
			RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);
			pFieldMgr->SetAFieldProp(CurFieldMidPosInWorld, *m_pNtlFieldProp, eNFP_NAME);
		}
		else
		{
			RECT RTInUI = dGETHDL()->m_pFieldUIManager->GetFieldDragInfo();
			RwV3d STmp, ETmp;
			STmp.x = static_cast<RwReal>(RTInUI.left);
			STmp.z = static_cast<RwReal>(RTInUI.top);
			ETmp.x = static_cast<RwReal>(RTInUI.right);
			ETmp.z = static_cast<RwReal>(RTInUI.bottom);
			RwV3d SPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(STmp);
			RwV3d EPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(ETmp);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(SPos, FALSE);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(EPos, FALSE);

			for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
			{
				for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
				{
					RwV3d CurPos;
					CurPos.x = j + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					CurPos.z = i + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					pFieldMgr->SetAFieldProp(CurPos, *m_pNtlFieldProp, eNFP_NAME);
				}
			}
		}*/

		dGETDOC()->ApplyFieldProp(((CNtlWEApp*)(AfxGetApp()))->m_pDoc->m_pNtlFieldProp, eNFP_NAME);

		// 인도어
// 		switch (GetSceneManager()->GetActiveWorldType())
// 		{
// 		case AW_HEGITHFIELD:
// 			dGETHDL()->m_pFieldUIManager->ApplyFieldProp(m_pNtlFieldProp, eNFP_NAME);
// 			break;
// 		case AW_RWWORLD:
// 			dGETFRM()->m_pWorldViewDlg->ApplyFieldProp(m_pNtlFieldProp, eNFP_NAME);
// 			break;
// 		}
	}
}

void CNtlWEDoc::OnInformation()
{
	CFieldsInformation Dlg;
	Dlg.DoModal();
}

void CNtlWEDoc::OnFieldMonopower()
{
	CBloomPropDlg Dlg;
	if(Dlg.DoModal() == IDOK)
	{
		/*RwBool					IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
		CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

		if(IsAField)
		{
			RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
			RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);
			pFieldMgr->SetAFieldProp(CurFieldMidPosInWorld, *m_pNtlFieldProp, eNFP_BLOOM);
		}
		else
		{
			RECT RTInUI = dGETHDL()->m_pFieldUIManager->GetFieldDragInfo();
			RwV3d STmp, ETmp;
			STmp.x = static_cast<RwReal>(RTInUI.left);
			STmp.z = static_cast<RwReal>(RTInUI.top);
			ETmp.x = static_cast<RwReal>(RTInUI.right);
			ETmp.z = static_cast<RwReal>(RTInUI.bottom);
			RwV3d SPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(STmp);
			RwV3d EPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(ETmp);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(SPos, FALSE);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(EPos, FALSE);

			for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
			{
				for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
				{
					RwV3d CurPos;
					CurPos.x = j + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					CurPos.z = i + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					pFieldMgr->SetAFieldProp(CurPos, *m_pNtlFieldProp, eNFP_BLOOM);
				}
			}
		}*/

		dGETDOC()->ApplyFieldProp(((CNtlWEApp*)(AfxGetApp()))->m_pDoc->m_pNtlFieldProp, eNFP_BLOOM);

		// 인도어
// 		switch (GetSceneManager()->GetActiveWorldType())
// 		{
// 		case AW_HEGITHFIELD:
// 			dGETHDL()->m_pFieldUIManager->ApplyFieldProp(m_pNtlFieldProp, eNFP_BLOOM);
// 			break;
// 		case AW_RWWORLD:
// 			dGETFRM()->m_pWorldViewDlg->ApplyFieldProp(m_pNtlFieldProp, eNFP_BLOOM);
// 			break;
// 		}
	}
}
void CNtlWEDoc::OnCreateTransparentTiles()
{
	dGETHDL()->SetTransparentTiles(TRUE);
}

void CNtlWEDoc::OnDeleteTransparentTiles()
{
	dGETHDL()->SetTransparentTiles(FALSE);
}
/*
if(m_SLPropDlg.DoModal() == IDOK)
{
	RwBool					IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
	CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	RwChar					Text[40];
	RwInt32					SLFieldNum	= 0;
	RwInt32					Counter		= 0;
	CProgressWnd			WndProgress((CNtlWEFrm*)AfxGetMainWnd(), "Slope lighting");

	if(IsAField)
	{
		RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
		RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);

		WndProgress.GoModal();
		WndProgress.SetRange(0, 1);

		::sprintf_s(Text, 40, "1 / 1 field is now calculating...");
		WndProgress.SetText(Text);
		WndProgress.StepIt();
		WndProgress.PeekAndPump();

		SetSLData(CurFieldMidPosInWorld);

		WndProgress.SetText("Done.");
		Sleep(500);
	}
	else
	{
		RECT RTInUI = dGETHDL()->m_pFieldUIManager->GetFieldDragInfo();
		RwV3d STmp, ETmp;
		STmp.x = static_cast<RwReal>(RTInUI.left);
		STmp.z = static_cast<RwReal>(RTInUI.top);
		ETmp.x = static_cast<RwReal>(RTInUI.right);
		ETmp.z = static_cast<RwReal>(RTInUI.bottom);
		RwV3d SPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(STmp);
		RwV3d EPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(ETmp);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(SPos, FALSE);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(EPos, FALSE);

		// count how many fields
		for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
		{
			for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
			{
				++SLFieldNum;
			}
		}

		WndProgress.GoModal();
		WndProgress.SetRange(0, SLFieldNum);

		for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
		{
			for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
			{
				::sprintf_s(Text, 40, "%d / %d field is now calculating...", Counter++, SLFieldNum);
				WndProgress.SetText(Text);

				RwV3d CurPos;
				CurPos.x = j + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
				CurPos.z = i + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
				SetSLData(CurPos);

				WndProgress.StepIt();
				WndProgress.PeekAndPump();
			}
		}

		WndProgress.SetText("Done.");
		Sleep(500);
	}
}*/

//void CNtlWEDoc::OnCreatePvs()
//{
//	CNtlWorldFieldManager* pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
//
//	if(::AfxMessageBox("Are you sure?", MB_YESNO) == IDYES)	
//	{
//		RwBool			IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
//		CNtlWorldField*	pFields		= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
//
//		CProgressWnd	WndProgress((CNtlWEFrm*)AfxGetMainWnd(), "PVS Building.");
//		RwInt32			Counter	= 0;
//		RwChar			Text[40];
//
//		if(IsAField)
//		{
//			RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
//			RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
//			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);
//
//			RwInt32 CurFieldIdx		= pFieldMgr->GetFieldIdx(CurFieldMidPosInWorld);
//			RwV3d	SPosOfCurField	= pFields[CurFieldIdx].GetSPos();
//			RwV3d	SectorSPos;
//			RwInt32	CurSectorIdx;
//
//			Counter = 4;
//			WndProgress.GoModal();
//			WndProgress.SetRange(0, Counter);
//
//			for(RwInt32 l = static_cast<RwInt32>(SPosOfCurField.z); l < static_cast<RwInt32>(SPosOfCurField.z) + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
//			{
//				for(RwInt32 m = static_cast<RwInt32>(SPosOfCurField.x); m < static_cast<RwInt32>(SPosOfCurField.x) + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
//				{
//					SectorSPos.x	= static_cast<RwReal>(m);
//					SectorSPos.z	= static_cast<RwReal>(l);
//					CurSectorIdx	= pFieldMgr->GetSectorIdx(SectorSPos);
//					
//					::sprintf_s(Text, 40, "%d's sector is now calculating...", CurSectorIdx);
//					WndProgress.SetText(Text);
//					WndProgress.StepIt();
//					WndProgress.PeekAndPump();
//
//					pFieldMgr->m_pSectors[CurSectorIdx].CreateSectorPVS(CurSectorIdx);
//				}
//			}
//		}
//		else
//		{
//			RECT RTInUI = dGETHDL()->m_pFieldUIManager->GetFieldDragInfo();
//			RwV3d STmp, ETmp;
//			STmp.x = static_cast<RwReal>(RTInUI.left);
//			STmp.z = static_cast<RwReal>(RTInUI.top);
//			ETmp.x = static_cast<RwReal>(RTInUI.right);
//			ETmp.z = static_cast<RwReal>(RTInUI.bottom);
//			RwV3d SPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(STmp);
//			RwV3d EPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(ETmp);
//			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(SPos, FALSE);
//			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(EPos, FALSE);
//
//			for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
//			{
//				for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
//				{
//					Counter++;
//				}
//			}
//
//			Counter *= 4;
//			WndProgress.GoModal();
//			WndProgress.SetRange(0, Counter);
//
//			for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
//			{
//				for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
//				{
//					RwV3d CurPos;
//					CurPos.x = j + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
//					CurPos.z = i + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
//
//					RwInt32 CurFieldIdx		= pFieldMgr->GetFieldIdx(CurPos);
//					RwV3d	SPosOfCurField	= pFields[CurFieldIdx].GetSPos();
//					RwV3d	SectorSPos;
//					RwInt32	CurSectorIdx;
//
//					for(RwInt32 l = static_cast<RwInt32>(SPosOfCurField.z); l < static_cast<RwInt32>(SPosOfCurField.z) + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
//					{
//						for(RwInt32 m = static_cast<RwInt32>(SPosOfCurField.x); m < static_cast<RwInt32>(SPosOfCurField.x) + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
//						{
//							SectorSPos.x	= static_cast<RwReal>(m);
//							SectorSPos.z	= static_cast<RwReal>(l);
//							CurSectorIdx	= pFieldMgr->GetSectorIdx(SectorSPos);
//
//							::sprintf_s(Text, 40, "%d's sector is now calculating...", CurSectorIdx);
//							WndProgress.SetText(Text);
//							WndProgress.StepIt();
//							WndProgress.PeekAndPump();
//
//							pFieldMgr->m_pSectors[CurSectorIdx].CreateSectorPVS(CurSectorIdx);
//						}
//					}
//				}
//			}
//		}
//
//		// refresh PVS at current position; could work for the next frame
//		pFieldMgr->RefreshPVS();
//
//		WndProgress.SetText("Done.");
//		Sleep(500);
//	}
//}

//void CNtlWEDoc::OnDestroyPvs()
//{
//	if(::AfxMessageBox("Are you sure?", MB_YESNO) == IDYES)	
//	{
//		RwBool					IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
//		CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
//		CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
//
//		if(IsAField)
//		{
//			RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
//			RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
//			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);
//
//			RwInt32 CurFieldIdx		= pFieldMgr->GetFieldIdx(CurFieldMidPosInWorld);
//			RwV3d	SPosOfCurField	= pFields[CurFieldIdx].GetSPos();
//			RwV3d	SectorSPos;
//			RwInt32	CurSectorIdx;
//
//			for(RwInt32 l = static_cast<RwInt32>(SPosOfCurField.z); l < static_cast<RwInt32>(SPosOfCurField.z) + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
//			{
//				for(RwInt32 m = static_cast<RwInt32>(SPosOfCurField.x); m < static_cast<RwInt32>(SPosOfCurField.x) + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
//				{
//					SectorSPos.x	= static_cast<RwReal>(m);
//					SectorSPos.z	= static_cast<RwReal>(l);
//					CurSectorIdx	= pFieldMgr->GetSectorIdx(SectorSPos);
//
//					pFieldMgr->m_pSectors[CurSectorIdx].DestroySectorPVS(CurSectorIdx);
//				}
//			}
//		}
//		else
//		{
//			RECT RTInUI = dGETHDL()->m_pFieldUIManager->GetFieldDragInfo();
//			RwV3d STmp, ETmp;
//			STmp.x = static_cast<RwReal>(RTInUI.left);
//			STmp.z = static_cast<RwReal>(RTInUI.top);
//			ETmp.x = static_cast<RwReal>(RTInUI.right);
//			ETmp.z = static_cast<RwReal>(RTInUI.bottom);
//			RwV3d SPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(STmp);
//			RwV3d EPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(ETmp);
//			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(SPos, FALSE);
//			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(EPos, FALSE);
//
//			for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
//			{
//				for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
//				{
//					RwV3d CurPos;
//					CurPos.x = j + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
//					CurPos.z = i + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
//
//					RwInt32 CurFieldIdx		= pFieldMgr->GetFieldIdx(CurPos);
//					RwV3d	SPosOfCurField	= pFields[CurFieldIdx].GetSPos();
//					RwV3d	SectorSPos;
//					RwInt32	CurSectorIdx;
//
//					for(RwInt32 l = static_cast<RwInt32>(SPosOfCurField.z); l < static_cast<RwInt32>(SPosOfCurField.z) + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
//					{
//						for(RwInt32 m = static_cast<RwInt32>(SPosOfCurField.x); m < static_cast<RwInt32>(SPosOfCurField.x) + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
//						{
//							SectorSPos.x	= static_cast<RwReal>(m);
//							SectorSPos.z	= static_cast<RwReal>(l);
//							CurSectorIdx	= pFieldMgr->GetSectorIdx(SectorSPos);
//
//							pFieldMgr->m_pSectors[CurSectorIdx].DestroySectorPVS(CurSectorIdx);
//						}
//					}
//				}
//			}
//		}
//	}
//}

void CNtlWEDoc::OnMaterialPropUpdate()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CMaterialPropDlg Dlg;
	Dlg.DoModal();
}

void CNtlWEDoc::OnFieldcolor()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if(m_FieldColorPropDlg.DoModal() == IDOK)
	{
		/*RwBool					IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
		CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

		if(IsAField)
		{
			RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
			RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);
			pFieldMgr->SetAFieldProp(CurFieldMidPosInWorld, *m_pNtlFieldProp, eNFP_FIELDCOLOR);
		}
		else
		{
			RECT RTInUI = dGETHDL()->m_pFieldUIManager->GetFieldDragInfo();
			RwV3d STmp, ETmp;
			STmp.x = static_cast<RwReal>(RTInUI.left);
			STmp.z = static_cast<RwReal>(RTInUI.top);
			ETmp.x = static_cast<RwReal>(RTInUI.right);
			ETmp.z = static_cast<RwReal>(RTInUI.bottom);
			RwV3d SPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(STmp);
			RwV3d EPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(ETmp);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(SPos, FALSE);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(EPos, FALSE);

			for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
			{
				for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
				{
					RwV3d CurPos;
					CurPos.x = j + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					CurPos.z = i + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					pFieldMgr->SetAFieldProp(CurPos, *m_pNtlFieldProp, eNFP_FIELDCOLOR);
				}
			}
		}*/

		dGETDOC()->ApplyFieldProp(((CNtlWEApp*)(AfxGetApp()))->m_pDoc->m_pNtlFieldProp, eNFP_FIELDCOLOR);

		// 인도어
// 		switch (GetSceneManager()->GetActiveWorldType())
// 		{
// 		case AW_HEGITHFIELD:
// 			dGETHDL()->m_pFieldUIManager->ApplyFieldProp(m_pNtlFieldProp, eNFP_FIELDCOLOR);
// 			break;
// 		case AW_RWWORLD:
// 			dGETFRM()->m_pWorldViewDlg->ApplyFieldProp(m_pNtlFieldProp, eNFP_FIELDCOLOR);
// 			break;
// 		}
	}
}

void CNtlWEDoc::OnFieldsearch()
{
	((CNtlWEFrm*)AfxGetMainWnd())->m_pFieldSearchDlg->ShowWindow(SW_SHOW);
	((CNtlWEFrm*)AfxGetMainWnd())->m_pFieldSearchDlg->InitFieldSearch();
}

void CNtlWEDoc::OnWeather()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if(m_WeatherDlg.DoModal() == IDOK)
	{
		/*RwBool					IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
		CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

		if(IsAField)
		{
			RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
			RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);
			pFieldMgr->SetAFieldProp(CurFieldMidPosInWorld, *m_pNtlFieldProp, eNFP_WEATHER);
		}
		else
		{
			RECT RTInUI = dGETHDL()->m_pFieldUIManager->GetFieldDragInfo();
			RwV3d STmp, ETmp;
			STmp.x = static_cast<RwReal>(RTInUI.left);
			STmp.z = static_cast<RwReal>(RTInUI.top);
			ETmp.x = static_cast<RwReal>(RTInUI.right);
			ETmp.z = static_cast<RwReal>(RTInUI.bottom);
			RwV3d SPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(STmp);
			RwV3d EPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(ETmp);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(SPos, FALSE);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(EPos, FALSE);

			for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
			{
				for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
				{
					RwV3d CurPos;
					CurPos.x = j + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					CurPos.z = i + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					pFieldMgr->SetAFieldProp(CurPos, *m_pNtlFieldProp, eNFP_WEATHER);
				}
			}
		}*/

		dGETDOC()->ApplyFieldProp(((CNtlWEApp*)(AfxGetApp()))->m_pDoc->m_pNtlFieldProp, eNFP_WEATHER);

		// 인도어
// 		switch (GetSceneManager()->GetActiveWorldType())
// 		{
// 		case AW_HEGITHFIELD:
// 			dGETHDL()->m_pFieldUIManager->ApplyFieldProp(m_pNtlFieldProp, eNFP_WEATHER);
// 			break;
// 		case AW_RWWORLD:
// 			dGETFRM()->m_pWorldViewDlg->ApplyFieldProp(m_pNtlFieldProp, eNFP_WEATHER);
// 			break;
// 		}
	}
}

void CNtlWEDoc::OnFieldheathaze()
{
	((CNtlWEFrm*)AfxGetMainWnd())->m_pFieldHeatHazeDlg->ShowWindow(SW_SHOW);
	((CNtlWEFrm*)AfxGetMainWnd())->m_pFieldHeatHazeDlg->InitializeWorldData();
}
void CNtlWEDoc::OnBlockPortal()
{
	if (((CNtlWEFrm*)AfxGetMainWnd())->m_pWorldViewDlg->IsSelectEmpty())
	{
		return;
	}

	RwV3d vCurPortal = ((CNtlWEFrm*)AfxGetMainWnd())->m_pWorldViewDlg->GetSelectBlockMidPos();

	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(vCurPortal);
	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlPLWorldEntity()->SetPortalPosition(vCurPortal);

	// force the camera to be free
	CNtlWeControlUi::GetInstance().SetActiveWeController(dGETAPP().GetNtlGameCamera());
	dGETFRM()->m_pCameraPortal->SetCurCamMode(0);
}

void CNtlWEDoc::OnSectorPortal()
{
	if (((CNtlWEFrm*)AfxGetMainWnd())->m_pWorldViewDlg->IsSelectEmpty())
	{
		return;
	}

	RwV3d vCurPortal = ((CNtlWEFrm*)AfxGetMainWnd())->m_pWorldViewDlg->GetSelectBlockMidPos();

	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(vCurPortal);
	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlPLWorldEntity()->SetPortalPosition(vCurPortal);

	// force the camera to be free
	CNtlWeControlUi::GetInstance().SetActiveWeController(dGETAPP().GetNtlGameCamera());
	dGETFRM()->m_pCameraPortal->SetCurCamMode(0);
}

void CNtlWEDoc::OnFieldPortal()
{
	if (((CNtlWEFrm*)AfxGetMainWnd())->m_pWorldViewDlg->IsSelectEmpty())
	{
		return;
	}

	RwV3d vCurPortal = ((CNtlWEFrm*)AfxGetMainWnd())->m_pWorldViewDlg->GetSelectBlockMidPos();

	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(vCurPortal);
	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlPLWorldEntity()->SetPortalPosition(vCurPortal);

	// force the camera to be free
	CNtlWeControlUi::GetInstance().SetActiveWeController(dGETAPP().GetNtlGameCamera());
	dGETFRM()->m_pCameraPortal->SetCurCamMode(0);
}

void CNtlWEDoc::OnBlockGameProperty()
{
	if(m_GamePropDlg.DoModal() == IDOK)
	{
		// 인도어
		switch (GetSceneManager()->GetActiveWorldType())
		{
		case AW_RWWORLD:
			dGETFRM()->m_pWorldViewDlg->ApplyBlockProp(m_pNtlBlockProp, eNBP_GAMEPROP);
			break;
		}
	}
}

void CNtlWEDoc::OnBlockSkyfloating()
{
	((CNtlWEFrm*)AfxGetMainWnd())->OnSkyFloatDlg();
}

RwBool CNtlWEDoc::GetFieldProp(sNTL_FIELD_PROP& sNtlFieldProp)
{
	RwBool bGetResult = FALSE;
	RwBool bIsAField  = FALSE;
	CNtlWorldFieldManager* pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	switch (dGETDOC()->GetFieldPropMode())
	{
	case EFIELDPROP_MODE_OUTDOOR:
		{
			bIsAField = dGETHDL()->m_pFieldUIManager->IsSingleSelected();		

			if(bIsAField)
			{
				RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
				RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
				((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);

				bGetResult = pFieldMgr->GetAFieldProp(CurFieldMidPosInWorld, sNtlFieldProp);
			}

			break;
		}		
	case EFIELDPROP_MODE_INDOOR:
		{
			RwInt32 iXNum, iYNum;
			dGETFRM()->m_pWorldViewDlg->GetSelectBlockNum(&iXNum, &iYNum);
			if (dGETFRM()->m_pWorldViewDlg->GetBlockMode() == EBLOCK_MODE_BLOCK && iXNum == 1 && iYNum == 1)
			{
				bIsAField = TRUE;
				bGetResult = pFieldMgr->GetAFieldProp(dGETFRM()->m_pWorldViewDlg->GetSelectBlockMidPos(), sNtlFieldProp);				
			}			
			break;
		}		
	case EFIELDPROP_MODE_OBJECT:
		{
			if (dGETHDL()->GetCapturedDoodadsNum() == 1)
			{
				bIsAField = TRUE;

				if (dGETHDL()->GetCurDoodad()->GetObjectTypePtr() && dGETHDL()->GetCurDoodad()->GetObjectTypePtr()->GetFuncFlags() & EPL_OBJECT_FUNC_FLAG_PROPERTY)
				{
					CNtlPLObjectTypeHasProperty* pObjectType = (CNtlPLObjectTypeHasProperty*)(dGETHDL()->GetCurDoodad()->GetObjectTypePtr());
					bGetResult = pObjectType->GetObjectProperty(sNtlFieldProp);
				}
			}
			break;
		}
	}
	
	if (bIsAField)
	{
		DBO_TRACE(bGetResult, "CNtlWEDoc::GetFieldProp, can't get a field property.");

		return TRUE;
	}
	return FALSE;
}

VOID CNtlWEDoc::ApplyFieldProp(sNTL_FIELD_PROP* pNtlFieldProp, eNTL_FIELD_PROPID eNtlFieldPropID)
{
	switch (dGETDOC()->GetFieldPropMode())
	{
	case EFIELDPROP_MODE_OUTDOOR:
		{
			dGETHDL()->m_pFieldUIManager->ApplyFieldProp(pNtlFieldProp, eNtlFieldPropID);
			break;
		}
	case EFIELDPROP_MODE_INDOOR:
		{
			dGETFRM()->m_pWorldViewDlg->ApplyFieldProp(pNtlFieldProp, eNtlFieldPropID);
			break;
		}
	case EFIELDPROP_MODE_OBJECT:
		{
			dGETHDL()->ApplyObjectProperty(pNtlFieldProp, eNtlFieldPropID);
			break;
		}
	}
}
void CNtlWEDoc::OnPropertyClipboardCopy()
{
	if (!GetFieldProp(*m_pNtlFieldPropClipboard))
	{
		AfxMessageBox("not available for the multi fields selected.");
	}
}

void CNtlWEDoc::OnPropertyClipboardPaste()
{
	ApplyFieldProp(m_pNtlFieldPropClipboard, eNFP_NAME);
	//ApplyFieldProp(m_pNtlFieldPropClipboard, eNFP_BITPROP);
	ApplyFieldProp(m_pNtlFieldPropClipboard, eNFP_FOG);
	ApplyFieldProp(m_pNtlFieldPropClipboard, eNFP_SKY);
	ApplyFieldProp(m_pNtlFieldPropClipboard, eNFP_LIGHT);
	ApplyFieldProp(m_pNtlFieldPropClipboard, eNFP_SOUND);
	ApplyFieldProp(m_pNtlFieldPropClipboard, eNFP_BLOOM);
	ApplyFieldProp(m_pNtlFieldPropClipboard, eNFP_PLANET);
	ApplyFieldProp(m_pNtlFieldPropClipboard, eNFP_SPECULAR);
	//ApplyFieldProp(m_pNtlFieldPropClipboard, eNFP_MATERIALPROP);
	ApplyFieldProp(m_pNtlFieldPropClipboard, eNFP_FIELDCOLOR);
	ApplyFieldProp(m_pNtlFieldPropClipboard, eNFP_WEATHER);
	ApplyFieldProp(m_pNtlFieldPropClipboard, eNFP_FIELDHEATHAZE);
}

void CNtlWEDoc::OnTempProofsinvalidsectorlink()
{
	RwBool					IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
	CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
	CProgressWnd			WndProgress((CNtlWEFrm*)AfxGetMainWnd(), "Proofs InvalidSectorLink");
	RwChar					StrProgress[128];
	RwInt32					NumFields;

	WndProgress.GoModal();

		if(IsAField)
		{
			RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
			RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);

			WndProgress.SetRange(0, 1);
			::sprintf_s(StrProgress, 128, "%d's Field is now progressing.", pFieldMgr->GetFieldIdx(CurFieldMidPosInWorld));
			WndProgress.SetText(StrProgress);
			WndProgress.StepIt();
			WndProgress.PeekAndPump();

			CNtlWorldMergeManager::GetInstance()->OnProofsInvalidSectorLink(pFieldMgr->GetFieldIdx(CurFieldMidPosInWorld));
		}
		else
		{
			RECT RTInUI = dGETHDL()->m_pFieldUIManager->GetFieldDragInfo();
			RwV3d STmp, ETmp;
			STmp.x = static_cast<RwReal>(RTInUI.left);
			STmp.z = static_cast<RwReal>(RTInUI.top);
			ETmp.x = static_cast<RwReal>(RTInUI.right);
			ETmp.z = static_cast<RwReal>(RTInUI.bottom);
			RwV3d SPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(STmp);
			RwV3d EPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(ETmp);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(SPos, FALSE);
			((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(EPos, FALSE);

			NumFields = 0;
			for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
			{
				for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
				{
					NumFields++;
				}
			}

			WndProgress.SetRange(0, NumFields);

			for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
			{
				for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
				{
					RwV3d CurPos;
					CurPos.x = j + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
					CurPos.z = i + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);

					::sprintf_s(StrProgress, 128, "%d's Field is now progressing.", pFieldMgr->GetFieldIdx(CurPos));
					WndProgress.SetText(StrProgress);
					WndProgress.StepIt();
					WndProgress.PeekAndPump();

					CNtlWorldMergeManager::GetInstance()->OnProofsInvalidSectorLink(pFieldMgr->GetFieldIdx(CurPos));
				}
			}
		}
	
	WndProgress.SetText("Done.");
	Sleep(100);
}
