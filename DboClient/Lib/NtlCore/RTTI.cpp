#include "precomp_ntlcore.h"
#include "rtti.h"


CNtlRTTI::CNtlRTTI(const std::string& className) : m_className(className), m_pBaseRTTI(NULL)
{
}

CNtlRTTI::CNtlRTTI(const std::string& className, const CNtlRTTI& baseRTTI) : m_className(className), m_pBaseRTTI(&baseRTTI)
{
}

const std::string& CNtlRTTI::GetClassName() const
{
	return m_className;
}

bool CNtlRTTI::IsExactly(const CNtlRTTI& rtti) const
{
	return (this == &rtti);
}

bool CNtlRTTI::DerivesFrom(const CNtlRTTI& rtti) const
{
    const CNtlRTTI *pCompare = this;

    while(pCompare != NULL)
    {
        if(pCompare == &rtti)
            return true;
		
        pCompare = pCompare->m_pBaseRTTI;
    }

    return false;
}
