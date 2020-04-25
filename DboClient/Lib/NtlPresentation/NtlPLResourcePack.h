/*****************************************************************************
 *
 * File			: NtlPLResourcePack.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2007. 3. 20	
 * Abstract		: Presentation layer resource pack class
 *****************************************************************************
 * Desc         : 
 * Adjust		: 
 *
 *****************************************************************************/
#ifndef __NTL_PLRESOURCE_PACK_H__
#define __NTL_PLRESOURCE_PACK_H__

#include <unordered_map>
#include "NtlCallbackWidget.h"
#include "NtlDebug.h"

#include "NtlFileSerializer.h"
#include "TableContainer.h"

#define NTL_PACK_CRYPT_KEY						"FFCF4403F048FFC683C30883FB1072E1SDFDF4D"

#define NTL_FILEPACK_NAME_SIZE		128
#define NTL_FILEPACK_BUFFER_SIZE	1024
#define NTL_MAX_FILEPACK_SIZE		20000000 // 20mb

//------------------------------------------------------------------------------
// pack result

#define NTL_FILEPACK_SUCCESS		0
#define NTL_FILEPACK_FILENOTFOUND	-1		// file을 찾을 수 없다.
#define NTL_FILEPACK_FILEREADONLY	-2		// file read only
#define NTL_FILEPACK_CREATEFAIL		-3		// file 생성 실패
#define NTL_FILEPACK_NOT_USEFILE	-4		// file 생성 실패
#define NTL_FILEPACK_ERROR			-5		// error


//------------------------------------------------------------------------------
// pack type

enum EFilePackType
{
	NTL_PACK_TYPE_TEXTURE,
	NTL_PACK_TYPE_OBJECT,	
	NTL_PACK_TYPE_GUI,
	NTL_PACK_TYPE_FLASH,
	NTL_PACK_TYPE_SOUND,
    NTL_PACK_TYPE_LANGUAGE,
    NTL_PACK_TYPE_SCRIPT,
    NTL_PACK_TYPE_TABLE,
    NTL_PACK_TYPE_PROPERTY,
    NTL_PACK_TYPE_TERRAIN,
	MAX_NTL_PACK_TYPE,
};

enum ELocalizeFilePackType
{
	NTL_LOCALIZE_PACK_TYPE_TEXTURE,
	NTL_LOCALIZE_PACK_TYPE_GUI,
	NTL_LOCALIZE_PACK_TYPE_FLASH,
	NTL_LOCALIZE_PACK_TYPE_SCRIPT,
	NTL_LOCALIZE_PACK_TYPE_TABLE,
	MAX_LOCALIZE_NTL_PACK_TYPE,
};

enum EFilePackTypeFlag
{
	NTL_PACK_TYPE_FLAG_TEXTURE		= (1<<NTL_PACK_TYPE_TEXTURE),
	NTL_PACK_TYPE_FLAG_OBJECT		= (1<<NTL_PACK_TYPE_OBJECT),	
	NTL_PACK_TYPE_FLAG_GUI			= (1<<NTL_PACK_TYPE_GUI),
	NTL_PACK_TYPE_FLAG_FLASH		= (1<<NTL_PACK_TYPE_FLASH),
	NTL_PACK_TYPE_FLAG_SOUND		= (1<<NTL_PACK_TYPE_SOUND),
    NTL_PACK_TYPE_FLAG_LANGUAGE     = (1<<NTL_PACK_TYPE_LANGUAGE),
    NTL_PACK_TYPE_FLAG_SCRIPT       = (1<<NTL_PACK_TYPE_SCRIPT),
    NTL_PACK_TYPE_FLAG_TABLE        = (1<<NTL_PACK_TYPE_TABLE),
    NTL_PACK_TYPE_FLAG_PROPERTY     = (1<<NTL_PACK_TYPE_PROPERTY),
    NTL_PACK_TYPE_FLAG_TERRAIN		= (1<<NTL_PACK_TYPE_TERRAIN),
};


//------------------------------------------------------------------------------
// pack type file define

#define NTL_PACK_FILE_PATH						".\\pack\\"

#define NTL_PACKNAME_TEXTURE_HEADER				"tex.pak"
#define NTL_PACKNAME_OBJECT_HEADER				"obj.pak"
#define NTL_PACKNAME_TERRAIN_HEADER				"ter.pak"
#define NTL_PACKNAME_GUI_HEADER					"gui.pak"
#define NTL_PACKNAME_FLASH_HEADER				"flash.pak"
#define NTL_PACKNAME_SOUND_HEADER				"sound.pak"
#define NTL_PACKNAME_LANGUAGE_HEADER            "lang.pak"
#define NTL_PACKNAME_SCRIPT_HEADER              "scr.pak"
#define NTL_PACKNAME_TABLE_HEADER               "tbl.pak"
#define NTL_PACKNAME_PROPERTY_HEADER            "prop.pak"

#define NTL_PACKNAME_TEXTURE_DATA				"tex"
#define NTL_PACKNAME_OBJECT_DATA				"obj"
#define NTL_PACKNAME_TERRAIN_DATA				"ter"
#define NTL_PACKNAME_GUI_DATA					"gui"
#define NTL_PACKNAME_FLASH_DATA					"flash"
#define NTL_PACKNAME_SOUND_DATA					"sound"
#define NTL_PACKNAME_LANGUAGE_DATA              "lang"
#define NTL_PACKNAME_SCRIPT_DATA                "scr"
#define NTL_PACKNAME_TABLE_DATA                 "tbl"
#define NTL_PACKNAME_PROPERTY_DATA              "prop"

//------------------------------------------------------------------------------
// pack directory type

#define NTL_FOLDER_TEXTURE						".\\texture"

#define NTL_FOLDER_OBJECT01						".\\character"
#define NTL_FOLDER_OBJECT02						".\\Item"
#define NTL_FOLDER_OBJECT03						".\\object"
#define NTL_FOLDER_OBJECT04						".\\effect"
#define NTL_FOLDER_OBJECT05						".\\world\\Mesh"

#define NTL_FOLDER_TERRAIN						".\\world"
#define NTL_FOLDER_GUI							".\\gui"
#define NTL_FOLDER_FLASH						".\\flash"
#define NTL_FOLDER_SOUND						".\\sound"
#define NTL_FOLDER_LANGUAGE                     ".\\language"
#define NTL_FOLDER_TS                           ".\\ts"
#define NTL_FOLDER_SCRIPT                       ".\\script"
#define NTL_FOLDER_TABLE                        ".\\data"
#define NTL_FOLDER_PROPERTY                     ".\\property"


//------------------------------------------------------------------------------


typedef struct _SFilePackHeaderElement
{
	RwUInt8		byUnitKey;
	RwChar		chName[NTL_FILEPACK_NAME_SIZE];
	RwUInt32	uiSize;
	RwUInt32	uiOffset;	
}SFilePackHeaderElement;

//------------------------------------------------------------------------------

typedef struct _SPackResFileData
{
	std::string strPackFileName;
	RwUInt32	uiOffset;
	RwUInt32	uiSize;
}SPackResFileData;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlPLResourcePack
{
public:
	
	typedef std::unordered_map<std::string, SFilePackHeaderElement*> HMapPackHeader;

protected:

	std::string		m_strHeaderName;
	std::string		m_strDataName;
	HMapPackHeader	m_hmapPackHeader;
	int*			m_piUnknownFirstByte;

public:

	CNtlPLResourcePack(int iUnknownFirstByte);
	~CNtlPLResourcePack();

	void		ClearHeader(void);
	void		RegistHeaderName(const RwChar *pFileName);
	void		RegistUnitFileName(RwUInt8 byUnit, const RwChar *pFileName);
	void		RegistDataName(const RwChar *pFileName);

	SFilePackHeaderElement*	FindHeader(std::string str); 
	SFilePackHeaderElement* FindHeader(const RwChar *pFileName); 

	void					AddHeader(RwUInt8 byUnit, const RwChar *pFileName, RwUInt32 uiSize, RwUInt32 uiOffset);
	HMapPackHeader&			GetHeaders(void);

	RwInt32					GetElementCount(void);

	void					GetPackDataFileName(RwUInt8 byUnit, std::string& strPackDataFileName);

	RwInt32		LoadPack(const RwChar *pFileName);
	RwInt32		SavePack(const RwChar *pFileName);
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlPLResoucePackManager
{
public:
	struct sPACK_DATA_INFO
	{
		BYTE	*pBuffer;
		RwInt32	nBuffSize;
	};

	typedef std::map< std::string, sPACK_DATA_INFO > mapdef_PackDataInfoList;

private:

	RwUInt32	m_uiFlags;

	// pack을 풀기 위한 참조 함수.
	RwInt32		m_iBufferSize;
	RwInt32		m_iCurrPathNum;
	RwChar		m_chPathBuffer[20][1024];
	RwChar		m_chFullBuffer[NTL_FILEPACK_BUFFER_SIZE];
	RwChar		m_chFileBuffer[NTL_FILEPACK_BUFFER_SIZE];

	RwInt32		m_iMemSize;
	RwChar		*m_pMemBuffer;

	CNtlPLResourcePack *m_pResPack[MAX_NTL_PACK_TYPE];

	CNtlCallbackParam2 *m_pCallMakePackNotify;

private:

	void	CreateAllPack(void);
	void	DeleteAllPack(void);

	void	GetAllFiles(std::string& strOffsetFolder, std::list<std::string>& listFiles);
	RwInt32	SavePackFile(CNtlPLResourcePack *pResPack, std::list<std::string>& listFiles, const RwChar *pPackDataFileName); 
	RwChar*	DumpFile(const char *pFileName, RwUInt64& uiSize);

	void	FilterPathBuffer(const RwChar *pPath, const RwChar *pFileName);
	void	CheckMemoryBuffer(RwInt32 iSize);

	RwInt32 LoadPack(RwUInt8 byPackType, const RwChar *pPackHeaderFileName);
	RwInt32 SavePack(RwUInt8 byPackType, std::list<std::string>& listFolder, const RwChar *pPackHeaderFileName, const RwChar *pPackDataFileName);
	
	RwInt32 SaveTexturePack(void);
	RwInt32 SaveObjectPack(void);
	RwInt32 SaveTerrainPack(void);
	RwInt32 SaveGuiPack(void);
	RwInt32 SaveFlashPack(void);
	RwInt32 SaveSoundPack(void);
    RwInt32 SaveLanguagePack(void);
    RwInt32 SaveScriptPack(void);
    RwInt32 SaveTablePack(void);
    RwInt32 SavePropertyPack(void);

	RwChar*	UnPackLeadBuffer(RwChar* pBuffer, const RwChar *pPackFileName, SFilePackHeaderElement *pElement);
	void	UnPackSaveBuffer(const RwChar *pFileName, const RwChar *pBuffer, SFilePackHeaderElement *pElement);
	void	UnPack(const RwChar *pFileName, const RwChar *pPackFileName, SFilePackHeaderElement *pElement, RwUInt32 uiPercent);

	FILE*	GetPakFileFromZip(const char* pFileName);

public:

	CNtlPLResoucePackManager();
	~CNtlPLResoucePackManager();

	static CNtlPLResoucePackManager* GetInstance(void);

	void					SetActiveTypeFlags(RwUInt32 uiFlags);
	RwUInt32				GetActiveFlags(void) const;

	CNtlPLResourcePack*		FindResourcePack(RwUInt8 byPackType);
	SFilePackHeaderElement* FindHeaderElement(RwUInt8 byPackType, const RwChar *pFileName);

	RwTexture*				LoadTexture(const RwChar *pPath, const RwChar *pFileName, const RwChar *pMaskName);
	RwBool					LoadObject(const RwChar *pFileName, SPackResFileData& sPackFileData);
	RwBool					LoadTerrain(const RwChar *pFileName, SPackResFileData& sPackFileData);
	std::string				LoadTerrain(const RwChar *pFileName, RwUInt32& uiOffset, RwUInt32& uiSize);
	void					LoadGui(const RwChar *pFileName, void **pData, RwUInt32 *uiSize);
	void					LoadFlash(const RwChar *pFileName, void **pData, RwInt32 *iSize );
	void					LoadSound(const RwChar *pFileName, void **pData, RwInt32 *iSize );
    void                    LoadLanguage(const RwChar* pFileName, void** pData, RwInt32* iSize);
    void                    LoadScript(const RwChar* pFileName, void** pData, RwInt32* iSize);
	void                    LoadTable(const RwChar* pFileName, void** pData, RwInt32* iSize);
    void                    LoadProperty(const RwChar* pFileName, void** pData, RwInt32* iSize);

	RwInt32					LoadPackHeader(void);
	RwInt32					SavePack(RwUInt8 byPackType);
	RwInt32					UnPack(void);
    RwInt32                 UnPack(EFilePackType ePackType);

	bool					IsExistSoundFile(const RwChar *pFileName);

	template <class Callbackclass>
	void LinkMakePackNotify(Callbackclass *cbclass,int (Callbackclass::*callback)(unsigned int uiParam0, unsigned int uiProgess))
	{
		if(m_pCallMakePackNotify)
			return;

		m_pCallMakePackNotify = NTL_NEW CNtlCallbackWidget2<Callbackclass> (cbclass,callback);
	}

	void UnLinkMakePackNotify(void)
	{
		if(m_pCallMakePackNotify)
		{
			NTL_DELETE(m_pCallMakePackNotify);
		}
	}
};

static CNtlPLResoucePackManager* GetNtlResourcePackManager(void)
{
	return CNtlPLResoucePackManager::GetInstance();
}


inline RwUInt32	CNtlPLResoucePackManager::GetActiveFlags(void) const
{
	return m_uiFlags;
}


RwTexture*	ResourcPackTextureLoad(const RwChar *pPath, const RwChar *pFileName, const RwChar *pMaskName);
void		GuiPackLoad(const RwChar *pFileName, void **pData, unsigned int *uiSize);
void		FlashPackLoad(const RwChar *pFileName, void **pData, int *iSize);
void		SoundPackLoad(const RwChar *pFileName, void **pData, int *iSize);
void		LanguagePackLoad(const RwChar *pFileName, void **pData, int *iSize);
void		ScriptPackLoad(const RwChar* pFileName, void **pData, int *iSize);

bool		IsExistSoundFile(const RwChar *pFileName);

//////////////////////////////////////////////////////////////////////////

/**
 * \ingroup NtlPresentation
 * \brief 팩 테이블 데이터를 읽어오기 위한 클래스 
 * 서버쪽 코드에 맞춰서 테이블만 따로 클래스를 만든다.
 * \date 2009-03-17
 * \author agebreak
 */
class CTablePackLoad : public CTableContainer::ICallBack
{
public:
	CTablePackLoad( void );
	virtual ~CTablePackLoad( void );

public:
    virtual bool Call( const char* pfilename, CNtlFileSerializer* pSeralize, const char* pszCryptPassword );  ///< 콜백되는 함수
};

#endif