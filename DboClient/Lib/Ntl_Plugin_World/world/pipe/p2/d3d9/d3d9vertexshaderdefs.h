#ifndef D3D9VERTEXSHADERDEFS_H
#define D3D9VERTEXSHADERDEFS_H

/* For use in the vertex shader descriptor */
#define VSD_REG_POS                     0
#define VSD_REG_WEIGHTS                 1
#define VSD_REG_INDICES                 2
#define VSD_REG_NORMAL                  3
#define VSD_REG_COLOR                   4
#define VSD_REG_TEXCOORD0               5
#define VSD_REG_TEXCOORD1               6
#define VSD_REG_TEXCOORD2               7
#define VSD_REG_TEXCOORD3               8
#define VSD_REG_TEXCOORD4               9
#define VSD_REG_TEXCOORD5               10
#define VSD_REG_TEXCOORD6               11
#define VSD_REG_TEXCOORD7               12
#define VSD_REG_TANGENT                 13
#define VSD_REG_POS2                    14
#define VSD_REG_NORMAL2                 15

/* Input register - For use in the vertex shader code */
#define VSIN_REG_POS                    v0
#define VSIN_REG_WEIGHTS                v1
#define VSIN_REG_INDICES                v2
#define VSIN_REG_NORMAL                 v3
#define VSIN_REG_COLOR                  v4
#define VSIN_REG_TEXCOORD0              v5
#define VSIN_REG_TEXCOORD1              v6
#define VSIN_REG_TEXCOORD2              v7
#define VSIN_REG_TEXCOORD3              v8
#define VSIN_REG_TEXCOORD4              v9
#define VSIN_REG_TEXCOORD5              v10
#define VSIN_REG_TEXCOORD6              v11
#define VSIN_REG_TEXCOORD7              v12
#define VSIN_REG_TANGENT                v13
#define VSIN_REG_POS2                   v14
#define VSIN_REG_NORMAL2                v15

/* Temporary register - For use in the vertex shader code */

/* (keep r1 free for pairing up ILU instructions!) */
#define VSTMP_REG_ILU_TMP               r1
#define VSTMP_REG_POS_TMP               r2
#define VSTMP_REG_POS_ACCUM             r3
#define VSTMP_REG_NORMAL_TMP            r4
#define VSTMP_REG_NORMAL_ACCUM          r5
#define VSTMP_REG_CLAMP_TMP             r6
#define VSTMP_REG_DIST_TMP              r7
#define VSTMP_REG_ATTEN_TMP             r8
#define VSTMP_REG_BUMPDIR_TMP           r9
#define VSTMP_REG_TEX                   r10
#define VSTMP_REG_COLOR_TMP             r11

/* Vertex shader defines */

/* bone uses 3 constants slots */
#define SHADERCONSTS_PER_BONE   3

/* Always need a transform */
#define VSCONST_REG_TRANSFORM_SIZE      4

/* always need an ambient light */
#define VSCONST_REG_AMBIENT_SIZE        1
 
/* optionally need some lights */
#define VSCONST_REG_DIR_LIGHT_SIZE      2

#define VSCONST_REG_PNT_LIGHT_SIZE      2

#define VSCONST_REG_SPOT_LIGHT_SIZE     4

/* always need a material color */
#define VSCONST_REG_MAT_COLOR_SIZE      1

/* sometimes need some env map coefficients */
#define VSCONST_REG_ENV_SIZE            2

/* world transform used for bump mapping */
#define VSCONST_REG_BUMP_SIZE           2

/* Texture transform matrices */
#define VSCONST_REG_TEXTRANS_SIZE       4

#endif /* D3D9VERTEXSHADERDEFS_H */
