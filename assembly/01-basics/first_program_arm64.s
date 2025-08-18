// 简单计算程序 - 演示ARM64汇编基本概念
// 计算两个数的和并通过退出码返回结果

.section __DATA,__data
num1: .long 15          // 第一个数 (32位)
num2: .long 27          // 第二个数 (32位)

.section __TEXT,__text,regular,pure_instructions
.globl _main
.p2align 2

_main:
    // 加载第一个数到寄存器
    adrp x1, num1@PAGE      // 获取num1地址
    add x1, x1, num1@PAGEOFF
    ldr w0, [x1]            // w0 = num1
    
    // 加载第二个数并相加
    adrp x1, num2@PAGE      // 获取num2地址
    add x1, x1, num2@PAGEOFF  
    ldr w1, [x1]            // w1 = num2
    add w0, w0, w1          // w0 = w0 + w1
    
    // 将结果作为退出码返回
    // ARM64中，返回值通过x0传递
    // w0自动扩展到x0
    ret
    
// 编译运行:
// clang -o first_program_arm64 first_program_arm64.s
// ./first_program_arm64
// echo $?  # 查看退出码(应该是42)