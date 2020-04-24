#ifndef _NTL_NAVI_MATH_H_
#define _NTL_NAVI_MATH_H_


//////////////////////////////////////////////////////////////////////////
//
//	Vector3
//
//////////////////////////////////////////////////////////////////////////


class CNaviVec3
{
public:
	float x, y, z;

public:
	CNaviVec3( void )
	{
	}

	CNaviVec3( const CNaviVec3& clVal )
	{
		this->x = clVal.x;
		this->y = clVal.y;
		this->z = clVal.z;
	}

	CNaviVec3( float x, float y, float z )
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	CNaviVec3& operator = ( const CNaviVec3& clVal )
	{
		this->x = clVal.x;
		this->y = clVal.y;
		this->z = clVal.z;

		return *this;
	}

public:
	float Length( void )
	{
		return sqrtf( x * x + y * y + z * z );
	}

	float LengthXZ( void )
	{
		return sqrtf( x * x + z * z );
	}

	float Distance( const CNaviVec3& clVal )
	{
		return sqrtf( this->x * clVal.x + this->y * clVal.y + this->z * clVal.z );
	}

	float DistanceXZ( const CNaviVec3& clVal )
	{
		return sqrtf( this->x * clVal.x + this->z * clVal.z );
	}
};


//////////////////////////////////////////////////////////////////////////
//
//	AABB
//
//////////////////////////////////////////////////////////////////////////

class CNaviAABB
{
public:
	CNaviVec3 minPos, maxPos;

public:
	CNaviAABB( void )
	{
	}

	CNaviAABB( const CNaviAABB& clVal )
	{
		this->minPos = clVal.minPos;
		this->maxPos = clVal.maxPos;
	}

	CNaviAABB( const CNaviVec3& clPos1, const CNaviVec3& clPos2 )
	{
		this->minPos.x = clPos1.x < clPos2.x ? clPos1.x : clPos2.x;
		this->minPos.y = clPos1.y < clPos2.y ? clPos1.y : clPos2.y;
		this->minPos.z = clPos1.z < clPos2.z ? clPos1.z : clPos2.z;

		this->maxPos.x = clPos1.x > clPos2.x ? clPos1.x : clPos2.x;
		this->maxPos.y = clPos1.y > clPos2.y ? clPos1.y : clPos2.y;
		this->maxPos.z = clPos1.z > clPos2.z ? clPos1.z : clPos2.z;
	}

	CNaviAABB& operator = ( const CNaviAABB& clVal )
	{
		this->minPos = clVal.minPos;
		this->maxPos = clVal.maxPos;

		return *this;
	}

public:
	CNaviVec3 GetCenter( void )
	{
		return CNaviVec3( this->minPos.x + (this->minPos.x + this->maxPos.x) * 0.5f,
						  this->minPos.y + (this->minPos.y + this->maxPos.y) * 0.5f,
						  this->minPos.z + (this->minPos.z + this->maxPos.z) * 0.5f );
	}

	void SetData( const CNaviVec3& clPos1, const CNaviVec3& clPos2 )
	{
		this->minPos.x = clPos1.x < clPos2.x ? clPos1.x : clPos2.x;
		this->minPos.y = clPos1.y < clPos2.y ? clPos1.y : clPos2.y;
		this->minPos.z = clPos1.z < clPos2.z ? clPos1.z : clPos2.z;

		this->maxPos.x = clPos1.x > clPos2.x ? clPos1.x : clPos2.x;
		this->maxPos.y = clPos1.y > clPos2.y ? clPos1.y : clPos2.y;
		this->maxPos.z = clPos1.z > clPos2.z ? clPos1.z : clPos2.z;
	}
};


#endif