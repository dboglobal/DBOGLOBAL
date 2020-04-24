////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "precomp_ntlpresentation.h"
#include "NtlWorldShadow.h"
#include "NtlMath.h"
#include "ntlworldcommon.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


CNtlWorldShadow::CNtlWorldShadow(void)
{
	m_pTexShadow		= NULL;
	m_rgbaShadow.alpha	= 255;
	m_rgbaShadow.red	= 128;
	m_rgbaShadow.green	= 128;
	m_rgbaShadow.blue	= 128;
	
#ifdef dNTL_WORLD_TOOL_MODE

	m_pNtlWorldShadowParam4Doodads = NULL;

#endif
}

CNtlWorldShadow::~CNtlWorldShadow(void)
{
	if(m_pTexShadow)
	{
		RwTextureDestroy(m_pTexShadow);
		m_pTexShadow = NULL;
	}

#ifdef dNTL_WORLD_TOOL_MODE

	NTL_DELETE(m_pNtlWorldShadowParam4Doodads);

#endif
}

RwBool CNtlWorldShadow::Create(RwInt32 _NtlWorldDir)
{
	RwMatrixRotate(&m_matLight, &XAxis, 50.0f, rwCOMBINEREPLACE);
	RwMatrixRotate(&m_matLight, &YAxis, -45.0f * _NtlWorldDir, rwCOMBINEPOSTCONCAT);
	RwMatrixUpdate(&m_matLight);

	return TRUE;
}

RwBool CNtlWorldShadow::Delete()
{
	if(m_pTexShadow)
	{
		RwTextureDestroy(m_pTexShadow);
		m_pTexShadow = NULL;
	}

	return TRUE;
}

void CNtlWorldShadow::Update()
{
	if(!m_pTexShadow)
		return;

	RwMatrix    MatFixedCam;
	RwMatrix    MatInvLight;
	RwV3d       Scaling;
	RwV3d		Translation;

	RwMatrixCopy(&MatFixedCam, RwFrameGetLTM(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	MatFixedCam.right.x = -MatFixedCam.right.x;
	MatFixedCam.right.y = -MatFixedCam.right.y;
	MatFixedCam.right.z = -MatFixedCam.right.z;

	RwMatrixInvert(&MatInvLight, &m_matLight);

	Scaling.x = Scaling.y = -0.5f / m_CamViewWindowX;
	Scaling.z = 1.0f / (m_CamViewWindowX + m_CamViewWindowX);
	RwMatrixScale(&MatInvLight, &Scaling, rwCOMBINEPOSTCONCAT);

	Translation.x = Translation.y = 0.5f;
	Translation.z = 0.0f;
	RwMatrixTranslate(&MatInvLight, &Translation, rwCOMBINEPOSTCONCAT);

	RwMatrixMultiply(&m_matTex, &MatFixedCam, &MatInvLight);

	((D3DMATRIX*)&m_matTex)->_14 = 0.0f;
	((D3DMATRIX*)&m_matTex)->_24 = 0.0f;
	((D3DMATRIX*)&m_matTex)->_34 = 0.0f;
	((D3DMATRIX*)&m_matTex)->_44 = 1.0f;
}

#ifdef dNTL_WORLD_TOOL_MODE

void CNtlWorldShadow::SaveSwapFile(RwInt32 _SectorIdx)
{
	::_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	::_chdir("swap");
	::sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.sdw", _SectorIdx);

	FILE* pFile;
	if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "wb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		return;
	}

	::fwrite(&m_NtlWorldShadowParam, sizeof(sNTL_WORLD_SHADOW_PARAM), 1, pFile);

	// doodad shadow
	RwBool DoodadPSMapExist = FALSE;
	if(m_pNtlWorldShadowParam4Doodads)
	{
		DoodadPSMapExist = TRUE;
		::fwrite(&DoodadPSMapExist, sizeof(RwBool), 1, pFile);
		::fwrite(m_pNtlWorldShadowParam4Doodads, sizeof(sNTL_WORLD_SHADOW_PARAM), 1, pFile);
	}
	else
	{
		DoodadPSMapExist = FALSE;
		::fwrite(&DoodadPSMapExist, sizeof(RwBool), 1, pFile);
	}

	::fclose(pFile);
	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
}

void CNtlWorldShadow::LoadSwapFile(RwInt32 _SectorIdx)
{
	::_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	::_chdir("swap");
	::sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.sdw", _SectorIdx);

	FILE* pFile;
	if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		return;
	}
	
	::fread(&m_NtlWorldShadowParam, sizeof(sNTL_WORLD_SHADOW_PARAM), 1, pFile);

	// doodad shadow
	RwBool DoodadPSMapExist = FALSE;
	::fread(&DoodadPSMapExist, sizeof(RwBool), 1, pFile);

	if(DoodadPSMapExist)
	{
		m_pNtlWorldShadowParam4Doodads = NTL_NEW sNTL_WORLD_SHADOW_PARAM;
		::fread(m_pNtlWorldShadowParam4Doodads, sizeof(sNTL_WORLD_SHADOW_PARAM), 1, pFile);
	}

	::fclose(pFile);
	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
}

void CNtlWorldShadow::UpdateSParam(sNTL_WORLD_SHADOW_PARAM& _NtlWorldShadowParam)
{
	m_rgbaShadow = m_NtlWorldShadowParam._RGBA;
	::CopyMemory(&m_NtlWorldShadowParam, &_NtlWorldShadowParam, sizeof(sNTL_WORLD_SHADOW_PARAM));
}

void CNtlWorldShadow::UpdateSParam4Doodads(sNTL_WORLD_SHADOW_PARAM& _NtlWorldShadowParam)
{
	if(!m_pNtlWorldShadowParam4Doodads)
	{
		m_pNtlWorldShadowParam4Doodads = NTL_NEW sNTL_WORLD_SHADOW_PARAM;
	}

	::CopyMemory(m_pNtlWorldShadowParam4Doodads, &_NtlWorldShadowParam, sizeof(sNTL_WORLD_SHADOW_PARAM));
}

#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////