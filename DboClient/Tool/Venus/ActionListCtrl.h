#pragma once


// CActionListCtrl
#include "VenusGridCtrl.h"



#include "NtlEffectDefine.h"
#include "NtlResourceParticleSystem.h"
#include "NtlResourceMeshSystem.h"
#include "NtlResourceBeamSystem.h"
#include "NtlResourceHurricaneSystem.h"
#include "NtlResourceLightningSystem.h"
#include "NtlResourceDecalSystem.h"
#include "NtlResourcePostEffectSystem.h"
#include "NtlResourceLineSystem.h"

class CActionListCtrl : public CVenusGridCtrl
{
//	DECLARE_DYNAMIC(CActionListCtrl)

public:
	CImageList m_image;//Must provide an imagelist

	enum IMAGE_INDEX
	{
		IMAGE_INDEX_EMITTER_COLOR = 0x00,
		IMAGE_INDEX_EMITTER_TEXCOORDS,
		IMAGE_INDEX_EMITTER_2DROTATE,
		IMAGE_INDEX_EMITTER_SIZE,
		IMAGE_INDEX_EMITTER_MATRIX,
		IMAGE_INDEX_EMITTER_HURRICANE,
		IMAGE_INDEX_EMITTER_ORBITPOINT,
		IMAGE_INDEX_EMITTER_FADE_OUT,
		IMAGE_INDEX_EMITTER_UVANIM,

		IMAGE_INDEX_EMITTER_EMITTER,
		IMAGE_INDEX_EMITTER_MULTI_COLOR,
		IMAGE_INDEX_EMITTER_MULTI_SIZE,
		IMAGE_INDEX_EMITTER_MULTI_ROTATE,
		IMAGE_INDEX_EMITTER_MULTI_TEXCOORDS,

		IMAGE_INDEX_EMITTER_MULTI_TEXTURES,

		IMAGE_INDEX_EMITTER_SPLINE_PATH,

		IMAGE_INDEX_POINT_EMITTER,
		IMAGE_INDEX_CIRCLE_EMITTER,
		IMAGE_INDEX_SPHERE_EMITTER,   

        IMAGE_INDEX_EMITTER_FINISH,
        IMAGE_INDEX_EMITTER_TRAIL,
	};

	BOOL		m_bListUpdate;

public:
	CActionListCtrl();
	virtual ~CActionListCtrl();

	void	InitializeGrid();

	RwUInt32	GetSelectResourceData();
	RwInt32		GetImageIndexFromDataFlag(RwUInt32 nDataFlag);

	void	ResetResourceSystem();

protected:
	void	SetResourceSystem(CNtlResourceParticleSystem* pResourceSystem);
	void	SetResourceSystem(CNtlResourceImVertexSystem* pResourceSystem);
	void	SetResourceSystem(CNtlResourceMeshSystem* pResourceSystem);
	void	SetResourceSystem(CNtlResourceBeamSystem* pResourceSystem);
	void	SetResourceSystem(CNtlResourceHurricaneSystem* pResourceSystem);
	void	SetResourceSystem(CNtlResourceLightningSystem* pResourceSystem);
	void	SetResourceSystem(CNtlResourceDecalSystem* pResourceSystem);
	void	SetResourceSystem(CNtlResourcePostEffectSystem* pResourceSystem);
    void    SetResourceSystem(CNtlResourceLineSystem* pResourceSystem);

	void	CopyAction(RwInt32 nImage, RwUInt32 nEmitterDataFlag, DWORD_PTR dataPtr);
	void	CreateAction(RwInt32 nImage, RwUInt32 nEmitterDataFlag, DWORD_PTR dataPtr);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

	afx_msg void OnDeleteAction();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	
    afx_msg void OnInsertAttachSystem();
	afx_msg void OnInsertCircleEmitter();
	afx_msg void OnInsertEmitter2drotate();
	afx_msg void OnInsertEmitterColor();
	afx_msg void OnInsertEmitterFadeOut();
	afx_msg void OnInsertEmitterHurricane();
	afx_msg void OnInsertEmitterMatrix();
	afx_msg void OnInsertEmitterMultiColor();
	afx_msg void OnInsertEmitterMultiRotate();
	afx_msg void OnInsertEmitterMultiSize();
	afx_msg void OnInsertEmitterMultiTexcoords();
	afx_msg void OnInsertEmitterMultiTextures();
	afx_msg void OnInsertEmitterOrbitPoint();
	afx_msg void OnInsertEmitterSize();
	afx_msg void OnInsertEmitterSplinePath();
	afx_msg void OnInsertEmitterTexcoords();
    afx_msg void OnInsertEmitterFinish();
	afx_msg void OnInsertPointEmitter();
	afx_msg void OnInsertSphereEmitter();
	afx_msg void OnInsertEmitterTrail();
	afx_msg void OnInsertEmitterUvanim();

	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
    
};


