#include "stdafx.h"
#include "BuffLinkedObject.h"


CBuffLinkedObject::CBuffLinkedObject(CBuff* pBuff)
{
	Init();
	m_pBuffRef = pBuff;
}

CBuffLinkedObject::~CBuffLinkedObject()
{
	Destroy();
}


void CBuffLinkedObject::Init()
{
	m_pBuffRef = NULL;
	m_bIsBuffRemoved = false;
}

void CBuffLinkedObject::Destroy()
{
	m_pBuffRef = NULL;
}

CBuff* CBuffLinkedObject::GetBuffRef()
{
	return m_pBuffRef;
}