#include "precomp_ntlpresentation.h"
#include "NtlPLPropertyContainer.h"
#include "NtlXMLDoc.h"
#include "NtlPLProperty.h"
#include "NtlDebug.h"
#include "NtlPLObjectProperty.h"
#include "NtlPLGuiProperty.h"
#include "NtlPLCharacterProperty.h"
#include "NtlPLItemProperty.h"
#include "NtlPLResourcePack.h"

// by agebreak
#include "NtlEffectDefine.h"
#include "NtlResourceEffect.h"
#include "NtlEffectSplinePath.h"

// woody1019
#include "NtlPLWorldProp.h"
#include "NtlPLWaterProp.h"
#include "NtlPLSoundProp.h"
#include "NtlPLShoreLineProp.h"

// hodong
#include "NtlPLItemParser.h"
#include "NtlPLCharacterParser.h"

// charmzine
#include "NtlPLPlantProperty.h"
#include "NtlPLWeatherProperty.h"

#include <set>

CNtlPLPropertyContainer* CNtlPLPropertyContainer::m_pInstance = 0;

CNtlPLPropertyContainer::CNtlPLPropertyContainer()
{
	NTL_PRE(m_pInstance == 0);

	m_pInstance = this;
}


CNtlPLPropertyContainer::~CNtlPLPropertyContainer()
{
	m_pInstance = 0;
}

CNtlPLPropertyContainer* CNtlPLPropertyContainer::GetInstance(void)
{
	return m_pInstance;
}


RwBool CNtlPLPropertyContainer::Create(void)
{
	return TRUE;
}

void CNtlPLPropertyContainer::Destroy(void)
{
	NTL_FUNCTION("CNtlPLPropertyContainer::Destroy");

	CNtlPLProperty *pProp;
	MapProp::iterator it;

	for(it = m_mapSky.begin(); it != m_mapSky.end(); it++)
	{
		pProp = (*it).second; 
		NTL_DELETE(pProp);
	}

	m_mapSky.clear(); 

	// woody1019
	for(it = m_mapNtlWorld.begin(); it != m_mapNtlWorld.end(); it++)
	{
		pProp = (*it).second; 
		NTL_DELETE(pProp);
	}

	m_mapNtlWorld.clear(); 

	for(it = m_mapWater.begin(); it != m_mapWater.end(); it++)
	{
		pProp = (*it).second; 
		NTL_DELETE(pProp);
	}

	m_mapWater.clear(); 

	for(it = m_mapShoreLine.begin(); it != m_mapShoreLine.end(); it++)
	{
		pProp = (*it).second; 
		NTL_DELETE(pProp);
	}

	m_mapShoreLine.clear(); 

	for(it = m_mapObject.begin(); it != m_mapObject.end(); it++)
	{
		pProp = (*it).second; 
		NTL_DELETE(pProp);	
	}

	m_mapObject.clear();	

	for(it = m_mapSound.begin(); it != m_mapSound.end(); it++)
	{
		pProp = (*it).second; 
		NTL_DELETE(pProp);	
	}

	m_mapSound.clear();	

	for(it = m_mapItem.begin(); it != m_mapItem.end(); it++)
	{
		pProp = (*it).second; 
		NTL_DELETE(pProp);
	}

	m_mapItem.clear();


	for(it = m_mapCharacter.begin(); it != m_mapCharacter.end(); it++)
	{
		pProp = (*it).second; 
		NTL_DELETE(pProp);
	}

	m_mapCharacter.clear();

	for(it = m_mapEffect.begin(); it != m_mapEffect.end(); it++)
	{
		pProp = (*it).second; 
		NTL_DELETE(pProp);
	}

	m_mapEffect.clear();

	// by agebreak
	for(it = m_mapSpline.begin(); it != m_mapSpline.end(); it++)
	{
		pProp = (*it).second; 
		NTL_DELETE(pProp);
	}
	m_mapSpline.clear();

	for(it = m_mapGui.begin(); it != m_mapGui.end(); it++)
	{
		pProp = (*it).second; 
		NTL_DELETE(pProp);
	}

	m_mapGui.clear();

	for(it = m_mapPlant.begin(); it != m_mapPlant.end(); it++)
	{
		pProp = (*it).second; 
		NTL_DELETE(pProp);
	}

	m_mapPlant.clear();

	for(it = m_mapWeather.begin(); it != m_mapWeather.end(); it++)
	{
		pProp = (*it).second; 
		NTL_DELETE(pProp);
	}

	m_mapWeather.clear();

	NTL_RETURNVOID();
}

RwBool CNtlPLPropertyContainer::AddProp(ENtlPLEntityType eType, CNtlPLProperty *pProp)
{
	NTL_FUNCTION("CNtlPLPropertyContainer::AddProp");

	NTL_PRE(pProp);

	const RwChar *pKey = pProp->GetName(); 
	//DBO_WARNING_MESSAGE("add prop: " << pKey);
	//	RwUInt32 uiId = pProp->GetId();

	switch(eType)
	{
		//woody1019
	case PLENTITY_WORLD:
		{
#ifdef _DEBUG
			MapProp::iterator it = m_mapNtlWorld.find(pKey);
			DBO_TRACE((it == m_mapNtlWorld.end()), "prop defined.(" << pKey << ")");
			NTL_DELETE(((CNtlPLProperty*)m_mapNtlWorld[pKey]));
#endif
			m_mapNtlWorld[pKey] = pProp;
			break;
		}

	case PLENTITY_WATER:
		{
#ifdef _DEBUG
			MapProp::iterator it = m_mapWater.find(pKey);
			DBO_TRACE((it == m_mapWater.end()), "prop defined.(" << pKey << ")");
			NTL_DELETE(((CNtlPLProperty*)m_mapWater[pKey]));
#endif
			m_mapWater[pKey] = pProp;
			break;
		}

	case PLENTITY_SHORELINE:
		{
#ifdef _DEBUG
			MapProp::iterator it = m_mapShoreLine.find(pKey);
			DBO_TRACE((it == m_mapShoreLine.end()), "prop defined.(" << pKey << ")");
			NTL_DELETE(((CNtlPLProperty*)m_mapShoreLine[pKey]));
#endif
			m_mapShoreLine[pKey] = pProp;
			break;
		}

	case PLENTITY_SKY:
		{
#ifdef _DEBUG
			MapProp::iterator it = m_mapSky.find(pKey);
			DBO_TRACE((it == m_mapSky.end()), "prop defined.(" << pKey << ")");
			NTL_DELETE(((CNtlPLProperty*)m_mapSky[pKey]));
#endif
			m_mapSky[pKey] = pProp;
			break;
		}

	case PLENTITY_OBJECT:
		{
#ifdef _DEBUG
			MapProp::iterator it = m_mapObject.find(pKey);
			DBO_TRACE((it == m_mapObject.end()), "prop defined.(" << pKey << ")");
			NTL_DELETE(((CNtlPLProperty*)m_mapObject[pKey]));
#endif
			m_mapObject[pKey] = pProp; 
			break;
		}

	case PLENTITY_SOUND:
		{
#ifdef _DEBUG
			MapProp::iterator it = m_mapSound.find(pKey);
			DBO_TRACE((it == m_mapSound.end()), "prop defined.(" << pKey << ")");
			NTL_DELETE(((CNtlPLProperty*)m_mapSound[pKey]));
#endif
			m_mapSound[pKey] = pProp; 
			break;
		}

	case PLENTITY_CHARACTER:
		{
#ifdef _DEBUG
			MapProp::iterator it = m_mapCharacter.find(pKey);
			DBO_TRACE((it == m_mapCharacter.end()), "prop defined.(" << pKey << ")");
			NTL_DELETE(((CNtlPLProperty*)m_mapCharacter[pKey]));
#endif
			m_mapCharacter[pKey] = pProp; 
			break;
		}

	case PLENTITY_ITEM:
		{
#ifdef _DEBUG
			MapProp::iterator it = m_mapItem.find(pKey);
			DBO_TRACE((it == m_mapItem.end()), "prop defined.(" << pKey << ")");
			NTL_DELETE(((CNtlPLProperty*)m_mapItem[pKey]));
#endif
			m_mapItem[pKey] = pProp;
			break;
		}

	case PLENTITY_EFFECT:
		{
#ifdef _DEBUG
			MapProp::iterator it = m_mapEffect.find(pKey);
			DBO_TRACE((it == m_mapEffect.end()), "prop defined.(" << pKey << ")");			
#endif
			m_mapEffect[pKey] = pProp; 
			break;
		}
		// by agebreak
	case PLENTITY_SPLINE:
		{
#ifdef _DEBUG
			MapProp::iterator it = m_mapSpline.find(pKey);
			DBO_TRACE((it == m_mapSpline.end()), "prop defined.(" << pKey << ")");
			NTL_DELETE(((CNtlPLProperty*)m_mapNtlWorld[pKey]));
#endif
			m_mapSpline[pKey] = pProp; 
			break;
		}
	case PLENTITY_GUI:
		{
#ifdef _DEBUG
			MapProp::iterator it = m_mapGui.find(pKey);
			DBO_TRACE((it == m_mapGui.end()), "prop defined.(" << pKey << ")");
			NTL_DELETE(((CNtlPLProperty*)m_mapGui[pKey]));
#endif
			m_mapGui[pKey] = pProp;
			break;
		}
	case PLENTITY_PLANT:
		{
#ifdef _DEBUG
			MapProp::iterator it = m_mapPlant.find(pKey);
			DBO_TRACE((it == m_mapPlant.end()), "prop defined.(" << pKey << ")");
			NTL_DELETE(((CNtlPLProperty*)m_mapPlant[pKey]));
#endif
			m_mapPlant[pKey] = pProp;
			break;
		}
	case PLENTITY_WEATHER:
		{
#ifdef _DEBUG
			MapProp::iterator it = m_mapWeather.find(pKey);
			DBO_TRACE((it == m_mapWeather.end()), "prop defined.(" << pKey << ")");
			NTL_DELETE(((CNtlPLProperty*)m_mapWeather[pKey]));
#endif
			m_mapWeather[pKey] = pProp;
			break;
		}
	default:
		DBO_FAIL("Not defined entity type");
		break;
	}

	NTL_RETURN(TRUE);
}

RwBool CNtlPLPropertyContainer::Load()
{
	NTL_FUNCTION("CNtlPLPropertyContainer::Load");

	if(!LoadNtlWorld(""))
	{
		NTL_RETURN(FALSE);
	}

	if(!LoadWater(".\\script\\waterproperty.xml"))
	{
		NTL_RETURN(FALSE);
	}

	if(!LoadShoreLine(".\\script\\ShoreLineProp.xml"))
	{
		NTL_RETURN(FALSE);
	}

	// BY DANEOS:
	// DISABLED SO WE LOAD ( AT BOTTOM ) PROPERTY FROM .rdf FILE. Once we managed to get these list fully working, we can disable this and remove loading from rdf.
	/*
	if(!LoadObject(".\\devdata\\object\\ObjectPropertyList.XML"))
	{
		NTL_RETURN(FALSE);
	}
	if (!LoadCharacter(".\\devdata\\character\\characterpropertyList.xml"))
	{
		NTL_RETURN(FALSE);
	}
	if (!LoadCharacter(".\\devdata\\character\\mobpropertyList.xml"))
	{
		NTL_RETURN(FALSE);
	}
	if (!LoadCharacter(".\\devdata\\character\\npcpropertyList.xml"))
	{
		NTL_RETURN(FALSE);
	}
	if (!LoadCharacter(".\\devdata\\character\\VehiclePropertyList.xml"))
	{
		NTL_RETURN(FALSE);
	}
	if (!LoadItem(".\\devdata\\item\\itempropertylist.xml"))
	{
		NTL_RETURN(FALSE);
	}
	*/

	if(!LoadSound(".\\script\\Environment.tbl"))
	{
		NTL_RETURN(FALSE);
	}

	if (!LoadEffect(".\\Effect\\VisualEffect_01.eff"))
	{
		NTL_RETURN(FALSE);
	}
	if(!LoadEffect(".\\Effect\\VisualEffect_02.eff"))
	{
		NTL_RETURN(FALSE);
	}

	if(!LoadGui(".\\script\\guiproperty.xml"))
	{
		NTL_RETURN(FALSE);
	}

	if(!LoadPlant(".\\script\\PlantProperty.xml"))
	{
		NTL_RETURN(FALSE);
	}

	if(!LoadWeather(".\\script\\WeatherProperty.xml", 0))
	{
		NTL_RETURN(FALSE);
	}

	if(!LoadWeather(".\\script\\WeatherProperty_LV1.xml", 1))
	{
		NTL_RETURN(FALSE);
	}

	if(!LoadWeather(".\\script\\WeatherProperty_LV2.xml", 2))
	{
		NTL_RETURN(FALSE);
	}

	if(!LoadSoundField(".\\script\\BgmSoundProperty.xml", "FIELD_SOUND_BGM"))
	{
		NTL_RETURN(FALSE);
	}

	if(!LoadSoundField(".\\script\\EnvSoundProperty.xml", "FIELD_SOUND_ENV"))
	{
		NTL_RETURN(FALSE);
	}

	if(!LoadSoundField(".\\script\\ShareSoundProperty.xml", "FIELD_SOUND_SHARE"))
	{
		NTL_RETURN(FALSE);
	}


	CNtlFileSerializer nsl(1024 * 1024, 1024 * 1024);

	if (GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_PROPERTY)  // Pack 사용시    
	{
		void* pData = NULL;
		RwInt32 iSize = 0;

		//	Character
		GetNtlResourcePackManager()->LoadProperty(".\\property\\characterproperty.rdf", &pData, &iSize);
		DBO_ASSERT(iSize > 0, __FUNCTION__ << "Character Property Load Fail");
		if (iSize > 0)
		{
			nsl.In(pData, iSize);
			LoadCharacterSerialize(nsl);
		}

		NTL_ARRAY_DELETE(pData);

		// Object
		GetNtlResourcePackManager()->LoadProperty(".\\property\\objectproperty.rdf", &pData, &iSize);
		DBO_ASSERT(iSize > 0, __FUNCTION__ << "Object Property Load Fail");
		if (iSize > 0)
		{
			nsl.In(pData, iSize);
			LoadObjectSerialize(nsl);
		}

		NTL_ARRAY_DELETE(pData);

		// Item
		GetNtlResourcePackManager()->LoadProperty(".\\property\\itemproperty.rdf", &pData, &iSize);
		DBO_ASSERT(iSize > 0, __FUNCTION__ << "Item Property Load Fail");
		if (iSize > 0)
		{
			nsl.In(pData, iSize);
			LoadItemSerialize(nsl);
		}

		NTL_ARRAY_DELETE(pData);
	}
	else
	{
		RwBool bResult = TRUE;

		// load from xml 
	}


	NTL_RETURN(TRUE);
}

// woody1019
RwBool CNtlPLPropertyContainer::LoadShoreLine(const RwChar *pFileName)
{
	NTL_FUNCTION("CNtlPLPropertyContainer::LoadShoreLine");

	CNtlPLShoreLineProp *pProperty = NTL_NEW CNtlPLShoreLineProp;
	pProperty->SetName(NTL_PLEN_SHORELINE);

	CNtlXMLDoc doc;
	doc.Create();

	if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_SCRIPT)
	{		
		BYTE*	pPackBuffer	= NULL;
		RwInt32	iPackSize	= 0;

		GetNtlResourcePackManager()->LoadScript(pFileName, (void**)&pPackBuffer, &iPackSize);
		if (pPackBuffer)
		{			
			BYTE* pTempBuffer = NTL_NEW BYTE [iPackSize + 1];
			memcpy(pTempBuffer, pPackBuffer, iPackSize);
			pTempBuffer[iPackSize] = '\0';

			if(!doc.LoadXML((char*)pTempBuffer))
			{			
				doc.Destroy();
				NTL_ARRAY_DELETE(pTempBuffer);
				NTL_ARRAY_DELETE(pPackBuffer);
				NTL_RETURN(FALSE);
			}
			NTL_ARRAY_DELETE(pTempBuffer);
			NTL_ARRAY_DELETE(pPackBuffer);
		}
	}
	else
	{
		if(doc.Load((char*)pFileName) == false)
		{
			doc.Destroy();
			NTL_RETURN(FALSE);
		}
	}

	char				chBuffer[NTL_MAX_DIR_PATH]	= {0,};
	IXMLDOMNode*		pNode						= NULL;
	IXMLDOMNodeList*	pList0						= doc.SelectNodeList((char*)"/shoreline_property/uv_single_element/element");
	IXMLDOMNodeList*	pList1						= doc.SelectNodeList((char*)"/shoreline_property/uv_dual_element/element");
	IXMLDOMNodeList*	pList2						= doc.SelectNodeList((char*)"/shoreline_property/sequence_element/element");

	long  lNum;
	pList0->get_length(&lNum);

	for(RwInt32 i = 0; i < lNum ; ++i)
	{
		pList0->get_item(i,&pNode);

		if(!pProperty->LoadUVSingleShoreLineContainer(&doc, pNode))
		{
			NTL_RETURN(FALSE);
		}

		if(pNode)
		{
			pNode->Release(); 
			pNode = NULL;
		}
	}

	pList1->get_length(&lNum);

	for(RwInt32 i = 0; i < lNum ; ++i)
	{
		pList1->get_item(i,&pNode);

		if(!pProperty->LoadUVDualShoreLineContainer(&doc, pNode))
		{
			NTL_RETURN(FALSE);
		}

		if(pNode)
		{
			pNode->Release(); 
			pNode = NULL;
		}
	}

	pList2->get_length(&lNum);

	for(RwInt32 i = 0; i < lNum ; ++i)
	{
		pList2->get_item(i,&pNode);

		if(!pProperty->LoadSequenceShoreLineContainer(&doc, pNode))
		{
			NTL_RETURN(FALSE);
		}

		if(pNode)
		{
			pNode->Release(); 
			pNode = NULL;
		}
	}

	// object property add
	AddProp(PLENTITY_SHORELINE, pProperty);

	if(pList0)
	{
		pList0->Release(); 
		pList0 = NULL;
	}

	if(pList1)
	{
		pList1->Release(); 
		pList1= NULL;
	}

	if(pList2)
	{
		pList2->Release(); 
		pList2 = NULL;
	}

	NTL_RETURN(TRUE);
}

RwBool CNtlPLPropertyContainer::LoadNtlWorld(const RwChar *pFileName)
{
	NTL_FUNCTION("CNtlPLPropertyContainer::LoadNtlWorld");

	CNtlPLWorldProp *pProperty = NTL_NEW CNtlPLWorldProp;
	pProperty->SetName("NtlWorld");
	pProperty->Load(NULL, NULL); 

	NTL_RETURN(AddProp(PLENTITY_WORLD, pProperty));
}

RwBool CNtlPLPropertyContainer::LoadSound(const RwChar *pFileName)
{	
	NTL_FUNCTION("CNtlPLPropertyContainer::LoadSound");

	CNtlPLSoundProp *pProperty = NTL_NEW CNtlPLSoundProp;
	pProperty->SetName(NTL_PLEN_SOUND);
	pProperty->Load(NULL, NULL);

	AddProp(PLENTITY_SOUND, pProperty);

	NTL_RETURN(TRUE);
}

RwBool CNtlPLPropertyContainer::LoadSoundField(const RwChar *pFileName, const RwChar* pName)
{	
	NTL_FUNCTION("CNtlPLPropertyContainer::LoadSoundField");

	CNtlPLSoundFieldProp *pProperty = NULL;

	pProperty = NTL_NEW CNtlPLSoundFieldProp;
	pProperty->SetName(pName);

	CNtlXMLDoc doc;
	doc.Create();

	if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_SCRIPT)
	{		
		BYTE*	pPackBuffer	= NULL;
		RwInt32	iPackSize	= 0;

		GetNtlResourcePackManager()->LoadScript(pFileName, (void**)&pPackBuffer, &iPackSize);
		if (pPackBuffer)
		{			
			BYTE* pTempBuffer = NTL_NEW BYTE [iPackSize + 1];
			memcpy(pTempBuffer, pPackBuffer, iPackSize);
			pTempBuffer[iPackSize] = '\0';

			if(!doc.LoadXML((char*)pTempBuffer))
			{			
				doc.Destroy();
				NTL_ARRAY_DELETE(pTempBuffer);
				NTL_ARRAY_DELETE(pPackBuffer);
				NTL_RETURN(FALSE);
			}
			NTL_ARRAY_DELETE(pTempBuffer);
			NTL_ARRAY_DELETE(pPackBuffer);
		}
	}
	else
	{
		if(doc.Load((char*)pFileName) == false)
		{
			doc.Destroy();
			NTL_RETURN(FALSE);
		}
	}

	IXMLDOMNode*		pNode = NULL;
	IXMLDOMNodeList*	pList = doc.SelectNodeList((char*)"/sound_property/sound");

	long  lNum;
	pList->get_length(&lNum);

	for(RwInt32 i = 0; i < lNum ; ++i)
	{
		pList->get_item(i,&pNode);

		if(!pProperty->Load(&doc, pNode))
		{
			NTL_RETURN(FALSE);
		}

		if(pNode)
		{
			pNode->Release(); 
			pNode = NULL;
		}
	}

	// object property add
	AddProp(PLENTITY_SOUND, pProperty);

	if(pList)
	{
		pList->Release(); 
		pList = NULL;
	}

	NTL_RETURN(TRUE);
}

RwBool CNtlPLPropertyContainer::LoadWater(const RwChar *pFileName)
{
	NTL_FUNCTION("CNtlPLPropertyContainer::LoadWater");

	CNtlPLWaterProp *pProperty = NTL_NEW CNtlPLWaterProp;
	pProperty->SetName("NtlWorldWater");

	CNtlXMLDoc doc;
	doc.Create();

	if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_SCRIPT)
	{		
		BYTE*	pPackBuffer	= NULL;
		RwInt32	iPackSize	= 0;

		GetNtlResourcePackManager()->LoadScript(pFileName, (void**)&pPackBuffer, &iPackSize);
		if (pPackBuffer)
		{			
			BYTE* pTempBuffer = NTL_NEW BYTE [iPackSize + 1];
			memcpy(pTempBuffer, pPackBuffer, iPackSize);
			pTempBuffer[iPackSize] = '\0';

			if(!doc.LoadXML((char*)pTempBuffer))
			{			
				doc.Destroy();
				NTL_ARRAY_DELETE(pTempBuffer);
				NTL_ARRAY_DELETE(pPackBuffer);
				NTL_RETURN(FALSE);
			}
			NTL_ARRAY_DELETE(pTempBuffer);
			NTL_ARRAY_DELETE(pPackBuffer);
		}
	}
	else
	{
		if(doc.Load((char*)pFileName) == false)
		{
			doc.Destroy();
			NTL_RETURN(FALSE);
		}
	}

	char chBuffer[NTL_MAX_DIR_PATH];

	IXMLDOMNode* pNode = doc.SelectSingleNode((char*)"/object_property/header");

	if(!doc.GetTextWithAttributeName(pNode, "ver", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}
	else
	{
		pProperty->SetVer(static_cast<RwReal>(::atof(chBuffer)));
	}

	if(!doc.GetTextWithAttributeName(pNode, "texpath", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}
	else
	{
		pProperty->SetTexPath(chBuffer);
	}

	if(pNode)
	{
		pNode->Release(); 
		pNode = NULL;
	}

	// object element list를 얻어온다.
	IXMLDOMNodeList *pList = doc.SelectNodeList((char*)"/object_property/object_element/element");

	long  lNum;
	pList->get_length(&lNum);

	for(RwInt32 i = 0; i < lNum ; ++i)
	{
		pList->get_item(i,&pNode);

		if(!pProperty->Load(&doc, pNode))
		{
			NTL_RETURN(FALSE);
		}

		if(pNode)
		{
			pNode->Release(); 
			pNode = NULL;
		}
	}

	// object property add
	AddProp(PLENTITY_WATER, pProperty);

	if(pList)
	{
		pList->Release(); 
		pList = NULL;
	}

	NTL_RETURN(TRUE);
}

RwBool CNtlPLPropertyContainer::LoadObject(const RwChar *pFileName)
{
	NTL_FUNCTION("CNtlPLPropertyContainer::LoadObject");

	CNtlXMLDoc doc;
	doc.Create();

	// xml doc load
	if(doc.Load( (char*)pFileName ) == false)
	{
		NTL_RETURN(FALSE);
	}

	char chBuffer[1024] = {0,};

	// object property header를 얻어온다.
	IXMLDOMNode* pNode = doc.SelectSingleNode((char*)"/OBJECT_PROPERTY/HEADER");

	if(!doc.GetTextWithAttributeName(pNode, "VER", chBuffer, 1024))
	{
		NTL_RETURN(FALSE);
	}

	//CNtlPLObjectProperty::m_strVer = chBuffer; // DISABLE BY DANEOS

	if(!doc.GetTextWithAttributeName(pNode, "OBJECTDATAPATH", chBuffer, 1024))
	{
		NTL_RETURN(FALSE);
	}
	CNtlPLObjectProperty::m_strMeshPath = chBuffer;


	if(pNode)
	{
		pNode->Release(); 
		pNode = NULL;
	}

	// object element list를 얻어온다.
	IXMLDOMNodeList *pList = doc.SelectNodeList((char*)"/OBJECT_PROPERTY/OBJECT_ELEMENT/ELEMENT");

	long  lNum;
	pList->get_length(&lNum);

	for(RwInt32 i = 0; i < lNum ; ++i)
	{
		pList->get_item(i,&pNode);

		// object property load
		CNtlPLObjectProperty *pProperty = NTL_NEW CNtlPLObjectProperty;
		if(!pProperty->Load(&doc, pNode))
		{
			NTL_RETURN(FALSE);
		}

		// object property add
		AddProp(PLENTITY_OBJECT, pProperty);

		if(pNode)
		{
			pNode->Release(); 
			pNode = NULL;
		}
	}

	if(pList)
	{
		pList->Release(); 
		pList = NULL;
	}

	NTL_RETURN(TRUE);
}

RwBool CNtlPLPropertyContainer::LoadItem(const RwChar *pFileName)
{
	NTL_FUNCTION("CNtlPLPropertyContainer::LoadItem");

	CNtlXMLDoc doc;
	doc.Create();

	// xml doc load
	if(doc.Load( (char*)pFileName ) == false)
	{
		NTL_RETURN(FALSE);
	}

	char chBuffer[1024] = {0,};

	// object property header를 얻어온다.
	IXMLDOMNode* pNode = doc.SelectSingleNode((char*)"/ITEM_PROPERTY/HEADER");

	if(!doc.GetTextWithAttributeName(pNode, "VER", chBuffer, 1024))
	{
		NTL_RETURN(FALSE);
	}
	//CNtlPLItemProperty::m_strVer = chBuffer;

	if(!doc.GetTextWithAttributeName(pNode, "ITEMDATAPATH", chBuffer, 1024))
	{
		NTL_RETURN(FALSE);
	}
	CNtlPLItemProperty::m_strItemDataPath = chBuffer;

	if(pNode)
	{
		pNode->Release(); 
		pNode = NULL;
	}

	// object element list를 얻어온다.
	IXMLDOMNodeList *pList = doc.SelectNodeList((char*)"/ITEM_PROPERTY/ITEM_ELEMENT/ELEMENT");

	long  lNum;
	pList->get_length(&lNum);

	for(RwInt32 i = 0; i < lNum ; ++i)
	{
		pList->get_item(i,&pNode);

		// object property load
		CNtlPLItemProperty *pProperty = NTL_NEW CNtlPLItemProperty;
		if(!pProperty->Load(&doc, pNode))
		{
			OutputDebugString(CNtlPLItemParser::GetInstance().GetLastError().c_str());
			NTL_RETURN(FALSE);
		}
		// object property add
		AddProp(PLENTITY_ITEM, pProperty);

		if(pNode)
		{
			pNode->Release(); 
			pNode = NULL;
		}
	}

	if(pList)
	{
		pList->Release(); 
		pList = NULL;
	}

	//Add Face, Head Item
	/*
	CNtlPLItemProperty *pProperty = NTL_NEW CNtlPLItemProperty;
	pProperty->SetName("HUMAN_AD_HEAD_1");
	pProperty->SetBaseTextureName(
	AddProp(PLENTITY_ITEM, pProperty);
	*/

	NTL_RETURN(TRUE);
}

RwBool CNtlPLPropertyContainer::LoadCharacter(const RwChar *pFileName)
{
	NTL_FUNCTION("CNtlPLPropertyContainer::LoadCharacter");
	if(pFileName == "")
	{
		CNtlPLCharacterProperty	*pProperty = NTL_NEW CNtlPLCharacterProperty;
		pProperty->SetName("toolcharacter"); 
		AddProp(PLENTITY_CHARACTER, pProperty);
	}
	else
	{
		CNtlXMLDoc doc;
		doc.Create();

		// xml doc load
		if(doc.Load( (char*)pFileName ) == false)
		{
			NTL_RETURN(FALSE);
		}

		char chBuffer[1024] = {0,};

		// object property header를 얻어온다.
		IXMLDOMNode* pNode = doc.SelectSingleNode((char*)"/CHARACTER_PROPERTY/HEADER");

		if(!doc.GetTextWithAttributeName(pNode, "VER", chBuffer, 1024))
		{
			NTL_RETURN(FALSE);
		}

		//CNtlPLCharacterProperty::m_strVer = chBuffer; // DISABLE BY DANEOS

		if(!doc.GetTextWithAttributeName(pNode, "CHARACTERDATAPATH", chBuffer, 1024))
		{
			NTL_RETURN(FALSE);
		}
		CNtlPLCharacterProperty::m_strCharacterDataPath = chBuffer;

		if(pNode)
		{
			pNode->Release(); 
			pNode = NULL;
		}

		// character element list 얻어 온다.
		IXMLDOMNodeList *pList = doc.SelectNodeList((char*)"/CHARACTER_PROPERTY/CHARACTER_ELEMENT/ELEMENT");

		long  lNum;
		pList->get_length(&lNum);

		for(RwInt32 i = 0; i < lNum ; ++i)
		{
			pList->get_item(i,&pNode);

			// character property load
			CNtlPLCharacterProperty *pProperty = NTL_NEW CNtlPLCharacterProperty;
			if(!pProperty->Load(&doc, pNode))
			{
				NTL_RETURN(FALSE);
			}
			// object property add
			AddProp(PLENTITY_CHARACTER, pProperty);

			if(pNode)
			{
				pNode->Release(); 
				pNode = NULL;
			}
		}

		if(pList)
		{
			pList->Release(); 
			pList = NULL;
		}
	}
	NTL_RETURN(TRUE);
}

// by agebreak
RwBool CNtlPLPropertyContainer::LoadEffect(const RwChar *pFileName)
{
	FILE* pFile = NULL;

	SPackResFileData sPackFileData;
	RwBool bPack = GetNtlResourcePackManager()->LoadObject(pFileName, sPackFileData);
	if(bPack)
	{
		pFile = fopen(sPackFileData.strPackFileName.c_str(), "rb");
		if(pFile)
			fseek(pFile, sPackFileData.uiOffset, SEEK_SET);
	}

	if(pFile == NULL)
		pFile = fopen(pFileName, "rb");

	if (pFile == NULL)
	{
		DBO_WARNING_MESSAGE("couldnt open file: " << pFileName);
		return FALSE;
	}

	RwChar strFileHeader[NTL_MAX_DIR_PATH];
	memset(strFileHeader, 0, NTL_MAX_DIR_PATH);
	fread(strFileHeader,				strlen(EFFECT_FILE_HEADER),					1,			pFile);

	if (_stricmp(strFileHeader, EFFECT_FILE_HEADER) != 0)
	{
		fclose(pFile);
		return FALSE;
	}

	fread(&CNtlResourceEffect::m_nVersion,			sizeof(RwUInt32),							1,			pFile);
	//DBO_WARNING_MESSAGE("VERSION: " << CNtlResourceEffect::m_nVersion);
	if (CNtlResourceEffect::m_nVersion < 0x00000002)
	{
		fclose(pFile);
		return FALSE;
	}

	// ============================================================================
	// Spline Property
	// ============================================================================
	RwInt32 nSplineCount;
	fread(&nSplineCount,				sizeof(RwInt32),							1,			pFile);
	//DBO_WARNING_MESSAGE("nSplineCount: " << nSplineCount);
	if (nSplineCount > 0)
	{
		RwChar strName[NTL_MAX_DIR_PATH] = {0,};
		for (RwInt32 i = 0; i < nSplineCount; ++ i)
		{
			RwInt32 length;
			fread(&length, sizeof(RwInt32), 1, pFile);

			memset(strName, 0, NTL_MAX_DIR_PATH);
			fread(strName, sizeof(RwChar) * length, 1, pFile);

			CNtlEffectSplinePath* pEffectSplinePath = NTL_NEW CNtlEffectSplinePath(strName);
			pEffectSplinePath->Load(pFile);

			// If you merge, the spline with the same name has the same content. (precondition)
			// Therefore, it is not added in duplicate.
			if (GetProperty(PLENTITY_SPLINE, strName) != NULL)
			{
				continue;				
			}
			AddProp(PLENTITY_SPLINE, pEffectSplinePath);
		}
	}

	// ============================================================================
	// Effect Property
	// ============================================================================
	RwInt32 nEffectCount;
	fread(&nEffectCount,				sizeof(RwInt32),							1,			pFile);

	if (nEffectCount > 0)
	{
		RwChar strName[NTL_MAX_DIR_PATH] = {0,};                		

		for (RwInt32 i = 0; i < nEffectCount; ++i)
		{
			RwInt32 length;
			fread(&length,					sizeof(RwInt32),							1,			pFile);

			memset(strName, 0, NTL_MAX_DIR_PATH);
			fread(strName,					sizeof(RwChar) * length,					1,		pFile);
		//	DBO_WARNING_MESSAGE("nEffectCount: " << nEffectCount << " load effect-name: " << strName << " length: " << length)
			// Consider the case of merge and think about the case of the same name.
			// If a merge clause is used, effects with the same name do not merge.

			CNtlResourceEffect* pResourceEffect = NTL_NEW CNtlResourceEffect(strName);			
			if(pResourceEffect->Load(pFile))
			{
				// Check the effect name without distinction
                if(m_mapEffect.find(strName) != m_mapEffect.end())
                {
				//	DBO_WARNING_MESSAGE("effect already exist:" << strName)
                    NTL_DELETE(pResourceEffect);
                    continue;
                }

				AddProp(PLENTITY_EFFECT, pResourceEffect);                
			}
			else
			{
				NTL_DELETE(pResourceEffect);
			}
		}
	}

	fclose(pFile);
	return TRUE;
}

RwBool CNtlPLPropertyContainer::LoadGui(const RwChar *pFileName)
{
	NTL_FUNCTION("CNtlPLPropertyContainer::LoadGui");

	CNtlXMLDoc doc;
	doc.Create();

	// xml doc load
	if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_SCRIPT)
	{
		BYTE* pPackBuffer = NULL; 
		RwInt32	iPackSize	= 0;

		GetNtlResourcePackManager()->LoadScript(pFileName, (void**)&pPackBuffer, &iPackSize);
		if (pPackBuffer)
		{			
			BYTE* pTempBuffer = NTL_NEW BYTE [iPackSize + 1];
			memcpy(pTempBuffer, pPackBuffer, iPackSize);
			pTempBuffer[iPackSize] = '\0';

			if(!doc.LoadXML((char*)pTempBuffer))
			{			
				doc.Destroy();
				NTL_ARRAY_DELETE(pTempBuffer);
				NTL_ARRAY_DELETE(pPackBuffer);
				NTL_RETURN(FALSE);
			}

			NTL_ARRAY_DELETE(pTempBuffer);
			NTL_ARRAY_DELETE(pPackBuffer);
		}
	}
	else
	{
		if(doc.Load( (char*)pFileName ) == false)
		{
			NTL_RETURN(FALSE);
		}
	}

	char chBuffer[1024];

	// object property header를 얻어온다.
	IXMLDOMNode* pNode = doc.SelectSingleNode((char*)"/gui_property/header");
	if(!doc.GetTextWithAttributeName(pNode, "ver", chBuffer, 1024))
	{
		NTL_RETURN(FALSE);
	}

	//CNtlPLGuiProperty::m_strVer = chBuffer;
	CNtlPLGuiProperty::m_strVer = atoi(chBuffer);

	if(!doc.GetTextWithAttributeName(pNode, "scriptpath", chBuffer, 1024))
	{
		NTL_RETURN(FALSE);
	}

	CNtlPLGuiProperty::m_strScriptPath = chBuffer;

	if(!doc.GetTextWithAttributeName(pNode, "texpath", chBuffer, 1024))
	{
		NTL_RETURN(FALSE);
	}

	CNtlPLGuiProperty::m_strTexPath = chBuffer;

	if(pNode)
	{
		pNode->Release(); 
		pNode = NULL;
	}

	// object element list를 얻어온다.
	IXMLDOMNodeList *pList = doc.SelectNodeList((char*)"/gui_property/gui_element/element");

	long  lNum;
	pList->get_length(&lNum);

	for(RwInt32 i = 0; i < lNum ; ++i)
	{
		pList->get_item(i,&pNode);

		// object property load
		CNtlPLGuiProperty *pProperty = NTL_NEW CNtlPLGuiProperty;
		if(!pProperty->Load(&doc, pNode))
		{
			NTL_RETURN(FALSE);
		}

		// object property add
		AddProp(PLENTITY_GUI, pProperty);

		if(pNode)
		{
			pNode->Release(); 
			pNode = NULL;
		}
	}

	if(pList)
	{
		pList->Release(); 
		pList = NULL;
	}

	NTL_RETURN(TRUE);
}

RwBool CNtlPLPropertyContainer::LoadPlant(const RwChar *pFileName)
{
	NTL_FUNCTION("CNtlPLPropertyContainer::LoadPlant");

	CNtlPLPlantProperty *pProperty = NTL_NEW CNtlPLPlantProperty;
	pProperty->SetName(NTL_PLEN_PLANT);

	CNtlXMLDoc doc;
	doc.Create();

	if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_SCRIPT)
	{		
		BYTE*	pPackBuffer	= NULL;
		RwInt32	iPackSize	= 0;

		GetNtlResourcePackManager()->LoadScript(pFileName, (void**)&pPackBuffer, &iPackSize);
		if (pPackBuffer)
		{			
			BYTE* pTempBuffer = NTL_NEW BYTE [iPackSize + 1];
			memcpy(pTempBuffer, pPackBuffer, iPackSize);
			pTempBuffer[iPackSize] = '\0';

			if(!doc.LoadXML((char*)pTempBuffer))
			{			
				doc.Destroy();
				NTL_ARRAY_DELETE(pTempBuffer);
				NTL_ARRAY_DELETE(pPackBuffer);
				NTL_RETURN(FALSE);
			}
			NTL_ARRAY_DELETE(pTempBuffer);
			NTL_ARRAY_DELETE(pPackBuffer);
		}
	}
	else
	{
		if(doc.Load((char*)pFileName) == false)
		{
			doc.Destroy();
			NTL_RETURN(FALSE);
		}
	}

	char				chBuffer[NTL_MAX_DIR_PATH]	= {0,};
	IXMLDOMNode*		pNode						= doc.SelectSingleNode((char*)"/plant_property/header");
	IXMLDOMNodeList*	pList						= doc.SelectNodeList((char*)"/plant_property/set_element");

	if(!doc.GetTextWithAttributeName(pNode, "respath", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	pProperty->SetResourceFilePath(chBuffer);

	if(!doc.GetTextWithAttributeName(pNode, "texpath", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	pProperty->SetTextureFilePath(chBuffer);

	if(!doc.GetTextWithAttributeName(pNode, "clipdistance", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	pProperty->SetClipDistance(static_cast<RwReal>(::atof(chBuffer)));

	if (pNode)
	{
		pNode->Release();
		pNode = NULL;
	}

	long lNum; 
	pList->get_length(&lNum);
	for(RwInt32 i = 0; i < lNum; ++i)
	{
		pList->get_item(i, &pNode);
		if (!pProperty->Load(&doc, pNode))
		{
			NTL_RETURN(FALSE);
		}

		if(pNode)
		{
			pNode->Release(); 
			pNode = NULL;
		}
	}

	AddProp(PLENTITY_PLANT, pProperty);

	if(pList)
	{
		pList->Release(); 
		pList = NULL;
	}

	NTL_RETURN(TRUE);
}


RwBool CNtlPLPropertyContainer::LoadWeather(const RwChar *pFileName, RwUInt32 uiLevel)
{
	NTL_FUNCTION("CNtlPLPropertyContainer::LoadWeather");

	CNtlPLWeatherProperty *pProperty = NTL_NEW CNtlPLWeatherProperty;

	char acTemp[256];
	sprintf_s(acTemp, 256, "%s_%u", NTL_PLEN_WEATHER, uiLevel);
	pProperty->SetName(acTemp);

	CNtlXMLDoc doc;
	doc.Create();
	if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_SCRIPT)
	{		
		BYTE*	pPackBuffer	= NULL;
		RwInt32	iPackSize	= 0;

		GetNtlResourcePackManager()->LoadScript(pFileName, (void**)&pPackBuffer, &iPackSize);
		if (pPackBuffer)
		{			
			BYTE* pTempBuffer = NTL_NEW BYTE [iPackSize + 1];
			memcpy(pTempBuffer, pPackBuffer, iPackSize);
			pTempBuffer[iPackSize] = '\0';

			if(!doc.LoadXML((char*)pTempBuffer))
			{			
				doc.Destroy();
				NTL_ARRAY_DELETE(pTempBuffer);
				NTL_ARRAY_DELETE(pPackBuffer);
				NTL_RETURN(FALSE);
			}
			NTL_ARRAY_DELETE(pTempBuffer);
			NTL_ARRAY_DELETE(pPackBuffer);
		}
	}
	else
	{
		if(doc.Load((char*)pFileName) == false)
		{
			doc.Destroy();
			NTL_RETURN(FALSE);
		}
	}

	char				chBuffer[NTL_MAX_DIR_PATH]	= {0,};
	IXMLDOMNode*		pNode						= doc.SelectSingleNode((char*)"/weather_property/header");
	IXMLDOMNodeList*	pList						= doc.SelectNodeList((char*)"/weather_property/weather");

	if(!doc.GetTextWithAttributeName(pNode, "sound_path", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	pProperty->SetSoundPath(chBuffer);

	if(!doc.GetTextWithAttributeName(pNode, "texture_path", chBuffer, NTL_MAX_DIR_PATH))
	{
		NTL_RETURN(FALSE);
	}

	pProperty->SetTexturePath(chBuffer);

	if (pNode)
	{
		pNode->Release();
		pNode = NULL;
	}

	long lNum; 
	pList->get_length(&lNum);
	for(RwInt32 i = 0; i < lNum; ++i)
	{
		pList->get_item(i, &pNode);
		if (!pProperty->Load(&doc, pNode))
		{
			NTL_RETURN(FALSE);
		}

		if(pNode)
		{
			pNode->Release(); 
			pNode = NULL;
		}
	}

	AddProp(PLENTITY_WEATHER, pProperty);

	if(pList)
	{
		pList->Release(); 
		pList = NULL;
	}

	NTL_RETURN(TRUE);
}

CNtlPLProperty* CNtlPLPropertyContainer::GetProperty(ENtlPLEntityType eType, const RwChar *pKey)
{
	NTL_FUNCTION("CNtlPLPropertyContainer::GetProp");

	CNtlPLProperty *pProp = NULL;
	MapProp::iterator it;

	switch(eType)
	{
		case PLENTITY_SKY:
		{
			it = m_mapSky.find(pKey);
			if (it != m_mapSky.end())
				pProp = (*it).second;
		}
		break;

		case PLENTITY_WORLD:
		{
			it = m_mapNtlWorld.find(pKey);
			if (it != m_mapNtlWorld.end())
				pProp = (*it).second;
		}
		break;

		case PLENTITY_WATER:
		{
			it = m_mapWater.find(pKey);
			if (it != m_mapWater.end())
				pProp = (*it).second;
		}
		break;

		case PLENTITY_SHORELINE:
		{
			it = m_mapShoreLine.find(pKey);
			if (it != m_mapShoreLine.end())
				pProp = (*it).second;
		}
		break;

		case PLENTITY_OBJECT:
		{
			it = m_mapObject.find(pKey);
			if (it != m_mapObject.end())
				pProp = (*it).second;
		}
		break;

		case PLENTITY_CHARACTER:
		{
			it = m_mapCharacter.find(pKey);
			if (it != m_mapCharacter.end())
				pProp = (*it).second;
		}
		break;
		case PLENTITY_ITEM:
		{
			it = m_mapItem.find(pKey);
			if (it != m_mapItem.end())
				pProp = (*it).second;
		}
		break;

		case PLENTITY_EFFECT:
		{
			it = m_mapEffect.find(pKey);
			if (it != m_mapEffect.end())
				pProp = (*it).second;
		}
		break;
			// by agebreak
		case PLENTITY_SPLINE:
		{
			it = m_mapSpline.find(pKey);
			if (it != m_mapSpline.end())
				pProp = (*it).second;
		}
		break;

		case PLENTITY_LIGHT:
		break;
		// woody1019
	
		case PLENTITY_FOG:
		break;

		case PLENTITY_GUI:
		{
			it = m_mapGui.find(pKey);
			if (it != m_mapGui.end())
				pProp = (*it).second;
		}
		break;


		case PLENTITY_SOUND:
		{
		it = m_mapSound.find(pKey);
		if (it != m_mapSound.end())
			pProp = (*it).second;
		}
		break;


	
		case PLENTITY_PLANT:
		{
			it = m_mapPlant.find(pKey);
			if (it != m_mapPlant.end())
				pProp = (*it).second;
		}
		break;
		case PLENTITY_WEATHER:
		{
			it = m_mapWeather.find(pKey);
			if (it != m_mapWeather.end())
				pProp = (*it).second;
		}
		break;

		default:
		{
			DBO_FAIL("Not defined entity type");
		}
		break;
	}

	NTL_RETURN(pProp);
}

CNtlPLPropertyContainer::MapProp& CNtlPLPropertyContainer::Gets(ENtlPLEntityType eType)
{
	NTL_FUNCTION("CNtlPLPropertyContainer::Gets");

	switch(eType)
	{
		// woody1019
	case PLENTITY_WORLD:
		return m_mapNtlWorld;
		break;

	case PLENTITY_WATER:
		return m_mapWater;
		break;

	case PLENTITY_SHORELINE:
		return m_mapShoreLine;
		break;

	case PLENTITY_SKY:
		return m_mapSky;
		break;

	case PLENTITY_OBJECT:
		return m_mapObject;
		break;

	case PLENTITY_SOUND:
		return m_mapSound;
		break;

	case PLENTITY_ITEM:
		return m_mapItem;
		break;

	case PLENTITY_CHARACTER:
		return m_mapCharacter;
		break;
	case PLENTITY_EFFECT:
		return m_mapEffect;
		break;
		// by agebreak
	case PLENTITY_SPLINE:
		return m_mapSpline;
		break;
	case PLENTITY_GUI:
		return m_mapGui;
		break;
	case PLENTITY_PLANT:
		return m_mapPlant;
		break;
	case PLENTITY_WEATHER:
		return m_mapWeather;
		break;
	default:
		DBO_FAIL("Not defined entity type");
		break;
	}

	NTL_RETURN(m_mapObject);
}

RwBool CNtlPLPropertyContainer::SaveSerialize(RwBool bEncrypt /* = FALSE */, RwChar* szCryptPassword)
{
	// 현재 로드되어 Map에 있는 프로퍼티들을 바이너리 파일에 저장한다.
	CNtlFileSerializer nsl(1024 * 2048, 1024 * 1024);	// 기본 버퍼를 2메가로 잡는다.

	RwBool bResut = TRUE;

	SaveCharacterSerialize(nsl);
	if(bEncrypt)
	{
		bResut = nsl.SaveFile(".\\property\\characterproperty.edf", TRUE, szCryptPassword);
	}
	else
	{
		bResut = nsl.SaveFile(".\\property\\characterproperty.rdf");	
	}

	if(!bResut)
		return FALSE;

	SaveObjectSerialzie(nsl);
	if(bEncrypt)
	{
		bResut = nsl.SaveFile(".\\property\\objectproperty.edf", TRUE, szCryptPassword);
	}
	else
	{
		bResut = nsl.SaveFile(".\\property\\ObjectProperty.rdf");
	}

	if(!bResut)
		return FALSE;

	SaveItemSerialize(nsl);
	if(bEncrypt)
	{
		bResut = nsl.SaveFile(".\\property\\itemproperty.edf", TRUE, szCryptPassword);
	}
	else
	{
		bResut = nsl.SaveFile(".\\property\\itemproperty.rdf");
	}

	if(!bResut)
		return FALSE;

	return bResut;
}

void CNtlPLPropertyContainer::SaveObjectScript(std::string &strFileName)
{
	MapProp::iterator it = m_mapObject.begin();
	for (; it != m_mapObject.end(); ++it)
	{
		CNtlPLObjectProperty* pProperty = (CNtlPLObjectProperty*)it->second;
		if (pProperty)
		{
			std::string strPath = strFileName + it->first + ".xml";
			pProperty->SaveScript(strPath);
		}
	}
}

void CNtlPLPropertyContainer::SaveCharacterScript(std::string &strFileName)
{
	MapProp::iterator it = m_mapCharacter.begin();
	for (; it != m_mapCharacter.end(); ++it)
	{
		CNtlPLCharacterProperty* pProperty = (CNtlPLCharacterProperty*)it->second;
		if (pProperty)
		{
			std::string strPath = strFileName + it->first + ".xml";
			pProperty->SaveScript(strPath);
		}
	}
}

void CNtlPLPropertyContainer::SaveItemScript(std::string &strFileName)
{
	MapProp::iterator it = m_mapItem.begin();
	for (; it != m_mapItem.end(); ++it)
	{
		CNtlPLItemProperty* pProperty = (CNtlPLItemProperty*)it->second;
		if (pProperty)
		{
			std::string strPath = strFileName + it->first + ".xml";
			pProperty->SaveScript(strPath);
		}
	}
}

RwBool CNtlPLPropertyContainer::LoadSerialize(RwBool bEncrypt /* = FALSE */, RwChar* szCryptPassword)
{
	NTL_FUNCTION(__FUNCTION__);

	if(!LoadNtlWorld(""))
	{
		DBO_WARNING_MESSAGE("load LoadNtlWorld fail..");
		NTL_RETURN(FALSE);
	}

	if(!LoadWater(".\\script\\waterproperty.xml"))
	{
		DBO_WARNING_MESSAGE("load waterproperty fail..");
		NTL_RETURN(FALSE);
	}

	if(!LoadShoreLine(".\\script\\ShoreLineProp.xml"))
	{
		DBO_WARNING_MESSAGE("load ShoreLineProp fail..");
		NTL_RETURN(FALSE);
	}

	if(!LoadSound(".\\script\\Environment.tbl"))
	{
		DBO_WARNING_MESSAGE("load Environment fail..");
		NTL_RETURN(FALSE);
	}

	/*if (!LoadEffect(".\\Effect\\VisualEffect.eff"))
	{
		NTL_RETURN(FALSE);
	}*/
	if (!LoadEffect(".\\Effect\\VisualEffect_01.eff"))
	{
		DBO_WARNING_MESSAGE("load VisualEffect fail..");
		NTL_RETURN(FALSE);
	}
	if(!LoadEffect(".\\Effect\\VisualEffect_02.eff"))
	{
		DBO_WARNING_MESSAGE("load VisualEffect fail..");
		NTL_RETURN(FALSE);
	}

	if(!LoadGui(".\\script\\guiproperty.xml"))
	{
		DBO_WARNING_MESSAGE("load guiproperty fail..");
		NTL_RETURN(FALSE);
	}

	if(!LoadPlant(".\\script\\PlantProperty.xml"))
	{
		DBO_WARNING_MESSAGE("load PlantProperty fail..");
		NTL_RETURN(FALSE);
	}

	if(!LoadWeather(".\\script\\WeatherProperty.xml", 0))
	{
		DBO_WARNING_MESSAGE("load WeatherProperty fail..");
		NTL_RETURN(FALSE);
	}

	if(!LoadWeather(".\\script\\WeatherProperty_LV1.xml", 1))
	{
		DBO_WARNING_MESSAGE("load WeatherProperty_LV1 fail..");
		NTL_RETURN(FALSE);
	}

	if(!LoadWeather(".\\script\\WeatherProperty_LV2.xml", 2))
	{
		DBO_WARNING_MESSAGE("load WeatherProperty_LV2 fail..");
		NTL_RETURN(FALSE);
	}

	if(!LoadSoundField(".\\script\\BgmSoundProperty.xml", "FIELD_SOUND_BGM"))
	{
		DBO_WARNING_MESSAGE("load BgmSoundProperty fail..");
		NTL_RETURN(FALSE);
	}

	if(!LoadSoundField(".\\script\\EnvSoundProperty.xml", "FIELD_SOUND_ENV"))
	{
		DBO_WARNING_MESSAGE("load EnvSoundProperty fail..");
		NTL_RETURN(FALSE);
	}

	if(!LoadSoundField(".\\script\\ShareSoundProperty.xml", "FIELD_SOUND_SHARE"))
	{
		DBO_WARNING_MESSAGE("load ShareSoundProperty fail..");
		NTL_RETURN(FALSE);
	}

	// 캐릭터, Item, Object를 로드한다.
	CNtlFileSerializer nsl(1024 * 1024, 1024 * 1024);	// 기본 버퍼를 10메가로 잡는다.

	/*if (nsl.LoadFile(".\\property\\ObjectProperty.rdf") == false)
		DBO_ASSERT(FALSE, __FUNCTION__ << " Object Property LoadFile Fail");
	if(LoadObjectSerialize(nsl) == false)
		DBO_ASSERT(FALSE, __FUNCTION__ << " Object Property Load Fail");*/

	if (!LoadSound(".\\script\\Environment.tbl"))
	{
		DBO_WARNING_MESSAGE("load Environment fail..");
		NTL_RETURN(FALSE);
	}


	/*if (nsl.LoadFile(".\\property\\characterproperty.rdf") == false)
		DBO_ASSERT(FALSE, __FUNCTION__ << " Character Property LoadFile Fail");
	if(LoadCharacterSerialize(nsl) == false)
		DBO_ASSERT(FALSE, __FUNCTION__ << " Character Property Load Fail");

	if (nsl.LoadFile(".\\property\\itemproperty.rdf") == false)
		DBO_ASSERT(FALSE, __FUNCTION__ << " Item Property LoadFile Fail");
	if(LoadItemSerialize(nsl) == false)
		DBO_ASSERT(FALSE, __FUNCTION__ << " Item Property Load Fail");*/


	if(GetNtlResourcePackManager()->GetActiveFlags() & NTL_PACK_TYPE_FLAG_PROPERTY)  // Pack 사용시    
	{   
		void* pData = NULL;
		RwInt32 iSize = 0;

	 //	Character
		GetNtlResourcePackManager()->LoadProperty(".\\property\\characterproperty.rdf", &pData, &iSize);
		DBO_ASSERT(iSize > 0, __FUNCTION__ << "Character Property Load Fail");
		if(iSize > 0)
		{
			nsl.In(pData, iSize);
			LoadCharacterSerialize(nsl);
		}

        NTL_ARRAY_DELETE(pData);        

		// Object
		GetNtlResourcePackManager()->LoadProperty(".\\property\\objectproperty.rdf", &pData, &iSize);
		DBO_ASSERT(iSize > 0 , __FUNCTION__ << "Object Property Load Fail");
		if(iSize > 0)
		{
			nsl.In(pData, iSize);
			LoadObjectSerialize(nsl);
		}

        NTL_ARRAY_DELETE(pData);

		// Item
		GetNtlResourcePackManager()->LoadProperty(".\\property\\itemproperty.rdf", &pData, &iSize);
		DBO_ASSERT(iSize > 0, __FUNCTION__ << "Item Property Load Fail");
		if(iSize > 0)
		{
			nsl.In(pData, iSize);
			LoadItemSerialize(nsl);
		}

        NTL_ARRAY_DELETE(pData);
	}
	else
	{
		RwBool bResult = TRUE;

		// load from xml 
	}

	NTL_RETURN(TRUE);
}

RwBool CNtlPLPropertyContainer::SaveCharacterSerialize(CNtlFileSerializer& sl)
{
	sl.Refresh();

	sl<<CNtlPLCharacterProperty::m_strVer;
	sl<<CNtlPLCharacterProperty::m_strCharacterDataPath;

	sl<<m_mapCharacter.size();
	MapProp::iterator it = m_mapCharacter.begin();
	for(; it != m_mapCharacter.end(); ++it)
	{
		CNtlPLProperty* pProperty = it->second;
		if(pProperty)
		{
			pProperty->SaveSerialize(sl);
		}
	}

	return TRUE;
}

RwBool CNtlPLPropertyContainer::LoadCharacterSerialize( CNtlFileSerializer& sl ) 
{
	sl>> CNtlPLCharacterProperty::m_strVer;
	sl>> CNtlPLCharacterProperty::m_strCharacterDataPath;

	UINT uiPropertySize = 0;
	sl>>uiPropertySize;

	//DBO_WARNING_MESSAGE("version: " << CNtlPLCharacterProperty::m_strVer << " uiPropertySize: " << uiPropertySize);
	for(UINT i = 0; i < uiPropertySize; ++i)
	{
		CNtlPLCharacterProperty* pProperty = NTL_NEW CNtlPLCharacterProperty();
		pProperty->LoadSerialize(sl);

		AddProp(PLENTITY_CHARACTER, pProperty);
	}

	return TRUE;
}

RwBool CNtlPLPropertyContainer::SaveObjectSerialzie( CNtlFileSerializer& sl ) 
{
	sl.Refresh();

	sl<<CNtlPLObjectProperty::m_strVer;
	sl<<CNtlPLObjectProperty::m_strMeshPath;

	sl<<m_mapObject.size();

	MapProp::iterator it = m_mapObject.begin();
	for(; it != m_mapObject.end(); ++it)
	{
		CNtlPLProperty* pProperty = it->second;
		if(pProperty)
		{
			pProperty->SaveSerialize(sl);
		}
	}

	return TRUE;
}

RwBool CNtlPLPropertyContainer::LoadObjectSerialize( CNtlFileSerializer& sl ) 
{
	sl>> CNtlPLObjectProperty::m_strVer;
	sl>> CNtlPLObjectProperty::m_strMeshPath;

	UINT uiPropertySize = 0;
	sl>>uiPropertySize;
//	DBO_WARNING_MESSAGE("version: " << CNtlPLObjectProperty::m_strVer << " uiPropertySize: " << uiPropertySize);
	for(UINT i = 0; i < uiPropertySize; ++i)
	{
		CNtlPLObjectProperty* pProperty = NTL_NEW CNtlPLObjectProperty();
		pProperty->LoadSerialize(sl);

		AddProp(PLENTITY_OBJECT, pProperty);
	}

	return TRUE;
}

RwBool CNtlPLPropertyContainer::SaveItemSerialize( CNtlFileSerializer& sl ) 
{
	sl.Refresh();

	sl<<CNtlPLItemProperty::m_strVer;
	sl<<CNtlPLItemProperty::m_strItemDataPath;

	sl<<m_mapItem.size();
	MapProp::iterator it = m_mapItem.begin();
	for(; it != m_mapItem.end(); ++it)
	{
		CNtlPLProperty* pProperty = it->second;
		if(pProperty)
		{
			pProperty->SaveSerialize(sl);
		}
	}

	return TRUE;
}

RwBool CNtlPLPropertyContainer::LoadItemSerialize( CNtlFileSerializer& sl ) 
{
	sl>> CNtlPLItemProperty::m_strVer;
	sl>> CNtlPLItemProperty::m_strItemDataPath;

	UINT uiPropertySize = 0;
	sl>>uiPropertySize;

	//DBO_WARNING_MESSAGE("version: " << CNtlPLObjectProperty::m_strVer << " uiPropertySize: " << uiPropertySize);
	for(UINT i = 0; i < uiPropertySize; ++i)
	{
		CNtlPLItemProperty* pProperty = NTL_NEW CNtlPLItemProperty();
		pProperty->LoadSerialize(sl);

		AddProp(PLENTITY_ITEM, pProperty);
	}

	return TRUE;
}

#ifdef dNTL_WORLD_TOOL_MODE
RwBool CNtlPLPropertyContainer::ReloadPlant()
{
	CNtlPLProperty *pProp;
	MapProp::iterator it;

	for(it = m_mapPlant.begin(); it != m_mapPlant.end(); it++)
	{
		pProp = (*it).second; 
		NTL_DELETE(pProp);
	}

	m_mapPlant.clear();
	if(!LoadPlant(".\\script\\PlantProperty.xml"))
	{
		return FALSE;
	}

	return TRUE;
}

RwBool CNtlPLPropertyContainer::ReloadWeather()
{
	CNtlPLProperty *pProp;
	MapProp::iterator it;

	for(it = m_mapWeather.begin(); it != m_mapWeather.end(); it++)
	{
		pProp = (*it).second; 
		NTL_DELETE(pProp);
	}

	m_mapWeather.clear();
	if(!LoadWeather(".\\script\\WeatherProperty.xml", 0))
	{
		return FALSE;
	}

	// 	if(!LoadWeather(".\\script\\WeatherProperty_1.xml", 1))
	// 	{
	// 		NTL_RETURN(FALSE);
	// 	}

	return TRUE;
}
#endif
