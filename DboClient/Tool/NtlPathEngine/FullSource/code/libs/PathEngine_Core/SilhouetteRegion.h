//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef SILHOUETTE_REGION_INCLUDED
#define SILHOUETTE_REGION_INCLUDED

#include "SilhouetteZone.h"
#include "libs/Mesh2D/interface/tMesh_Header.h"
#include "common/STL/vector.h"
#include "common/STL/utility.h"
#include "common/STL/list.h"

class cCircuitElement;
class cMeshCut;
class cElementInFaceEntry;
class cCornerInFaceEntry;
class cFlagVector;
class cInternalPosition;

namespace nSilhouetteRegion
{

typedef std::pair<eSilhouetteZone, const cCircuitElement*> tTarget;
typedef std::list<tTarget> tTargetList;
typedef std::pair<eSilhouetteZone, tSigned32> tIndexedTarget;
typedef std::list<tIndexedTarget> tIndexedTargetList;

void
build(const std::vector<std::vector<cElementInFaceEntry> >& lookup,
      cFlagVector& elementReferencedFlags,
      const std::vector<std::vector<cCornerInFaceEntry> >& cornerLookup,
      const cCircuitElement& corner, eSilhouetteZone zone,
      cFlagVector& facesReferenced,
      cMeshCut& result
      );

void
build_Unanchored(const std::vector<std::vector<cElementInFaceEntry> >& lookup,
      cFlagVector& elementReferencedFlags,
      const std::vector<std::vector<cCornerInFaceEntry> >& cornerLookup,
      const cInternalPosition& corner, const tPoint& axisBefore, const tPoint& axisAfter,
      eSilhouetteZone zone,
      cFlagVector& facesReferenced,
      cMeshCut& result
      );

void
build_360(const std::vector<std::vector<cElementInFaceEntry> >& lookup,
      cFlagVector& elementReferencedFlags,
      const std::vector<std::vector<cCornerInFaceEntry> >& cornerLookup,
      const cInternalPosition& centre,
      cFlagVector& facesReferenced,
      cMeshCut& result
      );

}

#endif
