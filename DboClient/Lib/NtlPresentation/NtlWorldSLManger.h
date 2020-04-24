#pragma once


#include <rwplcore.h>
#include <vector>
#include <windows.h>

using std::vector;


#define dGET_WORLD_SLMGR() (&g_NtlWorldSLMangager)


class CNtlWorldSector;

#pragma pack(1)

struct sNTL_WORLD_SL
{ 
	// color density
	float m_Softness;

	// [0] : min brightness, [1] : max brightness
	float m_Brightness[2];

	// light direction
	int m_Dir;

	// Light Color( alpha value is used for a boolean flag whether applying slope lighting(1.0f) or not(0.0f) )
	RwRGBAReal _Clr;

public:
	sNTL_WORLD_SL::sNTL_WORLD_SL()
	{
		m_Softness		= 10.0f;
		m_Brightness[0]	= 0.1f;
		m_Brightness[1]	= 1.0f;
		m_Dir			= 3;
		_Clr.alpha		= 1.0f;
		_Clr.red		= 0.0f;
		_Clr.green		= 0.0f;
		_Clr.blue		= 0.0f;
	}
};

#pragma pack()

class CNtlWorldSLManger
{
public:
	CNtlWorldSLManger(void);
	~CNtlWorldSLManger(void);

private:
	sNTL_WORLD_SL m_Attr;

private:
	float GetHeightValueFromFile(float PosX, float PosZ, FILE* pFile);

public:
	// set attr. 4 run-time
	void SetAttr(sNTL_WORLD_SL& NtlWorldSL);

	// set vert using heightfield raw files
	bool SetVert(RwRGBA& Result, DWORD* pVertPaintMap, DWORD* pVertShadowMap, float PosX, float PosZ, FILE* pFile);

	sNTL_WORLD_SL& GetAttr() { return m_Attr; }
};


extern CNtlWorldSLManger g_NtlWorldSLMangager;
