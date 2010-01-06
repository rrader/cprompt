  .globl CallCDecl
  .type CallCDecl, @function
   	CallCDecl:
    pushl %ebp
    movl %esp, %ebp

    movl $0, %eax
    movl 16(%ebp), %ebx   # 16(%ebp) - pointer to arguments
    movl 12(%ebp),%ecx   # 12(%ebp) - param count
l1: cmpl %ecx, %eax
    je l2
    pushl (%ebx,%eax,4)
    addl $1,%eax
    jmp l1
l2: call *8(%ebp)

    leave
   	ret
