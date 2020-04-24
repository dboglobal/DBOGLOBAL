#ifndef _NTL_NAVI_WORLD_DATA_LAODER_H_
#define _NTL_NAVI_WORLD_DATA_LAODER_H_


#include "NtlNaviMath.h"

#include <vector>

class CNtlNaviWorld_WE;
class CNtlNaviFieldProp_WE;
class CNtlNaviField_WE;


class CNtlNaviWorldDataLoader
{
// Declarations
public:
	struct sWORLD_INFO
	{
		std::string				strWorldPath;
		unsigned int			uiWorldID;
		CNaviAABB				clWorldRgn;
		float					fTileSize;
		int						nGroupSize;
		float					fGroupOverlap;
		std::vector<float>		vecAgentList;
	};

	typedef std::list< CNtlNaviWorld_WE* > listdef_NaviWorldList;


// Member variables
protected:
	listdef_NaviWorldList		m_defNaviWorldList;
	listdef_NaviWorldList::iterator m_itNaviWorldList;


// Constructions and Destructions
public:
	CNtlNaviWorldDataLoader( void );
	~CNtlNaviWorldDataLoader( void );


// Operations
public:
	CNtlNaviWorld_WE*			Begin( void );
	CNtlNaviWorld_WE*			Next( void );

	bool						Load( const char* pRootFolder );


// Implementations
protected:
	void						Destroy( void );

	bool						LoadWorldInfo( std::string& strWorldFolder, sWORLD_INFO& sWorldInfo );

	CNtlNaviWorld_WE*			LoadWorld( sWORLD_INFO& sWorldInfo );
};


#endif