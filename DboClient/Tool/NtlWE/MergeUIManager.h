#pragma once


class CFieldUIManager;
class CProgressWnd;


#include "FileController.h"


class CMergeUIManager : public CFileController
{
friend class CFieldUIManager;

public:
	CMergeUIManager(void);
	~CMergeUIManager(void);

private:
	enum eMERGEUI_CASE
	{
		eFC_IDLE_MAP = 0,
		eFC_MOVE_MAP,
		eFC_DRAG_MAP,
	};

	enum eMERGEUI_DIR
	{
		eFD_N = 0,
		eFD_E,
		eFD_S,
		eFD_W,
	};

private:
	struct sMERGE_UIMAP
	{
		RwIm2DVertex	_Panel[4];
		RwTexture*		_pTextureField;
		RECT			_Collision;
		RwReal			_FieldUVTabSize;
	};

	struct sDRAG_INFO
	{
		RECT			_Rect;
		POINT			_SPos, _EPos;
		RwIm2DVertex	_Line[4];
		RwTexture*		_pTextureField;
		bool			_bRender;
	};

	struct sCAM_INFO
	{
		RwIm2DVertex	_Line[4];
		RwTexture*		_pTextureCamera;
		bool			_bRender;
	};

	struct sFIELD_INFO
	{
		RwIm2DVertex	_Line[4];
		bool			_bRender;
	};

	struct sMERGED_INFO
	{
		RwIm2DVertex	_VertBuf[4];
		RwTexture*		_pMergedTex;
		vector<RwInt32>	_vecMergedField;
	};

private:
	RwInt32			m_FieldGridNum;
	RwInt32			m_FieldGridSize;
	bool			m_Update;
	bool			m_Working;
	eMERGEUI_CASE	m_State;
	RwIm2DVertex	m_GridPanel[4];
	RwTexture*		m_pTextureGrid;
	sMERGE_UIMAP	m_MergeUIMap;
	sDRAG_INFO		m_MergeDrag;
	sCAM_INFO		m_CamInfo;
	sFIELD_INFO		m_FieldInfo;
	sMERGED_INFO	m_MergedInfo;

private:
	RECT	GetCurFieldArea2World();
	POINT	GetFieldSPosInFieldUIMap(RwReal x, RwReal z);

public:
	RwBool  IsMergeEnable();
	RwBool	IsSelected();
	RwBool	IsCollidedInCurFieldArea(RECT& rt);
	RwBool	IsCollidedInCurFieldArea(RwInt32 nX, RwInt32 nY);
	RwBool	IsCollided(RwInt32 nX, RwInt32 nY);
	RwBool	IsWorking();
	RwBool  IsSingleSelected();

	RwV3d	GetWorldPT(RwV3d& MERGEUIMapPT);
	RwV3d	GetPortalPT();
	RECT	GetFieldDragInfo();
	RwV3d	GetFieldMidPos();
	RwInt32	GetCurDragFieldsNum() { return m_MergedInfo._vecMergedField.size(); }

	void	MoveFieldUIMap(eMERGEUI_DIR Dir);
	void	Switch(bool OnOff = TRUE) { m_Working = OnOff; }

	void	Restore();
	void	Create();
	void	Delete();
	VOID	InsertMergedArea(RwV3d& FieldPos);

	RwBool	UpdateMerge(RwV3d& SFieldDatumPos, RwV3d& EFieldDatumPos, RwV3d& SFieldDatumPos4CurWorld, RwV3d& EFieldDatumPos4CurWorld, RwBool Singular = TRUE);
	VOID	UpdateMergeHistory(RwChar *fmt, ... );
	void	UpdateRenderInfo();
	void	Update(CFieldUIManager* _pFieldUIManager);

	VOID	RenderMergeArea();
	void	Render();
	void	RenderText(RtCharset* _pCharset);

	void	LoadFieldTexture(const char* pName);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////