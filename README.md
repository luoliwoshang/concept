# Concept

编程概念学习仓库

## 概念学习跳转树

记录学习过程中的概念跳转路径，展现知识点之间的关联性：

```
volatile关键字概念 ✅ (通过实际例子理解了作用)
├── C语言多线程编程 📝 TODO (只用过pthread，需系统学习)
│   ├── pthread库详细使用 📝 TODO
│   ├── 线程同步机制 📝 TODO
│   ├── 互斥锁和条件变量 📝 TODO
│   └── 线程安全与竞态条件 📝 TODO
│
├── 信号处理机制 🔄 (了解基础，需深入)
│   ├── signal()函数注册 ✅
│   ├── SIGINT信号捕获 ✅  
│   ├── 异步事件处理 📝 TODO
│   ├── 信号处理函数编写 📝 TODO
│   └── 信号屏蔽与处理 📝 TODO
│
├── 编译器优化原理 📝 TODO (只看到了结果，原理需学习)
│   ├── 内存访问优化 📝 TODO
│   ├── 循环优化策略 📝 TODO
│   ├── 代码重排序 📝 TODO
│   ├── 死代码消除 📝 TODO
│   └── 常量折叠 📝 TODO
│
└── LLVM IR分析 🔄 (进行中)
    ├── 第一个IR示例 ✅
    │   ├── 基本语法 ✅
    │   ├── alloca/store/load指令 ✅
    │   └── 函数调用机制 ✅
    │
    ├── 中间表示语法进阶 ✅
    │   ├── SSA形式深入理解 ✅
    │   ├── 类型系统详解 ✅
    │   └── 指令分类与用法 ✅
    │
    ├── 控制流概念 ✅
    │   ├── 基本块结构 ✅
    │   ├── 跳转指令详解 ✅
    │   ├── PHI节点概念 ✅
    │   ├── 循环结构 ✅
    │   └── SSA在循环中的工作原理 ✅
    │
    ├── 函数调用机制 🔄 (进行中)
    │   ├── 参数属性详解 ✅
    │   ├── 数值扩展机制 ✅
    │   ├── 调用约定 📝 TODO
    │   └── 函数属性系统 📝 TODO
    │
    ├── 指针操作 📝 TODO
    │   ├── getelementptr指令详解 📝 TODO
    │   ├── 内存地址计算 📝 TODO
    │   └── 数组和结构体访问 📝 TODO
    │
    ├── 尾调用优化 📝 TODO (准备学习)
    │   ├── 尾调用基本概念 📝 TODO
    │   ├── 栈帧复用原理 📝 TODO
    │   └── 递归优化策略 📝 TODO
    │
    ├── 优化pass分析 📝 TODO
    │   ├── volatile示例中的优化现象分析 🔄 TODO
    │   ├── 循环不变量提取 📝 TODO
    │   └── 控制流图优化 📝 TODO
    │
    └── TinyGo与LLVM集成 📝 TODO
```

### 学习路径说明
1. **起点**: volatile关键字的作用机制
2. **实践验证**: 通过多线程和信号处理对比测试
3. **深入分析**: 生成LLVM IR查看编译器实际优化
4. **核心发现**: 编译器将内存检查移动到"更高效"位置的优化策略
5. **关键洞察**: volatile阻止编译器重新调度内存访问操作

## 目录索引

### 编译器相关概念

#### 内存管理
- [`compiler/memory/`](compiler/memory/) - 内存访问和编译器优化相关概念
  - **volatile关键字** - 防止编译器优化的内存访问控制
    - 多线程和信号处理对比实验
    - LLVM IR 差异分析
    - 编译器优化策略深度解析

#### LLVM 工具链
- [`compiler/llvm/`](compiler/llvm/) - LLVM 编译器基础设施学习
  - **LLVM IR 学习路径**
    - [`01-first-ir/`](compiler/llvm/01-first-ir/) - 第一个 LLVM IR 示例
      - 简单 add 函数的 C 代码到 IR 转换
      - alloca, store, load 指令详解
      - 函数调用机制分析
    - [`02-ir-syntax/`](compiler/llvm/02-ir-syntax/) - LLVM IR 语法详解
      - 类型系统和标识符命名
      - 指令分类和SSA形式特点
      - 属性和元数据系统
    - [`03-control-flow/`](compiler/llvm/03-control-flow/) - 控制流概念
      - 基本块结构和跳转指令
      - PHI节点和控制流合并
      - 循环结构和SSA在循环中的工作原理
    - [`04-functions/`](compiler/llvm/04-functions/) - 函数调用机制详解
      - 参数属性和数值扩展机制
      - 调用约定和函数属性系统
    - [`05-getelementptr/`](compiler/llvm/05-getelementptr/) - 指针操作详解
      - getelementptr指令和内存地址计算
      - 数组和结构体访问模式

### 计算机基础原理
- [`computer-fundamentals/`](computer-fundamentals/) - 计算机系统基础知识
  - CPU内部结构和工作原理
  - 指令执行的四个阶段详解 (取指-译码-执行-回写)
  - 二进制基础和数据表示
  - 为学习汇编、编译器等提供理论基础

### 汇编语言学习
- [`assembly/`](assembly/) - 汇编语言完整学习路径
  - **汇编基础概念**
    - [`01-basics/`](assembly/01-basics/) - 汇编语法和第一个程序
      - 汇编语言基本语法和程序结构
      - x86-64寄存器系统详解
      - 内存模型和寻址模式
      - Hello World和简单计算示例
  - **指令集详解** (待完成)
    - 数据传输、算术运算、逻辑运算指令
    - 控制流和字符串操作指令  
    - 系统调用和处理器控制指令
