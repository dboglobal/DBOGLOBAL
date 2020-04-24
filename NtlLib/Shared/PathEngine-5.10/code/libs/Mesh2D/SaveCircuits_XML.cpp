//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "interface/SaveCircuits_XML.h"
#include "common/interface/iXMLOutputStream.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "libs/Mesh2D/interface/tMesh.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "common/STL/vector.h"
#include "common/STL/sstream.h"
#include "platform_common/PointMultiplication.h"

void SaveMeshCuts_XML_Prologue(iXMLOutputStream& os)
{
    os.openElement("cuts");
}
void SaveMeshCut_XML(const cMeshCut& cut, iXMLOutputStream& os)
{
//    os.openElement("cut");

    if(!cut.getStartElement()->getPrev())
    {
        os.addAttribute("unconnected", "true");
    }

    if(cut.isInternalLoop())
    {
        os.addAttribute("start_face", cut.startFace().index());
    }
    else if(cut.getStartElement()->enters())
    {
        tEdge entry = cut.getStartElement()->entryEdge();
        os.addAttribute("entry", entry.index());
    }
    else
    {
        assertD(!cut.getStartElement()->getPrev());
        os.addAttribute("start_face", cut.startFace().index());
        const tLine& startCap = cut.getStartElement()->refZeroLengthLine();
        std::ostringstream oss;
		oss << startCap.start().getX() << ",";
		oss << startCap.start().getY() << ",";
		oss << startCap.end().getX() << ",";
		oss << startCap.end().getY();
        os.addAttribute("startCap", oss.str().c_str());
    }

    const cCircuitElement* e = cut.getStartElement();
    do
    {
        os.openElement("element");
        const tLine& l = e->refLine();
        std::ostringstream oss;
        oss << l.start().getX() << ",";
        oss << l.start().getY() << ",";
        oss << l.end().getX() << ",";
        oss << l.end().getY();
        os.addAttribute("line", oss.str().c_str());
        if(!e->exits())
		{
		    if(!e->getNext())
            {
	            const tLine& endCap = e->refEndCapLine();
                std::ostringstream oss;
			    oss << endCap.start().getX() << ",";
			    oss << endCap.start().getY() << ",";
			    oss << endCap.end().getX() << ",";
			    oss << endCap.end().getY();
                os.addAttribute("endCap", oss.str().c_str());
            }
            else
            {
	            const tLine& nextL = e->getNext()->refLine();
			    if(l.side(nextL) == SIDE_CENTRE)
			    {
				    assertD(l.direction(nextL) == DIRECTION_BACK);
				    const tLine& intersect = e->getEndIntersect_DoesntExit();
				    assertD(l.side(intersect) == SIDE_RIGHT);
                    std::ostringstream oss;
				    oss << intersect.start().getX() << ",";
				    oss << intersect.start().getY() << ",";
				    oss << intersect.end().getX() << ",";
				    oss << intersect.end().getY();
                    os.addAttribute("intersect", oss.str().c_str());
			    }
            }
		}
        os.closeElement("element");
        e = e->getNext();
    }
    while(e && e != cut.getStartElement());

//    os.closeElement("cut");
}
void SaveMeshCuts_XML_Epilogue(iXMLOutputStream& os)
{
    os.closeElement("cuts");
}

void
SaveCutSequence_XML(const tMeshCutSequence& sequence, iXMLOutputStream& os)
{
    os.openElement("circuit");
    tMeshCutSequence::const_iterator j;
    for(j = sequence.begin(); j != sequence.end(); ++j)
    {
        os.openElement("cut");
        SaveMeshCut_XML(**j, os);
        os.closeElement("cut");
    }
    os.closeElement("circuit");
}

void
SaveCircuits_XML(const tMeshCircuits& circuits, iXMLOutputStream& os)
{
    os.openElement("circuits");
    tMeshCircuits::const_iterator i;
    for(i = circuits.begin(); i != circuits.end(); ++i)
    {
        SaveCutSequence_XML(**i, os);
    }
    os.closeElement("circuits");
}

