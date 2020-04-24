#pragma once


class CNtlWorldFieldManager;


enum eFILE_POS_AT_SECTOR
{
	eFPAS_HEIGHTFIELD = 0,
	eFPAS_DOODADS,
};


class CMergeManager
{
public:
	CMergeManager(void);
	~CMergeManager(void);

public:
	static CMergeManager* GetInstance(void);

	// Return value FILE* should be closed right after using.
	FILE* SetFilePosAtSector(RwV3d _WorldPos, eFILE_POS_AT_SECTOR _FilePosAtSector);
};
