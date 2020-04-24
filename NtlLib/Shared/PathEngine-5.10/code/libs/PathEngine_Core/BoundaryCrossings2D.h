//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Geometry/interface/tPoint_Header.h"
#include "libs/Geometry/interface/tLine_Header.h"

class cAxisFraction;

bool
LinesCross(const tPoint* line1, const tPoint* line2);

bool
LinesCross(const tLine& line1, const tLine& line2);

bool
ElementsPotentiallyCreateConcaveCorner(
               const tPoint* element1, bool element1HasConvexCorner,
               const tPoint* element2, bool element2HasConvexCorner,
               bool& atCorner1, bool& atCorner2);

enum
eLazyCornerType
{
    LCT_CONVEX,
    LCT_NONCONVEX_POINT,
    LCT_NONCONVEX_CROSSING,
};

//bool
//LineCrossesLazyBoundaryElement(
//               const tLine& elementLine,
//               eLazyCornerType startType,
//               const tPoint& pointBefore,
//               eLazyCornerType endType,
//               const tPoint& pointAfter,
//               const tLine& testLine
//               );

bool
LineCrossesBoundaryElement(
               const tLine& elementLine,
               const tLine& lineBefore,
               const tLine& lineAfter,
               const tLine& testLine
               );
bool
LineCrossesBoundaryElement(
               const tLine& elementLine,
               const tLine& lineBefore,
               const tLine& lineAfter,
               const tLine& testLine,
               bool& atStart, bool& atEnd
               );


bool
LineCrossesBoundaryElement(
               const tLine& elementLine,
               const tLine& lineBefore,
               const tLine& lineAfter,
               const tLine& testLine,
               const cAxisFraction& startFraction,
               const cAxisFraction& endFraction
               );
bool
LineCrossesBoundaryElement(
               const tLine& elementLine,
               const tLine& lineBefore,
               const tLine& lineAfter,
               const tLine& testLine,
               const cAxisFraction& startFraction,
               const cAxisFraction& endFraction,
               bool& atStart, bool& atEnd
               );


bool
HalfLineCrossesBoundaryElement(
               const tLine& elementLine,
               const tLine& lineBefore,
               const tLine& lineAfter,
               const tLine& testLine,
               bool& atStart, bool& atEnd
               );
bool
BiasedHalfLineCrossesBoundaryElement(
               const tLine& elementLine,
               const tLine& lineBefore,
               const tLine& lineAfter,
               const tLine& testLine,
               eSide bias,
               bool& atStart, bool& atEnd
               );

bool
InfiniteLineCrossesBoundaryElement(
               const tLine& elementLine,
               const tLine& lineBefore,
               const tLine& lineAfter,
               const tLine& testLine,
               bool& atStart, bool& atEnd
               );



