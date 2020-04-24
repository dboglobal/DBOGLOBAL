/*****************************************************************************
 * File			: NtlPLGuiGroup.h
 * Author		: Hong Sungbock
 * Copyright	: (주)NTL
 * Date			: 2007. 12. 13
 * Abstract		: PL단의 Gui group을 생성한다
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#pragma once

class CNtlPLSceneChangerGui;
class CNtlPLProfilerGui;


class CNtlPLGuiGroup
{
public:
	CNtlPLGuiGroup();
	~CNtlPLGuiGroup();

	RwBool	Create(void);
	void	Destroy(void);

protected:
	CNtlPLSceneChangerGui*		m_pSceneChanger;
	CNtlPLProfilerGui*			m_pProfilerGui;
};
