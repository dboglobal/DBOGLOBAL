// TSToolDefine.h
//


#pragma once


/** 
	Version
*/


enum { eTS_VERSION = 0 };


/** 
	Select group type
*/

enum eSEL_GROUP_TYPE
{
	eSEL_GROUP_TYPE_MAIN,
	eSEL_GROUP_TYPE_EXCEPTION,

	eSEL_GROUP_TYPE_INVALID				= 0xffffffff
};


/** 
	Shapes
*/


typedef int HSHAPE;
