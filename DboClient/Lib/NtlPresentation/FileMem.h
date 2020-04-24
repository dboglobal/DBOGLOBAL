#pragma once


class CFileMem
{
public:
	CFileMem(void);
	~CFileMem(void);

private:
	BYTE*	m_pFileMem;
	RwInt32	m_MemSize;
	FILE*	m_pFile;

public:
	RwBool	Load(RwChar* _pFilePath, RwChar* _pFileMode);
	RwBool	Save();
	VOID	Free();
	BYTE*	GetDat();
	RwBool	SetDat(RwInt32 _MemPos, RwInt32 _MemSize, BYTE* pDat);
	RwInt32 GetSize() { return m_MemSize; }

	// reallocate if needed
	RwBool	Update(RwInt32 _FileSize);
};
