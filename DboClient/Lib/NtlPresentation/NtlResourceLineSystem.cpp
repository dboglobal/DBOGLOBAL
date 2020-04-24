#include "precomp_ntlpresentation.h"
#include "NtlDebug.h"
#include "NtlResourceEffect.h"
#include "NtlResourceLineSystem.h"

CNtlResourceLineSystem::CNtlResourceLineSystem(const RwChar* strName)
: CNtlResourceImVertexSystem(strName)
{
    m_nSystemType = RESOURCE_SYSTEMTYPE_LINE;
    
}

CNtlResourceLineSystem::~CNtlResourceLineSystem(void)
{
    Destroy();    
}

void CNtlResourceLineSystem::Destroy()
{
    if(CNtlResourceEffect::m_nVersion > 0x00000003)
    {
        NTL_ARRAY_DELETE(m_EmitterPrtMultiRotate.list);
    }

    CNtlResourceImVertexSystem::Destroy();
}

RwBool CNtlResourceLineSystem::Load(FILE* pFile)
{
    CNtlResourceImVertexSystem::Load(pFile);

    fread(&m_EmitterStandard, sizeof(SNtlLineEmitterStandard), 1, pFile);

  //  // Converting
    if(CNtlResourceEffect::m_nVersion == 0x00000003)
    {
        fread(&m_EmitterSphere, sizeof(SNtlPrtStdEmitterPrtSphere), 1, pFile);        
        fread(&m_EmitterPrtRotate, sizeof(SNtlPrtStdEmitterPrtRotate), 1, pFile);        
        fread(&m_EmitterPrtMultiRotate,		sizeof(SNtlAdvMultiRotateEmitter),				1,				pFile);
        return TRUE;
    }

    if(IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGSPHERE))
    {
        fread(&m_EmitterSphere, sizeof(SNtlPrtStdEmitterPrtSphere), 1, pFile);  
    }

    if(IsEmitterDataFlag(rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))
    {
        fread(&m_EmitterPrtRotate, sizeof(SNtlPrtStdEmitterPrtRotate), 1, pFile);        
    }

    if(IsEmitterDataFlag(rpPRTADVEMITTERDATAFLAGMULTIROTATE))
    {
        fread(&m_EmitterPrtMultiRotate.numRotate,		sizeof(RwUInt32),				1,				pFile);
        if (m_EmitterPrtMultiRotate.numRotate > 0)
        {
            m_EmitterPrtMultiRotate.list = NTL_NEW SNtlAdvEmtPrtRotateItem[m_EmitterPrtMultiRotate.numRotate];
            NTL_ASSERTE(m_EmitterPrtMultiRotate.list);
            fread(m_EmitterPrtMultiRotate.list,	sizeof(SNtlAdvEmtPrtRotateItem),	m_EmitterPrtMultiRotate.numRotate,			pFile);
        }  
    }

    return TRUE;
}