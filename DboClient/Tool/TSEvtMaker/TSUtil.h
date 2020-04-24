#pragma once


#include "Table.h"


class CTableContainer;


CTableContainer*	CreateTableContainer( const char* pPath, CTable::eLOADING_METHOD eLoadingMethod );
void				DeleteTableContainer( CTableContainer*& pTblContainer );
