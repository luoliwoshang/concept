// 基础LLVM内联汇编演示
// 展示如何在C代码中使用内联汇编

#include <stdio.h>

// 1. 最简单的内联汇编 - 无操作数
void basic_nop() {
    printf("执行NOP指令前\n");
    
    // ARM64的NOP指令
    __asm__("nop");
    
    printf("执行NOP指令后\n");
}

// 2. 简单的mov指令 - 使用立即数
int simple_mov() {
    int result;
    
    // 将立即数42移动到寄存器，然后存储到result
    __asm__("mov %0, #42" 
            : "=r"(result)    // 输出：%0 = result，使用通用寄存器
    );
    
    return result;
}

// 3. 带输入和输出的加法
int inline_add(int a, int b) {
    int result;
    
    // ARM64加法指令：add result, a, b
    __asm__("add %0, %1, %2"
            : "=r"(result)    // 输出：%0 = result
            : "r"(a), "r"(b)  // 输入：%1 = a, %2 = b
    );
    
    return result;
}

int main() {
    printf("=== LLVM内联汇编基础演示 ===\n\n");
    
    // 测试1: 基础NOP
    printf("1. 基础NOP指令:\n");
    basic_nop();
    printf("\n");
    
    // 测试2: 简单MOV
    printf("2. MOV立即数指令:\n");
    int mov_result = simple_mov();
    printf("MOV结果: %d\n\n", mov_result);
    
    // 测试3: 加法指令
    printf("3. ADD指令:\n");
    int a = 15, b = 27;
    int add_result = inline_add(a, b);
    printf("ADD %d + %d = %d\n", a, b, add_result);
    
    return 0;
}

/*
编译运行:
clang -o basic basic.c
./basic

生成LLVM IR查看:
clang -S -emit-llvm basic.c -o basic.ll
*/