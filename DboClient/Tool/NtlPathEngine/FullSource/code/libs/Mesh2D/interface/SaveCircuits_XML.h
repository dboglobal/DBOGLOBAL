//**********************************************************************
//
// Copyright (c) 2002
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#ifndef SAVE_CIRCUITS_XML_INCLUDED
#define SAVE_CIRCUITS_XML_INCLUDED

#include "libs/Mesh2D/interface/MeshCut_Header.h"

class iXMLOutputStream;


void SaveMeshCuts_XML_Prologue(iXMLOutputStream& os);
void SaveMeshCut_XML(const cMeshCut& cut, iXMLOutputStream& os);
void SaveMeshCuts_XML_Epilogue(iXMLOutputStream& os);

void SaveCutSequence_XML(const tMeshCutSequence& cuts, iXMLOutputStream& os);
void SaveCircuits_XML(const tMeshCircuits& circuits, iXMLOutputStream& os);

#endif
