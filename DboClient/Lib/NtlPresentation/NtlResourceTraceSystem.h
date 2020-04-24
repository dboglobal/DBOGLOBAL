#pragma once

#include "NtlResourceImVertexSystem.h"

/**
 * \ingroup NtlPresentation
 * \brief 궤적(Trace) 이펙트를 만들기 위한 리소스 클래스
 * \date 2006-09-14
 * \author agebreak
 */
class CNtlResourceTraceSystem : public CNtlResourceImVertexSystem
{
public:
    CNtlResourceTraceSystem(const RwChar* strName);
    virtual ~CNtlResourceTraceSystem(void);

    virtual void Destroy();

    virtual RwBool Load(FILE* pFile);           ///< 파일로 부터 데이터를 로드한다.

public:
    // 프로퍼티 속성들
    RwInt32 m_nMaxFrameCount;                   ///< 궤적을 그릴 최대 프레임 개수. 정확히는 면의 개수. 버텍스는 이값의 2배로 생성된다. (위아래 짝). 1프레임에 하나씩 위아래짝 라인이 그려진다.

protected:

protected:

};
