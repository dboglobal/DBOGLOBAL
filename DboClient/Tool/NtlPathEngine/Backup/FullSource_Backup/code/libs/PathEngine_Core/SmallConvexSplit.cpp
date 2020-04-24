//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "SmallConvexSplit.h"
#include "ShapeBoundaryIsClear.h"
#include "ElementLookupCollisionTester.h"
#include "interface/LocalisedConvexShapeSet.h"
#include "interface/LocalisedConvexShape.h"
#include "interface/CollisionAgainstShapeSet.h"
#include "interface/QueryContext.h"
#include "libs/Mesh2D/interface/AddInternalVertices.h"
#include "libs/Mesh2D/interface/TranslateToPartitioned.h"
#include "libs/Mesh_Common/interface/MeshWithTrackedTransform.h"
#include "libs/Instrumentation/interface/MemoryReport.h"
#include "common/Containers/FlagVector.h"
#include "common/interface/iXMLOutputStream.h"
#include "common/interface/iXMLInputStream.h"
#include "common/interface/Attributes.h"
#include "common/STL_Helpers.h"

void
cSmallConvexSplit::constructorCommon(
        cQueryContext& qc,
        const cLocalisedConvexShapeSet& baseObstacleExpansions
        )
{
    _smallConvexOnOriginal = new cLocalisedConvexShapeSet(qc.getNumberOfInternalFaces());
    for(tSigned32 i = 0; i != SizeL(baseObstacleExpansions); ++i)
    {
        if(_isSmallConvex[i])
        {
            _smallConvexOnOriginal->push_back(&baseObstacleExpansions[i]);
        }
    }
    _smallConvexOnOriginal->setCompleted();
    _nonSmallConvex = new cLocalisedConvexShapeSet(qc.getNumberOfInternalFaces());
    for(tSigned32 i = 0; i != SizeL(baseObstacleExpansions); ++i)
    {
        if(!_isSmallConvex[i])
        {
            const cLocalisedConvexShape& shape = baseObstacleExpansions[i];
            if(!shape.exitsMesh())
            {
                // check if this shape is contained by a small convex
                if(PointCollision_AgainstShapeSet(*_smallConvexOnOriginal, shape.centre()))
                {
                    continue;
                }
            }
            _nonSmallConvex->push_back(&baseObstacleExpansions[i]);
        }
    }
    _nonSmallConvex->setCompleted();

//    {
//        MEMORY_REPORT_SCOPE("cSmallConvexSplit partitioning");
//        _smallConvexPartitioned = new cMeshWithTrackedTransform<tMesh>(qc.refMesh(), qc.refIndex());
//        {
//            tSigned32 i;
////            for(i = 0; i < _smallConvexOnOriginal->size(); i += 15) //.. partitioning frequency hardwired here
//            for(i = 0; i < _smallConvexOnOriginal->size(); i += 60) //.. partitioning frequency hardwired here
//            {
//                cInternalPosition p = (*_smallConvexOnOriginal)[i].centre();
//                AddInternalVertex(*_smallConvexPartitioned, p.f, p.p);
//            }
//        }
//        _smallConvexPartitioned->splitLeavesAndCollapseIndices();
//    }

//    {
//        MEMORY_REPORT_SCOPE("cSmallConvexSplit small convex on partitioned");
//        {
//            _smallConvexOnPartitioned = new cLocalisedConvexShapeSet(_smallConvexPartitioned->index().internalFaces());
//
//            cFlagVector faceFlagsBuffer(_smallConvexPartitioned->index().internalFaces());
//            std::vector<tFace> faceBuffer(_smallConvexPartitioned->index().internalFaces());
//
//            for(tSigned32 i = 0; i != _smallConvexOnOriginal->size(); ++i)
//            {
//                const cLocalisedConvexShape& s = (*_smallConvexOnOriginal)[i];
//                cInternalPosition pPartitioned = TranslateToPartitioned(*_smallConvexPartitioned, s.centre());
//                cLocalisedConvexShape* lcs = new cLocalisedConvexShape(s, pPartitioned, &faceBuffer.front(), faceFlagsBuffer);
//                _ownedShapes.push_back(lcs);
//                _smallConvexOnPartitioned->push_back(lcs);
//            }
//            _smallConvexOnPartitioned->setCompleted();
//        }
//    }

    _smallConvexOnOriginal->shrinkToFit();
    _nonSmallConvex->shrinkToFit();
    ShrinkToFit(_ownedShapes);
//    _smallConvexOnPartitioned->shrinkToFit();
}

cSmallConvexSplit::cSmallConvexSplit(
        cQueryContext& qc,
        const cLocalisedConvexShapeSet& baseObstacleExpansions,
        const cElementLookupCollisionTester& edgeExpansionCollisionTester,
        tSigned32 maxCircumference
        )
{
    _isSmallConvex.resize(baseObstacleExpansions.size(), false);
    for(tSigned32 i = 0; i != SizeL(baseObstacleExpansions); ++i)
    {
        const cLocalisedConvexShape& shape = baseObstacleExpansions[i];
        if(shape.exitsMesh())
        {
            continue;
        }
        if(!shape.poly().satisfiesMaxCircumference(maxCircumference))
        {
            continue;
        }
        if(!ShapeBoundaryIsClear(qc, baseObstacleExpansions, i, edgeExpansionCollisionTester, edgeExpansionCollisionTester))
        {
            continue;
        }
        _isSmallConvex.setBit(i, true);
    }

    constructorCommon(qc, baseObstacleExpansions);
}


cSmallConvexSplit::cSmallConvexSplit(
        iXMLInputStream& is,
        cQueryContext& qc,
        const cLocalisedConvexShapeSet& baseObstacleExpansions
        )
{
    _isSmallConvex.resize(baseObstacleExpansions.size(), false);
    const char* nextElement;
    do
    {
        nextElement = is.peekNextElement();
        if(nextElement && strcmp(nextElement, "isSmallConvex") == 0)
        {
            const char *const* attributes = is.openNextElement();
            tSigned32 index = ValueAsLong(attributes, "index");
            assertD(!_isSmallConvex[index]);
            _isSmallConvex.setBit(index, true);
            is.closeElement();
        }
        else if(nextElement)
        {
            is.skipNextElement();
        }
    }
    while(nextElement);

    constructorCommon(qc, baseObstacleExpansions);
}

cSmallConvexSplit::~cSmallConvexSplit()
{
    delete _smallConvexOnOriginal;
    delete _nonSmallConvex;
//    delete _smallConvexPartitioned;
//    delete _smallConvexOnPartitioned;
    tSigned32 i;
    for(i = 0; i < SizeL(_ownedShapes); ++i)
    {
        delete _ownedShapes[i];
    }
}

void
cSmallConvexSplit::save(iXMLOutputStream& os) const
{
    tSigned32 i;
    for(i = 0; i != SizeL(_isSmallConvex); ++i)
    {
        if(_isSmallConvex[i])
        {
            os.openElement("isSmallConvex");
            os.addAttribute("index", i);
            os.closeElement("isSmallConvex");
        }
    }
}

const cMeshWithTrackedTransform<tMesh>*
cSmallConvexSplit::getPartitioned() const
{
//    return _smallConvexPartitioned;
    return 0;
}

tFace
cSmallConvexSplit::translateToPartitioned(tFace f, const tPoint& p) const
{
//    return TranslateToPartitioned(*_smallConvexPartitioned, f, p);
    return f;
}

const cLocalisedConvexShapeSet&
cSmallConvexSplit::refSmallConvexOnPartitioned() const
{
//    return *_smallConvexOnPartitioned;
    return *_smallConvexOnOriginal;
}


const cLocalisedConvexShapeSet& 
cSmallConvexSplit::refSmallConvexOnOriginal() const
{
    return *_smallConvexOnOriginal;
}
const cLocalisedConvexShapeSet& 
cSmallConvexSplit::refNonSmallConvex() const
{
    return *_nonSmallConvex;
}

