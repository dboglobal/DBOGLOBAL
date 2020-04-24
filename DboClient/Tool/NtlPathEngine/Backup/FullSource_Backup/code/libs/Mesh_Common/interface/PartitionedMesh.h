//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef PARTITIONED_MESH_INCLUDED
#define PARTITIONED_MESH_INCLUDED

template <class tMesh>
class cPartitionedMesh
{
    tMesh _partitioned;

    // prevent copying and assignment
    cPartitionedMesh(const cPartitionedMesh&);
    const cPartitionedMesh& operator=(const cPartitionedMesh&);

public:

    cPartitionedMesh(tMesh& sourceMesh)
    {
    }


};

#endif
