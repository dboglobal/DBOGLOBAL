#ifndef __DIB_H__
#define __DIB_H__

class CDib
{
public:

	BYTE m_byKeyRed;
	BYTE m_byKeyGreen;
	BYTE m_byKeyBlue;

	int m_nWidth;
	int m_nHeight;

	int	m_nSize;
	int *m_pData;

public:

	CDib();
	~CDib();

	BOOL Load(char *pFileName);
	BOOL LoadLLE(char *pFileName);
	BOOL SaveLLE(char *pFileName);
	
	void UnLoad(void);

	BOOL IsLoad(void) 
	{
		if(m_pData)
			return TRUE;
		return FALSE;
	}
};


#endif