//**********************************************************************
//
// Copyright (c) 2004-2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

class Interface;
class cAnchorsAndPinnedShapes;

void BuildAnchorsAndShapesFromMax(Interface* ip, const char* prefix, cAnchorsAndPinnedShapes& toBuild, float scale, bool negateY);
void BuildAnchorsAndShapesFromMax_AllInScene(Interface* ip, cAnchorsAndPinnedShapes& toBuild, float scale, bool negateY);
