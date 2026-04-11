#ifdef __aarch64__

/*
 * arm64 (Apple Silicon) context switch implementation.
 *
 * Struct mcontext offsets used (see Context.h):
 *   56  – mc_x6  (mc_rax alias): return-value trick slot / new-context arg
 *  160  – mc_pc  (mc_rip alias): saved PC / entry point
 *  184  – mc_sp  (mc_rsp alias): saved stack pointer
 *  192  – mc_x19 through mc_x30 (callee-saved regs, 8 bytes each)
 */

.globl _getmcontext
_getmcontext:
    /* x0 = mctx * */

    /* Save return-value trick slot as 1.
       When this context is later restored by setmcontext, x0 will be 1,
       making swapcontext's "if (getcontext() == 0)" branch fall through
       and return 0 to the original caller. */
    mov     x1, #1
    str     x1, [x0, #56]      /* mc_x6 = 1 */

    /* Save return address (lr) as the resumption PC */
    str     x30, [x0, #160]    /* mc_pc = lr */

    /* Save stack pointer */
    mov     x1, sp
    str     x1, [x0, #184]     /* mc_sp = sp */

    /* Save callee-saved registers */
    str     x19, [x0, #192]
    str     x20, [x0, #200]
    str     x21, [x0, #208]
    str     x22, [x0, #216]
    str     x23, [x0, #224]
    str     x24, [x0, #232]
    str     x25, [x0, #240]
    str     x26, [x0, #248]
    str     x27, [x0, #256]
    str     x28, [x0, #264]
    str     x29, [x0, #272]    /* frame pointer */
    str     x30, [x0, #280]    /* link register */

    /* Return 0 on the direct call */
    mov     x0, #0
    ret

.globl _setmcontext
_setmcontext:
    /* x0 = const mctx * */

    /* Load target PC into scratch register before x0 is overwritten */
    ldr     x16, [x0, #160]    /* x16 = mc_pc */

    /* Restore stack pointer */
    ldr     x1,  [x0, #184]    /* x1  = mc_sp */
    mov     sp,  x1

    /* Restore callee-saved registers */
    ldr     x19, [x0, #192]
    ldr     x20, [x0, #200]
    ldr     x21, [x0, #208]
    ldr     x22, [x0, #216]
    ldr     x23, [x0, #224]
    ldr     x24, [x0, #232]
    ldr     x25, [x0, #240]
    ldr     x26, [x0, #248]
    ldr     x27, [x0, #256]
    ldr     x28, [x0, #264]
    ldr     x29, [x0, #272]    /* frame pointer */
    ldr     x30, [x0, #280]    /* link register */

    /* Load return value / first argument (must be last — x0 is the ctx ptr) */
    ldr     x0,  [x0, #56]     /* x0 = mc_x6 (1 for saved ctx; arg for new ctx) */

    /* Jump to saved PC */
    br      x16

#else   /* x86_64 */

.globl _setmcontext
_setmcontext:
	movq	16(%rdi), %rsi
	movq	24(%rdi), %rdx
	movq	32(%rdi), %rcx
	movq	40(%rdi), %r8
	movq	48(%rdi), %r9
	movq	56(%rdi), %rax
	movq	64(%rdi), %rbx
	movq	72(%rdi), %rbp
	movq	80(%rdi), %r10
	movq	88(%rdi), %r11
	movq	96(%rdi), %r12
	movq	104(%rdi), %r13
	movq	112(%rdi), %r14
	movq	120(%rdi), %r15
	movq	184(%rdi), %rsp
	pushq	160(%rdi)	/* new %eip */
	movq	8(%rdi), %rdi
	ret

.globl _getmcontext
_getmcontext:
	movq	%rdi, 8(%rdi)
	movq	%rsi, 16(%rdi)
	movq	%rdx, 24(%rdi)
	movq	%rcx, 32(%rdi)
	movq	%r8, 40(%rdi)
	movq	%r9, 48(%rdi)
	movq	$1, 56(%rdi)	/* %rax */
	movq	%rbx, 64(%rdi)
	movq	%rbp, 72(%rdi)
	movq	%r10, 80(%rdi)
	movq	%r11, 88(%rdi)
	movq	%r12, 96(%rdi)
	movq	%r13, 104(%rdi)
	movq	%r14, 112(%rdi)
	movq	%r15, 120(%rdi)

	movq	(%rsp), %rcx	/* %rip */
	movq	%rcx, 160(%rdi)
	leaq	8(%rsp), %rcx	/* %rsp */
	movq	%rcx, 184(%rdi)

	movq	32(%rdi), %rcx	/* restore %rcx */
	movq	$0, %rax
	ret

#endif
