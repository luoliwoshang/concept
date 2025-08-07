# getelementptr 指令详解

`getelementptr`（简写为 `gep`）是 LLVM IR 中最重要但也最容易混淆的指令。它的作用是**计算地址**，而不是加载数据。

## 基本概念

### 什么是 getelementptr？
- **GEP** = **G**et **E**lement **P**ointer
- 作用：根据基址和索引计算新的地址
- **重要**：它只计算地址，不访问内存！

### 基本语法
```llvm
%ptr = getelementptr <type>, ptr <base>, <indices>...
```

## 最简单的例子

### 数组访问
```c
// C 代码
int arr[10];
int *p = &arr[3];  // 获取第3个元素的地址
```

对应的 LLVM IR：
```llvm
%arr = alloca [10 x i32]                          ; 分配数组
%ptr = getelementptr [10 x i32], ptr %arr, i64 0, i64 3   ; 计算 arr[3] 的地址
```

### 理解索引参数
```llvm
getelementptr [10 x i32], ptr %arr, i64 0, i64 3
              ^^^^^^^^^^^      ^^^^    ^^^^  ^^^^
              类型信息         基址    索引1 索引2
```

- **第一个索引 `i64 0`**：从基址开始的偏移（通常是0）
- **第二个索引 `i64 3`**：数组内的第3个元素

## 实际例子演示

让我们创建一个实际的 C 代码来看看：

```c
int main() {
    int arr[5] = {10, 20, 30, 40, 50};
    int *p1 = &arr[0];  // 第0个元素
    int *p2 = &arr[2];  // 第2个元素
    return *p2;
}
```

## 为什么需要两个索引？

这是最容易困惑的地方：

### 内存布局视角
```
数组变量 arr 本身是一个指针
  ↓
  +---+---+---+---+---+
  |10 |20 |30 |40 |50 |  ← 实际的数组数据
  +---+---+---+---+---+
   ^           ^
   |           |
arr[0]      arr[2]
```

### 两个索引的含义
1. **第一个索引 `0`**：相当于 `*arr`，解引用到数组数据的开始
2. **第二个索引 `3`**：在数组数据中向前移动3个位置

### 等价理解
```llvm
getelementptr [10 x i32], ptr %arr, i64 0, i64 3
```
相当于 C 语言中的：
```c
&((*arr)[3])  // 或者简写为 &arr[3]
```

## 结构体访问

```c
struct Point {
    int x;      // 字段0
    int y;      // 字段1
};

struct Point p;
int *px = &p.x;  // 访问字段0
int *py = &p.y;  // 访问字段1
```

LLVM IR：
```llvm
%Point = type { i32, i32 }
%p = alloca %Point
%px = getelementptr %Point, ptr %p, i64 0, i32 0    ; 访问字段x
%py = getelementptr %Point, ptr %p, i64 0, i32 1    ; 访问字段y
```

## 指针算术

```c
int *ptr = ...;
int *next = ptr + 1;  // 指向下一个int
```

LLVM IR：
```llvm
%next = getelementptr i32, ptr %ptr, i64 1    ; 向前移动1个i32
```

这里只有一个索引，因为 `%ptr` 已经是指针了！

## 关键要点

1. **只计算地址**：`getelementptr` 不读取内存，只算出地址
2. **类型安全**：编译时就知道结果指针的类型
3. **索引数量**：取决于要访问的数据结构层次
4. **第一个索引**：通常是0，表示"解引用基址"
5. **后续索引**：在解引用后的数据结构中导航

## 常见错误理解

❌ **错误**：认为 `getelementptr` 会加载数据
✅ **正确**：它只计算地址，要用 `load` 才能读取数据

❌ **错误**：不理解为什么需要第一个索引0
✅ **正确**：第一个0表示从基址开始，不跳过任何元素

这个指令理解了，很多复杂的 LLVM IR 代码就清晰了！