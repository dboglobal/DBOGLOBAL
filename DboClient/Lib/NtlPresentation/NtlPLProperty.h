/*****************************************************************************
 *
 * File			: NtlPLProperty.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 8. 01	
 * Abstract		: Presentation layer property base class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PLPROPERTY_H__
#define __NTL_PLPROPERTY_H__

#define SIZE_PROP_BUF   64

#include "NtlSerializer.h"

class CNtlXMLDoc;

/**
 * \ingroup NtlPresentation
 * entity의 attibute를 나타내는 base class이다.
 * 모든 entity들은 instance가 생성된 후에 SetProperty interface 함수를 통하여 property data를 전달된다.
 *
 */
class CNtlPLProperty
{
protected:

	RwUInt32 m_uiId;
	std::string m_strName;				/**< String describing the presentation entity property name */
	std::string m_strClassID;			/**< String describing the presentation entity property class id */
    RwUInt32    m_uiFlag;               ///< 각 Entity에 따른 Flag 

public:

	/**
    *  Default constructor for Presentation entity property.
    *
    */
	CNtlPLProperty() {m_uiId = 0xffffffff; m_uiFlag = 0;}

	/**
    *  Virtual destructor for Presentation entity property.
    *
    */
	virtual ~CNtlPLProperty() {}

	/**
	*  prosentation entity property id setting.
	*  \param pName property id.
	*  \see GetId
	*/
	void SetId(const RwUInt32 uiId);

	/**
	*  prosentation entity property name setting.
	*  \return property id을 리턴한다.
	*  \see SetId
	*/
	RwUInt32 GetId(void) const;

	/**
	*  prosentation entity property name setting.
	*  \param pName property name.
	*  \see GetName
	*/
	void SetName(const RwChar *pName);

	/**
	*  prosentation entity property name을 얻어오는 함수.
	*  \return property name string을 리턴한다.
	*  \see SetName
	*/
	const RwChar* GetName(void);

	/**
	*  prosentation entity property class id setting.
	*  \return property name을 리턴한다.
	*  \see GetClassID
	*/
	void SetClassID(const RwChar *pClassID);

	/**
	*  prosentation entity property class id를 얻어오는 함수.
	*  \return property class id string을 리턴한다.
	*  \see SetClassID
	*/
	const RwChar* GetClassID(void);

	///**
	//*  property data load 함수.
	//*  \return 성공이면TRUE, 실패이면 FALSE
	//*  \param pNode XML doc node
	//*  \see Save
	//*/
	virtual RwBool Load(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode) = 0;

	///**
	//*  property data save 함수.
	//*  \return 성공이면TRUE, 실패이면 FALSE
	//*  \param pNode XML doc node
	//*  \see Load
	//*/
	virtual RwBool Save(CNtlXMLDoc *pDoc, IXMLDOMNode *pNode) = 0;

	virtual void	SaveSerialize(CNtlSerializer& sOut) {}											///< Serialize에 데이터를 저장하는 함수
	virtual void	LoadSerialize(CNtlSerializer& sIn) {}											///< Serialize로부터 데이터를 가져오는 함수

    virtual RwUInt32 GetFlag() {return m_uiFlag;}
    virtual void     SetFlag(RwUInt32 uiFlag) {m_uiFlag = uiFlag;}
};

inline void CNtlPLProperty::SetId(const RwUInt32 uiId)
{
	m_uiId = uiId;
}

inline RwUInt32 CNtlPLProperty::GetId(void) const
{
	return m_uiId;
}

inline void CNtlPLProperty::SetName(const RwChar *pName)
{
	m_strName = pName;
}

inline const RwChar* CNtlPLProperty::GetName(void)
{
	return m_strName.c_str(); 
}

inline void CNtlPLProperty::SetClassID(const RwChar *pClassID)
{
	m_strClassID = pClassID;
}

inline const RwChar* CNtlPLProperty::GetClassID(void) 
{
	return m_strClassID.c_str(); 
}


#endif