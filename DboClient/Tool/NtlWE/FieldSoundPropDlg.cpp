// FieldSoundPropDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "NtlWEDoc.h"

#include "NtlWorldFieldManager.h"

#include "NtlPLPropertyContainer.h"
#include "NtlPLSoundProp.h"

#include "NtlPLWorldEntity.h"

#include "FieldSoundPropDlg.h"

// CFieldSoundPropDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFieldSoundPropDlg, CDialog)

CFieldSoundPropDlg::CFieldSoundPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFieldSoundPropDlg::IDD, pParent)
{

}

CFieldSoundPropDlg::~CFieldSoundPropDlg()
{
}

void CFieldSoundPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BGM, m_cbBGMSound);
	DDX_Control(pDX, IDC_COMBO_ENV, m_cbENVSound);
	DDX_Control(pDX, IDC_COMBO_SHARE_0, m_cbShareSound[0]);
	DDX_Control(pDX, IDC_COMBO_SHARE_1, m_cbShareSound[1]);
	DDX_Control(pDX, IDC_COMBO_SHARE_2, m_cbShareSound[2]);
	DDX_Control(pDX, IDC_COMBO_SHARE_3, m_cbShareSound[3]);
	DDX_Control(pDX, IDC_COMBO_SHARE_4, m_cbShareSound[4]);
}


BEGIN_MESSAGE_MAP(CFieldSoundPropDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CFieldSoundPropDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CFieldSoundPropDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_BGM, &CFieldSoundPropDlg::OnBnClickedButtonExportBgm)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_ENV, &CFieldSoundPropDlg::OnBnClickedButtonExportEnv)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT_SHARE, &CFieldSoundPropDlg::OnBnClickedButtonExportShare)
END_MESSAGE_MAP()


// CFieldSoundPropDlg 메시지 처리기입니다.
BOOL CFieldSoundPropDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cbBGMSound.ResetContent();
	m_cbBGMSound.SetItemData(m_cbBGMSound.AddString("None"), -1);

	CNtlPLSoundFieldProp* pSoundFieldProp = NULL;
	
	pSoundFieldProp = static_cast<CNtlPLSoundFieldProp*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_SOUND, "FIELD_SOUND_BGM"));
	for (CNtlPLSoundFieldProp::MAP_SOUND_IT it = pSoundFieldProp->m_mapSound.begin(); it != pSoundFieldProp->m_mapSound.end(); ++it)
	{
		m_cbBGMSound.SetItemData(m_cbBGMSound.AddString(it->second.c_str()), it->first);
	}
	pSoundFieldProp = NULL;

	m_cbENVSound.ResetContent();
	m_cbENVSound.SetItemData(m_cbENVSound.AddString("None"), -1);

	pSoundFieldProp = static_cast<CNtlPLSoundFieldProp*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_SOUND, "FIELD_SOUND_ENV"));
	for (CNtlPLSoundFieldProp::MAP_SOUND_IT it = pSoundFieldProp->m_mapSound.begin(); it != pSoundFieldProp->m_mapSound.end(); ++it)
	{
		m_cbENVSound.SetItemData(m_cbENVSound.AddString(it->second.c_str()), it->first);
	}
	pSoundFieldProp = NULL;

	for (int i = 0; i < 5; ++i)
	{
		m_cbShareSound[i].ResetContent();
		m_cbShareSound[i].SetItemData(m_cbShareSound[i].AddString("None"), -1);

		pSoundFieldProp = static_cast<CNtlPLSoundFieldProp*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_SOUND, "FIELD_SOUND_SHARE"));
		for (CNtlPLSoundFieldProp::MAP_SOUND_IT it = pSoundFieldProp->m_mapSound.begin(); it != pSoundFieldProp->m_mapSound.end(); ++it)
		{
			m_cbShareSound[i].SetItemData(m_cbShareSound[i].AddString(it->second.c_str()), it->first);
		}
		pSoundFieldProp = NULL;
	}

	// Field Info Setting
	sNTL_FIELD_PROP				NtlFieldProp;
	CNtlWorldFieldManager*		pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();

	if (dGETDOC()->GetFieldProp(NtlFieldProp))
	{
		SetBGMData(NtlFieldProp._IdxBGMTbl);
		SetENVData(NtlFieldProp._IdxEnvTbl);
		SetShareData(0, NtlFieldProp._IdxShareTbl[0]);
		SetShareData(1, NtlFieldProp._IdxShareTbl[1]);
		SetShareData(2, NtlFieldProp._IdxShareTbl[2]);
		SetShareData(3, NtlFieldProp._IdxShareTbl[3]);
		SetShareData(4, NtlFieldProp._IdxShareTbl[4]);
	}
	else
	{
		SetBGMData(-1);
		SetENVData(-1);
		SetShareData(0, -1);
		SetShareData(1, -1);
		SetShareData(2, -1);
		SetShareData(3, -1);
		SetShareData(4, -1);
	}


	return TRUE;
}

void CFieldSoundPropDlg::SetBGMData(RwInt32 iIdx)
{
	RwInt32 iCnt =  m_cbBGMSound.GetCount();
	for (int i = 0; i < iCnt; ++i)
	{
		if (m_cbBGMSound.GetItemData(i) == iIdx)
		{
			m_cbBGMSound.SetCurSel(i);
			return;
		}
	}
	m_cbENVSound.SetCurSel(-1);
}

void CFieldSoundPropDlg::SetENVData(RwInt32 iIdx)
{
	RwInt32 iCnt = m_cbENVSound.GetCount();
	for (int i = 0; i < iCnt; ++i)
	{
		if (m_cbENVSound.GetItemData(i) == iIdx)
		{
			m_cbENVSound.SetCurSel(i);
			return;
		}
	}
	m_cbENVSound.SetCurSel(-1);
}

void CFieldSoundPropDlg::SetShareData(RwInt32 iArrayIdx, RwInt32 iIdx)
{
	RwInt32 iCnt =  m_cbShareSound[iArrayIdx].GetCount();
	for (int i = 0; i < iCnt; ++i)
	{
		if (m_cbShareSound[iArrayIdx].GetItemData(i) == iIdx)
		{
			m_cbShareSound[iArrayIdx].SetCurSel(i);
			return;
		}
	}
	m_cbShareSound[iArrayIdx].SetCurSel(-1);
}


void CFieldSoundPropDlg::OnBnClickedOk()
{
	if (m_cbBGMSound.GetCurSel() != -1)
	{		
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_IdxBGMTbl = m_cbBGMSound.GetItemData(m_cbBGMSound.GetCurSel());
	}
	else
	{
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_IdxBGMTbl = -1;
	}

	if (m_cbENVSound.GetCurSel() != -1)
	{		
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_IdxEnvTbl = m_cbENVSound.GetItemData(m_cbENVSound.GetCurSel());
	}
	else
	{
		static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_IdxEnvTbl = -1;
	}

	for (int i = 0; i < 5; ++i)
	{
		if (m_cbShareSound[i].GetCurSel() != -1)
		{
			static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_IdxShareTbl[i] = m_cbShareSound[i].GetItemData(m_cbShareSound[i].GetCurSel());;
		}
		else
		{
			static_cast<CNtlWEDoc*>(((CNtlWEApp*)(AfxGetApp()))->m_pDoc)->m_pNtlFieldProp->_IdxShareTbl[i] = -1;
		}
	}
		
	OnOK();
}

void CFieldSoundPropDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CFieldSoundPropDlg::OnBnClickedButtonExportBgm()
{
	RwInt32 iIndex = m_cbBGMSound.GetItemData(m_cbBGMSound.GetCurSel());

	ExportFieldSound(dFSP_EXPORT_SOUND_BGM, iIndex);
}

void CFieldSoundPropDlg::OnBnClickedButtonExportEnv()
{
	RwInt32 iIndex = m_cbENVSound.GetItemData(m_cbENVSound.GetCurSel());

	ExportFieldSound(dFSP_EXPORT_SOUND_ENV, iIndex);
}

void CFieldSoundPropDlg::OnBnClickedButtonExportShare()
{
	RwInt32 iIndex = m_cbShareSound[0].GetItemData(m_cbShareSound[0].GetCurSel());

	ExportFieldSound(dFSP_EXPORT_SOUND_SHARE, iIndex);
}

void CFieldSoundPropDlg::ExportFieldSound(RwUInt32 iExportType, RwInt32 iIdx)
{
	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		
	RwImage* pImage = RwImageCreate(dGET_WORLD_PARAM()->WorldFieldNum, dGET_WORLD_PARAM()->WorldFieldNum, 32);
	if(!pImage)
	{
		DBO_TRACE(FALSE, "CFieldSoundPropDlg::ExportFieldSound, image create failed.");
		return;
	}

	RwImageAllocatePixels(pImage);

	CNtlWorldFieldManager*	pFMgr	= dGETMGR();
	CNtlWorldField*			pFields	= const_cast<CNtlWorldField*>(pFMgr->GetFields());
	DWORD*					pBits	= (DWORD*)RwImageGetPixels(pImage);
	DWORD*					pDest;

	for(RwInt32 i = 0; i < dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum; ++i)
	{		
		RwInt32 iFieldIdx	= dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum - 1 - i;

		RwV3d vPos;
		vPos.x	= pFields[iFieldIdx].GetSPos().x + dGET_WORLD_PARAM()->WorldSectorSize;
		vPos.z	= pFields[iFieldIdx].GetSPos().z + dGET_WORLD_PARAM()->WorldSectorSize;
		pDest	= pBits + i;

		sNTL_FIELD_PROP sNtlFieldProp;
		if (pFMgr->GetAFieldProp(vPos, sNtlFieldProp))
		{			
			RwBool bCheck;
			RwBool bExist;
			RwBool bNULL;
							
			switch (iExportType)
			{
			case dFSP_EXPORT_SOUND_BGM:
				{
					CNtlPLSoundFieldProp* pSoundFieldProp = static_cast<CNtlPLSoundFieldProp*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_SOUND, "FIELD_SOUND_BGM"));

					bNULL	= (sNtlFieldProp._IdxBGMTbl == -1);
					bExist	= (sNtlFieldProp._IdxBGMTbl == -1 || (pSoundFieldProp->m_mapSound.find(sNtlFieldProp._IdxBGMTbl) != pSoundFieldProp->m_mapSound.end()));
					bCheck	= (sNtlFieldProp._IdxBGMTbl == iIdx);

					break;
				}
			case dFSP_EXPORT_SOUND_ENV:
				{
					CNtlPLSoundFieldProp* pSoundFieldProp = static_cast<CNtlPLSoundFieldProp*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_SOUND, "FIELD_SOUND_ENV"));
					
					bNULL	= (sNtlFieldProp._IdxEnvTbl == -1);
					bExist	= (sNtlFieldProp._IdxEnvTbl == -1 || (pSoundFieldProp->m_mapSound.find(sNtlFieldProp._IdxEnvTbl) != pSoundFieldProp->m_mapSound.end()));
					bCheck	= (sNtlFieldProp._IdxEnvTbl == iIdx);

					break;
				}
			case dFSP_EXPORT_SOUND_SHARE:
				{
					CNtlPLSoundFieldProp* pSoundFieldProp = static_cast<CNtlPLSoundFieldProp*>(CNtlPLPropertyContainer::GetInstance()->GetProperty(PLENTITY_SOUND, "FIELD_SOUND_SHARE"));

					bCheck	= FALSE;
					bExist	= TRUE;
					bNULL	= TRUE;
					for (int i = 0; i < 5; ++i)
					{
						bNULL	&= (sNtlFieldProp._IdxShareTbl[i] == -1);
						bExist	&= (sNtlFieldProp._IdxShareTbl[i] == -1 || (pSoundFieldProp->m_mapSound.find(sNtlFieldProp._IdxEnvTbl) != pSoundFieldProp->m_mapSound.end()));
						bCheck	|= (sNtlFieldProp._IdxShareTbl[i] == iIdx);
					}

					break;
				}
			}

			if (bCheck)
			{
				(*pDest) = (DWORD)(0 << 24) | (DWORD)(255 << 16) | (DWORD)(0 << 8) | (DWORD)(0 << 0);
			}
			else if (bNULL)
			{
				(*pDest) = (DWORD)(0 << 24) | (DWORD)(255 << 16) | (DWORD)(255 << 8) | (DWORD)(255 << 0);
			}
			else if (bExist)
			{
				(*pDest) = (DWORD)(0 << 24) | (DWORD)(0 << 16) | (DWORD)(255 << 8) | (DWORD)(0 << 0);
			}					
			else 
			{
				(*pDest) = (DWORD)(0 << 24) | (DWORD)(0 << 16) | (DWORD)(0 << 8) | (DWORD)(255 << 0);
			}
		}
		else
		{
			(*pDest) = (DWORD)(0 << 24) | (DWORD)(0 << 16) | (DWORD)(0 << 8) | (DWORD)(0 << 0);
		}
	}

	RwImageWrite(pImage, "tool\\saber\\texture\\grid map\\FieldSound.bmp");
	RwImageDestroy(pImage);

	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	AfxMessageBox("Done.");
}
