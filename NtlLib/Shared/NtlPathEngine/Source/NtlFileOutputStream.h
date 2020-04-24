#ifndef _NTL_FILE_OUTPUT_STREAM_H_
#define _NTL_FILE_OUTPUT_STREAM_H_

#include "..\PathEngine\i_pathengine.h"

class CNtlFileOutputStream : public iOutputStream
{
	FILE *m_fp;

public:
	CNtlFileOutputStream(const char* name);
	~CNtlFileOutputStream();
	
	// Implement
	void put(const char* data, unsigned long dataSize);	
};

#endif
