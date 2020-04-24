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
#include "ConnectPortals.h"
#include "CutPortal.h"
#include "iFaceIsOnTerrainCallBack.h"
#include "ExternalShapeInfo.h"
#include "libs/Mesh3D/interface/Mesh3DPartitioning.h"
#include "libs/Mesh_Common/interface/MeshWithTrackedTransform.h"
#include "libs/Mesh_Common/interface/PositionForPointInFace.h"
#include "libs/Geometry/interface/Point_PointLike.h"
#include "common/TemplateMesh/MeshElement.h"
#include "common/TemplateMesh/External.h"
#include "common/interface/Error.h"

typedef tMesh_3D_FaceTracked tMesh;
typedef tFace_3D_FaceTracked tFace;
typedef tEdge_3D_FaceTracked tEdge;
typedef tVertex_3D_FaceTracked tVertex;

static cMeshElement<tMesh>
ResolveCutStart(const cMesh3DPartitioning& partitioning, tSigned32* resultsBuffer,
                tMesh& originalMesh,
                tVertex v,
                iFaceIsOnTerrainCallBack& faceIsOnTerrainCallBack,
                float minZ, float maxZ,
                bool& failedBecauseOfMultipleLayers,
                bool& lastIntersectCheckedHeightValid,
                float& lastIntersectCheckedHeight
                )
{
    tPoint p = GetVertexPoint(v);
    tSigned32 segments = partitioning.queryFacesPotentiallyStabbed(p, resultsBuffer);

    cMeshElement<tMesh> result;
    result.clear();
    cMeshElement<tMesh> vertexElement(v);

    tSigned32 i;
    for(i = 0; i < segments; i++)
    {
        tSigned32 fIndex = resultsBuffer[i];
        tFace f = originalMesh.faceAt(fIndex);
        cMeshElement<tMesh> candidate = ElementForPointInOverlappingFace<tMesh>(f, p);
        if(!candidate.valid())
        {
            continue;
        }
        if(candidate == vertexElement)
        {
            continue;
        }
        if(faceIsOnTerrainCallBack.faceIsOnTerrain(f))
        {
            continue;
        }
        float height = f->heightForF(p);
        if(height < minZ || height > maxZ)
        {
            lastIntersectCheckedHeightValid = true;
            lastIntersectCheckedHeight = height;
            continue;
        }
        if(result.valid())
        {
            if(candidate.isPositionallyEquivalent(result))
            {
                continue;
            }
            failedBecauseOfMultipleLayers = true;
            result.clear();
            return result;
        }

//        result = candidate;

        cMeshElement<tMesh> temp = candidate; //... attempted workaround for GCC bug - http://gcc.gnu.org/bugzilla/show_bug.cgi?id=30590
        result = temp;
    }
    failedBecauseOfMultipleLayers = false;
    return result;
}

static cMeshElement<tMesh>
ResolveCutStartToTerrain(
        const cMesh3DPartitioning& partitioning, tSigned32* resultsBuffer,
        tMesh& originalMesh,
        tVertex v,
        iFaceIsOnTerrainCallBack& faceIsOnTerrainCallBack,
        bool& failedBecauseOfMultipleLayers
        )
{
    tPoint p = GetVertexPoint(v);
    tSigned32 segments = partitioning.queryFacesPotentiallyStabbed(p, resultsBuffer);

    cMeshElement<tMesh> result;
    result.clear();
    cMeshElement<tMesh> vertexElement(v);

    tSigned32 i;
    for(i = 0; i < segments; i++)
    {
        tSigned32 fIndex = resultsBuffer[i];
        tFace f = originalMesh.faceAt(fIndex);
        cMeshElement<tMesh> candidate = ElementForPointInOverlappingFace<tMesh>(f, p);
        if(!candidate.valid())
        {
            continue;
        }
        if(candidate == vertexElement)
        {
            continue;
        }
        if(!faceIsOnTerrainCallBack.faceIsOnTerrain(f))
        {
            continue;
        }
        if(result.valid())
        {
            if(candidate.isPositionallyEquivalent(result))
            {
                continue;
            }
            failedBecauseOfMultipleLayers = true;
            result.clear();
            return result;
        }
        result = candidate;
    }
    failedBecauseOfMultipleLayers = false;
    return result;
}

class cMesh3DInterpolator
{
public:

    void interpolate(const cMeshElement<tMesh>& el, const tPoint& p,
        tMesh::tVertexData& vertexData, tMesh::tEdgeData& edgeData)
    {
        tFace f;
        if(el.type() == bMeshElement::FACE)
        {
            f = el.asFace();
        }
        else
        {
            f = el.asEdge().face();
        }
        assertD(!f->isExternal());
        float z = f->heightForF(p);
        vertexData = tMesh::tVertexData(p.getX(), p.getY());
        edgeData = tMesh::tEdgeData(z);
    }

    void interpolateEdge(tEdge e)
    {
        e->_z = e.twin().next()->_z;
    }
};

void
ConnectPortals(
        tMesh& baseMesh,
        iFaceIsOnTerrainCallBack& faceIsOnTerrain0CallBack,
        iFaceIsOnTerrainCallBack& faceIsOnTerrainCallBack,
        const std::vector<cExternalShapeInfo*>& externalShapes
        )
{
    assertF(ValidateMeshConnections(baseMesh));
    cMeshWithTrackedTransform<tMesh> transformed(&baseMesh);
    assertF(ValidateMeshConnections(transformed.mesh()));
    cMesh3DPartitioning partitioning(baseMesh);
    std::vector<tSigned32> resultsBuffer(partitioning.maximumResultsSize());

    cMesh3DInterpolator interpolator;

    cBitVector edgeMustNotBeCut;

    tSigned32 i;

// first pass: resolve start points and make portal cuts

    for(i = 0; i < SizeL(externalShapes); ++i)
    {
        cExternalShapeInfo& shape = *externalShapes[i];
        tSigned32 j;
        for(j = 0; j < SizeL(shape._insidePortalEdges); ++j)
        {
            tEdge insideEdge = shape._insidePortalEdges[j];
            if(!insideEdge.valid())
            {
                continue;
            }
            float minZ = shape._portalResolveRanges[j * 2];
            float maxZ = shape._portalResolveRanges[j * 2 + 1];

            bool lastIntersectCheckedHeightValid = false;
            float lastIntersectCheckedHeight;

            bool failedBecauseOfMultipleLayers;
            cMeshElement<tMesh> startElement;
            if(shape._portalResolvesToTerrain[j])
            {
                startElement = ResolveCutStartToTerrain(
                        partitioning, &resultsBuffer.front(),
                        baseMesh, insideEdge.vertex(),
                        faceIsOnTerrain0CallBack,
                        failedBecauseOfMultipleLayers
                        );
            }
            else
            {
                startElement = ResolveCutStart(
                        partitioning, &resultsBuffer.front(),
                        baseMesh, insideEdge.vertex(),
                        faceIsOnTerrainCallBack,
                        minZ, maxZ,
                        failedBecauseOfMultipleLayers,
                        lastIntersectCheckedHeightValid,
                        lastIntersectCheckedHeight
                        );
            }
            if(!startElement.valid())
            {
                cAttributesBuilder attributes;
                attributes.add(shape._errorAttributes.get());
                attributes.add("faceIndex", shape._portalFaceIndices[j]);
                attributes.add("portalStartX", GetVertexPoint(insideEdge).getX());
                attributes.add("portalStartY", GetVertexPoint(insideEdge).getY());
                attributes.add("portalEndX", GetVertexPoint(insideEdge.next()).getX());
                attributes.add("portalEndY", GetVertexPoint(insideEdge.next()).getY());
                if(shape._portalResolvesToTerrain[j])
                {
                    if(failedBecauseOfMultipleLayers)
                    {
                        Error("NonFatal", "Failed to resolve target position for a building portal connection to terrain."
                                " Multiple faces were intersected within the portal's z range."
                                " The portal will not be connected.",
                                attributes.get()
                                );
                    }
                    else
                    {
                        Error("NonFatal", "Failed to resolve target position for a building portal connection to terrain."
                                " No faces were intersected within the portal's z range."
                                " The portal will not be connected.",
                                attributes.get()
                                );
                    }
                }
                else
                {
                    attributes.add("minZ", minZ);
                    attributes.add("maxZ", maxZ);
                    if(lastIntersectCheckedHeightValid)
                    {
                        attributes.add("lastIntersectCheckedHeight", lastIntersectCheckedHeight);
                    }
                    if(failedBecauseOfMultipleLayers)
                    {
                        Error("NonFatal", "Failed to resolve target position for a building portal connection."
                            " Multiple faces were intersected within the portal's z range."
                            " The portal will not be connected.",
                            attributes.get());
                    }
                    else
                    {
                        Error("NonFatal", "Failed to resolve target position for a building portal connection."
                            " No faces were intersected within the portal's z range."
                            " The portal will not be connected.",
                            attributes.get());
                    }
                }
                continue;
            }

            if(startElement.isVertex())
            {
                tEdge e = tMesh::firstEdge(startElement.asVertex());
                if(VertexIsExternal(e))
                {
                    cAttributesBuilder attributes;
                    attributes.add(shape._errorAttributes.get());
                    attributes.add("faceIndex", shape._portalFaceIndices[j]);
                    attributes.add("portalStartX", GetVertexPoint(insideEdge).getX());
                    attributes.add("portalStartY", GetVertexPoint(insideEdge).getY());
//                    attributes.add("portalEndX", GetVertexPoint(insideEdge.next()).getX());
//                    attributes.add("portalEndY", GetVertexPoint(insideEdge.next()).getY());
                    attributes.add("externalVertexX", GetVertexPoint(e).getX());
                    attributes.add("externalVertexY", GetVertexPoint(e).getY());
                    tEdge nextExternal = ExternalEdgeForVertex(e).next();
                    attributes.add("nextExternalVertexX", GetVertexPoint(nextExternal).getX());
                    attributes.add("nextExternalVertexY", GetVertexPoint(nextExternal).getY());
                    Error("NonFatal", "The start position for a building portal connection falls exactly on an external vertex."
                        " The portal will not be connected.",
                        attributes.get());
                    continue;
                }
            }
            cPoint_PointLike pl(GetVertexPoint(insideEdge));
            cMeshElement<tMesh> startOnTransformed = transformed.translate(startElement, pl);
            if(!ElementIsPermittedOnPortalCut(startOnTransformed, true, edgeMustNotBeCut))
            {
                cAttributesBuilder attributes;
                attributes.add(shape._errorAttributes.get());
                attributes.add("faceIndex", shape._portalFaceIndices[j]);
                Error("NonFatal", "The start position for a building portal connection touches the edge of the mesh or another portal."
                    " The portal will not be connected.",
                    attributes.get());
                continue;
            }
            assertF(ValidateMeshConnections(transformed.mesh()));
            tEdge behindEdge;
            behindEdge = CutPortal(transformed, interpolator, edgeMustNotBeCut, startOnTransformed, GetEdgeLine(insideEdge));
            if(!behindEdge.valid())
            {
                cAttributesBuilder attributes;
                attributes.add(shape._errorAttributes.get());
                attributes.add("faceIndex", shape._portalFaceIndices[j]);
                Error("NonFatal", "The cut for a building portal connection would exit or touch the edge of the mesh"
                    " or another portal cut."
                    " The portal will not be connected.",
                    attributes.get());
                continue;
            }
            assertF(ValidateMeshConnections(transformed.mesh()));
            shape._behindPortalEdges[j] = behindEdge;

            shape._insideEdgeOnTransformed[j] = transformed.edgeAt(insideEdge.index());

            if(behindEdge.index() + 1 > SizeL(edgeMustNotBeCut))
            {
                edgeMustNotBeCut.resize(behindEdge.index() + 1, false);
            }
            if(behindEdge.twin().index() + 1 > SizeL(edgeMustNotBeCut))
            {
                edgeMustNotBeCut.resize(behindEdge.twin().index() + 1, false);
            }
            edgeMustNotBeCut.setBit(behindEdge.index(), true);
            edgeMustNotBeCut.setBit(behindEdge.twin().index(), true);
        }
    }

    assertF(ValidateMeshConnections(transformed.mesh()));
    baseMesh.clear();
    transformed.extractMesh(baseMesh);

    RemoveExternalFaces(baseMesh);    

// second pass: connect portals

    for(i = 0; i < SizeL(externalShapes); ++i)
    {
        cExternalShapeInfo& shape = *externalShapes[i];
        tSigned32 j;
        for(j = 0; j < SizeL(shape._insidePortalEdges); ++j)
        {
            tEdge behindEdge = shape._behindPortalEdges[j];
            if(!behindEdge.valid())
            {
                continue;
            }
            tEdge inFrontEdge = behindEdge.twin();
            baseMesh.breakEdge(behindEdge);
            tEdge insideEdge = shape._insideEdgeOnTransformed[j];
            baseMesh.connectEdges(inFrontEdge, insideEdge);
        }
    }

    AddExternalFaces(baseMesh);
}
