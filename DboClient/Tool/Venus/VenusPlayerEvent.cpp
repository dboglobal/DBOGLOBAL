//***********************************************************************************
//	File		:	VenusPlayerEvent.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#include "StdAfx.h"
#include ".\venusplayerevent.h"

CVenusPlayerEvent::CVenusPlayerEvent(const RwChar* strName)
{
	m_strName = strName;
}

CVenusPlayerEvent::~CVenusPlayerEvent(void)
{
	m_Avatar.Delete();
}

void CVenusPlayerEvent::Delete()
{
	m_Avatar.Delete();
}

void CVenusPlayerEvent::DeleteEffectEntity()
{
	m_Avatar.DeleteEffectEntity();
}

void CVenusPlayerEvent::Create()
{
	m_Avatar.Create();
}

void CVenusPlayerEvent::CopyData(CVenusPlayerEvent* pSrcPlayerEvent)
{
	m_Avatar.CopyData(&pSrcPlayerEvent->m_Avatar);
}

//------------------------------------------------------------------
//	FuncName	: Update
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPlayerEvent::Update(RwReal fElapsedTime)
{
	m_Avatar.Update(fElapsedTime);
	return TRUE;
}

//------------------------------------------------------------------
//	FuncName	: Render
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPlayerEvent::Render()
{
	m_Avatar.Render();
	return TRUE;
}


//------------------------------------------------------------------
//	FuncName	: Save
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPlayerEvent::Save(FILE* pFile)
{
	return m_Avatar.Save(pFile);
}


//------------------------------------------------------------------
//	FuncName	: Load
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
RwBool CVenusPlayerEvent::Load(FILE* pFile)
{
	return m_Avatar.Load(pFile);
}