// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
// Copyright (c) 2018-2019, The TurtleCoin Developers
//
// Please see the included LICENSE file for more information.

#include "Context.h"

#include <string.h>

void makecontext(uctx *ucp, void (*func)(void), intptr_t arg)
{
    long *sp;

    memset(&ucp->uc_mcontext, 0, sizeof ucp->uc_mcontext);

#ifdef __aarch64__
    /*
     * On arm64, x0 is both the first argument and the return-value register.
     * setmcontext always loads x0 from offset 56 (mc_x6, aliased as mc_rdi).
     * Store arg there so the coroutine entry function receives it in x0.
     *
     * arm64 uses the link register (x30) for returns, not a stack slot.
     * mc_x30 is left zero (from memset); if the coroutine ever returns it
     * will branch to address 0 — the same undefined behaviour as x86_64
     * where a 0 return address is pushed onto the stack.
     */
    ucp->uc_mcontext.mc_rdi = (long)arg;           /* → mc_x6, offset 56 */

    sp = (long *)ucp->uc_stack.ss_sp + ucp->uc_stack.ss_size / sizeof(long);
    sp = (void *)((uintptr_t)sp - (uintptr_t)sp % 16); /* 16-align for ABI */
    /* No return-address slot pushed: arm64 returns via lr, not stack */
#else
    /*
     * x86_64: first argument goes in rdi; return address (0) pushed on stack.
     */
    ucp->uc_mcontext.mc_rdi = (long)arg;

    sp = (long *)ucp->uc_stack.ss_sp + ucp->uc_stack.ss_size / sizeof(long);
    sp -= 1;
    sp = (void *)((uintptr_t)sp - (uintptr_t)sp % 16); /* 16-align for OS X */
    *--sp = 0;                                          /* return address */
#endif

    ucp->uc_mcontext.mc_rip = (long)func;
    ucp->uc_mcontext.mc_rsp = (long)sp;
}

int swapcontext(uctx *oucp, const uctx *ucp)
{
    if (getcontext(oucp) == 0)
        setcontext(ucp);
    return 0;
}
