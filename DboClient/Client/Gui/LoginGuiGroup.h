/*****************************************************************************
 *
 * File			: LoginGuiGroup.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 9. 20	
 * Abstract		: DBO Login stage gui group.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __LOGIN_GUI_GROUP_H__
#define __LOGIN_GUI_GROUP_H__

class CLogoGui;
class CLogInGui;
class CMsgBoxGui;
class CContractGui;

class CLogInGuiGroup
{
private:

	CLogoGui*			m_pLogoGui;

	CContractGui*		m_pContractGui;

	CLogInGui*			m_pLogin;

	
public:

	CLogInGuiGroup();
	~CLogInGuiGroup();

	RwBool Create(void);
	void Destroy(void);
};

#endif
