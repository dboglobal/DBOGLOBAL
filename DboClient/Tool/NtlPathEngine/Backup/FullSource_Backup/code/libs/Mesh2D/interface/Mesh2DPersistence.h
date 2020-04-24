//**********************************************************************
//
// Copyright (c) 2002-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "libs/Mesh2D/interface/tMesh_Header.h"
#include "common/STL/string.h"

class cInternalPosition;

void EdgeFromString(tMesh& mesh, const char* str, tEdge& result);
void FaceFromString(tMesh& mesh, const char* str, tFace& result);
void EntryInfoFromString(tMesh& mesh, const char* str, tEdge& e, bool& fromVertex);
void ExitInfoFromString(tMesh& mesh, const char* str, tEdge& e, bool& toVertex);

std::string PositionToString(const cInternalPosition& p);
cInternalPosition PositionFromString(tMesh& mesh, const char* str);
