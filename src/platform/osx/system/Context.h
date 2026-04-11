// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
// Copyright (c) 2018-2019, The TurtleCoin Developers
//
// Please see the included LICENSE file for more information.

#pragma once

#define setcontext(u) setmcontext(&(u)->uc_mcontext)
#define getcontext(u) getmcontext(&(u)->uc_mcontext)

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdlib.h>

    typedef struct mcontext mctx;
    typedef struct ucontext uctx;

    extern int  swapcontext(uctx *, const uctx *);
    extern void makecontext(uctx *, void (*)(void), intptr_t);
    extern int  getmcontext(mctx *);
    extern void setmcontext(const mctx *);

#ifdef __aarch64__

    /*
     * arm64 / Apple Silicon context structure.
     *
     * The first 24 `long` fields (offsets 0–184) mirror the x86_64 layout so
     * that Context.c can use the same mc_rdi / mc_rip / mc_rsp aliases on
     * both architectures.  arm64 callee-saved registers follow at offset 192.
     *
     * Key offsets (also used by asm.s):
     *   56  – mc_x6  (aliased as mc_rax): return-value trick / new-context arg
     *  160  – mc_pc  (aliased as mc_rip): saved PC / entry point
     *  184  – mc_sp  (aliased as mc_rsp): saved stack pointer
     *  192+ – mc_x19 … mc_x30: callee-saved registers
     */
    struct mcontext
    {
        long mc_onstack;        /*   0 */
        long mc_x0;             /*   8 */
        long mc_x1;             /*  16 */
        long mc_x2;             /*  24 */
        long mc_x3;             /*  32 */
        long mc_x4;             /*  40 */
        long mc_x5;             /*  48 */
        long mc_x6;             /*  56  ← mc_rax alias: return-value / arg */
        long mc_x7;             /*  64 */
        long mc_x8;             /*  72 */
        long mc_x9;             /*  80 */
        long mc_x10;            /*  88 */
        long mc_x11;            /*  96 */
        long mc_x12;            /* 104 */
        long mc_x13;            /* 112 */
        long mc_x14;            /* 120 */
        long mc_trapno;         /* 128 */
        long mc_addr;           /* 136 */
        long mc_flags;          /* 144 */
        long mc_err;            /* 152 */
        long mc_pc;             /* 160  ← mc_rip alias: entry point / return addr */
        long mc_cs;             /* 168 */
        long mc_nzcv;           /* 176 */
        long mc_sp;             /* 184  ← mc_rsp alias: stack pointer */
        /* arm64 callee-saved registers */
        long mc_x19;            /* 192 */
        long mc_x20;            /* 200 */
        long mc_x21;            /* 208 */
        long mc_x22;            /* 216 */
        long mc_x23;            /* 224 */
        long mc_x24;            /* 232 */
        long mc_x25;            /* 240 */
        long mc_x26;            /* 248 */
        long mc_x27;            /* 256 */
        long mc_x28;            /* 264 */
        long mc_x29;            /* 272 – frame pointer */
        long mc_x30;            /* 280 – link register */
        long mc_len;
        long mc_fpformat;
        long mc_ownedfp;
        long mc_fpstate[64];
        long mc_spare[8];
    };

    /* Aliases so Context.c compiles unchanged on both architectures */
#define mc_rdi mc_x6   /* makecontext stores arg here; setmcontext returns it as x0 */
#define mc_rip mc_pc
#define mc_rsp mc_sp

#else   /* x86_64 */

    struct mcontext
    {
        /*
         * The first 20 fields must match the definition of
         * sigcontext. So that we can support sigcontext
         * and ucontext_t at the same time.
         */
        long mc_onstack;        /* XXX - sigcontext compat. */
        long mc_rdi;            /* machine state (struct trapframe) */
        long mc_rsi;
        long mc_rdx;
        long mc_rcx;
        long mc_r8;
        long mc_r9;
        long mc_rax;
        long mc_rbx;
        long mc_rbp;
        long mc_r10;
        long mc_r11;
        long mc_r12;
        long mc_r13;
        long mc_r14;
        long mc_r15;
        long mc_trapno;
        long mc_addr;
        long mc_flags;
        long mc_err;
        long mc_rip;
        long mc_cs;
        long mc_rflags;
        long mc_rsp;
        long mc_ss;

        long mc_len;
#define _MC_FPFMT_NODEV 0x10000
#define _MC_FPFMT_XMM   0x10002
        long mc_fpformat;
#define _MC_FPOWNED_NONE 0x20000
#define _MC_FPOWNED_FPU  0x20001
#define _MC_FPOWNED_PCB  0x20002
        long mc_ownedfp;
        long mc_fpstate[64];
        long mc_spare[8];
    };

#endif  /* __aarch64__ */

    struct ucontext
    {
        sigset_t         uc_sigmask;
        mctx             uc_mcontext;
        struct __ucontext *uc_link;
        stack_t          uc_stack;
        int              __spare__[8];
    };

#ifdef __cplusplus
}
#endif
