#include "precomp_ntlpresentation.h"
#include "TextureStageStateView.h"

CTextureStageStateView::CTextureStageStateView()
{
}

CTextureStageStateView::~CTextureStageStateView()
{
}

void CTextureStageStateView::View(DWORD dwStage)
{
	DWORD	dwValue;
	char	cTemp[256];
	char	cValue[32];

	OutputDebugStringA("------------------------------------------------------------------------------------------\n");
	sprintf_s(cTemp, 256, "TEXTURE_STAGE_STATE = %d\n", dwStage); OutputDebugStringA(cTemp);
	RwD3D9GetTextureStageState(dwStage, D3DTSS_COLOROP, &dwValue);
	sprintf_s(cTemp, 256, "D3DTSS_COLOROP = %s\n", GetTOP(cValue, 32, (D3DTEXTUREOP)dwValue));	OutputDebugStringA(cTemp);
	RwD3D9GetTextureStageState(dwStage, D3DTSS_COLORARG1, &dwValue);
	sprintf_s(cTemp, 256, "D3DTSS_COLORARG1 = %s\n", GetDTA(cValue, 32, dwValue)); OutputDebugStringA(cTemp);
	RwD3D9GetTextureStageState(dwStage, D3DTSS_COLORARG2, &dwValue);
	sprintf_s(cTemp, 256, "D3DTSS_COLORARG2 = %s\n", GetDTA(cValue, 32, dwValue)); OutputDebugStringA(cTemp);

	RwD3D9GetTextureStageState(dwStage, D3DTSS_ALPHAOP, &dwValue);
	sprintf_s(cTemp, 256, "D3DTSS_ALPHAOP = %s\n", GetTOP(cValue, 32, (D3DTEXTUREOP)dwValue)); OutputDebugStringA(cTemp);
	RwD3D9GetTextureStageState(dwStage, D3DTSS_ALPHAARG1, &dwValue);
	sprintf_s(cTemp, 256, "D3DTSS_ALPHAARG1 = %s\n", GetDTA(cValue, 32, dwValue)); OutputDebugStringA(cTemp);
	RwD3D9GetTextureStageState(dwStage, D3DTSS_ALPHAARG2, &dwValue);
	sprintf_s(cTemp, 256, "D3DTSS_ALPHAARG2 = %s\n", GetDTA(cValue, 32, dwValue)); OutputDebugStringA(cTemp);	

	RwD3D9GetTextureStageState(dwStage, D3DTSS_BUMPENVMAT00, &dwValue);
	sprintf_s(cTemp, 256, "D3DTSS_BUMPENVMAT00 = %f\n", *((FLOAT*)&dwValue)); OutputDebugStringA(cTemp);	
	RwD3D9GetTextureStageState(dwStage, D3DTSS_BUMPENVMAT01, &dwValue);
	sprintf_s(cTemp, 256, "D3DTSS_BUMPENVMAT01 = %f\n", *((FLOAT*)&dwValue)); OutputDebugStringA(cTemp);	
	RwD3D9GetTextureStageState(dwStage, D3DTSS_BUMPENVMAT10, &dwValue);
	sprintf_s(cTemp, 256, "D3DTSS_BUMPENVMAT10 = %f\n", *((FLOAT*)&dwValue)); OutputDebugStringA(cTemp);	
	RwD3D9GetTextureStageState(dwStage, D3DTSS_BUMPENVMAT11, &dwValue);
	sprintf_s(cTemp, 256, "D3DTSS_BUMPENVMAT11 = %f\n", *((FLOAT*)&dwValue)); OutputDebugStringA(cTemp);

	RwD3D9GetTextureStageState(dwStage, D3DTSS_BUMPENVLSCALE, &dwValue);
	sprintf_s(cTemp, 256, "D3DTSS_BUMPENVLSCALE = %f\n", *((FLOAT*)&dwValue)); OutputDebugStringA(cTemp);
	RwD3D9GetTextureStageState(dwStage, D3DTSS_BUMPENVLOFFSET, &dwValue);
	sprintf_s(cTemp, 256, "D3DTSS_BUMPENVLOFFSET = %f\n", *((FLOAT*)&dwValue)); OutputDebugStringA(cTemp);

	RwD3D9GetTextureStageState(dwStage, D3DTSS_TEXCOORDINDEX, &dwValue);
	sprintf_s(cTemp, 256, "D3DTSS_TEXCOORDINDEX = %d\n", dwValue); OutputDebugStringA(cTemp);	
	RwD3D9GetTextureStageState(dwStage, D3DTSS_TEXTURETRANSFORMFLAGS, &dwValue);
	sprintf_s(cTemp, 256, "D3DTSS_TEXTURETRANSFORMFLAGS = %d\n", dwValue); OutputDebugStringA(cTemp);	

	RwD3D9GetTextureStageState(dwStage, D3DTSS_CONSTANT, &dwValue);
	sprintf_s(cTemp, 256, "D3DTSS_CONSTANT = %d\n", dwValue); OutputDebugStringA(cTemp);	

	RwD3D9GetTextureStageState(dwStage, D3DTSS_COLORARG0, &dwValue);
	sprintf_s(cTemp, 256, "D3DTSS_COLORARG0 = %s\n", GetDTA(cValue, 32, dwValue)); OutputDebugStringA(cTemp);	
	RwD3D9GetTextureStageState(dwStage, D3DTSS_ALPHAARG0, &dwValue);
	sprintf_s(cTemp, 256, "D3DTSS_ALPHAARG0 = %s\n", GetDTA(cValue, 32, dwValue)); OutputDebugStringA(cTemp);	
	RwD3D9GetTextureStageState(dwStage, D3DTSS_RESULTARG, &dwValue);
	sprintf_s(cTemp, 256, "D3DTSS_RESULTARG = %s\n", GetDTA(cValue, 32, dwValue)); OutputDebugStringA(cTemp);	
	OutputDebugStringA("------------------------------------------------------------------------------------------\n");
}

#define CASE_TO_SPRINTS(x, p, s) case x:sprintf_s(p, s, #x );break
char* CTextureStageStateView::GetTOP(char* pChar, size_t size, D3DTEXTUREOP value)
{
	switch (value)
	{
		CASE_TO_SPRINTS(D3DTOP_DISABLE, pChar, size);
		CASE_TO_SPRINTS(D3DTOP_SELECTARG1, pChar, size);
		CASE_TO_SPRINTS(D3DTOP_SELECTARG2, pChar, size);
		CASE_TO_SPRINTS(D3DTOP_MODULATE, pChar, size);
		CASE_TO_SPRINTS(D3DTOP_MODULATE2X, pChar, size);
		CASE_TO_SPRINTS(D3DTOP_MODULATE4X, pChar, size);
		CASE_TO_SPRINTS(D3DTOP_ADD, pChar, size);
		CASE_TO_SPRINTS(D3DTOP_ADDSIGNED, pChar, size);
		CASE_TO_SPRINTS(D3DTOP_ADDSIGNED2X, pChar, size);
		CASE_TO_SPRINTS(D3DTOP_SUBTRACT, pChar, size);
		CASE_TO_SPRINTS(D3DTOP_ADDSMOOTH, pChar, size);
		CASE_TO_SPRINTS(D3DTOP_BLENDDIFFUSEALPHA, pChar, size);
		CASE_TO_SPRINTS(D3DTOP_BLENDTEXTUREALPHA, pChar, size);
		CASE_TO_SPRINTS(D3DTOP_BLENDFACTORALPHA, pChar, size);
		CASE_TO_SPRINTS(D3DTOP_BLENDTEXTUREALPHAPM, pChar, size);
		CASE_TO_SPRINTS(D3DTOP_BLENDCURRENTALPHA, pChar, size);
		CASE_TO_SPRINTS(D3DTOP_PREMODULATE, pChar, size);
		CASE_TO_SPRINTS(D3DTOP_MODULATEALPHA_ADDCOLOR, pChar, size);
		CASE_TO_SPRINTS(D3DTOP_MODULATECOLOR_ADDALPHA, pChar, size);
		CASE_TO_SPRINTS(D3DTOP_MODULATEINVALPHA_ADDCOLOR, pChar, size);
		CASE_TO_SPRINTS(D3DTOP_MODULATEINVCOLOR_ADDALPHA, pChar, size);
		CASE_TO_SPRINTS(D3DTOP_BUMPENVMAP, pChar, size);
		CASE_TO_SPRINTS(D3DTOP_BUMPENVMAPLUMINANCE, pChar, size);
		CASE_TO_SPRINTS(D3DTOP_DOTPRODUCT3, pChar, size);
		CASE_TO_SPRINTS(D3DTOP_MULTIPLYADD, pChar, size);
		CASE_TO_SPRINTS(D3DTOP_LERP, pChar, size);
	default:
		sprintf_s(pChar, size, "NULL");
	}

	return pChar;
}

char* CTextureStageStateView::GetDTA(char* pChar, size_t size, DWORD value)
{
	switch (value)
	{
		CASE_TO_SPRINTS(D3DTA_SELECTMASK, pChar, size);
		CASE_TO_SPRINTS(D3DTA_DIFFUSE, pChar, size);
		CASE_TO_SPRINTS(D3DTA_CURRENT, pChar, size);
		CASE_TO_SPRINTS(D3DTA_TEXTURE, pChar, size);
		CASE_TO_SPRINTS(D3DTA_TFACTOR, pChar, size);
		CASE_TO_SPRINTS(D3DTA_SPECULAR, pChar, size);
		CASE_TO_SPRINTS(D3DTA_TEMP, pChar, size);
		CASE_TO_SPRINTS(D3DTA_CONSTANT, pChar, size);
		CASE_TO_SPRINTS(D3DTA_COMPLEMENT, pChar, size);
		CASE_TO_SPRINTS(D3DTA_ALPHAREPLICATE, pChar, size);
	default:
			sprintf_s(pChar, size, "NULL");
	}

	return pChar;
}