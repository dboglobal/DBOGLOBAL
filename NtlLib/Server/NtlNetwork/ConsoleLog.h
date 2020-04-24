#pragma once
//***********************************************************************************
//
//	File		:	ConsoleLog.h
//
//	Begin		:	2008-03-18 ¿ÀÀü 11:03:20
//
//	Copyright	:	¨Ï NTL-Inc Co. Ltd
//
//	strAuthor	:	Seung Goon. Chi   ( sgpro@ntl-inc.com )
//
//	strDesc		:	
//
//***********************************************************************************

//-- include ------------------------------------------------------------------------
#include <string>
#include <iostream>
#include <fstream>
//-----------------------------------------------------------------------------------

class CConsoleLog
{

public:
	typedef std::basic_string< TCHAR > tstring;

public:
	CConsoleLog();
	~CConsoleLog();

public:
	void Create( char* pszFileName );
	void Destroy( void );

public:
	void Write( TCHAR* ptszData, bool bIsAutoEnter = true );
	bool IsValid( void ) { return m_bIsValid; }
	void Flush( void );
private:
	std::ofstream* m_pOutFile;
	bool m_bIsValid;

};//end of class ( CConsoleLog )