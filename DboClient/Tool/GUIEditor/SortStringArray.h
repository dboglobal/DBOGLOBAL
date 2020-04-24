// SortStringArray.h: Schnittstelle f? die Klasse CSortStringArray.
//
//////////////////////////////////////////////////////////////////////

#if !defined __SORTSTRINGARRAY_H__
#define __SORTSTRINGARRAY_H__

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CSortStringArray : public CStringArray  
{
public:
	void Sort();
private: 
	BOOL CompareAndSwap( int pos );

};

#endif // !defined(AFX_SORTSTRINGARRAY_H__64C6B322_B44E_11D2_955E_204C4F4F5020__INCLUDED_)
