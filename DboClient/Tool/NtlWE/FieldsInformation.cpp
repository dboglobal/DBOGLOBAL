// FieldsInformation.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "FieldsInformation.h"
#include "FieldUIManager.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLWorldEntity.h"
#include "NtlWorldBGMManager.h"
#include "NtlWorldPropBaseTemplate.h"
#include "ntltext.h"
#include "TextAllTable.h"
#include "NtlSLAPI.h"
#include "TableContainer.h"
#include "table.h"
#include "ntlworldfieldmanager.h"
#include "SpawnManager.h"
#include "NtlWorldSectorInfo.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

// CFieldsInformation 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFieldsInformation, CDialog)

CFieldsInformation::CFieldsInformation(CWnd* pParent /*=NULL*/)
	: CDialog(CFieldsInformation::IDD, pParent)
{

}

CFieldsInformation::~CFieldsInformation()
{
}

void CFieldsInformation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FI_FIELDS, m_Fields);
	DDX_Control(pDX, IDC_FI_INFORMATION, m_Information);
}


BEGIN_MESSAGE_MAP(CFieldsInformation, CDialog)
	ON_LBN_SELCHANGE(IDC_FI_FIELDS, &CFieldsInformation::OnLbnSelchangeFiFields)
END_MESSAGE_MAP()


// CFieldsInformation 메시지 처리기입니다.

BOOL CFieldsInformation::PreTranslateMessage(MSG* pMsg)
{
	// prevent keys; escape, return
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
				return true;
				break;
			}
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CFieldsInformation::OnInitDialog()
{
	CDialog::OnInitDialog();

	RwInt32					CurFieldIdx;
	RwBool					IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
	CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	RwChar					FieldIdxName[10];
	CString					StrInformation;

	m_Fields.ResetContent();
	m_Information.SetWindowText("");

	if(IsAField)
	{
		RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
		RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);
		CurFieldIdx = pFieldMgr->GetFieldIdx(CurFieldMidPosInWorld);

		// Fill field indices into the list control
		sprintf_s(FieldIdxName, 10, "%d", CurFieldIdx);
		m_Fields.AddString(FieldIdxName);
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

				CurFieldIdx = pFieldMgr->GetFieldIdx(CurPos);

				// Fill field indices into the list control
				sprintf_s(FieldIdxName, 10, "%d", CurFieldIdx);
				m_Fields.AddString(FieldIdxName);
			}
		}
	}

	// Setup current sel then put the field information into the edit control
	m_Fields.SetCurSel(0);

	RwChar	StrFieldIdx[32];
	RwInt32	FieldSelIdx;

	if(m_Fields.GetText(0, StrFieldIdx) == LB_ERR)
	{
		NTL_ASSERTE("CFieldsInformation::OnInitDialog");
	}
	else
	{
		FieldSelIdx = static_cast<RwInt32>(atoi(StrFieldIdx));
		RefreshInfo(FieldSelIdx);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

VOID CFieldsInformation::RefreshInfo(RwInt32 FieldIdx)
{
	RwBool					IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
	CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
	RwV3d					SPos		= pFields[FieldIdx].GetSPos();

	sNTL_FIELD_PROP NtlFieldProp;
	if(!pFieldMgr->GetAFieldProp(SPos, NtlFieldProp))
	{
		NTL_ASSERTE("CFieldsInformation::RefreshInfo, pFieldMgr->GetAFieldProp(SPos, NtlFieldProp)");
	}

	// 1. BGM Name
	const RwChar* pBGMName = dGET_BGM_MANAGER()->GetBGMSoundName(NtlFieldProp._IdxBGMTbl);
	if(pBGMName)
	{
		m_StrBGMName.Format("1. BGM Name\r\n- %s\r\n", pBGMName);
	}
	else
	{
		m_StrBGMName.Format("1. BGM Name\r\n- \"No BGM\"\r\n", pBGMName);
	}
	
	m_Result = m_StrBGMName;
	m_StrBGMName.Format("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\r\n");
	m_Result += m_StrBGMName;

	// Env Name
	const RwChar* pEnvName = dGET_BGM_MANAGER()->GetEnvSoundName(NtlFieldProp._IdxEnvTbl);
	if(pEnvName)
	{
		m_StrEnvName.Format("  Env Name\r\n- %s\r\n", pEnvName);
	}
	else
	{
		m_StrEnvName.Format("  Env Name\r\n- \"No Env\"\r\n", pEnvName);
	}

	m_Result += m_StrEnvName;
	m_StrEnvName.Format("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\r\n");
	m_Result += m_StrEnvName;

	// 2. Normal Map Prop.
	RwInt32 TilePropNum = (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2);
	RwInt32 NMPArray[16];
	DWORD	CurTileProp;

	for(RwInt32 i = 0; i < 16; ++i)
	{
		NMPArray[i] = 999;

		for(RwInt32 j = 0; j < TilePropNum; ++j)
		{
			CurTileProp = NtlFieldProp._pBitProp[j];

			if(CNtlWorldPropBaseTemplate::GetHexCode(i) & CurTileProp)
			{
				NMPArray[i] = i;
				break;
			}
		}
	}
	
	RwBool AtLeastOnce = FALSE;
	for(RwInt32 i = 0; i < 16; ++i)
	{
		if(NMPArray[i] != 999)
		{
			AtLeastOnce = TRUE;
			break;
		}
	}

	if(AtLeastOnce)
	{
		m_StrNMapSpec.Format("2. Map Property\r\n- ");
		m_Result += m_StrNMapSpec;

		for(RwInt32 i = 0; i < 16; ++i)
		{
			if(NMPArray[i] != 999)
			{
				m_StrNMapSpec.Format("%d ", NMPArray[i]);
				m_Result += m_StrNMapSpec;
			}
		}

		m_StrNMapSpec.Format("\r\n");
		m_Result += m_StrNMapSpec;
	}
	else
	{
		m_StrNMapSpec.Format("2. Map Property\r\n- \"No Property\"\r\n");
		m_Result += m_StrNMapSpec;
	}

	m_StrNMapSpec.Format("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\r\n");
	m_Result += m_StrNMapSpec;

	// 3. Map Name
	if(!_stricmp(NtlFieldProp._Name, "null") || !_stricmp(NtlFieldProp._Name, ""))
	{
		m_MapName.Format("3. Map Name\r\n- \"No Name\"\r\n");
		m_Result += m_MapName;
	}
	else
	{
		CTextTable*	pTextMobTbl	= static_cast<CTextTable*>(API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl());
		TBLIDX		TableIdx	= static_cast<TBLIDX>(atoi(NtlFieldProp._Name));
		std::wstring WStr		= pTextMobTbl->GetText(TableIdx);

		RwChar MBStr[256];
		::WideCharToMultiByte(GetACP(), 0, WStr.c_str(), -1, MBStr, 256, NULL, NULL);

		m_MapName.Format("3. Map Name\r\n- %s\r\n- %s\r\n", NtlFieldProp._Name, MBStr);
		m_Result += m_MapName;
	}

	m_MapName.Format("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\r\n");
	m_Result += m_MapName;

	// 4. Trigger Object
	CFileMem	FileMem;
	BYTE*		pPosCurFileMem;
	BYTE*		pTmp;
	RwInt32		TriggerObjCnt = 0;
	RwV3d		TriggerTrans;
	RwBool		TriggerEnable;
	RwUInt32	TriggerID;
	RwInt32		TriggerTotNum;

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);
	FileMem.Load(dGET_WORLD_PARAM()->WorldChar64Buf, "rb");
	pPosCurFileMem = FileMem.GetDat();

	m_Trigger.Format("4. Trigger Object\r\n");
	m_Result += m_Trigger;

	for(RwInt32 i = 0; i < dGET_WORLD_PARAM()->WorldFieldSectorNum * dGET_WORLD_PARAM()->WorldFieldSectorNum; ++i)
	{
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorMaterialSkipToFileMem(pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorHeightfieldSkipToFileMem(pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorPrelightsSkipToFileMem(pPosCurFileMem);
		
		CopyMemory(&TriggerTotNum, pPosCurFileMem, sizeof(RwInt32));
		pPosCurFileMem += sizeof(RwInt32);

		RwInt32 TriggerObjCnt = 0;

		pTmp = pPosCurFileMem;
		for(RwInt32 j = 0; j < TriggerTotNum; ++j)
		{
			RwBool bEnabled = FALSE;
			pTmp = GetNtlWorldSectorInfo()->ObjectSkipToFileMemGetEnabledTrigger(pTmp, AW_HEGITHFIELD, &bEnabled);
			if (bEnabled)
			{
				++TriggerObjCnt;
			}
		}

		m_Trigger.Format("- Counts at %d's Sector : [%d]\r\n", i, TriggerObjCnt);
		m_Result += m_Trigger;

		for(RwInt32 j = 0; j < TriggerTotNum; ++j)
		{
			// 1. name
			if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
			{
				pPosCurFileMem += (sizeof(RwChar) * 128);
			}
			else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
			{
				RwUInt32 uiLength = 0;
				CopyMemory(&uiLength, pPosCurFileMem, sizeof(RwUInt32));
				pPosCurFileMem += sizeof(RwUInt32);

				pPosCurFileMem += (sizeof(RwChar) * uiLength);
			}

			// 2. scaling
			pPosCurFileMem += sizeof(RwV3d);

			// 3. rotation
			pPosCurFileMem += sizeof(RwV3d);

			// 4. translation
			CopyMemory(&TriggerTrans, pPosCurFileMem, sizeof(RwV3d));
			pPosCurFileMem += sizeof(RwV3d);

			RwUInt32 uiObjectType;
			CopyMemory(&uiObjectType, pPosCurFileMem, sizeof(RwUInt32));
			pPosCurFileMem += sizeof(RwUInt32);

			if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
				uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
			{
				// Object Propperty Range
				pPosCurFileMem = CNtlPLObjectTypeHasProperty::SkipToFileMem(pPosCurFileMem);
			}

			// 5. serial ID
			CopyMemory(&TriggerID, pPosCurFileMem, sizeof(RwUInt32));
			pPosCurFileMem += sizeof(RwUInt32);

			if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
			{
				pPosCurFileMem += (sizeof(RwUInt32));		// Object  serial id	
			}

			// 6. object name index
			pPosCurFileMem += sizeof(DWORD);

			// 7. visibility distance
			pPosCurFileMem += sizeof(RwReal);

			if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
			{
				// 8. enable trigger
				CopyMemory(&TriggerEnable, pPosCurFileMem, sizeof(RwBool));
				pPosCurFileMem += sizeof(RwBool);

				// 9. enable selection
				pPosCurFileMem += sizeof(RwBool);

				// 10. enable alterable; dynamic obj.s.
				pPosCurFileMem += sizeof(RwBool);

				// 11. enable portal
				pPosCurFileMem += sizeof(RwBool);

				// 12. enable path engine collision
				pPosCurFileMem += sizeof(RwBool);

				// 13. enable shadowprop
				pPosCurFileMem += sizeof(RwBool);
			}
			else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
			{
				RwUInt32 uiFlags = 0;
				CopyMemory(&uiFlags, pPosCurFileMem, sizeof(RwUInt32));
				pPosCurFileMem += sizeof(RwUInt32);

				TriggerEnable = ((uiFlags & NTL_PLEFLAG_TRIGGER) != 0);
			}

			if(TriggerEnable)
			{
				m_Trigger.Format("   * %u, (%.2f, %.2f, %.2f)\r\n", TriggerID, TriggerTrans.x, TriggerTrans.y, TriggerTrans.z);
				m_Result += m_Trigger;
			}

			// PS Map Size
			pPosCurFileMem = CNtlPLObject::SkipPSMapToFileMem(pPosCurFileMem);
			

			// milepost
			RwBool EnableFlg;
			CopyMemory(&EnableFlg, pPosCurFileMem, sizeof(RwBool));
			pPosCurFileMem += sizeof(RwBool);

			if(EnableFlg)
			{
				pPosCurFileMem += sizeof(RwUInt32);
			}

			// attach light object skip
			pPosCurFileMem = CNtlPLObject::SkipLightObjectAttachDataToFileMem(pPosCurFileMem);

			if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
			{
				pPosCurFileMem = CNtlPLObject::SkipLightmapToFileMem(pPosCurFileMem);
			}
		}

		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorWaterSkipToFileMem(pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorShadowSkipToFileMem(pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorEffectSkipToFileMem(pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorSoundEffectSkipToFileMem(pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorCullSkipToFileMem(pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorTileTransparencySkipToFileMem(pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorShoreLineSkipToFileMem(pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorDecalSkipToFileMem(pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorPlantSkipToFileMem(pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorWorldLightSkipToFileMem(pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorOccluderSkipToFileMem(pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorHeatHazeObjectSkipToFileMem(pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorLightObjectSkipToFileMem(pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorDojoSkipToFileMem(pPosCurFileMem);
	}
	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	FileMem.Free();

	m_Trigger.Format("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\r\n");
	m_Result += m_Trigger;

	// 5. MOB Spawn
	m_MobSpawn.Format("5. MOB Spawn Object\r\n");
	m_Result += m_MobSpawn;

	RwInt32 SpawnCnt = GetSpawnInstance()->GetSpawnCnt(FieldIdx);
	m_MobSpawn.Format("- Total Counts : [%d]\r\n", SpawnCnt);
	m_Result += m_MobSpawn;

	m_MobSpawn.Format("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\r\n");
	m_Result += m_MobSpawn;

	// The last one to display
	m_Information.SetWindowText(m_Result.GetBuffer(0));
}


void CFieldsInformation::OnLbnSelchangeFiFields()
{
	RwInt32 IdxCurSel = m_Fields.GetCurSel();
	RwChar	StrFieldIdx[32];
	RwInt32	FieldSelIdx;

	if(m_Fields.GetText(IdxCurSel, StrFieldIdx) == LB_ERR)
	{
		NTL_ASSERTE("CFieldsInformation::OnLbnSelchangeFiFields");
	}
	else
	{
		FieldSelIdx = static_cast<RwInt32>(atoi(StrFieldIdx));
		RefreshInfo(FieldSelIdx);
	}

	UpdateData(FALSE);
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

// CFieldsInformation 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFieldsInformation, CDialog)

CFieldsInformation::CFieldsInformation(CWnd* pParent /*=NULL*/)
: CDialog(CFieldsInformation::IDD, pParent)
{

}

CFieldsInformation::~CFieldsInformation()
{
}

void CFieldsInformation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FI_FIELDS, m_Fields);
	DDX_Control(pDX, IDC_FI_INFORMATION, m_Information);
}


BEGIN_MESSAGE_MAP(CFieldsInformation, CDialog)
	ON_LBN_SELCHANGE(IDC_FI_FIELDS, &CFieldsInformation::OnLbnSelchangeFiFields)
END_MESSAGE_MAP()


// CFieldsInformation 메시지 처리기입니다.

BOOL CFieldsInformation::PreTranslateMessage(MSG* pMsg)
{
	// prevent keys; escape, return
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
				return true;
				break;
			}
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CFieldsInformation::OnInitDialog()
{
	CDialog::OnInitDialog();

	RwInt32					CurFieldIdx;
	RwBool					IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
	CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	RwChar					FieldIdxName[10];
	CString					StrInformation;

	m_Fields.ResetContent();
	m_Information.SetWindowText("");

	if(IsAField)
	{
		RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
		RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);
		CurFieldIdx = pFieldMgr->GetFieldIdx(CurFieldMidPosInWorld);

		// Fill field indices into the list control
		sprintf_s(FieldIdxName, 10, "%d", CurFieldIdx);
		m_Fields.AddString(FieldIdxName);
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

				CurFieldIdx = pFieldMgr->GetFieldIdx(CurPos);

				// Fill field indices into the list control
				sprintf_s(FieldIdxName, 10, "%d", CurFieldIdx);
				m_Fields.AddString(FieldIdxName);
			}
		}
	}

	// Setup current sel then put the field information into the edit control
	m_Fields.SetCurSel(0);

	RwChar	StrFieldIdx[32];
	RwInt32	FieldSelIdx;

	if(m_Fields.GetText(0, StrFieldIdx) == LB_ERR)
	{
		NTL_ASSERTE("CFieldsInformation::OnInitDialog");
	}
	else
	{
		FieldSelIdx = static_cast<RwInt32>(atoi(StrFieldIdx));
		RefreshInfo(FieldSelIdx);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

VOID CFieldsInformation::RefreshInfo(RwInt32 FieldIdx)
{
	RwBool					IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
	CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
	RwV3d					SPos		= pFields[FieldIdx].GetSPos();

	sNTL_FIELD_PROP NtlFieldProp;
	if(!pFieldMgr->GetAFieldProp(SPos, NtlFieldProp))
	{
		NTL_ASSERTE("CFieldsInformation::RefreshInfo, pFieldMgr->GetAFieldProp(SPos, NtlFieldProp)");
	}

	// 1. BGM Name
	const RwChar* pBGMName = dGET_BGM_MANAGER()->GetBGMSoundName(NtlFieldProp._IdxBGMTbl);
	if(pBGMName)
	{
		m_StrBGMName.Format("1. BGM Name\r\n- %s\r\n", pBGMName);
	}
	else
	{
		m_StrBGMName.Format("1. BGM Name\r\n- \"No BGM\"\r\n", pBGMName);
	}

	m_Result = m_StrBGMName;
	m_StrBGMName.Format("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\r\n");
	m_Result += m_StrBGMName;

	// Env Name
	const RwChar* pEnvName = dGET_BGM_MANAGER()->GetEnvSoundName(NtlFieldProp._IdxEnvTbl);
	if(pEnvName)
	{
		m_StrEnvName.Format("  Env Name\r\n- %s\r\n", pEnvName);
	}
	else
	{
		m_StrEnvName.Format("  Env Name\r\n- \"No Env\"\r\n", pEnvName);
	}

	m_Result += m_StrEnvName;
	m_StrEnvName.Format("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\r\n");
	m_Result += m_StrEnvName;

	// 2. Normal Map Prop.
	RwInt32 TilePropNum = (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2);
	RwInt32 NMPArray[16];
	DWORD	CurTileProp;

	for(RwInt32 i = 0; i < 16; ++i)
	{
		NMPArray[i] = 999;

		for(RwInt32 j = 0; j < TilePropNum; ++j)
		{
			CurTileProp = NtlFieldProp._pBitProp[j];

			if(CNtlWorldPropBaseTemplate::GetHexCode(i) & CurTileProp)
			{
				NMPArray[i] = i;
				break;
			}
		}
	}

	RwBool AtLeastOnce = FALSE;
	for(RwInt32 i = 0; i < 16; ++i)
	{
		if(NMPArray[i] != 999)
		{
			AtLeastOnce = TRUE;
			break;
		}
	}

	if(AtLeastOnce)
	{
		m_StrNMapSpec.Format("2. Map Property\r\n- ");
		m_Result += m_StrNMapSpec;

		for(RwInt32 i = 0; i < 16; ++i)
		{
			if(NMPArray[i] != 999)
			{
				m_StrNMapSpec.Format("%d ", NMPArray[i]);
				m_Result += m_StrNMapSpec;
			}
		}

		m_StrNMapSpec.Format("\r\n");
		m_Result += m_StrNMapSpec;
	}
	else
	{
		m_StrNMapSpec.Format("2. Map Property\r\n- \"No Property\"\r\n");
		m_Result += m_StrNMapSpec;
	}

	m_StrNMapSpec.Format("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\r\n");
	m_Result += m_StrNMapSpec;

	// 3. Map Name
	if(!_stricmp(NtlFieldProp._Name, "null") || !_stricmp(NtlFieldProp._Name, ""))
	{
		m_MapName.Format("3. Map Name\r\n- \"No Name\"\r\n");
		m_Result += m_MapName;
	}
	else
	{
		CTextTable*	pTextMobTbl	= static_cast<CTextTable*>(API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl());
		TBLIDX		TableIdx	= static_cast<TBLIDX>(atoi(NtlFieldProp._Name));
		std::wstring WStr		= pTextMobTbl->GetText(TableIdx);

		RwChar MBStr[256];
		::WideCharToMultiByte(GetACP(), 0, WStr.c_str(), -1, MBStr, 256, NULL, NULL);

		m_MapName.Format("3. Map Name\r\n- %s\r\n- %s\r\n", NtlFieldProp._Name, MBStr);
		m_Result += m_MapName;
	}

	m_MapName.Format("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\r\n");
	m_Result += m_MapName;

	// 4. Trigger Object
	CFileMem	FileMem;
	BYTE*		pPosCurFileMem;
	BYTE*		pTmp;
	RwInt32		TriggerObjCnt = 0;
	RwV3d		TriggerTrans;
	RwBool		TriggerEnable;
	RwUInt32	TriggerID;
	RwInt32		TriggerTotNum;

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);
	FileMem.Load(dGET_WORLD_PARAM()->WorldChar64Buf, "rb");
	pPosCurFileMem = FileMem.GetDat();

	m_Trigger.Format("4. Trigger Object\r\n");
	m_Result += m_Trigger;

	for(RwInt32 i = 0; i < dGET_WORLD_PARAM()->WorldFieldSectorNum * dGET_WORLD_PARAM()->WorldFieldSectorNum; ++i)
	{
		pPosCurFileMem = Skip_Material(pPosCurFileMem);
		pPosCurFileMem = Skip_Heightfield(pPosCurFileMem);
		pPosCurFileMem = Skip_Diffuse(pPosCurFileMem);

		CopyMemory(&TriggerTotNum, pPosCurFileMem, sizeof(RwInt32));
		pPosCurFileMem += sizeof(RwInt32);

		RwInt32 TriggerObjCnt = 0;

		pTmp = pPosCurFileMem;

		for(RwInt32 j = 0; j < TriggerTotNum; ++j)
		{
			// name
			pTmp += (sizeof(RwChar) * 128);

			// scaling
			pTmp += sizeof(RwV3d);

			// rotation
			pTmp += sizeof(RwV3d);

			// translation
			CopyMemory(&TriggerTrans, pTmp, sizeof(RwV3d));
			pTmp += sizeof(RwV3d);

			RwUInt32 uiObjectType;
			CopyMemory(&uiObjectType, pTmp, sizeof(RwUInt32));
			pTmp += sizeof(RwUInt32);

			if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
				uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
			{
				// Object Propperty Range
				RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
					(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
					sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
					(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);

				pTmp += iPropertyRange;
			}

			// trigger id
			CopyMemory(&TriggerID, pTmp, sizeof(RwUInt32));
			pTmp += sizeof(RwUInt32);

			// object name index
			pTmp += sizeof(DWORD);

			// visibility distance
			pTmp += sizeof(RwReal);

			// enable trigger
			CopyMemory(&TriggerEnable, pTmp, sizeof(RwBool));
			pTmp += sizeof(RwBool);

			if(TriggerEnable)
			{
				++TriggerObjCnt;
			}

			// enable selection
			pTmp += sizeof(RwBool);

			// enable alterable; dynamic obj.s.
			pTmp += sizeof(RwBool);

			// enable portal
			pTmp += sizeof(RwBool);

			// enable path engine collision
			pTmp += sizeof(RwBool);

			// 13. enable shadowprop
			pTmp += sizeof(RwBool);

			// PS Map Size
			RwInt32 PSAtomicCnt = 0;
			CopyMemory(&PSAtomicCnt, pTmp, sizeof(RwInt32));
			pTmp += sizeof(RwInt32);

			for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
			{
				// Map name
				pTmp += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

				// Frustum view window extent
				pTmp += (sizeof(RwReal));

				// Light matrices
				pTmp += (sizeof(RwMatrix));

				// Map size
				RwInt32 Size = 0;
				CopyMemory(&Size, pTmp, sizeof(RwInt32));
				pTmp += (sizeof(RwInt32));

				// Map
				pTmp += (sizeof(DWORD) * Size * Size);
			}

			// milepost
			RwBool EnableFlg;
			CopyMemory(&EnableFlg, pTmp, sizeof(RwBool));
			pTmp += sizeof(RwBool);

			if(EnableFlg)
			{
				pTmp += sizeof(RwUInt32);
			}

			// attach light object skip
			RwInt32 iLightObjectNum;
			CopyMemory(&iLightObjectNum, pTmp, sizeof(RwInt32));
			pTmp += sizeof(RwInt32);

			for (int j = 0; j < iLightObjectNum; ++j)
			{
				pTmp += sizeof(RwChar) * MAX_ATTR_BONE_NAME;

				pTmp += sizeof(RpLightType);
				pTmp += sizeof(RwBool);

				pTmp += sizeof(RwV3d);

				pTmp += sizeof(RwV3d);
				pTmp += sizeof(RwRGBAReal);
				pTmp += sizeof(RwReal);
				pTmp += sizeof(RwReal);

				pTmp += sizeof(RwBool);
				pTmp += sizeof(RwBool);
			}
		}

		m_Trigger.Format("- Counts at %d's Sector : [%d]\r\n", i, TriggerObjCnt);
		m_Result += m_Trigger;

		for(RwInt32 j = 0; j < TriggerTotNum; ++j)
		{
			// 1. name
			pPosCurFileMem += (sizeof(RwChar) * 128);

			// 2. scaling
			pPosCurFileMem += sizeof(RwV3d);

			// 3. rotation
			pPosCurFileMem += sizeof(RwV3d);

			// 4. translation
			CopyMemory(&TriggerTrans, pPosCurFileMem, sizeof(RwV3d));
			pPosCurFileMem += sizeof(RwV3d);

			RwUInt32 uiObjectType;
			CopyMemory(&uiObjectType, pPosCurFileMem, sizeof(RwUInt32));
			pPosCurFileMem += sizeof(RwUInt32);

			if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
				uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
			{
				// Object Propperty Range
				RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
					(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
					sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
					(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);

				pPosCurFileMem += iPropertyRange;
			}

			// 5. serial ID
			CopyMemory(&TriggerID, pPosCurFileMem, sizeof(RwUInt32));
			pPosCurFileMem += sizeof(RwUInt32);

			// 6. object name index
			pPosCurFileMem += sizeof(DWORD);

			// 7. visibility distance
			pPosCurFileMem += sizeof(RwReal);

			// 8. enable trigger
			CopyMemory(&TriggerEnable, pPosCurFileMem, sizeof(RwBool));
			pPosCurFileMem += sizeof(RwBool);

			// 9. enable selection
			pPosCurFileMem += sizeof(RwBool);

			// 10. enable alterable; dynamic obj.s.
			pPosCurFileMem += sizeof(RwBool);

			// 11. enable portal
			pPosCurFileMem += sizeof(RwBool);

			// 12. enable path engine collision
			pPosCurFileMem += sizeof(RwBool);

			// 13. enable shadowprop
			pPosCurFileMem += sizeof(RwBool);

			if(TriggerEnable)
			{
				m_Trigger.Format("   * %u, (%.2f, %.2f, %.2f)\r\n", TriggerID, TriggerTrans.x, TriggerTrans.y, TriggerTrans.z);
				m_Result += m_Trigger;
			}

			// PS Map Size
			RwInt32 PSAtomicCnt = 0;
			CopyMemory(&PSAtomicCnt, pPosCurFileMem, sizeof(RwInt32));
			pPosCurFileMem += sizeof(RwInt32);

			for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
			{
				// Map name
				pPosCurFileMem += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

				// Frustum view window extent
				pPosCurFileMem += (sizeof(RwReal));

				// Light matrices
				pPosCurFileMem += (sizeof(RwMatrix));

				// Map size
				RwInt32 Size = 0;
				CopyMemory(&Size, pPosCurFileMem, sizeof(RwInt32));
				pPosCurFileMem += (sizeof(RwInt32));

				// Map
				pPosCurFileMem += (sizeof(DWORD) * Size * Size);
			}

			// milepost
			RwBool EnableFlg;
			CopyMemory(&EnableFlg, pPosCurFileMem, sizeof(RwBool));
			pPosCurFileMem += sizeof(RwBool);

			if(EnableFlg)
			{
				pPosCurFileMem += sizeof(RwUInt32);
			}

			// attach light object skip
			RwInt32 iLightObjectNum;
			CopyMemory(&iLightObjectNum, pPosCurFileMem, sizeof(RwInt32));
			pPosCurFileMem += sizeof(RwInt32);

			for (int j = 0; j < iLightObjectNum; ++j)
			{
				pPosCurFileMem += sizeof(RwChar) * MAX_ATTR_BONE_NAME;

				pPosCurFileMem += sizeof(RpLightType);
				pPosCurFileMem += sizeof(RwBool);

				pPosCurFileMem += sizeof(RwV3d);

				pPosCurFileMem += sizeof(RwV3d);
				pPosCurFileMem += sizeof(RwRGBAReal);
				pPosCurFileMem += sizeof(RwReal);
				pPosCurFileMem += sizeof(RwReal);

				pPosCurFileMem += sizeof(RwBool);
				pPosCurFileMem += sizeof(RwBool);
			}
		}

		pPosCurFileMem = Skip_Water(pPosCurFileMem);
		pPosCurFileMem = Skip_Shadow(pPosCurFileMem);
		pPosCurFileMem = Skip_Effects(pPosCurFileMem);
		pPosCurFileMem = Skip_SE(pPosCurFileMem);
		// CZ-SS
		//pPosCurFileMem = Skip_SS(pPosCurFileMem);
		pPosCurFileMem = Skip_SectorCull(pPosCurFileMem);
		pPosCurFileMem = Skip_TileTransparency(pPosCurFileMem);
		pPosCurFileMem = Skip_Shoreline(pPosCurFileMem);
		pPosCurFileMem = Skip_Decal(pPosCurFileMem);
		pPosCurFileMem = Skip_Plant(pPosCurFileMem);
		pPosCurFileMem = Skip_WorldLight(pPosCurFileMem);
		pPosCurFileMem = Skip_Occluder(pPosCurFileMem);
		pPosCurFileMem = Skip_HeatHazeObject(pPosCurFileMem);
		pPosCurFileMem = Skip_LightObject(pPosCurFileMem);
		pPosCurFileMem = Skip_Dojo(pPosCurFileMem);
	}
	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	FileMem.Free();

	m_Trigger.Format("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\r\n");
	m_Result += m_Trigger;

	// 5. MOB Spawn
	m_MobSpawn.Format("5. MOB Spawn Object\r\n");
	m_Result += m_MobSpawn;

	RwInt32 SpawnCnt = GetSpawnInstance()->GetSpawnCnt(FieldIdx);
	m_MobSpawn.Format("- Total Counts : [%d]\r\n", SpawnCnt);
	m_Result += m_MobSpawn;

	m_MobSpawn.Format("ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ\r\n");
	m_Result += m_MobSpawn;

	// The last one to display
	m_Information.SetWindowText(m_Result.GetBuffer(0));
}


void CFieldsInformation::OnLbnSelchangeFiFields()
{
	RwInt32 IdxCurSel = m_Fields.GetCurSel();
	RwChar	StrFieldIdx[32];
	RwInt32	FieldSelIdx;

	if(m_Fields.GetText(IdxCurSel, StrFieldIdx) == LB_ERR)
	{
		NTL_ASSERTE("CFieldsInformation::OnLbnSelchangeFiFields");
	}
	else
	{
		FieldSelIdx = static_cast<RwInt32>(atoi(StrFieldIdx));
		RefreshInfo(FieldSelIdx);
	}

	UpdateData(FALSE);
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif