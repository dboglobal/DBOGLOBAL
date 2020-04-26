#pragma once


#include "Table.h"
#include "TableContainer.h"

bool				CreateTableContainer(const char* pPath, CTable::eLOADING_METHOD eLoadingMethod);
void				DeleteTableContainer();

CTableContainer*	GetTableContainer();