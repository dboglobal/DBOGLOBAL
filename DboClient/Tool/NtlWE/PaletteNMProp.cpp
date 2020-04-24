// PaletteNMProp.cpp : implementation file
//

#include "stdafx.h"
#include "NtlWE.h"

#include "TableContainer.h"
#include "TextAllTable.h"
#include "WorldTable.h"

#include "NtlWorldPaletteDefine.h"

#include "NtlWorldPropNMapController.h"

#include "NtlWorldFieldManager.h"

#include "NtlPLWorldEntity.h"

#include "NtlSLApi.h"

#include "PaletteNMProp.h"


// CPaletteNMProp dialog

IMPLEMENT_DYNAMIC(CPaletteNMProp, CDialog)

CPaletteNMProp::CPaletteNMProp(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteNMProp::IDD, pParent)
	, m_TName00(_T("null"))
	, m_TName01(_T("null"))
	, m_TName02(_T("null"))
	, m_TName03(_T("null"))
	, m_TName04(_T("null"))
	, m_TName05(_T("null"))
	, m_TName06(_T("null"))
	, m_TName07(_T("null"))
	, m_TName08(_T("null"))
	, m_TName09(_T("null"))
	, m_TName10(_T("null"))
	, m_TName11(_T("null"))
	, m_TName12(_T("null"))
	, m_TName13(_T("null"))
	, m_TName14(_T("null"))
	, m_TName15(_T("null"))
{
	ZeroMemory(m_NMapProp, sizeof(RwBool) * 16);

	m_pController = NTL_NEW CNtlWorldPropNMapController;
}

CPaletteNMProp::~CPaletteNMProp()
{
	NTL_DELETE(m_pController);
}

void CPaletteNMProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NMAPPROP00, m_PropChk00);
	DDX_Control(pDX, IDC_NMAPPROP01, m_PropChk01);
	DDX_Control(pDX, IDC_NMAPPROP02, m_PropChk02);
	DDX_Control(pDX, IDC_NMAPPROP03, m_PropChk03);
	DDX_Control(pDX, IDC_NMAPPROP04, m_PropChk04);
	DDX_Control(pDX, IDC_NMAPPROP05, m_PropChk05);
	DDX_Control(pDX, IDC_NMAPPROP06, m_PropChk06);
	DDX_Control(pDX, IDC_NMAPPROP07, m_PropChk07);
	DDX_Control(pDX, IDC_NMAPPROP08, m_PropChk08);
	DDX_Control(pDX, IDC_NMAPPROP09, m_PropChk09);
	DDX_Control(pDX, IDC_NMAPPROP10, m_PropChk10);
	DDX_Control(pDX, IDC_NMAPPROP11, m_PropChk11);
	DDX_Control(pDX, IDC_NMAPPROP12, m_PropChk12);
	DDX_Control(pDX, IDC_NMAPPROP13, m_PropChk13);
	DDX_Control(pDX, IDC_NMAPPROP14, m_PropChk14);
	DDX_Control(pDX, IDC_NMAPPROP15, m_PropChk15);
	DDX_Text(pDX, IDC_NMP_TNAME_00, m_TName00);
	DDX_Text(pDX, IDC_NMP_TNAME_01, m_TName01);
	DDX_Text(pDX, IDC_NMP_TNAME_02, m_TName02);
	DDX_Text(pDX, IDC_NMP_TNAME_03, m_TName03);
	DDX_Text(pDX, IDC_NMP_TNAME_04, m_TName04);
	DDX_Text(pDX, IDC_NMP_TNAME_05, m_TName05);
	DDX_Text(pDX, IDC_NMP_TNAME_06, m_TName06);
	DDX_Text(pDX, IDC_NMP_TNAME_07, m_TName07);
	DDX_Text(pDX, IDC_NMP_TNAME_08, m_TName08);
	DDX_Text(pDX, IDC_NMP_TNAME_09, m_TName09);
	DDX_Text(pDX, IDC_NMP_TNAME_10, m_TName10);
	DDX_Text(pDX, IDC_NMP_TNAME_11, m_TName11);
	DDX_Text(pDX, IDC_NMP_TNAME_12, m_TName12);
	DDX_Text(pDX, IDC_NMP_TNAME_13, m_TName13);
	DDX_Text(pDX, IDC_NMP_TNAME_14, m_TName14);
	DDX_Text(pDX, IDC_NMP_TNAME_15, m_TName15);
}


BEGIN_MESSAGE_MAP(CPaletteNMProp, CDialog)
	ON_BN_CLICKED(IDC_NMP_LOAD00, &CPaletteNMProp::OnBnClickedNmpLoad00)
	ON_BN_CLICKED(IDC_NMP_LOAD01, &CPaletteNMProp::OnBnClickedNmpLoad01)
	ON_BN_CLICKED(IDC_NMP_LOAD02, &CPaletteNMProp::OnBnClickedNmpLoad02)
	ON_BN_CLICKED(IDC_NMP_LOAD03, &CPaletteNMProp::OnBnClickedNmpLoad03)
	ON_BN_CLICKED(IDC_NMP_LOAD04, &CPaletteNMProp::OnBnClickedNmpLoad04)
	ON_BN_CLICKED(IDC_NMP_LOAD05, &CPaletteNMProp::OnBnClickedNmpLoad05)
	ON_BN_CLICKED(IDC_NMP_LOAD06, &CPaletteNMProp::OnBnClickedNmpLoad06)
	ON_BN_CLICKED(IDC_NMP_LOAD07, &CPaletteNMProp::OnBnClickedNmpLoad07)
	ON_BN_CLICKED(IDC_NMP_LOAD08, &CPaletteNMProp::OnBnClickedNmpLoad08)
	ON_BN_CLICKED(IDC_NMP_LOAD09, &CPaletteNMProp::OnBnClickedNmpLoad09)
	ON_BN_CLICKED(IDC_NMP_LOAD10, &CPaletteNMProp::OnBnClickedNmpLoad10)
	ON_BN_CLICKED(IDC_NMP_LOAD11, &CPaletteNMProp::OnBnClickedNmpLoad11)
	ON_BN_CLICKED(IDC_NMP_LOAD12, &CPaletteNMProp::OnBnClickedNmpLoad12)
	ON_BN_CLICKED(IDC_NMP_LOAD13, &CPaletteNMProp::OnBnClickedNmpLoad13)
	ON_BN_CLICKED(IDC_NMP_LOAD14, &CPaletteNMProp::OnBnClickedNmpLoad14)
	ON_BN_CLICKED(IDC_NMP_LOAD15, &CPaletteNMProp::OnBnClickedNmpLoad15)
	//ON_BN_CLICKED(IDC_NMP_INIT, &CPaletteNMProp::OnBnClickedNmpInit)
	ON_BN_CLICKED(IDC_NMAPPROP00, &CPaletteNMProp::OnBnClickedNmapprop00)
	ON_BN_CLICKED(IDC_NMAPPROP01, &CPaletteNMProp::OnBnClickedNmapprop01)
	ON_BN_CLICKED(IDC_NMAPPROP02, &CPaletteNMProp::OnBnClickedNmapprop02)
	ON_BN_CLICKED(IDC_NMAPPROP03, &CPaletteNMProp::OnBnClickedNmapprop03)
	ON_BN_CLICKED(IDC_NMAPPROP04, &CPaletteNMProp::OnBnClickedNmapprop04)
	ON_BN_CLICKED(IDC_NMAPPROP05, &CPaletteNMProp::OnBnClickedNmapprop05)
	ON_BN_CLICKED(IDC_NMAPPROP06, &CPaletteNMProp::OnBnClickedNmapprop06)
	ON_BN_CLICKED(IDC_NMAPPROP07, &CPaletteNMProp::OnBnClickedNmapprop07)
	ON_BN_CLICKED(IDC_NMAPPROP08, &CPaletteNMProp::OnBnClickedNmapprop08)
	ON_BN_CLICKED(IDC_NMAPPROP09, &CPaletteNMProp::OnBnClickedNmapprop09)
	ON_BN_CLICKED(IDC_NMAPPROP10, &CPaletteNMProp::OnBnClickedNmapprop10)
	ON_BN_CLICKED(IDC_NMAPPROP11, &CPaletteNMProp::OnBnClickedNmapprop11)
	ON_BN_CLICKED(IDC_NMAPPROP12, &CPaletteNMProp::OnBnClickedNmapprop12)
	ON_BN_CLICKED(IDC_NMAPPROP13, &CPaletteNMProp::OnBnClickedNmapprop13)
	ON_BN_CLICKED(IDC_NMAPPROP14, &CPaletteNMProp::OnBnClickedNmapprop14)
	ON_BN_CLICKED(IDC_NMAPPROP15, &CPaletteNMProp::OnBnClickedNmapprop15)
	ON_BN_CLICKED(IDC_EXPORT_NMP, &CPaletteNMProp::OnBnClickedExportNmp)
END_MESSAGE_MAP()


// CPaletteNMProp message handlers

void CPaletteNMProp::LoadPropTex(RwInt32 IdxCurProp)
{
	static bool Floating = false;

	if(Floating)
	{
		return;
	}

	Floating = true;

	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(TRUE,
						"load normal map property bmp textures.",
						"*.png",
						OFN_HIDEREADONLY,
						"Data Files (*.png)|*.png||");

	FileDlg.m_ofn.lpstrInitialDir = "tool\\SABER\\Texture\\NP texture";

	if(FileDlg.DoModal() == IDCANCEL)
	{
		Floating = false;
		::SetCurrentDirectory(dirBackup);
		return;
	}

	::SetCurrentDirectory(dirBackup);

	m_pController->LoadPropImage(FileDlg.GetFileName().GetBuffer(0)/*FileDlg.GetFileTitle().GetBuffer()*/, IdxCurProp);

	// refresh fields
	CNtlWorldFieldManager*	pFieldMgr = dGETMGR();

	for(RwUInt32 i = 0; i < 36; ++i)
	{
		if(pFieldMgr->m_Fields6x6[1][i] != -1)
		{
			pFieldMgr->RefreshField(pFieldMgr->m_Fields6x6[1][i]);
		}
	}

	Floating = false;
}

void CPaletteNMProp::OnBnClickedNmpLoad00()	{	LoadPropTex(0);		}
void CPaletteNMProp::OnBnClickedNmpLoad01()	{	LoadPropTex(1);		}
void CPaletteNMProp::OnBnClickedNmpLoad02()	{	LoadPropTex(2);		}
void CPaletteNMProp::OnBnClickedNmpLoad03()	{	LoadPropTex(3);		}
void CPaletteNMProp::OnBnClickedNmpLoad04()	{	LoadPropTex(4);		}
void CPaletteNMProp::OnBnClickedNmpLoad05()	{	LoadPropTex(5);		}
void CPaletteNMProp::OnBnClickedNmpLoad06()	{	LoadPropTex(6);		}
void CPaletteNMProp::OnBnClickedNmpLoad07()	{	LoadPropTex(7);		}
void CPaletteNMProp::OnBnClickedNmpLoad08()	{	LoadPropTex(8);		}
void CPaletteNMProp::OnBnClickedNmpLoad09()	{	LoadPropTex(9);		}
void CPaletteNMProp::OnBnClickedNmpLoad10()	{	LoadPropTex(10);	}
void CPaletteNMProp::OnBnClickedNmpLoad11()	{	LoadPropTex(11);	}
void CPaletteNMProp::OnBnClickedNmpLoad12()	{	LoadPropTex(12);	}
void CPaletteNMProp::OnBnClickedNmpLoad13()	{	LoadPropTex(13);	}
void CPaletteNMProp::OnBnClickedNmpLoad14()	{	LoadPropTex(14);	}
void CPaletteNMProp::OnBnClickedNmpLoad15()	{	LoadPropTex(15);	}

void CPaletteNMProp::OnBnClickedNmpInit()
{
	// TODO: Add your control notification handler code here
}

void CPaletteNMProp::InitializeWorldData()
{
	CNtlWorldPropToolData* pWorldPropToolData = &CNtlWEApplication::GetInstance()->m_WorldPropToolData;

	m_TName00 = pWorldPropToolData->GetPropName(0);
	m_TName01 = pWorldPropToolData->GetPropName(1);
	m_TName02 = pWorldPropToolData->GetPropName(2);
	m_TName03 = pWorldPropToolData->GetPropName(3);
	m_TName04 = pWorldPropToolData->GetPropName(4);
	m_TName05 = pWorldPropToolData->GetPropName(5);
	m_TName06 = pWorldPropToolData->GetPropName(6);
	m_TName07 = pWorldPropToolData->GetPropName(7);
	m_TName08 = pWorldPropToolData->GetPropName(8);
	m_TName09 = pWorldPropToolData->GetPropName(9);
	m_TName10 = pWorldPropToolData->GetPropName(10);
	m_TName11 = pWorldPropToolData->GetPropName(11);
	m_TName12 = pWorldPropToolData->GetPropName(12);
	m_TName13 = pWorldPropToolData->GetPropName(13);
	m_TName14 = pWorldPropToolData->GetPropName(14);
	m_TName15 = pWorldPropToolData->GetPropName(15);

	UpdateData(FALSE);
}

BOOL CPaletteNMProp::PreTranslateMessage(MSG* pMsg)
{
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
				//dMAINFRM->m_pView->SetFocus();
				//return 1;
				break;
			}
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CPaletteNMProp::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		{
			dGET_CUR_PALETTE() = ePM_DEFAULT;
			dGETHDL()->DeleteAllPaletteDummies();
			break;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

VOID CPaletteNMProp::UpdateNMapPropData()
{
	if(!GetNMapPropActivated())
	{
		dGETHDL()->m_vecTileAttr.clear();
	}
	else
	{
		dGETHDL()->UpdateNMPropTile();
	}
}

RwBool CPaletteNMProp::GetNMapPropActivated()
{
	if(m_PropChk00.GetCheck() ||
	   m_PropChk01.GetCheck() ||
	   m_PropChk02.GetCheck() ||
	   m_PropChk03.GetCheck() ||
	   m_PropChk04.GetCheck() ||
	   m_PropChk05.GetCheck() ||
	   m_PropChk06.GetCheck() ||
	   m_PropChk07.GetCheck() ||
	   m_PropChk08.GetCheck() ||
	   m_PropChk09.GetCheck() ||
	   m_PropChk10.GetCheck() ||
	   m_PropChk11.GetCheck() ||
	   m_PropChk12.GetCheck() ||
	   m_PropChk13.GetCheck() ||
	   m_PropChk14.GetCheck() ||
	   m_PropChk15.GetCheck())
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

	return FALSE;
}

DWORD CPaletteNMProp::GetCurProp()
{
	DWORD CurProp = 0x00000000;

	if(m_NMapProp[0])	{	CurProp |= 0x00000001;	}
	if(m_NMapProp[1])	{	CurProp |= 0x00000002;	}
	if(m_NMapProp[2])	{	CurProp |= 0x00000004;	}
	if(m_NMapProp[3])	{	CurProp |= 0x00000008;	}
	if(m_NMapProp[4])	{	CurProp |= 0x00000010;	}
	if(m_NMapProp[5])	{	CurProp |= 0x00000020;	}
	if(m_NMapProp[6])	{	CurProp |= 0x00000040;	}
	if(m_NMapProp[7])	{	CurProp |= 0x00000080;	}
	if(m_NMapProp[8])	{	CurProp |= 0x00000100;	}
	if(m_NMapProp[9])	{	CurProp |= 0x00000200;	}
	if(m_NMapProp[10])	{	CurProp |= 0x00000400;	}
	if(m_NMapProp[11])	{	CurProp |= 0x00000800;	}
	if(m_NMapProp[12])	{	CurProp |= 0x00001000;	}
	if(m_NMapProp[13])	{	CurProp |= 0x00002000;	}
	if(m_NMapProp[14])	{	CurProp |= 0x00004000;	}
	if(m_NMapProp[15])	{	CurProp |= 0x00008000;	}

	return CurProp;
}

RwBool CPaletteNMProp::IsMatched(DWORD& _BitProp)
{
	RwBool NoCheck = TRUE;

	for(int i = 0; i < 16; ++i)
	{
		if(m_NMapProp[i])
		{
			NoCheck = FALSE;
		}
	}

	if(NoCheck)
	{
		return FALSE;
	}

	if(m_NMapProp[0])
	{
		if(!(_BitProp & 0x00000001)) {	return FALSE; }
	}
	if(m_NMapProp[1])
	{
		if(!(_BitProp & 0x00000002)) {	return FALSE; }
	}
	if(m_NMapProp[2])
	{
		if(!(_BitProp & 0x00000004)) {	return FALSE; }
	}
	if(m_NMapProp[3])
	{
		if(!(_BitProp & 0x00000008)) {	return FALSE; }
	}
	if(m_NMapProp[4])
	{
		if(!(_BitProp & 0x00000010)) {	return FALSE; }
	}
	if(m_NMapProp[5])
	{
		if(!(_BitProp & 0x00000020)) {	return FALSE; }
	}
	if(m_NMapProp[6])
	{
		if(!(_BitProp & 0x00000040)) {	return FALSE; }
	}
	if(m_NMapProp[7])
	{
		if(!(_BitProp & 0x00000080)) {	return FALSE; }
	}
	if(m_NMapProp[8])
	{
		if(!(_BitProp & 0x00000100)) {	return FALSE; }
	}
	if(m_NMapProp[9])
	{
		if(!(_BitProp & 0x00000200)) {	return FALSE; }
	}
	if(m_NMapProp[10])
	{
		if(!(_BitProp & 0x00000400)) {	return FALSE; }
	}
	if(m_NMapProp[11])
	{
		if(!(_BitProp & 0x00000800)) {	return FALSE; }
	}
	if(m_NMapProp[12])
	{
		if(!(_BitProp & 0x00001000)) {	return FALSE; }
	}
	if(m_NMapProp[13])
	{
		if(!(_BitProp & 0x00002000)) {	return FALSE; }
	}
	if(m_NMapProp[14])
	{
		if(!(_BitProp & 0x00004000)) {	return FALSE; }
	}
	if(m_NMapProp[15])
	{
		if(!(_BitProp & 0x00008000)) {	return FALSE; }
	}

	return TRUE;
}

void CPaletteNMProp::OnBnClickedNmapprop00()	{ m_NMapProp[0]		= m_PropChk00.GetCheck();	UpdateNMapPropData(); }
void CPaletteNMProp::OnBnClickedNmapprop01()	{ m_NMapProp[1]		= m_PropChk01.GetCheck();	UpdateNMapPropData(); }
void CPaletteNMProp::OnBnClickedNmapprop02()	{ m_NMapProp[2]		= m_PropChk02.GetCheck();	UpdateNMapPropData(); }
void CPaletteNMProp::OnBnClickedNmapprop03()	{ m_NMapProp[3]		= m_PropChk03.GetCheck();	UpdateNMapPropData(); }
void CPaletteNMProp::OnBnClickedNmapprop04()	{ m_NMapProp[4]		= m_PropChk04.GetCheck();	UpdateNMapPropData(); }
void CPaletteNMProp::OnBnClickedNmapprop05()	{ m_NMapProp[5]		= m_PropChk05.GetCheck();	UpdateNMapPropData(); }
void CPaletteNMProp::OnBnClickedNmapprop06()	{ m_NMapProp[6]		= m_PropChk06.GetCheck();	UpdateNMapPropData(); }
void CPaletteNMProp::OnBnClickedNmapprop07()	{ m_NMapProp[7]		= m_PropChk07.GetCheck();	UpdateNMapPropData(); }
void CPaletteNMProp::OnBnClickedNmapprop08()	{ m_NMapProp[8]		= m_PropChk08.GetCheck();	UpdateNMapPropData(); }
void CPaletteNMProp::OnBnClickedNmapprop09()	{ m_NMapProp[9]		= m_PropChk09.GetCheck();	UpdateNMapPropData(); }
void CPaletteNMProp::OnBnClickedNmapprop10()	{ m_NMapProp[10]	= m_PropChk10.GetCheck();	UpdateNMapPropData(); }
void CPaletteNMProp::OnBnClickedNmapprop11()	{ m_NMapProp[11]	= m_PropChk11.GetCheck();	UpdateNMapPropData(); }
void CPaletteNMProp::OnBnClickedNmapprop12()	{ m_NMapProp[12]	= m_PropChk12.GetCheck();	UpdateNMapPropData(); }
void CPaletteNMProp::OnBnClickedNmapprop13()	{ m_NMapProp[13]	= m_PropChk13.GetCheck();	UpdateNMapPropData(); }
void CPaletteNMProp::OnBnClickedNmapprop14()	{ m_NMapProp[14]	= m_PropChk14.GetCheck();	UpdateNMapPropData(); }
void CPaletteNMProp::OnBnClickedNmapprop15()	{ m_NMapProp[15]	= m_PropChk15.GetCheck();	UpdateNMapPropData(); }

void CPaletteNMProp::OnBnClickedExportNmp()
{
	// multi checks
	RwBool	FlgEnable	= TRUE;
	RwInt32	CntCheck	= 0;
	RwInt32 IdxProp;

	for(int i = 0; i < 16; ++i)
	{
		if(m_NMapProp[i])
		{
			CntCheck++;
			IdxProp = i;
		}
	}

	if(CntCheck != 1)
	{
		FlgEnable = FALSE;
	}

	if(FlgEnable)
	{
		CString CurPropName;
		RwChar	IdxPropName[128]	= {0,};
		RwInt32 NumTileInWorld		= dGET_WORLD_PARAM()->WorldFieldSectorNum * dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldSectorTileNum;

		if(IdxProp == 0)	CurPropName = m_TName00;
		if(IdxProp == 1)	CurPropName = m_TName01;
		if(IdxProp == 2)	CurPropName = m_TName02;
		if(IdxProp == 3)	CurPropName = m_TName03;
		if(IdxProp == 4)	CurPropName = m_TName04;
		if(IdxProp == 5)	CurPropName = m_TName05;
		if(IdxProp == 6)	CurPropName = m_TName06;
		if(IdxProp == 7)	CurPropName = m_TName07;
		if(IdxProp == 8)	CurPropName = m_TName08;
		if(IdxProp == 9)	CurPropName = m_TName09;
		if(IdxProp == 10)	CurPropName = m_TName00;
		if(IdxProp == 11)	CurPropName = m_TName01;
		if(IdxProp == 12)	CurPropName = m_TName02;
		if(IdxProp == 13)	CurPropName = m_TName03;
		if(IdxProp == 14)	CurPropName = m_TName04;
		if(IdxProp == 15)	CurPropName = m_TName05;
		if(IdxProp == 16)	CurPropName = m_TName06;

		std::vector<std::string>	vecWorldName;
		CWorldTable*				pTable = static_cast<CWorldTable*>(API_GetTableContainer()->GetWorldTable());

		CTable::TABLEIT	it = pTable->Begin();
		while (it != pTable->End())
		{
			RwUInt32		uiIndex		= it->first;
			sWORLD_TBLDAT*	pTableData	= static_cast<sWORLD_TBLDAT*>(it->second);

			std::vector<std::string>::iterator itWorldName = std::find(vecWorldName.begin(), vecWorldName.end(), pTableData->szName);
			if (itWorldName == vecWorldName.end())
			{
				vecWorldName.push_back(pTableData->szName);
			}
			it++;
		}

		CString strWorldProjNameOnly = dGET_WORLD_PARAM()->WorldProjectFolderName;
		strWorldProjNameOnly.Delete(strWorldProjNameOnly.GetLength() - 1, 1);
		strWorldProjNameOnly.Delete(0, strWorldProjNameOnly.ReverseFind('\\') + 1);

		RwBool bIsIn = FALSE;
		for (int i = 0; i < (int)vecWorldName.size(); ++i)
		{
			CString strSrc = strWorldProjNameOnly;
			CString strCmp = vecWorldName.at(i).c_str();

			RwInt32 iLenSrc = strSrc.GetLength();
			RwInt32 iLenCmp = strCmp.GetLength();

			RwInt32 iPos = strSrc.Find(strCmp.GetBuffer());
			while (iPos != -1 && iPos <= iLenSrc - iLenCmp)
			{
				if (strSrc.Mid(iPos, iLenCmp) == strCmp)
				{
					bIsIn = TRUE;
					break;
				}
				iPos = strSrc.Find(strCmp.GetBuffer(), iPos);
			}
			if (bIsIn)
			{
				strWorldProjNameOnly = strCmp;
				break;
			}
		}	

		// 09.08.10 그래픽팀 요청으로 화일명 변경.
		sprintf_s(IdxPropName, "Property%.2d_%s.png", IdxProp, strWorldProjNameOnly.GetBuffer());
		//sprintf_s(IdxPropName, "Property0%d_Exported_%s.png", IdxProp, CurPropName.GetBuffer(0));

		m_pController->SavePropImage(NumTileInWorld, NumTileInWorld, 32, IdxProp, IdxPropName);
	}
	else
	{
		::AfxMessageBox("Only one channel can be exported.", MB_OK);

		return;
	}
}
