//***********************************************************************************
//
//	File		:	NtlLinkList.cpp
//
//	Begin		:	2005-11-30
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Linked List
//
//***********************************************************************************


#include "StdAfx.h"
#include "NtlLinkList.h"

#include <stdio.h>

class CTest : public CNtlLinkObject
{
public:
	CTest( int id ) : m_id(id) {}

	int		m_id;
};

void Dump(CNtlLinkList * pList)
{
	CTest * pTest = (CTest*) pList->GetFirst();

	while( pTest )
	{
		printf("%d\n", pTest->m_id);
		pTest = (CTest*) pTest->GetNext();
	}
	printf("-----------------------------------------\n");
}

void CNtlLinkList::UnitTest()
{
	CNtlLinkList list;

	CTest a(1);
	CTest b(2);
	CTest c(3);
	CTest d(4);
	CTest e(5);
	CTest f(0);

	//- hyun : Not referenced variable;
//	CTest *pTest = 0;

	list.Append(&a);
	Dump( &list );

	list.InsertAfter(&a, &d);
	Dump( &list );

	list.InsertBefore(&d, &b);
	Dump( &list );

	list.InsertBefore(&d, &c);
	Dump( &list );

	list.Append(&e);
	Dump( &list );

	list.Prepend(&f);
	Dump( &list );

	printf("Dumping list: expect 0,1,2,3,4,5\n");
	Dump( &list );

	list.RemoveAll();

	list.Append(&a);
	list.Append(&b);
	list.Append(&d);
	list.Append(&c);
	list.Append(&e);
	list.Append(&f);

	printf("Dumping list: expect 1,2,4,3,5,0\n");
	Dump( &list );

	list.Remove( &c );
	Dump( &list );

	list.InsertAfter(&b, &c);
	Dump( &list );

	CTest * pTemp1 = (CTest*) list.Pop();
	CTest * pTemp2 = (CTest*) list.Pop();
	while( list.Pop() );

	Dump( &list );

	list.Push( (CTest *) pTemp2 );
	list.Push( (CTest *) pTemp1 );

	Dump( &list );
}

