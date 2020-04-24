#pragma once


class CMemStream
{

protected:

	BYTE*			m_pMemBlock;
	BYTE*			m_pMemBlockEnd;
	BYTE*			m_pDataPosStart;
	BYTE*			m_pDataPosEnd;

public:

	CMemStream();

public:

	void				Attach(BYTE* pBlock, int nBlockSize);

	BYTE*				Detach();

	void				Reset();

	BYTE*				GetFilledPos();

	BYTE*				GetEndPos();

	//GetFilledSize

	//GetEmptySize

	bool				IsEmptyBlock();

	//Push

	CMemStream&			Push(BYTE* p, DWORD dwSize);

	//Pop

	bool				IsEnough(DWORD dwSize);

	//Refine
};