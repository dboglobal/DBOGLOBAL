#ifndef _NTL_NAVI_LOADING_QUEUE_H_
#define _NTL_NAVI_LOADING_QUEUE_H_


#include "NtlNaviSync.h"


//////////////////////////////////////////////////////////////////////////
//
//	CNtlNaviLoadEntity
//
//////////////////////////////////////////////////////////////////////////

class CNtlNaviLoadingEntity
{
protected:
	CNtlNaviCS						m_csError;
	bool							m_bError;


public:
	CNtlNaviLoadingEntity( void ) { m_bError = false; }
	virtual ~CNtlNaviLoadingEntity( void ) { return; }


public:
	bool							IsError( void );
	void							SetError( bool bError );

	virtual void					RunMultiThread( void ) = 0;
};


//////////////////////////////////////////////////////////////////////////
//
//	CNtlLoadingQueue
//
//////////////////////////////////////////////////////////////////////////

class CNtlLoadingQueue
{
public:
	enum { eMAX_THREAD_CNT = 1 };

	typedef std::vector< CNtlNaviLoadingEntity* > vecdef_ENTITY_LIST;


protected:
	static CNtlLoadingQueue*		s_pLoadingQueue;


public:
	static CNtlLoadingQueue*		GetInstance( void );


protected:
	bool							m_bCreated;

	CNtlNaviCS						m_clExitCS;
	bool							m_bExit;

	HANDLE							m_hEvent;

	HANDLE							m_arhThread[eMAX_THREAD_CNT];

	CNtlNaviCS						m_clEntityToLoadCS;
	vecdef_ENTITY_LIST				m_defEntityToLoadList;

	CNtlNaviCS						m_clEntityLoadedCS;
	vecdef_ENTITY_LIST				m_defEntityLoadedList;


public:
	CNtlLoadingQueue( void );
	~CNtlLoadingQueue( void );


public:
	bool							Create( void );
	void							Delete( void );

	bool							IsExit( void );
	void							SetExit( void );

	bool							IsEmptyEntityToLoad( void );
	void							AttachEntityToLoad( CNtlNaviLoadingEntity* pEntity );
	bool							DetachEntityToLoad( CNtlNaviLoadingEntity* pEntity );

	void							AttachEntityLoaded( CNtlNaviLoadingEntity* pEntity );
	bool							DetachEntityLoaded( CNtlNaviLoadingEntity* pEntity );

	unsigned int					ThreadCallBackFunc( void );


protected:
	CNtlNaviLoadingEntity*			TakeEntityToLoad( void );

	static unsigned int __stdcall	ThreaFuncCB( void* pParam );
};



#endif