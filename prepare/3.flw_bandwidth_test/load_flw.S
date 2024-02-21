/*************************************************
 *
 *  Created by Aidget on 2022/11/30.           
 *  Copyright © 2022,  developed by Midea AIIC 
 *
 *************************************************/

# void load(size_t mc,         // nr <= 16, a0
#           size_t nc,         // mr <= 4,  a1
#           const float* a,    // mr * k,   a3
#           float* c          // mr * nr,  a5
#                 );
.global load
.type load, @function

#define mc a0
#define nc a1
#define ap a2
#define cp a3

#define mt       t0
#define nt       t1
#define ap0      t2

load:
    # mv s3, nc
    # vsetvli s2, s3, e32, m4
    mv mt, mc
    mv nt, nc
    mv ap0, ap

.start:
.loop1:
    mv mt, mc
    addi nc, nc, -1
    slti t6, nc, 0      # nc < 0, t6 = 1
    bnez t6, .end
.loop2:
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4
    flw ft0, (ap0)
    addi ap0, ap0, 4

    addi mt, mt, -32
    slti t6, mt, 1     # nt < 1, t6 = 1
    beqz t6, .loop2
    
    j .loop1

.end:
    ret
