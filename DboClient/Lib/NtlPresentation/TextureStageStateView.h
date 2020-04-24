#pragma once

class CTextureStageStateView
{
public:
	CTextureStageStateView();
	virtual ~CTextureStageStateView();

	static void View(DWORD dwStage);

protected:
	static char* GetTSS(char* pChar, size_t size, D3DTEXTURESTAGESTATETYPE value); 
	static char* GetTOP(char* pChar, size_t size, D3DTEXTUREOP value); 
	static char* GetDTA(char* pChar, size_t size, DWORD value); 
};