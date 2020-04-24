//***********************************************************************************
//	File		:	CVenusPropertyContainer.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	ⓒ 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once

#include "NtlPLPropertyContainer.h"

#include "NtlEffectSplinePath.h"
#include "NtlResourceEffect.h"
#include "NtlResourceParticleSystem.h"
#include "NtlResourceMeshSystem.h"
#include "NtlResourceBeamSystem.h"
#include "NtlResourceHurricaneSystem.h"
#include "NtlResourceLightningSystem.h"
#include "NtlResourceDecalSystem.h"
#include "NtlResourcePostEffectSystem.h"
#include "NtlResourceLineSystem.h"

class CVenusPropertyContainer : public CNtlPLPropertyContainer
{
public:
    enum ECopyPasteFlag             ///< 이펙트의 Copy&Paste에 사용되는 플래그
    {
        E_COPY_EFFECT,              ///< 이펙트를 카피한 경우
        E_COPY_SYSTEM,              ///< 이펙트 시스템을 카피한 경우
        E_COPY_FOLDER,              ///< 폴더를 카피한 경우
    };

public:
	LPDIRECT3DDEVICE9						m_lpDevice;

	typedef std::vector<CNtlPLProperty*>	svdef_Property;
	svdef_Property							m_svEffectProperty;    

	svdef_Property							m_svSplinePathProperty;


	RwBool									m_bSave;

	// Path
	typedef std::vector<RwInt32>			svdef_Int32;
	svdef_Int32								m_svSelectSplineControlPoint;

	CNtlEffectSplinePath*					m_pEffectSplinePath;

	RwIm3DVertex							m_SplinePointVertex[8];
	RwImVertexIndex							m_SplinePointIndices[36];

	RwReal									m_fControlPointRadius;

	RwBool									m_bCopyResourceData;
	CNtlResourceEffect*						m_pCopyResourceEffect;
	CNtlResourceComponentSystem*			m_pCopyResourceSystem;
	RwInt32									m_nCopySystemDataFlag;

	RwBool									m_bCopySplinePath;
	CNtlEffectSplinePath*					m_pCopySplinePath;

public:
	CVenusPropertyContainer(void);
	~CVenusPropertyContainer(void);

	static	CVenusPropertyContainer&	GetInstance(void);

	virtual RwBool	Create(void);
	virtual void	Destroy(void);
			void	Clear();			///< Effect에 관련된 Data들만 Clear한다. (지형 관련 프로퍼티는 삭제하지 않는다.)

			void	Render(void);

            void    OutputUsedMemory(); ///< 이펙트들이 사용하는 메모리들을 텍스트 파일로 출력한다.


	virtual RwBool					AddProp(ENtlPLEntityType eType, CNtlPLProperty *pProp);

	CNtlPLProperty*					CreateProperty(ENtlPLEntityType eType, const RwChar* strName, CNtlResourceEffect* pSrcResourceEffect = NULL);
	RwBool							DeleteProperty(ENtlPLEntityType eType, const RwChar* strName);
    CNtlResourceEffect*             FindProperty(const RwChar* strName);

	RwBool							IsResourceSystem(CNtlResourceEffect* pResourceEffect, const RwChar* strSystemName);

	CNtlResourceComponentSystem*	CreateResourceSystem(CNtlResourceEffect* pResourceEffect, RwInt8 nType, const RwChar* strName, CNtlResourceComponentSystem* pSrcSystem = NULL);
	RwBool							DeleteResourceSystem(CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceSystem);

	RwBool							ChangePropertyName(ENtlPLEntityType eType, const RwChar* strSrcName, const RwChar* strDestName);

	void							SetResourceEffectSize(CNtlResourceEffect* pResourceEffect, RwReal fScale);

	void							SetEmitterDataFlag(CNtlResourceComponentSystem* pDestSystem, RwUInt32 dataFlag, CNtlResourceComponentSystem* pSrcSystem = NULL);

	void							SetEmitterDataFlag_ComponentSystem(CNtlResourceComponentSystem* pDestSystem, RwUInt32 dataFlag, CNtlResourceComponentSystem* pSrcSystem = NULL);
	void							SetEmitterDataFlag_ParticleSystem(CNtlResourceParticleSystem* pDestParticleSystem, RwUInt32 dataFlag, CNtlResourceParticleSystem* pSrcParticleSystem = NULL);
	void							SetEmitterDataFlag_MeshSystem(CNtlResourceMeshSystem* pDestSystem, RwUInt32 dataFlag, CNtlResourceMeshSystem* pSrcSystem = NULL);
	void							SetEmitterDataFlag_ImVertexSystem(CNtlResourceImVertexSystem* pDestSystem, RwUInt32 dataFlag, CNtlResourceImVertexSystem* pSrcSystem = NULL);
	void							SetEmitterDataFlag_BeamSystem(CNtlResourceBeamSystem* pDestSystem, RwUInt32 dataFlag, CNtlResourceBeamSystem* pSrcSystem = NULL);
	void							SetEmitterDataFlag_HurricaneSystem(CNtlResourceHurricaneSystem* pDestSystem, RwUInt32 dataFlag, CNtlResourceHurricaneSystem* pSrcSystem = NULL);
	void							SetEmitterDataFlag_LightningSystem(CNtlResourceLightningSystem* pDestSystem, RwUInt32 dataFlag, CNtlResourceLightningSystem* pSrcSystem = NULL);
	void							SetEmitterDataFlag_DecalSystem(CNtlResourceDecalSystem* pDestSystem, RwUInt32 dataFlag, CNtlResourceDecalSystem* pSrcSystem = NULL);
	void							SetEmitterDataFlag_PostEffectSystem(CNtlResourcePostEffectSystem* pDestSystem, RwUInt32 dataFlag, CNtlResourcePostEffectSystem* pSrcSystem = NULL);    
    void                            SetEmitterDataFlag_LineSystem(CNtlResourceLineSystem* pDestSystem, RwUInt32 dataFlag, CNtlResourceLineSystem* pSrcSystem = NULL);    

	void							DeleteEmitterDataFlag(CNtlResourceComponentSystem* pResourceComponentSystem, RwUInt32 dataFlag);
	void							DeleteEmitterDataFlag_ParticleSystem(CNtlResourceParticleSystem* pResourceParticleSystem, RwUInt32 dataFlag);
	void							DeleteEmitterDataFlag_MeshSystem(CNtlResourceMeshSystem* pResourceMeshSystem, RwUInt32 dataFlag);
	void							DeleteEmitterDataFlag_ImVertexSystem(CNtlResourceImVertexSystem* pResourceImVertexSystem, RwUInt32 dataFlag);
	void							DeleteEmitterDataFlag_BeamSystem(CNtlResourceBeamSystem* pResourceBeamSystem, RwUInt32 dataFlag);
	void							DeleteEmitterDataFlag_HurricaneSystem(CNtlResourceHurricaneSystem* pResourceHurricaneSystem, RwUInt32 dataFlag);
	void							DeleteEmitterDataFlag_LightningSystem(CNtlResourceLightningSystem* pResourceLightningSystem, RwUInt32 dataFlag);
	void							DeleteEmitterDataFlag_DecalSystem(CNtlResourceDecalSystem* pResourceDecalSystem, RwUInt32 dataFlag);
	void							DeleteEmitterDataFlag_PostEffectSystem(CNtlResourcePostEffectSystem* pResourcePostEffectSystem, RwUInt32 dataFlag);    
    void                            DeleteEmitterDataFlag_LineSystem(CNtlResourceLineSystem* pResourceLineSystem, RwUInt32 dataFlag);

	void							InitialEmitterStandard(RpPrtStdEmitterStandard *pEmitterStandard);
	void							InitialEmitterStandard(SNtlMeshEmitterStandard *pEmitterStandard);
	void							InitialEmitterStandard(SNtlBeamEmitterStandard *pEmitterStandard);
	void							InitialEmitterStandard(SNtlHurricaneEmitterStandard *pEmitterStandard);
	void							InitialEmitterStandard(SNtlLightningEmitterStandard *pEmitterStandard);
	void							InitialEmitterStandard(SNtlDecalEmitterStandard *pEmitterStandard);
	void							InitialEmitterStandard(SNtlPostEffectEmitterStandard *pEmitterStandard);

	void							InitialEmitterPrtColor(RpPrtStdEmitterPrtColor* pEmitterPrtColor);
	void							InitialEmitterPrtTexCoords(RpPrtStdEmitterPrtTexCoords* pEmitterPrtTexCoords);
	void							InitialEmitterPrt2DRotate(RpPrtStdEmitterPrt2DRotate* pEmitterPrt2DRotate);
	void							InitialEmitterPrt2DRotate(SNtlPrtStdEmitterPrtRotate* pEmitterPrt2DRotate);
	void							InitialEmitterPrtSize(RpPrtStdEmitterPrtSize* pEmitterPrtSize);
	void							InitialEmitterPrtSize(SNtlPrtStdEmitterPrtSize* pEmitterPrtSize);
	void							InitialEmitterPrtMatrix(RpPrtStdEmitterPrtMatrix* pEmitterPrtMatrix);
	void							InitialEmitterPrtHurricane(SNtlPrtStdEmitterPrtHurricane* pEmitterPrtHurricane);
	void							InitialEmitterPrtOrbitPoint(SNtlPrtStdEmitterPrtOrbitPoint* pEmitterPrtOrbitPoint);
	void							InitialEmitterPrtFadeOut(SNtlPrtStdEmitterPrtFadeOut* pEmitterPrtFadeOut);

	void							InitialEmitterPointList(RpPrtAdvPointListEmitter* pPointListEmitter);
	void							InitialCircleEmitter(RpPrtAdvCircleEmitter* pCircleEmitter);
	void							InitialSphereEmitter(RpPrtAdvSphereEmitter* pSphereEmitter);

	void							InitialPrtEmitterEmitter(RpPrtAdvPrtEmitterEmitter* pPrtEmitterEmitter);
	void							InitialMultiColorEmitter(RpPrtAdvPrtMultiColorEmitter* pMultiColorEmitter);
	void							InitialMultiSizeEmitter(RpPrtAdvPrtMultiSizeEmitter* pMultiSizeEmitter);
	void							InitialMultiSizeEmitter(SNtlAdvMultiSizeEmitter* pMultiSizeEmitter);
	void							InitialMultiTexCoordsEmitter(RpPrtAdvEmtPrtMultiTexCoords* pMultiTexCoordsEmitter);
	void							InitialMultiTexturesEmitter(RpPrtAdvEmtPrtMultiTextures* pMultiTexturesEmitter);
	void							InitialMultiRotateEmitter(SNtlAdvMultiRotateEmitter* pMultiRotateEmitter);

	void							InitialEmitterSplinePath(SNtlPrtEmitterSplinePath* pEmitterSplinePath);

	void 							CopySystem_Component(CNtlResourceComponentSystem* pDestSystem, CNtlResourceComponentSystem* pSrcSystem);
	void 							CopySystem_Particle(CNtlResourceParticleSystem* pDestSystem, CNtlResourceParticleSystem* pSrcSystem);
	void 							CopySystem_Mesh(CNtlResourceMeshSystem* pDestSystem, CNtlResourceMeshSystem* pSrcSystem);
	void 							CopySystem_ImVertex(CNtlResourceImVertexSystem* pDestSystem, CNtlResourceImVertexSystem* pSrcSystem);
	void 							CopySystem_Beam(CNtlResourceBeamSystem* pDestSystem, CNtlResourceBeamSystem* pSrcSystem);
	void 							CopySystem_Hurricane(CNtlResourceHurricaneSystem* pDestSystem, CNtlResourceHurricaneSystem* pSrcSystem);
	void 							CopySystem_Lightning(CNtlResourceLightningSystem* pDestSystem, CNtlResourceLightningSystem* pSrcSystem);
	void							CopySystem_Decal(CNtlResourceDecalSystem* pDestSystem, CNtlResourceDecalSystem* pSrcSystem);
	void							CopySystem_PostEffect(CNtlResourcePostEffectSystem* pDestSystem, CNtlResourcePostEffectSystem* pSrcSystem);
    void                            CopySystem_Line(CNtlResourceLineSystem* pDestSystem, CNtlResourceLineSystem* pSrcSystem);

	RwBool							Save(const RwChar* strPathName);
	RwBool							Save(FILE* pFile, CNtlEffectSplinePath* pEffectSplinePath);
	RwBool							Save(FILE* pFile, CNtlResourceEffect* pResourceEffect);

	RwBool							Save(FILE* pFile, CNtlResourceComponentSystem* pResourceSystem);
	RwBool							Save(FILE* pFile, CNtlResourceParticleSystem* pResourceSystem);
	RwBool							Save(FILE* pFile, CNtlResourceMeshSystem* pResourceSystem);

	RwBool							Save(FILE* pFile, CNtlResourceImVertexSystem* pResourceSystem);
	RwBool							Save(FILE* pFile, CNtlResourceBeamSystem* pResourceSystem);
	RwBool							Save(FILE* pFile, CNtlResourceHurricaneSystem* pResourceSystem);
	RwBool							Save(FILE* pFile, CNtlResourceLightningSystem* pResourceSystem);
	RwBool							Save(FILE* pFile, CNtlResourceDecalSystem* pResourceSystem);
	RwBool							Save(FILE* pFile, CNtlResourcePostEffectSystem* pResourceSystem);
    RwBool                          Save(FILE* pFile, CNtlResourceLineSystem* pResourceLineSystem);

	RwBool							Save(FILE* pFile, std::string& string);


	RwBool							Load(const RwChar* strPathName);
	virtual RwBool					LoadSerialize(RwBool bEncrypt = FALSE, RwChar* szCryptPassword = NULL);				///< 바이너리로 컨버팅된 데이터들을 로드한다.

	void							Sort(RwBool bSequence);
	RwInt32							Sort(const RwChar* strSrc, const RwChar* strDest);


	//===================================================================================
	// VenusPropertyContainer_Spline.cpp
	//===================================================================================
	CNtlEffectSplinePath*			BuildSplinePath(CNtlEffectSplinePath* pEffectSpline, CNtlEffectSplinePath::INFO& Info, CNtlEffectSplinePath* pSrcSpline = NULL);

	void							BuildSplineCurvePoint(CNtlEffectSplinePath* pEffectSpline);
	void							BuildSplineCurveTime(CNtlEffectSplinePath* pEffectSpline);

	void							SetSplineVertexColor(RwRGBA& color);

	RwReal							GetDrawScaleFactor(const RwV3d* pViewPos, RwReal x, RwReal y, RwReal z, RwReal factor = 30.f);
	RwReal							ModulateCameraScaleMatrix(RwMatrix& matScale, const RwV3d* pViewPos, const RwV3d* pObjectPos, RwReal factor = 30.f);

	void							DrawSpline(RwV3d* pSrc, RwV3d* pDest, RwRGBA* pColor, RwMatrix* pWorldMatrix = NULL);
	void							DrawSpline(CNtlEffectSplinePath* pEffectSpline, RwMatrix* pWorldMatrix = NULL);

	void							IntersectionSplineControlPoint(POINT point, RwBool bShiftButton, RwBool bCtrlButton);
	void							IntersectionSplineControlPoint(POINT OldPoint, POINT CurPoint, RwBool bShiftButton, RwBool bCtrlButton);
	void							IntersectionSplineControlPoint(RwInt32 index, RwBool bShiftButton, RwBool bCtrlButton);

	void							EditSelectControlPoint(RwMatrix* pEditMatrix);
	void							AddSelectControlPoint(RwV3d* pEditVector);
	void							SetSelectControlPoint(RwV3d* pEditVector);
	void							SetSelectControlPoint_X(float x);
	void							SetSelectControlPoint_Y(float y);
	void							SetSelectControlPoint_Z(float z);

public:
	void	SetSelectSplinePath(CNtlEffectSplinePath* pEffectSplinePath);
	void	SetCopyResourceData(RwBool bCopy, CNtlResourceEffect* pResourceEffect = NULL, CNtlResourceComponentSystem* pResourceComponentSystem = NULL, RwInt32 nSystemDataFlag = 0);
	void	SetCopySplinePath(RwBool bCopy, CNtlEffectSplinePath* pCopySplinePath = NULL);
};


inline void CVenusPropertyContainer::SetSelectSplinePath(CNtlEffectSplinePath* pEffectSplinePath)
{
	m_svSelectSplineControlPoint.clear();
	m_pEffectSplinePath = pEffectSplinePath;
}

inline void CVenusPropertyContainer::SetCopyResourceData(RwBool bCopy, CNtlResourceEffect* pResourceEffect, CNtlResourceComponentSystem* pResourceComponentSystem, RwInt32 nSystemDataFlag)
{
	m_bCopyResourceData		= bCopy;
	m_pCopyResourceEffect	= pResourceEffect;
	m_pCopyResourceSystem	= pResourceComponentSystem;
	m_nCopySystemDataFlag	= nSystemDataFlag;
}

inline void CVenusPropertyContainer::SetCopySplinePath(RwBool bCopy, CNtlEffectSplinePath* pCopySplinePath)
{
	m_bCopySplinePath	= bCopy;
	m_pCopySplinePath	= pCopySplinePath;
}
