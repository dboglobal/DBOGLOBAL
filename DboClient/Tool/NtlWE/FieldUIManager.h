//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// class CFieldUIManager
// {
// friend class CMergeUIManager;
// 
// public:
// 	CFieldUIManager(void);
// 	virtual ~CFieldUIManager(void);
// 
// private:
// 	enum eFIELDUI_CASE
// 	{
// 		eFC_IDLE_MAP = 0,
// 		eFC_MOVE_MAP,
// 		eFC_DRAG_MAP,
// 	};
// 
// 	enum eFIELDUI_DIR
// 	{
// 		eFD_N = 0,
// 		eFD_E,
// 		eFD_S,
// 		eFD_W,
// 	};
// 
// private:
// 	struct sFIELD_UIMAP
// 	{
// 		RwIm2DVertex	_Panel[4];
// 		RwTexture*		_pTextureField;
// 		RECT			_Collision;
// 		RwReal			_FieldUVTabSize;
// 	};
// 
// 	struct sDRAG_INFO
// 	{
// 		RECT			_Rect;
// 		POINT			_SPos, _EPos;
// 		RwIm2DVertex	_Line[4];
// 		RwTexture*		_pTextureField;
// 		bool			_bRender;
// 	};
// 
// 	struct sCAM_INFO
// 	{
// 		RwIm2DVertex	_Line[4];
// 		RwTexture*		_pTextureCamera;
// 		bool			_bRender;
// 	};
// 
// 	struct sFIELD_INFO
// 	{
// 		RwIm2DVertex	_Line[4];
// 		bool			_bRender;
// 	};
// 
// private:
// 	RwInt32			m_FieldGridNum;
// 	RwInt32			m_FieldGridSize;
// 
// 	bool			m_Update;
// 	bool			m_Working;
// 	eFIELDUI_CASE	m_State;
// 	RwIm2DVertex	m_GridPanel[4];
// 	RwTexture*		m_pTextureGrid;
// 	sFIELD_UIMAP	m_FieldUIMap;
// 	sDRAG_INFO		m_FieldDrag;
// 	sCAM_INFO		m_CamInfo;
// 	sFIELD_INFO		m_FieldInfo;
// 
// private:
// 	RECT	GetCurFieldArea2World();
// 	POINT	GetFieldSPosInFieldUIMap(RwReal x, RwReal z);
// 
// public:
// 	RwBool  IsSingleSelected();
// 	RwBool  IsSelected();
// 	RwBool	IsCollidedInCurFieldArea(RECT& rt);
// 	RwBool	IsCollidedInCurFieldArea(RwInt32 nX, RwInt32 nY);
// 	RwBool	IsCollided(RwInt32 nX, RwInt32 nY);
// 	RwBool	IsWorking();
// 
// 	RwV3d	GetWorldPT(RwV3d& FieldUIMapPT);
// 	RwV3d	GetFieldMidPos();
// 	RECT	GetFieldDragInfo();
// 	RwInt32	GetFieldGridSize() { return m_FieldGridSize; }
// 	
// 	void	MoveFieldUIMap(eFIELDUI_DIR Dir);
// 	void	Switch(bool OnOff = TRUE) { m_Working = OnOff; }
// 
// 	void	Restore();
// 	void	Create();
// 	void	Delete();
// 	void	UpdateRenderInfo();
// 	void	Update();
// 	void	Render();
// 	void	RenderText(RtCharset* _pCharset);
// 
// 	void	LoadFieldTexture(const char* pName);
// };

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CFieldUIManager
{
	friend class CMergeUIManager;

public:
	CFieldUIManager(void);
	virtual ~CFieldUIManager(void);

private:
	enum eFIELDUI_CASE
	{
		eFC_IDLE_MAP = 0,
		eFC_MOVE_MAP,
		eFC_DRAG_MAP,
	};

	enum eFIELDUI_DIR
	{
		eFD_N = 0,
		eFD_E,
		eFD_S,
		eFD_W,
	};

private:
	struct sFIELD_UIMAP
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

private:
	RwInt32			m_iWorldFieldSize;
	RwInt32			m_iWorldFieldNum;

	RwInt32			m_FieldGridNum;
	RwInt32			m_FieldGridSize;

	bool			m_Update;
	bool			m_Working;
	eFIELDUI_CASE	m_State;
	RwIm2DVertex	m_GridPanel[4];
	RwTexture*		m_pTextureGrid;
	sFIELD_UIMAP	m_FieldUIMap;
	sDRAG_INFO		m_FieldDrag;
	sCAM_INFO		m_CamInfo;
	sFIELD_INFO		m_FieldInfo;

private:
	RECT	GetCurFieldArea2World();
	POINT	GetFieldSPosInFieldUIMap(RwReal x, RwReal z);

public:
	RwBool  IsSingleSelected();
	RwBool  IsSelected();
	RwBool	IsCollidedInCurFieldArea(RECT& rt);
	RwBool	IsCollidedInCurFieldArea(RwInt32 nX, RwInt32 nY);
	RwBool	IsCollided(RwInt32 nX, RwInt32 nY);
	RwBool	IsWorking();

	RwV3d	GetWorldPT(RwV3d& FieldUIMapPT);
	RwV3d	GetFieldMidPos();
	RECT	GetFieldDragInfo();
	RwInt32	GetFieldGridSize() { return m_FieldGridSize; }

	void	MoveFieldUIMap(eFIELDUI_DIR Dir);
	void	Switch(bool OnOff = TRUE) { m_Working = OnOff; }

	void	Restore();
	void	Create(RwInt32 iWorldFieldNum, RwInt32 iFieldSize);
	void	Delete();
	void	UpdateRenderInfo();
	void	Update();
	void	Render();
	void	RenderText(RtCharset* _pCharset);

	void	LoadFieldTexture(const char* pName);

	void	ApplyFieldProp(sNTL_FIELD_PROP* pNtlFieldProp, eNTL_FIELD_PROPID eNtlFieldPropID);
};