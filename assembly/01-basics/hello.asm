; 第一个汇编程序 - Hello World
; 这个程序演示了汇编语言的基本结构和系统调用

section .data
    ; 定义字符串常量
    msg db 'Hello, Assembly World!', 0xA, 0    ; 消息 + 换行符 + 空字符
    msg_len equ $ - msg - 1                    ; 计算字符串长度（不包括空字符）

section .text
    global _start       ; 声明程序入口点

_start:
    ; write 系统调用 - 输出字符串
    mov rax, 1          ; 系统调用号: sys_write
    mov rdi, 1          ; 文件描述符: stdout
    mov rsi, msg        ; 要输出的字符串地址
    mov rdx, msg_len    ; 字符串长度
    syscall             ; 执行系统调用

    ; exit 系统调用 - 退出程序
    mov rax, 60         ; 系统调用号: sys_exit
    mov rdi, 0          ; 退出状态码: 0 (成功)
    syscall             ; 执行系统调用

; 编译和运行说明:
; 1. 汇编: nasm -f elf64 hello.asm -o hello.o
; 2. 链接: ld hello.o -o hello
; 3. 运行: ./hello