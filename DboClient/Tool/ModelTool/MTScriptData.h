#ifndef _MTSCRIPTDATA_H
#define _MTSCRIPTDATA_H

#include "NtlPLCharacter.h"

/// 스크립트의 내용을 저장하는 구조체
struct SMTScriptData
{
    std::string     m_sClumpFileName;                   ///< 사용되는 Clump 파일명
    SBoneScaleData  m_BoneScaledata[MAX_NUM_BONES];     ///< Bone의 정보를 담고있는 구조체 배열
    RwInt32         m_nBoneNum;                         ///< Bone의 개수

    // todo. Anim 관련 데이터 설정 필요

    SMTScriptData()
    {
        m_sClumpFileName = "";
        ZeroMemory(m_BoneScaledata, sizeof(m_BoneScaledata));
        m_nBoneNum = 0;
    };
};


#endif