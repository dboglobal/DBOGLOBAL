//***********************************************************************************
//	File		:	VenusPlayerEvent.h
//	Desc		:	
//	Begin		:	2005. 7.28
//	Copyright	:	¨Ï 2005 by agebreak CO., Ltd
//	Author		:	agebreak
//	Update		:	
//***********************************************************************************

#pragma once
#include <string>

#include <rwcore.h>

#include "VenusPlayer.h"

class CVenusPlayerEvent
{
public:
	std::string			m_strName;

	CVenusPlayer		m_Avatar;

public:
	CVenusPlayerEvent(const RwChar* strName);
	~CVenusPlayerEvent(void);

	void	Delete();
	void	DeleteEffectEntity();
	void	Create();

	void	CopyData(CVenusPlayerEvent* pSrcPlayerEvent);

	RwBool	Update(RwReal fElapsedTime);
	RwBool	Render();

	RwBool	Save(FILE* pFile);
	RwBool	Load(FILE* pFile);

public:
	const RwChar*	GetName()									{ return m_strName.c_str(); }
			void	SetName(const RwChar* strName)				{ m_strName = strName; }

};
