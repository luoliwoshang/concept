# 汇编语言基础概念

汇编语言是最接近机器语言的编程语言，每条汇编指令几乎直接对应一条机器指令。理解汇编语言是深入理解计算机系统运行原理的关键。

## 核心概念

### 1. 什么是汇编语言

汇编语言是一种低级编程语言，它使用助记符（mnemonics）来表示机器指令。

```
高级语言 → 编译器 → 汇编语言 → 汇编器 → 机器语言
```

#### 示例对比
```c
// C 语言
int a = 5;
int b = 10;
int c = a + b;
```

```nasm
; x86-64 汇编
mov rax, 5      ; 将 5 加载到 rax 寄存器
mov rbx, 10     ; 将 10 加载到 rbx 寄存器
add rax, rbx    ; rax = rax + rbx (5 + 10)
```

### 2. 汇编语言的特点

- **直接控制硬件**: 可以直接操作寄存器和内存
- **无抽象**: 没有高级语言的抽象概念，直接面对硬件
- **平台相关**: 不同的CPU架构有不同的汇编语言
- **精确控制**: 可以精确控制程序的每一步执行

### 3. 为什么学习汇编语言

1. **深入理解系统**: 理解程序如何在硬件上实际运行
2. **性能优化**: 写出高效的代码，理解编译器优化
3. **系统编程**: 操作系统、驱动程序、嵌入式开发
4. **安全研究**: 逆向工程、漏洞分析
5. **调试技能**: 更好地分析和调试程序问题

## 基本语法结构

### 指令格式
```nasm
操作码 操作数1, 操作数2
```

示例：
```nasm
mov rax, rbx    ; 操作码: mov, 操作数: rax(目标), rbx(源)
add rax, 10     ; 操作码: add, 操作数: rax, 10(立即数)
```

### 注释
```nasm
; 这是单行注释，使用分号开始
mov rax, 5      ; 行尾注释
```

### 标签 (Labels)
```nasm
main:           ; 标签定义
    mov rax, 5
    ret

loop_start:     ; 循环开始标签
    dec rax
    jnz loop_start  ; 跳转到标签
```

### 伪指令 (Directives)
```nasm
section .data   ; 定义数据段
    msg db 'Hello, World!', 0  ; 定义字符串

section .text   ; 定义代码段
    global _start   ; 声明全局符号
```

## 第一个汇编程序

### hello.asm
```nasm
section .data
    msg db 'Hello, World!', 0xA, 0    ; 字符串 + 换行符 + 空字符
    msg_len equ $ - msg - 1           ; 计算字符串长度

section .text
    global _start

_start:
    ; write系统调用
    mov rax, 1          ; sys_write
    mov rdi, 1          ; stdout
    mov rsi, msg        ; 消息地址
    mov rdx, msg_len    ; 消息长度
    syscall             ; 调用系统调用

    ; exit系统调用
    mov rax, 60         ; sys_exit
    mov rdi, 0          ; 退出状态
    syscall             ; 调用系统调用
```

### 编译和运行
```bash
# 使用 NASM 汇编
nasm -f elf64 hello.asm -o hello.o

# 使用 ld 链接
ld hello.o -o hello

# 运行
./hello
```

## 数据类型和大小

### 两个重要概念

#### 1. 寄存器 - CPU内部的存储盒子
寄存器是CPU芯片内部的高速存储单元，用来临时存放数据：

| 大小 | 寄存器示例 | 说明 |
|------|------------|------|
| 8位  | al, bl, cl | 最小的寄存器 |
| 16位 | ax, bx, cx | 16位寄存器 |
| 32位 | eax, ebx, ecx | 32位寄存器 |
| 64位 | rax, rbx, rcx | 64位寄存器 |

```nasm
mov rax, 5      ; 将数字5放到rax寄存器中
mov al, 10      ; 将数字10放到al寄存器中
```

#### 2. 内存数据定义 - 在内存中划定存储空间
使用伪指令在内存中定义不同大小的数据：

| 大小 | 伪指令 | 示例 |
|------|--------|------|
| 8位  | db     | `my_byte db 5` |
| 16位 | dw     | `my_word dw 1234` |
| 32位 | dd     | `my_dword dd 12345678` |
| 64位 | dq     | `my_qword dq 123456789` |

```nasm
section .data
    my_byte  db 5           ; 在内存中定义8位数据
    my_word  dw 1234        ; 在内存中定义16位数据
    my_dword dd 12345678    ; 在内存中定义32位数据
    my_qword dq 123456789   ; 在内存中定义64位数据
    
    numbers db 1,2,3,4,5    ; 字节数组
    string db "Hello",0     ; 字符串（以0结尾）
    buffer resb 64          ; 保留64字节的未初始化空间
```

### 尺寸匹配的重要性

寄存器和数据的尺寸必须匹配，才能正确操作：

```nasm
section .data
    my_byte db 5
    my_word dw 1234

section .text
    ; ✅ 正确：8位数据用8位寄存器
    mov al, [my_byte]
    
    ; ✅ 正确：16位数据用16位寄存器  
    mov ax, [my_word]
    
    ; ❌ 错误：尺寸不匹配
    mov rax, [my_byte]      ; 64位寄存器装8位数据
    mov al, [my_word]       ; 8位寄存器装16位数据
```

**关键理解：**
- 寄存器在CPU内部，速度最快
- 内存数据在RAM中，需要通过地址访问
- 操作时必须尺寸匹配，这样CPU效率最高

## 立即数、寄存器和内存

### 立即数 (Immediate)
```nasm
mov rax, 42         ; 将立即数 42 移动到 rax
add rbx, 100        ; 将立即数 100 加到 rbx
```

### 寄存器 (Register)
```nasm
mov rax, rbx        ; 将 rbx 的值复制到 rax
add rcx, rdx        ; rcx = rcx + rdx
```

### 内存 (Memory)
```nasm
mov rax, [rbx]      ; 将 rbx 指向的内存内容加载到 rax
mov [rax], rbx      ; 将 rbx 的值存储到 rax 指向的内存
mov rax, [rbx+8]    ; 带偏移的内存访问
```

## 实践练习

1. **编写第一个汇编程序**: 修改 hello.asm，输出你的名字
2. **计算练习**: 编写程序计算两个数的和并输出结果
3. **数据操作**: 定义一个数组，读取并输出第一个元素

## 下一步

完成基础概念学习后，继续学习：
- [寄存器系统](registers.md) - CPU寄存器详解
- [内存模型](memory.md) - 内存布局和访问方式
- [基本指令](../02-instructions/) - 常用汇编指令详解

## 参考资源

- **Intel手册**: Intel 64 and IA-32 Architectures Software Developer's Manual
- **NASM文档**: NASM 汇编器用户手册
- **系统调用**: Linux System Call Table