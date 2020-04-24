/*****************************************************************************
*
* File			: DboTLSystem.h
* Author		: JeongHo, Rho
* Copyright		: (аж)NTL
* Date			: 2008. 4. 22	
* Abstract		: 
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_DBO_TL_SYSTEM_H__
#define __NTL_DBO_TL_SYSTEM_H__


#include "ceventhandler.h"


class CDboTLLoader;
class CDboTLController;


class CDboTLSystem
{
// Member variables
protected:
	RwBool				m_bActive;

	CDboTLLoader*		m_pTLLoader;
	CDboTLController*	m_pTLController;


// Constructions and Destructions
public:
	CDboTLSystem( void );
	~CDboTLSystem( void );


// Operations
public:
	void				SetActive( RwBool bActive );

	RwBool				Create( void );
	void				Update( RwReal fElapsed );
	void				Delete( void );

	void				Skip( void );

	RwBool				IsSkipping();

	void				HandleEvents( RWS::CMsg &pMsg );
};


#endif
