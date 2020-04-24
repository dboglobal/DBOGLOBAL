#include "drvmodel.h"

int im2dvertex_screenx = offsetof(RwIm2DVertex, x);
int im2dvertex_screeny = offsetof(RwIm2DVertex, y);
int im2dvertex_screenz = offsetof(RwIm2DVertex, z);
int im2dvertex_rhw = offsetof(RwIm2DVertex, rhw);
int emissiveColor = offsetof(RwIm2DVertex, emissiveColor);
int im2dvertex_texu = offsetof(RwIm2DVertex, u);
int im2dvertex_texv = offsetof(RwIm2DVertex, v);
