//**********************************************************************
//
// Copyright (c) 2004
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include <vector>
#include <string>

class Interface;
class INode;
class Point3;

class cBuildingConnectionInfo
{

public:

    std::string _nodeName;
    std::string _name;
    tSigned32 _orientation;
    tSigned32 _x, _y, _zStart, _zEnd;
};

class cShape
{
public:

    std::string _nodeName;
    std::string _name;
    size_t _anchorIndex;
    std::vector<tSigned32> _coords;
};

class cAnchorsAndShapes
{
public:

    std::vector<cAnchor> _anchors;
    std::vector<cShape> _shapes;
    std::vector<cShape> _namedObstructionShapes;
};
