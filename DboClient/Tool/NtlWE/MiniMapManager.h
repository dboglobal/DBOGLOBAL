#pragma once



#define dMM_PROP_MOB		0x00000001
#define dMM_PROP_NPC		0x00000002
#define dMM_PROP_TRIGGER	0x00000004


class CMiniMapManager
{
public:
	CMiniMapManager(void);
	~CMiniMapManager(void);

	static CMiniMapManager* GetInstance(VOID)
	{
		static CMiniMapManager g_MiniMapManager;
		return &g_MiniMapManager;
	}

private:
	RwInt32		m_TexSize;
	RwBool		m_CreateAll;
	RwBool		m_IsCreating;
	DWORD		m_MMProp;

public:
	RwBool		Create(RwInt32 FieldIdx);
	RwBool		CreateProp(RwInt32 FieldIdx);

	RwBool		IsCreating()					{ return m_IsCreating; }

	RwInt32&	GetTexSize()					{ return m_TexSize; }
	RwBool&		GetAllFields()					{ return m_CreateAll; }

	DWORD		GetMMProp()						{ return m_MMProp; }
	VOID		SetMMProp(DWORD _MMProp)		{ m_MMProp = _MMProp; }

protected:
	void		SaveMinimap(RwInt32 FieldIdx, RwUInt32 uiLayer);
};
