# 汇编指令集详解

## 数据传输指令

### MOV - 数据移动
```nasm
mov dest, src       ; dest = src

; 示例
mov rax, rbx        ; 寄存器到寄存器
mov rax, 42         ; 立即数到寄存器
mov rax, [rbx]      ; 内存到寄存器
mov [rax], rbx      ; 寄存器到内存
```

### LEA - 加载有效地址
```nasm
lea rax, [rbx + rcx*2 + 8]  ; rax = rbx + rcx*2 + 8 (地址计算)
; 不访问内存，只计算地址
```

### PUSH/POP - 栈操作
```nasm
push rax            ; 将rax压入栈
pop rbx             ; 从栈弹出到rbx

; 多寄存器操作
pushad              ; 压入所有通用寄存器
popad               ; 弹出所有通用寄存器
```

## 算术运算指令

### 基本算术
```nasm
add rax, rbx        ; rax = rax + rbx
sub rax, rbx        ; rax = rax - rbx  
mul rbx             ; rax = rax * rbx (无符号)
imul rax, rbx       ; rax = rax * rbx (有符号)
div rbx             ; rax = rax / rbx, rdx = rax % rbx
```

### 自增自减
```nasm
inc rax             ; rax++
dec rax             ; rax--
```

### 带标志的算术
```nasm
adc rax, rbx        ; rax = rax + rbx + CF (带进位加法)
sbb rax, rbx        ; rax = rax - rbx - CF (带借位减法)
```

## 逻辑运算指令

### 位运算
```nasm
and rax, rbx        ; rax = rax & rbx (按位与)
or  rax, rbx        ; rax = rax | rbx (按位或)
xor rax, rbx        ; rax = rax ^ rbx (按位异或)
not rax             ; rax = ~rax (按位非)
```

### 移位运算
```nasm
shl rax, 1          ; 逻辑左移1位
shr rax, 1          ; 逻辑右移1位
sal rax, 1          ; 算术左移1位
sar rax, 1          ; 算术右移1位
rol rax, 1          ; 循环左移1位
ror rax, 1          ; 循环右移1位
```

## 比较和测试指令

### CMP - 比较
```nasm
cmp rax, rbx        ; 比较rax和rbx (计算rax-rbx但不保存结果)
; 设置标志位用于条件跳转
```

### TEST - 测试
```nasm
test rax, rax       ; 测试rax是否为0
test rax, 1         ; 测试最低位是否为1
```

## 控制流指令

### 无条件跳转
```nasm
jmp label           ; 跳转到label
call function       ; 调用函数
ret                 ; 返回
```

### 条件跳转
```nasm
; 基于标志位的跳转
jz  label           ; 零标志位置位时跳转 (ZF=1)
jnz label           ; 零标志位清零时跳转 (ZF=0)
jc  label           ; 进位标志位置位时跳转 (CF=1)
jnc label           ; 进位标志位清零时跳转 (CF=0)

; 有符号比较跳转
je  label           ; 等于 (ZF=1)
jne label           ; 不等于 (ZF=0)
jg  label           ; 大于 (有符号)
jl  label           ; 小于 (有符号)
jge label           ; 大于等于 (有符号)
jle label           ; 小于等于 (有符号)

; 无符号比较跳转
ja  label           ; 高于 (无符号)
jb  label           ; 低于 (无符号)
jae label           ; 高于等于 (无符号)
jbe label           ; 低于等于 (无符号)
```

## 字符串操作指令

### 字符串移动
```nasm
movsb               ; 移动字节 [rdi] = [rsi], rsi++, rdi++
movsw               ; 移动字 (16位)
movsd               ; 移动双字 (32位)
movsq               ; 移动四字 (64位)

; 重复前缀
rep movsb           ; 重复rcx次移动字节
```

### 字符串比较
```nasm
cmpsb               ; 比较字节
rep cmpsb           ; 重复比较直到不相等或rcx=0
```

## 系统指令

### 系统调用
```nasm
syscall             ; 64位系统调用
int 0x80            ; 32位系统调用 (兼容模式)
```

### 处理器控制
```nasm
nop                 ; 空操作
hlt                 ; 停机
cli                 ; 禁用中断
sti                 ; 启用中断
```

## 实践示例

### 简单循环
```nasm
mov rcx, 10         ; 循环计数器
loop_start:
    ; 循环体
    dec rcx         ; 计数器减1
    jnz loop_start  ; 如果不为0继续循环
```

### 函数调用示例
```nasm
; 调用函数
push rdi            ; 保存寄存器
push rsi
call my_function    ; 调用函数
pop rsi             ; 恢复寄存器
pop rdi

my_function:
    ; 函数体
    ret             ; 返回
```