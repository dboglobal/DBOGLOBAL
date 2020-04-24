#pragma once


#include "NtlCharacter.h"
#include "NtlPLItemProperty.h"
#include "NtlXmlDoc.h"


#define DEF_ITEM_XML_FILE     ".\\script\\DefaultItemList.XML"


enum eRACE_GENDER_CLASS_TYPE
{
	HUMAN_ADULT_MALE_FIGHTER = 0,
	HUMAN_ADULT_MALE_MYSTIC,
	HUMAN_ADULT_MALE_ENGINEER,
	HUMAN_CHILD_MALE_FIGHTER,
	HUMAN_CHILD_MALE_MYSTIC, 
	HUMAN_CHILD_MALE_ENGINEER,
	HUMAN_ADULT_FEMALE_FIGHTER,
	HUMAN_ADULT_FEMALE_MYSTIC, 
	HUMAN_ADULT_FEMALE_ENGINEER,
	HUMAN_CHILD_FEMALE_FIGHTER,
	HUMAN_CHILD_FEMALE_MYSTIC, 
	HUMAN_CHILD_FEMALE_ENGINEER,

	NAMEK_ADULT_FIGHTER,
	NAMEK_ADULT_MYSTIC,
	NAMEK_CHILD_FIGHTER,
	NAMEK_CHILD_MYSTIC,

	MAJIN_ADULT_MALE_MIGHTY,
	MAJIN_ADULT_MALE_WONDER,
	MAJIN_CHILD_MALE_MIGHTY,
	MAJIN_CHILD_MALE_WONDER,
	MAJIN_ADULT_FEMALE_MIGHTY,
	MAJIN_ADULT_FEMALE_WONDER,
	MAJIN_CHILD_FEMALE_MIGHTY,
	MAJIN_CHILD_FEMALE_WONDER,

	RGCT_TOT_CNT,
	RACE_GENDER_CLASS_TYPE_INVALID,
};

enum EClassGenderType
{
	CLASS_GENDER_INVALID,

	HUMAN_ADULT_MALE,
	HUMAN_ADULT_FEMALE,
	HUMAN_CHILD_MALE,
	HUMAN_CHILD_FEMALE,
	
	NAMEK_ADULT,
	NAMEK_CHILD,

	MAJIN_ADULT_MALE,
	MAJIN_ADULT_FEMALE,
	MAJIN_CHILD_MALE,
	MAJIN_CHILD_FEMALE,
};

#define CREATE_DEF_ITEM_HEAD(EClassGenderType, HEAD_TYPE)					(EClassGenderType << 16) | HEAD_TYPE
#define CREATE_DEF_ITEM_FACE(EClassGenderType, FACE_TYPE)					(EClassGenderType << 16) | FACE_TYPE

#define CREATE_DEF_ITEM_BODY(EClassGenderType, COLOR_TYPE, ATTACH_SLOT)		(EClassGenderType << 16) | (COLOR_TYPE << 8) | ATTACH_SLOT
#define CREATE_DEF_ITEM_TEX(EClassGenderType, FACE_TYPE, SKIN_COLOR_TYPE)	(EClassGenderType << 16) | (FACE_TYPE << 8) | SKIN_COLOR_TYPE

#define CREATE_DEF_ITEM_SKIN_COLOR(EClassGenderType, SKIN_COLOR_TYPE)		(EClassGenderType << 16) | SKIN_COLOR_TYPE
#define CREATE_DEF_ITEM_HEAD_COLOR(EClassGenderType, HEAD_COLOR_TYPE)		(EClassGenderType << 16) | HEAD_COLOR_TYPE

#define DEF_ITEM_START	1
#define DEF_ITEM_END	5

struct SScouterAttachInfo
{
	RwReal	fScaleX;
	RwReal	fScaleY;
	RwReal	fScaleZ;

	RwReal  fRotX;

	RwReal	fOffsetX;
	RwReal	fOffsetY;
	RwReal	fOffsetZ;

	SScouterAttachInfo() : fScaleX(1.f), 
		                   fScaleY(1.f), 
						   fScaleZ(1.f), 
						   fRotX(0.0f),
						   fOffsetX(0.f), 
						   fOffsetY(0.f), 
						   fOffsetZ(0.f) {}
};

typedef std::map<D3DCOLOR, RwTexture *>					MAP_CHAR_SKIN_TEX;
typedef	std::map<D3DCOLOR, RwTexture *>::iterator		MAP_CHAR_SKIN_TEX_ITER;

typedef	std::map<RwUInt32, RwRGBA *>					DEF_ITEM_HEAD_COLOR_MAP;
typedef	std::map<RwUInt32, RwRGBA *>::iterator			DEF_ITEM_HEAD_COLOR_MAP_ITER;

typedef	std::map<RwUInt32, RwRGBA *>					DEF_ITEM_SKIN_COLOR_MAP;
typedef	std::map<RwUInt32, RwRGBA *>::iterator			DEF_ITEM_SKIN_COLOR_MAP_ITER;

typedef	std::map<RwUInt32, std::string >				DEF_ITEM_HEAD_KEY_NAME;
typedef	std::map<RwUInt32, std::string >::iterator		DEF_ITEM_HEAD_KEY_NAME_ITER;

typedef	std::map<RwUInt32, std::string >				DEF_ITEM_FACE_KEY_NAME;
typedef	std::map<RwUInt32, std::string >::iterator		DEF_ITEM_FACE_KEY_NAME_ITER;

typedef	std::map<RwUInt32, std::string >				DEF_ITEM_FACE_BASE_TEX_NAME_MAP;
typedef	std::map<RwUInt32, std::string >::iterator		DEF_ITEM_FACE_BASE_TEX_NAME_MAP_ITER;

typedef	std::map<RwUInt32, std::string >				DEF_ITEM_FACE_MULTI_TEX_NAME_MAP;
typedef	std::map<RwUInt32, std::string >::iterator		DEF_ITEM_FACE_MULTI_TEX_NAME_MAP_ITER;

typedef	std::map<RwUInt32, std::string >				DEF_ITEM_HEAD_BASE_TEX_NAME_MAP;
typedef	std::map<RwUInt32, std::string >::iterator		DEF_ITEM_HEAD_BASE_TEX_NAME_MAP_ITER;

typedef	std::map<RwUInt32, std::string >				DEF_ITEM_HEAD_MULTI_TEX_NAME_MAP;
typedef	std::map<RwUInt32, std::string >::iterator		DEF_ITEM_HEAD_MULTI_TEX_NAME_MAP_ITER;

typedef	std::map<RwUInt32, std::string >				DEF_ITEM_PARTS_MAP;
typedef	std::map<RwUInt32, std::string >::iterator		DEF_ITEM_PARTS_MAP_ITER;

typedef std::map<RwUInt32, SScouterAttachInfo *>		   DEF_ITEM_SCOUTER_ATTACH_INFO_MAP;
typedef std::map<RwUInt32, SScouterAttachInfo *>::iterator DEF_ITEM_SCOUTER_ATTACH_INFO_MAP_ITER;



class CNtlDefaultItemTable
{
public:
	CNtlDefaultItemTable() {Create();};
	~CNtlDefaultItemTable() {Destroy();};	

protected:
	MAP_CHAR_SKIN_TEX					m_mapCharSkinTex;			// Skin textures for characters

	DEF_ITEM_HEAD_COLOR_MAP				m_mapHeadColor;				///< Head Color
	DEF_ITEM_SKIN_COLOR_MAP				m_mapSkinColor;				///< Skin Color

	DEF_ITEM_HEAD_KEY_NAME				m_mapHeadKeyName;			///< Head Key Name
	DEF_ITEM_HEAD_BASE_TEX_NAME_MAP		m_mapHeadBaseTexName;		///< Head Base Tex Name
	
	DEF_ITEM_FACE_KEY_NAME				m_mapFaceKeyName;			///< Face Key Name
	DEF_ITEM_FACE_BASE_TEX_NAME_MAP		m_mapFaceBaseTexName;		///< BaseTexName
	DEF_ITEM_FACE_MULTI_TEX_NAME_MAP	m_mapFaceMultiTexName;		///< MultiTexName
	DEF_ITEM_FACE_MULTI_TEX_NAME_MAP	m_mapFaceAniMultiTexName;	///< AniMultiTexName

	DEF_ITEM_PARTS_MAP					m_mapParts;					///< Default Parts
	DEF_ITEM_SCOUTER_ATTACH_INFO_MAP	m_mapScouterAttachInfo;		///< Scouter Attach Info

    // 초사이어인용 데이터
    DEF_ITEM_HEAD_KEY_NAME              m_mapSuperSaiyanHeadKeyName;    ///< 초사이어인용 Head
    DEF_ITEM_FACE_MULTI_TEX_NAME_MAP    m_mapSuperSaiyanFaceMultiTexName;   ///< 초사이어인용 Face

public:
	void operator = (CNtlDefaultItemTable& _TmpTbl);

public:

	static	CNtlDefaultItemTable&	GetInstance();
	static	EClassGenderType		GetClassGenderType(RwUInt32 uiRace, RwUInt32 uiGender, RwBool bIsAdult);
	static	eRACE_GENDER_CLASS_TYPE GetCharType(RwUInt32 _Class, RwUInt32 _Gender, RwBool bIsAdult);

	std::string *GetHeadKeyName(EClassGenderType eClassGenderType, RwUInt32 uiHeadType);
	RwBool		 AddHeadKeyName(EClassGenderType eClassGenderType, RwUInt32 uiHeadType, const std::string &strHeadKeyName);

	std::string *GetHeadBaseTextureName(EClassGenderType eClassGenderType, RwUInt32 uiHeadType, RwUInt32 uiSkinColorType);
	RwBool		 AddHeadBaseTextureName(EClassGenderType eClassGenderType, RwUInt32 uiHeadType, RwUInt32 uiSkinColorType, const std::string &strFaceTextureName);
	
	std::string *GetFaceKeyName(EClassGenderType eClassGenderType, RwUInt32 uiFaceType);
	RwBool		 AddFaceKeyName(EClassGenderType eClassGenderType, RwUInt32 uiFaceType, const std::string &strFaceKeyName);
	
	std::string *GetFaceBaseTextureName(EClassGenderType eClassGenderType, RwUInt32 uiFaceType, RwUInt32 uiSkinColorType);
	RwBool		 AddFaceBaseTextureName(EClassGenderType eClassGenderType, RwUInt32 uiFaceType, RwUInt32 uiSkinColorType, const std::string &strFaceTextureName);

	std::string *GetFaceMultiTextureName(EClassGenderType eClassGenderType, RwUInt32 uiFaceType, RwUInt32 uiSkinColorType);
	RwBool		 AddFaceMultiTextureName(EClassGenderType eClassGenderType, RwUInt32 uiFaceType, RwUInt32 uiSkinColortype, const std::string &strFaceTextureName);

	std::string	*GetFaceAniMultiTextureName(EClassGenderType eClassGenderType, RwUInt32 uiFaceType, RwUInt32 uiSkinColorType);
	RwBool		 AddFaceAniMultiTextureName(EClassGenderType eClassGenderType, RwUInt32 uiFaceType, RwUInt32 uiSkinColorType, const std::string &strFaceTextureName);

	std::string *GetBodyKeyName(EClassGenderType eClassGenderType, RwUInt32 uiColorType, RwUInt32 uiAttachSlotType);
	RwBool		 AddBodyKeyName(EClassGenderType eClassGenderType, RwUInt32 uiColorType, RwUInt32 uiAttachSlotType, const std::string &strPartsName);
	
	RwRGBA		*GetSkinColor(EClassGenderType eClassGenderType, RwUInt32 uiSkinColorType);
	RwBool		 AddSkinColor(EClassGenderType eClassGenderType, RwUInt32 uiSkinColorType, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
	RwTexture*	 GetSkinTex(D3DCOLOR& _KeyColor);

	RwRGBA		*GetHeadColor(EClassGenderType eClassGenderType, RwUInt32 uiHeadColorType);
	RwBool		 AddHeadColor(EClassGenderType eClassGenderType, RwUInt32 uiHeadColorType, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);

	SScouterAttachInfo *GetScouterAttachInfo(eRACE_GENDER_CLASS_TYPE eClassGenderType);
	RwBool				AddScouterAttachInfo(eRACE_GENDER_CLASS_TYPE eClassGenderType, RwReal fScaleX, RwReal fScaleY, RwReal fScaleZ, RwReal fRotX, RwReal fOffsetX, RwReal fOffsetY, RwReal fOffsetZ);

    std::string *GetSuperSaiyanHeadKeyName(EClassGenderType eClassGenderType, RwUInt32 uiHeadType);
    RwBool		 AddSuperSaiyanHeadKeyName(EClassGenderType eClassGenderType, RwUInt32 uiHeadType, const std::string &strHeadKeyName);

    std::string *GetSuperSaiyanFaceMultiTextureName(EClassGenderType eClassGenderType, RwUInt32 uiFaceType, RwUInt32 uiSkinColorType);
    RwBool		 AddSuperSaiyanFaceMultiTextureName(EClassGenderType eClassGenderType, RwUInt32 uiFaceType, RwUInt32 uiSkinColortype, const std::string &strFaceTextureName);

	void	Create();
	void	Destroy();
	void	Refresh();

protected:
	RwBool	LoadXml();
	RwBool	LoadXmlData(CNtlXMLDoc *pDoc);
};
