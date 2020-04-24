#ifndef _QUERYRESULT_H
#define _QUERYRESULT_H

#include "Field.h"

class QueryResult
{
public:
	QueryResult(UINT32 fields, UINT32 rows) : mFieldCount(fields), mRowCount(rows), mCurrentRow(NULL) {}
	virtual ~QueryResult() {}

	virtual bool NextRow() = 0;
	void Delete() { delete this; }

	inline Field* Fetch() { return mCurrentRow; }
	inline UINT32 GetFieldCount() const { return mFieldCount; }
	inline UINT32 GetRowCount() const { return mRowCount; }
	inline Field* operator [](int i) { return &mCurrentRow[i]; }

protected:
	UINT32 mFieldCount;
	UINT32 mRowCount;
	Field *mCurrentRow;
};



#endif