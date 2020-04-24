#include "precomp_ntlcore.h"
#include "callstack.h"
#include "debugmacros.h"
#include "NtlCoreUtil.h"

CNtlCallStack::CNtlCallStack()
{
	memset(m_Depth, 0, sizeof(char)*ECsDepth*ECSLen);
}

CNtlCallStack* CNtlCallStack::GetInstance(void)
{
	static CNtlCallStack cs;
	return &cs;
}

void CNtlCallStack::Push(const char *pString)
{
	int iDepth = RWS::_GetCallDepth();
	if(iDepth >= ECsDepth)
		return;

	sprintf_s(m_Depth[iDepth], ECSLen, "%s", pString);
}

void CNtlCallStack::LogFilePrint(void)
{
	NtlLogFilePrint(" === Dbo Function CallStack Start === ");

	int iDepth = RWS::_GetCallDepth();
	if(iDepth >= ECsDepth)
		iDepth = ECsDepth;

	for(int i = 0; i < iDepth; ++i)
		NtlLogFilePrint(m_Depth[i]);

	NtlLogFilePrint(" === Dbo Function CallStack End === ");
}