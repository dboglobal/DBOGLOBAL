////////////////////////////////////////////////////////////////////////////////
// File: SkillCustomizeParser.h
// Desc: SkillCustomize Script Parser
//
// 2008.11.10 Peessi@hitel.net  
// To Be Desired:
////////////////////////////////////////////////////////////////////////////////

#ifndef __SKILL_CUSTOMIZE_PARSER_H__
#define __SKILL_CUSTOMIZE_PARSER_H__

#include "gui_precomp.h"
#include "guicore_precomp.h"
#include "SkillTable.h"
#include "HTBSetTable.h"
#include "ActionTable.h"
#include "TextAllTable.h"

enum SKILL_CUSTOMIZE_SKILL_TYPE
{
	SKILL_TYPE_SKILL,
	SKILL_TYPE_HTB,
	SKILL_TYPE_ACTION,

	SKILL_TYPE_INVALID = 255
};

enum SKILL_CUSTOMIZE_LINE_TYPE
{
	LINE_TYPE_UPGRADE,
	LINE_TYPE_OPTION,

	LINE_TYPE_INVALID = 255
};

enum SKILL_CUSTOMIZE_LINE_POS
{
	LINE_POS_LEFT,
	LINE_POS_UP,
	LINE_POS_RIGHT,
	LINE_POS_DOWN,

	LINE_POS_INVALID = 255
};

struct stLINE_PATH
{
	RwUInt8	byDirection;
	RwInt32	nX;
	RwInt32	nY;

	VOID SetLinePath( RwUInt8 _byDirection, RwInt32 _nX, RwInt32 _nY )
	{
		byDirection = _byDirection; nX = _nX; nY = _nY;
	}
};

class CSkillCustomizeSkillItem;

class CSkillCustomizeLineItem
{
public:
	//! Type	
	//struct stLINESURFACE
	//{
	//	enum SURFACETYPE { UPGRADE_ARROW, UPGRADE_BAR, SUB_VERTICAL, SUB_HORIZONTAL, SUB_LEFTDOWN, SUB_LEFTTOP, SUB_RIGHTDOWN, SUB_RIGHTTOP };

	//	gui::CSurface	surface;
	//	RwUInt8			byType;
	//};
	struct stLINEPOS
	{
		CPos	pos;
		RwUInt8	byDirection;
	};

	typedef std::list<gui::CSurface>			LIST_SURFACE;
	typedef std::list<gui::CSurface>::iterator	LIST_SURFACE_ITER;

	//! Constructor
	CSkillCustomizeLineItem( gui::CComponent* pParent, RwUInt8 byLineType, RwUInt8 byBeginPos, RwUInt8 byEndPos,
							 CSkillCustomizeSkillItem* pBeginSkill, CSkillCustomizeSkillItem* pEndSkill, 
							 const std::string& strLineName, std::string& strReport );						 
	~CSkillCustomizeLineItem(VOID);

	//! Operation
	VOID Show( bool bShow );
	VOID SetAlpha( RwUInt8 byAlpha );
	VOID SetActive( RwBool bActive );

	//! Attribute
	RwUInt8 GetLineType(VOID) { return m_byLineType; }
	RwInt32 GetBottomPos(VOID);

private:
	//! Implement
	gui::CSurface	GetLineSurface( RwChar* surfacename );
	VOID	SetLineSurface( gui::CSurface& Surface, gui::CSurface& ActiveSurface, CRectangle& rect );
	VOID	GetLinePath( stLINE_PATH* pLinePath, RwUInt8 byLinePos, CSkillCustomizeSkillItem* pSkill, RwUInt8 byLineSize );
	RwBool	CalcLinePath( stLINE_PATH* pCurrentPath, stLINE_PATH* pDestPath, CSkillCustomizeSkillItem* pBeginSkill, CSkillCustomizeSkillItem* pEndSkill, 
						  RwUInt8 byLineSize, const std::string& strLineName, std::string& strReport );
	VOID	LineError( const std::string& strLineName, std::string& strReport );
			
	//! Variable
	LIST_SURFACE	m_listLineSurface;
	LIST_SURFACE	m_listActiveLineSurface;
	gui::CPanel*	m_ppnlLine;
	RwUInt8			m_byLineType;
};

class CSkillCustomizeSkillItem
{
public:
	//! TYPE 
	enum TYPE { SKILL, HTB, ACTION };
	typedef std::list<CSkillCustomizeLineItem*>				LIST_LINEITEM;
	typedef std::list<CSkillCustomizeLineItem*>::iterator	LIST_LINEITEM_ITER;

	//! Constructor
	CSkillCustomizeSkillItem( gui::CComponent* pParent, RwUInt8 bySkillType, sTBLDAT* pTblData, RwBool bMastery, 
							  RwInt32 nX, RwInt32 nY, gui::CSurfaceManager* pSurfaceManager, std::string& strReport );
	~CSkillCustomizeSkillItem(VOID);

	//! Operation
	VOID Show( bool bShow );
	VOID SetAlpha( RwUInt8 byAlpha );
	VOID SetColor( RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue );
	VOID SetActive( RwBool bActive, std::string* pstrReport = NULL, RwBool* pbSPUseBtn = NULL, RwBool* pbRPUseBtn = NULL );

	VOID SetLineShow( bool bShow, RwUInt8 byLineType );
	VOID SetLineAlpha( RwUInt8 byAlpha, RwUInt8 byLineType );
	VOID SetLineActive( RwBool bActive, RwUInt8 byLineType );	

	VOID SetInitSkill(VOID);
	VOID SetSkillData( sTBLDAT* pData );
	VOID SetLine( CSkillCustomizeLineItem* pLineItem );
	
	VOID SetIconTexture( gui::CTexture* pTexture );
	VOID SetIconTexture( const RwChar* szFileName );
	VOID UnsetIconTexture(VOID);

	//! Attribute
	RwUInt32			GetBaseSkillIndex(VOID) { return m_uiBaseSkillIndex; }
	sTBLDAT*			GetSkillData(VOID) { return m_pTblData; }
	RwUInt8				GetSkillType(VOID) { return m_bySkillType; }
	CRectangle      	GetSkillPosition(VOID);
	RwInt32				GetBottomPos(VOID);
	CRectangle      	GetSkillScreenRect();
	CRectangle*      	GetSkillRectangle();
	
	gui::CPanel*		GetSkillIcon(VOID) { return m_ppnlIcon; }
	gui::CButton*		GetSPButton(VOID) { return m_pbtnSPUse; }
	gui::CButton*		GetRPButton(VOID) { return m_pbtnRPUse; }	

	bool				IsShow(VOID);
	RwBool				IsMasterySkill(VOID) { return m_bMastery; }
	
private:
	//! Implement
	gui::CTexture*	CreateTexture( const RwChar* szFileName, RwBool* bCreateFlag );
	VOID			DeleteTexture( gui::CTexture** ppTexture, RwBool* bCreateFlag );
	
	//! Variable
	gui::CPanel*		m_ppnlBack;
	gui::CPanel*		m_ppnlIcon;
	gui::CButton*		m_pbtnSPUse;
	gui::CButton*		m_pbtnRPUse;
	gui::CStaticBox*	m_pstbMasteryName;

	LIST_LINEITEM	m_listLineItem;

	RwUInt32		m_uiBaseSkillIndex;
	RwUInt8			m_bySkillType;
	sTBLDAT*		m_pTblData;

	RwBool			m_bIconTextureCreated;
	RwBool			m_bMastery;
};

struct stSKILLCUSTOMIZEPARSEDDATA
{
	stSKILLCUSTOMIZEPARSEDDATA( RwUInt8 byType ) { m_byType = byType; }

	enum TYPE { SKILL, LINE, TYPE_INVALID };
	virtual RwBool	SetData( const RwChar* szDataName, const RwChar* szDataValue, std::string& strReport ) = 0;
	virtual VOID	SetName( const RwChar* szName ) { m_strName = szName; }

	std::string&	GetName(VOID) { return m_strName; }
	RwUInt8			GetType(VOID) { return m_byType; }

	std::string		m_strName;
	RwUInt8			m_byType;
};

struct stSKILLCUSTOMIZEPARSEDSKILL : public stSKILLCUSTOMIZEPARSEDDATA
{
	stSKILLCUSTOMIZEPARSEDSKILL( RwUInt8 byType );

	RwBool SetData( const RwChar* szDataName, const RwChar* szDataValue, std::string& strReport );

	RwInt32		nX;
	RwInt32		nY;
	RwBool		bMastery;
	RwUInt32	tblidx;
	RwUInt8		bySkillType;
};

// Line은 일단 담아두었다가 Skill이 생성 종료되면 생성, 그 때를 위한 보존객체.
struct stSKILLCUSTOMIZEPARSEDLINE : public stSKILLCUSTOMIZEPARSEDDATA
{
	stSKILLCUSTOMIZEPARSEDLINE( RwUInt8 byType );

	RwBool		SetData( const RwChar* szDataName, const RwChar* szDataValue, std::string& strReport );
	RwUInt8		GetPosition( const RwChar* szValue );
	
	std::string	strBeginSkillName;
	std::string strEndSkillName;
	RwUInt8		byBeginLinePos;
	RwUInt8		byEndLinePos;
	RwUInt8		byLineType;
};

class CSkillCustomizeParser
{
public:
	//! Type
	typedef std::map<std::string,CSkillCustomizeSkillItem*>					MAP_SKILLITEM;	// 이 데이터가 원본. 
	typedef std::map<std::string,CSkillCustomizeSkillItem*>::iterator		MAP_SKILLITEM_ITER;	
	
	// SKILL Item이 생성된 후 삭제.
	typedef std::map<std::string,stSKILLCUSTOMIZEPARSEDLINE*>				MAP_PARSED_LINEDATA;		
	typedef std::map<std::string,stSKILLCUSTOMIZEPARSEDLINE*>::iterator		MAP_PARSED_LINEDATA_ITER;	
	typedef std::map<std::string,stSKILLCUSTOMIZEPARSEDSKILL*>				MAP_PARSED_SKILLDATA;
	typedef std::map<std::string,stSKILLCUSTOMIZEPARSEDSKILL*>::iterator	MAP_PARSED_SKILLDATA_ITER;	
		
	//! Constructor
	CSkillCustomizeParser( const RwChar* szFileName );
	~CSkillCustomizeParser(VOID);	

	//! Operation
	RwBool			GenerateGuiItem( gui::CComponent* pParent );
	MAP_SKILLITEM&	GetComponentMap(VOID) { return m_mapSkillItem; }
	VOID			ReadError( std::string& strError ) { strError = m_strError; }

	stSKILLCUSTOMIZEPARSEDDATA* IsValidType( const RwChar* szTypeName );

	//! Static Operation 생성전에 미리 지정주어야 함. 
	static	VOID	SetCommonData( CSkillTable* pSkillTable, CHTBSetTable* pHTBTable, CActionTable* pActionTable, CTextAllTable* pTextTable, gui::CSurfaceManager* pSurfaceManager );

private:
	//! Implements
	RwBool	CheckStaticValue(VOID);
	VOID	DeleteParsedItem(VOID);
	VOID	ErrorBeforeThis( stSKILLCUSTOMIZEPARSEDDATA* pData );
	
	//! Variable
	MAP_SKILLITEM			m_mapSkillItem;
	MAP_PARSED_LINEDATA		m_mapParsedLineData;	
	MAP_PARSED_SKILLDATA	m_mapParsedSkillData;

	std::string		m_strError;

public:
	static CSkillTable*		m_pSkillTable;
	static CHTBSetTable*	m_pHTBTable;
	static CActionTable*	m_pActionTable;
	static CTextAllTable*	m_pTextTable;

	static gui::CSurfaceManager* m_pSurfaceManager;
};

#endif//__SKILL_CUSTOMIZE_PARSER_H__