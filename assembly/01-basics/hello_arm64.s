.section __TEXT,__text,regular,pure_instructions
.globl _main
.p2align 2

_main:
    // 准备write系统调用的参数
    mov x0, #1                      // 文件描述符 stdout
    adrp x1, msg@PAGE              // 获取字符串地址
    add x1, x1, msg@PAGEOFF
    mov x2, #23                     // 字符串长度
    
    // 调用write系统调用
    mov x16, #4                     // write系统调用号
    svc #0x80                       // 执行系统调用
    
    // 返回0
    mov x0, #0
    ret

.section __TEXT,__cstring,cstring_literals
msg:
    .asciz "Hello, Assembly World!\n"