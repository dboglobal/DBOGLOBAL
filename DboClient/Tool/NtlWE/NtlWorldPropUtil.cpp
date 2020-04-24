#include "stdafx.h"
#include "NtlWorldPropUtil.h"


CNtlWorldPropToolData::CNtlWorldPropToolData()
{
	for (int i = 0; i < 16; ++i)
	{
		m_strPropName[i] = "";
		m_rgbaPropColor[i].alpha = 255;
		m_rgbaPropColor[i].red	 = 255;
		m_rgbaPropColor[i].green = 255;
		m_rgbaPropColor[i].blue	 = 255;
	}
}

CNtlWorldPropToolData::~CNtlWorldPropToolData()
{
}

void CNtlWorldPropToolData::LoadIni(std::string strFilePath)
{
	char acWorkingDir[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, acWorkingDir);

	char acResult[NTL_MAX_DIR_PATH] = { 0, };

	RwInt32 iRed, iGreen, iBlue;
	for (int i = 0; i < 16; ++i)
	{
		char acKeyName[NTL_MAX_DIR_PATH];

		sprintf_s(acKeyName, NTL_MAX_DIR_PATH, "property%.2d", i);
		::GetPrivateProfileString("property", acKeyName, "error", acResult, NTL_MAX_DIR_PATH, strFilePath.c_str());
		m_strPropName[i] = acResult;
		
		sprintf_s(acKeyName, NTL_MAX_DIR_PATH, "color%.2d", i);
		::GetPrivateProfileString("color", acKeyName, "255,255,255", acResult, NTL_MAX_DIR_PATH, strFilePath.c_str());
		sscanf_s(acResult, "%d,%d,%d", &iRed, &iGreen, &iBlue);

		m_rgbaPropColor[i].red	= static_cast<RwUInt8>(iRed);
		m_rgbaPropColor[i].green= static_cast<RwUInt8>(iGreen);
		m_rgbaPropColor[i].blue	= static_cast<RwUInt8>(iBlue);
	}

	::GetPrivateProfileString("color", "colorintersec", "255,255,255", acResult, NTL_MAX_DIR_PATH, strFilePath.c_str());
	sscanf_s(acResult, "%d,%d,%d", &iRed, &iGreen, &iBlue);
	m_rgbaPropColorIntersection.red		= static_cast<RwUInt8>(iRed);
	m_rgbaPropColorIntersection.green	= static_cast<RwUInt8>(iGreen);
	m_rgbaPropColorIntersection.blue	= static_cast<RwUInt8>(iBlue);

	::SetCurrentDirectory(acWorkingDir);
}