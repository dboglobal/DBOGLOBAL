//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef SHAPE_OVERLAPS_OBSTRUCTED_SPACE_INCLUDED
#define SHAPE_OVERLAPS_OBSTRUCTED_SPACE_INCLUDED

class cQueryContext;
class cLocalisedConvexShapeSet;
class iPointCollisionTester;
class iLineCollisionTester;

//... doesnt need to exclude the agent itself any more (so the parameters could be rationalised a bit)
bool
ShapeBoundaryIsClear(
        cQueryContext& qc,
        const cLocalisedConvexShapeSet& shapeSet,
        tSigned32 indexForShapeToTest,
        const iPointCollisionTester& basePointCollision,
        const iLineCollisionTester& baseLineCollision
        );

#endif

