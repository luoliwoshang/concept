# 第一个 LLVM IR 示例

最简单的 C 代码和对应的 LLVM IR。

## 原始 C 代码
```c
int add(int a, int b) {
    return a + b;
}

int main() {
    int result = add(5, 3);
    return result;
}
```

## 生成 LLVM IR
```bash
clang -S -emit-llvm simple.c -o simple.ll
```

## 核心 LLVM IR 解读

### add 函数 (第7-16行)
```llvm
define i32 @add(i32 noundef %0, i32 noundef %1) #0 {
  %3 = alloca i32, align 4      ; 为参数a分配栈空间
  %4 = alloca i32, align 4      ; 为参数b分配栈空间
  store i32 %0, ptr %3, align 4 ; 存储第一个参数
  store i32 %1, ptr %4, align 4 ; 存储第二个参数
  %5 = load i32, ptr %3, align 4 ; 加载第一个参数
  %6 = load i32, ptr %4, align 4 ; 加载第二个参数
  %7 = add nsw i32 %5, %6       ; 执行加法
  ret i32 %7                    ; 返回结果
}
```

**参数属性说明**：
- `noundef`: 表示参数不会是未定义值，调用者保证传入有效的已初始化值
- 这个属性帮助 LLVM 进行更好的优化，因为可以假设参数总是有定义的

### main 函数 (第19-27行)
```llvm
define i32 @main() #0 {
  %1 = alloca i32, align 4         ; 为返回值分配空间
  %2 = alloca i32, align 4         ; 为result变量分配空间
  store i32 0, ptr %1, align 4     ; 初始化返回值为0
  %3 = call i32 @add(i32 noundef 5, i32 noundef 3) ; 调用add(5,3)
  store i32 %3, ptr %2, align 4    ; 存储调用结果到result
  %4 = load i32, ptr %2, align 4   ; 加载result作为返回值
  ret i32 %4                       ; 返回result
}
```

## 关键概念

1. **函数定义**: `define i32 @函数名(...)`
2. **变量名**: `%0`, `%1`, `%2`... 自动编号
3. **内存分配**: `alloca` 在栈上分配空间
4. **内存操作**: `store` 存储，`load` 加载
5. **函数调用**: `call` 指令
6. **类型**: `i32` 表示32位整数
7. **返回**: `ret` 指令
8. **参数属性**: 
   - `noundef`: 参数保证不是未定义值
   - 帮助编译器做出更激进的优化假设

## 为什么这么复杂？

简单的 `return a + b` 变成了这么多指令是因为：
- 编译器没有优化 (`-O0` 默认)
- 保持了 C 语言的内存模型
- 每个操作都明确表示

这样的好处是非常容易理解每一步在做什么！