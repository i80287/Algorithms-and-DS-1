	.file	"f1.cpp"
	.text
	.align 2
	.p2align 4
	.def	_ZNSt6vectorIZL16find_clear_cycleRKS_IN12_GLOBAL__N_16node_tESaIS1_EEE6StatusSaIS6_EED2Ev;	.scl	3;	.type	32;	.endef
	.seh_proc	_ZNSt6vectorIZL16find_clear_cycleRKS_IN12_GLOBAL__N_16node_tESaIS1_EEE6StatusSaIS6_EED2Ev
_ZNSt6vectorIZL16find_clear_cycleRKS_IN12_GLOBAL__N_16node_tESaIS1_EEE6StatusSaIS6_EED2Ev:
.LFB2964:
	.seh_endprologue
	movq	(%rcx), %rax
	testq	%rax, %rax
	je	.L1
	movq	16(%rcx), %rdx
	movq	%rax, %rcx
	subq	%rax, %rdx
	jmp	_ZdlPvy
.L1:
	ret
	.seh_endproc
	.def	_ZNSt6vectorIZL16find_clear_cycleRKS_IN12_GLOBAL__N_16node_tESaIS1_EEE6StatusSaIS6_EED1Ev;	.scl	3;	.type	32;	.endef
	.set	_ZNSt6vectorIZL16find_clear_cycleRKS_IN12_GLOBAL__N_16node_tESaIS1_EEE6StatusSaIS6_EED1Ev,_ZNSt6vectorIZL16find_clear_cycleRKS_IN12_GLOBAL__N_16node_tESaIS1_EEE6StatusSaIS6_EED2Ev
	.section .rdata,"dr"
	.align 8
.LC0:
	.ascii "cannot create std::vector larger than max_size()\0"
	.text
	.align 2
	.p2align 4
	.def	_ZNSt6vectorIySaIyEEC1EyRKS0_.isra.0;	.scl	3;	.type	32;	.endef
	.seh_proc	_ZNSt6vectorIySaIyEEC1EyRKS0_.isra.0
_ZNSt6vectorIySaIyEEC1EyRKS0_.isra.0:
.LFB3521:
	pushq	%rbp
	.seh_pushreg	%rbp
	pushq	%rdi
	.seh_pushreg	%rdi
	pushq	%rsi
	.seh_pushreg	%rsi
	pushq	%rbx
	.seh_pushreg	%rbx
	subq	$40, %rsp
	.seh_stackalloc	40
	.seh_endprologue
	movq	%rdx, %rax
	movq	%rcx, %rbx
	movq	%rdx, %rsi
	shrq	$60, %rax
	jne	.L12
	pxor	%xmm0, %xmm0
	movq	$0, 16(%rcx)
	movups	%xmm0, (%rcx)
	testq	%rdx, %rdx
	je	.L13
	leaq	0(,%rdx,8), %rdi
	movq	%rdi, %rcx
	call	_Znwy
	xorl	%edx, %edx
	subq	$1, %rsi
	leaq	(%rax,%rdi), %rbp
	movq	%rax, (%rbx)
	leaq	8(%rax), %rcx
	movq	%rbp, 16(%rbx)
	movq	%rdx, (%rax)
	je	.L7
	cmpq	%rcx, %rbp
	je	.L8
	leaq	-8(%rdi), %r8
	xorl	%edx, %edx
	call	memset
.L8:
	movq	%rbp, %rcx
.L7:
	movq	%rcx, 8(%rbx)
	addq	$40, %rsp
	popq	%rbx
	popq	%rsi
	popq	%rdi
	popq	%rbp
	ret
.L13:
	xorl	%ecx, %ecx
	movq	%rcx, (%rbx)
	movq	%rcx, 16(%rbx)
	xorl	%ecx, %ecx
	jmp	.L7
.L12:
	leaq	.LC0(%rip), %rcx
	call	_ZSt20__throw_length_errorPKc
	nop
	.seh_endproc
	.section .rdata,"dr"
.LC1:
	.ascii "vector::_M_realloc_insert\0"
	.text
	.align 2
	.p2align 4
	.def	_ZNSt6vectorIZL16find_clear_cycleRKS_IN12_GLOBAL__N_16node_tESaIS1_EEE10StackFrameSaIS6_EE12emplace_backIJRKyZL16find_clear_cycleS5_E5StateEEERS6_DpOT_.constprop.0.isra.0;	.scl	3;	.type	32;	.endef
	.seh_proc	_ZNSt6vectorIZL16find_clear_cycleRKS_IN12_GLOBAL__N_16node_tESaIS1_EEE10StackFrameSaIS6_EE12emplace_backIJRKyZL16find_clear_cycleS5_E5StateEEERS6_DpOT_.constprop.0.isra.0
_ZNSt6vectorIZL16find_clear_cycleRKS_IN12_GLOBAL__N_16node_tESaIS1_EEE10StackFrameSaIS6_EE12emplace_backIJRKyZL16find_clear_cycleS5_E5StateEEERS6_DpOT_.constprop.0.isra.0:
.LFB3524:
	pushq	%r12
	.seh_pushreg	%r12
	pushq	%rbp
	.seh_pushreg	%rbp
	pushq	%rdi
	.seh_pushreg	%rdi
	pushq	%rsi
	.seh_pushreg	%rsi
	pushq	%rbx
	.seh_pushreg	%rbx
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	movq	8(%rcx), %rbx
	movq	%rcx, %rsi
	movq	16(%rcx), %rcx
	cmpq	%rcx, %rbx
	je	.L15
	movq	(%rdx), %rax
	addq	$16, %rbx
	movq	$0, -8(%rbx)
	movq	%rax, -16(%rbx)
	movq	%rbx, 8(%rsi)
	addq	$32, %rsp
	popq	%rbx
	popq	%rsi
	popq	%rdi
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L15:
	movabsq	$576460752303423487, %r9
	movq	(%rsi), %rbp
	movq	%rbx, %r12
	subq	%rbp, %r12
	movq	%r12, %rax
	sarq	$4, %rax
	cmpq	%r9, %rax
	je	.L34
	cmpq	%rbp, %rbx
	je	.L35
	leaq	(%rax,%rax), %r9
	cmpq	%rax, %r9
	jb	.L28
	testq	%r9, %r9
	jne	.L36
	movq	(%rdx), %xmm0
	xorl	%edi, %edi
	xorl	%eax, %eax
	movhps	(%r8), %xmm0
	movups	%xmm0, (%r12)
	xorl	%r12d, %r12d
.L23:
	movq	%rbp, %rdx
	.p2align 4,,10
	.p2align 3
.L25:
	movdqu	(%rdx), %xmm1
	addq	$16, %rdx
	addq	$16, %rax
	movups	%xmm1, -16(%rax)
	cmpq	%rdx, %rbx
	jne	.L25
	subq	%rbp, %rbx
	leaq	16(%r12,%rbx), %rbx
.L24:
	testq	%rbp, %rbp
	je	.L26
	subq	%rbp, %rcx
	movq	%rcx, %rdx
	movq	%rbp, %rcx
	call	_ZdlPvy
.L26:
	movq	%r12, (%rsi)
	movq	%rbx, 8(%rsi)
	movq	%rdi, 16(%rsi)
	addq	$32, %rsp
	popq	%rbx
	popq	%rsi
	popq	%rdi
	popq	%rbp
	popq	%r12
	ret
	.p2align 4,,10
	.p2align 3
.L35:
	addq	$1, %rax
	jc	.L28
	movabsq	$576460752303423487, %rcx
	cmpq	%rcx, %rax
	movq	%rcx, %rdi
	cmovbe	%rax, %rdi
	salq	$4, %rdi
.L21:
	movq	%rdi, %rcx
	movq	%r8, 96(%rsp)
	movq	%rdx, 88(%rsp)
	call	_Znwy
	movq	88(%rsp), %rdx
	movq	96(%rsp), %r8
	movq	16(%rsi), %rcx
	addq	%rax, %rdi
	movq	(%rdx), %xmm0
	movhps	(%r8), %xmm0
	movups	%xmm0, (%rax,%r12)
	movq	%rax, %r12
	cmpq	%rbp, %rbx
	jne	.L23
	leaq	16(%rax), %rbx
	jmp	.L24
	.p2align 4,,10
	.p2align 3
.L28:
	movabsq	$9223372036854775792, %rdi
	jmp	.L21
.L36:
	movabsq	$576460752303423487, %rax
	cmpq	%rax, %r9
	cmova	%rax, %r9
	movq	%r9, %rdi
	salq	$4, %rdi
	jmp	.L21
.L34:
	leaq	.LC1(%rip), %rcx
	call	_ZSt20__throw_length_errorPKc
	nop
	.seh_endproc
	.align 2
	.p2align 4
	.def	_ZNSt6vectorIZL16find_clear_cycleRKS_IN12_GLOBAL__N_16node_tESaIS1_EEE10StackFrameSaIS6_EED2Ev;	.scl	3;	.type	32;	.endef
	.seh_proc	_ZNSt6vectorIZL16find_clear_cycleRKS_IN12_GLOBAL__N_16node_tESaIS1_EEE10StackFrameSaIS6_EED2Ev
_ZNSt6vectorIZL16find_clear_cycleRKS_IN12_GLOBAL__N_16node_tESaIS1_EEE10StackFrameSaIS6_EED2Ev:
.LFB2976:
	.seh_endprologue
	movq	(%rcx), %rax
	testq	%rax, %rax
	je	.L37
	movq	16(%rcx), %rdx
	movq	%rax, %rcx
	subq	%rax, %rdx
	jmp	_ZdlPvy
.L37:
	ret
	.seh_endproc
	.def	_ZNSt6vectorIZL16find_clear_cycleRKS_IN12_GLOBAL__N_16node_tESaIS1_EEE10StackFrameSaIS6_EED1Ev;	.scl	3;	.type	32;	.endef
	.set	_ZNSt6vectorIZL16find_clear_cycleRKS_IN12_GLOBAL__N_16node_tESaIS1_EEE10StackFrameSaIS6_EED1Ev,_ZNSt6vectorIZL16find_clear_cycleRKS_IN12_GLOBAL__N_16node_tESaIS1_EEE10StackFrameSaIS6_EED2Ev
	.align 2
	.p2align 4
	.def	_ZNSt6vectorIN12_GLOBAL__N_16node_tESaIS1_EED2Ev;	.scl	3;	.type	32;	.endef
	.seh_proc	_ZNSt6vectorIN12_GLOBAL__N_16node_tESaIS1_EED2Ev
_ZNSt6vectorIN12_GLOBAL__N_16node_tESaIS1_EED2Ev:
.LFB2996:
	.seh_endprologue
	movq	(%rcx), %rax
	testq	%rax, %rax
	je	.L39
	movq	16(%rcx), %rdx
	movq	%rax, %rcx
	subq	%rax, %rdx
	jmp	_ZdlPvy
.L39:
	ret
	.seh_endproc
	.def	_ZNSt6vectorIN12_GLOBAL__N_16node_tESaIS1_EED1Ev;	.scl	3;	.type	32;	.endef
	.set	_ZNSt6vectorIN12_GLOBAL__N_16node_tESaIS1_EED1Ev,_ZNSt6vectorIN12_GLOBAL__N_16node_tESaIS1_EED2Ev
	.section	.text$_ZNSt6vectorIySaIyEED1Ev,"x"
	.linkonce discard
	.align 2
	.p2align 4
	.globl	_ZNSt6vectorIySaIyEED1Ev
	.def	_ZNSt6vectorIySaIyEED1Ev;	.scl	2;	.type	32;	.endef
	.seh_proc	_ZNSt6vectorIySaIyEED1Ev
_ZNSt6vectorIySaIyEED1Ev:
.LFB2953:
	.seh_endprologue
	movq	(%rcx), %rax
	testq	%rax, %rax
	je	.L41
	movq	16(%rcx), %rdx
	movq	%rax, %rcx
	subq	%rax, %rdx
	jmp	_ZdlPvy
	.p2align 4,,10
	.p2align 3
.L41:
	ret
	.seh_endproc
	.def	__main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
.LC3:
	.ascii "TAK\0"
.LC4:
	.ascii "NIE\0"
	.section	.text.startup,"x"
	.p2align 4
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
.LFB2664:
	pushq	%r15
	.seh_pushreg	%r15
	pushq	%r14
	.seh_pushreg	%r14
	pushq	%r13
	.seh_pushreg	%r13
	pushq	%r12
	.seh_pushreg	%r12
	pushq	%rbp
	.seh_pushreg	%rbp
	pushq	%rdi
	.seh_pushreg	%rdi
	pushq	%rsi
	.seh_pushreg	%rsi
	pushq	%rbx
	.seh_pushreg	%rbx
	subq	$264, %rsp
	.seh_stackalloc	264
	.seh_endprologue
	call	__main
	xorl	%ecx, %ecx
.LEHB0:
	call	_ZNSt8ios_base15sync_with_stdioEb
.LEHE0:
	movq	.refptr._ZSt3cin(%rip), %rax
	pxor	%xmm0, %xmm0
	movl	$32768, %ecx
	movq	$0, 144(%rsp)
	movq	$0, 232(%rax)
	movups	%xmm0, 128(%rsp)
.LEHB1:
	call	_Znwy
	movq	%rax, %r12
	movq	%rax, 128(%rsp)
	leaq	72(%rsp), %rdx
	leaq	32768(%rax), %rax
	movq	$0, 16(%r12)
	movq	.refptr._ZSt3cin(%rip), %rcx
	movb	$0, 24(%r12)
	movq	$0, (%r12)
	movq	$0, 8(%r12)
	movq	$0, 48(%r12)
	movb	$0, 56(%r12)
	movq	$0, 32(%r12)
	movq	$0, 40(%r12)
	movq	$0, 80(%r12)
	movb	$0, 88(%r12)
	movq	$0, 64(%r12)
	movq	$0, 72(%r12)
	movq	%rax, 144(%rsp)
	leaq	96(%r12), %rax
	movq	%rax, 136(%rsp)
	call	_ZNSi10_M_extractIyEERSiRT_
.LEHE1:
	leaq	240(%rsp), %rax
	cmpq	$0, 72(%rsp)
	movq	$0, 232(%rsp)
	movq	%rax, 224(%rsp)
	leaq	224(%rsp), %rax
	movb	$0, 240(%rsp)
	movq	%rax, 48(%rsp)
	je	.L44
	.p2align 4,,10
	.p2align 3
.L59:
	movq	48(%rsp), %rdx
	movq	.refptr._ZSt3cin(%rip), %rcx
.LEHB2:
	call	_ZStrsIcSt11char_traitsIcESaIcEERSt13basic_istreamIT_T0_ES7_RNSt7__cxx1112basic_stringIS4_S5_T1_EE
.LEHE2:
	movq	224(%rsp), %rbx
	movq	232(%rsp), %r15
	addq	%rbx, %r15
	cmpq	%r15, %rbx
	je	.L89
	movl	$2, %edi
	jmp	.L58
	.p2align 4,,10
	.p2align 3
.L46:
	addq	$1, %rbx
	cmpq	%rbx, %r15
	je	.L128
.L58:
	movzbl	(%rbx), %esi
	salq	$5, %rdi
	leaq	(%r12,%rdi), %rax
	movq	%rdi, %r13
	subq	$48, %rsi
	movq	(%rax,%rsi,8), %rdi
	testq	%rdi, %rdi
	jne	.L46
	movq	136(%rsp), %rbp
	movq	%rbp, %rcx
	subq	%r12, %rcx
	movq	%rcx, %r14
	movq	%rcx, 32(%rsp)
	movq	144(%rsp), %rcx
	sarq	$5, %r14
	movq	%rcx, 40(%rsp)
	movq	%r14, %rdi
	cmpq	%rcx, %rbp
	je	.L47
	movq	$0, 0(%rbp)
	addq	$32, %rbp
	movq	$0, -24(%rbp)
	movq	$0, -16(%rbp)
	movb	$0, -8(%rbp)
	movq	%rbp, 136(%rsp)
.L48:
	addq	$1, %rbx
	movq	%r14, (%rax,%rsi,8)
	cmpq	%rbx, %r15
	jne	.L58
.L128:
	salq	$5, %rdi
.L45:
	subq	$1, 72(%rsp)
	movb	$1, 24(%r12,%rdi)
	jne	.L59
.L44:
	movq	48(%rsp), %r15
	leaq	192(%rsp), %rbx
	movq	%r15, %rcx
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv
	movq	136(%rsp), %rbp
	movq	%rbx, %rcx
	subq	%r12, %rbp
	movq	%rbp, %rdi
	sarq	$5, %rdi
	movq	%rdi, %rdx
.LEHB3:
	call	_ZNSt6vectorIySaIyEEC1EyRKS0_.isra.0
.LEHE3:
	movq	192(%rsp), %rsi
	movdqu	.LC2(%rip), %xmm0
	movq	%rdi, %rdx
	movq	%r15, %rcx
	movq	$1, 16(%rsi)
	movq	$2, 80(%r12)
	movups	%xmm0, 32(%r12)
.LEHB4:
	call	_ZNSt6vectorIySaIyEEC1EyRKS0_.isra.0
.LEHE4:
	movq	224(%rsp), %r10
	movl	$1, %r8d
	xorl	%ecx, %ecx
	movq	$2, (%r10)
	jmp	.L68
	.p2align 4,,10
	.p2align 3
.L130:
	movq	%r9, (%rdx)
	movq	8(%rdx), %r9
	movq	8(%rax), %rax
	testq	%r9, %r9
	jne	.L64
.L131:
	movq	%rax, 8(%rdx)
	cmpq	%r8, %rcx
	jnb	.L129
.L68:
	addq	$1, %rcx
	movq	-8(%r10,%rcx,8), %rax
	movq	%rax, %rdx
	movq	(%rsi,%rax,8), %rax
	salq	$5, %rdx
	addq	%r12, %rdx
	salq	$5, %rax
	movq	(%rdx), %r11
	addq	%r12, %rax
	movq	(%rax), %r9
	testq	%r11, %r11
	je	.L130
	movq	%r9, %r13
	movq	%r9, (%rsi,%r11,8)
	salq	$5, %r13
	addq	%r12, %r13
	cmpb	$0, 24(%r13)
	movq	16(%r13), %r14
	jne	.L63
	cmpq	$2, %r9
	cmovne	%r14, %r9
.L63:
	movq	%r11, %r13
	salq	$5, %r13
	movq	%r9, 16(%r12,%r13)
	movq	%r11, (%r10,%r8,8)
	movq	8(%rdx), %r9
	addq	$1, %r8
	movq	8(%rax), %rax
	testq	%r9, %r9
	je	.L131
.L64:
	movq	%rax, %rdx
	movq	%rax, (%rsi,%r9,8)
	salq	$5, %rdx
	addq	%r12, %rdx
	cmpb	$0, 24(%rdx)
	movq	16(%rdx), %r11
	jne	.L67
	cmpq	$2, %rax
	cmovne	%r11, %rax
.L67:
	movq	%r9, %rdx
	salq	$5, %rdx
	movq	%rax, 16(%r12,%rdx)
	movq	%r9, (%r10,%r8,8)
	addq	$1, %r8
	cmpq	%r8, %rcx
	jb	.L68
.L129:
	movq	48(%rsp), %rcx
	call	_ZNSt6vectorIySaIyEED1Ev
	movq	%rbx, %rcx
	call	_ZNSt6vectorIySaIyEED1Ev
	leaq	.LC3(%rip), %rax
	movq	$3, 96(%rsp)
	movq	%rax, 104(%rsp)
	leaq	.LC4(%rip), %rax
	movq	$3, 112(%rsp)
	movq	%rax, 120(%rsp)
	testq	%rbp, %rbp
	js	.L132
	pxor	%xmm0, %xmm0
	movups	%xmm0, 168(%rsp)
	testq	%rdi, %rdi
	je	.L73
	salq	$2, %rdi
	movq	%rdi, %rcx
.LEHB5:
	call	_Znwy
.LEHE5:
	leaq	(%rax,%rdi), %rbp
	movq	%rdi, %r8
	xorl	%edx, %edx
	movq	%rax, %rcx
	movq	%rax, 160(%rsp)
	movq	%rax, %rsi
	movq	%rbp, 176(%rsp)
	call	memset
.L74:
	movq	48(%rsp), %r8
	pxor	%xmm0, %xmm0
	movq	%rbx, %rcx
	leaq	kRootNodeIndex(%rip), %rdx
	movq	%rbp, 168(%rsp)
	movq	$0, 208(%rsp)
	movq	$0, 224(%rsp)
	movups	%xmm0, 192(%rsp)
.LEHB6:
	call	_ZNSt6vectorIZL16find_clear_cycleRKS_IN12_GLOBAL__N_16node_tESaIS1_EEE10StackFrameSaIS6_EE12emplace_backIJRKyZL16find_clear_cycleS5_E5StateEEERS6_DpOT_.constprop.0.isra.0
.L127:
	movq	200(%rsp), %rax
	movq	192(%rsp), %r8
	.p2align 4,,10
	.p2align 3
.L126:
	cmpq	%rax, %r8
	je	.L75
.L137:
	movq	-8(%rax), %rdx
	movq	-16(%rax), %rcx
	cmpq	$1, %rdx
	je	.L76
	cmpq	$2, %rdx
	je	.L77
	testq	%rdx, %rdx
	jne	.L126
	movl	$1, (%rsi,%rcx,4)
	salq	$5, %rcx
	movq	(%r12,%rcx), %rcx
	movq	%rcx, %rdx
	movq	%rcx, 80(%rsp)
	salq	$5, %rdx
	addq	%r12, %rdx
	movdqu	16(%rdx), %xmm4
	movdqu	(%rdx), %xmm3
	movups	%xmm4, 240(%rsp)
	cmpb	$0, 248(%rsp)
	movups	%xmm3, 224(%rsp)
	movq	$1, -8(%rax)
	jne	.L126
	cmpq	$2, 240(%rsp)
	jne	.L126
	movl	(%rsi,%rcx,4), %edx
	cmpl	$1, %edx
	je	.L81
	cmpl	$2, %edx
	je	.L126
	xorl	%edx, %edx
	leaq	88(%rsp), %r8
	movq	%rbx, %rcx
	movq	%rdx, 88(%rsp)
	leaq	80(%rsp), %rdx
	call	_ZNSt6vectorIZL16find_clear_cycleRKS_IN12_GLOBAL__N_16node_tESaIS1_EEE10StackFrameSaIS6_EE12emplace_backIJRKyZL16find_clear_cycleS5_E5StateEEERS6_DpOT_.constprop.0.isra.0
.LEHE6:
	jmp	.L127
	.p2align 4,,10
	.p2align 3
.L47:
	movabsq	$288230376151711743, %rax
	cmpq	%rax, %r14
	je	.L133
	cmpq	%rbp, %r12
	je	.L134
	leaq	(%r14,%r14), %rax
	cmpq	%r14, %rax
	jb	.L91
	testq	%rax, %rax
	jne	.L135
	movq	32(%rsp), %rax
	xorl	%r8d, %r8d
	xorl	%r9d, %r9d
	movq	$0, (%rax)
	movq	$0, 8(%rax)
	movq	$0, 16(%rax)
	movb	$0, 24(%rax)
.L88:
	subq	%r12, %rbp
	movq	%r12, %rdx
	movq	%r8, %rax
	addq	%r8, %rbp
	.p2align 4,,10
	.p2align 3
.L56:
	movdqu	(%rdx), %xmm1
	movdqu	16(%rdx), %xmm2
	addq	$32, %rax
	addq	$32, %rdx
	movups	%xmm1, -32(%rax)
	movups	%xmm2, -16(%rax)
	cmpq	%rax, %rbp
	jne	.L56
.L55:
	testq	%r12, %r12
	je	.L57
	movq	40(%rsp), %rdx
	movq	%r12, %rcx
	movq	%r9, 56(%rsp)
	movq	%r8, 32(%rsp)
	subq	%r12, %rdx
	call	_ZdlPvy
	movq	56(%rsp), %r9
	movq	32(%rsp), %r8
	.p2align 4,,10
	.p2align 3
.L57:
	addq	$32, %rbp
	movq	%r8, 128(%rsp)
	leaq	(%r8,%r13), %rax
	movq	%r8, %r12
	movq	%rbp, 136(%rsp)
	movq	%r9, 144(%rsp)
	jmp	.L48
	.p2align 4,,10
	.p2align 3
.L134:
	movq	%r14, %rdx
	addq	$1, %rdx
	jc	.L91
	movabsq	$288230376151711743, %rax
	cmpq	%rax, %rdx
	cmovbe	%rdx, %rax
	salq	$5, %rax
	movq	%rax, 56(%rsp)
	movq	%rax, %rcx
.L53:
.LEHB7:
	call	_Znwy
.LEHE7:
	movq	56(%rsp), %r9
	movq	%rax, %r8
	addq	%rax, %r9
	movq	32(%rsp), %rax
	addq	%r8, %rax
	movq	$0, (%rax)
	movq	$0, 8(%rax)
	movq	$0, 16(%rax)
	movb	$0, 24(%rax)
	cmpq	%rbp, %r12
	jne	.L88
	movq	%r8, %rbp
	jmp	.L55
	.p2align 4,,10
	.p2align 3
.L89:
	movl	$64, %edi
	jmp	.L45
	.p2align 4,,10
	.p2align 3
.L91:
	movabsq	$9223372036854775776, %rax
	movq	%rax, 56(%rsp)
	movq	%rax, %rcx
	jmp	.L53
.L73:
	xorl	%ecx, %ecx
	xorl	%esi, %esi
	xorl	%ebp, %ebp
	movq	%rcx, 160(%rsp)
	movq	%rcx, 176(%rsp)
	jmp	.L74
	.p2align 4,,10
	.p2align 3
.L76:
	salq	$5, %rcx
	movq	8(%r12,%rcx), %rcx
	movq	%rcx, %rdx
	movq	%rcx, 80(%rsp)
	salq	$5, %rdx
	addq	%r12, %rdx
	movdqu	16(%rdx), %xmm3
	movdqu	(%rdx), %xmm5
	movups	%xmm3, 240(%rsp)
	cmpb	$0, 248(%rsp)
	movups	%xmm5, 224(%rsp)
	movq	$2, -8(%rax)
	jne	.L82
	cmpq	$2, 240(%rsp)
	je	.L136
.L82:
	movq	200(%rsp), %rax
	cmpq	%rax, %r8
	jne	.L137
.L75:
	movq	%rbx, %rcx
	call	_ZNSt6vectorIZL16find_clear_cycleRKS_IN12_GLOBAL__N_16node_tESaIS1_EEE10StackFrameSaIS6_EED1Ev
	leaq	160(%rsp), %rcx
	call	_ZNSt6vectorIZL16find_clear_cycleRKS_IN12_GLOBAL__N_16node_tESaIS1_EEE6StatusSaIS6_EED1Ev
	leaq	112(%rsp), %rax
.L85:
	movq	8(%rax), %rdx
	movq	.refptr._ZSt4cout(%rip), %rcx
	movl	$3, %r8d
.LEHB8:
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x
.LEHE8:
	leaq	128(%rsp), %rcx
	call	_ZNSt6vectorIN12_GLOBAL__N_16node_tESaIS1_EED1Ev
	xorl	%eax, %eax
	addq	$264, %rsp
	popq	%rbx
	popq	%rsi
	popq	%rdi
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
	.p2align 4,,10
	.p2align 3
.L77:
	subq	$16, %rax
	movl	$2, (%rsi,%rcx,4)
	movq	%rax, 200(%rsp)
	jmp	.L126
.L136:
	movl	(%rsi,%rcx,4), %eax
	cmpl	$1, %eax
	je	.L81
	cmpl	$2, %eax
	je	.L82
	xorl	%eax, %eax
	leaq	80(%rsp), %rdx
	leaq	88(%rsp), %r8
	movq	%rbx, %rcx
	movq	%rax, 88(%rsp)
.LEHB9:
	call	_ZNSt6vectorIZL16find_clear_cycleRKS_IN12_GLOBAL__N_16node_tESaIS1_EEE10StackFrameSaIS6_EE12emplace_backIJRKyZL16find_clear_cycleS5_E5StateEEERS6_DpOT_.constprop.0.isra.0
.LEHE9:
	movq	192(%rsp), %r8
	jmp	.L82
.L81:
	movq	%rbx, %rcx
	call	_ZNSt6vectorIZL16find_clear_cycleRKS_IN12_GLOBAL__N_16node_tESaIS1_EEE10StackFrameSaIS6_EED1Ev
	leaq	160(%rsp), %rcx
	call	_ZNSt6vectorIZL16find_clear_cycleRKS_IN12_GLOBAL__N_16node_tESaIS1_EEE6StatusSaIS6_EED1Ev
	leaq	96(%rsp), %rax
	jmp	.L85
.L135:
	movabsq	$288230376151711743, %rdx
	cmpq	%rdx, %rax
	cmova	%rdx, %rax
	salq	$5, %rax
	movq	%rax, 56(%rsp)
	movq	%rax, %rcx
	jmp	.L53
.L132:
	leaq	.LC0(%rip), %rcx
.LEHB10:
	call	_ZSt20__throw_length_errorPKc
.LEHE10:
.L133:
	leaq	.LC1(%rip), %rcx
.LEHB11:
	call	_ZSt20__throw_length_errorPKc
.LEHE11:
.L95:
	movq	%rbx, %rcx
	movq	%rax, %rsi
	call	_ZNSt6vectorIySaIyEED1Ev
.L72:
	leaq	128(%rsp), %rcx
	call	_ZNSt6vectorIN12_GLOBAL__N_16node_tESaIS1_EED1Ev
	movq	%rsi, %rcx
.LEHB12:
	call	_Unwind_Resume
.LEHE12:
.L94:
	movq	48(%rsp), %rcx
	movq	%rax, %rbx
	movq	%rbx, %rsi
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv
	jmp	.L72
.L93:
	movq	%rax, %rsi
	jmp	.L72
.L96:
	movq	%rbx, %rcx
	movq	%rax, %rsi
	call	_ZNSt6vectorIZL16find_clear_cycleRKS_IN12_GLOBAL__N_16node_tESaIS1_EEE10StackFrameSaIS6_EED1Ev
	leaq	160(%rsp), %rcx
	call	_ZNSt6vectorIZL16find_clear_cycleRKS_IN12_GLOBAL__N_16node_tESaIS1_EEE6StatusSaIS6_EED1Ev
	jmp	.L72
	.def	__gxx_personality_seh0;	.scl	2;	.type	32;	.endef
	.seh_handler	__gxx_personality_seh0, @unwind, @except
	.seh_handlerdata
.LLSDA2664:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE2664-.LLSDACSB2664
.LLSDACSB2664:
	.uleb128 .LEHB0-.LFB2664
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB2664
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L93-.LFB2664
	.uleb128 0
	.uleb128 .LEHB2-.LFB2664
	.uleb128 .LEHE2-.LEHB2
	.uleb128 .L94-.LFB2664
	.uleb128 0
	.uleb128 .LEHB3-.LFB2664
	.uleb128 .LEHE3-.LEHB3
	.uleb128 .L93-.LFB2664
	.uleb128 0
	.uleb128 .LEHB4-.LFB2664
	.uleb128 .LEHE4-.LEHB4
	.uleb128 .L95-.LFB2664
	.uleb128 0
	.uleb128 .LEHB5-.LFB2664
	.uleb128 .LEHE5-.LEHB5
	.uleb128 .L93-.LFB2664
	.uleb128 0
	.uleb128 .LEHB6-.LFB2664
	.uleb128 .LEHE6-.LEHB6
	.uleb128 .L96-.LFB2664
	.uleb128 0
	.uleb128 .LEHB7-.LFB2664
	.uleb128 .LEHE7-.LEHB7
	.uleb128 .L94-.LFB2664
	.uleb128 0
	.uleb128 .LEHB8-.LFB2664
	.uleb128 .LEHE8-.LEHB8
	.uleb128 .L93-.LFB2664
	.uleb128 0
	.uleb128 .LEHB9-.LFB2664
	.uleb128 .LEHE9-.LEHB9
	.uleb128 .L96-.LFB2664
	.uleb128 0
	.uleb128 .LEHB10-.LFB2664
	.uleb128 .LEHE10-.LEHB10
	.uleb128 .L93-.LFB2664
	.uleb128 0
	.uleb128 .LEHB11-.LFB2664
	.uleb128 .LEHE11-.LEHB11
	.uleb128 .L94-.LFB2664
	.uleb128 0
	.uleb128 .LEHB12-.LFB2664
	.uleb128 .LEHE12-.LEHB12
	.uleb128 0
	.uleb128 0
.LLSDACSE2664:
	.section	.text.startup,"x"
	.seh_endproc
	.globl	kRootNodeIndex
	.section	.rdata$kRootNodeIndex,"dr"
	.linkonce same_size
	.align 8
kRootNodeIndex:
	.quad	2
	.section .rdata,"dr"
	.align 16
.LC2:
	.quad	2
	.quad	2
	.ident	"GCC: (Rev3, Built by MSYS2 project) 13.2.0"
	.def	_ZdlPvy;	.scl	2;	.type	32;	.endef
	.def	_Znwy;	.scl	2;	.type	32;	.endef
	.def	memset;	.scl	2;	.type	32;	.endef
	.def	_ZSt20__throw_length_errorPKc;	.scl	2;	.type	32;	.endef
	.def	_ZNSt8ios_base15sync_with_stdioEb;	.scl	2;	.type	32;	.endef
	.def	_ZNSi10_M_extractIyEERSiRT_;	.scl	2;	.type	32;	.endef
	.def	_ZStrsIcSt11char_traitsIcESaIcEERSt13basic_istreamIT_T0_ES7_RNSt7__cxx1112basic_stringIS4_S5_T1_EE;	.scl	2;	.type	32;	.endef
	.def	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv;	.scl	2;	.type	32;	.endef
	.def	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_x;	.scl	2;	.type	32;	.endef
	.def	_Unwind_Resume;	.scl	2;	.type	32;	.endef
	.section	.rdata$.refptr._ZSt4cout, "dr"
	.globl	.refptr._ZSt4cout
	.linkonce	discard
.refptr._ZSt4cout:
	.quad	_ZSt4cout
	.section	.rdata$.refptr._ZSt3cin, "dr"
	.globl	.refptr._ZSt3cin
	.linkonce	discard
.refptr._ZSt3cin:
	.quad	_ZSt3cin
