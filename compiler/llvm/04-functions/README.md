# LLVM IR 函数调用机制详解

深入理解 LLVM IR 中的函数定义、参数传递和调用约定。

## 函数定义基础

我们已经见过简单的函数定义，但实际的函数定义可能更复杂：

```llvm
; 简单函数
define i32 @simple_func(i32 %x) {
  ret i32 %x
}

; 复杂函数
define i32 @complex_func(i32 noundef %x, i32* %ptr) nounwind {
  ; 函数体
  ret i32 42
}
```

这里的关键概念：
1. **参数属性**：`noundef` 表示参数不能是未定义值
2. **函数属性**：`nounwind` 表示函数不会抛出异常

## 参数属性详解

参数属性帮助编译器进行优化和正确的代码生成。

### 1. 数值扩展属性

当小于32位的整数作为参数传递时，需要扩展到寄存器大小：

```llvm
define i32 @func_signext(i32 signext %x) {    ; 有符号扩展
  ret i32 %x
}

define i32 @func_zeroext(i32 zeroext %x) {    ; 零扩展
  ret i32 %x
}
```

#### 零扩展 (Zero Extension)
用0填充高位，适用于无符号数：

```
原始值: i8 255 (二进制: 11111111)
零扩展到i32: 00000000 00000000 00000000 11111111
结果: i32 255
```

#### 有符号扩展 (Sign Extension)  
用符号位填充高位，适用于有符号数：

```
原始值: i8 -1 (二进制: 11111111)
有符号扩展到i32: 11111111 11111111 11111111 11111111  
结果: i32 -1
```

#### 扩展示例对比

```llvm
define void @extension_example() {
  ; 零扩展示例
  %x1 = trunc i32 255 to i8          ; 255 -> i8 (11111111)
  call void @func_zero(i8 zeroext %x1)  ; 扩展为 i32 255
  
  ; 有符号扩展示例  
  %x2 = trunc i32 -1 to i8           ; -1 -> i8 (11111111)
  call void @func_sign(i8 signext %x2)  ; 扩展为 i32 -1
  
  ret void
}

declare void @func_zero(i8 zeroext)
declare void @func_sign(i8 signext)
```

### 2. 内存相关属性

```llvm
define void @memory_attrs(i32* readonly %ptr1, i32* writeonly %ptr2) {
  %val = load i32, i32* %ptr1      ; 只读指针
  store i32 42, i32* %ptr2         ; 只写指针
  ret void
}
```

- `readonly`：指针指向的内存不会被修改
- `writeonly`：指针只用于写入，不会读取

### 3. 值定义属性

```llvm
define i32 @value_attrs(i32 noundef %x) {
  ret i32 %x
}
```

- `noundef`：参数不能是未定义值，帮助优化器进行更激进的优化