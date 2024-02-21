# |<-- k=4 -->|
# +++++++++++++ -           f0, f4, f0, f4
# +           + |           f1, f5, f1, f5
# +     A     + mr=4        f2, f6, f2, f6
# +           + |           f3, f7, f3, f7
# +++++++++++++ -       
                       

# |<--  nr=16  -->|
# +++++++++++++++++ -       v0 v1 v2 v3
# +               + |       v4 v5 v6 v7
# +       B       + k=4     v8,v9,v10,v11     
# +               + |       v12,v13,v14,v15
# +++++++++++++++++ -


# |<--    nr    -->|
# +++++++++++++++++ -       
# +               + |       v16,v17,v18,v19
# +       C       + k=4     v20,v21,v22,v23
# +               + |       v24,v25,v26,v27
# +++++++++++++++++ -       v28,v29,v30,v31
#

# void RvvSgemm4x16(size_t nr,         // nr <= 16, a0
#                   size_t mr,         // mr <= 4,  a1
#                   size_t k,          // astride = k*sizeof(float), a2
#                   const float* a,    // mr * k,   a3
#                   const float* b,    // k * 16,   a4
#                   float* c,          // mr * nr,  a5
#                   size_t c_stride,  // Len(N) * sizeof(float), a6
#                   const float* bias  // bias, a7
#                   );
.global RvvSgemm4x16
.type RvvSgemm4x16, @function

#define nr a0
#define mr a1
#define k  a2
#define ap a3
#define bp a4
#define cp a5
#define c_stride a6
#define bias a7

#define ap1 t0
#define ap2 t1
#define ap3 t2
#define cp1 t3
#define cp2 t4
#define cp3 t5
#define a_stride s0
#define bp0      s1
#define biasp    s2
#define kt       s3
#define ap0      s4
#define cp0      s5
#define bp1      s6
#define vl       s7
#define ap_offset       s10
#define cp_offset       s11

#define FRAMESIZE 104

RvvSgemm4x16:
    addi sp, sp, -FRAMESIZE # callee update stack pointer
    sd s0, 96(sp)           # callee saved frame pointer
    addi s0, sp, FRAMESIZE  # generate new frame pointer
    sd s1, -16(s0)
    sd s2, -24(s0)
    sd s3, -32(s0)
    sd s4, -40(s0)
    sd s5, -48(s0)
    sd s6, -56(s0)
    sd s7, -64(s0)
    sd s8, -72(s0)
    sd s9, -80(s0)
    sd s10, -88(s0)
    sd s11, -96(s0)

    li ap_offset, 0
    li cp_offset, 0
    slli a_stride, k, 2     # astride = k * sizeof(float)
    mv s3, nr
    vsetvli s2, s3, e32, m4
    mv ap0, ap
    mv bp0, bp
    mv cp0, cp
.a1_offset:
    mv ap1, ap0
    mv cp1, cp0
    slti t6, mr, 2          # mr < 2
    bnez t6, .a2_offset
    add ap1, ap0, a_stride
    add cp1, cp0, c_stride
.a2_offset:
    mv ap2, ap1
    mv cp2, cp1
    slti t6, mr, 3          # mr < 3
    bnez t6, .a3_offset
    add ap2, ap1, a_stride
    add cp2, cp1, c_stride
.a3_offset:
    mv ap3, ap2
    mv cp3, cp2
    slti t6, mr, 4          # mr < 4
    bnez t6, .start
    add ap3, ap2, a_stride
    add cp3, cp2, c_stride

.start:
    mv biasp, bias
    mv kt, k
    beqz mr, .end

    vlw.v v16, (biasp)
    vlw.v v20, (biasp)
    vlw.v v24, (biasp)
    vlw.v v28, (biasp)
    addi biasp, biasp, 64
    slti t6, kt, 4               # kt < 4, t6 = 1
    bnez t6, .k2_tail
    

    // flw fs0, 64(ap0)          # pre-load A
    // flw fs1, 64(ap1)          # pre-load A
    // flw fs2, 64(ap2)          # pre-load A
    // flw fs3, 64(ap3)          # pre-load A
    
    // flw fs4, 512(bp0)       # pre-load B
    // flw fs5, 576(bp0)       # pre-load B
    // flw fs6, 640(bp0)       # pre-load B
    // flw fs7, 704(bp0)       # pre-load B

    // load 4 row A (A0, A1, A2, A3)
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft1, (ap1)
    addi ap1, ap1, 4
    flw ft2, (ap2)
    addi ap2, ap2, 4
    flw ft3, (ap3)
    addi ap3, ap3, 4
    // load 16 col B(B0, B1, B2, B3)
    vlw.v v0, (bp0)
    addi bp0,bp0,64

    addi kt, kt, -4         # Decrement k counter
    slti t6, kt, 4          # kt < 4
    bnez t6, .k4_tail       # jump to k4_tail

.k4_main:
    addi kt, kt, -4         # Decrement k counter
    // first group of 16 fma, second group load
    vfmacc.vf v16,  ft0, v0
    vlw.v v4, (bp0)         # b0'->v4
    // flw fs4, 384(bp0)       # pre-load B
    addi bp0,bp0,64
    vfmacc.vf v20,  ft1, v0
    flw ft4, (ap0)          # a0'->ft4
    addi ap0, ap0, 4
    vfmacc.vf v24,  ft2, v0
    flw ft5, (ap1)          # a1'->ft5
    addi ap1, ap1, 4
    vfmacc.vf v28,  ft3, v0
    flw ft6, (ap2)          # a2'->ft6
    addi ap2, ap2, 4
    flw ft7, (ap3)          # a3'->ft7
    addi ap3, ap3, 4
    // second group of 16 fma, third group load
    slti t6, kt, 4              # kt < 4, t6 = 1
    vfmacc.vf v16,  ft4, v4
    vlw.v v8, (bp0)         # b0'->v0
    // flw fs5, 384(bp0)       # pre-load B
    addi bp0,bp0,64
    vfmacc.vf v20,  ft5, v4
    flw ft0, (ap0)          # a0'->ft0
    addi ap0, ap0, 4
    vfmacc.vf v24,  ft6, v4
    flw ft1, (ap1)          # a1'->ft1
    addi ap1, ap1, 4
    vfmacc.vf v28,  ft7, v4
    flw ft2, (ap2)          # a2'->ft2
    addi ap2, ap2, 4
    flw ft3, (ap3)          # a3'->ft3
    addi ap3, ap3, 4
    // third group of 16 fma, fourth group load
    vfmacc.vf v16,  ft0, v8
    vlw.v v12, (bp0)         # b0'->v0
    // flw fs6, 384(bp0)       # pre-load B
    addi bp0,bp0,64
    vfmacc.vf v20,  ft1, v8
    flw ft4, (ap0)          # a0'->ft0
    addi ap0, ap0, 4
    vfmacc.vf v24,  ft2, v8
    flw ft5, (ap1)          # a1'->ft1
    addi ap1, ap1, 4
    vfmacc.vf v28,  ft3, v8
    flw ft6, (ap2)          # a2'->ft2
    addi ap2, ap2, 4
    flw ft7, (ap3)          # a3'->ft3
    addi ap3, ap3, 4
    // fourth group of 16 fma, first group load
    vfmacc.vf v16,  ft4, v12
    vlw.v v0, (bp0)         # b0'->v0
    // flw fs7, 384(bp0)       # pre-load B
    addi bp0,bp0,64
    vfmacc.vf v20,  ft5, v12
    flw ft0, (ap0)          # a0'->ft0
    addi ap0, ap0, 4
    vfmacc.vf v24,  ft6, v12
    flw ft1, (ap1)          # a0'->ft0
    addi ap1, ap1, 4
    vfmacc.vf v28,  ft7, v12
    flw ft2, (ap2)          # a0'->ft0
    addi ap2, ap2, 4
    flw ft3, (ap3)          # a0'->ft0
    addi ap3, ap3, 4

    // flw fs0, 64(ap0)          # pre-load A
    // flw fs1, 64(ap1)          # pre-load A
    // flw fs2, 64(ap2)          # pre-load A
    // flw fs3, 64(ap3)          # pre-load A

    beqz t6, .k4_main
.k4_tail:
    // first group of 16 fma, second group load
    vfmacc.vf v16,  ft0, v0
    vlw.v v4, (bp0)         # b0'->v4
    addi bp0,bp0,64
    vfmacc.vf v20,  ft1, v0
    flw ft4, (ap0)          # a0'->ft4
    addi ap0, ap0, 4
    vfmacc.vf v24,  ft2, v0
    flw ft5, (ap1)          # a1'->ft5
    addi ap1, ap1, 4
    vfmacc.vf v28,  ft3, v0
    flw ft6, (ap2)          # a2'->ft6
    addi ap2, ap2, 4
    flw ft7, (ap3)          # a3'->ft7
    addi ap3, ap3, 4
    // second group of 16 fma, third group load
    vfmacc.vf v16,  ft4, v4
    vlw.v v8, (bp0)         # b0'->v0
    addi bp0,bp0,64
    vfmacc.vf v20,  ft5, v4
    flw ft0, (ap0)          # a0'->ft0
    addi ap0, ap0, 4
    vfmacc.vf v24,  ft6, v4
    flw ft1, (ap1)          # a1'->ft1
    addi ap1, ap1, 4
    vfmacc.vf v28,  ft7, v4
    flw ft2, (ap2)          # a2'->ft2
    addi ap2, ap2, 4
    flw ft3, (ap3)          # a3'->ft3
    addi ap3, ap3, 4
    // third group of 16 fma, fourth group load
    vfmacc.vf v16,  ft0, v8
    vlw.v v12, (bp0)         # b0'->v0
    addi bp0,bp0,64
    vfmacc.vf v20,  ft1, v8
    flw ft4, (ap0)          # a0'->ft0
    addi ap0, ap0, 4
    vfmacc.vf v24,  ft2, v8
    flw ft5, (ap1)          # a1'->ft1
    addi ap1, ap1, 4
    vfmacc.vf v28,  ft3, v8
    flw ft6, (ap2)          # a2'->ft2
    addi ap2, ap2, 4
    flw ft7, (ap3)          # a3'->ft3
    addi ap3, ap3, 4
    // fourth group of 16 fma, no group load
    vfmacc.vf v16,  ft4, v12
    vfmacc.vf v20,  ft5, v12
    vfmacc.vf v24,  ft6, v12
    vfmacc.vf v28,  ft7, v12
.k2_tail:
    slti t6, kt, 2          # kt < 2
    bnez t6, .k1_tail
    flw ft0, (ap0)
    addi ap0, ap0, 4
    vlw.v v0, (bp0)
    addi bp0,bp0,64
    flw ft1, (ap1)
    addi ap1, ap1, 4
    flw ft2, (ap2)
    addi ap2, ap2, 4
    flw ft3, (ap3)
    addi ap3, ap3, 4
    // first group of 16 fma, second group load
    vfmacc.vf v16,  ft0, v0
    vlw.v v4, (bp0)         # b0'->v4
    addi bp0,bp0,64
    vfmacc.vf v20,  ft1, v0
    flw ft4, (ap0)          # a0'->ft4
    addi ap0, ap0, 4
    vfmacc.vf v24,  ft2, v0
    flw ft5, (ap1)          # a1'->ft5
    addi ap1, ap1, 4
    vfmacc.vf v28,  ft3, v0
    flw ft6, (ap2)          # a2'->ft6
    addi ap2, ap2, 4
    flw ft7, (ap3)          # a3'->ft7
    addi ap3, ap3, 4
    // second group of 16 fma, third group load
    vfmacc.vf v16,  ft4, v4
    vfmacc.vf v20,  ft5, v4
    vfmacc.vf v24,  ft6, v4
    vfmacc.vf v28,  ft7, v4
.k1_tail:
    slti t6, kt, 1          # kt < 1
    bnez t6, .store_tile
    flw ft0, (ap0)
    addi ap0, ap0, 4
    vlw.v v0, (bp0)
    addi bp0,bp0,64
    flw ft1, (ap1)
    addi ap1, ap1, 4
    flw ft2, (ap2)
    addi ap2, ap2, 4
    flw ft3, (ap3)
    addi ap3, ap3, 4
    vfmacc.vf v16,  ft0, v0
    vfmacc.vf v20,  ft1, v0
    vfmacc.vf v24,  ft2, v0
    vfmacc.vf v28,  ft3, v0
.store_tile:
    add cp0, cp0, cp_offset
    vsw.v v16, (cp0)
    addi cp0, cp0, 64

    vsw.v v20, (cp1)
    addi cp1, cp1, 64

    vsw.v v24, (cp2)
    addi cp2, cp2, 64

    vsw.v v28, (cp3)
    addi cp3, cp3, 64
    j .end

.end:
    ld s0, 96(sp)
    ld s1, 88(sp)
    ld s2, 80(sp)
    ld s3, 72(sp)
    ld s4, 64(sp)
    ld s5, 56(sp)
    ld s6, 48(sp)
    ld s7, 40(sp)
    ld s8, 32(sp)
    ld s9, 24(sp)
    ld s10, 16(sp)
    ld s11, 8(sp)
    addi sp, sp, FRAMESIZE
    ret
