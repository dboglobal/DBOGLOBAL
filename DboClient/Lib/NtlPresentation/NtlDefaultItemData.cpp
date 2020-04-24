#include "precomp_ntlpresentation.h"
#include "NtlDefaultItemData.h"
#include "NtlDebug.h"
#include "NtlLogic.h"

#include "NtlPLResourcePack.h"
#include "NtlPLResourceManager.h"


CNtlDefaultItemTable &CNtlDefaultItemTable::GetInstance()
{
	static CNtlDefaultItemTable	s_DefaultItemTable;
	return s_DefaultItemTable;
}


eRACE_GENDER_CLASS_TYPE CNtlDefaultItemTable::GetCharType(RwUInt32 _Class, RwUInt32 _Gender, RwBool bIsAdult)
{
	/////////////////////////////////////////////////////////////////////////////////////
	// HUMAN
	/////////////////////////////////////////////////////////////////////////////////////    

	if(_Class == PC_CLASS_HUMAN_FIGHTER || _Class == PC_CLASS_STREET_FIGHTER || _Class == PC_CLASS_SWORD_MASTER)
	{
		if(_Gender == GENDER_MALE)			
            return bIsAdult ? HUMAN_ADULT_MALE_FIGHTER : HUMAN_CHILD_MALE_FIGHTER;
		else if(_Gender == GENDER_FEMALE)	
            return bIsAdult ? HUMAN_ADULT_FEMALE_FIGHTER : HUMAN_CHILD_FEMALE_FIGHTER;
	}
	else if( _Class == PC_CLASS_HUMAN_MYSTIC || _Class == PC_CLASS_CRANE_ROSHI || _Class == PC_CLASS_TURTLE_ROSHI)
	{
		if(_Gender == GENDER_MALE)			
            return bIsAdult ? HUMAN_ADULT_MALE_MYSTIC : HUMAN_CHILD_MALE_MYSTIC;
		else if(_Gender == GENDER_FEMALE)	
            return bIsAdult ? HUMAN_ADULT_MALE_MYSTIC : HUMAN_CHILD_FEMALE_MYSTIC;
	}
	else if( _Class == PC_CLASS_HUMAN_ENGINEER || _Class == PC_CLASS_GUN_MANIA || _Class == PC_CLASS_MECH_MANIA)
	{
		if(_Gender == GENDER_MALE)			
            return bIsAdult ? HUMAN_ADULT_MALE_ENGINEER : HUMAN_CHILD_MALE_ENGINEER;
		else if(_Gender == GENDER_FEMALE)	
            return bIsAdult ? HUMAN_ADULT_FEMALE_ENGINEER : HUMAN_CHILD_FEMALE_ENGINEER;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	// NAMEK
	/////////////////////////////////////////////////////////////////////////////////////
	else if(_Class == PC_CLASS_NAMEK_FIGHTER || _Class == PC_CLASS_DARK_WARRIOR || _Class == PC_CLASS_SHADOW_KNIGHT)
	{
        return bIsAdult ? NAMEK_ADULT_FIGHTER : NAMEK_CHILD_FIGHTER;
	}	
	else if( _Class == PC_CLASS_NAMEK_MYSTIC || _Class == PC_CLASS_DENDEN_HEALER || _Class == PC_CLASS_POCO_SUMMONER)
	{
        return bIsAdult ? NAMEK_ADULT_MYSTIC : NAMEK_CHILD_MYSTIC;		
	}	

	/////////////////////////////////////////////////////////////////////////////////////
	// MAJIN
	/////////////////////////////////////////////////////////////////////////////////////
	if(_Class == PC_CLASS_MIGHTY_MAJIN || _Class == PC_CLASS_GRAND_MA || _Class == PC_CLASS_ULTI_MA)
	{
		if(_Gender == GENDER_MALE)			
            return bIsAdult ? MAJIN_ADULT_MALE_MIGHTY :  MAJIN_CHILD_MALE_MIGHTY;
		else if(_Gender == GENDER_FEMALE)	
            return bIsAdult ? MAJIN_ADULT_FEMALE_MIGHTY : MAJIN_CHILD_FEMALE_MIGHTY;
	}
	else if( _Class == PC_CLASS_WONDER_MAJIN || _Class == PC_CLASS_PLAS_MA || _Class == PC_CLASS_KAR_MA)
	{
		if(_Gender == GENDER_MALE)			
            return bIsAdult ? MAJIN_ADULT_MALE_WONDER : MAJIN_CHILD_MALE_WONDER;
		else if(_Gender == GENDER_FEMALE)	
            return bIsAdult ? MAJIN_ADULT_FEMALE_WONDER : MAJIN_CHILD_FEMALE_WONDER;
	}		

	return RACE_GENDER_CLASS_TYPE_INVALID;
}

EClassGenderType CNtlDefaultItemTable::GetClassGenderType(RwUInt32 uiRace, RwUInt32 uiGender, RwBool bIsAdult)
{
    if(uiRace == RACE_HUMAN)
    {
        if(uiGender == GENDER_MALE)
        {
            return bIsAdult ? HUMAN_ADULT_MALE : HUMAN_CHILD_MALE;
        }
        else
        {
            return bIsAdult ? HUMAN_ADULT_FEMALE : HUMAN_CHILD_FEMALE;
        }
    }
    else if(uiRace == RACE_NAMEK)
    {
        return bIsAdult ? NAMEK_ADULT : NAMEK_CHILD;
    }
    else if(uiRace == RACE_MAJIN)
    {
        if(uiGender == GENDER_MALE)
        {
            return bIsAdult ? MAJIN_ADULT_MALE : MAJIN_CHILD_MALE;
        }
        else 
        {
            return bIsAdult ? MAJIN_ADULT_FEMALE : MAJIN_CHILD_FEMALE;
        }
    }    

    return CLASS_GENDER_INVALID;
}

std::string *CNtlDefaultItemTable::GetHeadKeyName(EClassGenderType eClassGenderType, RwUInt32 uiHeadType)
{
	RwUInt32 uiKey = CREATE_DEF_ITEM_HEAD(eClassGenderType, uiHeadType);

	if(m_mapHeadKeyName.find( uiKey ) == m_mapHeadKeyName.end())
		return NULL;
	
	return &m_mapHeadKeyName[uiKey];
}

RwBool CNtlDefaultItemTable::AddHeadKeyName(EClassGenderType eClassGenderType, RwUInt32 uiHeadType, const std::string &strHeadKeyName)
{
	RwUInt32 uiKey = CREATE_DEF_ITEM_HEAD(eClassGenderType, uiHeadType);

	if(m_mapHeadKeyName.find( uiKey ) != m_mapHeadKeyName.end())
	{
		NTL_ASSERTE(0);
		return FALSE;
	}
	
	m_mapHeadKeyName[uiKey] = strHeadKeyName;

	return TRUE;
}

std::string * CNtlDefaultItemTable::GetSuperSaiyanHeadKeyName( EClassGenderType eClassGenderType, RwUInt32 uiHeadType ) 
{
    RwUInt32 uiKey = CREATE_DEF_ITEM_HEAD(eClassGenderType, uiHeadType);

    if(m_mapSuperSaiyanHeadKeyName.find( uiKey ) == m_mapSuperSaiyanHeadKeyName.end())
        return NULL;

    return &m_mapSuperSaiyanHeadKeyName[uiKey];
}

RwBool CNtlDefaultItemTable::AddSuperSaiyanHeadKeyName( EClassGenderType eClassGenderType, RwUInt32 uiHeadType, const std::string &strHeadKeyName ) 
{
    RwUInt32 uiKey = CREATE_DEF_ITEM_HEAD(eClassGenderType, uiHeadType);

    if(m_mapSuperSaiyanHeadKeyName.find( uiKey ) != m_mapSuperSaiyanHeadKeyName.end())
    {
        NTL_ASSERTE(0);
        return FALSE;
    }

    m_mapSuperSaiyanHeadKeyName[uiKey] = strHeadKeyName;

    return TRUE;
}

std::string *CNtlDefaultItemTable::GetHeadBaseTextureName(EClassGenderType eClassGenderType, RwUInt32 uiHeadType, RwUInt32 uiSkinColorType)
{
	RwUInt32 uiKey = CREATE_DEF_ITEM_TEX(eClassGenderType, uiHeadType, uiSkinColorType);
	if(m_mapHeadBaseTexName.find( uiKey ) == m_mapHeadBaseTexName.end())
		return NULL;

	return &m_mapHeadBaseTexName[uiKey];
}

RwBool CNtlDefaultItemTable::AddHeadBaseTextureName(EClassGenderType eClassGenderType, RwUInt32 uiHeadType, RwUInt32 uiSkinColorType, const std::string &strFaceTextureName)
{
	RwUInt32 uiKey = CREATE_DEF_ITEM_TEX(eClassGenderType, uiHeadType, uiSkinColorType);
	if(m_mapHeadBaseTexName.find( uiKey ) != m_mapHeadBaseTexName.end())
	{
		NTL_ASSERTE(0);
		return FALSE;
	}

	m_mapHeadBaseTexName[uiKey] = strFaceTextureName;
	return TRUE;
}

std::string *CNtlDefaultItemTable::GetFaceKeyName(EClassGenderType eClassGenderType, RwUInt32 uiFaceType)
{
	RwUInt32 uiKey = CREATE_DEF_ITEM_FACE(eClassGenderType, uiFaceType);
	if(m_mapFaceKeyName.find( uiKey ) == m_mapFaceKeyName.end())
		return NULL;
	return &m_mapFaceKeyName[uiKey];
}

RwBool CNtlDefaultItemTable::AddFaceKeyName(EClassGenderType eClassGenderType, RwUInt32 uiFaceType, const std::string &strFaceKeyName)
{
	RwUInt32 uiKey = CREATE_DEF_ITEM_FACE(eClassGenderType, uiFaceType);

	if(m_mapFaceKeyName.find( uiKey ) != m_mapFaceKeyName.end())
	{
		NTL_ASSERTE(0);
		return FALSE;
	}

	m_mapFaceKeyName[uiKey] = strFaceKeyName;
	
	return TRUE;
}

std::string *CNtlDefaultItemTable::GetFaceBaseTextureName(EClassGenderType eClassGenderType, RwUInt32 uiFaceType, RwUInt32 uiColorType)
{
	RwUInt32 uiKey = CREATE_DEF_ITEM_TEX(eClassGenderType, uiFaceType, uiColorType);
	if(m_mapFaceBaseTexName.find( uiKey ) == m_mapFaceBaseTexName.end())
		return NULL;

	return &m_mapFaceBaseTexName[uiKey];
}

RwBool CNtlDefaultItemTable::AddFaceBaseTextureName(EClassGenderType eClassGenderType, RwUInt32 uiFaceType, RwUInt32 uiColorType, const std::string &strFaceTextureName)
{
	RwUInt32 uiKey = CREATE_DEF_ITEM_TEX(eClassGenderType, uiFaceType, uiColorType);
	if(m_mapFaceBaseTexName.find( uiKey ) != m_mapFaceBaseTexName.end())
	{
		NTL_ASSERTE(0);
		return FALSE;
	}

	m_mapFaceBaseTexName[uiKey] = strFaceTextureName;
	return TRUE;
}

std::string *CNtlDefaultItemTable::GetFaceMultiTextureName(EClassGenderType eClassGenderType, RwUInt32 uiFaceType, RwUInt32 uiSkinColorType)
{
	RwUInt32 uiKey = CREATE_DEF_ITEM_TEX(eClassGenderType, uiFaceType, uiSkinColorType);
	if(m_mapFaceMultiTexName.find( uiKey ) == m_mapFaceMultiTexName.end())
		return NULL;

	return &m_mapFaceMultiTexName[uiKey];
}

RwBool CNtlDefaultItemTable::AddFaceMultiTextureName(EClassGenderType eClassGenderType, RwUInt32 uiFaceType, RwUInt32 uiSkinColorType, const std::string &strHairKeyName)
{
	RwUInt32 uiKey = CREATE_DEF_ITEM_TEX(eClassGenderType, uiFaceType, uiSkinColorType);
	if(m_mapFaceMultiTexName.find( uiKey ) != m_mapFaceMultiTexName.end())
	{
		NTL_ASSERTE(0);
		return FALSE;
	}
	
	m_mapFaceMultiTexName[uiKey] = strHairKeyName;
	return TRUE;
}


std::string * CNtlDefaultItemTable::GetSuperSaiyanFaceMultiTextureName( EClassGenderType eClassGenderType, RwUInt32 uiFaceType, RwUInt32 uiSkinColorType ) 
{
    RwUInt32 uiKey = CREATE_DEF_ITEM_TEX(eClassGenderType, uiFaceType, uiSkinColorType);
    if(m_mapSuperSaiyanFaceMultiTexName.find( uiKey ) == m_mapSuperSaiyanFaceMultiTexName.end())
        return NULL;

    return &m_mapSuperSaiyanFaceMultiTexName[uiKey];
}

RwBool CNtlDefaultItemTable::AddSuperSaiyanFaceMultiTextureName( EClassGenderType eClassGenderType, RwUInt32 uiFaceType, RwUInt32 uiSkinColortype, const std::string &strFaceTextureName ) 
{
    RwUInt32 uiKey = CREATE_DEF_ITEM_TEX(eClassGenderType, uiFaceType, uiSkinColortype);
    if(m_mapSuperSaiyanFaceMultiTexName.find( uiKey ) != m_mapSuperSaiyanFaceMultiTexName.end())
    {
        NTL_ASSERTE(0);
        return FALSE;
    }

    m_mapSuperSaiyanFaceMultiTexName[uiKey] = strFaceTextureName;
    return TRUE;
}

std::string	*CNtlDefaultItemTable::GetFaceAniMultiTextureName(EClassGenderType eClassGenderType, RwUInt32 uiFaceType, RwUInt32 uiSkinColorType)
{
	RwUInt32 uiKey = CREATE_DEF_ITEM_TEX(eClassGenderType, uiFaceType, uiSkinColorType);
	if(m_mapFaceAniMultiTexName.find( uiKey ) == m_mapFaceAniMultiTexName.end())
		return NULL;

	return &m_mapFaceAniMultiTexName[uiKey];
}

RwBool CNtlDefaultItemTable::AddFaceAniMultiTextureName(EClassGenderType eClassGenderType, RwUInt32 uiFaceType, RwUInt32 uiSkinColorType, const std::string &strFaceTextureName)
{
	RwUInt32 uiKey = CREATE_DEF_ITEM_TEX(eClassGenderType, uiFaceType, uiSkinColorType);
	if(m_mapFaceAniMultiTexName.find( uiKey ) != m_mapFaceAniMultiTexName.end())
	{
		NTL_ASSERTE(0);
		return FALSE;
	}
	
	m_mapFaceAniMultiTexName[uiKey] = strFaceTextureName;
	return TRUE;
}

std::string *CNtlDefaultItemTable::GetBodyKeyName(EClassGenderType eClassGenderType, RwUInt32 uiColorType, RwUInt32 uiAttachSlotType)
{
	RwUInt32 uiKey = CREATE_DEF_ITEM_BODY(eClassGenderType, uiColorType, uiAttachSlotType);
	if(m_mapParts.find( uiKey ) == m_mapParts.end())
		return NULL;

	return &m_mapParts[uiKey];
}

RwBool CNtlDefaultItemTable::AddBodyKeyName(EClassGenderType eClassGenderType, RwUInt32 uiColorType, RwUInt32 uiAttachSlotType, const std::string &strPartsName)
{
	RwUInt32 uiKey = CREATE_DEF_ITEM_BODY(eClassGenderType, uiColorType, uiAttachSlotType);

	if(m_mapParts.find( uiKey ) != m_mapParts.end())
	{
		NTL_ASSERTE(0);
		return FALSE;
	}

	m_mapParts[uiKey] = strPartsName;
	
	return TRUE;
}


RwRGBA *CNtlDefaultItemTable::GetSkinColor(EClassGenderType eClassGenderType, RwUInt32 uiSkinColorType)
{
	RwUInt32 uiKey = CREATE_DEF_ITEM_SKIN_COLOR(eClassGenderType, uiSkinColorType);
	if(m_mapSkinColor.find( uiKey ) == m_mapSkinColor.end())
		return NULL;

	return m_mapSkinColor[uiKey];
}

RwTexture* CNtlDefaultItemTable::GetSkinTex(D3DCOLOR& _KeyColor)
{
	D3DCOLOR ARGB = 0xff000000 | _KeyColor;

	if(m_mapCharSkinTex.find(ARGB) == m_mapCharSkinTex.end())
	{
		return NULL;
	}

	return m_mapCharSkinTex[ARGB];
}

RwBool CNtlDefaultItemTable::AddSkinColor(EClassGenderType eClassGenderType, RwUInt32 uiSkinColorType, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	RwUInt32 uiKey = CREATE_DEF_ITEM_SKIN_COLOR(eClassGenderType, uiSkinColorType);

	if(m_mapSkinColor.find( uiKey ) != m_mapSkinColor.end())
	{
		NTL_ASSERTE(0);
		return FALSE;
	}
	
	RwRGBA *pColor = NTL_NEW RwRGBA;
	pColor->red		= byRed;
	pColor->green	= byGreen;
	pColor->blue    = byBlue;

	m_mapSkinColor[uiKey] = pColor;

	// Skin textures
	D3DCOLOR	KeyColor		= 0xff000000 | (RwUInt32)byRed << 16 | (RwUInt32)byGreen << 8 | (RwUInt32)byBlue;
	RwBool		ColorExisting	= FALSE;
	RwTexture*  pCurSkinTex;

	if(m_mapCharSkinTex.find(KeyColor) == m_mapCharSkinTex.end())
	{
		pCurSkinTex = CNtlPLResourceManager::GetInstance()->CreateTexture("SkinTex", 2, 2, 32, rwRASTERFORMAT8888);
		CNtlPLResourceManager::GetInstance()->SetColor(pCurSkinTex, static_cast<DWORD>(KeyColor));

		m_mapCharSkinTex[KeyColor] = pCurSkinTex;
	}
	
	return TRUE;
}

RwRGBA *CNtlDefaultItemTable::GetHeadColor(EClassGenderType eClassGenderType, RwUInt32 uiHeadColorType)
{
	RwUInt32 uiKey = CREATE_DEF_ITEM_HEAD_COLOR(eClassGenderType, uiHeadColorType);
	if(m_mapHeadColor.find( uiKey ) == m_mapHeadColor.end())
		return NULL;

	return m_mapHeadColor[uiKey];
}

RwBool CNtlDefaultItemTable::AddHeadColor(EClassGenderType eClassGenderType, RwUInt32 uiHeadColorType, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue)
{
	RwUInt32 uiKey = CREATE_DEF_ITEM_HEAD_COLOR(eClassGenderType, uiHeadColorType);

	if(m_mapHeadColor.find( uiKey ) != m_mapHeadColor.end())
	{
		NTL_ASSERTE(0);
		return FALSE;
	}
	
	RwRGBA *pColor = NTL_NEW RwRGBA;
	pColor->red		= byRed;
	pColor->green	= byGreen;
	pColor->blue    = byBlue;

	m_mapHeadColor[uiKey] = pColor;
	
	return TRUE;
}

SScouterAttachInfo *CNtlDefaultItemTable::GetScouterAttachInfo(eRACE_GENDER_CLASS_TYPE eRGCT)
{
	RwUInt32 uiKey = static_cast<RwUInt32>(eRGCT);
	if( m_mapScouterAttachInfo.find( uiKey ) == m_mapScouterAttachInfo.end() )
		return NULL;

	return m_mapScouterAttachInfo[uiKey];
}

RwBool CNtlDefaultItemTable::AddScouterAttachInfo(eRACE_GENDER_CLASS_TYPE eRGCT, RwReal fScaleX, RwReal fScaleY, RwReal fScaleZ, RwReal fRotX, RwReal fOffsetX, RwReal fOffsetY, RwReal fOffsetZ)
{
	RwUInt32 uiKey = static_cast<RwUInt32>(eRGCT);
	if(m_mapScouterAttachInfo.find( uiKey ) != m_mapScouterAttachInfo.end() )
	{
		NTL_ASSERTE(0);
		return FALSE;
	}

	SScouterAttachInfo *pAttachInfo = NTL_NEW SScouterAttachInfo;
	pAttachInfo->fScaleX = fScaleX;
	pAttachInfo->fScaleY = fScaleY;
	pAttachInfo->fScaleZ = fScaleZ;

	pAttachInfo->fRotX = fRotX;

	pAttachInfo->fOffsetX = fOffsetX;
	pAttachInfo->fOffsetY = fOffsetY;
	pAttachInfo->fOffsetZ = fOffsetZ;

	m_mapScouterAttachInfo[uiKey] = pAttachInfo;

	return TRUE;
}

/*
SScouterAttachInfo *CNtlDefaultItemTable::GetScouterAttachInfo(EClassGenderType eClassGenderType)
{
	RwUInt32 uiKey = eClassGenderType;
	if( m_mapScouterAttachInfo.find( uiKey ) == m_mapScouterAttachInfo.end() )
		return NULL;

	return m_mapScouterAttachInfo[uiKey];
}

RwBool CNtlDefaultItemTable::AddScouterAttachInfo(EClassGenderType eClassGenderType, RwReal fScaleX, RwReal fScaleY, RwReal fScaleZ, RwReal fOffsetX, RwReal fOffsetY, RwReal fOffsetZ)
{
	RwUInt32 uiKey = eClassGenderType;
	if(m_mapScouterAttachInfo.find( uiKey ) != m_mapScouterAttachInfo.end() )
	{
		NTL_ASSERTE(0);
		return FALSE;
	}

	SScouterAttachInfo *pAttachInfo = NTL_NEW SScouterAttachInfo;
	pAttachInfo->fScaleX = fScaleX;
	pAttachInfo->fScaleY = fScaleY;
	pAttachInfo->fScaleZ = fScaleZ;

	pAttachInfo->fOffsetX = fOffsetX;
	pAttachInfo->fOffsetY = fOffsetY;
	pAttachInfo->fOffsetZ = fOffsetZ;

	m_mapScouterAttachInfo[uiKey] = pAttachInfo;

	return TRUE;
}
*/
void CNtlDefaultItemTable::operator = (CNtlDefaultItemTable& _TmpTbl)
{
	// need to clone specific part of scripts

}

void CNtlDefaultItemTable::Refresh()
{
	Destroy();
	Create();
}

void CNtlDefaultItemTable::Create()
{
	LoadXml();
}

void CNtlDefaultItemTable::Destroy()
{
	MAP_CHAR_SKIN_TEX_ITER stSkinMapIT  = m_mapCharSkinTex.begin();
	MAP_CHAR_SKIN_TEX_ITER endSkinMapIT = m_mapCharSkinTex.end();
	for(;stSkinMapIT != endSkinMapIT;)
	{
		RwTexture* pTex = stSkinMapIT->second;
		if(pTex != NULL)
		{
			RwTextureDestroy(pTex);
		}
		++stSkinMapIT;
	}
	m_mapCharSkinTex.clear();

	DEF_ITEM_SKIN_COLOR_MAP_ITER stSkinColorMapIT  = m_mapSkinColor.begin();
	DEF_ITEM_SKIN_COLOR_MAP_ITER endSkinColorMapIT = m_mapSkinColor.end();
	for(;stSkinColorMapIT != endSkinColorMapIT;)
	{
		RwRGBA *pColor = stSkinColorMapIT->second;
		if(pColor != NULL)
		{
			NTL_DELETE( pColor );
		}
		++stSkinColorMapIT;
	}
	m_mapSkinColor.clear();

	DEF_ITEM_HEAD_COLOR_MAP_ITER stHeadColorMapIT  = m_mapHeadColor.begin();
	DEF_ITEM_HEAD_COLOR_MAP_ITER endHeadColorMapIT = m_mapHeadColor.end();
	for(;stHeadColorMapIT != endHeadColorMapIT;)
	{
		RwRGBA *pColor = stHeadColorMapIT->second;
		if(pColor != NULL)
		{
			NTL_DELETE( pColor );
		}
		++stHeadColorMapIT;
	}
	
	DEF_ITEM_SCOUTER_ATTACH_INFO_MAP_ITER stScouterAttachInfoMapIT = m_mapScouterAttachInfo.begin();
	DEF_ITEM_SCOUTER_ATTACH_INFO_MAP_ITER endScouterAttachInfoMapIT = m_mapScouterAttachInfo.end();
	for(;stScouterAttachInfoMapIT != endScouterAttachInfoMapIT;)
	{
		SScouterAttachInfo *pAttachInfo = stScouterAttachInfoMapIT->second;
		if(pAttachInfo != NULL)
		{
			NTL_DELETE( pAttachInfo );
		}
		++stScouterAttachInfoMapIT;
	}

	m_mapCharSkinTex.clear();
	m_mapHeadColor.clear();
	m_mapSkinColor.clear();
	m_mapHeadKeyName.clear();
	m_mapHeadBaseTexName.clear();
	m_mapFaceKeyName.clear();
	m_mapFaceBaseTexName.clear();
	m_mapFaceMultiTexName.clear();
	m_mapFaceAniMultiTexName.clear();
	m_mapParts.clear();
	m_mapScouterAttachInfo.clear();
    m_mapSuperSaiyanHeadKeyName.clear();
    m_mapSuperSaiyanFaceMultiTexName.clear();
}

RwBool CNtlDefaultItemTable::LoadXml() 
{
	CNtlXMLDoc xmlDoc;              
	
	if(!xmlDoc.Create())
	{
		NTL_ASSERTFAIL("xmlDoc.Create Fail\n");
		return FALSE;
	}
	
	if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_SCRIPT)
	{		
		BYTE*	pPackBuffer	= NULL;
		RwInt32	iPackSize	= 0;

		GetNtlResourcePackManager()->LoadScript(DEF_ITEM_XML_FILE, (void**)&pPackBuffer, &iPackSize);
		if (pPackBuffer)
		{
			BYTE* pTempBuffer = NTL_NEW BYTE [iPackSize + 1];
			memcpy(pTempBuffer, pPackBuffer, iPackSize);
			pTempBuffer[iPackSize] = '\0';

			if(!xmlDoc.LoadXML((char*)pTempBuffer))
			{			
				xmlDoc.Destroy();
				NTL_ARRAY_DELETE(pTempBuffer);
				NTL_ARRAY_DELETE(pPackBuffer);
				return FALSE;
			}
			NTL_ARRAY_DELETE(pTempBuffer);
			NTL_ARRAY_DELETE(pPackBuffer);
		}
	}
	else
	{
		if(!xmlDoc.Load(DEF_ITEM_XML_FILE))
		{
			xmlDoc.Destroy();
			return FALSE;
		}
	}

	if(!LoadXmlData(&xmlDoc))
	{
		NTL_ASSERTFAIL("Item/DefaultItemList.xml parse Fail\n");
		return FALSE;
	}
	

	xmlDoc.Destroy();

	return TRUE;
}

RwBool CNtlDefaultItemTable::LoadXmlData(CNtlXMLDoc *pDoc) 
{
	IXMLDOMNodeList* pItemPropertyNodeList = pDoc->SelectNodeList((char*)"DEFAULT_ITEM_PROPERTY");
    if(pItemPropertyNodeList == NULL)
	{
		NTL_ASSERTFAIL("script/DefaultItemList.xml error");
        return FALSE;
	}

	EClassGenderType eClassGenderType = CLASS_GENDER_INVALID;

	long itemPropertyCount = 0;
	pItemPropertyNodeList->get_length(&itemPropertyCount);
	for(long i = 0; i < itemPropertyCount; ++i)
	{
		IXMLDOMNode* pItemPropertyNode = NULL;
		pItemPropertyNodeList->get_item(i, &pItemPropertyNode);
		if(pItemPropertyNode)
		{
			IXMLDOMNodeList* pCharTypeNodeList = NULL;
			pItemPropertyNode->selectNodes(L"CHARACTER_TYPE", &pCharTypeNodeList);
			if(pCharTypeNodeList == NULL)
			{
				
				NTL_ASSERTFAIL("script/DefaultItemList.xml error");
				return FALSE;
			}

			if(pCharTypeNodeList)
			{
				long charTypeCount = 0;
				pCharTypeNodeList->get_length(&charTypeCount);
				for(long m = 0; m < charTypeCount; ++m)
				{
					IXMLDOMNode* pCharTypeNode = NULL;
					pCharTypeNodeList->get_item(m, &pCharTypeNode);
					if(pCharTypeNode)
					{
						char	szCharTypeName[64] = {0,};
						if(!pDoc->GetTextWithAttributeName(pCharTypeNode,"TYPE", szCharTypeName, sizeof(szCharTypeName)))
						{
							NTL_ASSERTFAIL("script/DefaultItemList.xml error");
							return FALSE;
						}
						
						//Character Type
						if(_stricmp(szCharTypeName, "HUMAN_ADULT_MALE") == 0)
							eClassGenderType = HUMAN_ADULT_MALE;
						else if(_stricmp(szCharTypeName, "HUMAN_ADULT_FEMALE") == 0)
							eClassGenderType = HUMAN_ADULT_FEMALE;
						else if(_stricmp(szCharTypeName, "HUMAN_CHILD_MALE") == 0)
							eClassGenderType = HUMAN_CHILD_MALE;
						else if(_stricmp(szCharTypeName, "HUMAN_CHILD_FEMALE") == 0)
							eClassGenderType = HUMAN_CHILD_FEMALE;
						else if(_stricmp(szCharTypeName, "NAMEK_ADULT") == 0)
							eClassGenderType = NAMEK_ADULT;
						else if(_stricmp(szCharTypeName, "NAMEK_CHILD") == 0)
							eClassGenderType = NAMEK_CHILD;
						else if(_stricmp(szCharTypeName, "MAJIN_ADULT_MALE") == 0)
							eClassGenderType = MAJIN_ADULT_MALE;
						else if(_stricmp(szCharTypeName, "MAJIN_ADULT_FEMALE") == 0)
							eClassGenderType = MAJIN_ADULT_FEMALE;
						else if(_stricmp(szCharTypeName, "MAJIN_CHILD_MALE") == 0)
							eClassGenderType = MAJIN_CHILD_MALE;
						else if(_stricmp(szCharTypeName, "MAJIN_CHILD_FEMALE") == 0)
							eClassGenderType = MAJIN_CHILD_FEMALE;
						else
						{
							NTL_ASSERTFAIL("script/DefaultItemList.xml error");
							return FALSE;
						}
						
						IXMLDOMNodeList* pType_NodeList		= NULL;
						IXMLDOMNodeList* pData_NodeList		= NULL;
						long typeCount = 0;
						long dataCount = 0;
						
						pCharTypeNode->selectNodes(L"FACE_MESH", &pType_NodeList);
						if(pType_NodeList)
						{
							pType_NodeList->get_length(&typeCount);
							for(long n = 0; n < typeCount; ++n)
							{
								IXMLDOMNode* pTypeNode = NULL;
								pType_NodeList->get_item(n, &pTypeNode);
								pTypeNode->selectNodes(L"DATA", &pData_NodeList);
								if(pData_NodeList)
								{
									pData_NodeList->get_length(&dataCount);
									for(long t = 0; t < dataCount; t++)
									{
										IXMLDOMNode* pDataNode = NULL;
										pData_NodeList->get_item(t, &pDataNode);
										char szType[32] = {0,};
										char szKeyName[64] = {0,};
										
										if(!pDoc->GetTextWithAttributeName(pDataNode,"TYPE", szType, sizeof(szType)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}

										if(!pDoc->GetTextWithAttributeName(pDataNode,"KEY_NAME", szKeyName, sizeof(szKeyName)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}
										AddFaceKeyName(eClassGenderType,(RwUInt32)atoi(szType), szKeyName); 
									}
								}
							}
						}

						pCharTypeNode->selectNodes(L"FACE_BASE_TEXTURE", &pType_NodeList);
						if(pType_NodeList)
						{
							pType_NodeList->get_length(&typeCount);
							for(long n = 0; n < typeCount; ++n)
							{
								IXMLDOMNode* pTypeNode = NULL;
								pType_NodeList->get_item(n, &pTypeNode);
								pTypeNode->selectNodes(L"DATA", &pData_NodeList);
								if(pData_NodeList)
								{
									pData_NodeList->get_length(&dataCount);
									for(long t = 0; t < dataCount; t++)
									{
										IXMLDOMNode* pDataNode = NULL;
										pData_NodeList->get_item(t, &pDataNode);
										char szType[32] = {0,};
										char szColorType[32] = {0,};
										char szTexName[64] = {0,};
										
										if(!pDoc->GetTextWithAttributeName(pDataNode,"TYPE", szType, sizeof(szType)))
										{
											NTL_ASSERTFAIL("scirpt/DefaultItemList.xml error");
											return FALSE;
										}

										if(!pDoc->GetTextWithAttributeName(pDataNode,"COLOR_TYPE", szColorType, sizeof(szColorType)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}

										if(!pDoc->GetTextWithAttributeName(pDataNode,"TEX_NAME", szTexName, sizeof(szTexName)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}
										AddFaceBaseTextureName(eClassGenderType, (RwUInt32)atoi(szType), (RwUInt32)atoi(szColorType), szTexName); 
									}
								}
							}
						}
					
						pCharTypeNode->selectNodes(L"FACE_MULTI_TEXTURE", &pType_NodeList);
						if(pType_NodeList)
						{
							pType_NodeList->get_length(&typeCount);
							for(long n = 0; n < typeCount; ++n)
							{
								IXMLDOMNode* pTypeNode = NULL;
								pType_NodeList->get_item(n, &pTypeNode);
								pTypeNode->selectNodes(L"DATA", &pData_NodeList);
								if(pData_NodeList)
								{
									pData_NodeList->get_length(&dataCount);
									for(long t = 0; t < dataCount; t++)
									{
										IXMLDOMNode* pDataNode = NULL;
										pData_NodeList->get_item(t, &pDataNode);
										char szType[32] = {0,};
										char szColorType[32] = {0,};
										char szTexName[64] = {0,};
										
										if(!pDoc->GetTextWithAttributeName(pDataNode,"TYPE", szType, sizeof(szType)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}

										if(!pDoc->GetTextWithAttributeName(pDataNode,"COLOR_TYPE", szColorType, sizeof(szColorType)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}

										if(!pDoc->GetTextWithAttributeName(pDataNode,"TEX_NAME", szTexName, sizeof(szTexName)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}
										AddFaceMultiTextureName(eClassGenderType, (RwUInt32)atoi(szType), (RwUInt32)atoi(szColorType), szTexName); 
									}
								}
							}
						}

                        pCharTypeNode->selectNodes(L"FACE_MULTI_TEXTURE_SUPER_SAIYAN", &pType_NodeList);
                        if(pType_NodeList)
                        {
                            pType_NodeList->get_length(&typeCount);
                            for(long n = 0; n < typeCount; ++n)
                            {
                                IXMLDOMNode* pTypeNode = NULL;
                                pType_NodeList->get_item(n, &pTypeNode);
                                pTypeNode->selectNodes(L"DATA", &pData_NodeList);
                                if(pData_NodeList)
                                {
                                    pData_NodeList->get_length(&dataCount);
                                    for(long t = 0; t < dataCount; t++)
                                    {
                                        IXMLDOMNode* pDataNode = NULL;
                                        pData_NodeList->get_item(t, &pDataNode);
                                        char szType[32] = {0,};
                                        char szColorType[32] = {0,};
                                        char szTexName[64] = {0,};

                                        if(!pDoc->GetTextWithAttributeName(pDataNode,"TYPE", szType, sizeof(szType)))
                                        {
                                            NTL_ASSERTFAIL("script/DefaultItemList.xml error");
                                            return FALSE;
                                        }

                                        if(!pDoc->GetTextWithAttributeName(pDataNode,"COLOR_TYPE", szColorType, sizeof(szColorType)))
                                        {
                                            NTL_ASSERTFAIL("script/DefaultItemList.xml error");
                                            return FALSE;
                                        }

                                        if(!pDoc->GetTextWithAttributeName(pDataNode,"TEX_NAME", szTexName, sizeof(szTexName)))
                                        {
                                            NTL_ASSERTFAIL("script/DefaultItemList.xml error");
                                            return FALSE;
                                        }
                                        AddSuperSaiyanFaceMultiTextureName(eClassGenderType, (RwUInt32)atoi(szType), (RwUInt32)atoi(szColorType), szTexName); 
                                    }
                                }
                            }
                        }

						pCharTypeNode->selectNodes(L"FACE_ANI_MULTI_TEXTURE", &pType_NodeList);
						if(pType_NodeList)
						{
							pType_NodeList->get_length(&typeCount);
							for(long n = 0; n < typeCount; ++n)
							{
								IXMLDOMNode* pTypeNode = NULL;
								pType_NodeList->get_item(n, &pTypeNode);
								pTypeNode->selectNodes(L"DATA", &pData_NodeList);
								if(pData_NodeList)
								{
									pData_NodeList->get_length(&dataCount);
									for(long t = 0; t < dataCount; t++)
									{
										IXMLDOMNode* pDataNode = NULL;
										pData_NodeList->get_item(t, &pDataNode);
										char szType[32] = {0,};
										char szColorType[32] = {0,};
										char szTexName[64] = {0,};
										
										if(!pDoc->GetTextWithAttributeName(pDataNode,"TYPE", szType, sizeof(szType)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}

										if(!pDoc->GetTextWithAttributeName(pDataNode,"COLOR_TYPE", szColorType, sizeof(szColorType)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}

										if(!pDoc->GetTextWithAttributeName(pDataNode,"TEX_NAME", szTexName, sizeof(szTexName)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}
										AddFaceAniMultiTextureName(eClassGenderType, (RwUInt32)atoi(szType), (RwUInt32)atoi(szColorType), szTexName); 
									}
								}
							}
						}
						
						pCharTypeNode->selectNodes(L"HEAD_MESH", &pType_NodeList);
						if(pType_NodeList)
						{
							pType_NodeList->get_length(&typeCount);
							for(long n = 0; n < typeCount; ++n)
							{
								IXMLDOMNode* pTypeNode = NULL;
								pType_NodeList->get_item(n, &pTypeNode);
								pTypeNode->selectNodes(L"DATA", &pData_NodeList);
								if(pData_NodeList)
								{
									pData_NodeList->get_length(&dataCount);
									for(long t = 0; t < dataCount; t++)
									{
										IXMLDOMNode* pDataNode = NULL;
										pData_NodeList->get_item(t, &pDataNode);
										char szType[32] = {0,};
										char szKeyName[64] = {0,};
										
										if(!pDoc->GetTextWithAttributeName(pDataNode,"TYPE", szType, sizeof(szType)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}
										if(!pDoc->GetTextWithAttributeName(pDataNode,"KEY_NAME", szKeyName, sizeof(szKeyName)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}
										AddHeadKeyName(eClassGenderType, (RwUInt32)atoi(szType), szKeyName);
									}
								}
							}
						}

                        pCharTypeNode->selectNodes(L"HEAD_MESH_SUPER_SAIYAN", &pType_NodeList);
                        if(pType_NodeList)
                        {
                            pType_NodeList->get_length(&typeCount);
                            for(long n = 0; n < typeCount; ++n)
                            {
                                IXMLDOMNode* pTypeNode = NULL;
                                pType_NodeList->get_item(n, &pTypeNode);
                                pTypeNode->selectNodes(L"DATA", &pData_NodeList);
                                if(pData_NodeList)
                                {
                                    pData_NodeList->get_length(&dataCount);
                                    for(long t = 0; t < dataCount; t++)
                                    {
                                        IXMLDOMNode* pDataNode = NULL;
                                        pData_NodeList->get_item(t, &pDataNode);
                                        char szType[32] = {0,};
                                        char szKeyName[64] = {0,};

                                        if(!pDoc->GetTextWithAttributeName(pDataNode,"TYPE", szType, sizeof(szType)))
                                        {
                                            NTL_ASSERTFAIL("script/DefaultItemList.xml error");
                                            return FALSE;
                                        }
                                        if(!pDoc->GetTextWithAttributeName(pDataNode,"KEY_NAME", szKeyName, sizeof(szKeyName)))
                                        {
                                            NTL_ASSERTFAIL("script/DefaultItemList.xml error");
                                            return FALSE;
                                        }
                                        AddSuperSaiyanHeadKeyName(eClassGenderType, (RwUInt32)atoi(szType), szKeyName);
                                    }
                                }
                            }
                        }

						pCharTypeNode->selectNodes(L"HEAD_BASE_TEXTURE", &pType_NodeList);
						if(pType_NodeList)
						{
							pType_NodeList->get_length(&typeCount);
							for(long n = 0; n < typeCount; ++n)
							{
								IXMLDOMNode* pTypeNode = NULL;
								pType_NodeList->get_item(n, &pTypeNode);
								pTypeNode->selectNodes(L"DATA", &pData_NodeList);
								if(pData_NodeList)
								{
									pData_NodeList->get_length(&dataCount);
									for(long t = 0; t < dataCount; t++)
									{
										IXMLDOMNode* pDataNode = NULL;
										pData_NodeList->get_item(t, &pDataNode);
										char szType[32] = {0,};
										char szColorType[32] = {0,};
										char szTexName[64] = {0,};
										
										if(!pDoc->GetTextWithAttributeName(pDataNode,"TYPE", szType, sizeof(szType)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}

										if(!pDoc->GetTextWithAttributeName(pDataNode,"COLOR_TYPE", szColorType, sizeof(szColorType)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}

										if(!pDoc->GetTextWithAttributeName(pDataNode,"TEX_NAME", szTexName, sizeof(szTexName)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}
										AddHeadBaseTextureName(eClassGenderType, (RwUInt32)atoi(szType), (RwUInt32)atoi(szColorType), szTexName); 
									}
								}
							}
						}


						pCharTypeNode->selectNodes(L"BODY_MESH", &pType_NodeList);
						if(pType_NodeList)
						{
							pType_NodeList->get_length(&typeCount);
							for(long n = 0; n < typeCount; ++n)
							{
								IXMLDOMNode* pTypeNode = NULL;
								pType_NodeList->get_item(n, &pTypeNode);
								pTypeNode->selectNodes(L"DATA", &pData_NodeList);
								if(pData_NodeList)
								{
									pData_NodeList->get_length(&dataCount);
									for(long t = 0; t < dataCount; t++)
									{
										IXMLDOMNode* pDataNode = NULL;
										pData_NodeList->get_item(t, &pDataNode);
										char szType[32] = {0,};
										char szEquip[64] = {0,};
										char szKeyName[64] = {0,};
										
										if(!pDoc->GetTextWithAttributeName(pDataNode,"TYPE", szType, sizeof(szType)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}
										
										if(!pDoc->GetTextWithAttributeName(pDataNode,"EQUIP", szEquip, sizeof(szType)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}

										if(!pDoc->GetTextWithAttributeName(pDataNode,"KEY_NAME", szKeyName, sizeof(szKeyName)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}
										
										if(_stricmp("EQUIP_JACKET", szEquip) == 0)
										{
											AddBodyKeyName(eClassGenderType, (RwUInt32)atoi(szType), ITEM_EQUIP_JACKET,	szKeyName);
										}
										else if(_stricmp("EQUIP_PANTS", szEquip) == 0)
										{
											AddBodyKeyName(eClassGenderType, (RwUInt32)atoi(szType),ITEM_EQUIP_PANTS,	szKeyName);
										}
										else if(_stricmp("EQUIP_BOOTS", szEquip) == 0)
										{
											AddBodyKeyName(eClassGenderType, (RwUInt32)atoi(szType),ITEM_EQUIP_BOOTS,	szKeyName);
										}
										else if(_stricmp("EQUIP_TWO_HAND", szEquip) == 0)
										{
											AddBodyKeyName(eClassGenderType, (RwUInt32)atoi(szType),ITEM_EQUIP_TWO_HAND,	szKeyName);
										}
										else
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}
									}
								}
							}
						}

						pCharTypeNode->selectNodes(L"SKIN_COLOR", &pType_NodeList);
						if(pType_NodeList)
						{
							pType_NodeList->get_length(&typeCount);
							for(long n = 0; n < typeCount; ++n)
							{
								IXMLDOMNode* pTypeNode = NULL;
								pType_NodeList->get_item(n, &pTypeNode);
								pTypeNode->selectNodes(L"DATA", &pData_NodeList);
								if(pData_NodeList)
								{
									pData_NodeList->get_length(&dataCount);
									for(long t = 0; t < dataCount; t++)
									{
										IXMLDOMNode* pDataNode = NULL;
										pData_NodeList->get_item(t, &pDataNode);
										char szType[32] = {0,};
										char szRedColor[32] = {0,};
										char szGreenColor[32] = {0,};
										char szBlueColor[32] = {0,};

										if(!pDoc->GetTextWithAttributeName(pDataNode,"TYPE", szType, sizeof(szType)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}
										if(!pDoc->GetTextWithAttributeName(pDataNode,"RED", szRedColor, sizeof(szRedColor)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}
										if(!pDoc->GetTextWithAttributeName(pDataNode,"GREEN", szGreenColor, sizeof(szGreenColor)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}
										if(!pDoc->GetTextWithAttributeName(pDataNode,"BLUE", szBlueColor, sizeof(szBlueColor)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}

										AddSkinColor(eClassGenderType, (RwUInt32)atoi(szType), (RwUInt8)atoi(szRedColor), (RwUInt8)atoi(szGreenColor), (RwUInt8)atoi(szBlueColor));
									}
								}
							}
						}

						pCharTypeNode->selectNodes(L"HEAD_COLOR", &pType_NodeList);
						if(pType_NodeList)
						{
							pType_NodeList->get_length(&typeCount);
							for(long n = 0; n < typeCount; ++n)
							{
								IXMLDOMNode* pTypeNode = NULL;
								pType_NodeList->get_item(n, &pTypeNode);
								pTypeNode->selectNodes(L"DATA", &pData_NodeList);
								if(pData_NodeList)
								{
									pData_NodeList->get_length(&dataCount);
									for(long t = 0; t < dataCount; t++)
									{
										IXMLDOMNode* pDataNode = NULL;
										pData_NodeList->get_item(t, &pDataNode);
										char szType[32] = {0,};
										char szRedColor[32] = {0,};
										char szGreenColor[32] = {0,};
										char szBlueColor[32] = {0,};

										if(!pDoc->GetTextWithAttributeName(pDataNode,"TYPE", szType, sizeof(szType)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}
										if(!pDoc->GetTextWithAttributeName(pDataNode,"RED", szRedColor, sizeof(szRedColor)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}
										if(!pDoc->GetTextWithAttributeName(pDataNode,"GREEN", szGreenColor, sizeof(szGreenColor)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}
										if(!pDoc->GetTextWithAttributeName(pDataNode,"BLUE", szBlueColor, sizeof(szBlueColor)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}

										AddHeadColor(eClassGenderType, (RwUInt32)atoi(szType), (RwUInt8)atoi(szRedColor), (RwUInt8)atoi(szGreenColor), (RwUInt8)atoi(szBlueColor));
									}
								}
							}
						}

						pCharTypeNode->selectNodes(L"SCOUTER_TRANSFORM", &pType_NodeList);
						if(pType_NodeList)
						{
							pType_NodeList->get_length(&typeCount);
							for(long n = 0; n < typeCount; ++n)
							{
								IXMLDOMNode* pTypeNode = NULL;
								pType_NodeList->get_item(n, &pTypeNode);
								pTypeNode->selectNodes(L"DATA", &pData_NodeList);
								if(pData_NodeList)
								{
									pData_NodeList->get_length(&dataCount);
									for(long t = 0; t < dataCount; t++)
									{
										IXMLDOMNode* pDataNode = NULL;
										pData_NodeList->get_item(t, &pDataNode);
										char szType[64] = {0,};
										char szSCL[3][32] = {0,};
										char szROT[32] = {0,};
										char szPOS[3][32] = {0,};

										if(!pDoc->GetTextWithAttributeName(pDataNode,"TYPE", szType, sizeof(szType)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}
										if(!pDoc->GetTextWithAttributeName(pDataNode,"CHAR_TYPE", szType, sizeof(szType)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}
										if(!pDoc->GetTextWithAttributeName(pDataNode,"SCL_X", szSCL[0], sizeof(szSCL[0])))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}
										if(!pDoc->GetTextWithAttributeName(pDataNode,"SCL_Y", szSCL[1], sizeof(szSCL[1])))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}
										if(!pDoc->GetTextWithAttributeName(pDataNode,"SCL_Z", szSCL[2], sizeof(szSCL[2])))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}
										if(!pDoc->GetTextWithAttributeName(pDataNode,"ROT_X", szROT, sizeof(szROT)))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}
										if(!pDoc->GetTextWithAttributeName(pDataNode,"POS_X", szPOS[0], sizeof(szPOS[0])))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}
										if(!pDoc->GetTextWithAttributeName(pDataNode,"POS_Y", szPOS[1], sizeof(szPOS[1])))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}
										if(!pDoc->GetTextWithAttributeName(pDataNode,"POS_Z", szPOS[2], sizeof(szPOS[2])))
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error");
											return FALSE;
										}

										// get a char type id
										eRACE_GENDER_CLASS_TYPE eRGCT = RACE_GENDER_CLASS_TYPE_INVALID;

										if(!_stricmp(szType, "HUMAN_ADULT_MALE_FIGHTER"))			eRGCT = HUMAN_ADULT_MALE_FIGHTER;
										else if(!_stricmp(szType, "HUMAN_ADULT_MALE_MYSTIC"))		eRGCT = HUMAN_ADULT_MALE_MYSTIC;
										else if(!_stricmp(szType, "HUMAN_ADULT_MALE_ENGINEER"))		eRGCT = HUMAN_ADULT_MALE_ENGINEER;
										else if(!_stricmp(szType, "HUMAN_CHILD_MALE_FIGHTER"))		eRGCT = HUMAN_CHILD_MALE_FIGHTER;
										else if(!_stricmp(szType, "HUMAN_CHILD_MALE_MYSTIC"))		eRGCT = HUMAN_CHILD_MALE_MYSTIC;
										else if(!_stricmp(szType, "HUMAN_CHILD_MALE_ENGINEER"))		eRGCT = HUMAN_CHILD_MALE_ENGINEER;
										else if(!_stricmp(szType, "HUMAN_ADULT_FEMALE_FIGHTER"))	eRGCT = HUMAN_ADULT_FEMALE_FIGHTER;
										else if(!_stricmp(szType, "HUMAN_ADULT_FEMALE_MYSTIC"))		eRGCT = HUMAN_ADULT_FEMALE_MYSTIC;
										else if(!_stricmp(szType, "HUMAN_ADULT_FEMALE_ENGINEER"))	eRGCT = HUMAN_ADULT_FEMALE_ENGINEER;
										else if(!_stricmp(szType, "HUMAN_CHILD_FEMALE_FIGHTER"))	eRGCT = HUMAN_CHILD_FEMALE_FIGHTER;
										else if(!_stricmp(szType, "HUMAN_CHILD_FEMALE_MYSTIC"))		eRGCT = HUMAN_CHILD_FEMALE_MYSTIC;
										else if(!_stricmp(szType, "HUMAN_CHILD_FEMALE_ENGINEER"))	eRGCT = HUMAN_CHILD_FEMALE_ENGINEER;
										else if(!_stricmp(szType, "NAMEK_ADULT_FIGHTER"))			eRGCT = NAMEK_ADULT_FIGHTER;
										else if(!_stricmp(szType, "NAMEK_ADULT_MYSTIC"))			eRGCT = NAMEK_ADULT_MYSTIC;
										else if(!_stricmp(szType, "NAMEK_CHILD_FIGHTER"))			eRGCT = NAMEK_CHILD_FIGHTER;
										else if(!_stricmp(szType, "NAMEK_CHILD_MYSTIC"))			eRGCT = NAMEK_CHILD_MYSTIC;
										else if(!_stricmp(szType, "MAJIN_ADULT_MALE_MIGHTY"))		eRGCT = MAJIN_ADULT_MALE_MIGHTY;
										else if(!_stricmp(szType, "MAJIN_ADULT_MALE_WONDER"))		eRGCT = MAJIN_ADULT_MALE_WONDER;
										else if(!_stricmp(szType, "MAJIN_CHILD_MALE_MIGHTY"))		eRGCT = MAJIN_CHILD_MALE_MIGHTY;
										else if(!_stricmp(szType, "MAJIN_CHILD_MALE_WONDER"))		eRGCT = MAJIN_CHILD_MALE_WONDER;
										else if(!_stricmp(szType, "MAJIN_ADULT_FEMALE_MIGHTY"))		eRGCT = MAJIN_ADULT_FEMALE_MIGHTY;
										else if(!_stricmp(szType, "MAJIN_ADULT_FEMALE_WONDER"))		eRGCT = MAJIN_ADULT_FEMALE_WONDER;
										else if(!_stricmp(szType, "MAJIN_CHILD_FEMALE_MIGHTY"))		eRGCT = MAJIN_CHILD_FEMALE_MIGHTY;
										else if(!_stricmp(szType, "MAJIN_CHILD_FEMALE_WONDER"))		eRGCT = MAJIN_CHILD_FEMALE_WONDER;
										else
										{
											NTL_ASSERTFAIL("script/DefaultItemList.xml error; <SCOUTER_TRANSFORM> node's got some wrong values");
											return FALSE;
										}

										AddScouterAttachInfo(	eRGCT,
																(RwReal)atof(szSCL[0]),
																(RwReal)atof(szSCL[1]),
																(RwReal)atof(szSCL[2]),
																(RwReal)atof(szROT),
																(RwReal)atof(szPOS[0]), 
																(RwReal)atof(szPOS[1]), 
																(RwReal)atof(szPOS[2]) );
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return TRUE;
}


