#pragma once


#include "DboTSCMain.h"


class CTableContainer;


class CExportEvtTSMng : public CDboTSCMain
{
protected:
	CTableContainer*			m_pTblContainer;

public:
	CExportEvtTSMng( void );
	virtual ~CExportEvtTSMng( void );


public:
	bool						Create( const char* pTSPath, CTableContainer* pTblContainer );


protected:
	virtual bool				LoadLog( void );
	virtual void				UnloadLog( void );

	virtual bool				LoadEventMappers( void );
};