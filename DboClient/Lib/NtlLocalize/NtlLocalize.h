/*****************************************************************************
 *
 * File			: NtlLocalize.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2007. 3. 19	
 * Abstract		: Localizing에 따른 정의.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

//----------------------------------------
// DBO Launcher에서의 함수의 실행 순서
//----------------------------------------

// 1. Local_Init();
// 2. Local_DetectData();
// 3. Local_DBOEncryptConfig();
// 4. DBO Client 실행할 때 command에 DBO encrypt config data를 넣어준다.
// 5. Local_Terminate();


//----------------------------------------
// DBO client 에서의 함수의 실행 순서
//----------------------------------------

// 1. Local_Init();
// 2. Local_DBODecryptConfig();  command에서의 값을 넣어준다.
// 3. Local_Terminate();


#ifndef __NTL_LOCALIZE__
#define __NTL_LOCALIZE__

#include "NtlLocalizeDef.h"

void			Local_Init(void);
void			Local_Terminate(void);

unsigned char	Local_GetType(void);	
SLocalConfig*	Local_GetConfig(void);

// Localizing data 검출.(launcher로 넘어오는 decrypt key)
bool			Local_DetectData(const char *pKey);


// DBO data make
const char*		Local_DBOEncryptConfig(void);
bool			Local_DBODecryptConfig(const char *pConfig);


#endif
