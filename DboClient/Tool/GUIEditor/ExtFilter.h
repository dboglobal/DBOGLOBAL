//-----------------------------------------------------------------------------
// Name: class CExtFilter
// Desc: PathName을 받아서 확장자만 돌려주거나 클래스.
//       등록된 확장자인지 가리는 클래스. 
//
// 2006.01.17 Peessi@hitel.net  
//-----------------------------------------------------------------------------
#pragma once

#include <map>
#include <string>
#include "windows.h"

class CExtFilter
{
public:
	CExtFilter(void);
	~CExtFilter(void);

	enum    EXTTYPE { NONE = 0, IMAGE, RESOURCE, SURFACE, COMPONENT };

	typedef std::string					   stlString;
	typedef std::map< stlString, EXTTYPE > stlExtMap;
	
	void	Create();
	void    Destroy();

	void    ExtDefinition();		// 여기에 쓰이는 확장자를 때려 넣는다.

	// operation
	BOOL	IsValid( LPCTSTR szPath );
	EXTTYPE GetType( LPCTSTR szPath );
	BOOL	GetExt( LPCTSTR szPath, stlString* pExt );

private:
	
    // Variables
	stlExtMap m_stlExtMap;
};
