#ifndef __NTL_NAVI_DATA_DEFINE_H__
#define __NTL_NAVI_DATA_DEFINE_H__


enum eNAVI_INFO
{
	eNAVI_INFO_BASE,
	eNAVI_INFO_WORLD,
	eNAVI_INFO_WORLD_OUTDOOR,
	eNAVI_INFO_WORLD_INDOOR,
	eNAVI_INFO_GROUP_OUTDOOR,
	eNAVI_INFO_GROUP_INDOOR,
	eNAVI_INFO_PROP_OUTDOOR,
	eNAVI_INFO_PROP_INDOOR
};


enum eNAVI_WORLD_TYPE
{
	eNAVI_WORLD_TYPE_OUTDOOR,
	eNAVI_WORLD_TYPE_INDOOR,

	eNAVI_WORLD_TYPE_INVALID
};

#define IDENTITY_SIZE_BY_BYTE				(4)

#define IDENTITY_NAVI_WORLD					("NVWI")
#define IDENTITY_NAVI_GROUP					("NVGI")
#define IDENTITY_NAVI_PROP					("NVPI")

#define VERSION_NAVI_WORLD					(0)
#define VERSION_NAVI_GROUP					(0)
#define VERSION_NAVI_PROP					(0)

#define NAVI_WORLD_NAME						("worldinfo.dat")
#define NAVI_GROUP_OUTDOOR_NAME(b, p)		sprintf( b, "groupinfo_od_%04d.dat", p )
#define NAVI_GROUP_INDOOR_NAME(b)			sprintf( b, "groupinfo_id.dat" )
#define NAVI_PROINFO_OUTDOOR_NAME(b, p)		sprintf( b, "propinfo_od_%04d.dat", p )
#define NAVI_PROINFO_INDOOR_NAME(b, p)		sprintf( b, "propinfo_id_%04d.dat.dat", p )


#endif