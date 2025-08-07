# LLVM IR 控制流详解

深入理解 LLVM IR 中的控制流结构，包括基本块、跳转指令和 PHI 节点。

## 基本块 (Basic Block)

基本块是LLVM IR的核心概念，它是一段连续执行的指令序列。

想象你写代码时，有些代码是"一口气执行完"的，比如：
```c
int x = 1;
int y = 2;
int z = x + y;
```

这三行代码会连续执行，中间不会跳到别的地方。这就是一个**基本块**的概念。

### 基本块的特点
- **只有一个入口**：只能从头部进入
- **只有一个出口**：只能从末尾离开  
- **顺序执行**：中间不能跳转

### 基本块示例

```llvm
entry:                    ; 这是基本块的标签（名字）
  %x = add i32 1, 2      ; 普通指令
  %y = add i32 %x, 3     ; 普通指令
  br label %next         ; 跳转指令（基本块的结尾）
```

在这个例子中：
- `entry:` 是基本块的标签
- `%x = add i32 1, 2` 和 `%y = add i32 %x, 3` 是普通指令
- `br label %next` 是跳转指令，标志着基本块的结束

## 跳转指令

既然基本块只能有一个出口，那怎么从一个基本块跳到另一个基本块呢？这就需要**跳转指令**。

### 1. 无条件跳转 (`br label`)

最简单的跳转指令：
```llvm
br label %next
```

这就是"无条件跳转"，意思是"直接跳到 %next 这个基本块"。

### 2. 条件跳转 (`br i1`)

根据条件选择跳转目标：
```llvm
br i1 %condition, label %true_block, label %false_block
```

意思是：如果 %condition 为真，跳到 %true_block；如果为假，跳到 %false_block。

### 跳转指令示例

```llvm
define i32 @jump_example(i32 %x) {
entry:
  %cond = icmp eq i32 %x, 0
  br i1 %cond, label %is_zero, label %not_zero

is_zero:
  ret i32 100

not_zero:
  ret i32 200
}

## PHI 节点

PHI节点是LLVM IR中最重要的概念之一，用于处理控制流汇合点的值选择。

想象这种情况：你有两个分支，每个分支计算出不同的值，最后要合并到一起：

```c
int x;
if (condition) {
    x = 10;
} else {
    x = 20;
}
// 这里的 x 应该是多少？
```

在普通程序里，x的值取决于走了哪个分支。但在SSA形式的LLVM IR中，每个变量只能赋值一次，所以需要PHI节点来"选择"值。

### PHI 节点语法
```llvm
%result = phi <type> [ <value1>, <label1> ], [ <value2>, <label2> ], ...
```

### PHI 节点示例
```llvm
define i32 @phi_example(i1 %condition) {
entry:
  br i1 %condition, label %true_branch, label %false_branch

true_branch:
  br label %merge

false_branch:
  br label %merge

merge:
  ; PHI节点根据来源基本块选择值
  %x = phi i32 [ 10, %true_branch ], [ 20, %false_branch ]
  ret i32 %x
}
```

这个PHI节点的意思是：如果是从%true_branch过来的，%x就是10；如果是从%false_branch过来的，%x就是20。

## 循环结构

循环在LLVM IR中很有意思。我们以一个简单的for循环为例：

```c
int sum = 0;
for (int i = 0; i < n; i++) {
    sum += i;
}
```

这个循环在LLVM IR中会变成这样的结构：

```llvm
define i32 @sum_loop(i32 %n) {
entry:
  br label %loop_header

loop_header:
  %i = phi i32 [ 0, %entry ], [ %next_i, %loop_body ]
  %sum = phi i32 [ 0, %entry ], [ %next_sum, %loop_body ]
  %cond = icmp slt i32 %i, %n
  br i1 %cond, label %loop_body, label %exit

loop_body:
  %next_sum = add i32 %sum, %i      ; sum += i (累加过程)
  %next_i = add i32 %i, 1           ; i++ (递增过程)
  br label %loop_header             ; 跳回循环头

exit:
  ret i32 %sum
}
```

### SSA形式在循环中的工作原理

关键理解：在SSA形式中，**每个变量名确实只能被赋值一次**，这个原则在循环中也不会被违反。

- **静态视角**：`%next_sum` 只被定义一次（在 `add` 指令中）
- **动态执行**：这条指令会被执行多次，每次产生不同的值
- **PHI节点作用**：在每次重新进入循环头时，选择正确的值

```llvm
loop_header:
  %sum = phi i32 [ 0, %entry ], [ %next_sum, %loop_body ]
```

这个PHI节点在说：
- 第1次进入循环：`%sum` = 0（来自entry）  
- 第2次进入循环：`%sum` = 第1次迭代的`%next_sum`值
- 第3次进入循环：`%sum` = 第2次迭代的`%next_sum`值
- ...

### SSA的"单赋值"真正含义

SSA的"单赋值"指的是：在程序的**静态表示**中，每个变量名只能出现在一个赋值指令的左侧。但这个指令可以被**动态执行多次**。

所以 `%next_sum` 在静态IR中只有一个定义点，但运行时会产生多个不同的值，PHI节点负责在控制流汇合点选择正确的值。