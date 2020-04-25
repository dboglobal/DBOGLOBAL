/*****************************************************************************
* File			: gui_particle_doc.h
* Author		: HaeSung, Cho
* Copyright		: (주)NTL
* Date			: 2008. 10. 7 ( last update : 2008. 10. 15 )
* Abstract		: 
*****************************************************************************
* Desc          : 
* CParticleDoc		
*     |
* std::map ( CParticleGroup - CParticleGroup - CParticleGroup .. )
*                 |
*             std::map ( CParticleItem - CParticleItem - CParticleItem .. )
*****************************************************************************/

#ifndef _GUI_PARTICLE_DOC_H_
#define	_GUI_PARTICLE_DOC_H_

START_GUI

#define NAME_BUFFER_SIZE		32

struct SNTL_COMMON
{
	float			fLifeTime;
	int				nLimit;

	SNTL_COMMON()
	{
		fLifeTime = 0.0f; nLimit = 0;
	}
};

struct SNTL_EMITTER
{
	RwV2d			v2dPos;
	float			fEmissivity;
	float			fLifeMin;
	float			fLifeMax;
	float			fVelMin;
	float			fVelMax;
	RwV2d			v2dEmitDir;
	float			fEmitAngle;
	float			fRotRateMin;
	float			fRotRateMax;
	float			fScaleMin;
	float			fScaleMax;
	RwRGBA			rgbaMin;
	RwRGBA			rgbaMax;
	char			acSurfaceFile[NAME_BUFFER_SIZE];
	char			acSurfaceName[NAME_BUFFER_SIZE];

	SNTL_EMITTER()
	{
		v2dPos.x = 0.0f; v2dPos.y = 0.0f; fEmissivity = 0.0f; fLifeMin = 0.0f; fLifeMax = 0.0f;
		fVelMin = 0.0f; fVelMax = 0.0f; v2dEmitDir.x = 0.0f; v2dEmitDir.y = 0.0f; fEmitAngle = 0.0f;
		fRotRateMin = 0.0f; fRotRateMax = 0.0f; fScaleMin = 0.0f; fScaleMax = 0.0f; rgbaMin.red = 255;
		rgbaMin.green = 255; rgbaMin.blue = 255; rgbaMin.alpha = 255; rgbaMax.red = 255; rgbaMax.green = 255;
		rgbaMax.blue = 255; rgbaMax.alpha = 255; 
		memset( acSurfaceFile, 0, sizeof( char ) * NAME_BUFFER_SIZE );
		memset( acSurfaceName, 0, sizeof( char ) * NAME_BUFFER_SIZE );
	}
};

struct SNTL_EMITTER_BOX : public SNTL_EMITTER
{
	RwInt32			nWidth;
	RwInt32			nHeight;

	SNTL_EMITTER_BOX()
	{
		nWidth = 10;
		nHeight = 10;
	}
};

struct SNTL_GRAVITY
{
	RwV2d			v2dGravity;
	float			fWeight;

	SNTL_GRAVITY()
	{
		v2dGravity.x = 0.0f; v2dGravity.y = 0.0f; fWeight = 0.0f;
	}
};

struct SNTL_COLOR
{
	unsigned char	uRed;
	unsigned char	uGreen;
	unsigned char	uBlue;
	unsigned char	uAlpha;
	BOOL			bRed;
	BOOL			bGreen;
	BOOL			bBlue;
	BOOL			bAlpha;

	SNTL_COLOR()
	{
		uRed = 255; uGreen = 255; uBlue = 255; uAlpha = 255;
		bRed = FALSE; bGreen = FALSE; bBlue = FALSE; bAlpha = FALSE;
	}
};

struct SCtrlPoint;

class CValueItem
{
public:
	CValueItem();
	virtual ~CValueItem();

	BOOL	AddValue( std::string strKey, std::string strValue );
	BOOL	AddValue( std::string strKey, int nValue );
	BOOL	AddValue( std::string strKey, float fValue );
	BOOL	RemoveValue( std::string strKey );

	BOOL	GetValue( std::string strKey, int& nValue );
	BOOL	GetValue( std::string strKey, RwV2d& v2dValue );
	BOOL	GetValue( std::string strKey, float& fValue );
	BOOL	GetValue( std::string strKey, RwRGBA& colorValue);
	BOOL	GetValue( std::string strKey, float& fValue1, float& fValue2 );
	BOOL	GetValue( std::string strKey, int& nValue1, int& nValue2 );
	BOOL	GetValue( std::string strKey, unsigned char& uValue );
	BOOL	GetValue( std::string strKey, unsigned char& uValue1, unsigned char& uValue2,
		unsigned char& uValue3, unsigned char& uValue4 );
	BOOL	GetValue( std::string strKey, std::string& strValue );
	BOOL	GetValue( std::string strKey, char* pBuffer, int nBufferSize );
	BOOL	GetValue( std::string strKey, SCtrlPoint* pBuffer, int nBufferSize );

	void	SetName( std::string strName );
	void	SetType( std::string strType );

	std::string GetName();
	std::string GetType();

	int		GetSize();

	void	Clear();

	typedef std::map< std::string, std::string >	VALUEMAP;
	VALUEMAP	m_mapValue;			///< 데이타를 가지고 있는 맵

protected:

	std::string	m_strName;
	std::string m_strType;
};

class CParticleItem
{
public:
	CParticleItem();
	~CParticleItem();

	enum eItemType
	{
		ITEM_COMMON,
		ITEM_EMITTER,
		ITEM_GRAVITY,
		ITEM_COLOR,
		ITEM_PATH,

		ITEM_NUMS,
		INVALID_ITEM = 0xFF
	};

	void		SetName( std::string strName );
	std::string GetName();

	BOOL		AddValue( eItemType eType, std::string strKey, std::string strValue );
	BOOL		RemoveValue( eItemType eType, std::string strKey );
	CValueItem*	GetValueItem( eItemType eType );

	BOOL		SetValueItemName( eItemType eType, std::string strName );
	BOOL		SetValueItemType( eItemType eType, std::string strType );

protected:
	std::string	m_strName;

	CValueItem	m_itemValue[ITEM_NUMS];
};

class CParticleGroup
{
public:
	CParticleGroup();
	virtual ~CParticleGroup();

	std::string		GetName();
	void			SetName( std::string strName );

	int				GetSize();

	CParticleItem*	AddParticle( const char* parname );
	CParticleItem*	GetParticle( const char* parname );
	CParticleItem*	GetReplaceItemKey( const char* parname, const char* replacename );

	BOOL			DelParticle( const char* parname );

	CParticleItem*	GetFirstParticle();
	CParticleItem*	GetNextParticle();
	CParticleItem*	GetCurrentParticle();
	
public:
	typedef std::map< std::string, CParticleItem* >				MAPPARTICLE;
	typedef std::map< std::string, CParticleItem* >::iterator	MAPPARTICLEIT;
	MAPPARTICLE		m_mapParticle;
	MAPPARTICLEIT	m_itCurrent;

	std::string		m_strGroupName;
};

class CParticleDoc
{
public:
	virtual ~CParticleDoc();

	// File Interface
public:
	void	RegisterHandler();

	void	Load( const char* filename );
	void	LoadFromPack( const char* filename );
	void	Save( const char* filename );

	void	Clear();

	// Edit Interface
public:
	CParticleGroup*	AddParticleGroup( const char* groupname );
	CParticleItem*	AddParticle( const char* groupname, const char* parname );
	BOOL			DelParticleGroup( const char* groupname );
	BOOL			DelParticle( const char* groupname, const char* parname );

public:
	CParticleGroup* GetParticleGroup( const char* pGroupName );
	CParticleGroup* GetReplaceGroupKey( const char* pGroupName, const char* pReplaceName );

	// Iterator
public:
	CParticleGroup*	GetFirstGroup();
	CParticleGroup*	GetNextGroup();
	CParticleGroup*	GetCurrentGroup();

	static CParticleDoc*	GetInstance()
	{
		static CParticleDoc instance;
		return &instance;
	}

protected:
	CParticleDoc();

	BOOL	ParseGroupHandler( char* pLineBuffer );
	BOOL	ParseParticleHandler( char* pLineBuffer );
	BOOL	ParseEmitterHandler( char* pLineBuffer );
	BOOL	ParseAffectorHandler( char* pLineBuffer );
	BOOL	ParsePathHandler( char* pLineBuffer );

	CParticleGroup* m_pCurrentGroup;
	CParticleItem*	m_pCurrentItem;
	BYTE			m_byCurrentValueType;

protected:
	// 주어진 키워드에 따라서 호출될 핸들러 함수들을 미리 정의한다.
	typedef BOOL (CParticleDoc::*ParsingCall)(char*);
	typedef std::unordered_map<std::string, ParsingCall> HMapHandler;
	HMapHandler	m_hmapHandler;

	typedef std::map< std::string, CParticleGroup* >			MAPGROUP;
	typedef std::map< std::string, CParticleGroup* >::iterator  MAPGROUPIT;
	MAPGROUP		m_mapGroup;
	MAPGROUPIT		m_itCurrent;
};

static CParticleDoc* GetParticleDoc()
{
	return CParticleDoc::GetInstance();
}

END_GUI

#endif