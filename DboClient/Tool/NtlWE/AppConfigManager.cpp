#include "StdAfx.h"
#include "AppConfigManager.h"
#include "NtlDebug.h"

#include "NtlPLSceneManager.h"
#include "NtlPLVisualManager.h"

#include "NtlWorldSectorManager.h"
#include "NtlWorldFieldManager.h"
#include "NtlWorldFieldManager4RWWorld.h"

#include "NtlPLWorldEntity.h"


CAppConfigManager& CAppConfigManager::GetInstance(void)
{
	static CAppConfigManager AppConfigManager;
	return AppConfigManager;
}

CAppConfigManager::CAppConfigManager(void)
{
	Init();
}

CAppConfigManager::~CAppConfigManager(void)
{
	Free();
}

void CAppConfigManager::Init(void)
{
	// brush
	m_vecItem.push_back("brush size");
	m_vecItem.push_back("heightfield sensitivity");
	m_vecItem.push_back("texture sensitivity");
	m_vecItem.push_back("diffuse opacity sensitivity");

	// camera
	m_vecItem.push_back("camera world position x");
	m_vecItem.push_back("camera world position y");
	m_vecItem.push_back("camera world position z");
}

void CAppConfigManager::Free(void)
{
	m_vecItem.clear();
}

bool CAppConfigManager::Load()
{
	NTL_FUNCTION("CAppConfigManager::Load");

	string	SubKey = "SOFTWARE\\NTL\\DBO\\CONFIG";
	string	ValueName, Value;
	RwV3d	vSaberStartPosition;

	vSaberStartPosition.x = 0.0f;
	vSaberStartPosition.y = 0.0f;
	vSaberStartPosition.z = 0.0f;

	vector<string>::iterator Iter = m_vecItem.begin();
	while(Iter != m_vecItem.end())
	{

		// brush size
		if(!(*Iter).compare("brush size"))
		{
			Iter++;

			ValueName = "brush size";
			CRegistry::GetRegistry(SubKey, ValueName, Value);

			if(!Value.compare("") || !Value.compare("ERROR"))
			{
				continue;
			}

			dGET_WORLD_PARAM()->NumBrushRadiusInTile = static_cast<RwReal>(::atof(Value.c_str()));
		}

		// brush sensitivity
		else if(!(*Iter).compare("heightfield sensitivity"))
		{
			Iter++;

			ValueName = "heightfield sensitivity";
			CRegistry::GetRegistry(SubKey, ValueName, Value);

			if(!Value.compare("") || !Value.compare("ERROR"))
			{
				continue;
			}

			dGET_WORLD_PARAM()->ValHeightfieldSensitivity = static_cast<RwReal>(::atof(Value.c_str()));
		}
		else if(!(*Iter).compare("texture sensitivity"))
		{
			Iter++;

			ValueName = "texture sensitivity";
			CRegistry::GetRegistry(SubKey, ValueName, Value);

			if(!Value.compare("") || !Value.compare("ERROR"))
			{
				continue;
			}
			
			dGET_WORLD_PARAM()->ValTextureSensitivity = static_cast<RwReal>(::atof(Value.c_str()));
		}
		else if(!(*Iter).compare("diffuse opacity sensitivity"))
		{
			Iter++;

			ValueName = "diffuse opacity sensitivity";
			CRegistry::GetRegistry(SubKey, ValueName, Value);

			if(!Value.compare("") || !Value.compare("ERROR"))
			{
				continue;
			}

			dGET_WORLD_PARAM()->ValDiffuseOpacitySensitivity = static_cast<RwReal>(::atof(Value.c_str()));
		}
		// camera world position
		else if(!(*Iter).compare("camera world position x"))
		{
			Iter++;

			ValueName = "camera world position x";
			CRegistry::GetRegistry(SubKey, ValueName, Value);

			if(!Value.compare("") || !Value.compare("ERROR"))
			{
				continue;
			}

			vSaberStartPosition.x = static_cast<RwReal>(::atof(Value.c_str()));

// 			RwV3d vPos = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
// 			vPos.x = static_cast<RwReal>(::atof(Value.c_str()));
// 			
// 			//RwReal Max = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - RwCameraGetFarClipPlane(CNtlPLGlobal::m_RwCamera) - 1);
// 			//RwReal Min = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + RwCameraGetFarClipPlane(CNtlPLGlobal::m_RwCamera) + 1);
// 			//RwReal Max = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - (dGET_WORLD_PARAM()->WorldFieldSize * 2) - 1);
// 			//RwReal Min = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + (dGET_WORLD_PARAM()->WorldFieldSize * 2) + 1);
// 			RwReal Max = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - dNTL_WORLD_EXC_AOI_SIZE - 1);
// 			RwReal Min = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + dNTL_WORLD_EXC_AOI_SIZE + 1);
// 
// 			CLAMP(vPos.x, Min, Max);
// 
// 			if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
// 			{
// 				RwFrameTranslate(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera), &vPos, rwCOMBINEREPLACE);
// 			}
// 			else
// 			{
// 				vPos.x = 0.0f;
// 				RwFrameTranslate(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera), &vPos, rwCOMBINEREPLACE);
// 			}
		}
		else if(!(*Iter).compare("camera world position y"))
		{
			Iter++;

			ValueName = "camera world position y";
			CRegistry::GetRegistry(SubKey, ValueName, Value);

			if(!Value.compare("") || !Value.compare("ERROR"))
			{
				continue;
			}

			vSaberStartPosition.y = static_cast<RwReal>(::atof(Value.c_str()));

// 			RwV3d vPos = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
// 			vPos.y = static_cast<RwReal>(::atof(Value.c_str()));
// 
// 			//RwReal Max = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - RwCameraGetFarClipPlane(CNtlPLGlobal::m_RwCamera) - 1);
// 			//RwReal Min = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + RwCameraGetFarClipPlane(CNtlPLGlobal::m_RwCamera) + 1);
// 			//RwReal Max = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - (dGET_WORLD_PARAM()->WorldFieldSize * 2) - 1);
// 			//RwReal Min = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + (dGET_WORLD_PARAM()->WorldFieldSize * 2) + 1);
// 			RwReal Max = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - dNTL_WORLD_EXC_AOI_SIZE - 1);
// 			RwReal Min = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + dNTL_WORLD_EXC_AOI_SIZE + 1);
// 			CLAMP(vPos.y, Min, Max);
// 
// 			if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
// 			{
// 				RwFrameTranslate(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera), &vPos, rwCOMBINEREPLACE);
// 			}
// 			else
// 			{
// 				vPos.y = 0.0f;
// 				RwFrameTranslate(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera), &vPos, rwCOMBINEREPLACE);
// 			}
		}
		else if(!(*Iter).compare("camera world position z"))
		{
			Iter++;

			ValueName = "camera world position z";
			CRegistry::GetRegistry(SubKey, ValueName, Value);

			if(!Value.compare("") || !Value.compare("ERROR"))
			{
				continue;
			}

			vSaberStartPosition.z = static_cast<RwReal>(::atof(Value.c_str()));

// 			RwV3d vPos = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
// 			vPos.z = static_cast<RwReal>(::atof(Value.c_str()));
// 
// 			//RwReal Max = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - RwCameraGetFarClipPlane(CNtlPLGlobal::m_RwCamera) - 1);
// 			//RwReal Min = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + RwCameraGetFarClipPlane(CNtlPLGlobal::m_RwCamera) + 1);
// 			//RwReal Max = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - (dGET_WORLD_PARAM()->WorldFieldSize * 2) - 1);
// 			//RwReal Min = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + (dGET_WORLD_PARAM()->WorldFieldSize * 2) + 1);
// 			RwReal Max = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMax - dNTL_WORLD_EXC_AOI_SIZE - 1);
// 			RwReal Min = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldValueMin + dNTL_WORLD_EXC_AOI_SIZE + 1);
// 			CLAMP(vPos.z, Min, Max);
// 
// 			if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
// 			{
// 				RwFrameTranslate(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera), &vPos, rwCOMBINEREPLACE);
// 			}
// 			else
// 			{
// 				vPos.z = 0.0f;
// 				RwFrameTranslate(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera), &vPos, rwCOMBINEREPLACE);
// 			}
		}
	}

	if (GetSceneManager()->GetWorld() && GetSceneManager()->GetAvailablePos(vSaberStartPosition) && GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
	{
		GetSceneManager()->GetWorld()->SetPortalPosition(vSaberStartPosition);
	}
	else
	{
		vSaberStartPosition.x = 0.0f;
		vSaberStartPosition.y = 0.0f;
		vSaberStartPosition.z = 0.0f;

		GetSceneManager()->GetWorld()->SetPortalPosition(vSaberStartPosition);
	}

	NTL_RETURN(true);
}

bool CAppConfigManager::Save()
{
	NTL_FUNCTION("CAppConfigManager::Save");

	string SubKey, ValueName, Value;
	char ValueInput[10] = {0,};

	vector<string>::iterator Iter = m_vecItem.begin();
	while(Iter != m_vecItem.end())
	{
		// brush size
		if(!(*Iter).compare("brush size"))
		{
			Iter++;

			SubKey = "SOFTWARE\\NTL\\DBO\\CONFIG";
			ValueName = "brush size";
			sprintf_s(ValueInput, 10, "%.2f", dGET_WORLD_PARAM()->NumBrushRadiusInTile);
			Value = ValueInput;
			CRegistry::SetRegistry(SubKey, ValueName, Value);
		}

		// brush sensitivity
		else if(!(*Iter).compare("heightfield sensitivity"))
		{
			Iter++;

			SubKey = "SOFTWARE\\NTL\\DBO\\CONFIG";
			ValueName = "heightfield sensitivity";
			sprintf_s(ValueInput, 10, "%.2f", dGET_WORLD_PARAM()->ValHeightfieldSensitivity);
			Value = ValueInput;
			CRegistry::SetRegistry(SubKey, ValueName, Value);
		}
		else if(!(*Iter).compare("texture sensitivity"))
		{
			Iter++;

			ValueName = "texture sensitivity";
			sprintf_s(ValueInput, 10, "%.2f", dGET_WORLD_PARAM()->ValTextureSensitivity);
			Value = ValueInput;
			CRegistry::SetRegistry(SubKey, ValueName, Value);
		}
		else if(!(*Iter).compare("diffuse opacity sensitivity"))
		{
			Iter++;

			ValueName = "diffuse opacity sensitivity";
			sprintf_s(ValueInput, 10, "%.2f", dGET_WORLD_PARAM()->ValDiffuseOpacitySensitivity);
			Value = ValueInput;
			CRegistry::SetRegistry(SubKey, ValueName, Value);
		}

		// camera world position
		else if(!(*Iter).compare("camera world position x"))
		{
			Iter++;

			SubKey = "SOFTWARE\\NTL\\DBO\\CONFIG";
			ValueName = "camera world position x";
			RwV3d vPos = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
			sprintf_s(ValueInput, 10, "%.2f", vPos.x);
			Value = ValueInput;
			CRegistry::SetRegistry(SubKey, ValueName, Value);
		}
		else if(!(*Iter).compare("camera world position y"))
		{
			Iter++;

			SubKey = "SOFTWARE\\NTL\\DBO\\CONFIG";
			ValueName = "camera world position y";
			RwV3d vPos = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
			sprintf_s(ValueInput, 10, "%.2f", vPos.y);
			Value = ValueInput;
			CRegistry::SetRegistry(SubKey, ValueName, Value);
		}
		else if(!(*Iter).compare("camera world position z"))
		{
			Iter++;

			SubKey = "SOFTWARE\\NTL\\DBO\\CONFIG";
			ValueName = "camera world position z";
			RwV3d vPos = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
			sprintf_s(ValueInput, 10, "%.2f", vPos.z);
			Value = ValueInput;
			CRegistry::SetRegistry(SubKey, ValueName, Value);
		}
	}

	NTL_RETURN(true);
}