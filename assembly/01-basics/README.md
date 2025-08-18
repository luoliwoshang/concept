# 汇编语言基础概念 (ARM64)

> **重要说明**: 本教程专门针对 **macOS ARM64 架构** (Apple Silicon Mac)  
> ARM64使用独特的汇编语法，与x86架构的Intel/AT&T语法完全不同  
> 本文档所有代码示例都在macOS ARM64环境下测试通过

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

```asm
// ARM64 汇编 (在macOS ARM64上运行)
mov x0, #5      // 将 5 加载到 x0 寄存器
mov x1, #10     // 将 10 加载到 x1 寄存器
add x2, x0, x1  // x2 = x0 + x1 (5 + 10)
```

### 2. ARM64汇编语言的特点

- **独特语法**: ARM64有自己的汇编语法，不同于x86的Intel/AT&T语法
- **直接控制硬件**: 可以直接操作寄存器和内存
- **无抽象**: 没有高级语言的抽象概念，直接面对硬件
- **平台专用**: ARM64指令集和寄存器系统与x86完全不同
- **RISC设计**: 精简指令集，指令格式规整统一
- **立即数前缀**: 使用`#`表示立即数，如`#5`

### 3. 为什么学习汇编语言

1. **深入理解系统**: 理解程序如何在硬件上实际运行
2. **性能优化**: 写出高效的代码，理解编译器优化
3. **系统编程**: 操作系统、驱动程序、嵌入式开发
4. **安全研究**: 逆向工程、漏洞分析
5. **调试技能**: 更好地分析和调试程序问题

## ARM64基本语法结构

**重要**: ARM64汇编语法与x86架构的Intel/AT&T语法完全不同，ARM64有自己独特的语法规则。

### 指令格式
```asm
操作码 目标操作数, 源操作数1, 源操作数2
```

示例：
```asm
mov x0, x1      // 操作码: mov, 将x1的值复制到x0
add x0, x1, #10 // 操作码: add, x0 = x1 + 10(立即数)
```

### 注释
```asm
// 这是单行注释，使用双斜杠开始
mov x0, #5      // 行尾注释
```

### 标签 (Labels)
```asm
_main:              // 标签定义
    mov x0, #5
    ret

loop_start:         // 循环开始标签
    subs x0, x0, #1
    bne loop_start  // 不为零时跳转到标签
```

### 伪指令 (Directives)
```asm
.section __DATA,__data          // 定义数据段
    msg: .asciz "Hello, World!" // 定义字符串

.section __TEXT,__text,regular,pure_instructions
.globl _main                    // 声明全局符号
```

## 第一个ARM64汇编程序

### hello_arm64.s
```asm
.section __TEXT,__text,regular,pure_instructions
.globl _main
.p2align 2

_main:
    // 准备write系统调用的参数
    mov x0, #1                      // 文件描述符: stdout
    adrp x1, msg@PAGE              // 获取字符串地址(高位)
    add x1, x1, msg@PAGEOFF        // 获取字符串地址(低位)
    mov x2, #23                     // 字符串长度
    
    // 调用write系统调用
    mov x16, #4                     // write系统调用号
    svc #0x80                       // 执行系统调用
    
    // 返回0
    mov x0, #0                      // 返回值
    ret                             // 返回

.section __TEXT,__cstring,cstring_literals
msg:
    .asciz "Hello, Assembly World!\n"
```

### 编译和运行
```bash
# 使用 clang 编译
clang -o hello_arm64 hello_arm64.s

# 运行
./hello_arm64
```

## ARM64数据类型和寄存器

### 两个重要概念

#### 1. 寄存器 - CPU内部的存储盒子
ARM64寄存器是CPU芯片内部的高速存储单元，用来临时存放数据：

| 大小 | 寄存器示例 | 说明 |
|------|------------|------|
| 32位 | w0, w1, w2 | 32位通用寄存器 |
| 64位 | x0, x1, x2 | 64位通用寄存器 |

```asm
mov x0, #5      // 将数字5放到x0寄存器中(64位)
mov w0, #10     // 将数字10放到w0寄存器中(32位)
```

**重要**: w0是x0的低32位，修改w0会自动清零x0的高32位！

#### 2. 内存数据定义 - 在内存中划定存储空间
使用伪指令在内存中定义不同大小的数据：

| 大小 | 伪指令 | 示例 |
|------|--------|------|
| 8位  | .byte  | `my_byte: .byte 5` |
| 16位 | .short | `my_short: .short 1234` |
| 32位 | .long  | `my_long: .long 12345678` |
| 64位 | .quad  | `my_quad: .quad 123456789` |

```asm
.section __DATA,__data
my_byte:  .byte 5           // 在内存中定义8位数据
my_short: .short 1234       // 在内存中定义16位数据
my_long:  .long 12345678    // 在内存中定义32位数据
my_quad:  .quad 123456789   // 在内存中定义64位数据

numbers: .byte 1,2,3,4,5    // 字节数组
string:  .asciz "Hello"     // 字符串（自动以0结尾）
```

### 尺寸匹配的重要性

寄存器和数据的尺寸应该合理匹配，才能高效操作：

```asm
.section __DATA,__data
my_byte: .byte 5
my_long: .long 1234

.section __TEXT,__text,regular,pure_instructions
    // ✅ 推荐：32位数据用32位寄存器
    adrp x1, my_long@PAGE
    add x1, x1, my_long@PAGEOFF
    ldr w0, [x1]            // 加载32位数据到w0
    
    // ✅ 推荐：8位数据用专用指令
    adrp x1, my_byte@PAGE
    add x1, x1, my_byte@PAGEOFF
    ldrb w0, [x1]           // 加载8位数据到w0(零扩展)
```

**关键理解：**
- 寄存器在CPU内部，速度最快
- 内存数据在RAM中，需要通过地址访问
- ARM64有专门的加载指令：`ldr`(32/64位), `ldrb`(8位), `ldrh`(16位)

## 立即数、寄存器和内存

### 立即数 (Immediate)
```asm
mov x0, #42         // 将立即数 42 移动到 x0
add x1, x0, #100    // x1 = x0 + 100(立即数)
```

### 寄存器 (Register)
```asm
mov x0, x1          // 将 x1 的值复制到 x0
add x2, x0, x1      // x2 = x0 + x1
```

### 内存 (Memory)
```asm
// 加载内存数据到寄存器
adrp x1, data@PAGE      // 获取data地址的高位
add x1, x1, data@PAGEOFF // 获取完整地址
ldr x0, [x1]            // 从x1指向的内存加载数据到x0

// 存储寄存器数据到内存
str x0, [x1]            // 将x0的值存储到x1指向的内存
ldr x0, [x1, #8]        // 带偏移的内存访问: x1+8
```

## 实践练习

1. **编写第一个ARM64汇编程序**: 修改 hello_arm64.s，输出你的名字
2. **计算练习**: 编写程序计算两个数的和并通过退出码返回
3. **数据操作**: 定义一个数组，读取并处理第一个元素

## 下一步

完成基础概念学习后，继续学习：
- [ARM64寄存器系统](registers.md) - ARM64寄存器详解
- [内存模型](memory.md) - ARM64内存布局和访问方式
- [基本指令](../02-instructions/) - 常用ARM64汇编指令详解

## 架构说明

**ARM64 vs x86架构语法对比**:

| 特性 | x86 (Intel语法) | x86 (AT&T语法) | ARM64 (本教程) |
|------|----------------|----------------|----------------|
| 立即数 | `mov eax, 5` | `movl $5, %eax` | `mov x0, #5` |
| 寄存器 | `eax, ebx` | `%eax, %ebx` | `x0, x1` (无前缀) |
| 注释 | `; comment` | `# comment` | `// comment` |
| 编译器 | NASM, MASM | GAS | clang, GAS |

**重要理解**: ARM64不是x86的"另一种语法"，而是完全不同的架构！

## 参考资源

- **ARM手册**: ARM Architecture Reference Manual ARMv8
- **clang文档**: Clang 汇编器用户手册  
- **系统调用**: macOS System Call Table
- **平台**: 本教程所有示例在Apple Silicon Mac (ARM64)上测试