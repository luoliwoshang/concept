# 内存模型和寻址模式

## 内存布局

### 典型程序内存布局
```
高地址
+----------------+
|   内核空间      |
+----------------+
|   栈 (Stack)   | ← RSP 栈指针
|      ↓         |
+----------------+
|   未使用区域    |
+----------------+
|      ↑         |
|   堆 (Heap)    |
+----------------+
| BSS段(未初始化) |
+----------------+
| 数据段(已初始化) |
+----------------+
|   代码段(TEXT)  |
+----------------+
低地址
```

## 寻址模式

### 1. 立即寻址
```nasm
mov rax, 42        ; 将立即数42加载到rax
```

### 2. 寄存器寻址
```nasm
mov rax, rbx       ; 将rbx的值复制到rax
```

### 3. 直接寻址
```nasm
mov rax, [0x1000]  ; 从地址0x1000读取数据到rax
```

### 4. 间接寻址
```nasm
mov rax, [rbx]     ; 从rbx指向的地址读取数据
```

### 5. 基址+偏移寻址
```nasm
mov rax, [rbx+8]   ; 从(rbx+8)地址读取数据
```

### 6. 变址寻址 (SIB: Scale-Index-Base)
```nasm
mov rax, [rbx + rcx*2 + 8]
; 地址 = rbx + rcx*2 + 8
; Scale: 1,2,4,8 (乘数)
; Index: 索引寄存器
; Base: 基址寄存器
; Displacement: 偏移量
```

## 实际示例

### 数组访问
```nasm
section .data
    array dd 10, 20, 30, 40, 50  ; 32位整数数组

section .text
    mov rbx, array      ; 数组基址
    mov rcx, 2          ; 索引
    mov eax, [rbx + rcx*4]  ; array[2] (每个元素4字节)
```

### 结构体访问
```c
// C结构体
struct Point {
    int x;    // 偏移0
    int y;    // 偏移4
};
```

```nasm
; 汇编访问结构体
mov rbx, point_addr     ; 结构体地址
mov eax, [rbx + 0]      ; 访问x成员
mov edx, [rbx + 4]      ; 访问y成员
```

### 栈操作
```nasm
push rax        ; 等价于: sub rsp, 8; mov [rsp], rax
pop rbx         ; 等价于: mov rbx, [rsp]; add rsp, 8

; 手动栈操作
sub rsp, 8      ; 分配8字节栈空间
mov [rsp], rax  ; 保存rax到栈
mov rax, [rsp]  ; 从栈恢复rax
add rsp, 8      ; 释放栈空间
```