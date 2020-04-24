/*****************************************************************************
*
* File			: NtlPLPropertyContaier.h
* Author		: HyungSuk, Jang
* Copyright	: (주)NTL
* Date			: 2005. 8. 09	
* Abstract		: Presentation object property container class
*****************************************************************************
* Desc         : 
*
*****************************************************************************/


#ifndef __NTL_PLPROPERTY_CONTAINER_H__
#define __NTL_PLPROPERTY_CONTAINER_H__

#include "NtlWorldCommon.h"
#include "NtlPLDef.h"
#include "NtlFileSerializer.h"

#pragma warning(disable : 4996)

class CNtlPLProperty;

/**
* \ingroup NtlPresentation
* Presentation에 사용하는 모든 property를 관리하는 conatiner class 이다.\
* 이 객체는 singleton 객체이다. GetInstance() 함수를 사용한다.
*
*/
class CNtlPLPropertyContainer
{
public:

	typedef std::map<std::string, CNtlPLProperty*> MapProp;

private:

	static CNtlPLPropertyContainer* m_pInstance;

protected:

	MapProp m_mapSky;			/* sky property **/
	MapProp m_mapNtlWorld;		/* woody1019 **/
	MapProp m_mapSound;			/* woody1019 **/
	MapProp m_mapShoreLine;		/* woody1019 **/
	MapProp m_mapWater;			
	MapProp m_mapObject;		/* object property **/
	MapProp m_mapCharacter;		/* character property **/
	MapProp m_mapEffect;		/* effect property **/	
	MapProp m_mapSpline;		/* by agebreak **/	
	MapProp m_mapGui;
	MapProp m_mapItem;          /* by hodong **/
	MapProp m_mapPlant;
	MapProp m_mapWeather;

public:

	/**
	*  property를 container에 add한다.
	*  \param eType entity type.
	*  \param pProp add할 property pointer.
	*/
	// by agebreak
	virtual RwBool AddProp(ENtlPLEntityType eType, CNtlPLProperty *pProp);

protected:

	// woody1019
	RwBool	LoadNtlWorld(const RwChar *pFileName);
	RwBool	LoadWater(const RwChar *pFileName);
	RwBool	LoadShoreLine(const RwChar *pFileName);
	RwBool	LoadSound(const RwChar *pFileName);

	/**
	*  effect property를 load 하는 함수.
	*  \param pFileName property file name.
	*  \return 성공하면 TRUE, 실패하면 FALSE.
	*  \see LoadObject
	*  \see LoadCharacter
	*/
	RwBool	LoadEffect(const RwChar *pFileName);

	/**
	*  effect property를 load 하는 함수.
	*  \param pFileName property file name.
	*  \return 성공하면 TRUE, 실패하면 FALSE.
	*  \see LoadObject
	*  \see LoadCharacter
	*  \see LoadEffect
	*/
	RwBool	LoadGui(const RwChar *pFileName);

	/**
	*  plant property를 load 하는 함수.
	*  \param pFileName property file name.
	*  \return 성공하면 TRUE, 실패하면 FALSE.
	*/
	RwBool	LoadPlant(const RwChar *pFileName);

	/**
	*  weather property를 load 하는 함수.
	*  \param pFileName property file name.
	*  \return 성공하면 TRUE, 실패하면 FALSE.
	*/
	RwBool	LoadWeather(const RwChar *pFileName, RwUInt32 uiLevel);

	/**
	*  field bgm/env/share property를 load 하는 함수.
	*  \param pFileName property file name.
	*  \return 성공하면 TRUE, 실패하면 FALSE.
	*/
	RwBool	LoadSoundField(const RwChar *pFileName, const RwChar* pName);

	// Serialize 저장/로드 관련 메소드들
	RwBool	SaveCharacterSerialize(CNtlFileSerializer& sl);
	RwBool	LoadCharacterSerialize(CNtlFileSerializer& sl);

	RwBool	SaveObjectSerialzie(CNtlFileSerializer& sl);
	RwBool	LoadObjectSerialize(CNtlFileSerializer& sl);

	RwBool	SaveItemSerialize(CNtlFileSerializer& sl);
	RwBool	LoadItemSerialize(CNtlFileSerializer& sl);

public:

	/**
	*  Default constructor for Presentation property container
	*
	*/
	CNtlPLPropertyContainer();

	/**
	*  destructor for Presentation property container
	*
	*/
	virtual ~CNtlPLPropertyContainer();

	/**
	*  property container 객체가 생성된 다음 호출하는 함수.
	*  \see Destroy
	*/
	virtual RwBool Create(void);

	/**
	*  property container 객체가 생성된 다음 호출하는 함수.
	*  \see Destroy
	*/
	virtual void Destroy(void);

	/**
	*  property container instaince 함수.
	*  이 함수를 사용하여 멤버 함수/변수에 접근한다.
	*  \see Destroy
	*/
	static CNtlPLPropertyContainer* GetInstance(void);

	/**
	*  presentation에 사용하는 모든 property file을 loading하는 함수.
	*  \param pFileName는 filename이다.
	*/
	virtual RwBool Load();

	/**
	*  object property를 load 하는 함수.
	*  \param pFileName property file name.
	*  \return 성공하면 TRUE, 실패하면 FALSE.
	*  \see LoadCharacter
	*  \see LoadEffect
	*/
	RwBool LoadObject(const RwChar *pFileName);

	/**
	*  character property를 load 하는 함수.
	*  \param pFileName property file name.
	*  \return 성공하면 TRUE, 실패하면 FALSE.
	*  \see LoadObject
	*  \see LoadEffect
	*/
	RwBool LoadCharacter(const RwChar *pFileName);

	/**
	*  item property를 load 하는 함수.
	*  \param pFileName property file name.
	*  \return 성공하면 TRUE, 실패하면 FALSE.
	*  \see LoadObject
	*  \see LoadEffect
	*/
	RwBool LoadItem(const RwChar *pFileName);


	virtual RwBool LoadSerialize(RwBool bEncrypt = FALSE, RwChar* szCryptPassword = NULL);				///< 바이너리로 컨버팅된 데이터들을 로드한다.
	virtual RwBool SaveSerialize(RwBool bEncrypt = FALSE, RwChar* szCryptPassword = NULL);				///< 바이너리로 컨버팅된 데이터들을 세이브한다.

	void		SaveObjectScript(std::string &strFileName);
	void		SaveCharacterScript(std::string &strFileName);
	void		SaveItemScript(std::string &strFileName);

	/**
	*  presentation에 사용하는 모든 property file을 loading하는 함수.
	*  \param eType는 entity type
	*  \param uiId 얻어올 property id
	*  \return 성공하면 property 객체 pointer, 실패하면 NULL 이다.
	*/
	CNtlPLProperty* GetProperty(ENtlPLEntityType eType, const RwChar *pKey);

	/**
	*  entity type에 해당하는 property container의 참조 변수를 얻어온다.
	*  \param eType는 entity type
	*  \param mapProps property container를 얻을 참조 변수.
	*/
	MapProp& Gets(ENtlPLEntityType eType);

#ifdef dNTL_WORLD_TOOL_MODE
	RwBool ReloadPlant();
	RwBool ReloadWeather();
#endif
};

#endif
