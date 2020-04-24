#pragma once

#include "NtlResourceImVertexSystem.h"

/**
 * \ingroup NtlPresentation
 * \brief 라인(두께를 가진 선) 이펙트를 만들기 위한 리소스 클래스
 * \date 2006-09-21
 * \author agebreak
 */
class CNtlResourceLineSystem : public CNtlResourceImVertexSystem
{
public:
    CNtlResourceLineSystem(const RwChar* strName);
    virtual ~CNtlResourceLineSystem(void);

    virtual void Destroy();

    virtual RwBool Load(FILE* pFile);   ///< 파일로부터 데이터를 로드한다.

public:   
    SNtlLineEmitterStandard         m_EmitterStandard;          ///< Line System의 기본 속성값들    
    
    SNtlPrtStdEmitterPrtSphere      m_EmitterSphere;            ///< Sphere Emitter
    SNtlPrtStdEmitterPrtRotate		m_EmitterPrtRotate;         ///< Rotate Emitter
    SNtlAdvMultiRotateEmitter		m_EmitterPrtMultiRotate;    ///< Multi Rotate

};
