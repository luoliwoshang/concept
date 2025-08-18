// LLVM内联汇编约束类型详解
// 演示各种约束字符的用法和含义

#include <stdio.h>
#include <stdint.h>

// 1. 基本约束类型
void demo_basic_constraints() {
    int value1 = 42;
    int value2 = 100;
    int result;
    
    printf("演示: 基本约束类型\n");
    
    // "r" - 通用寄存器约束
    __asm__("add %0, %1, %2"
            : "=r"(result)        // "=r": 输出到通用寄存器
            : "r"(value1),        // "r": 从通用寄存器读取
              "r"(value2)         // "r": 从通用寄存器读取
    );
    
    printf("通用寄存器约束: %d + %d = %d\n", value1, value2, result);
}

// 2. 立即数约束
void demo_immediate_constraints() {
    int result1, result2, result3;
    int input = 10;
    
    printf("演示: 立即数约束\n");
    
    // "i" - 立即数约束（编译时常量）
    __asm__("add %0, %1, %2"
            : "=r"(result1)
            : "r"(input), "i"(25)  // "i": 立即数25
    );
    
    // "n" - 数值立即数约束  
    __asm__("sub %0, %1, %2"
            : "=r"(result2)
            : "r"(input), "n"(5)   // "n": 数值立即数5
    );
    
    // "I" - ARM64特定立即数约束（0-4095）
    __asm__("mov %0, %1"
            : "=r"(result3)
            : "I"(123)             // "I": ARM64立即数123
    );
    
    printf("立即数约束: %d + 25 = %d\n", input, result1);
    printf("数值约束: %d - 5 = %d\n", input, result2);
    printf("ARM64立即数: mov = %d\n", result3);
}

// 3. 内存约束
void demo_memory_constraints() {
    int array[] = {10, 20, 30, 40};
    int loaded_value;
    int new_value = 99;
    
    printf("演示: 内存约束\n");
    
    // "m" - 内存约束，允许任何内存位置
    __asm__("ldr %0, %1"
            : "=r"(loaded_value)   // 输出到寄存器
            : "m"(array[2])        // "m": 从内存读取array[2]
    );
    
    printf("从内存加载: array[2] = %d\n", loaded_value);
    
    // "m" - 内存约束，写入内存
    __asm__("str %1, %0"
            : "=m"(array[1])       // "=m": 写入内存array[1]
            : "r"(new_value)       // 从寄存器读取
    );
    
    printf("写入内存后: array[1] = %d\n", array[1]);
}

// 4. 输入输出约束
void demo_inout_constraints() {
    int value = 50;
    int counter = 0;
    
    printf("演示: 输入输出约束\n");
    printf("原始值: value=%d, counter=%d\n", value, counter);
    
    // "+" - 既是输入又是输出
    __asm__("add %0, %0, #10"      // value = value + 10
            : "+r"(value)          // "+r": value既是输入又是输出
    );
    
    // "&" - 早期修改约束，防止与输入重用同一寄存器
    __asm__("mov %0, %2\n\t"
            "add %1, %1, #1"
            : "=&r"(value),        // "=&r": 早期修改，不与输入共享寄存器
              "+r"(counter)        // "+r": 输入输出
            : "r"(100)
    );
    
    printf("处理后: value=%d, counter=%d\n", value, counter);
}

// 5. ARM64特定约束
void demo_arm64_specific_constraints() {
    uint64_t value1 = 0x123456789ABCDEF0;
    uint64_t value2;
    uint32_t shift_amount = 8;
    
    printf("演示: ARM64特定约束\n");
    
    // "w" - 32位寄存器约束
    uint32_t w_result;
    __asm__("mov %w0, %w1"
            : "=r"(w_result)       // 输出到寄存器
            : "r"((uint32_t)value1) // 输入32位值，%w强制32位
    );
    
    // "x" - 64位寄存器约束
    __asm__("ror %0, %1, %2"       // 循环右移
            : "=r"(value2)         // 64位输出
            : "r"(value1),         // 64位输入
              "r"(shift_amount)    // 移位量
    );
    
    printf("32位操作: 0x%x\n", w_result);
    printf("64位循环移位: 0x%lx\n", value2);
}

// 7. 副作用和内存屏障
void demo_side_effects() {
    volatile int shared_var = 42;
    int local_copy;
    
    printf("演示: 副作用和内存屏障\n");
    
    // __asm__ volatile - 防止编译器优化
    __asm__ volatile(
        "ldr %0, %1\n\t"
        "dmb sy"                   // 数据内存屏障
        : "=r"(local_copy)         // 输出
        : "m"(shared_var)          // 内存输入
        : "memory"                 // 内存副作用
    );
    
    printf("带内存屏障的加载: %d\n", local_copy);
    
    // 修改共享变量
    __asm__ volatile(
        "mov w1, #100\n\t"
        "str w1, %0\n\t"
        "dmb sy"
        :                          // 无输出
        : "m"(shared_var)          // 内存输入/输出
        : "w1", "memory"           // w1被修改，有内存副作用
    );
    
    printf("修改后的共享变量: %d\n", shared_var);
}

// 8. 约束修饰符总结
void demo_constraint_modifiers() {
    int input = 50;
    int output1, output2;
    
    printf("演示: 约束修饰符总结\n");
    
    __asm__(
        "// 演示各种约束修饰符\n\t"
        "add %0, %2, #10\n\t"      // output1 = input + 10
        "sub %1, %2, #5"           // output2 = input - 5
        
        : "=r"(output1),           // "=": 输出约束
          "=&r"(output2)           // "=&": 早期修改输出约束
        : "r"(input)               // 输入约束
        :                          // 无副作用
    );
    
    printf("约束修饰符结果: %d -> %d, %d\n", input, output1, output2);
}

int main() {
    printf("=== LLVM内联汇编约束类型演示 ===\n\n");
    
    // 1. 基本约束
    printf("1. 基本约束类型:\n");
    demo_basic_constraints();
    printf("\n");
    
    // 2. 立即数约束
    printf("2. 立即数约束:\n");
    demo_immediate_constraints();
    printf("\n");
    
    // 3. 内存约束
    printf("3. 内存约束:\n");
    demo_memory_constraints();
    printf("\n");
    
    // 4. 输入输出约束
    printf("4. 输入输出约束:\n");
    demo_inout_constraints();
    printf("\n");
    
    // 5. ARM64特定约束
    printf("5. ARM64特定约束:\n");
    demo_arm64_specific_constraints();
    printf("\n");
    
    // 6. 副作用和内存屏障
    printf("6. 副作用和内存屏障:\n");
    demo_side_effects();
    printf("\n");
    
    // 7. 约束修饰符
    printf("7. 约束修饰符:\n");
    demo_constraint_modifiers();
    
    return 0;
}

/*
约束类型总结:
输出约束:
- "=r": 输出到通用寄存器
- "=m": 输出到内存
- "=&r": 早期修改，不与输入共享寄存器

输入约束:
- "r": 通用寄存器输入
- "m": 内存输入
- "i": 立即数输入
- "n": 数值立即数
- "I": ARM64立即数(0-4095)

输入输出约束:
- "+r": 既是输入又是输出
- "+m": 内存输入输出

ARM64特定:
- "w": 强制32位寄存器
- "x": 强制64位寄存器

修饰符:
- "=": 输出
- "&": 早期修改
- "+": 输入输出
- "%": 可交换操作数

编译运行:
clang -o constraints constraints.c
./constraints
*/