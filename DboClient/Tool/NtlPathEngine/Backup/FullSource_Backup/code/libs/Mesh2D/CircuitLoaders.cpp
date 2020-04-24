//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "CircuitLoaders.h"
#include "libs/Mesh2D/interface/Mesh2DPersistence.h"
#include "libs/Mesh2D/interface/CircuitElement.h"
#include "libs/Mesh2D/interface/MeshCut.h"
#include "libs/Mesh2D/interface/MeshTraversal.h"
#include "libs/Mesh_Common/interface/MeshTraversal_SI.h"
#include "libs/Geometry/interface/Intersection_PointLike.h"
#include "libs/Geometry/interface/GeometryPersistence.h"
#include "platform_common/PointMultiplication.h"
#include "libs/ParseXML/interface/DocumentHandler.h"
#include "libs/ParseXML/interface/ParseXML.h"
#include "common/interface/Attributes.h"
#include "common/CLib/string.h"

cCutLoader::cCutLoader(tMesh& mesh) :
    _mesh(mesh)
{
}

void
cCutLoader::init(const char *const* attributes)
{
    const char* value;
    _startCapped = false;
    value = GetValueForAttribute(attributes, "entry");
    if(value == 0)
    {
    // internal circuit
        _loops = true;
        value = GetValueForAttribute(attributes, "start_face");
        assertD(value);
        FaceFromString(_mesh, value, _startFace);
        value = GetValueForAttribute(attributes, "startCap");
        if(value)
        {
            _loops = false;
            _startCapped = true;
            LineFromString(value, _startCap);
        }
    }
    else
    {
        assertD(GetValueForAttribute(attributes, "start_face") == 0);
        _loops = false;
        tSigned32 edgeIndex = ValueAsLong(attributes, "entry");
        _entry_SI = _mesh.edgeAt(edgeIndex);
    }
    _lines.clear();
	_intersects.clear();
	_hasIntersect.clear();
    _unconnected = ValueAsBoolWithDefault(attributes, "unconnected", false);
}

iXMLHandler*
cCutLoader::element(const char* el, const char *const* attributes)
{
     if(strcmp(el, "element") == 0)
    {
        const char* value = GetValueForAttribute(attributes, "line");
        assertD(value);
        _lines.push_back(tLine());
        LineFromString(value, _lines.back());
		_intersects.push_back(tLine());
		_hasIntersect.push_back(false);
        value = GetValueForAttribute(attributes, "intersect");
        if(value)
		{
			LineFromString(value, _intersects.back());
			_hasIntersect.back() = true;
		}
        else
        {
            value = GetValueForAttribute(attributes, "endCap");
            if(value)
		    {
			    LineFromString(value, _intersects.back());
			    _hasIntersect.back() = true;
            }
        }
    }
    return 0;
}

void
cCutLoader::completed()
{
    assertD(!_lines.empty());

    cCircuitElement* ce;
    cCircuitElement* previous=0;
    cCircuitElement* first = 0;

    cMeshElement<tMesh> pos;
    size_t j;
    for(j = 0; j < _lines.size(); j++)
    {
        size_t next_j=j+1;
        if(next_j==_lines.size())
            next_j=0;

        cEdgesTraversal traversal;

        if(j == 0)
        {
            if(_loops)
            {
                traversal.init_InFace(_startFace);
            }
            else if(_startCapped)
            {
                traversal.init_InFace(_startFace);
            }
            else
            {
                traversal.init_Entry(_entry_SI);
            }
        }
        else
        {
            traversal.init_InFace(previous->lastFaceCrossed());
        }

        tLine intersect;
        bool useIntersect;
        if(_hasIntersect[j])
        {
    		intersect = _intersects[j];
            useIntersect = true;
        }
        else if(j + 1 < _lines.size() || _loops)
        {
    		intersect = _lines[next_j];
            useIntersect = true;
        }
        else
        {
            useIntersect = false;
        }

        tFace f = traversal.lastFaceCrossed();
        tEdge edgeCrossed;
        if(useIntersect)
        {
            cIntersection_PointLike target(_lines[j], intersect);
            while(TraverseToTarget_SI<tMesh>(f, _lines[j], target, edgeCrossed))
            {
                traversal.pushEdgeCrossing(edgeCrossed);
            }
        }
        else
        {
            while(TraverseToExit_SI<tMesh>(f, _lines[j], edgeCrossed))
            {
                traversal.pushEdgeCrossing(edgeCrossed);
            }
            traversal.pushExit(edgeCrossed);
        }

        ce = new cCircuitElement;
        if(j == 0 && _startCapped)
        {
		    if(_hasIntersect[j])
		    {
			    ce->init_WithStartAndEndCaps(_lines[j], _startCap, _intersects[j], traversal);
		    }
		    else
		    {
			    ce->init_WithStartCap(_lines[j], _startCap, traversal);
		    }
        }
        else
        {
		    if(_hasIntersect[j])
		    {
			    ce->init_WithEndCap(_lines[j], _intersects[j], traversal);
		    }
		    else
		    {
			    ce->init(_lines[j], traversal);
		    }
        }
        if(previous)
            previous->connectTo(*ce);
        previous=ce;

        if(j==0)
            first=ce;
    }

    if(!_unconnected)
    {
        previous->connectTo(*first);
    }

    _result = new cMeshCut(first);
}

cHandle<cMeshCut>
cCutLoader::getResult()
{
    return _result;
}

// ========================

cCircuitsLoader::cCircuitsLoader(tMesh& mesh) :
 _cutLoader(mesh)
{
}

void
cCircuitsLoader::init(const char *const* attributes)
{
    _result = new tMeshCircuits;
}

cHandle<tMeshCircuits>
cCircuitsLoader::getResult()
{
    return _result;
}

iXMLHandler*
cCircuitsLoader::element(const char* el, const char *const* attributes)
{
    if(getDepth() == 0 && strcmp(el, "circuit") == 0)
    {
        _result->_list.push_back(new tMeshCutSequence);
        return this;
    }
    if(getDepth() == 1 && strcmp(el, "cut") == 0)
    {
        _cutLoader.init(attributes);
        return &_cutLoader;
    }
    return 0;
}

void
cCircuitsLoader::elementCompleted(iXMLHandler* elementHandler)
{
    if(elementHandler != this)
    {
        assertD(getDepth() == 1);
        assertD(elementHandler == &_cutLoader);
        _result->_list.back()->_list.push_back(_cutLoader.getResult());
    }
}


