#ifndef __GUI_TEXTURE_H__
#define __GUI_TEXTURE_H__

#include "gui_define.h"

START_GUI

class CTexture
{
private:



public:

	CTexture();
	virtual ~CTexture();

	bool Load(const char *pFileName);
	bool Load(RwTexture *pRwTexture);
	VOID Unload(VOID);

	const char* GetName(VOID); 
	int GetWidth(VOID);
	int	GetHeight(VOID);
	int GetStride(VOID);
	int GetFormat(VOID);
	bool IsMaskColor(int nX, int nY, unsigned int dwMaskColor = 0);

	unsigned char* LockWrite(VOID);
	VOID UnLock(VOID);

	VOID FillClear(VOID);
		
	RwTexture* GetTexture(VOID);

	bool IsValid(VOID);

private:

	RwTexture *m_pTexture;
};

END_GUI

#endif
