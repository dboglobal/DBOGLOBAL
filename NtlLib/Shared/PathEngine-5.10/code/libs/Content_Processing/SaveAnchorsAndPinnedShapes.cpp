//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "SaveAnchorsAndPinnedShapes.h"
#include "platform_common/FloatPersistence.h"
#include "common/interface/Assert.h"
#include "common/interface/iXMLOutputStream.h"
#include "i_pathengine.h"
#include "common/STL/sstream.h"

void
SaveAnchorsAndPinnedShapes(
        const iAnchorsAndPinnedShapes& toSave,
        iXMLOutputStream& xos
        )
{
    xos.openElement("anchorsAndPinnedShapes");
    if(toSave.numberOfAnchors() == 0)
    {
        xos.closeElement("anchorsAndPinnedShapes");
        return;
    }
    xos.openElement("anchors");
    tSigned32 i;
    for(i = 0; i != toSave.numberOfAnchors(); ++i)
    {
        xos.openElement("anchor");
        if(toSave.anchor_Name(i)[0])
        {
            xos.addAttribute("name", toSave.anchor_Name(i));
        }
        {
            tSigned32 x, y;
            float zStart, zEnd;
            toSave.anchor_HorizontalPosition(i, x, y);
            toSave.anchor_VerticalRange(i, zStart, zEnd);
            xos.addAttribute("x", x);
            xos.addAttribute("y", y);
            {
                std::string persistent = FloatToPersistent(zStart);
                xos.addAttribute("zStart", persistent.c_str());    
            }
            {
                std::string persistent = FloatToPersistent(zEnd);
                xos.addAttribute("zEnd", persistent.c_str());
            }
        }
        xos.addAttribute("orientation", toSave.anchor_Orientation(i));
        assertC(PE_AnchorAttribute_Top == 0);
        if(toSave.anchor_Attribute(i, PE_AnchorAttribute_ResolvesToTerrain) != 0)
        {
            xos.addAttribute("_resolvesToTerrain", toSave.anchor_Attribute(i, PE_AnchorAttribute_ResolvesToTerrain));
        }
        xos.closeElement("anchor");
    }
    xos.closeElement("anchors");

    if(toSave.numberOfPinnedShapes())
    {
        xos.openElement("pinnedShapes");
        for(i = 0; i != toSave.numberOfPinnedShapes(); ++i)
        {
            xos.openElement("pinnedShape");
            if(toSave.pinnedShape_Name(i)[0])
            {
                xos.addAttribute("name", toSave.pinnedShape_Name(i));
            }
            xos.addAttribute("anchorIndex", toSave.pinnedShape_AnchorIndex(i));
            {
                std::ostringstream oss;
                tSigned32 j;
                for(j = 0; j != toSave.pinnedShape_NumberOfVertices(i); ++j)
                {
                    if(j)
                    {
                        oss << ',';
                    }
                    tSigned32 x, y;
                    toSave.pinnedShape_Vertex(i, j, x, y);
                    oss << x << ',' << y;
                }
                xos.addAttribute("vertices", oss.str().c_str());
            }
            assertC(PE_PinnedShapeAttribute_Top == 0);
            if(toSave.pinnedShape_Attribute(i, PE_PinnedShapeAttribute_Type) != PE_PinnedShapeType_BurntIn)
            {
                xos.addAttribute("_type", toSave.pinnedShape_Attribute(i, PE_PinnedShapeAttribute_Type));
            }
            xos.closeElement("pinnedShape");
        }
        xos.closeElement("pinnedShapes");
    }
    xos.closeElement("anchorsAndPinnedShapes");
}
