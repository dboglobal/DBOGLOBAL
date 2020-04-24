#include "StdAfx.h"
#include "NtlWEUtil.h"


CNtlWEUtil::CNtlWEUtil(void)
{
}

CNtlWEUtil::~CNtlWEUtil(void)
{
}

VOID CNtlWEUtil::DestroyModalessDialogs(CDialog* _pModalessDialogs)
{
	if(!_pModalessDialogs)
	{
		return;
	}

	_pModalessDialogs->DestroyWindow();
	delete _pModalessDialogs;
	_pModalessDialogs = NULL;
}


RwReal CNtlWEUtil::GetRwRealVal(CString& _Str)
{
	return static_cast<RwReal>(atof(_Str.GetBuffer(0)));
}

RwInt32	CNtlWEUtil::GetRwInt32Val(CString& _Str)
{
	return static_cast<RwInt32>(atoi(_Str.GetBuffer(0)));
}

CString	CNtlWEUtil::GetStr(RwReal& _Num)
{
	CString Ret;
	Ret.Format("%.2f", _Num);

	return Ret;
}

CString	CNtlWEUtil::GetStr(RwInt32& _Num)
{
	CString Ret;
	Ret.Format("%d", _Num);

	return Ret;
}

CSortData::CSortData(CString str, DWORD dw)
{
	m_strText	= str;
	m_dw		= dw;
}

int CALLBACK StringCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CSortData*	pLc1	= (CSortData*)lParam1;
	CSortData*	pLc2	= (CSortData*)lParam2;
	RwInt32		nReturn	= pLc1->m_strText.Compare(pLc2->m_strText);

	if(lParamSort)
	{
		return nReturn;
	}
	else
	{
		return -nReturn;
	}
}

int CALLBACK NumberCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CSortData*	pLc1	= (CSortData*)lParam1;
	CSortData*	pLc2	= (CSortData*)lParam2;
	RwInt32		LC1Val	= static_cast<RwInt32>(atof(pLc1->m_strText.GetBuffer(0)));
	RwInt32		LC2Val	= static_cast<RwInt32>(atof(pLc2->m_strText.GetBuffer(0)));
	RwInt32		nReturn	= LC1Val < LC2Val ? -1 : 1;

	if(lParamSort)
	{
		return nReturn;
	}
	else
	{
		return -nReturn;
	}
}