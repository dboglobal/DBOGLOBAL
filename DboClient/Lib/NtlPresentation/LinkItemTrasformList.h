#pragma once


#include "NtlDefaultItemData.h"


#define dLINK_ITEM_TRANSFORM_LIST_XML_FILE_PATH (".\\script\\LinkItemTrasformList.xml")


class CLinkItemTrasformChunk;


typedef std::map<std::string, CLinkItemTrasformChunk*>				tLINK_ITEM_TRASFORM_MAP;
typedef std::map<std::string, CLinkItemTrasformChunk*>::value_type	tLINK_ITEM_TRASFORM_MAP_VALUE;
typedef	std::map<std::string, CLinkItemTrasformChunk*>::iterator	tLINK_ITEM_TRASFORM_MAP_ITER;


struct sLINK_ITEM_TRASFORM
{
	RwReal	_SCL[3], _ROT[3], _POS[3];

	sLINK_ITEM_TRASFORM()
	{
		_SCL[0] = _SCL[1] = _SCL[2] = 1.0f;
		_ROT[0] = _ROT[1] = _ROT[2] = _POS[0] = _POS[1] = _POS[2] = 0.0f;
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CLinkItemTrasformChunk
{
public:
	CLinkItemTrasformChunk(eRACE_GENDER_CLASS_TYPE& _EGCT, sLINK_ITEM_TRASFORM& _LIT);
	~CLinkItemTrasformChunk(void);

protected:
	sLINK_ITEM_TRASFORM m_LIT[RGCT_TOT_CNT];

public:
	VOID					Set(eRACE_GENDER_CLASS_TYPE& _EGCT, sLINK_ITEM_TRASFORM& _LIT);
	sLINK_ITEM_TRASFORM*	Get(eRACE_GENDER_CLASS_TYPE _EGCT);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CLinkItemTrasformList
{
public:
	CLinkItemTrasformList(void);
	~CLinkItemTrasformList(void);

public:
	static CLinkItemTrasformList& GetInstance();

protected:
	void	Create();
	void	Destroy();
	VOID	Set(string& _KeyStr, eRACE_GENDER_CLASS_TYPE& _EGCT, sLINK_ITEM_TRASFORM& _LIT);

protected:
	tLINK_ITEM_TRASFORM_MAP	m_mapLinkItemTransformChunk;

public:
	sLINK_ITEM_TRASFORM*	Get(string& _KeyStr, eRACE_GENDER_CLASS_TYPE _EGCT);
	void					Refresh();
};
