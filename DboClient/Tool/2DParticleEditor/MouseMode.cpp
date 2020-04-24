#include "StdAfx.h"
#include "MouseMode.h"

CMouseMode::CMouseMode(void)
{
	m_byMode = 0xFF;
}

CMouseMode::~CMouseMode(void)
{
}

void CMouseMode::SetMode( RwUInt8 byMode )
{
	m_byMode = byMode;
}

RwUInt8 CMouseMode::GetMode()
{
	return m_byMode;
}