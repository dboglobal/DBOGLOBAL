//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

class iAnchorsAndPinnedShapes;
class MObject;

void
ImportAnchorsAndShapes_Maya(
        const iAnchorsAndPinnedShapes& anchorsAndShapes,
        float scale,
        MObject& linkUnder,
        float anchorDirectionIndicatorLength
        );
