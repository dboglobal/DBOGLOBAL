#pragma once

void RenderGeometryCone(RwMatrix* pmat, RwRGBA* prgbaColor, RwReal fConeAngle, RwReal fConeSize, RwBool bWire);
void RenderGeometryArrow(RwMatrix* pmat, RwRGBA* prgbaColor, RwReal fArrowSize, RwBool bWire);
void RenderGeometryBillboardSphere(RwMatrix* pmat, RwRGBA* prgbaColor, RwReal fRadius, RwCamera* pCamera, RwBool bWire);
void RenderGeometrySpherePlane(RwMatrix* pmat, RwRGBA* prgbaColor, RwReal fRadius);
void RenderGeometryQuadPlane(RwMatrix* pmat, RwRGBA* prgbaColor, RwReal fSizeHalfX, RwReal fSizeHalfZ);

void RenderGeometryBox(RwMatrix* pmat, const RwBBox* pBBox, RwRGBA* prgbaColorLine, RwRGBA* prgbaColorPlane, RwReal fOffset, RwBool bWire);