#pragma once

#include "ntlworldcommon.h"

#define dMINI_INDOOR_EFFECT_SWITCHING_TIME (2.0f)

enum EPL_OBJECT_TYPE
{
	EPL_OBJECT_TYPE_OUTDOOR_OBJECT = 0,
	EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE,
	EPL_OBJECT_TYPE_MINI_INDOOR_OPEN,
	EPL_OBJECT_TYPE_MINI_INDOOR_OBJECT,
};

enum EPL_OBJECT_FUNC_FLAG
{
	EPL_OBJECT_FUNC_FLAG_NONE		= 0x00000000,
	EPL_OBJECT_FUNC_FLAG_PROPERTY	= 0x00000001,
};

struct sNTL_OBJECT_PROP : public sNTL_FIELD_PROP
{
	sNTL_OBJECT_PROP::sNTL_OBJECT_PROP(RwUInt32 Param) : sNTL_FIELD_PROP(Param)
	{
	}

	sNTL_OBJECT_PROP::~sNTL_OBJECT_PROP()
	{
	}

	inline void operator = (sNTL_FIELD_PROP& Prop)
	{
		sNTL_FIELD_PROP::operator = (Prop);		
	}
};

class CNtlPLObject;
class CNtlPLObjectType
{
public:
	CNtlPLObjectType();
	virtual ~CNtlPLObjectType();

	virtual void					Create(const CNtlPLObject* pPLObject);
	virtual void					Destroy();

	virtual void					Update(RwReal fElapsed);

	virtual RwBool					LoadFromFile(FILE* pFile);
	virtual RwBool					SaveIntoFile(FILE* pFile);

	virtual RwUInt32				GetType()		{ return EPL_OBJECT_TYPE_OUTDOOR_OBJECT; }
	virtual RwUInt32				GetFuncFlags()	{ return EPL_OBJECT_FUNC_FLAG_NONE; }

protected:
	CNtlPLObject*					m_pPLObject;
};

class CNtlPLObjectTypeHasProperty : public CNtlPLObjectType
{
public:
	CNtlPLObjectTypeHasProperty();
	virtual ~CNtlPLObjectTypeHasProperty();

	virtual void					Create(const CNtlPLObject* pPLObject);
	virtual void					Destroy();

	virtual void					Update(RwReal fElapsed);

	virtual RwBool					LoadFromFile(FILE* pFile);
	virtual RwBool					SaveIntoFile(FILE* pFile);
	static  RwInt32					SkipToFile(FILE* pFile);

	static	BYTE*					SaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	static	BYTE*					SkipToFileMem(BYTE* pFileMem);

	virtual RwUInt32				GetFuncFlags()	{ return EPL_OBJECT_FUNC_FLAG_PROPERTY; }

	virtual void					SetObjectProperty(sNTL_FIELD_PROP& sNtlFieldProp, eNTL_FIELD_PROPID eNtlFieldPropID);

	virtual RwBool					SetObjectProperty(sNTL_FIELD_PROP& sNtlFieldProp);
	virtual RwBool					GetObjectProperty(sNTL_FIELD_PROP& sNtlFieldProp);

	virtual RwBool					HasObjectProperty() { return (m_pNtlObjectProp ? TRUE : FALSE); }

protected:
	RwReal							m_fObjectPropVariationRestTime;
	sNTL_OBJECT_PROP*				m_pNtlObjectProp;
};

class CNtlPLObjectType_MiniIndoor_Close : public CNtlPLObjectTypeHasProperty
{
public:
	CNtlPLObjectType_MiniIndoor_Close();
	virtual ~CNtlPLObjectType_MiniIndoor_Close();

	virtual RwUInt32				GetType()	{ return EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE; }	
};

class CNtlPLObjectType_MiniIndoor_Open : public CNtlPLObjectTypeHasProperty
{
public:
	CNtlPLObjectType_MiniIndoor_Open();
	virtual ~CNtlPLObjectType_MiniIndoor_Open();

	virtual RwUInt32				GetType()	{ return EPL_OBJECT_TYPE_MINI_INDOOR_OPEN; }
};

class CNtlPLObjectType_MiniIndoor_Object : public CNtlPLObjectType
{
public:
	CNtlPLObjectType_MiniIndoor_Object();
	virtual ~CNtlPLObjectType_MiniIndoor_Object();

	virtual RwUInt32				GetType()	{ return EPL_OBJECT_TYPE_MINI_INDOOR_OBJECT; }
};


