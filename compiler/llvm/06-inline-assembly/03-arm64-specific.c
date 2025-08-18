// ARM64特定内联汇编演示
// 展示ARM64架构特有的指令和特性

#include <stdio.h>
#include <stdint.h>

// 1. ARM64寄存器类型演示
void demo_register_types() {
    uint32_t w_result;  // 32位结果
    uint64_t x_result;  // 64位结果
    
    printf("演示: ARM64寄存器类型\n");
    
    // 32位寄存器操作 (w寄存器)
    __asm__("mov %w0, #42"        // 使用%w0强制32位
            : "=r"(w_result)
    );
    
    // 64位寄存器操作 (x寄存器)  
    __asm__("mov %0, #0x1234"       // ARM64立即数限制，使用小值
            : "=r"(x_result)
    );
    
    printf("32位结果: %u\n", w_result);
    printf("64位结果: %lu\n", x_result);
}

// 2. ARM64条件码和标志位
int demo_condition_flags(int a, int b) {
    int result;
    
    printf("演示: ARM64条件码\n");
    printf("比较 %d 和 %d\n", a, b);
    
    __asm__(
        "cmp %1, %2\n\t"          // 比较a和b，设置标志位
        "csel %0, %1, %2, gt"     // 如果a>b则选择a，否则选择b
        : "=r"(result)
        : "r"(a), "r"(b)
        : "cc"                    // 说明修改了条件码
    );
    
    printf("较大值: %d\n", result);
    return result;
}

// 3. ARM64内存操作
void demo_memory_operations() {
    uint64_t data[] = {10, 20, 30, 40, 50};
    uint64_t loaded_value;
    uint64_t sum = 0;
    
    printf("演示: ARM64内存操作\n");
    
    // 加载数组第3个元素 (索引2)
    __asm__("ldr %0, [%1, #16]"   // 加载base+16字节处的值(8*2=16)
            : "=r"(loaded_value)
            : "r"(data)
    );
    
    printf("数组[2] = %lu\n", loaded_value);
    
    // 使用寻址模式计算数组前3个元素的和
    __asm__(
        "mov %0, #0\n\t"          // sum = 0
        "ldr x1, [%1]\n\t"        // 加载data[0]
        "add %0, %0, x1\n\t"      // sum += data[0]
        "ldr x1, [%1, #8]\n\t"    // 加载data[1] 
        "add %0, %0, x1\n\t"      // sum += data[1]
        "ldr x1, [%1, #16]\n\t"   // 加载data[2]
        "add %0, %0, x1"          // sum += data[2]
        : "=r"(sum)
        : "r"(data)
        : "x1"                    // x1被修改
    );
    
    printf("前3个元素和: %lu\n", sum);
}

// 4. ARM64位操作
uint64_t demo_bit_operations(uint64_t value) {
    uint64_t result;
    
    printf("演示: ARM64位操作\n");
    printf("原始值: 0x%lx\n", value);
    
    __asm__(
        "// 位操作序列\n\t"
        "orr %0, %1, #(1 << 3)\n\t"   // 设置第3位
        "eor %0, %0, #(1 << 7)\n\t"   // 翻转第7位  
        "and %0, %0, #0xFFFFFFFE"     // 清除第0位
        : "=r"(result)
        : "r"(value)
    );
    
    printf("处理后: 0x%lx\n", result);
    return result;
}

// 5. ARM64移位操作
void demo_shift_operations() {
    uint32_t value = 0x12345678;
    uint32_t left_shift, right_shift, rotate;
    
    printf("演示: ARM64移位操作\n");
    printf("原始值: 0x%x\n", value);
    
    __asm__(
        "lsl %0, %3, #4\n\t"      // 逻辑左移4位
        "lsr %1, %3, #4\n\t"      // 逻辑右移4位
        "ror %2, %3, #8"          // 循环右移8位
        : "=r"(left_shift),
          "=r"(right_shift), 
          "=r"(rotate)
        : "r"(value)
    );
    
    printf("左移4位: 0x%x\n", left_shift);
    printf("右移4位: 0x%x\n", right_shift);
    printf("循环右移8位: 0x%x\n", rotate);
}

// 6. 简单的原子操作演示
int demo_atomic_operations() {
    int counter = 0;
    int old_value;
    
    printf("演示: ARM64原子操作\n");
    
    // 原子加法
    __asm__(
        "mov w1, #5\n\t"           // 要加的值
        "ldadd w1, %w0, [%1]"      // 原子加法并返回旧值
        : "=r"(old_value)
        : "r"(&counter)
        : "w1", "memory"
    );
    
    printf("原子加法前: %d, 后: %d\n", old_value, counter);
    return counter;
}

int main() {
    printf("=== ARM64特定内联汇编演示 ===\n\n");
    
    // 1. 寄存器类型
    printf("1. 寄存器类型:\n");
    demo_register_types();
    printf("\n");
    
    // 2. 条件码
    printf("2. 条件码操作:\n");
    demo_condition_flags(100, 50);
    printf("\n");
    
    // 3. 内存操作
    printf("3. 内存操作:\n");
    demo_memory_operations();
    printf("\n");
    
    // 4. 位操作
    printf("4. 位操作:\n");
    demo_bit_operations(0x12345678);
    printf("\n");
    
    // 5. 移位操作
    printf("5. 移位操作:\n");
    demo_shift_operations();
    printf("\n");
    
    // 6. 原子操作
    printf("6. 原子操作:\n");
    demo_atomic_operations();
    
    return 0;
}

/*
ARM64特性说明:
- %w0 强制使用32位w寄存器
- %0 默认使用64位x寄存器  
- ARM64有丰富的寻址模式: [base], [base, #offset], [base, index]
- 条件选择指令: csel, csinc, csinv等
- 原子指令: ldadd, ldclr, ldeor等

编译运行:
clang -o arm64 arm64-specific.c
./arm64
*/