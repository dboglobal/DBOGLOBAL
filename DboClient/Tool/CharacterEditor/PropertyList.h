#if !defined(_PROPERTY_LIST_H)
#define _PROPERTY_LIST_H
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// MyList.h
//
/////////////////////////////////////////////////////////////////////////////
//
// Copyright ?1999, Stefan Belopotocan, http://welcome.to/StefanBelopotocan
//
/////////////////////////////////////////////////////////////////////////////

#include <afxtempl.h>

//////////////////////////////////////////////////////////////////////
// CMyTypedPtrList

template<class BASE_CLASS, class TYPE>
class CPropertyTypedPtrList : public CTypedPtrList<BASE_CLASS, TYPE>
{
public:
	CPropertyTypedPtrList(int nBlockSize = 10) 
		: CTypedPtrList<BASE_CLASS, TYPE>(nBlockSize) 
	{
	}

	~CPropertyTypedPtrList()
	{
		Destroy();
	}

	void Destroy()
	{
		POSITION pos = GetHeadPosition();

		while(pos != NULL)
		{
			TYPE node = GetNext(pos);
			delete node;
		}

		RemoveAll();
	}
};

#endif //!defined(_PROPERTY_PARTICLE_LIST_H)