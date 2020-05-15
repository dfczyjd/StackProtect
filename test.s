	.file	"test.c"
	.text
	.section .rdata,"dr"
	.text
save_ret:
	pushl	%eax
	movl	0x4(%esp), %eax
	addl	$0x5, %eax
	movl	%eax, (%edi, %esi, 4)
	popl	%eax
	ret
LC0:
	.ascii "Copied string is %s\12\0"
	.text
	.globl	_foo
	.def	_foo;	.scl	2;	.type	32;	.endef
_foo:
LFB14:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$56, %esp
	movl	8(%ebp), %eax
	movl	%eax, 4(%esp)
	leal	-28(%ebp), %eax
	movl	%eax, (%esp)
	call	_strcpy
	leal	-28(%ebp), %eax
	movl	%eax, 4(%esp)
	movl	$LC0, (%esp)
	call	_printf
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	movl	(%edi), %esi
	decl	%esi
	movl	%esi, (%edi)
	incl	%esi
	addl	$0x4, %esp
	jmp		*(%edi,%esi,4)
	.cfi_endproc
LFE14:
	.def	___main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
LC1:
	.ascii "Missing argument\0"
	.text
	.globl	_main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
LFB15:
	pushl	$0x10000
	call	_malloc
	addl	$4, %esp
	movl	%eax, %edi
	movl	$0, (%eax)
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	subl	$16, %esp
	call	___main
	cmpl	$1, 8(%ebp)
	jg	L3
	movl	$LC1, (%esp)
	call	_puts
	movl	$-1, %eax
	jmp	L4
L3:
	movl	12(%ebp), %eax
	addl	$4, %eax
	movl	(%eax), %eax
	movl	%eax, (%esp)
	movl	(%edi), %esi
	incl	%esi
	movl	%esi, (%edi)
	pushl	$0
	call	save_ret
	jmp		_foo
	movl	$0, %eax
L4:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE15:
	.ident	"GCC: (i686-posix-dwarf-rev0, Built by MinGW-W64 project) 8.1.0"
	.def	_strcpy;	.scl	2;	.type	32;	.endef
	.def	_printf;	.scl	2;	.type	32;	.endef
	.def	_puts;	.scl	2;	.type	32;	.endef
