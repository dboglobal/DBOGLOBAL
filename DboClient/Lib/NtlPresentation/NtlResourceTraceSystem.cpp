#include "precomp_ntlpresentation.h"
#include "NtlResourceTraceSystem.h"

CNtlResourceTraceSystem::CNtlResourceTraceSystem(const RwChar* strName)
: CNtlResourceImVertexSystem(strName)
{
    m_nSystemType = RESOURCE_SYSTEMTYPE_TRACE;
    m_nMaxFrameCount = 0;
    m_fLifeTime = 1.0f;
}

CNtlResourceTraceSystem::~CNtlResourceTraceSystem(void)
{
    Destroy();
}

void CNtlResourceTraceSystem::Destroy()
{
    CNtlResourceImVertexSystem::Destroy();
}

RwBool CNtlResourceTraceSystem::Load(FILE* pFile)
{
    CNtlResourceImVertexSystem::Load(pFile);

    return TRUE;
}