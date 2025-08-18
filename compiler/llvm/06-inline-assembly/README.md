# LLVM内联汇编教学

> **平台**: macOS ARM64 (Apple Silicon)  
> **编译器**: clang  
> **目标**: 学习LLVM内联汇编的语法、操作数占位符和约束系统

## 教学文件结构

```
06-inline-assembly/
├── 01-basic.c              # 基础内联汇编语法
├── 02-operands.c           # 操作数占位符 ($0, $1, $2...)
├── 03-arm64-specific.c     # ARM64特定指令和特性
├── 04-constraints.c        # 约束类型详解
├── build.sh                # 编译和测试脚本
└── README.md               # 本文件
```

## 快速开始

### 编译所有示例
```bash
./build.sh build
```

### 运行所有测试
```bash
./build.sh test
```

### 生成LLVM IR文件
```bash
./build.sh llvm-ir
```

### 生成汇编文件
```bash
./build.sh asm
```

## 教学内容概览

### 1. 基础语法 (01-basic.c)
- 最简单的内联汇编
- 输入输出操作数
- 基本的mov和add指令

**核心概念**:
```c
__asm__("instruction template"
        : "output constraints"(output_vars)
        : "input constraints"(input_vars)
        : "clobbered registers"
);
```

**关键理解 - 操作数占位符机制**:

操作数占位符（如`%0`, `%1`, `%2`）**不是寄存器名**，而是编译器的占位符：

```c
__asm__("mov %0, #42" 
        : "=r"(result)    // %0 = 操作数占位符
);
```

**编译器转换过程**:
1. **你写的**: `mov %0, #42`
2. **编译器分配寄存器**: 选择一个实际寄存器（如`x8`）
3. **生成的汇编**: `mov x8, #42`
4. **存储结果**: `str w8, [sp, #12]` → 写入`result`变量

**约束语法详解**:

约束的完整语法是：`"约束字符串"(C变量名)`

```c
// 基本约束语法示例
"=r"(result)    // 输出到通用寄存器
"r"(input)      // 从通用寄存器读取  
"=m"(memory)    // 输出到内存
"i"(42)         // 立即数常量
"+r"(var)       // 既输入又输出
```

**约束字符串组成**:
- `=` - 输出约束（write-only）
- `+` - 输入输出约束（read-write）  
- `r` - 通用寄存器
- `m` - 内存位置
- `i` - 立即数

**__asm__语法结构**:
```c
__asm__("汇编指令" : 输出约束 : 输入约束 : 破坏列表);
```

冒号后面的都是**可选的**：
```c
// 最简单形式
__asm__("nop");

// 只有输出约束
__asm__("mov %0, #42" : "=r"(result));

// 完整形式
__asm__("add %0, %1, %2" : "=r"(result) : "r"(a), "r"(b) : "memory");
```

**核心**: 占位符让你在汇编中引用C变量，编译器负责寄存器分配和数据传递！

**重要澄清 - 占位符不是汇编语言特性**:

占位符语法（`%0`, `%1`, `%2`）**不是**ARM64汇编语言本身支持的，而是**编译器的内联汇编扩展**：

```bash
# ❌ 在纯汇编文件(.s)中这样写会报错：
mov %0, #42    # error: unknown token in expression

# ✅ 必须写具体的寄存器：
mov x8, #42    # 这才是真正的ARM64汇编
```

```c
// ✅ 只有在C代码的内联汇编中才能使用占位符：
__asm__("mov %0, #42" : "=r"(result));  // 编译器扩展语法
```

**编译器工作流程**:
1. C编译器解析内联汇编语法
2. 将占位符(`%0`)替换为实际寄存器(`x8`)
3. 生成标准ARM64汇编代码
4. 汇编器处理标准汇编代码

**类比理解**: 就像模板系统
- 模板: `Hello, {{name}}!` → 渲染: `Hello, Alice!`
- 内联汇编: `mov %0, #42` → 编译: `mov x8, #42`

### 2. 操作数占位符 (02-operands.c)
- `$0`, `$1`, `$2`...占位符系统
- 单个和多个操作数映射
- 输入输出混合操作数

**核心概念**:
```c
__asm__("add %0, %1, %2"
        : "=r"(result)    // %0 = result
        : "r"(a),         // %1 = a
          "r"(b)          // %2 = b
);
```

### 3. ARM64特定特性 (03-arm64-specific.c)
- 32位/64位寄存器操作
- 条件码和标志位
- 内存寻址模式
- 位操作和移位指令
- 简单原子操作

**核心概念**:
```c
// 强制32位寄存器
__asm__("mov %w0, #42" : "=r"(result));

// 条件选择
__asm__("csel %0, %1, %2, gt" : "=r"(result) : "r"(a), "r"(b) : "cc");
```

### 4. 约束类型 (04-constraints.c)
- 基本约束：`r`, `m`, `i`
- 输出约束：`=r`, `=m`
- 输入输出约束：`+r`, `+m`
- 早期修改：`=&r`
- ARM64特定约束

**约束类型表**:
| 约束 | 含义 |
|------|------|
| `r` | 通用寄存器 |
| `m` | 内存位置 |
| `i` | 立即数 |
| `=` | 输出 |
| `+` | 输入输出 |
| `&` | 早期修改 |

## 实用命令

### 查看生成的内联汇编
```bash
./build.sh check-asm          # 查看汇编文件中的内联汇编
./build.sh check-llvm-ir      # 查看LLVM IR中的内联汇编
```

### 清理文件
```bash
./build.sh clean             # 清理所有生成的文件
```

### 查看帮助
```bash
./build.sh help              # 显示所有可用命令
```

### 单独编译和运行
```bash
clang -o basic 01-basic.c
./basic

clang -S -emit-llvm 01-basic.c -o basic.ll  # 生成LLVM IR
clang -S 01-basic.c -o basic.s               # 生成汇编
```

## 学习路径建议

1. **从01-basic.c开始** - 理解基本语法
2. **学习02-operands.c** - 掌握操作数占位符
3. **探索03-arm64-specific.c** - 了解ARM64特性
4. **深入04-constraints.c** - 掌握约束系统

## 调试技巧

### 查看编译器生成的代码
```bash
# 查看优化前的汇编
clang -S -O0 file.c -o file.s

# 查看优化后的汇编  
clang -S -O2 file.c -o file.s

# 查看LLVM IR
clang -S -emit-llvm file.c -o file.ll
```

### 验证内联汇编是否正确
```bash
# 编译时显示详细信息
clang -v -o program file.c

# 使用objdump查看生成的机器码
objdump -d program
```

## 常见问题

### Q: 为什么我的内联汇编被优化掉了？
A: 使用`__asm__ volatile`防止编译器优化

### Q: 操作数占位符编号规则是什么？
A: 先数所有输出约束，再数所有输入约束，从0开始编号

### Q: 如何确保内存操作的顺序？
A: 使用内存屏障指令如`dmb sy`，并添加`"memory"`到clobber列表

## 相关资源

- [LLVM内联汇编官方文档](https://llvm.org/docs/LangRef.html#inline-assembler-expressions)
- [ARM64指令集参考](https://developer.arm.com/documentation/ddi0596/)
- [GCC内联汇编文档](https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html)