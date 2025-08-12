# LLVM IR 语法详解

深入理解 LLVM IR 的语法结构和组成部分。

## LLVM IR 文件结构

一个完整的 `.ll` 文件包含以下几个主要部分：

```llvm
; 1. 模块信息
; ModuleID = 'simple.c'
source_filename = "simple.c"
target datalayout = "..."
target triple = "..."

; 2. 全局变量声明
@global_var = global i32 42, align 4

; 3. 函数声明
declare i32 @printf(ptr, ...)

; 4. 函数定义
define i32 @main() {
  ; 函数体
}

; 5. 元数据和属性
attributes #0 = { ... }
!llvm.module.flags = !{...}
```

## 标识符和命名

### 1. 全局标识符 (以 `@` 开头)
```llvm
@global_var     ; 全局变量
@main          ; 函数名
@.str          ; 匿名全局变量
```

### 2. 局部标识符 (以 `%` 开头)
```llvm
%1             ; 临时变量（数字）
%result        ; 命名变量
%"my var"      ; 包含特殊字符的名称
```

### 3. 命名规则
- **有名称**: `%result`, `@main`
- **匿名**: `%1`, `%2`, `@.str`
- **特殊字符**: 用双引号包围 `%"complex name"`

## 基本类型系统

### 整数类型
```llvm
i1      ; 1位（布尔值）
i8      ; 8位（char）
i16     ; 16位
i32     ; 32位（int）
i64     ; 64位（long）
i128    ; 128位
```

### 浮点类型
```llvm
half     ; 16位浮点
float    ; 32位浮点
double   ; 64位浮点
```

### 指针类型
```llvm
ptr         ; 通用指针（新版本）
i32*        ; 指向i32的指针（旧版本）
```

### 聚合类型
```llvm
[10 x i32]              ; 数组：10个i32
{i32, float, i8}        ; 结构体：包含不同类型
<4 x i32>               ; 向量：4个i32的向量
```

## 常用指令分类

### 1. 内存操作指令
```llvm
%ptr = alloca i32                    ; 分配栈内存
store i32 42, ptr %ptr               ; 存储值到内存
%val = load i32, ptr %ptr            ; 从内存加载值
%gep = getelementptr i32, ptr %ptr, i64 1  ; 计算地址
```

### 2. 算术运算指令
```llvm
%add = add i32 %a, %b               ; 加法
%sub = sub i32 %a, %b               ; 减法
%mul = mul i32 %a, %b               ; 乘法
%div = sdiv i32 %a, %b              ; 有符号除法
%rem = srem i32 %a, %b              ; 有符号取余
```

### 3. 比较指令
```llvm
%cmp1 = icmp eq i32 %a, %b          ; 整数相等比较
%cmp2 = icmp slt i32 %a, %b         ; 有符号小于比较
%cmp3 = icmp ugt i32 %a, %b         ; 无符号大于比较
%fcmp = fcmp olt float %x, %y       ; 浮点小于比较

; 示例：常量比较和类型转换
%const_cmp = icmp eq i32 0, 0        ; 比较两个常量，结果为 i1 true
%extended = zext i1 %const_cmp to i32 ; 将 i1 零扩展为 i32 (true变为1, false变为0)
```

### 4. 控制流指令
```llvm
br label %next                       ; 无条件跳转
br i1 %cond, label %true, label %false  ; 条件跳转
ret i32 %value                       ; 返回值
ret void                             ; 无返回值
```

### 5. 类型转换指令
```llvm
; 整数类型转换
%trunc = trunc i32 %val to i16       ; 截断（高位丢弃）
%zext = zext i16 %val to i32         ; 零扩展（高位补0）
%sext = sext i16 %val to i32         ; 符号扩展（高位补符号位）

; 浮点类型转换
%fptrunc = fptrunc double %val to float    ; 浮点截断
%fpext = fpext float %val to double        ; 浮点扩展

; 整数和浮点间转换
%sitofp = sitofp i32 %val to float         ; 有符号整数转浮点
%uitofp = uitofp i32 %val to float         ; 无符号整数转浮点
%fptosi = fptosi float %val to i32         ; 浮点转有符号整数
```

### 6. 函数调用
```llvm
%result = call i32 @func(i32 %arg1, i32 %arg2)    ; 函数调用
tail call void @func()                             ; 尾调用
```

## SSA 形式特点

### 1. 单赋值原则
```llvm
; ❌ 错误：变量被重新赋值
%x = add i32 1, 2
%x = add i32 %x, 3      ; 错误！%x 已被定义

; ✅ 正确：使用不同的变量名
%x = add i32 1, 2
%y = add i32 %x, 3      ; 正确
```

### 2. PHI 节点处理控制流合并
```llvm
entry:
  br i1 %cond, label %true, label %false

true:
  %val1 = add i32 1, 2
  br label %merge

false:
  %val2 = add i32 3, 4
  br label %merge

merge:
  ; PHI 节点选择来自不同分支的值
  %result = phi i32 [ %val1, %true ], [ %val2, %false ]
```

## 属性和元数据

### 指令属性
```llvm
%1 = add nsw nuw i32 %a, %b         ; nsw: 有符号不溢出, nuw: 无符号不溢出
%2 = load volatile i32, ptr %ptr     ; volatile: 不可优化的内存访问
%3 = call fastcc i32 @func()         ; fastcc: 快速调用约定
```

### 函数属性
```llvm
define noinline i32 @func() nounwind {  ; noinline: 不内联, nounwind: 不抛异常
  ret i32 0
}

; 或者使用属性组引用
define i32 @func() #0 {
  ret i32 0
}
attributes #0 = { noinline nounwind }
```

这就是 LLVM IR 的基本语法框架，接下来我们可以通过具体例子来深入理解这些概念！