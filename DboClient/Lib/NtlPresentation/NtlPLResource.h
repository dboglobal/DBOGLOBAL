/*****************************************************************************
 *
 * File			: NtlPLResouce.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 7. 21	
 * Abstract		: Presentation layer resource base class
 *****************************************************************************
 * Desc         : 
 * Adjust		: HuunSuk, Jang(2005. 7 29)
 *				    - dictory type 변수/함수 추가.
 *
 *****************************************************************************/

#ifndef __NTL_PLRESOURCE_H__
#define __NTL_PLRESOURCE_H__

#include <list>
#include <rwcore.h>
#include <rpworld.h>
#include <rtanim.h> 

/**
 * \ingroup NtlPresentation
 * RenderWare resource wrapping class이다.
 * RenderWare에서는 clump 와 atomic data는 clone을 할 수 있는데, 
 * resource의 일관성 있는 관리를 위하여 같은 이름의 data를 사용할 경우 reference count를 사용하였으며, 
 * clone resource일 경우에는 original CNtlPLResource에 clone list로 CNtlPLReseource를 link 하는 방식을 선택하였다.
 */

class CNtlPLResource
{
private:

	std::string m_strName;					/**< String describing the resource name */
	unsigned int m_uiType;					/**< id describing the resource type */
	const void *m_pData;					/**< Pointer to resource */
    unsigned int m_uiRefCount;				/**< Reference count used to lock the resource */
	bool m_bClone;

public:
      
	  CNtlPLResource(void) { m_uiType = 0; m_uiRefCount = 0; m_pData = NULL; m_bClone = false; }
	  ~CNtlPLResource(void) {} 
	  
      /**
      *  Set the name of the resource, managed by this CResource object.
      *
      *  \param pName A string containing the name of the resource.
      */
	  void SetName(const char *pName) { m_strName = pName; }

      /**
      *  Set the type of the resource, managed by this CResource object.
      *
      *   \param pType A unsigned int containing the type of the resource.
      */
      void SetType(unsigned int uiType)  {  m_uiType = uiType; }
     
	  /**
      *  Set resource pointer to the resource, managed by this CResource object.
      *
      *   \param bClone renderware resource clone flag
      */
	  void SetClone(bool bClone) { m_bClone = bClone; }

      /**
      *  Set resource pointer to the resource, managed by this CResource object.
      *
      *   \param pResource A pointer to the resource
      */
      void SetData(const void *pData) { m_pData = pData; }
          
      /**
      *  refcount of the resource, increment.
      */
      void AddRef(void) { m_uiRefCount++; }

	  /**
      *  refcount of the resource, decrement.
      */
	  void ReleaseRef(void) { if(m_uiRefCount > 0) m_uiRefCount--; }
      
      /**
      *  Get the name of the resource, managed by this CResource object.
      *
      *   \return A string containing the type of the resource.
      */
      const char* GetName(void) const {return m_strName.c_str(); }

      /**
      *  Get the type of the resource, managed by this CResource object.
      *
      *   \return A unsigned int containing the type of the resource.
      */
      unsigned int GetType(void) const {return m_uiType;}

       /**
      *  Get the type of the resource, managed by this CResource object.
      *
      *   \return clone flag the renderware resource.
      */

	  bool GetClone(void) const { return m_bClone; }

      /**
      *  Get resource pointer to the resource, managed by this CResource object.
      *
      *   \return A pointer to the resource
      */
      const void* GetData(void) const {return m_pData;}
      
      /**
      *  Get the lock refcount of the resource, managed by this CResource object.
      *
      *   \return The lock refcount of this resource
      */
      unsigned int GetRefCount(void) const {return m_uiRefCount;}

	  /**
      *  RpWorld pointer로 casting 한다.
      */
	  RpWorld* GetWorld(void) { return reinterpret_cast<RpWorld*>(const_cast<void*>(m_pData)); }

  	  /**
      *  RpClump pointer로 casting 한다.
      */
	  RpClump* GetClump(void) { return reinterpret_cast<RpClump*>(const_cast<void*>(m_pData)); }

	  /**
      *  RpAtomic pointer로 casting 한다.
      */
	  RpAtomic* GetAtomic(void) { return reinterpret_cast<RpAtomic*>(const_cast<void*>(m_pData)); }

	  /**
      *  RtAnimAnimation pointer로 casting 한다.
      */
	  RtAnimAnimation* GetAnimation(void) { return reinterpret_cast<RtAnimAnimation*>(const_cast<void*>(m_pData)); }
};


#endif
