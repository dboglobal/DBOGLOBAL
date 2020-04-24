//***********************************************************************************
//	File		:	VenusConfig.cpp
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "StdAfx.h"
#include ".\venusconfig.h"

#include "rpworld.h"

#include ".\venusgrid.h"

#include "OptionDialog.h"
#include "VenusDefine.h"
#include "VenusFramework.h"

CVenusConfig::CVenusConfig(void)
{
	m_nCurrentLayer = 0;
	m_nEditMode = EDIT_MODE_MOVE;

	m_fEditMoveSpeed = 0.01f;
	m_fEditRotateSpeed = 1.10f;
	m_fEditScaleSpeed = 0.02f;

	m_bShowUpdateNews	= FALSE;

	m_fTimeSpeed		= 1.f;
	m_bTimePause		= FALSE;
	m_bTimeRepeat		= TRUE;

	m_bShowGrid			= TRUE;
	m_bShowTerrain		= TRUE;
	m_bShowSky			= TRUE;

	m_bUseAmbient		= TRUE;
	m_AmbientColor.red = m_AmbientColor.green = m_AmbientColor.blue = 0.5f;

	m_bUseLight			= TRUE;
	m_nLightType		= rpLIGHTSPOTSOFT;
	m_LightColor.red = m_LightColor.green = m_LightColor.blue = 1.f;
	m_vLightDirection.x = m_vLightDirection.z = 0.f;
	m_vLightDirection.y = -1;

	m_fLightRadius		= 30.f;
	m_fLightConeAngle	= 45.f;

	m_strClumpTexturePath	= ".\\Textures\\Effect\\Mesh\\";;
	m_strClumpPath			= "Object\\Effect";
	m_strAnimationPath		= "Object\\Effect";
	m_strUvAnimationPath	= "Object\\Effect";
	m_strEffectTexturePath	= "Textures\\Effect";
    m_strSoundPath          = "sound";

	// Ball Speed
	m_fEventBallSpeed		= 10.f;
	m_fEventBallLifeTime	= 2.f;

	m_fAutoSaveMaxTime	= 300.f;

	m_strAppName = "Venus ";
    m_strAppName += VERSION_VENUS;

	m_D3DFillMode		= D3DFILL_SOLID;

    m_BackGroundColor.red = m_BackGroundColor.green = m_BackGroundColor.blue = 150;
    m_BackGroundColor.alpha = 255;

    m_bRenderBoundingSphere = FALSE;
}

CVenusConfig::~CVenusConfig(void)
{
}


CVenusConfig& CVenusConfig::GetInstance()
{
	static CVenusConfig config;
	return config;
}

void CVenusConfig::SetMainDirectory()
{
	SetCurrentDirectory(m_strMainPath.c_str());
}


void CVenusConfig::Load()
{
	FILE* pFile;
	pFile = fopen("TOOL\\VenusData\\Config.txt", "rt");

	if (pFile == NULL)
	{
		return;
	}

	char cTemp[1024], cHead[1024];
    ZeroMemory(cTemp, sizeof(cTemp));
    ZeroMemory(cHead, sizeof(cHead));        

    ZeroMemory(cTemp, sizeof(cTemp));
	fscanf(pFile, "%s %s", cTemp, cHead);
	if (_stricmp(cHead, "RESOURCE") != 0)
	{
		fclose(pFile);
		return;
	} 

	unsigned int READ_VERSION;
    ZeroMemory(cTemp, sizeof(cTemp));
	fscanf(pFile, "%s %d", cTemp, &READ_VERSION);

    ZeroMemory(cTemp, sizeof(cTemp));
	fscanf(pFile, "%s", cTemp);
	while(_stricmp(cTemp, "]") != 0)
	{
		if (_stricmp(cTemp, "GRID") == 0)
		{
			LoadGrid(pFile);
		}
		else if (_stricmp(cTemp, "TIME") == 0)
		{
			LoadTime(pFile);
		}
		else if (_stricmp(cTemp, "DYMMY") == 0)
		{
			LoadDummy(pFile);
		}
		else if (_stricmp(cTemp, "LIGHT") == 0)
		{
			LoadLight(pFile);
		}
		else if (_stricmp(cTemp, "EVENT") == 0)
		{
			LoadEvent(pFile);
		}
		else if (_stricmp(cTemp, "PATH") == 0)
		{
			LoadPath(pFile);
		}

        ZeroMemory(cTemp, sizeof(cTemp));
		fscanf(pFile, "%s", cTemp);
	}

	fclose(pFile);

	m_bShowUpdateNews = FALSE;
}

//------------------------------------------------------------------
//	FuncName	: LoadGrid
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusConfig::LoadGrid(FILE* pFile)
{
    char cTemp[256] = {0,};
    char cHead[256] = {0,};
    int     nTileCount = 0;
    RwReal  fGridWidth = 0.0f; 

	fscanf(pFile, "%s", cHead);
	while(_stricmp(cTemp, "]") != 0)
	{
		if (_stricmp(cTemp, "GRID_COUNT") == 0)
		{
			fscanf(pFile, "%d", &nTileCount);
            CVenusGrid::GetInstance().SetTileCount(nTileCount);
		}
		else if (_stricmp(cTemp, "GRID_WIDTH") == 0)
		{
			fscanf(pFile, "%f", &fGridWidth);
            CVenusGrid::GetInstance().SetGridWidth(fGridWidth);
		}
        else if(_stricmp(cTemp, "BACK_COLOR") == 0)
        {
            int red, green, blue;            
            fscanf(pFile, "%d %d %d", &red, &green, &blue);
            m_BackGroundColor.red = red;
            m_BackGroundColor.green = green;
            m_BackGroundColor.blue = blue;
            m_BackGroundColor.alpha = 255;
            CVenusFramework::GetInstance().SetBackGroundColor(m_BackGroundColor);
        }

        ZeroMemory(cTemp, sizeof(cTemp));
		fscanf(pFile, "%s", cTemp);
	}
}

//------------------------------------------------------------------
//	FuncName	: SaveGrid
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusConfig::LoadTime(FILE* pFile)
{
    char cTemp[256] = {0,};
    char cHead[256] = {0,};

	fscanf(pFile, "%s", cHead);
	while(_stricmp(cTemp, "]") != 0)
	{
		if (_stricmp(cTemp, "TIME_SPEED") == 0)
		{
			fscanf(pFile, "%f", &m_fTimeSpeed);
		}
		else if (_stricmp(cTemp, "TIME_PAUSE") == 0)
		{
			fscanf(pFile, "%d", &m_bTimePause);
		}
		else if (_stricmp(cTemp, "TIME_REPEAT") == 0)
		{
			fscanf(pFile, "%d", &m_bTimeRepeat);
		}

        ZeroMemory(cTemp, sizeof(cTemp));
		fscanf(pFile, "%s", cTemp);
	}
}

//------------------------------------------------------------------
//	FuncName	: LoadDummy
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusConfig::LoadDummy(FILE* pFile)
{
    char cTemp[256] = {0,};
    char cHead[256] = {0,};

	fscanf(pFile, "%s", cHead);
	while(_stricmp(cTemp, "]") != 0)
	{
		if (_stricmp(cTemp, "SHOW_GRID") == 0)
		{
			fscanf(pFile, "%d", &m_bShowGrid);
		}
		else if (_stricmp(cTemp, "SHOW_SKY") == 0)
		{
			fscanf(pFile, "%d", &m_bShowSky);
		}
		else if (_stricmp(cTemp, "SHOW_TERRAIN") == 0)
		{
			fscanf(pFile, "%d", &m_bShowTerrain);
		}

        ZeroMemory(cTemp, sizeof(cTemp));
		fscanf(pFile, "%s", cTemp);
	}
}

//------------------------------------------------------------------
//	FuncName	: LoadLight
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusConfig::LoadLight(FILE* pFile)
{
    char cTemp[256] = {0,};
    char cHead[256] = {0,};

	fscanf(pFile, "%s", cHead);
	while(_stricmp(cTemp, "]") != 0)
	{
		if (_stricmp(cTemp, "USE_AMBIENT") == 0)
		{
			fscanf(pFile, "%d", &m_bUseAmbient);
		}
		else if (_stricmp(cTemp, "AMBIENT_COLOR") == 0)
		{
			fscanf(pFile, "%f %f %f", &m_AmbientColor.red, &m_AmbientColor.green, &m_AmbientColor.blue);
		}
		else if (_stricmp(cTemp, "USE_LIGHT") == 0)
		{
			fscanf(pFile, "%d", &m_bUseLight);
		}
		else if (_stricmp(cTemp, "LIGHT_TYPE") == 0)
		{
			fscanf(pFile, "%d", &m_nLightType);
		}
		else if (_stricmp(cTemp, "LIGHT_COLOR") == 0)
		{
			fscanf(pFile, "%f %f %f", &m_LightColor.red, &m_LightColor.green, &m_LightColor.blue);
		}

		else if (_stricmp(cTemp, "LIGHT_RADIUS") == 0)
		{
			fscanf(pFile, "%f", &m_fLightRadius);
		}
		else if (_stricmp(cTemp, "LIGHT_CONEANGLE") == 0)
		{
			fscanf(pFile, "%f", &m_fLightConeAngle);
		}

        ZeroMemory(cTemp, sizeof(cTemp));
		fscanf(pFile, "%s", cTemp);
	}
}

//------------------------------------------------------------------
//	FuncName	: LoadEvent
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusConfig::LoadEvent(FILE* pFile)
{
    char cTemp[256] = {0,};
    char cHead[256] = {0,};

	fscanf(pFile, "%s", cHead);
	while(_stricmp(cTemp, "]") != 0)
	{
		if (_stricmp(cTemp, "SPEED") == 0)
		{
			fscanf(pFile, "%f", &m_fEventBallSpeed);
		}
		else if (_stricmp(cTemp, "LIFETIME") == 0)
		{
			fscanf(pFile, "%f", &m_fEventBallLifeTime);
		}

        ZeroMemory(cTemp, sizeof(cTemp));
		fscanf(pFile, "%s", cTemp);
	}
}

//------------------------------------------------------------------
//	FuncName	: LoadLight
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusConfig::LoadPath(FILE* pFile)
{
    char cTemp[256] = {0,};
    char cHead[256] = {0,};

	fscanf(pFile, "%s", cHead);
	while(_stricmp(cTemp, "]") != 0)
	{
		if (_stricmp(cTemp, "CLUMP_TEXTURE_PATH") == 0)
		{
            ZeroMemory(cTemp, sizeof(cTemp));
			fgets(cTemp, 256, pFile);
			GetString(cHead, cTemp);
			m_strClumpTexturePath = cHead;
		}
		else if (_stricmp(cTemp, "CLUMP") == 0)
		{
            ZeroMemory(cTemp, sizeof(cTemp));
			fgets(cTemp, 256, pFile);
			GetString(cHead, cTemp);
			m_strClumpPath = cHead;
		}
		else if (_stricmp(cTemp, "ANIMATNION") == 0)
		{
            ZeroMemory(cTemp, sizeof(cTemp));
			fgets(cTemp, 256, pFile);
			GetString(cHead, cTemp);
			m_strAnimationPath = cHead;
		}
		else if (_stricmp(cTemp, "UVANIMATION") == 0)
		{
            ZeroMemory(cTemp, sizeof(cTemp));
			fgets(cTemp, 256, pFile);
			GetString(cHead, cTemp);
			m_strUvAnimationPath = cHead;
		}
		else if (_stricmp(cTemp, "EFFECT_TEXTURE_PATH") == 0)
		{
            ZeroMemory(cTemp, sizeof(cTemp));
			fgets(cTemp, 256, pFile);
			GetString(cHead, cTemp);
			m_strEffectTexturePath = cHead;
		}
        else if(_stricmp(cTemp, "SOUND") == 0)
        {
            ZeroMemory(cTemp, sizeof(cTemp));
            fgets(cTemp, 256, pFile);
            GetString(cHead, cTemp);
            m_strSoundPath = cHead;
        }
		else if(_stricmp(cTemp, "WORLD") == 0)
		{
            ZeroMemory(cTemp, sizeof(cTemp));
			fgets(cTemp, 256, pFile);
			GetString(cHead, cTemp);
			m_strWorldPath = cHead;
		}

        ZeroMemory(cTemp, sizeof(cTemp));
		fscanf(pFile, "%s", cTemp);
	}
}


void CVenusConfig::Save() 
{
	FILE* pFile;
	pFile = fopen("TOOL\\VenusData\\Config.txt", "wt");

	if(pFile == NULL)
	{
		return;
	}

	fprintf(pFile, "HEADER \tRESOURCE \n");

	fprintf(pFile, "VERSION %d \n", DATA_VERSION);

	fprintf(pFile, "\n");

	fprintf(pFile, "[ \n");

	SaveGrid(pFile, "\t");
	SaveTime(pFile, "\t");
	SaveDummy(pFile, "\t");
	SaveLight(pFile, "\t");
	SaveEvent(pFile, "\t");
	SavePath(pFile, "\t");

	fprintf(pFile, "] \n");

	fclose(pFile);
}

//------------------------------------------------------------------
//	FuncName	: SaveGrid
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusConfig::SaveGrid(FILE* pFile, CString strHead)
{
	fprintf(pFile, "%sGRID \n", strHead);
	fprintf(pFile, "%s[ \n", strHead);
	{
		fprintf(pFile, "%s\tGRID_COUNT \t%d \n",		strHead + "\t",	CVenusGrid::GetInstance().GetTileCount());
		fprintf(pFile, "%s\tGRID_WIDTH \t%f \n",		strHead + "\t",	CVenusGrid::GetInstance().GetGridWidth());
        fprintf(pFile, "%s\tBACK_COLOR \t%d %d %d \n",   strHead + "\t", CVenusFramework::GetInstance().GetBackGroundColor().red,
            CVenusFramework::GetInstance().GetBackGroundColor().green,
            CVenusFramework::GetInstance().GetBackGroundColor().blue);            
	}
	fprintf(pFile, "%s] \n", strHead);
	fprintf(pFile, "\n");
}

//------------------------------------------------------------------
//	FuncName	: SaveGrid
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusConfig::SaveTime(FILE* pFile, CString strHead)
{
	fprintf(pFile, "%sTIME \n", strHead);
	fprintf(pFile, "%s[ \n", strHead);
	{
		fprintf(pFile, "%s\tTIME_SPEED \t%f \n",		strHead + "\t",	m_fTimeSpeed);
		fprintf(pFile, "%s\tTIME_PAUSE \t%d \n",		strHead + "\t",	m_bTimePause);
		fprintf(pFile, "%s\tTIME_REPEAT \t%d \n",		strHead + "\t",	m_bTimeRepeat);
	}
	fprintf(pFile, "%s] \n", strHead);
	fprintf(pFile, "\n");
}

//------------------------------------------------------------------
//	FuncName	: SaveDummy
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusConfig::SaveDummy(FILE* pFile, CString strHead)
{
	fprintf(pFile, "%sDYMMY \n", strHead);
	fprintf(pFile, "%s[ \n", strHead);
	{
		fprintf(pFile, "%s\tSHOW_GRID \t%d \n",		strHead + "\t",	m_bShowGrid);
		fprintf(pFile, "%s\tSHOW_SKY \t%d \n",		strHead + "\t",	m_bShowSky);
		fprintf(pFile, "%s\tSHOW_TERRAIN \t%d \n",	strHead + "\t",	m_bShowTerrain);
	}
	fprintf(pFile, "%s] \n", strHead);
	fprintf(pFile, "\n");
}

//------------------------------------------------------------------
//	FuncName	: SavePath
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusConfig::SaveLight(FILE* pFile, CString strHead)
{
	fprintf(pFile, "%sLIGHT \n", strHead);
	fprintf(pFile, "%s[ \n", strHead);
	{
		fprintf(pFile, "%s\tUSE_AMBIENT \t%d \n",			strHead + "\t",	m_bUseAmbient);
		fprintf(pFile, "%s\tAMBIENT_COLOR \t%f %f %f \n",	strHead + "\t",	m_AmbientColor.red, m_AmbientColor.green, m_AmbientColor.blue);

		fprintf(pFile, "%s\tUSE_LIGHT \t%d \n",				strHead + "\t",	m_bUseLight);
		fprintf(pFile, "%s\tLIGHT_TYPE \t%d \n",			strHead + "\t",	m_nLightType);
		fprintf(pFile, "%s\tLIGHT_COLOR \t%f %f %f \n",		strHead + "\t",	m_LightColor.red, m_LightColor.green, m_LightColor.blue);

		fprintf(pFile, "%s\tLIGHT_RADIUS \t%f \n",			strHead + "\t",	m_fLightRadius);
		fprintf(pFile, "%s\tLIGHT_CONEANGLE \t%f \n",		strHead + "\t",	m_fLightConeAngle);
	}
	fprintf(pFile, "%s] \n", strHead);
	fprintf(pFile, "\n");
}


//------------------------------------------------------------------
//	FuncName	: SaveEvent
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusConfig::SaveEvent(FILE* pFile, CString strHead)
{
	fprintf(pFile, "%sEVENT \n", strHead);
	fprintf(pFile, "%s[ \n", strHead);
	{
		fprintf(pFile, "%s\tSPEED \t%f \n",			strHead + "\t",	m_fEventBallSpeed);
		fprintf(pFile, "%s\tLIFETIME \t%f\n",		strHead + "\t",	m_fEventBallLifeTime);
	}
	fprintf(pFile, "%s] \n", strHead);
	fprintf(pFile, "\n");
}

//------------------------------------------------------------------
//	FuncName	: SaveLight
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusConfig::SavePath(FILE* pFile, CString strHead)
{
	fprintf(pFile, "%sPATH \n", strHead);
	fprintf(pFile, "%s[ \n", strHead);
	{
		fprintf(pFile, "%s\tCLUMP_TEXTURE_PATH \t%s \n",	strHead + "\t",	m_strClumpTexturePath.c_str());
		fprintf(pFile, "%s\tCLUMP \t%s\n",					strHead + "\t",	m_strClumpPath.c_str());
		fprintf(pFile, "%s\tANIMATNION \t%s \n",			strHead + "\t",	m_strAnimationPath.c_str());
		fprintf(pFile, "%s\tUVANIMATION \t%s \n",			strHead + "\t",	m_strUvAnimationPath.c_str());
		fprintf(pFile, "%s\tEFFECT_TEXTURE_PATH \t%s \n",	strHead + "\t",	m_strEffectTexturePath.c_str());
        fprintf(pFile, "%s\tSOUND \t%s \n",					strHead + "\t",	m_strSoundPath.c_str());
		fprintf(pFile, "%s\tWORLD \t%s \n",					strHead + "\t", m_strWorldPath.c_str());
	}
	fprintf(pFile, "%s] \n", strHead);
	fprintf(pFile, "\n");
}

//------------------------------------------------------------------
//	FuncName	: GetString
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusConfig::GetString(RwChar* strDest, RwChar* strSrc)
{
	int i = 0;
	for (i = 0; i < (int)strlen(strSrc); ++ i)
	{
		if (strSrc[i] != ' ' && strSrc[i] != '\t') break;
	}
	strSrc = &strSrc[i];

	char* crpos = strchr(&strSrc[i], '\n');
	if(crpos) *crpos = '\0';

	for (int i = (int)strlen(strSrc)-1; i >= 0; -- i)
	{
		if (strSrc[i] != ' ' && strSrc[i] != '\t')
		{
			strSrc[i+1] = '\0';
			break;
		}
	}
	strcpy(strDest, strSrc);
}

//------------------------------------------------------------------
//	FuncName	: ShowUpdateNews
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusConfig::ShowUpdateNews()
{
	if (m_strWinwordPath.empty())
	{
		if (!FindWinword(m_strWinwordPath, "C:\\Program Files\\"))
		{
			MessageBox(NULL, "Word Not Found", "Error", MB_OK);
			return;
		}
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	CString strCommandLine;
	strCommandLine = m_strWinwordPath.c_str();
	strCommandLine += " ";
	strCommandLine += "UpdateNews.doc";

	CString strPath;
	strPath = m_strMainPath.c_str();
	strPath += "\\TOOL\\VenusData\\";
	SetCurrentDirectory(strPath.GetBuffer(0));

	if (!CreateProcess(NULL, strCommandLine.GetBuffer(0), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		SetMainDirectory();
		return;
	}
	CloseHandle(pi.hProcess);
	SetMainDirectory();
}

//------------------------------------------------------------------
//	FuncName	: LoadPath()
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusConfig::FindWinword(std::string& strWinwordPath, RwChar* path)
{
	RwChar strCurDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, strCurDir);

	if (!SetCurrentDirectory(path))
	{
		return FALSE;
	}

	RwBool bResult = SubFindWinword(strWinwordPath);

	SetCurrentDirectory(strCurDir);
	return bResult;
}

RwBool CVenusConfig::SubFindWinword(std::string& strWinwordPath)
{
	RwChar strCurPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, strCurPath);

	std::string strFullFileName;

	WIN32_FIND_DATA FindFileData;
	HANDLE hFile = FindFirstFile("*.*", &FindFileData);
	if(hFile == INVALID_HANDLE_VALUE) return FALSE;

	do 
	{
		strFullFileName		= strCurPath;
		strFullFileName		+= "\\";
		strFullFileName		+= FindFileData.cFileName;

		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//  디렉토리이면
			if(FindFileData.cFileName[0] == '.')
			{
				//  Dot 디렉토리는 스킵한다.
			} 
			else
			{
				BOOL ret = SetCurrentDirectory(strFullFileName.c_str());
				if (SubFindWinword(strWinwordPath))
				{
					return TRUE;
				}
			}
		} 
		else
		{
			if (_stricmp(FindFileData.cFileName, "winword.exe") == 0)
			{
				strWinwordPath = strFullFileName.c_str();
				return TRUE;
			}
		}
	} while(FindNextFile(hFile, &FindFileData));

	FindClose(hFile);
	return FALSE;
}
