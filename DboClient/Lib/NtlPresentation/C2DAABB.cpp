#include "precomp_ntlpresentation.h"
#include "C2DAABB.h"


C2DAABB::C2DAABB(void)
{
	m_Min.x	= RwRealMAXVAL;
	m_Min.y	= RwRealMAXVAL;
	m_Max.x	= -RwRealMAXVAL;
	m_Max.y	= -RwRealMAXVAL;
}

C2DAABB::~C2DAABB(void)
{
}

RwBool C2DAABB::CollisionTest(RwV2d& _Pos)
{
	if((_Pos.x < m_Max.x && _Pos.y < m_Max.y) && (_Pos.x > m_Min.x && _Pos.y > m_Min.y))
	{
		return TRUE;		
	}

	return FALSE;
}

RwBool C2DAABB::CollisionTest(C2DAABB& _CompareAABB)
{
	RwV2d BBSizeConvex;
	RwV2d BBSizeAdding;

	BBSizeConvex.x = (_CompareAABB.m_Max.x > m_Max.x ? _CompareAABB.m_Max.x : m_Max.x) - (_CompareAABB.m_Min.x < m_Min.x ? _CompareAABB.m_Min.x : m_Min.x);
	BBSizeConvex.y = (_CompareAABB.m_Max.y > m_Max.y ? _CompareAABB.m_Max.y : m_Max.y) - (_CompareAABB.m_Min.y < m_Min.y ? _CompareAABB.m_Min.y : m_Min.y);
	BBSizeAdding.x = (_CompareAABB.m_Max.x - _CompareAABB.m_Min.x) + (m_Max.x - m_Min.x);
	BBSizeAdding.y = (_CompareAABB.m_Max.y - _CompareAABB.m_Min.y) + (m_Max.y - m_Min.y);

	if(BBSizeConvex.x > BBSizeAdding.x)	return FALSE;
	if(BBSizeConvex.y > BBSizeAdding.y)	return FALSE;

	return TRUE;
}

VOID C2DAABB::SetNewMinMax(RwV2d& _Pos)
{
	if(m_Min.x > _Pos.x)
	{
		m_Min.x = _Pos.x;
	}

	if(m_Min.y > _Pos.y)
	{
		m_Min.y = _Pos.y;
	}

	if(m_Max.x < _Pos.x)
	{
		m_Max.x = _Pos.x;
	}

	if(m_Max.y < _Pos.y)
	{
		m_Max.y = _Pos.y;
	}

	// make a rough convex
	m_Min.x += -0.1f;
	m_Min.y += -0.1f;
	m_Max.x += 0.1f;
	m_Max.y += 0.1f;
}

VOID C2DAABB::SetNewMinMax(RwV2d& _Min, RwV2d& _Max)
{
	if(_Min.x < m_Min.x)
	{
		m_Min.x = _Min.x;
	}

	if(_Min.y < m_Min.y)
	{
		m_Min.y = _Min.y;
	}

	if(m_Max.x < _Max.x)
	{
		m_Max.x = _Max.x;
	}

	if(m_Max.y < _Max.y)
	{
		m_Max.y = _Max.y;
	}
}