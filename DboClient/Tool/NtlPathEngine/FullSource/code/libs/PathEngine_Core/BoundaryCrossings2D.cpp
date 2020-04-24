//**********************************************************************
//
// Copyright (c) 2004-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "BoundaryCrossings2D.h"
#include "libs/Geometry/interface/tPoint_Multiplication.h"
#include "libs/Geometry/interface/tLine.h"
#include "libs/Geometry/interface/AxisFraction.h"
#include "libs/Geometry/interface/tIntersection.h"

bool
LinesCross(const tPoint* line1, const tPoint* line2)
{
// check for lines crossing
    eSide side1, side2;
    side1 = tLine(line1[0], line1[1]).side(line2[0]);
    if(side1 == SIDE_CENTRE)
    {
        return false;
    }
    side2 = tLine(line1[0], line1[1]).side(line2[1]);
    if(side2 == SIDE_CENTRE || side2 == side1)
    {
        return false;
    }
    side1 = tLine(line2[0], line2[1]).side(line1[0]);
    if(side1 == SIDE_CENTRE)
    {
        return false;
    }
    side2 = tLine(line2[0], line2[1]).side(line1[1]);
    if(side2 == SIDE_CENTRE || side2 == side1)
    {
        return false;
    }
    return true;
}
bool
LinesCross(const tLine& line1, const tLine& line2)
{
// check for lines crossing
    eSide side1, side2;
    side1 = line1.side(line2.start());
    if(side1 == SIDE_CENTRE)
    {
        return false;
    }
    side2 = line1.side(line2.end());
    if(side2 == SIDE_CENTRE || side2 == side1)
    {
        return false;
    }
    side1 = line2.side(line1.start());
    if(side1 == SIDE_CENTRE)
    {
        return false;
    }
    side2 = line2.side(line1.end());
    if(side2 == SIDE_CENTRE || side2 == side1)
    {
        return false;
    }
    return true;
}


bool
ElementsPotentiallyCreateConcaveCorner(
               const tPoint* element1, bool element1HasConvexCorner,
               const tPoint* element2, bool element2HasConvexCorner,
               bool& atCorner1, bool& atCorner2)
{
    if(LinesCross(element1 + 1, element2 + 1))
    {
        atCorner1 = false;
        atCorner2 = false;
        return true;
    }
    if(element1HasConvexCorner && element2HasConvexCorner && element1[0] == element2[0])
    {
      // two corners coincide
      // test for one corner actually exiting the other
        eSide side = tLine(element1[0], element1[1]).side(element2[0]);
        if(side == SIDE_LEFT)
        {
          // check for element1 exiting element2
            if(tLine(element1[0], element1[1]).side(element2[2]) == SIDE_RIGHT
             && tLine(element1[1], element1[2]).side(element2[2]) == SIDE_RIGHT)
            {
                atCorner1 = true;
                atCorner2 = true;
                return true;
            }
        }
        else
        if(side == SIDE_RIGHT)
        {
          // check for element2 exiting element1
            if(tLine(element2[0], element2[1]).side(element1[2]) == SIDE_RIGHT
             && tLine(element2[1], element2[2]).side(element1[2]) == SIDE_RIGHT)
            {
                atCorner1 = true;
                atCorner2 = true;
                return true;
            }
        }
    }
    if(element1HasConvexCorner && tLine(element2[1], element2[2]).side(element1[1]) == SIDE_CENTRE)
    {
      // check for element2 line exiting from element1 corner
        if(tLine(element1[0], element1[1]).side(element2[1]) == SIDE_RIGHT
         && tLine(element1[1], element1[2]).side(element2[1]) == SIDE_RIGHT)
        {
          // line comes from inside corner
            if(tLine(element1[0], element1[1]).side(element2[2]) == SIDE_LEFT)
            {
                atCorner1 = true;
                atCorner2 = false;
                return true;
            }
        }
    }
    if(element2HasConvexCorner && tLine(element1[1], element1[2]).side(element2[1]) == SIDE_CENTRE)
    {
      // check for element1 line exiting from element2 corner
        if(tLine(element2[0], element2[1]).side(element1[1]) == SIDE_RIGHT
         && tLine(element2[1], element2[2]).side(element1[1]) == SIDE_RIGHT)
        {
          // line comes from inside corner
            if(tLine(element2[0], element2[1]).side(element1[2]) == SIDE_LEFT)
            {
                atCorner1 = false;
                atCorner2 = true;
                return true;
            }
        }
    }
    return false;
}

//bool
//LineCrossesLazyBoundaryElement(
//                const tLine& elementLine,
//                eLazyCornerType startType,
//                const tPoint& pointBefore,
//                eLazyCornerType endType,
//                const tPoint& pointAfter,
//                const tLine& testLine
//                )
//{
//    eSide side;
//    side = elementLine.side(testLine.start());
//    if(side == SIDE_RIGHT)
//    {
//        return false;
//    }
//    side = elementLine.side(testLine.end());
//    if(side != SIDE_RIGHT)
//    {
//        return false;
//    }
//    side = testLine.side(elementLine.start());
//    if(side != SIDE_RIGHT)
//    {
//        if(side == SIDE_LEFT)
//        {
//            return false;
//        }
//        if(startType == LCT_NONCONVEX_POINT)
//        {
//            return true;
//        }
//        if(startType == LCT_NONCONVEX_CROSSING)
//        {
//            return false;
//        }
//        return testLine.side(pointBefore) == SIDE_RIGHT;
//    }
//    side = testLine.side(elementLine.end());
//    if(side != SIDE_LEFT)
//    {
//        if(side == SIDE_RIGHT)
//        {
//            return false;
//        }
//        if(endType == LCT_NONCONVEX_POINT)
//        {
//            return true;
//        }
//        if(endType == LCT_NONCONVEX_CROSSING)
//        {
//            return false;
//        }
//        return testLine.side(pointAfter) == SIDE_LEFT;
//    }
//    return true;
//}

bool
LineCrossesBoundaryElement(
               const tLine& elementLine,
               const tLine& lineBefore,
               const tLine& lineAfter,
               const tLine& testLine
               )
{
    eSide side;
    side = elementLine.side(testLine.start());
    if(side == SIDE_RIGHT)
    {
        return false;
    }
    side = elementLine.side(testLine.end());
    if(side != SIDE_RIGHT)
    {
        return false;
    }
    if(elementLine.start() == lineBefore.end())
    {
        side = testLine.side(elementLine.start());
    }
    else
    {
        assertD(lineBefore.side(elementLine) == SIDE_LEFT);
        side = elementLine.intersectionOrder_Clockwise(testLine, lineBefore);
    }    
    if(side != SIDE_RIGHT)
    {
        if(side == SIDE_LEFT)
        {
            return false;
        }
        if(lineBefore.side(elementLine) == SIDE_LEFT)
        {
            return true;
        }
        return lineBefore.side(testLine) == SIDE_RIGHT;
    }
    if(elementLine.end() == lineAfter.start())
    {
        side = testLine.side(elementLine.end());
    }
    else
    {
        assertD(elementLine.side(lineAfter) == SIDE_LEFT);
        side = elementLine.intersectionOrder_Clockwise_Anticlockwise(testLine, lineAfter);
    }    
    if(side != SIDE_LEFT)
    {
        if(side == SIDE_RIGHT)
        {
            return false;
        }
        if(elementLine.side(lineAfter) == SIDE_LEFT)
        {
            return true;
        }
        return lineAfter.side(testLine) == SIDE_RIGHT;
    }
    return true;
}

bool
LineCrossesBoundaryElement(
               const tLine& elementLine,
               const tLine& lineBefore,
               const tLine& lineAfter,
               const tLine& testLine,
               bool& atStart, bool& atEnd
               )
{
    eSide side;
    side = elementLine.side(testLine.start());
    if(side == SIDE_RIGHT)
    {
        return false;
    }
    side = elementLine.side(testLine.end());
    if(side != SIDE_RIGHT)
    {
        return false;
    }
    if(elementLine.start() == lineBefore.end())
    {
        side = testLine.side(elementLine.start());
    }
    else
    {
        assertD(lineBefore.side(elementLine) == SIDE_LEFT);
        side = elementLine.intersectionOrder_Clockwise(testLine, lineBefore);
    }    
    if(side != SIDE_RIGHT)
    {
        if(side == SIDE_LEFT)
        {
            return false;
        }
        atStart = true;
        atEnd = false;
        if(lineBefore.side(elementLine) == SIDE_LEFT)
        {
            return true;
        }
        return lineBefore.side(testLine) == SIDE_RIGHT;
    }
    if(elementLine.end() == lineAfter.start())
    {
        side = testLine.side(elementLine.end());
    }
    else
    {
        assertD(elementLine.side(lineAfter) == SIDE_LEFT);
        side = elementLine.intersectionOrder_Clockwise_Anticlockwise(testLine, lineAfter);
    }    
    if(side != SIDE_LEFT)
    {
        if(side == SIDE_RIGHT)
        {
            return false;
        }
        atStart = false;
        atEnd = true;
        if(elementLine.side(lineAfter) == SIDE_LEFT)
        {
            return true;
        }
        return lineAfter.side(testLine) == SIDE_RIGHT;
    }
    atStart = false;
    atEnd = false;
    return true;
}


bool
LineCrossesBoundaryElement(
               const tLine& elementLine,
               const tLine& lineBefore,
               const tLine& lineAfter,
               const tLine& testLine,
               const cAxisFraction& startFraction,
               const cAxisFraction& endFraction
               )
{
    {
        if(elementLine.side(testLine) != SIDE_RIGHT)
        {
            return false;
        }
        tIntersection intersection(testLine, elementLine);
        cAxisFraction intersectionFraction;
        intersection.getAxisFraction(intersectionFraction.n, intersectionFraction.d);
//        side = elementLine.side(testLine.start());
//        if(side == SIDE_RIGHT)
//        {
//            return false;
//        }
//        side = elementLine.side(testLine.end());
//        if(side != SIDE_RIGHT)
//        {
//            return false;
//        }
        if(startFraction.compare(intersectionFraction) == -1)
        {
            return false;
        }
        if(endFraction.compare(intersectionFraction) != -1)
        {
            return false;
        }
    }

    eSide side;
    if(elementLine.start() == lineBefore.end())
    {
        side = testLine.side(elementLine.start());
    }
    else
    {
        assertD(lineBefore.side(elementLine) == SIDE_LEFT);
        side = elementLine.intersectionOrder_Clockwise(testLine, lineBefore);
    }    
    if(side != SIDE_RIGHT)
    {
        if(side == SIDE_LEFT)
        {
            return false;
        }
        if(lineBefore.side(elementLine) == SIDE_LEFT)
        {
            return true;
        }
        return lineBefore.side(testLine) == SIDE_RIGHT;
    }
    if(elementLine.end() == lineAfter.start())
    {
        side = testLine.side(elementLine.end());
    }
    else
    {
        assertD(elementLine.side(lineAfter) == SIDE_LEFT);
        side = elementLine.intersectionOrder_Clockwise_Anticlockwise(testLine, lineAfter);
    }    
    if(side != SIDE_LEFT)
    {
        if(side == SIDE_RIGHT)
        {
            return false;
        }
        if(elementLine.side(lineAfter) == SIDE_LEFT)
        {
            return true;
        }
        return lineAfter.side(testLine) == SIDE_RIGHT;
    }
    return true;
}

bool
LineCrossesBoundaryElement(
               const tLine& elementLine,
               const tLine& lineBefore,
               const tLine& lineAfter,
               const tLine& testLine,
               const cAxisFraction& startFraction,
               const cAxisFraction& endFraction,
               bool& atStart, bool& atEnd
               )
{
    {
        if(elementLine.side(testLine) != SIDE_RIGHT)
        {
            return false;
        }
        tIntersection intersection(testLine, elementLine);
        cAxisFraction intersectionFraction;
        intersection.getAxisFraction(intersectionFraction.n, intersectionFraction.d);
//        side = elementLine.side(testLine.start());
//        if(side == SIDE_RIGHT)
//        {
//            return false;
//        }
//        side = elementLine.side(testLine.end());
//        if(side != SIDE_RIGHT)
//        {
//            return false;
//        }
        if(startFraction.compare(intersectionFraction) == -1)
        {
            return false;
        }
        if(endFraction.compare(intersectionFraction) != -1)
        {
            return false;
        }
    }
    eSide side;
    if(elementLine.start() == lineBefore.end())
    {
        side = testLine.side(elementLine.start());
    }
    else
    {
        assertD(lineBefore.side(elementLine) == SIDE_LEFT);
        side = elementLine.intersectionOrder_Clockwise(testLine, lineBefore);
    }    
    if(side != SIDE_RIGHT)
    {
        if(side == SIDE_LEFT)
        {
            return false;
        }
        atStart = true;
        atEnd = false;
        if(lineBefore.side(elementLine) == SIDE_LEFT)
        {
            return true;
        }
        return lineBefore.side(testLine) == SIDE_RIGHT;
    }
    if(elementLine.end() == lineAfter.start())
    {
        side = testLine.side(elementLine.end());
    }
    else
    {
        assertD(elementLine.side(lineAfter) == SIDE_LEFT);
        side = elementLine.intersectionOrder_Clockwise_Anticlockwise(testLine, lineAfter);
    }    
    if(side != SIDE_LEFT)
    {
        if(side == SIDE_RIGHT)
        {
            return false;
        }
        atStart = false;
        atEnd = true;
        if(elementLine.side(lineAfter) == SIDE_LEFT)
        {
            return true;
        }
        return lineAfter.side(testLine) == SIDE_RIGHT;
    }
    atStart = false;
    atEnd = false;
    return true;
}


//........... debug crosscheck
//bool
//LineCrossesBoundaryElement(
//               const tLine& elementLine,
//               const tLine& lineBefore,
//               const tLine& lineAfter,
//               const tLine& testLine
//               )
//{
//    bool result1 = LineCrossesBoundaryElement_Real(elementLine, lineBefore, lineAfter, testLine);
//    cAxisFraction start, end;
//    start.n = tPoint::tDistance(0);
//    start.d = tPoint::tDistance(1);
//    end.n = tPoint::tDistance(1);
//    end.d = tPoint::tDistance(1);
//    bool result2 = LineCrossesBoundaryElement_Real(elementLine, lineBefore, lineAfter, testLine);
//    assertR(result1 == result2);
//    return result1;
//}
//bool
//LineCrossesBoundaryElement(
//               const tLine& elementLine,
//               const tLine& lineBefore,
//               const tLine& lineAfter,
//               const tLine& testLine,
//               bool& atStart, bool& atEnd
//               )
//{
//    bool result1 = LineCrossesBoundaryElement_Real(elementLine, lineBefore, lineAfter, testLine, atStart, atEnd);
//    cAxisFraction start, end;
//    start.n = tPoint::tDistance(0);
//    start.d = tPoint::tDistance(1);
//    end.n = tPoint::tDistance(1);
//    end.d = tPoint::tDistance(1);
//    bool result2 = LineCrossesBoundaryElement_Real(elementLine, lineBefore, lineAfter, testLine, atStart, atEnd);
//    assertR(result1 == result2);
////.... doesnt bother checking atStart, atEnd
//    return result1;
//}


bool
HalfLineCrossesBoundaryElement(
               const tLine& elementLine,
               const tLine& lineBefore,
               const tLine& lineAfter,
               const tLine& testLine,
               bool& atStart, bool& atEnd
               )
{
    eSide side;
    side = elementLine.side(testLine.start());
    if(side == SIDE_RIGHT)
    {
        return false;
    }
    side = elementLine.side(testLine);
    if(side != SIDE_RIGHT)
    {
        return false;
    }
    if(elementLine.start() == lineBefore.end())
    {
        side = testLine.side(elementLine.start());
    }
    else
    {
        assertD(lineBefore.side(elementLine) == SIDE_LEFT);
        side = elementLine.intersectionOrder_Clockwise(testLine, lineBefore);
    }    
    if(side != SIDE_RIGHT)
    {
        if(side == SIDE_LEFT)
        {
            return false;
        }
        atStart = true;
        atEnd = false;
        if(lineBefore.side(elementLine) == SIDE_LEFT)
        {
            return true;
        }
        return lineBefore.side(testLine) == SIDE_RIGHT;
    }
    if(elementLine.end() == lineAfter.start())
    {
        side = testLine.side(elementLine.end());
    }
    else
    {
        assertD(elementLine.side(lineAfter) == SIDE_LEFT);
        side = elementLine.intersectionOrder_Clockwise_Anticlockwise(testLine, lineAfter);
    }    
    if(side != SIDE_LEFT)
    {
        if(side == SIDE_RIGHT)
        {
            return false;
        }
        atStart = false;
        atEnd = true;
        if(elementLine.side(lineAfter) == SIDE_LEFT)
        {
            return true;
        }
        return lineAfter.side(testLine) == SIDE_RIGHT;
    }
    atStart = false;
    atEnd = false;
    return true;
}

bool
BiasedHalfLineCrossesBoundaryElement(
               const tLine& elementLine,
               const tLine& lineBefore,
               const tLine& lineAfter,
               const tLine& testLine,
               eSide bias,
               bool& atStart, bool& atEnd
               )
{
    if(bias == SIDE_CENTRE)
    {
        return HalfLineCrossesBoundaryElement(elementLine, lineBefore, lineAfter, testLine, atStart, atEnd);
    }
    eSide side;
    side = elementLine.side(testLine.start());
    if(side == SIDE_RIGHT)
    {
        return false;
    }
    side = elementLine.side(testLine);
    if(side != SIDE_RIGHT)
    {
        return false;
    }
    if(elementLine.start() == lineBefore.end())
    {
        side = testLine.side(elementLine.start());
    }
    else
    {
        assertD(lineBefore.side(elementLine) == SIDE_LEFT);
        side = elementLine.intersectionOrder_Clockwise(testLine, lineBefore);
    }    
    if(side != SIDE_RIGHT)
    {
        if(side == SIDE_LEFT)
        {
            return false;
        }
        atStart = false;
        atEnd = false;
        return bias == SIDE_LEFT;
    }
    if(elementLine.end() == lineAfter.start())
    {
        side = testLine.side(elementLine.end());
    }
    else
    {
        assertD(elementLine.side(lineAfter) == SIDE_LEFT);
        side = elementLine.intersectionOrder_Clockwise_Anticlockwise(testLine, lineAfter);
    }    
    if(side != SIDE_LEFT)
    {
        if(side == SIDE_RIGHT)
        {
            return false;
        }
        atStart = false;
        atEnd = false;
        return bias == SIDE_RIGHT;
    }
    atStart = false;
    atEnd = false;
    return true;
}

bool
InfiniteLineCrossesBoundaryElement(
               const tLine& elementLine,
               const tLine& lineBefore,
               const tLine& lineAfter,
               const tLine& testLine,
               bool& atStart, bool& atEnd
               )
{
    eSide side;
    side = elementLine.side(testLine);
    if(side != SIDE_RIGHT)
    {
        return false;
    }
    if(elementLine.start() == lineBefore.end())
    {
        side = testLine.side(elementLine.start());
    }
    else
    {
        assertD(lineBefore.side(elementLine) == SIDE_LEFT);
        side = elementLine.intersectionOrder_Clockwise(testLine, lineBefore);
    }    
    if(side != SIDE_RIGHT)
    {
        if(side == SIDE_LEFT)
        {
            return false;
        }
        atStart = true;
        atEnd = false;
        if(lineBefore.side(elementLine) == SIDE_LEFT)
        {
            return true;
        }
        return lineBefore.side(testLine) == SIDE_RIGHT;
    }
    if(elementLine.end() == lineAfter.start())
    {
        side = testLine.side(elementLine.end());
    }
    else
    {
        assertD(elementLine.side(lineAfter) == SIDE_LEFT);
        side = elementLine.intersectionOrder_Clockwise_Anticlockwise(testLine, lineAfter);
    }    
    if(side != SIDE_LEFT)
    {
        if(side == SIDE_RIGHT)
        {
            return false;
        }
        atStart = false;
        atEnd = true;
        if(elementLine.side(lineAfter) == SIDE_LEFT)
        {
            return true;
        }
        return lineAfter.side(testLine) == SIDE_RIGHT;
    }
    atStart = false;
    atEnd = false;
    return true;
}
