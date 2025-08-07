# LLVM 概念学习

LLVM (Low Level Virtual Machine) 是一个编译器基础设施项目，提供了模块化、可重用的编译器和工具链技术。

## 核心概念

### 1. LLVM IR (Intermediate Representation)
LLVM 的中间表示是整个框架的核心，它是一种低级的、类似汇编的语言，但保持了平台无关性。

#### 基本特点
- **SSA 形式** (Static Single Assignment): 每个变量只能被赋值一次
- **强类型系统**: 明确的数据类型定义
- **平台无关**: 不依赖于特定的硬件架构
- **人类可读**: 文本形式，便于分析和理解

### 2. 从 volatile 示例学到的概念

在分析 `busy_wait_demo.ll` 时遇到的关键概念：

#### PHI 节点
```llvm
%8 = phi i32 [ %21, %19 ], [ %6, %0 ]
```
- 用于合并来自不同基本块的值
- 格式：`phi <type> [<val1>, <label1>], [<val2>, <label2>]`
- 解决 SSA 形式中的控制流合并问题

#### 基本块 (Basic Block)
```llvm
7:                                    ; 基本块标签
  %8 = phi i32 [ %21, %19 ], [ %6, %0 ]
  %9 = add nsw i32 %8, 1
  br i1 %14, label %15, label %19
```
- 顺序执行的指令序列
- 只有一个入口和一个出口
- 编译器优化的基本单位

#### 内存操作指令
```llvm
%4 = load i32, ptr @flag, align 4        ; 普通加载
%4 = load volatile i32, ptr @flag, align 4   ; volatile加载
store i32 1, ptr @flag, align 4          ; 普通存储
store volatile i32 1, ptr @flag, align 4     ; volatile存储
```

#### 控制流指令
```llvm
br i1 %5, label %7, label %23           ; 条件跳转
br label %19                            ; 无条件跳转
ret i32 0                              ; 返回
```

### 3. 编译器优化在 IR 中的体现

通过对比 volatile 和非 volatile 版本的 IR，我们看到：

#### 循环优化
- **循环不变量提取**: 将不变的计算移出循环
- **死代码消除**: 移除不会被执行的代码
- **控制流优化**: 重新组织基本块的顺序

#### 内存访问优化
- **加载/存储消除**: 移除冗余的内存访问
- **内存访问重排序**: 为了性能而调整访问顺序
- **volatile 语义保护**: 确保 volatile 访问不被优化

## 学习路径

### 基础概念
1. [`01-first-ir/`](01-first-ir/) - 第一个LLVM IR程序和基本概念
2. [`02-ir-syntax/`](02-ir-syntax/) - LLVM IR语法详解
3. [`03-control-flow/`](03-control-flow/) - 控制流：基本块、跳转指令、PHI节点、循环
4. [`04-functions/`](04-functions/) - 函数调用机制：参数属性、调用约定、函数属性
5. [`05-getelementptr/`](05-getelementptr/) - 指针操作和内存地址计算

### 其他资源
- [`ir-basics/`](ir-basics/) - LLVM IR 语法基础和示例