/*****************************************************************************
 *
 * File			: NtlSobCharPerfController.h
 * Author		: JeongHo, Rho
 * Copyright	: (аж) NTL
 * Date			: 2009. 6. 5
 * Abstract		: 
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SOBCHARPERF_CONTROLLER_H__
#define __NTL_SOBCHARPERF_CONTROLLER_H__


class CNtlSob;
class CNtlSobProxySystemEffect;


class CNtlSobCharPerfController
{
// Temp
public:
	static RwReal					s_fStandFrame;
	static RwReal					s_fRemainIntervalFrame;
	static RwReal					s_fIncRenderCharPerSec;
	static RwReal					s_fDecRenderCharPerSec;
	static RwReal					s_fMinRenderChar;

public:
	#define VISIBLE_RESET_TIME		(0.3f)

	struct sPerfEntity
	{
		CNtlSob*					pSobObj;
		CNtlSobProxySystemEffect*	pEffect;
	};

	typedef std::map< void*, sPerfEntity > mapdef_PerfList;

protected:
	mapdef_PerfList					m_defSobPCList;
	RwReal							m_fCurRenderCnt;
	RwReal							m_fVisibleResetTime;

public:
	CNtlSobCharPerfController( void );
	~CNtlSobCharPerfController( void );

public:
	void				Update( RwReal fElapsedTime );

	void				SobCreate( CNtlSob* pSobObj );
	void				SobDelete( CNtlSob* pSobObj );

protected:
	void				Destroy( void );

	void				JudgementRenderCounter( RwReal fElapsedTime );
	void				UpdateVisibility( RwReal fElapsedTime );
};


#endif