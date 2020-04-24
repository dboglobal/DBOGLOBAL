/*****************************************************************************
 *
 * File			: NtlDebug.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 08. 09	
 * Abstract		: memory handler
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __RWSMEMORY_H_
#define __RWSMEMORY_H_

#define RWS_NEW						new

#define RWS_DELETE( p )				if ( p ) { delete p; p = NULL; } 
#define RWS_ARRAY_DELETE( p )		if ( p ) { delete [] p; p = NULL; }


#endif      /* _RWSMEMORY_H_ */
