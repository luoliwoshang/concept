; 简单计算程序 - 演示基本汇编概念
; 计算两个数的和并通过退出码返回结果

section .data
    num1 dd 15          ; 第一个数 (32位)
    num2 dd 27          ; 第二个数 (32位)

section .text
    global _start

_start:
    ; 加载第一个数到寄存器
    mov eax, [num1]     ; eax = num1
    
    ; 加载第二个数并相加
    mov ebx, [num2]     ; ebx = num2
    add eax, ebx        ; eax = eax + ebx
    
    ; 将结果作为退出码返回
    mov rdi, rax        ; 退出码 = 计算结果
    mov rax, 60         ; sys_exit系统调用号
    syscall
    
; 编译运行:
; nasm -f elf64 first_program.asm -o first_program.o
; ld first_program.o -o first_program
; ./first_program
; echo $?  # 查看退出码(应该是42)