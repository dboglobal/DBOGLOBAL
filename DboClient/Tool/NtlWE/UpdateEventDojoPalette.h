#pragma once

#include "NtlHistoryController.h"

#include "NtlPLDojo.h"

class CUpdateEventDojoPalette
{
public:
	CUpdateEventDojoPalette();
	~CUpdateEventDojoPalette();

	virtual void				UpdateDojoPalette();
	virtual void				RenderDojoPalette();

	virtual void				DeleteDojoPaletteDummies();

	virtual void				RenderDisplayInfoDojoPalette(RtCharset* pRtCharSet, RwInt32 iWindowWidth, RwInt32 iWindowHeight);

	virtual CNtlPLDojo*			GetCurDojo();

private:
	std::vector<CNtlPLEntity*>	m_vecSelected;
	CNtlWorldBrush*				m_pBrushSelection;
};