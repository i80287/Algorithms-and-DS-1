	.file	"q.cpp"
	.text
#APP
	.globl _ZSt21ios_base_library_initv
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC0:
	.string	"void readmrow(std::bitset<4096>&, std::string&)"
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC1:
	.string	"q.cpp"
.LC2:
	.string	"s.capacity() >= s.size() + 16"
#NO_APP
	.text
	.p2align 4
	.type	_ZL8readmrowRSt6bitsetILm4096EERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE, @function
_ZL8readmrowRSt6bitsetILm4096EERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE:
.LFB4660:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	leaq	16(%rsi), %rax
	movq	%rsi, %rbp
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movq	(%rsi), %rdi
	cmpq	%rax, %rdi
	je	.L6
	movq	16(%rsi), %rax
.L2:
	movq	8(%rbp), %rcx
	leaq	16(%rcx), %rdx
	cmpq	%rdx, %rax
	jb	.L14
	xorl	%esi, %esi
	call	memset@PLT
	movq	%rbp, %rsi
	leaq	_ZSt3cin(%rip), %rdi
	call	_ZStrsIcSt11char_traitsIcESaIcEERSt13basic_istreamIT_T0_ES7_RNSt7__cxx1112basic_stringIS4_S5_T1_EE@PLT
	movq	8(%rbp), %r8
	movq	0(%rbp), %rcx
	addq	$3, %r8
	shrq	$2, %r8
	je	.L11
	xorl	%edx, %edx
	leaq	kTable(%rip), %rsi
	.p2align 4,,10
	.p2align 3
.L5:
	movzbl	1(%rcx,%rdx), %eax
	movzbl	2(%rcx,%rdx), %edi
	addq	$2, %rbx
	movq	(%rsi,%rax,8), %rax
	movq	(%rsi,%rdi,8), %rdi
	sall	$4, %eax
	sall	$8, %edi
	orl	%edi, %eax
	movzbl	(%rcx,%rdx), %edi
	orw	(%rsi,%rdi,8), %ax
	movzbl	3(%rcx,%rdx), %edi
	addq	$4, %rdx
	movq	(%rsi,%rdi,8), %rdi
	sall	$12, %edi
	orl	%edi, %eax
	movw	%ax, -2(%rbx)
	cmpq	%r8, %rdx
	jb	.L5
.L11:
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,10
	.p2align 3
.L6:
	.cfi_restore_state
	movl	$15, %eax
	jmp	.L2
.L14:
	leaq	.LC0(%rip), %rcx
	movl	$56, %edx
	leaq	.LC1(%rip), %rsi
	leaq	.LC2(%rip), %rdi
	call	__assert_fail@PLT
	.cfi_endproc
.LFE4660:
	.size	_ZL8readmrowRSt6bitsetILm4096EERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE, .-_ZL8readmrowRSt6bitsetILm4096EERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
	.section	.text._ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv,"axG",@progbits,_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv,comdat
	.align 2
	.p2align 4
	.weak	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv
	.type	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv, @function
_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv:
.LFB5102:
	.cfi_startproc
	endbr64
	movq	(%rdi), %rax
	leaq	16(%rdi), %rdx
	cmpq	%rdx, %rax
	je	.L15
	movq	16(%rdi), %rsi
	movq	%rax, %rdi
	addq	$1, %rsi
	jmp	_ZdlPvm@PLT
	.p2align 4,,10
	.p2align 3
.L15:
	ret
	.cfi_endproc
.LFE5102:
	.size	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv, .-_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv
	.section	.text._ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EE11_M_gen_randEv,"axG",@progbits,_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EE11_M_gen_randEv,comdat
	.align 2
	.p2align 4
	.weak	_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EE11_M_gen_randEv
	.type	_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EE11_M_gen_randEv, @function
_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EE11_M_gen_randEv:
.LFB5250:
	.cfi_startproc
	endbr64
	movq	%rdi, %r8
	movq	(%rdi), %rsi
	leaq	1816(%rdi), %rdi
	movq	%r8, %rdx
	.p2align 4,,10
	.p2align 3
.L19:
	andq	$-2147483648, %rsi
	addq	$8, %rdx
	movq	%rsi, %rcx
	movq	(%rdx), %rsi
	movq	%rsi, %rax
	andl	$2147483647, %eax
	orq	%rax, %rcx
	andl	$1, %eax
	negq	%rax
	shrq	%rcx
	xorq	3168(%rdx), %rcx
	andl	$2567483615, %eax
	xorq	%rcx, %rax
	movq	%rax, -8(%rdx)
	cmpq	%rdi, %rdx
	jne	.L19
	movdqa	.LC3(%rip), %xmm7
	movdqa	.LC4(%rip), %xmm6
	leaq	4984(%r8), %rax
	pxor	%xmm3, %xmm3
	movdqa	.LC5(%rip), %xmm5
	movdqa	.LC6(%rip), %xmm4
	.p2align 4,,10
	.p2align 3
.L20:
	movdqu	(%rdx), %xmm0
	movdqu	8(%rdx), %xmm1
	addq	$16, %rdx
	movdqu	-1832(%rdx), %xmm2
	pand	%xmm6, %xmm1
	pand	%xmm7, %xmm0
	por	%xmm1, %xmm0
	movdqa	%xmm0, %xmm1
	pand	%xmm5, %xmm0
	psrlq	$1, %xmm1
	pxor	%xmm2, %xmm1
	movdqa	%xmm3, %xmm2
	psubq	%xmm0, %xmm2
	pand	%xmm4, %xmm2
	movdqa	%xmm2, %xmm0
	pxor	%xmm1, %xmm0
	movups	%xmm0, -16(%rdx)
	cmpq	%rax, %rdx
	jne	.L20
	movq	4984(%r8), %rax
	movq	(%r8), %rdx
	movq	$0, 4992(%r8)
	andl	$2147483647, %edx
	andq	$-2147483648, %rax
	orq	%rdx, %rax
	movq	%rax, %rdx
	andl	$1, %eax
	negq	%rax
	shrq	%rdx
	xorq	3168(%r8), %rdx
	andl	$2567483615, %eax
	xorq	%rdx, %rax
	movq	%rax, 4984(%r8)
	ret
	.cfi_endproc
.LFE5250:
	.size	_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EE11_M_gen_randEv, .-_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EE11_M_gen_randEv
	.section	.text._ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EEclEv,"axG",@progbits,_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EEclEv,comdat
	.align 2
	.p2align 4
	.weak	_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EEclEv
	.type	_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EEclEv, @function
_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EEclEv:
.LFB5079:
	.cfi_startproc
	endbr64
	movq	4992(%rdi), %rax
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	cmpq	$623, %rax
	ja	.L26
.L24:
	leaq	1(%rax), %rdx
	movq	(%rbx,%rax,8), %rax
	movq	%rdx, 4992(%rbx)
	popq	%rbx
	.cfi_remember_state
	.cfi_def_cfa_offset 8
	movq	%rax, %rdx
	shrq	$11, %rdx
	movl	%edx, %edx
	xorq	%rax, %rdx
	movq	%rdx, %rax
	salq	$7, %rax
	andl	$2636928640, %eax
	xorq	%rdx, %rax
	movq	%rax, %rdx
	salq	$15, %rdx
	andl	$4022730752, %edx
	xorq	%rax, %rdx
	movq	%rdx, %rax
	shrq	$18, %rax
	xorq	%rdx, %rax
	ret
	.p2align 4,,10
	.p2align 3
.L26:
	.cfi_restore_state
	call	_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EE11_M_gen_randEv
	movq	4992(%rbx), %rax
	jmp	.L24
	.cfi_endproc
.LFE5079:
	.size	_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EEclEv, .-_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EEclEv
	.section	.text.unlikely,"ax",@progbits
.LCOLDB7:
	.text
.LHOTB7:
	.p2align 4
	.type	_ZL5solvev, @function
_ZL5solvev:
.LFB4701:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA4701
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r15
	pushq	%r14
	pushq	%r13
	pushq	%r12
	pushq	%rbx
	andq	$-32, %rsp
	subq	$4096, %rsp
	orq	$0, (%rsp)
	subq	$1504, %rsp
	.cfi_offset 15, -24
	.cfi_offset 14, -32
	.cfi_offset 13, -40
	.cfi_offset 12, -48
	.cfi_offset 3, -56
	leaq	_ZSt3cin(%rip), %rdi
	movq	%fs:40, %rax
	movq	%rax, 5592(%rsp)
	xorl	%eax, %eax
	leaq	56(%rsp), %rsi
	leaq	608(%rsp), %r14
.LEHB0:
	call	_ZNSi10_M_extractImEERSiRT_@PLT
.LEHE0:
	movq	56(%rsp), %r12
	movl	$30, %eax
	movq	%r14, 592(%rsp)
	movb	$0, 608(%rsp)
	leaq	592(%rsp), %rbx
	leaq	3(%r12), %r13
	movq	$0, 600(%rsp)
	shrq	$2, %r13
	addq	$16, %r13
	cmpq	%rax, %r13
	cmovb	%rax, %r13
	leaq	1(%r13), %rdi
.LEHB1:
	call	_Znwm@PLT
	movq	%rax, %rbx
	movq	600(%rsp), %rax
	movq	592(%rsp), %r15
	leaq	1(%rax), %rdx
	testq	%rax, %rax
	je	.L74
	testq	%rdx, %rdx
	jne	.L75
.L29:
	cmpq	%r14, %r15
	je	.L30
	movq	608(%rsp), %rax
	movq	%r15, %rdi
	leaq	1(%rax), %rsi
	call	_ZdlPvm@PLT
.L30:
	movq	%rbx, 592(%rsp)
	leaq	592(%rsp), %rbx
	movq	%r13, 608(%rsp)
	xorl	%r13d, %r13d
	testq	%r12, %r12
	je	.L76
.L31:
	movq	%r13, %rdi
	leaq	_ZL3m_a(%rip), %rax
	movq	%rbx, %rsi
	salq	$9, %rdi
	addq	%rax, %rdi
	call	_ZL8readmrowRSt6bitsetILm4096EERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
	addq	$1, %r13
	cmpq	%r13, %r12
	jne	.L31
	xorl	%r13d, %r13d
	leaq	_ZL3m_b(%rip), %r14
.L33:
	movq	%r13, %rdi
	movq	%rbx, %rsi
	salq	$9, %rdi
	addq	%r14, %rdi
	call	_ZL8readmrowRSt6bitsetILm4096EERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
	addq	$1, %r13
	cmpq	%r13, %r12
	jne	.L33
	xorl	%r15d, %r15d
	leaq	_ZL3m_c(%rip), %r13
.L34:
	movq	%r15, %rdi
	movq	%rbx, %rsi
	salq	$9, %rdi
	addq	%r13, %rdi
	call	_ZL8readmrowRSt6bitsetILm4096EERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
.LEHE1:
	addq	$1, %r15
	cmpq	%r15, %r12
	jne	.L34
.L35:
	movq	%rbx, %rdi
	leaq	64(%rsp), %r12
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv
	xorl	%eax, %eax
	movl	$64, %ecx
	movq	%r12, %rdi
	rep stosq
	movl	$64, %ecx
	movq	%rbx, %rdi
	xorl	%edx, %edx
	rep stosq
	leaq	_ZL3m_b(%rip), %rax
	xorl	%edi, %edi
	xorl	%r11d, %r11d
	movq	%rax, %rsi
.L32:
	movq	%r11, (%rsp)
	movq	%rdi, %r9
	movq	%rdx, %r8
	movq	%rsi, %r10
	movq	%rdx, 8(%rsp)
	negq	%r9
	movq	%rsi, %rdx
	movq	%r14, %rsi
	.p2align 4,,10
	.p2align 3
.L46:
	movq	%r10, %rcx
	xorl	%eax, %eax
	.p2align 4,,10
	.p2align 3
.L38:
	movq	(%rcx), %r11
	movq	%r11, (%r12,%rax,8)
	leaq	(%r9,%rcx), %r11
	addq	$512, %rcx
	movq	(%r11,%rdi), %r11
	movq	%r11, (%rbx,%rax,8)
	addq	$1, %rax
	cmpq	$64, %rax
	jne	.L38
	movq	%rdi, 40(%rsp)
	movl	$6, %r14d
	movl	$4294967295, %r11d
	movl	$32, %r15d
	movq	%r9, 32(%rsp)
	movq	%rdx, 24(%rsp)
	movq	%r10, 16(%rsp)
	.p2align 4,,10
	.p2align 3
.L39:
	movq	%r15, %r10
	movl	%r15d, %ecx
	xorl	%eax, %eax
	notq	%r10
	.p2align 4,,10
	.p2align 3
.L40:
	movq	64(%rsp,%rax,8), %rdx
	leaq	(%r15,%rax), %rdi
	movq	64(%rsp,%rdi,8), %r9
	shrq	%cl, %rdx
	xorq	%r9, %rdx
	andq	%r11, %rdx
	xorq	%rdx, %r9
	salq	%cl, %rdx
	movq	%r9, 64(%rsp,%rdi,8)
	addq	$1, %rdi
	xorq	%rdx, 64(%rsp,%rax,8)
	movq	%rdi, %rax
	andq	%r10, %rax
	cmpq	$63, %rax
	jbe	.L40
	shrq	%r15
	movq	%r11, %rax
	movl	%r15d, %ecx
	salq	%cl, %rax
	xorq	%rax, %r11
	subl	$1, %r14d
	jne	.L39
	movl	$6, %r14d
	movl	$4294967295, %r11d
	movl	$32, %r15d
	.p2align 4,,10
	.p2align 3
.L41:
	movq	%r15, %r10
	movl	%r15d, %ecx
	xorl	%eax, %eax
	notq	%r10
	.p2align 4,,10
	.p2align 3
.L42:
	movq	592(%rsp,%rax,8), %rdx
	leaq	(%r15,%rax), %rdi
	movq	592(%rsp,%rdi,8), %r9
	shrq	%cl, %rdx
	xorq	%r9, %rdx
	andq	%r11, %rdx
	xorq	%rdx, %r9
	salq	%cl, %rdx
	movq	%r9, 592(%rsp,%rdi,8)
	addq	$1, %rdi
	xorq	%rdx, 592(%rsp,%rax,8)
	movq	%rdi, %rax
	andq	%r10, %rax
	cmpq	$63, %rax
	jbe	.L42
	shrq	%r15
	movq	%r11, %rax
	movl	%r15d, %ecx
	salq	%cl, %rax
	xorq	%rax, %r11
	subl	$1, %r14d
	jne	.L41
	movq	16(%rsp), %r10
	movq	40(%rsp), %rdi
	xorl	%eax, %eax
	movq	32(%rsp), %r9
	movq	24(%rsp), %rdx
	movq	%r10, %rcx
	.p2align 4,,10
	.p2align 3
.L43:
	movq	(%rbx,%rax,8), %r11
	movq	(%r12,%rax,8), %r14
	addq	$1, %rax
	movq	%r11, (%rcx)
	leaq	(%r9,%rcx), %r11
	addq	$512, %rcx
	movq	%r14, (%r11,%rdi)
	cmpq	$64, %rax
	jne	.L43
	addq	$8, %r8
	addq	$8, %r10
	addq	$32760, %r9
	cmpq	$512, %r8
	jne	.L46
	movq	(%rsp), %r11
	movq	%rsi, %r14
	movq	%rdx, %rsi
	movq	8(%rsp), %rdx
	subq	$32760, %rdi
	addq	$32776, %rsi
	addq	$1, %r11
	addq	$8, %rdx
	cmpq	$64, %r11
	jne	.L32
	movl	$5489, %eax
	movl	$1, %ecx
	movq	$5489, 592(%rsp)
	movq	%rax, %rdx
.L47:
	movq	%rdx, %rax
	shrq	$30, %rax
	xorq	%rdx, %rax
	imulq	$1812433253, %rax, %rax
	leal	(%rax,%rcx), %edx
	movq	%rdx, (%rbx,%rcx,8)
	addq	$1, %rcx
	cmpq	$624, %rcx
	jne	.L47
	movl	$25000, %r10d
	movq	%r14, 16(%rsp)
	movq	%rbx, %r14
	movl	$1, %r11d
	movq	%r13, 8(%rsp)
	leaq	576(%rsp), %r15
	movq	%r12, %r13
	leaq	64(%rsp), %r9
	movq	$624, 5584(%rsp)
	movq	%r10, %r12
.L50:
	movq	%r14, %rdi
	movq	%r9, 24(%rsp)
	movb	%r11b, 32(%rsp)
	call	_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EEclEv
	movq	56(%rsp), %rcx
	xorl	%edx, %edx
	movq	%r14, %rdi
	divq	%rcx
	movq	%rcx, 40(%rsp)
	movq	%rdx, %rbx
	call	_ZNSt23mersenne_twister_engineImLm32ELm624ELm397ELm31ELm2567483615ELm11ELm4294967295ELm7ELm2636928640ELm15ELm4022730752ELm18ELm1812433253EEclEv
	movq	40(%rsp), %rcx
	xorl	%edx, %edx
	movq	%rbx, %rsi
	salq	$9, %rsi
	movq	24(%rsp), %r9
	movq	%r13, %rdi
	movzbl	32(%rsp), %r11d
	divq	%rcx
	leaq	_ZL3m_a(%rip), %rax
	movl	$64, %ecx
	movq	%r9, %r13
	addq	%rax, %rsi
	movq	%r9, %rax
	rep movsq
	movq	16(%rsp), %rcx
	movq	%rdx, %rsi
	salq	$9, %rsi
	addq	%rcx, %rsi
	movq	%r9, %rcx
	.p2align 4,,10
	.p2align 3
.L48:
	vmovdqa	(%rcx), %ymm1
	vandps	(%rsi), %ymm1, %ymm0
	addq	$32, %rcx
	addq	$32, %rsi
	vmovdqa	%ymm0, -32(%rcx)
	cmpq	%r15, %rcx
	jne	.L48
	xorl	%esi, %esi
	.p2align 4,,10
	.p2align 3
.L49:
	xorl	%ecx, %ecx
	addq	$8, %rax
	popcntq	-8(%rax), %rcx
	addq	%rcx, %rsi
	cmpq	%r15, %rax
	jne	.L49
	movq	8(%rsp), %rax
	salq	$9, %rbx
	movl	$1, %edi
	movl	%edx, %ecx
	salq	%cl, %rdi
	addq	%rbx, %rax
	movq	%rdx, %rbx
	shrq	$6, %rbx
	andq	(%rax,%rbx,8), %rdi
	setne	%al
	andl	$1, %esi
	movzbl	%al, %eax
	cmpq	%rsi, %rax
	sete	%al
	andl	%eax, %r11d
	subq	$1, %r12
	je	.L77
	vzeroupper
	jmp	.L50
.L77:
	movq	5592(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L78
	vzeroupper
	leaq	-40(%rbp), %rsp
	movl	%r11d, %eax
	popq	%rbx
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	popq	%rbp
	.cfi_remember_state
	.cfi_def_cfa 7, 8
	ret
.L74:
	.cfi_restore_state
	movzbl	(%r15), %eax
	movb	%al, (%rbx)
	jmp	.L29
.L76:
	leaq	_ZL3m_b(%rip), %r14
	leaq	_ZL3m_c(%rip), %r13
	jmp	.L35
.L75:
	movq	%r15, %rsi
	movq	%rbx, %rdi
	call	memcpy@PLT
	jmp	.L29
.L78:
	vzeroupper
	call	__stack_chk_fail@PLT
.L55:
	endbr64
	movq	%rax, %r12
	jmp	.L36
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA4701:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE4701-.LLSDACSB4701
.LLSDACSB4701:
	.uleb128 .LEHB0-.LFB4701
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB4701
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L55-.LFB4701
	.uleb128 0
.LLSDACSE4701:
	.text
	.cfi_endproc
	.section	.text.unlikely
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDAC4701
	.type	_ZL5solvev.cold, @function
_ZL5solvev.cold:
.LFSB4701:
.L36:
	.cfi_def_cfa 6, 16
	.cfi_offset 3, -56
	.cfi_offset 6, -16
	.cfi_offset 12, -48
	.cfi_offset 13, -40
	.cfi_offset 14, -32
	.cfi_offset 15, -24
	movq	%rbx, %rdi
	vzeroupper
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE10_M_disposeEv
	movq	5592(%rsp), %rax
	subq	%fs:40, %rax
	jne	.L79
	movq	%r12, %rdi
.LEHB2:
	call	_Unwind_Resume@PLT
.LEHE2:
.L79:
	call	__stack_chk_fail@PLT
	.cfi_endproc
.LFE4701:
	.section	.gcc_except_table
.LLSDAC4701:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSEC4701-.LLSDACSBC4701
.LLSDACSBC4701:
	.uleb128 .LEHB2-.LCOLDB7
	.uleb128 .LEHE2-.LEHB2
	.uleb128 0
	.uleb128 0
.LLSDACSEC4701:
	.section	.text.unlikely
	.text
	.size	_ZL5solvev, .-_ZL5solvev
	.section	.text.unlikely
	.size	_ZL5solvev.cold, .-_ZL5solvev.cold
.LCOLDE7:
	.text
.LHOTE7:
	.section	.rodata.str1.1
.LC8:
	.string	"YES"
.LC9:
	.string	"NO"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB4702:
	.cfi_startproc
	endbr64
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	leaq	.LC9(%rip), %rbx
	call	_ZL5solvev
	testb	%al, %al
	leaq	.LC8(%rip), %rax
	cmovne	%rax, %rbx
	movq	%rbx, %rdi
	call	strlen@PLT
	movq	%rbx, %rsi
	leaq	_ZSt4cout(%rip), %rdi
	movq	%rax, %rdx
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l@PLT
	xorl	%eax, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE4702:
	.size	main, .-main
	.weak	kTable
	.section	.rodata.kTable,"aG",@progbits,kTable,comdat
	.align 32
	.type	kTable, @gnu_unique_object
	.size	kTable, 2048
kTable:
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	8
	.quad	4
	.quad	12
	.quad	2
	.quad	10
	.quad	6
	.quad	14
	.quad	1
	.quad	9
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	0
	.quad	5
	.quad	13
	.quad	3
	.quad	11
	.quad	7
	.quad	15
	.zero	1480
	.local	_ZL3m_c
	.comm	_ZL3m_c,2097152,64
	.local	_ZL3m_b
	.comm	_ZL3m_b,2097152,64
	.local	_ZL3m_a
	.comm	_ZL3m_a,2097152,64
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC3:
	.quad	-2147483648
	.quad	-2147483648
	.align 16
.LC4:
	.quad	2147483647
	.quad	2147483647
	.align 16
.LC5:
	.quad	1
	.quad	1
	.align 16
.LC6:
	.quad	2567483615
	.quad	2567483615
	.hidden	DW.ref.__gxx_personality_v0
	.weak	DW.ref.__gxx_personality_v0
	.section	.data.rel.local.DW.ref.__gxx_personality_v0,"awG",@progbits,DW.ref.__gxx_personality_v0,comdat
	.align 8
	.type	DW.ref.__gxx_personality_v0, @object
	.size	DW.ref.__gxx_personality_v0, 8
DW.ref.__gxx_personality_v0:
	.quad	__gxx_personality_v0
	.ident	"GCC: (Ubuntu 13.2.0-23ubuntu4) 13.2.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
