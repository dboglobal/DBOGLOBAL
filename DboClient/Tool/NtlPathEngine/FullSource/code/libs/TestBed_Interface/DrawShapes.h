//**********************************************************************
//
// Copyright (c) 2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef DRAW_SHAPES_INCLUDED
#define DRAW_SHAPES_INCLUDED

class cLocalisedConvexShapeSet;
class iGraphicsInterface;
class cTranslationToOverlapped;

void
DrawShapes(
        const cLocalisedConvexShapeSet& shapes,
        iGraphicsInterface* gip,
        const cTranslationToOverlapped& translation
        );

#endif
