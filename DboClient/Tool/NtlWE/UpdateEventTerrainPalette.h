#pragma once

#include "NtlHistoryController.h"

class CNtlHistoryDataTerrainRaiseDrop : public CNtlHistoryData
{
public:
	RwV3d		vCenter;
	RwReal		fRadius;
	RwReal		fSensitivity;
};

class CNtlHistoryActionTerrain
{
protected:
	void				TerrainRaise(CNtlHistoryData* pHistoryData);
	void				TerrainDrop(CNtlHistoryData* pHistoryData);

	RwBool				CheckExceptionTerrainRaiseDrop(CNtlHistoryData* pHistoryData);

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlHistoryActionTerrainRaise : public CNtlHistoryAction, public CNtlHistoryActionTerrain
{
	virtual void		Undo(CNtlHistoryData* pHistoryData);
	virtual void		Redo(CNtlHistoryData* pHistoryData);
	virtual RwBool		CheckException(CNtlHistoryData* pHistoryData);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlHistoryActionTerrainDrop : public CNtlHistoryAction, public CNtlHistoryActionTerrain
{
	virtual void		Undo(CNtlHistoryData* pHistoryData);
	virtual void		Redo(CNtlHistoryData* pHistoryData);
	virtual RwBool		CheckException(CNtlHistoryData* pHistoryData);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CUpdateEventTerrainPalette
{
public:
	CUpdateEventTerrainPalette();
	~CUpdateEventTerrainPalette();

	virtual void UpdateTerrainPalette();
	virtual void RenderTerrainPalette();

	virtual void DeleteTerrainPaletteDummies();

	virtual void UpdateWorldSectorHeightInRectangle(RwReal Height);

protected:
	virtual void UpdateTerrainPaletteRaise();
	virtual void UpdateTerrainPaletteDrop();
	virtual void UpdateTerrainPalettePlateAU();
	virtual void UpdateTerrainPaletteJust();
	virtual void UpdateTerrainPaletteStandardize();
	virtual void UpdateTerrainPaletteSmooth();
	virtual void UpdateTerrainPaletteRange();
	virtual void UpdateTerrainPaletteRoad();
	virtual void UpdateTerrainPaletteCustomize();
	virtual void UpdateTerrainPaletteCustomizeE();
	virtual void UpdateTerrainPaletteSlope();

private:
	struct sTERRAIN_CUSTOMIZE_BRUSH_INFO
	{
		vector<sRECT>		m_vecMulti;
		CNtlWorldBrush*		m_pMultiSelection;
	};

	CNtlWorldBrush*					m_pBrushSelector;
	sTERRAIN_CUSTOMIZE_BRUSH_INFO	m_CustomizedTerrainBrush;

	CNtlHistoryController*			m_pNtlHistoryCtrl;
};