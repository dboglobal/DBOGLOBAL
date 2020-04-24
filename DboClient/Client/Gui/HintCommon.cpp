#include "precomp_dboclient.h"
#include "HintCommon.h"

///////////////////////////////////////////////////////////////////////////////
// Class : CHint
///////////////////////////////////////////////////////////////////////////////

CHint::CHint()
: m_uiHintType( eHINT_TYPE_INVALID )
{
}

CHint::~CHint() 
{
}

CHint::eHintType CHint::GetType() 
{
	return m_uiHintType;
}

VOID CHint::SetType( eHintType uiHintType ) 
{
	m_uiHintType = uiHintType;
}

///////////////////////////////////////////////////////////////////////////////
// Class : CHelpHint
///////////////////////////////////////////////////////////////////////////////

CHelpHint::CHelpHint( RwUInt32 uiEventType, RwUInt32 uiTableIndex, eHintType byType /*= CHint::eHINT_TYPE_HELP */ ) 
: m_uiEventType(uiEventType)
, m_uiTableIndex(uiTableIndex)
{
	CHint::m_uiHintType = byType;
}

CHelpHint::~CHelpHint() 
{
}

RwUInt32 CHelpHint::GetEventType() 
{
	return m_uiEventType;
}

RwUInt32 CHelpHint::GetTableIndex() 
{
	return m_uiTableIndex;
}

///////////////////////////////////////////////////////////////////////////////
// Class : CStringHint
///////////////////////////////////////////////////////////////////////////////

CStringHint::CStringHint( const WCHAR* pwcTitle, const WCHAR* pwcBuffer, RwBool bImmediate /* = FALSE */,
						 RwReal fLifeTime /* = 0.0f */, eHintType byType /*= CHint::eHINT_TYPE_STRING*/ ) 
: m_bImmediate(bImmediate)
, m_fLifeTime(fLifeTime)
{
	CHint::m_uiHintType = byType;
	wcscpy_s( m_awcTitle, MAX_HINT_STRING_TITLE_LEN, pwcTitle );
	wcscpy_s( m_awcBuffer, MAX_HINT_STRING_BUFFER_LEN, pwcBuffer );
}

CStringHint::~CStringHint() 
{
}

const WCHAR* CStringHint::GetTitle() 
{
	return m_awcTitle;
}

const WCHAR* CStringHint::GetBuffer() 
{
	return m_awcBuffer;
}

RwBool CStringHint::GetImmdiate() 
{
	return m_bImmediate;
}

RwReal CStringHint::GetLifeTime() 
{
	return m_fLifeTime;
}