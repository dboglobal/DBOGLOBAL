/*****************************************************************************
 *
 * File			: NtlPLSceneManagerFactroy.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 8. 01	
 * Abstract		: Presentation scene manager factroy class 
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_PLSCENEMANAGER_FACTROY_H__
#define __NTL_PLSCENEMANAGER_FACTROY_H__

class CNtlPLSceneManager;

/**
 * \ingroup NtlPresentation
 * 
 *
 */

class CNtlPLSceneManagerFactory
{
private:

	static CNtlPLSceneManager *m_pSceneManager;		/**< active된 scene manager instance pointer */
    	
public:

	/**
    *  scene manager instance를 얻어오는 함수.
	*
    *  \return scene manager instance pointer를 리턴.
    */
	static CNtlPLSceneManager* GetInstance();
	
	/**
    *  active된 scene manager pointer를 등록한다.
	*
    *  \param scene manager pointer.
    */
	static void ActiveSceneManager(CNtlPLSceneManager *pSceneManager);
};


#endif
