#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 6.3.9600.16384
//
//
// Buffer Definitions: 
//
// cbuffer $Globals
// {
//
//   float time;                        // Offset:    0 Size:     4
//
// }
//
//
// Resource Bindings:
//
// Name                                 Type  Format         Dim Slot Elements
// ------------------------------ ---------- ------- ----------- ---- --------
// $Globals                          cbuffer      NA          NA    0        1
//
//
//
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_Position              0   xyzw        0      POS   float       
// TexCoord                 0   xy          1     NONE   float   xy  
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_Target                0   xyzw        0   TARGET   float   xyzw
//
ps_5_0
dcl_globalFlags refactoringAllowed
dcl_constantbuffer cb0[1], immediateIndexed
dcl_input_ps linear v1.xy
dcl_output o0.xyzw
dcl_temps 3
mov r0.z, l(0.500000)
mul r1.xyz, cb0[0].xxxx, l(3.141590, 6.283180, 25.132721, 0.000000)
sincos r1.xzw, null, r1.xxzx
sincos null, r0.w, r1.y
mad r0.w, r0.w, l(0.100000), l(0.200000)
mad r0.xy, r1.xwxx, l(0.100000, 0.500000, 0.000000, 0.000000), l(0.500000, 0.500000, 0.000000, 0.000000)
mul r1.x, r1.z, l(0.020000)
add r0.xz, -r0.xxzx, v1.xxyx
mad r0.y, r0.y, l(80.000000), l(40.000000)
max r1.y, |r0.x|, l(1.000000)
div r1.y, l(1.000000, 1.000000, 1.000000, 1.000000), r1.y
min r1.z, |r0.x|, l(1.000000)
mul r1.y, r1.y, r1.z
mul r1.z, r1.y, r1.y
mad r1.w, r1.z, l(0.020835), l(-0.085133)
mad r1.w, r1.z, r1.w, l(0.180141)
mad r1.w, r1.z, r1.w, l(-0.330299)
mad r1.z, r1.z, r1.w, l(0.999866)
mul r1.w, r1.z, r1.y
mad r1.w, r1.w, l(-2.000000), l(1.570796)
lt r2.x, l(1.000000), |r0.x|
and r1.w, r1.w, r2.x
mad r1.y, r1.y, r1.z, r1.w
min r1.z, r0.x, l(1.000000)
dp2 r0.x, r0.xzxx, r0.xzxx
sqrt r0.x, r0.x
lt r0.z, r1.z, -r1.z
movc r0.z, r0.z, -r1.y, r1.y
mul r0.y, r0.y, r0.z
sincos null, r0.y, r0.y
mad r0.y, r1.x, r0.y, r0.w
add r0.x, -r0.y, r0.x
mul_sat r0.x, r0.x, l(100.000000)
mad r0.y, r0.x, l(-2.000000), l(3.000000)
mul r0.x, r0.x, r0.x
mul r0.x, r0.x, r0.y
mov r1.xy, v1.xyxx
mov r1.z, l(1.000000)
mul o0.xyz, r0.xxxx, r1.xyzx
mov o0.w, l(1.000000)
ret 
// Approximately 41 instruction slots used
#endif

const BYTE PixelShader[] =
{
     68,  88,  66,  67, 253,  26, 
     96, 212, 210,  65,  12,  84, 
    135, 180,  74,  78,  39, 157, 
    180,  82,   1,   0,   0,   0, 
    148,   7,   0,   0,   5,   0, 
      0,   0,  52,   0,   0,   0, 
     72,   1,   0,   0, 160,   1, 
      0,   0, 212,   1,   0,   0, 
    248,   6,   0,   0,  82,  68, 
     69,  70,  12,   1,   0,   0, 
      1,   0,   0,   0, 104,   0, 
      0,   0,   1,   0,   0,   0, 
     60,   0,   0,   0,   0,   5, 
    255, 255,   0,   1,   0,   0, 
    216,   0,   0,   0,  82,  68, 
     49,  49,  60,   0,   0,   0, 
     24,   0,   0,   0,  32,   0, 
      0,   0,  40,   0,   0,   0, 
     36,   0,   0,   0,  12,   0, 
      0,   0,   0,   0,   0,   0, 
     92,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,  36,  71, 108, 111, 
     98,  97, 108, 115,   0, 171, 
    171, 171,  92,   0,   0,   0, 
      1,   0,   0,   0, 128,   0, 
      0,   0,  16,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 168,   0,   0,   0, 
      0,   0,   0,   0,   4,   0, 
      0,   0,   2,   0,   0,   0, 
    180,   0,   0,   0,   0,   0, 
      0,   0, 255, 255, 255, 255, 
      0,   0,   0,   0, 255, 255, 
    255, 255,   0,   0,   0,   0, 
    116, 105, 109, 101,   0, 102, 
    108, 111,  97, 116,   0, 171, 
      0,   0,   3,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0, 173,   0,   0,   0, 
     77, 105,  99, 114, 111, 115, 
    111, 102, 116,  32,  40,  82, 
     41,  32,  72,  76,  83,  76, 
     32,  83, 104,  97, 100, 101, 
    114,  32,  67, 111, 109, 112, 
    105, 108, 101, 114,  32,  54, 
     46,  51,  46,  57,  54,  48, 
     48,  46,  49,  54,  51,  56, 
     52,   0, 171, 171,  73,  83, 
     71,  78,  80,   0,   0,   0, 
      2,   0,   0,   0,   8,   0, 
      0,   0,  56,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,  15,   0, 
      0,   0,  68,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   0,   0,   3,   3, 
      0,   0,  83,  86,  95,  80, 
    111, 115, 105, 116, 105, 111, 
    110,   0,  84, 101, 120,  67, 
    111, 111, 114, 100,   0, 171, 
    171, 171,  79,  83,  71,  78, 
     44,   0,   0,   0,   1,   0, 
      0,   0,   8,   0,   0,   0, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,  15,   0,   0,   0, 
     83,  86,  95,  84,  97, 114, 
    103, 101, 116,   0, 171, 171, 
     83,  72,  69,  88,  28,   5, 
      0,   0,  80,   0,   0,   0, 
     71,   1,   0,   0, 106,   8, 
      0,   1,  89,   0,   0,   4, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
     98,  16,   0,   3,  50,  16, 
     16,   0,   1,   0,   0,   0, 
    101,   0,   0,   3, 242,  32, 
     16,   0,   0,   0,   0,   0, 
    104,   0,   0,   2,   3,   0, 
      0,   0,  54,   0,   0,   5, 
     66,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0,   0,  63,  56,   0, 
      0,  11, 114,   0,  16,   0, 
      1,   0,   0,   0,   6, 128, 
     32,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0, 208,  15,  73,  64, 
    208,  15, 201,  64, 208,  15, 
    201,  65,   0,   0,   0,   0, 
     77,   0,   0,   6, 210,   0, 
     16,   0,   1,   0,   0,   0, 
      0, 208,   0,   0,   6,   2, 
     16,   0,   1,   0,   0,   0, 
     77,   0,   0,   6,   0, 208, 
      0,   0, 130,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,   9, 130,   0, 
     16,   0,   0,   0,   0,   0, 
     58,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
    205, 204, 204,  61,   1,  64, 
      0,   0, 205, 204,  76,  62, 
     50,   0,   0,  15,  50,   0, 
     16,   0,   0,   0,   0,   0, 
    198,   0,  16,   0,   1,   0, 
      0,   0,   2,  64,   0,   0, 
    205, 204, 204,  61,   0,   0, 
      0,  63,   0,   0,   0,   0, 
      0,   0,   0,   0,   2,  64, 
      0,   0,   0,   0,   0,  63, 
      0,   0,   0,  63,   0,   0, 
      0,   0,   0,   0,   0,   0, 
     56,   0,   0,   7,  18,   0, 
     16,   0,   1,   0,   0,   0, 
     42,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
     10, 215, 163,  60,   0,   0, 
      0,   8,  82,   0,  16,   0, 
      0,   0,   0,   0,   6,   2, 
     16, 128,  65,   0,   0,   0, 
      0,   0,   0,   0,   6,  17, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,   9,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 160,  66,   1,  64, 
      0,   0,   0,   0,  32,  66, 
     52,   0,   0,   8,  34,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16, 128, 129,   0, 
      0,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
    128,  63,  14,   0,   0,  10, 
     34,   0,  16,   0,   1,   0, 
      0,   0,   2,  64,   0,   0, 
      0,   0, 128,  63,   0,   0, 
    128,  63,   0,   0, 128,  63, 
      0,   0, 128,  63,  26,   0, 
     16,   0,   1,   0,   0,   0, 
     51,   0,   0,   8,  66,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16, 128, 129,   0, 
      0,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
    128,  63,  56,   0,   0,   7, 
     34,   0,  16,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,  42,   0, 
     16,   0,   1,   0,   0,   0, 
     56,   0,   0,   7,  66,   0, 
     16,   0,   1,   0,   0,   0, 
     26,   0,  16,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,   9, 130,   0,  16,   0, 
      1,   0,   0,   0,  42,   0, 
     16,   0,   1,   0,   0,   0, 
      1,  64,   0,   0,  95, 174, 
    170,  60,   1,  64,   0,   0, 
     54,  90, 174, 189,  50,   0, 
      0,   9, 130,   0,  16,   0, 
      1,   0,   0,   0,  42,   0, 
     16,   0,   1,   0,   0,   0, 
     58,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
    226, 118,  56,  62,  50,   0, 
      0,   9, 130,   0,  16,   0, 
      1,   0,   0,   0,  42,   0, 
     16,   0,   1,   0,   0,   0, 
     58,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      4,  29, 169, 190,  50,   0, 
      0,   9,  66,   0,  16,   0, 
      1,   0,   0,   0,  42,   0, 
     16,   0,   1,   0,   0,   0, 
     58,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
     56, 247, 127,  63,  56,   0, 
      0,   7, 130,   0,  16,   0, 
      1,   0,   0,   0,  42,   0, 
     16,   0,   1,   0,   0,   0, 
     26,   0,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,   9, 
    130,   0,  16,   0,   1,   0, 
      0,   0,  58,   0,  16,   0, 
      1,   0,   0,   0,   1,  64, 
      0,   0,   0,   0,   0, 192, 
      1,  64,   0,   0, 219,  15, 
    201,  63,  49,   0,   0,   8, 
     18,   0,  16,   0,   2,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128,  63,  10,   0, 
     16, 128, 129,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   7, 130,   0,  16,   0, 
      1,   0,   0,   0,  58,   0, 
     16,   0,   1,   0,   0,   0, 
     10,   0,  16,   0,   2,   0, 
      0,   0,  50,   0,   0,   9, 
     34,   0,  16,   0,   1,   0, 
      0,   0,  26,   0,  16,   0, 
      1,   0,   0,   0,  42,   0, 
     16,   0,   1,   0,   0,   0, 
     58,   0,  16,   0,   1,   0, 
      0,   0,  51,   0,   0,   7, 
     66,   0,  16,   0,   1,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,   1,  64, 
      0,   0,   0,   0, 128,  63, 
     15,   0,   0,   7,  18,   0, 
     16,   0,   0,   0,   0,   0, 
    134,   0,  16,   0,   0,   0, 
      0,   0, 134,   0,  16,   0, 
      0,   0,   0,   0,  75,   0, 
      0,   5,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     49,   0,   0,   8,  66,   0, 
     16,   0,   0,   0,   0,   0, 
     42,   0,  16,   0,   1,   0, 
      0,   0,  42,   0,  16, 128, 
     65,   0,   0,   0,   1,   0, 
      0,   0,  55,   0,   0,  10, 
     66,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16, 128,  65,   0,   0,   0, 
      1,   0,   0,   0,  26,   0, 
     16,   0,   1,   0,   0,   0, 
     56,   0,   0,   7,  34,   0, 
     16,   0,   0,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  42,   0,  16,   0, 
      0,   0,   0,   0,  77,   0, 
      0,   6,   0, 208,   0,   0, 
     34,   0,  16,   0,   0,   0, 
      0,   0,  26,   0,  16,   0, 
      0,   0,   0,   0,  50,   0, 
      0,   9,  34,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   1,   0,   0,   0, 
     26,   0,  16,   0,   0,   0, 
      0,   0,  58,   0,  16,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   8,  18,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16, 128,  65,   0,   0,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     56,  32,   0,   7,  18,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 200,  66,  50,   0, 
      0,   9,  34,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
      1,  64,   0,   0,   0,   0, 
      0, 192,   1,  64,   0,   0, 
      0,   0,  64,  64,  56,   0, 
      0,   7,  18,   0,  16,   0, 
      0,   0,   0,   0,  10,   0, 
     16,   0,   0,   0,   0,   0, 
     10,   0,  16,   0,   0,   0, 
      0,   0,  56,   0,   0,   7, 
     18,   0,  16,   0,   0,   0, 
      0,   0,  10,   0,  16,   0, 
      0,   0,   0,   0,  26,   0, 
     16,   0,   0,   0,   0,   0, 
     54,   0,   0,   5,  50,   0, 
     16,   0,   1,   0,   0,   0, 
     70,  16,  16,   0,   1,   0, 
      0,   0,  54,   0,   0,   5, 
     66,   0,  16,   0,   1,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128,  63,  56,   0, 
      0,   7, 114,  32,  16,   0, 
      0,   0,   0,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
     70,   2,  16,   0,   1,   0, 
      0,   0,  54,   0,   0,   5, 
    130,  32,  16,   0,   0,   0, 
      0,   0,   1,  64,   0,   0, 
      0,   0, 128,  63,  62,   0, 
      0,   1,  83,  84,  65,  84, 
    148,   0,   0,   0,  41,   0, 
      0,   0,   3,   0,   0,   0, 
      0,   0,   0,   0,   2,   0, 
      0,   0,  34,   0,   0,   0, 
      0,   0,   0,   0,   1,   0, 
      0,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   4,   0,   0,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0
};