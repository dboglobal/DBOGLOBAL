#pragma once

// Gui
#include "gui_particle_doc.h"

// CPropertiesPane
class CPropertiesPane : public CWnd
{
	DECLARE_DYNAMIC(CPropertiesPane)

public:
	CPropertiesPane();
	virtual ~CPropertiesPane();

	enum	eGridCategoryId
	{
		CATEGORY_COMMON,
		CATEGORY_EMITTER,
		CATEGORY_USEAFFECTOR,
		CATEGORY_GRAVITY,
		CATEGORY_COLOR,
		CATEGORY_PATH,
	};

	enum	eGridItemId
	{
		ITEM_PARTICLE_LIFE,
		ITEM_PARTICLE_LIMIT,

		// Box 일 경우
		ITEM_EMITTER_TYPE,

		ITEM_EMITTER_POSX,
		ITEM_EMITTER_POSY,

		// Box 일 경우에만 나옴
		ITEM_EMITTER_WIDTH,
		ITEM_EMITTER_HEIGHT,

		ITEM_EMITTER_EMISSIVITY,
		ITEM_EMITTER_LIFEMIN,
		ITEM_EMITTER_LIFEMAX,
		ITEM_EMITTER_VELMIN,
		ITEM_EMITTER_VELMAX,
		ITEM_EMITTER_DIRX,
		ITEM_EMITTER_DIRY,
		ITEM_EMITTER_ANGLE,
		ITEM_EMITTER_ROTRATEMIN,
		ITEM_EMITTER_ROTRATEMAX,
		ITEM_EMITTER_SCALEMIN,
		ITEM_EMITTER_SCALEMAX,
		ITEM_EMITTER_COLORMIN,
		ITEM_EMITTER_ALPHAMIN,
		ITEM_EMITTER_COLORMAX,
		ITEM_EMITTER_ALPHAMAX,
		ITEM_EMITTER_SURFACEFILE,
		ITEM_EMITTER_SURFACENAME,

		ITEM_USEAFFECTOR_GRAVITY,
		ITEM_USEAFFECTOR_COLOR,
		ITEM_USEAFFECTOR_PATH,

		ITEM_GRAVITY_DIRX,
		ITEM_GRAVITY_DIRY,
		ITEM_GRAVITY_WEIGHT,

		ITEM_COLOR_TRANSRED,
		ITEM_COLOR_ISTRANSRED,

		ITEM_COLOR_TRANSGREEN,
		ITEM_COLOR_ISTRANSGREEN,

		ITEM_COLOR_TRANSBLUE,
		ITEM_COLOR_ISTRANSBLUE,

		ITEM_COLOR_TRANSALPHA,
		ITEM_COLOR_ISTRANSALPHA,

		ITEM_PATH_TYPE,
		ITEM_PATH_TOTALTIME,
		ITEM_PATH_CTRLNUM,
		ITEM_PATH_CURVESUBNUM,
	};

	void		Show( BOOL bShow );
		
	void		SetProperty();
	void		SetActiveProperty( gui::CParticleItem* pItem );
	void		DeActiveProperty();

protected:
	gui::CParticleItem*		m_pActiveItem;

	CXTPPropertyGrid			m_PropGrid;

	// Categorys
	CXTPPropertyGridItemCategory*	m_pParticleItem;
	CXTPPropertyGridItemCategory*	m_pEmitterItem;
	CXTPPropertyGridItemCategory*	m_pUseAffectorItem;
	CXTPPropertyGridItemCategory*	m_pGravityItem;
	CXTPPropertyGridItemCategory*	m_pColorItem;
	CXTPPropertyGridItemCategory*	m_pPathItem;

	// Particle
	CXTPPropertyGridItemDouble*		m_pParLifeTime;
	CXTPPropertyGridItemNumber*		m_pParLimit; 
	
	// Emiter Point
	CXTPPropertyGridItemEnum*		m_pEmitType;
	CXTPPropertyGridItemNumber*		m_pEmitPosX;
	CXTPPropertyGridItemNumber*		m_pEmitPosY;
	CXTPPropertyGridItemNumber*		m_pEmitWidth;
	CXTPPropertyGridItemNumber*		m_pEmitHeight;
	CXTPPropertyGridItemNumber*		m_pEmissivity;
	CXTPPropertyGridItemDouble*		m_pLifeMin;
	CXTPPropertyGridItemDouble*		m_pLifeMax;
	CXTPPropertyGridItemDouble*		m_pVelMin;
	CXTPPropertyGridItemDouble*		m_pVelMax;
	CXTPPropertyGridItemDouble*		m_pEmitDirX;
	CXTPPropertyGridItemDouble*		m_pEmitDirY;
	CXTPPropertyGridItemDouble*		m_pEmitAngle;
	CXTPPropertyGridItemDouble*		m_pRotRateMin;
	CXTPPropertyGridItemDouble*		m_pRotRateMax;
	CXTPPropertyGridItemDouble*		m_pScaleMin;
	CXTPPropertyGridItemDouble*		m_pScaleMax;
	CXTPPropertyGridItemColor*		m_pColorMin;
	CXTPPropertyGridItemNumber*		m_pAlphaMin;
	CXTPPropertyGridItemColor*		m_pColorMax;
	CXTPPropertyGridItemNumber*		m_pAlphaMax;
	CXTPPropertyGridItem*			m_pSurfaceFile;
	CXTPPropertyGridItem*			m_pSurfaceName;

	// Use Affector
	CXTPPropertyGridItemBool*		m_pUseGravity;
	CXTPPropertyGridItemBool*		m_pUseColor;
	CXTPPropertyGridItemBool*		m_pUsePath;

	// Gravity Affector
	CXTPPropertyGridItemDouble*		m_pGravityDirX;
	CXTPPropertyGridItemDouble*		m_pGravityDirY;
	CXTPPropertyGridItemNumber*		m_pGravityWeight;

	// Gravity Color
	CXTPPropertyGridItemNumber*		m_pTransRed;
	CXTPPropertyGridItemBool*		m_pIsTransRed;
	CXTPPropertyGridItemNumber*		m_pTransGreen;
	CXTPPropertyGridItemBool*		m_pIsTransGreen;
	CXTPPropertyGridItemNumber*		m_pTransBlue;
	CXTPPropertyGridItemBool*		m_pIsTransBlue;
	CXTPPropertyGridItemNumber*		m_pTransAlpha;
	CXTPPropertyGridItemBool*		m_pIsTransAlpha;

	// Path Item
	CXTPPropertyGridItemEnum*		m_pPathType;
	CXTPPropertyGridItemDouble*		m_pPathTime;
	CXTPPropertyGridItemNumber*		m_pPathCtrl;
	CXTPPropertyGridItemNumber*		m_pCurveSub;
	
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	LRESULT OnGridNotify(WPARAM wParam, LPARAM lParam);
};


