#ifndef __GUI_TEXTUREMANAGER_H__
#define __GUI_TEXTUREMANAGER_H__

#include "gui_define.h"

START_GUI

class CTexture;

class CTextureManager
{
public:

	int GetSortSize(int nSize);

public:

	CTextureManager();
	~CTextureManager();

	bool Initialize(VOID);
	VOID Terminate(VOID);
	CTexture* CreateTexture(const char *pFileName);
	CTexture* CreateTexture(int nWidth, int nHeight);
	VOID DeleteTexture(CTexture *pTexture);

private:

	int m_nImtexNum;
	std::map<std::string, CTexture*> m_mapTexture;
};

END_GUI

extern gui::CTextureManager g_TextureMgr;

#endif
