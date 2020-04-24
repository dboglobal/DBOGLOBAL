#pragma once


// CEffectPropertyDialog 대화 상자입니다.
#include "PropTree.h"

#include "NtlEffectDefine.h"
#include "NtlResourceParticleSystem.h"
#include "NtlResourceMeshSystem.h"
#include "NtlResourceBeamSystem.h"
#include "NtlResourceHurricaneSystem.h"
#include "NtlResourceLightningSystem.h"
#include "NtlResourceDecalSystem.h"
#include "NtlResourcePostEffectSystem.h"
#include "NtlResourceLineSystem.h"

#include "NtlResourceEffect.h"

/// 빌보드 설정에 쓰이는 Flag
enum EBillBoardFlag
{
    BILLBOARD_NONE = 0,
    BILLBOARD_Y,
    BILLBOARD_X,
    BILLBOARD_Z,
};

class CEffectPropertyDialog : public CDialog
{
	DECLARE_DYNCREATE(CEffectPropertyDialog)

public:
	CEffectPropertyDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEffectPropertyDialog();

public:
	BOOL							m_Initialized;
	CPropTree						m_PropertyTree;

	RwUInt32						m_nCurrentPoint;

	enum TIME_TYPE
	{
		TIME_TYPE_RESET_PROPERTIES = 0x00,
		TIME_TYPE_CHANGE_SYSTEMNAME,
		TIME_TYPE_CHANGE_EFFECTNAME
	};

public:

	void	ResetProperties(RwUInt32 nCurrentPoint = 0);

	void	SetProperties_ResourceEffect(CNtlResourceEffect* pResourceEffect, CPropTreeItem* pRoot);

	void	SetProperties_ResourceComponentSystem(CNtlResourceComponentSystem* pResourceComponentSystem, CPropTreeItem* pRoot);


	void	SetProperties_ResourceParticleSystem(CNtlResourceParticleSystem* pResourceParticleSystem, CPropTreeItem* pRoot);
	void	SetProperties_EmitterStandard(CNtlResourceParticleSystem* pResourceParticleSystem, CPropTreeItem* pRoot);

	void	SetProperties_EmitterPrtColor(RpPrtStdEmitterPrtColor* pEmitterPrtColor, CPropTreeItem* pRoot);
	void	SetProperties_EmitterPrtTexCoords(RpPrtStdEmitterPrtTexCoords* pEmitterPrtTexCoords, CPropTreeItem* pRoot);
	void	SetProperties_EmitterPrt2DRotate(RpPrtStdEmitterPrt2DRotate* pEmitterPrt2DRotate, CPropTreeItem* pRoot);
	void	SetProperties_EmitterPrt2DRotate(SNtlPrtStdEmitterPrtRotate* pEmitterPrt2DRotate, CPropTreeItem* pRoot);
	void	SetProperties_EmitterPrtSize(RpPrtStdEmitterPrtSize* pEmitterPrtSize, CPropTreeItem* pRoot);
	void	SetProperties_EmitterPrtSize(SNtlPrtStdEmitterPrtSize* pEmitterPrtSize, CPropTreeItem* pRoot);
	void	SetProperties_EmitterPrtMatrix(RpPrtStdEmitterPrtMatrix* pEmitterPrtMatrix, CPropTreeItem* pRoot, RwUInt32 nEmitterFlag);
	void	SetProperties_EmitterPrtHurricane(SNtlPrtStdEmitterPrtHurricane* pEmitterPrtHurricane, CPropTreeItem* pRoot);
	void	SetProperties_EmitterPrtOrbitPoint(SNtlPrtStdEmitterPrtOrbitPoint* pEmitterPrtOrbitPoint, CPropTreeItem* pRoot);
	void	SetProperties_EmitterPrtFadeOut(SNtlPrtStdEmitterPrtFadeOut* pEmitterPrtFadeOut, CPropTreeItem* pRoot);
    void    SetProperties_EmitterPrtFinish(SNtlPrtStdEmitterPrtFinish* pEmitterPrtFinish, CPropTreeItem* pRoot);
    void    SetProperties_EmitterTrail(SNtlPrtStdEmitterTrail* pEmitterTrail, CPropTreeItem* pRoot);
	void	SetProperties_EmitterUVAnim(SNtlPrtStdEmitterPrtUVAnim* pEmitterUVAnim, CPropTreeItem* pRoot);

	void	SetProperties_PrtEmitterEmitter(RpPrtAdvPrtEmitterEmitter* pPrtEmitterEmitter, CPropTreeItem* pRoot);
	void	SetProperties_MultiColorEmitter(RpPrtAdvPrtMultiColorEmitter* pMultiColorEmitteer, CPropTreeItem* pRoot);
	void	SetProperties_MultiSizeEmitter(RpPrtAdvPrtMultiSizeEmitter* pEmitterPrtSize, CPropTreeItem* pRoot);
	void	SetProperties_MultiSizeEmitter(SNtlAdvMultiSizeEmitter* pMultiSizeEmitter, CPropTreeItem* pRoot);
	void	SetProperties_MultiRotateEmitter(SNtlAdvMultiRotateEmitter* pMultiRotateEmitter, CPropTreeItem* pRoot);
	void	SetProperties_MultiTexCoordsEmitter(RpPrtAdvEmtPrtMultiTexCoords* pMultiTexCoordsEmitter, CPropTreeItem* pRoot);

	void	SetProperties_MultiTexturesEmitter(RpPrtAdvEmtPrtMultiTextures* pMultiTexturesEmitter, CPropTreeItem* pRoot);

	void	SetProperties_SplinePathEmitter(SNtlPrtEmitterSplinePath* pSplinePathEmitter, CPropTreeItem* pRoot);

	void	SetProperties_EmitterPointList(RpPrtAdvEmtPointList* pEmitterPointList, CPropTreeItem* pRoot);
	void	SetProperties_CircleEmitter(RpPrtAdvCircleEmitter* pCircleEmitter, CPropTreeItem* pRoot);
	void	SetProperties_SphereEmitter(RpPrtAdvSphereEmitter* pSphereEmitter, CPropTreeItem* pRoot);
    void    SetProperties_SphereEmitter(SNtlPrtStdEmitterPrtSphere* pSphereEmitter, CPropTreeItem* pRoot);


	void	SetProperties_ResourceImVertexSystem(CNtlResourceImVertexSystem* pResourceImVertexSystem, CPropTreeItem* pRoot);

	void	SetProperties_ResourceMeshSystem(CNtlResourceMeshSystem* pResourceMeshSystem, CPropTreeItem* pRoot);
	void	SetProperties_EmitterStandard(CNtlResourceMeshSystem* pResourceMeshSystem, CPropTreeItem* pRoot);

	void	SetProperties_ResourceBeamSystem(CNtlResourceBeamSystem* pResourceBeamSystem, CPropTreeItem* pRoot);
	void	SetProperties_EmitterStandard(CNtlResourceBeamSystem* pResourceBeamSystem, CPropTreeItem* pRoot);

	void	SetProperties_ResourceHurricaneSystem(CNtlResourceHurricaneSystem* pResourceHurricaneSystem, CPropTreeItem* pRoot);
	void	SetProperties_EmitterStandard(CNtlResourceHurricaneSystem* pResourceHurricaneSystem, CPropTreeItem* pRoot);

	void	SetProperties_ResourceLightningSystem(CNtlResourceLightningSystem* pResourceLightningSystem, CPropTreeItem* pRoot);
	void	SetProperties_EmitterStandard(CNtlResourceLightningSystem* pResourceLightningSystem, CPropTreeItem* pRoot);

	void	SetProperties_ResourceDecalSystem(CNtlResourceDecalSystem* pResourceDecalSystem, CPropTreeItem* pRoot);
	void	SetProperties_EmitterStandard(CNtlResourceDecalSystem* pResourceDecalSystem, CPropTreeItem* pRoot);

	void	SetProperties_ResourcePostEffectSystem(CNtlResourcePostEffectSystem* pResourcePostEffectSystem, CPropTreeItem* pRoot);
	void	SetProperties_EmitterStandard(CNtlResourcePostEffectSystem* pResourcePostEffectSystem, CPropTreeItem* pRoot);

    void    SetProperties_ResourceLineSystem(CNtlResourceLineSystem* pResourceLineSystem, CPropTreeItem* pRoot);
    void    SetProperties_EmitterStandard(CNtlResourceLineSystem* pResourceLineSystem, CPropTreeItem* pRoot);

	void	ApplyProperties(CPropTreeItem* pItem);

	void	ApplyProperties_ResourceEffect(CPropTreeItem* pItem, CNtlResourceEffect* pResourceEffect);

	void	ApplyProperties_ResourceComponentSystem(CPropTreeItem* pItem, CNtlResourceComponentSystem*	pResourceComponentSystem);

	void	ApplyProperties_ResourceParticleSystem(CPropTreeItem* pItem, CNtlResourceParticleSystem* pResourceParticleSystem);
	void	ApplyProperties_ResourceImVertexSystem(CPropTreeItem* pItem, CNtlResourceImVertexSystem* pResourceImVertexSystem);
	void	ApplyProperties_ResourceMeshSystem(CPropTreeItem* pItem, CNtlResourceMeshSystem* pResourceMeshSystem);
	void	ApplyProperties_ResourceBeamSystem(CPropTreeItem* pItem, CNtlResourceBeamSystem* pResourceBeamSystem);
	void	ApplyProperties_ResourceHurricaneSystem(CPropTreeItem* pItem, CNtlResourceHurricaneSystem* pResourceHurricaneSystem);
	void	ApplyProperties_ResourceLightningSystem(CPropTreeItem* pItem, CNtlResourceLightningSystem* pResourceLightningSystem);
	void	ApplyProperties_ResourceDecalSystem(CPropTreeItem* pItem, CNtlResourceDecalSystem* pResourceDecalSystem);
	void	ApplyProperties_ResourcePostEffectSystem(CPropTreeItem* pItem, CNtlResourcePostEffectSystem* pResourcePostEffectSystem);
    void    ApplyProperties_ResourceLineSystem(CPropTreeItem* pItem, CNtlResourceLineSystem* pResourceLineSystem);

	void	ApplyProperties_EmitterStandard(CPropTreeItem* pItem, CNtlResourceParticleSystem* pResourceParticleSystem);
	void	ApplyProperties_EmitterStandard(CPropTreeItem* pItem, CNtlResourceMeshSystem* pResourceMeshSystem);
	void	ApplyProperties_EmitterStandard(CPropTreeItem* pItem, CNtlResourceBeamSystem* pResourceBeamSystem);
	void	ApplyProperties_EmitterStandard(CPropTreeItem* pItem, CNtlResourceHurricaneSystem* pResourceHurricaneSystem);
	void	ApplyProperties_EmitterStandard(CPropTreeItem* pItem, CNtlResourceLightningSystem* pResourceLightningSystem);
	void	ApplyProperties_EmitterStandard(CPropTreeItem* pItem, CNtlResourceDecalSystem* pResourceDecalSystem);
	void	ApplyProperties_EmitterStandard(CPropTreeItem* pItem, CNtlResourcePostEffectSystem* pResourcePostEffectSystem);
    void    ApplyProperties_EmitterStandard(CPropTreeItem* pItem, CNtlResourceLineSystem* pResourceLineSystem);

	void	ApplyProperties_EmitterPrtColor(CPropTreeItem* pItem, RpPrtStdEmitterPrtColor* pEmitterPrtCol);
	void	ApplyProperties_EmitterPrtTexCoords(CPropTreeItem* pItem, RpPrtStdEmitterPrtTexCoords* pEmitterPrtTexCoords);
	void	ApplyProperties_EmitterPrt2DRotate(CPropTreeItem* pItem, RpPrtStdEmitterPrt2DRotate* pEmitterPrt2DRotate);
	void	ApplyProperties_EmitterPrt2DRotate(CPropTreeItem* pItem, SNtlPrtStdEmitterPrtRotate* pEmitterPrt2DRotate);
	void	ApplyProperties_EmitterPrtSize(CPropTreeItem* pItem, RpPrtStdEmitterPrtSize* pEmitterPtrSize);
	void	ApplyProperties_EmitterPrtSize(CPropTreeItem* pItem, SNtlPrtStdEmitterPrtSize* pEmitterPtrSize);
	void	ApplyProperties_EmitterPrtMatrix(CPropTreeItem* pItem, RpPrtStdEmitterPrtMatrix* pEmitterPrtMatrix, CNtlResourceParticleSystem* pResourceParticleSystem);
	void	ApplyProperties_EmitterPrtHurricane(CPropTreeItem* pItem, SNtlPrtStdEmitterPrtHurricane* pEmitterPrtHurricane);
	void	ApplyProperties_EmitterPrtOrbitPoint(CPropTreeItem* pItem, SNtlPrtStdEmitterPrtOrbitPoint* pEmitterPrtOrbitPoint);
	void	ApplyProperties_EmitterPrtFadeOut(CPropTreeItem* pItem, SNtlPrtStdEmitterPrtFadeOut* pEmitterPrtFadeOut);
    void    ApplyProperties_EmitterPrtFinish(CPropTreeItem* pItem, SNtlPrtStdEmitterPrtFinish* pEmitterPrtFinish);
    void    ApplyProperties_EmitterTrail(CPropTreeItem* pItem, SNtlPrtStdEmitterTrail* pEmitterTrail);
	void	ApplyProperties_EmitterPrtUVAnim(CPropTreeItem* pItem, SNtlPrtStdEmitterPrtUVAnim* pEmitterUVAnim);

	void	ApplyProperties_PrtEmitterEmitter(CPropTreeItem* pItem, RpPrtAdvPrtEmitterEmitter* pPrtEmitterEmitter);
	void	ApplyProperties_MultiColorEmitter(CPropTreeItem* pItem, RpPrtAdvPrtMultiColorEmitter* pEmitterPrtMultiCol);
	void	ApplyProperties_MultiSizeEmitter(CPropTreeItem* pItem, RpPrtAdvPrtMultiSizeEmitter* pEmitterPrtMultiSize);
	void	ApplyProperties_MultiSizeEmitter(CPropTreeItem* pItem, SNtlAdvMultiSizeEmitter* pEmitterPrtMultiSize);
	void	ApplyProperties_MultiRotateEmitter(CPropTreeItem* pItem, SNtlAdvMultiRotateEmitter* pEmitterPrtMultiRotate);
	void	ApplyProperties_MultiTexCoordsEmitter(CPropTreeItem* pItem, RpPrtAdvEmtPrtMultiTexCoords *pEmitterPrtMultiTexCoords);

	void	ApplyProperties_MultiTexturesEmitter(CPropTreeItem* pItem, RpPrtAdvEmtPrtMultiTextures *pEmitterPrtMultiTextures);

	void	ApplyProperties_SplinePathEmitter(CPropTreeItem* pItem, SNtlPrtEmitterSplinePath* pEmitterSplinePath);

	void	ApplyProperties_EmitterPointList(CPropTreeItem* pItem, RpPrtAdvPointListEmitter* pEmitterPtList);
	void	ApplyProperties_CircleEmitter(CPropTreeItem* pItem, RpPrtAdvCircleEmitter* pEmitterPtrCircle);
	void	ApplyProperties_SphereEmitter(CPropTreeItem* pItem, RpPrtAdvSphereEmitter* pEmitterPtrSphere);
    void	ApplyProperties_SphereEmitter(CPropTreeItem* pItem, SNtlPrtStdEmitterPrtSphere* pEmitterPtrSphere);



	CPropTreeItem*	SetProperties_Textures(std::string& strCurrentName, CPropTreeItem* pRoot);
    CPropTreeItem*  SetProperties_Sound(std::string& strSoundName, CPropTreeItem* pRoot);

// 대화 상자 데이터입니다.
	enum { IDD = IDD_EFFECT_PROPERTY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	afx_msg void OnItemChanged(NMHDR* pNotifyStruct, LRESULT* plResult);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT nIDEvent);
};
