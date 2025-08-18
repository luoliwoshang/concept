// LLVM内联汇编操作数占位符详解
// 演示$0, $1, $2...占位符的使用

#include <stdio.h>

// 1. 单个输出操作数
int demo_single_output() {
    int result;
    
    printf("演示: 单个输出操作数 ($0)\n");
    __asm__("mov %0, #100"     // %0 对应第一个操作数
            : "=r"(result)     // %0 = result (输出)
    );
    
    return result;
}

// 2. 单个输入操作数  
void demo_single_input(int value) {
    printf("演示: 单个输入操作数 ($0)\n");
    printf("输入值: %d\n", value);
    
    // 使用输入值进行一些操作（这里只是演示占位符）
    __asm__("// 使用输入值: %0"
            :                  // 无输出
            : "r"(value)       // %0 = value (输入)
    );
}

// 3. 多个操作数演示
int demo_multiple_operands(int a, int b, int c) {
    int result1, result2;
    
    printf("演示: 多个操作数 ($0, $1, $2, $3, $4)\n");
    
    __asm__(
        "add %0, %2, %3\n\t"   // %0 = %2 + %3 (result1 = a + b)
        "sub %1, %2, %4"       // %1 = %2 - %4 (result2 = a - c)
        : "=r"(result1),       // %0 = result1 (输出)
          "=r"(result2)        // %1 = result2 (输出)  
        : "r"(a),              // %2 = a (输入)
          "r"(b),              // %3 = b (输入)
          "r"(c)               // %4 = c (输入)
    );
    
    printf("计算结果: %d + %d = %d\n", a, b, result1);
    printf("计算结果: %d - %d = %d\n", a, c, result2);
    
    return result1;
}

// 4. 输入输出混合
int demo_inout_operands(int input) {
    int value = input;
    
    printf("演示: 输入输出混合操作数\n");
    printf("原始值: %d\n", value);
    
    // 将value自增1，同时作为输入和输出
    __asm__("add %0, %0, #1"   // %0 = %0 + 1
            : "+r"(value)      // %0 = value (输入输出，+表示既读又写)
    );
    
    printf("自增后: %d\n", value);
    return value;
}

// 5. 复杂操作数映射演示
void demo_complex_mapping() {
    int a = 10, b = 20, c = 30;
    int result_add, result_sub, result_mul;
    
    printf("演示: 复杂操作数映射\n");
    printf("输入: a=%d, b=%d, c=%d\n", a, b, c);
    
    __asm__(
        "// 第一组计算\n\t"
        "add %0, %3, %4\n\t"   // result_add = a + b  (%0 = %3 + %4)
        "sub %1, %3, %5\n\t"   // result_sub = a - c  (%1 = %3 - %5)
        "// 简单乘法演示 (通过移位)\n\t"
        "lsl %2, %4, #1"       // result_mul = b * 2  (%2 = %4 << 1)
        
        : "=r"(result_add),    // %0 = result_add
          "=r"(result_sub),    // %1 = result_sub
          "=r"(result_mul)     // %2 = result_mul
        : "r"(a),              // %3 = a
          "r"(b),              // %4 = b  
          "r"(c)               // %5 = c
    );
    
    printf("结果: add=%d, sub=%d, mul=%d\n", 
           result_add, result_sub, result_mul);
}

int main() {
    printf("=== LLVM内联汇编操作数占位符演示 ===\n\n");
    
    // 1. 单个输出
    printf("1. 单个输出操作数:\n");
    int result1 = demo_single_output();
    printf("结果: %d\n\n", result1);
    
    // 2. 单个输入  
    printf("2. 单个输入操作数:\n");
    demo_single_input(42);
    printf("\n");
    
    // 3. 多个操作数
    printf("3. 多个操作数:\n");
    demo_multiple_operands(100, 50, 30);
    printf("\n");
    
    // 4. 输入输出混合
    printf("4. 输入输出混合:\n");
    demo_inout_operands(5);
    printf("\n");
    
    // 5. 复杂映射
    printf("5. 复杂操作数映射:\n");
    demo_complex_mapping();
    
    return 0;
}

/*
占位符规则总结:
- %0, %1, %2... 按照在约束列表中的顺序编号
- 先数输出约束，再数输入约束
- 例如: "=r"(out1), "=r"(out2) : "r"(in1), "r"(in2)
  %0=out1, %1=out2, %2=in1, %3=in2

编译测试:
clang -o operands operands.c
./operands
*/